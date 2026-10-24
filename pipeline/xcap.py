"""xcap.py -- a call captured in the running game (xikit capture=, docs/KIT.md), gated: the original and the
reconstruction run from exactly that call, on the memory it could reach.

xikit writes xikit_capture_<pid>_<RVA>_<n>.xcap at the entry of a manifest function: the registers, the modules,
and the pages of memory reachable from the registers and the stack (kit/xikit/xikit.c, capture_c). Here:

  * CaptureState: the capture behind LiveState's interface (live_state.py), so the gate's live mode runs on it
    as on a full dump. A page the capture lacks is unknown to the gate, which fills it as it fills any unknown
    page (the same bytes on both sides).
  * replay: the gate case for the captured function from the cases file, with `replay` set to the call's own
    `this` (ECX) and arguments (the dwords above the return address at the entry ESP, as many as the case's
    declaration has), each translated like any dump value; the live config (the capture's FFXiMain .data);
    and the gate run (diffemu_gfx.py, with coverage), into OUT_DIR.

The capture comes from the analysed build, or from a build the kit translates (docs/KIT.md, build translation):
xikit checks each entry's bytes before writing (xikit.c start, check_bytes). For another build, --build names its
translation (kit/xikit/builds/<stamp>.json; the build is the one xikit.log names at the start, the capture holds no
header page). Its data addresses are the analysed ones; a value pointing into one of its moved code ranges is
moved back by that range's shift, as translate() prepares any dump value. A value in code with no analysed
counterpart (inserted bytes) stays as it is: the same on both sides.

    python3 xcap.py CAPTURE.xcap CASES.json RECON.dll OUT_DIR [--trials 8] [--insn-limit N] [--build BUILD.json]
                                                                                                 (uses $FFXI_DLL)
prints the verdict and the coverage of the original's instructions in the compared trials."""
import argparse, json, os, struct, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import live_state

MAGIC = 0x50414358


class CaptureState(live_state.LiveState):
    def __init__(self, path, build_map=None, module='FFXiMain.dll'):
        b = open(path, 'rb').read()
        head = struct.unpack_from('<16I', b, 0)
        if head[0] != MAGIC or head[1] != 1: raise ValueError('%s: not an xcap v1 file' % path)
        self.entry, self.base, self.size, self.thread, self.frame, nmods, npages = head[2:9]
        o = 64
        self.regs = dict(zip(('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi', 'eflags'), struct.unpack_from('<9I', b, o)))
        o += 36
        mods = []
        for _ in range(nmods):
            base, size = struct.unpack_from('<2I', b, o); name = b[o + 8:o + 72].split(b'\0')[0].decode('latin1'); o += 72
            mods.append((base, size, name))
        addrs = struct.unpack_from('<%dI' % npages, b, o); o += 4 * npages
        self.pages = {a: b[o + 4096 * i:o + 4096 * (i + 1)] for i, a in enumerate(addrs)}
        game = next((m for m in mods if m[2].lower() == module.lower()), None)
        if game and (game[0], game[1]) != (self.base, self.size): raise ValueError('%s: module list and header disagree on %s' % (path, module))
        self.foreign = sorted(m for m in mods if m[2].lower() != module.lower())
        self.rel = self.base - 0x10000000; self._purges = {}
        self.same_build = True; self.f_sep_aug = self.g_sep_aug = self.aug_sep = None
        # runs of consecutive pages, as LiveState's segments (sandbox() looks for a gap among them)
        self.segs = []
        for a in sorted(self.pages):
            if self.segs and self.segs[-1][0] + self.segs[-1][1] == a: self.segs[-1] = (self.segs[-1][0], self.segs[-1][1] + 4096, None)
            else: self.segs.append((a, 4096, None))
        self.starts = [s[0] for s in self.segs]

    def set_build(self, path):
        """a capture of a translated build: [(lo, hi, shift)] in that build's numbering, the shifted ranges only"""
        self.moved = [(int(lo, 16) + d, int(hi, 16) + d, d) for lo, hi, d in json.load(open(path))['ranges'] if d]

    def translate(self, v):
        a = super().translate(v)
        if not (self.base <= v < self.base + self.size): return a        # not into the image: as it is
        for lo, hi, d in getattr(self, 'moved', ()):
            if lo <= a < hi: return a - d
        return a

    def read(self, a, n):
        out = b''
        while n > 0:
            p = self.pages.get(a & ~0xFFF)
            if p is None: return None
            k = min(n, 0x1000 - (a & 0xFFF)); out += p[a & 0xFFF:(a & 0xFFF) + k]; a += k; n -= k
        return out


def replay_case(cs, cases):
    """the gate case of the captured function, with the captured call's `this` and arguments"""
    c = next((x for x in cases if int(x['original'], 16) == cs.entry), None)
    if c is None: raise SystemExit('xcap.py: no gate case for 0x%08X in the cases file' % cs.entry)
    c = dict(c); esp = cs.regs['esp']; n = len(c.get('args', []))
    raw = cs.read(esp + 4, 4 * n) if n else b''
    if raw is None: raise SystemExit('xcap.py: the capture lacks the argument dwords above ESP 0x%08X' % esp)
    args = [cs.translate(v) for v in struct.unpack('<%dI' % n, raw)] if n else []
    c['replay'] = {'this': '0x%08X' % cs.translate(cs.regs['ecx']) if c.get('conv', 'thiscall') == 'thiscall' else None,
                   'args': ['0x%08X' % a for a in args], 'capture_thread': cs.thread, 'capture_frame': cs.frame}
    return c


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('xcap'); ap.add_argument('cases'); ap.add_argument('recon'); ap.add_argument('out')
    ap.add_argument('--trials', type=int, default=8)
    ap.add_argument('--insn-limit', type=int, help="the original's instruction budget a trial (the gate's default: "
                    "300000); a real call can need more (SkinAndDrawV2 skinning a whole model). A trial over it is not compared")
    ap.add_argument('--build', help="the translation of the build the capture was taken on (kit/xikit/builds/<stamp>.json)")
    o = ap.parse_args()
    os.makedirs(o.out, exist_ok=True)
    cs = CaptureState(o.xcap)
    if o.build: cs.set_build(o.build)
    case = replay_case(cs, json.load(open(o.cases)))
    if o.insn_limit: case['insn_limit'] = o.insn_limit
    stem = os.path.join(o.out, os.path.basename(o.xcap).rsplit('.', 1)[0])
    live = {'dump': os.path.abspath(o.xcap), 'build_map': None, 'image_base': '0x%08X' % cs.base, 'sandbox': '0x%08X' % cs.sandbox(),
            'data_writes': live_state.data_writes(cs, {}), 'this_sources': {}, 'instances': {}}
    json.dump(live, open(stem + '.live.json', 'w')); json.dump([case], open(stem + '.case.json', 'w'))
    if os.path.exists(stem + '.gate.jsonl'): os.remove(stem + '.gate.jsonl')
    env = dict(os.environ, FFXI_LIVE=stem + '.live.json', FFXI_COVERAGE='1')
    r = subprocess.run([sys.executable, os.path.join(P, 'diffemu_gfx.py'), os.environ['FFXI_DLL'], o.recon, stem + '.case.json',
                        stem + '.gate.jsonl', '--trials', str(o.trials)], env=env, capture_output=True, text=True)
    if r.returncode or not os.path.exists(stem + '.gate.jsonl'): raise SystemExit('xcap.py: gate failed\n' + r.stdout[-2000:] + r.stderr[-2000:])
    g = json.loads(open(stem + '.gate.jsonl').readline())
    cov = g.get('coverage') or {}
    print('%s 0x%08X: %s; outcomes %s; %d pages captured; coverage %s of %s instructions; this %s args %s' % (
        case['name'], cs.entry, g['verdict'], g.get('original_outcomes'), len(cs.pages), cov.get('covered'), cov.get('insns'),
        case['replay']['this'], case['replay']['args']))
    if g.get('first_mismatch'): print('  first mismatch:', g['first_mismatch'].get('why'))


if __name__ == '__main__':
    main()
