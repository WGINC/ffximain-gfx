"""live_state.py -- the gate's inputs from a memory dump of the running game, instead of fuzzing.

A Windows minidump of pol.exe in game holds every heap object and the game's globals. The dump may
be of another build than the one IDA analysed (build_map.py maps the two); here every value is
expressed in the analysed build:
  * pages: a demand-paged access to an address the dump holds gets the dump's page, with each dword
    that points into the dump's FFXiMain image translated to the analysed build (vtables, functions,
    globals) when the map knows it;
  * data_writes(): the analysed build's .data, filled from the dump's corresponding globals;
  * instances(vptr): heap objects whose first dword is that class's vtable;
  * this_source(func): how callers pass `this` (mov ecx, [global] / a static object), read from the code;
  * sandbox(): a 16 MB gap the dump does not use, for the gate's own fuzzed buffers.

    python3 live_state.py DUMP.dmp BUILD_MAP.json OUT.json      precompute (data writes, this sources,
                                                                instances for the cases' vtables)
"""
import bisect, json, mmap, os, struct, sys, collections

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
SEP = (0x10000000, 0x10BE2000)
DATA = (0x1034F000, 0x109FA000)


class LiveState:
    def __init__(self, dmp, build_map, module='FFXiMain.dll'):
        from minidump.minidumpfile import MinidumpFile
        m = MinidumpFile.parse(dmp)
        self.f = open(dmp, 'rb'); self.mm = mmap.mmap(self.f.fileno(), 0, access=mmap.ACCESS_READ)
        segs = sorted((s.start_virtual_address, s.size, s.start_file_address) for s in m.memory_segments_64.memory_segments)
        self.starts = [s[0] for s in segs]; self.segs = segs
        mod = next(x for x in m.modules.modules if x.name.lower().endswith(module.lower()))
        self.base, self.size = mod.baseaddress, mod.size
        # every other module (D3D8 wrapper, polcore, Windows): calls into them are answered as stubs
        self.foreign = sorted((x.baseaddress, x.size, x.name.split('\\')[-1]) for x in m.modules.modules if x is not mod)
        self._pops = {}
        self.rel = self.base - 0x10000000
        bm = json.load(open(build_map))
        self.f_sep_aug = {int(k, 16): int(v, 16) for k, v in bm['functions'].items()}
        self.g_sep_aug = {int(k, 16): int(v, 16) for k, v in bm['globals'].items()}
        self.aug_sep = {v: k for k, v in list(self.f_sep_aug.items()) + list(self.g_sep_aug.items())}

    def read(self, a, n):
        i = bisect.bisect_right(self.starts, a) - 1
        if i < 0: return None
        s, size, fo = self.segs[i]
        if a + n > s + size: return None
        return self.mm[fo + a - s:fo + a - s + n]

    def translate(self, v):
        if self.base <= v < self.base + self.size:
            return self.aug_sep.get(v - self.rel, v)
        return v

    def page(self, a):
        """the dump's 4 KB page at a (page-aligned), translated; None if the dump does not hold it"""
        b = self.read(a, 0x1000)
        if b is None: return None
        if self.base <= a < self.base + self.size: return b      # the dump's own image: as it is
        w = list(struct.unpack('<1024I', b))
        lo, hi = self.base, self.base + self.size
        for k, v in enumerate(w):
            if lo <= v < hi: w[k] = self.aug_sep.get(v - self.rel, v)
        return struct.pack('<1024I', *w)

    def value(self, sep_global):
        aug = self.g_sep_aug.get(sep_global)
        if aug is None: return None
        b = self.read(aug + self.rel, 4)
        return None if b is None else self.translate(struct.unpack('<I', b)[0])

    def pops(self, ea):
        """bytes a foreign function pops on return: its first `ret N` in the dump's code"""
        if ea not in self._pops:
            from capstone import Cs, CS_ARCH_X86, CS_MODE_32
            code = self.read(ea, 0x400) or self.read(ea, 0x100) or b''
            n = 0
            for i in Cs(CS_ARCH_X86, CS_MODE_32).disasm(code, ea):
                if i.mnemonic in ('ret', 'retn'):
                    n = int(i.op_str, 0) if i.op_str else 0; break
                if i.mnemonic == 'jmp' and not i.op_str.startswith('0x'): break   # import thunk / tail jump
            self._pops[ea] = n
        return self._pops[ea]

    def sandbox(self, size=0x1000000):
        a = 0x20000000
        for s, n, _ in self.segs:
            if s >= a + size and a >= 0x20000000: break
            if s + n > a: a = (s + n + 0xFFFFF) & ~0xFFFFF
        return a


def data_writes(ls, sizes):
    """(analysed address, bytes) for every mapped .data global; arrays as far as both builds agree"""
    out = []
    for sep, aug in sorted(ls.g_sep_aug.items()):
        if not (DATA[0] <= sep < DATA[1]): continue
        n = max(4, min(sizes.get(sep, 4), 0x2000)) & ~3
        b = ls.read(aug + ls.rel, n)
        if b is None: continue
        w = [ls.translate(x) for x in struct.unpack('<%dI' % (n // 4), b)]
        out.append(['0x%08X' % sep, struct.pack('<%dI' % (n // 4), *w).hex()])
    return out


def this_sources(dll, funcs):
    """{func: ('global', addr) | ('static', addr)} from the instruction that sets ECX before each call"""
    import pefile
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    want = set(funcs); votes = collections.defaultdict(collections.Counter)
    for k, rs in json.load(open(os.path.join(D, 'func_extents.json'))).items():
        for a, b in rs:
            ecx = None
            for i in md.disasm(img[a - base:b - base], a):
                ops = i.operands
                if i.mnemonic in ('mov', 'lea') and ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'ecx':
                    o = ops[1]
                    if i.mnemonic == 'mov' and o.type == X86_OP_MEM and not o.mem.base and not o.mem.index: ecx = ('global', o.mem.disp)
                    elif i.mnemonic == 'mov' and o.type == X86_OP_IMM and SEP[0] <= o.imm < SEP[1]: ecx = ('static', o.imm)
                    elif i.mnemonic == 'lea' and o.type == X86_OP_MEM and not o.mem.base and not o.mem.index: ecx = ('static', o.mem.disp)
                    else: ecx = None
                elif i.mnemonic == 'call':
                    if ops[0].type == X86_OP_IMM and ops[0].imm in want and ecx: votes[ops[0].imm][ecx] += 1
                    ecx = None
                elif ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) in ('ecx', 'cx', 'cl', 'ch') and i.mnemonic not in ('cmp', 'test', 'push'):
                    ecx = None
    return {f: c.most_common(1)[0][0] for f, c in votes.items()}


def instances(ls, vptrs, limit=16):
    """{sep vptr: [object addresses]} -- heap dwords equal to the class's vtable in the dump"""
    out = {}
    for v in vptrs:
        aug = ls.g_sep_aug.get(v)
        if aug is None: continue
        pat = struct.pack('<I', aug + ls.rel); found = []
        for s, n, fo in ls.segs:
            if ls.base <= s < ls.base + ls.size: continue
            i = ls.mm.find(pat, fo, fo + n)
            while i >= 0 and len(found) < limit:
                if (i - fo) % 4 == 0: found.append(s + i - fo)
                i = ls.mm.find(pat, i + 1, fo + n)
            if len(found) >= limit: break
        out['0x%08X' % v] = ['0x%08X' % x for x in found]
    return out


def main(dmp, build_map, out, dll=None, cases=None):
    dll = dll or os.environ['FFXI_DLL']
    ls = LiveState(dmp, build_map)
    sizes = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export_current.jsonl')):
        for o in json.loads(l)['objs'].values():
            if not o['func'] and 0 < o.get('size', 0) < 1 << 20: sizes[o['ea']] = o['size']
    cs = json.load(open(cases)) if cases else []
    th = this_sources(dll, [int(c['original'], 16) for c in cs if c.get('conv') == 'thiscall'])
    inst = instances(ls, {int(c['vptr'], 16) for c in cs if c.get('vptr')})
    res = {'dump': os.path.abspath(dmp), 'build_map': os.path.abspath(build_map), 'image_base': '0x%08X' % ls.base,
           'sandbox': '0x%08X' % ls.sandbox(), 'data_writes': data_writes(ls, sizes),
           'this_sources': {'0x%08X' % f: [k, '0x%08X' % a] for f, (k, a) in th.items()}, 'instances': inst}
    json.dump(res, open(out, 'w'))
    print('sandbox %s, %d data writes, %d this sources, instances %s' % (res['sandbox'], len(res['data_writes']), len(th),
          {k: len(v) for k, v in inst.items()}))


if __name__ == '__main__':
    main(*sys.argv[1:])
