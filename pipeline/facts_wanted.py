"""facts_wanted.py -- the call sites a facts=1 session should measure beyond the committed gate records, each with
why, from the code and the gate records given (data/facts_wanted.json; kit/xikit/build.sh passes it to
fact_sites.py sites --from-file).

  * a call a gate run stopped at because no definitive source gives its pop count (`call 0x...: pop count
    unknown`), from the gate records named on the command line;
  * a virtual call on a fuzzed object whose x87 result is read (`virtual call on a fuzzed object returning to
    0x...`): the call that ends at that return address; the facts give its target, so its float result can be
    modelled;
  * every indirect call in a function a frame runs (data/frame_set.json, game code) whose convention no source
    establishes (ghidra/conv_facts.icalls: its pop count not established, or popping 0 with no proven `this`):
    the measured target is a function whose own facts then apply;
  * every call through the object at [0x1066941C] (Dancer_InitDevice sets it from [[0x103D3CBC]+8]): `mov r,
    [0x1066941C]`, then `mov v, [r]`, then `call [v+N]` in the same block with r unchanged. No source says what
    the object is; the measured target and vtable of these calls do.

    FFXI_DLL=... python3 facts_wanted.py OUT.json [GATE.jsonl ...]"""
import bisect, json, os, re, sys
from capstone.x86 import X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S

DANCER_DEVICE = 0x1066941C


def dancer_sites(im, ext):
    out = {}
    for k, e in ext.items():
        ins = im.code(e); order = sorted(ins)
        for j, a in enumerate(order):
            i = ins[a]; o = i.operands
            if not (i.mnemonic == 'mov' and o[0].type == X86_OP_REG and o[1].type == X86_OP_MEM and not o[1].mem.base
                    and not o[1].mem.index and (o[1].mem.disp & 0xFFFFFFFF) == DANCER_DEVICE): continue
            dev, vt = i.reg_name(o[0].reg), None
            for b in order[j + 1:j + 12]:
                x = ins[b]; xo = x.operands
                if x.mnemonic.startswith(('j', 'ret')): break
                if x.mnemonic == 'mov' and xo[0].type == X86_OP_REG and xo[1].type == X86_OP_MEM and xo[1].mem.base \
                        and x.reg_name(xo[1].mem.base) == dev and xo[1].mem.disp == 0 and not xo[1].mem.index:
                    vt = x.reg_name(xo[0].reg); continue
                if x.mnemonic == 'call' and xo[0].type == X86_OP_MEM and vt and xo[0].mem.base and x.reg_name(xo[0].mem.base) == vt:
                    out['0x%08X' % b] = 'call through the object at [0x%08X] (slot %d), in 0x%s' % (DANCER_DEVICE, xo[0].mem.disp // 4, k[2:].upper())
                    break
                if xo and xo[0].type == X86_OP_REG and x.reg_name(xo[0].reg) == dev and x.mnemonic not in ('push', 'cmp', 'test'): break
    return out


def gate_sites(im, ext, paths):
    out, ret_at = {}, {}
    for p in paths:
        for l in open(p):
            r = json.loads(l)
            for k in (r.get('unresolved') or {}):
                m = re.match(r'call (0x[0-9A-Fa-f]+): pop count unknown', k)
                if m: out.setdefault('0x%08X' % int(m.group(1), 16), 'pop count unknown (gate: %s)' % r['original'])
                m = re.match(r'virtual call on a fuzzed object returning to (0x[0-9A-Fa-f]+)', k)
                if m: ret_at['0x%08X' % int(m.group(1), 16)] = r['original']
    if ret_at:                                                  # the call instruction that ends at each return address
        for k, e in ext.items():
            for a, i in im.code(e).items():
                key = '0x%08X' % (a + i.size)
                if i.mnemonic == 'call' and key in ret_at:
                    out.setdefault('0x%08X' % a, 'x87 result of a virtual call read, target unknown (gate: %s)' % ret_at[key])
    return out


def frame_icalls(ext):
    sys.path.insert(0, os.path.join(P, 'ghidra'))
    import conv_facts
    fs = {int(k, 16) for k, v in json.load(open(os.path.join(P, '..', 'data', 'frame_set.json')))['functions'].items() if not v.get('d3dx')}
    rng = sorted((a, b, int(k, 16)) for k, rs in ext.items() for a, b in rs); starts = [r[0] for r in rng]
    out = {}
    for k, (r, why) in conv_facts.icalls().items():
        if r: continue
        a = int(k, 16); j = bisect.bisect_right(starts, a) - 1
        if j >= 0 and rng[j][0] <= a < rng[j][1] and rng[j][2] in fs:
            out['0x%08X' % a] = 'indirect call in 0x%08X: %s' % (rng[j][2], why)
    return out


def main(out, *gates):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    want = dict(gate_sites(im, ext, gates))
    for k, v in frame_icalls(ext).items(): want.setdefault(k, v)
    for k, v in dancer_sites(im, ext).items(): want.setdefault(k, v)
    json.dump(dict(sorted(want.items())), open(out, 'w'), indent=0)
    kinds = lambda w: sum(w in v for v in want.values())
    print('%d sites: %d from the gate records, %d indirect calls in the frame set, %d through [0x%08X]' % (
        len(want), kinds('(gate:'), kinds('indirect call in'), kinds('[0x%08X]' % DANCER_DEVICE), DANCER_DEVICE))


if __name__ == '__main__':
    main(*sys.argv[1:])
