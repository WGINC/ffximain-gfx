"""ps2_elf.py -- the PS2 game executable's symbols, code references and data, read from the ELF itself.

The owner's PS2 executable (INSTALL.ELF: MIPS R5900, Metrowerks MIPS C 2.4.1.01, not stripped) is never part of
this repository: point PS2_ELF at a local copy. Everything here comes from the ELF's own tables:

  * functions: the symbol table's FUNC symbols in the `main` section (name, address, size), and NOTYPE symbols
    in it that start code (Metrowerks writes C++ methods as NOTYPE), each ending at the next symbol or its size;
  * data references: the relocation table `.relmain` (MIPS ELF ABI). An R_MIPS_HI16 at A followed by R_MIPS_LO16
    entries of the same symbol is one address, built from the two instructions' immediates as the linked image
    holds them: (hi << 16) + sign-extended lo. R_MIPS_26 is a direct call or jump: its target is the symbol's value.
    R_MIPS_32 is a 4-byte pointer stored in data (a vtable slot, a table entry).
  * the image: the `main` section's bytes, to read the strings and tables those addresses point at.

    PS2_ELF=... python3 ps2_elf.py SUMMARY     prints what was read"""
import bisect, os, struct, sys

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_GPREL16, R_MIPS_LITERAL = 2, 4, 5, 6, 7, 8


class Elf:
    def __init__(self, path):
        b = self.b = open(path, 'rb').read()
        if b[:4] != b'\x7fELF' or b[4] != 1 or b[5] != 1: raise SystemExit('%s: not a 32-bit little-endian ELF' % path)
        shoff, = struct.unpack_from('<I', b, 0x20); shentsize, shnum, shstrndx = struct.unpack_from('<HHH', b, 0x2E)
        secs = [struct.unpack_from('<10I', b, shoff + i * shentsize) for i in range(shnum)]
        strtab = secs[shstrndx]
        nm = lambda o: b[strtab[4] + o:b.index(b'\0', strtab[4] + o)].decode('latin-1')
        self.secs = [(nm(s[0]),) + s for s in secs]
        main = [s for s in self.secs if s[0] == 'main']
        if not main: raise SystemExit('%s: no `main` section' % path)
        _, _, _, _, self.base, off, size = main[0][:7]
        self.main_index = self.secs.index(main[0])
        self.img = b[off:off + size]
        sym = [s for s in self.secs if s[0] == '.symtab'][0]; sstr = self.secs[sym[7]]
        sname = lambda o: b[sstr[5] + o:b.index(b'\0', sstr[5] + o)].decode('latin-1')
        self.syms = []
        for i in range(sym[6] // 16):
            n, v, sz, info, other, shndx = struct.unpack_from('<IIIBBH', b, sym[5] + 16 * i)
            self.syms.append((sname(n) if n else '', v, sz, info & 0xF, shndx))
        ri = [s for s in self.secs if s[0] == '.reginfo']
        self.gp = struct.unpack_from('<I', b, ri[0][5] + 20)[0] if ri else None      # Elf32_RegInfo.ri_gp_value
        self.rels = []
        for s in self.secs:
            if s[0] != '.relmain': continue
            for i in range(s[6] // 8):
                o, info = struct.unpack_from('<II', b, s[5] + 8 * i)
                self.rels.append((o, info & 0xFF, info >> 8))

    def u32(self, a): return struct.unpack_from('<I', self.img, a - self.base)[0]
    def inside(self, a): return self.base <= a < self.base + len(self.img)

    def cstr(self, a, limit=512):
        """the NUL-terminated bytes at a, or None"""
        if not self.inside(a): return None
        o = a - self.base; e = self.img.find(b'\0', o, o + limit)
        return self.img[o:e] if e >= 0 else None

    def functions(self):
        """{start: (name, end)}: FUNC and code NOTYPE symbols of `main`; a function ends at its size, or else at the
        next symbol"""
        code = sorted({(v, n, sz, t) for n, v, sz, t, sh in self.syms if sh == self.main_index and n and t in (0, 2)
                       and not n.startswith(('@', '.'))})
        starts = sorted({v for v, _, _, _ in code})
        out = {}
        for v, n, sz, t in code:
            if v in out: continue
            j = bisect.bisect_right(starts, v)
            nxt = starts[j] if j < len(starts) else self.base + len(self.img)
            out[v] = (n, v + sz if sz else nxt)
        return out

    def refs(self):
        """[(at, kind, target)]: kind 'call' (R_MIPS_26), 'addr' (HI16/LO16 pair), 'ptr' (R_MIPS_32 in data),
        'gp' (R_MIPS_GPREL16: a small global) and 'lit' (R_MIPS_LITERAL: a literal pool constant), each $gp plus the
        instruction's signed 16-bit offset (.reginfo's gp)"""
        out, hi = [], {}
        for o, t, s in self.rels:
            v = self.syms[s][1] if s < len(self.syms) else 0
            if t == R_MIPS_26: out.append((o, 'call', v))
            elif t in (R_MIPS_GPREL16, R_MIPS_LITERAL) and self.gp is not None and self.inside(o):
                lo = self.u32(o) & 0xFFFF
                out.append((o, 'lit' if t == R_MIPS_LITERAL else 'gp', (self.gp + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF))
            elif t == R_MIPS_32: out.append((o, 'ptr', self.u32(o) if self.inside(o) else v))
            elif t == R_MIPS_HI16: hi[s] = o
            elif t == R_MIPS_LO16 and s in hi and self.inside(o) and self.inside(hi[s]):
                h = self.u32(hi[s]) & 0xFFFF; lo = self.u32(o) & 0xFFFF
                out.append((o, 'addr', ((h << 16) + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF))
        return out


    def float_immediates(self, lo, hi):
        """[(at, float bits)]: `lui r, H` (then `ori r, r, L`) moved to the FPU by `mtc1 r, $f` within the next 4
        instructions, with r not written in between (MIPS32: lui 0x0F, ori 0x0D, COP1 0x11 with rs 4 = MTC1)"""
        out = []; hi = min(hi, self.base + len(self.img))
        for a in range(lo, hi - 3, 4):
            w = self.u32(a)
            if w >> 26 != 0x0F: continue
            r = (w >> 16) & 31; val = (w & 0xFFFF) << 16
            for k in range(1, 5):
                b = a + 4 * k
                if b >= hi: break
                x = self.u32(b)
                if x >> 26 == 0x0D and (x >> 21) & 31 == r and (x >> 16) & 31 == r: val |= x & 0xFFFF; continue
                if x >> 26 == 0x11 and (x >> 21) & 31 == 4 and (x >> 16) & 31 == r: out.append((b, val)); break
                op = x >> 26                                               # r written (R-type rd; I-type rt): stop
                if (op == 0 and (x >> 11) & 31 == r) or ((8 <= op <= 15 or 32 <= op <= 39 or op == 55) and (x >> 16) & 31 == r): break
        return out


if __name__ == '__main__':
    e = Elf(os.environ['PS2_ELF'])
    f = e.functions(); r = e.refs()
    import collections
    print('main at 0x%X, %d bytes; %d symbols; %d functions; refs %s' % (
        e.base, len(e.img), len(e.syms), len(f), dict(collections.Counter(k for _, k, _ in r))))
