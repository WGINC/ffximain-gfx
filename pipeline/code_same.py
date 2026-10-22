"""code_same.py -- which tree functions compile to the same code in two builds, so an earlier gate verdict holds.

The gate runs a tree function's compiled code against the original (diffemu_gfx.py). The tree's functions call the
game's code at fixed addresses and the tree's runtime helpers, and read globals at fixed addresses. Two builds of a
function therefore run the same instructions on the same values when its bytes are the same, except where they
hold an address in the reconstruction's own image (a call, a jump, a constant in .rdata), and each such address
names the same thing in both: the same tree function (by its FFXIMAIN address), the same helper or import (by
name), or data with the same bytes. Each function's bytes are compared that way, instruction by instruction;
anything not matched (a symbol not found, an overloaded name, a different size) counts as changed.

    python3 code_same.py OLD_TREE NEW_TREE OUT.json
Output {"same": [addr, ...], "changed": {addr: why}}; both trees built (TREE/build/gfx_recon.dll)."""
import bisect, json, os, re, subprocess, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

NM = os.environ.get('NM', 'i686-w64-mingw32-nm')


def tree_names(tree):
    """{demangled base name (Class::method or free name): FFXIMAIN address}, None for a name defined twice"""
    out = {}
    for f in sorted(os.listdir(os.path.join(tree, 'src'))):
        cur = None
        for line in open(os.path.join(tree, 'src', f)):
            m = re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]{8})', line)
            if m: cur = int(m.group(1), 16); continue
            if cur is not None and not line.startswith('//'):
                s = re.sub(r'__attribute__\s*\(\(.*?\)\)', '', line)
                mm = re.search(r'((?:\w+::)?~?\w+)\s*\(', s)
                if mm:
                    n = mm.group(1); out[n] = None if n in out and out[n] != cur else cur
                cur = None
    return out


class Build:
    def __init__(self, tree):
        self.path = os.path.join(tree, 'build', 'gfx_recon.dll')
        self.pe = pefile.PE(self.path); self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.img = self.pe.get_memory_mapped_image()
        self.syms = []
        for l in subprocess.run([NM, '-C', '-n', self.path], capture_output=True, text=True).stdout.splitlines():
            p = l.split(' ', 2)
            if len(p) == 3 and re.match(r'^[0-9a-f]{8}$', p[0]): self.syms.append((int(p[0], 16), p[1], p[2]))
        self.addrs = [a for a, _, _ in self.syms]
        names = tree_names(tree)
        self.fn = {}                                          # FFXIMAIN address -> (start, end) in this image
        self.what = {}                                        # image address -> canonical name of a code symbol
        for k, (a, t, n) in enumerate(self.syms):
            base = re.sub(r'\(.*$', '', n).strip()
            nxt = self.syms[k + 1][0] if k + 1 < len(self.syms) else a
            if t.lower() == 't':
                ea = names.get(base)
                self.what[a] = ('fn', ea) if ea else ('sym', base)
                if ea and ea not in self.fn: self.fn[ea] = (a, nxt)
        self.md = Cs(CS_ARCH_X86, CS_MODE_32); self.md.detail = True
        self.lo, self.hi = self.base, self.base + len(self.img)

    def target(self, v):
        """what an address inside this image names: a code symbol, or the 16 bytes of data there"""
        if v in self.what: return self.what[v]
        j = bisect.bisect_right(self.addrs, v) - 1
        if j >= 0 and self.syms[j][1].lower() == 't' and v != self.syms[j][0]:
            return ('inside', self.what.get(self.syms[j][0]), v - self.syms[j][0])
        return ('data', bytes(self.img[v - self.base:v - self.base + 16]))

    def norm(self, ea):
        a, b = self.fn[ea]
        out = []
        for i in self.md.disasm(bytes(self.img[a - self.base:b - self.base]), a):
            ops = []
            for o in i.operands:
                if o.type == X86_OP_IMM:
                    v = o.imm & 0xFFFFFFFF
                    if a <= v < b: ops.append(('own', v - a))
                    elif self.lo <= v < self.hi: ops.append(self.target(v))
                    else: ops.append(('imm', v))
                elif o.type == X86_OP_MEM:
                    v = o.mem.disp & 0xFFFFFFFF
                    ops.append(('mem', o.mem.base, o.mem.index, o.mem.scale, o.size,
                                self.target(v) if self.lo <= v < self.hi and not o.mem.base else v))
                else: ops.append(('reg', o.reg))
            out.append((i.mnemonic, tuple(ops)))
        return out


def main(old, new, out):
    A, B = Build(old), Build(new)
    same, changed = [], {}
    for ea in sorted(B.fn):
        if ea not in A.fn: changed['0x%08X' % ea] = 'not in the old build'; continue
        try:
            x, y = A.norm(ea), B.norm(ea)
        except Exception as e:
            changed['0x%08X' % ea] = 'not compared: %r' % e; continue
        if x == y: same.append('0x%08X' % ea)
        else:
            k = next((n for n, (p, q) in enumerate(zip(x, y)) if p != q), min(len(x), len(y)))
            changed['0x%08X' % ea] = 'differs at instruction %d of %d/%d' % (k, len(x), len(y))
    json.dump({'same': same, 'changed': changed}, open(out, 'w'), indent=0)
    print('%d functions: %d the same code, %d changed' % (len(same) + len(changed), len(same), len(changed)))


if __name__ == '__main__':
    main(*sys.argv[1:4])
