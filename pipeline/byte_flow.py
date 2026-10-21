"""byte_flow.py -- where the bytes of one value go through a function's registers, one instruction at a time.

caller_reads.py follows a value (a function's entry EAX, or a callee's EAX result) over a function's control flow.
This module is the one-instruction step of that walk: which bits of EAX, ECX, EDX, EBX, ESI, EDI, EBP hold data that
came from which byte of the value, and whether an instruction looks at it.

The state holds, per register and per source byte s (0-3) of the value, two 32-bit masks, each a "may" (on some
path to here):
  * `c`: the register's bits that may hold data computed from byte s (changed);
  * `p`: the register's bits that may be byte s's own bits, unchanged and in their own place (copies only);
and the source bytes the flags were last computed from. A bit depends on byte s when it is in either (`d`). At a
join both masks are the union of the paths' (one path's unchanged byte and another's overwrite stay unchanged-or-
absent, never "changed").

Each instruction's effect, from its definition in the Intel SDM, bit by bit:
  * copies (mov, movzx, movsx, xchg between registers) move the bits; movzx clears the upper ones; movsx's upper
    bits are copies of the source's top bit (dependent, not unchanged);
  * add, adc, sub, sbb, inc, dec, neg, and lea's address arithmetic: a result bit depends on the operand bits at or
    below it (carries run upward); and, or, xor, not: on the same bit only, and an immediate bit that fixes the
    result bit (and with 0, or with 1) leaves no dependence there; xor/sub r, r give 0 and sbb r, r gives -CF;
  * shifts and rotates by a constant move the bits by that much (sar copies the top bit down); by CL, a result
    bit may depend on any bit (in the direction of the shift), and CL itself is examined;
  * flags: an instruction that writes them records the sources its result depends on; one that reads them (jcc,
    setcc, cmovcc, adc, sbb, ...) examines those sources.

Two kinds of read:
  * examined ("seen"): a compare or test (test with an immediate: the bits it keeps), bt, a multiply or divide, a
    string compare (scas, cmps), the bits used as an address (a memory operand's base or index; lea forms an address
    and uses none), a flag read of flags computed from them;
  * escaped ("may"): the bits stored to memory or pushed, or read by an instruction this module does not model.
    Where they go is not followed: they may be read there.
Every examined source is also counted escaped. An instruction not modelled drops the bits of what it writes."""
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

FULL = 0xFFFFFFFF
REG32 = ('eax', 'ecx', 'edx', 'ebx', 'esi', 'edi', 'ebp')
PART = {}                                                  # name -> (register, bit offset, width)
for r32, r16, lo, hi in (('eax', 'ax', 'al', 'ah'), ('ecx', 'cx', 'cl', 'ch'), ('edx', 'dx', 'dl', 'dh'),
                         ('ebx', 'bx', 'bl', 'bh')):
    PART.update({r32: (r32, 0, 32), r16: (r32, 0, 16), lo: (r32, 0, 8), hi: (r32, 8, 8)})
for r32, r16 in (('esi', 'si'), ('edi', 'di'), ('ebp', 'bp')):
    PART.update({r32: (r32, 0, 32), r16: (r32, 0, 16)})
CARRY = ('add', 'adc', 'sub', 'sbb', 'inc', 'dec', 'neg')
BITWISE = ('and', 'or', 'xor', 'not')
SHIFT = ('shl', 'sal', 'shr', 'sar', 'rol', 'ror', 'rcl', 'rcr')
EXAMINE = ('cmp', 'test', 'bt', 'mul', 'imul', 'div', 'idiv', 'scasb', 'scasw', 'scasd', 'cmpsb', 'cmpsw', 'cmpsd')
STORE = ('push', 'pushal', 'stosb', 'stosw', 'stosd')
SRC = (0, 1, 2, 3)


def up(x, width=32):
    """every bit at or above the lowest set bit (a carry's reach)"""
    return 0 if not x else (~((x & -x) - 1)) & ((1 << width) - 1)


def down(x, width=32):
    """every bit at or below the highest set bit"""
    return 0 if not x else (1 << x.bit_length()) - 1 & ((1 << width) - 1)


class State:
    """{register: ((c0..c3), (p0..p3))} with registers holding nothing left out, the flags' sources, and
    {register: constant} for registers that hold a known constant on every path (mov r, imm; xor r, r)"""
    __slots__ = ('regs', 'flags', 'k', '_k')

    def __init__(self, regs=None, flags=frozenset(), k=None):
        self.regs = {r: v for r, v in (regs or {}).items() if any(v[0]) or any(v[1])}
        self.flags = frozenset(flags); self.k = dict(k or {}); self._k = None

    def key(self):
        if self._k is None: self._k = (tuple(sorted(self.regs.items())), self.flags, tuple(sorted(self.k.items())))
        return self._k

    def c(self, reg): return self.regs.get(reg, ((0,) * 4, (0,) * 4))[0]

    def p(self, reg): return self.regs.get(reg, ((0,) * 4, (0,) * 4))[1]

    def d(self, reg):
        c, p = self.regs.get(reg, ((0,) * 4, (0,) * 4)); return tuple(x | y for x, y in zip(c, p))

    def srcs(self, reg, mask=FULL):
        """the source bytes some bit of reg under mask depends on"""
        return {s for s in SRC if self.d(reg)[s] & mask}

    def any(self): return bool(self.regs)


def entry_state(reg='eax', sources=SRC):
    """a register holding the value's bytes unchanged, byte k in bits 8k..8k+7"""
    p = tuple((0xFF << (8 * s)) if s in sources else 0 for s in SRC)
    return State({reg: ((0,) * 4, p)})


def join(a, b):
    """the union of two states (a may-analysis: what either path holds)"""
    regs = {}
    for r in set(a.regs) | set(b.regs):
        ca, pa = a.regs.get(r, ((0,) * 4, (0,) * 4)); cb, pb = b.regs.get(r, ((0,) * 4, (0,) * 4))
        regs[r] = (tuple(x | y for x, y in zip(ca, cb)), tuple(x | y for x, y in zip(pa, pb)))
    return State(regs, a.flags | b.flags, {r: v for r, v in a.k.items() if b.k.get(r) == v})


def part_mask(off, width): return ((1 << width) - 1) << off


def mem_regs(i):
    out = []
    for op in i.operands:
        if op.type == X86_OP_MEM:
            for r in (op.mem.base, op.mem.index):
                if r and i.reg_name(r) in PART: out.append((PART[i.reg_name(r)][0], op.mem.scale if r == op.mem.index else 1))
    return out


def step(i, st, strict=False):
    """(new State, examined sources, escaped sources) for one instruction that is not a call, ret or jump.
    strict (the walk for reads seen): an and/or with a register whose value is not a known constant keeps no
    dependence in its result, since which bits survive is not known (the may-walk keeps them all)"""
    m = i.mnemonic.split()[-1]                        # `rep stosd`: stosd
    o = i.operands
    regs = dict(st.regs)
    consts = dict(st.k)
    examined, escaped = set(), set()
    try: rd, wr = i.regs_access()
    except Exception: rd, wr = [], []
    rd = {i.reg_name(r) for r in rd}; wr = {i.reg_name(r) for r in wr}
    flags = set(st.flags)
    if 'eflags' in rd and flags: examined |= flags            # a flag read of flags computed from the value
    if m != 'lea':
        for r, _ in mem_regs(i): examined |= st.srcs(r)

    def get(name):
        """(c, p) of a register part, shifted down to bit 0"""
        reg, off, w = PART[name]; mk = (1 << w) - 1
        c, p = st.c(reg), st.p(reg)
        return tuple((x >> off) & mk for x in c), tuple((x >> off) & mk for x in p)

    def getd(name):
        c, p = get(name); return tuple(x | y for x, y in zip(c, p))

    def put(name, c, p):
        reg, off, w = PART[name]; mk = part_mask(off, w)
        oc, op_ = regs.get(reg, ((0,) * 4, (0,) * 4))
        nc = tuple((a & ~mk) | ((b << off) & mk) for a, b in zip(oc, c))
        npp = tuple((a & ~mk) | ((b << off) & mk) for a, b in zip(op_, p))
        regs[reg] = (nc, npp)

    def clear(reg): regs.pop(reg, None)

    def touched():
        s = set()
        for r in rd:
            if r in PART:
                reg, off, w = PART[r]; s |= st.srcs(reg, part_mask(off, w))
        return s

    Z = (0,) * 4
    rn = lambda op: i.reg_name(op.reg) if op.type == X86_OP_REG and i.reg_name(op.reg) in PART else None
    dname = rn(o[0]) if o else None
    sname = rn(o[1]) if len(o) > 1 else None
    width = PART[dname][2] if dname else 32
    wm = (1 << width) - 1
    new_flags = None
    if m in ('mov', 'movzx', 'movsx') and dname and sname:
        sc, sp = get(sname); sw = PART[sname][2]
        same_place = PART[sname][1] == PART[dname][1]
        cc = [c | (0 if same_place else p) for c, p in zip(sc, sp)]; pp = [p if same_place else 0 for p in sp]
        if m == 'movsx' and width > sw:
            top = 1 << (sw - 1)
            cc = [c | (((wm >> sw) << sw) if (c | p) & top else 0) for c, p in zip(cc, sp)]
        put(dname, tuple(cc), tuple(pp))
    elif m == 'xchg' and dname and sname:
        ac, ap = get(dname); bc, bp = get(sname)
        same = PART[sname][1] == PART[dname][1]
        if same: put(dname, bc, bp); put(sname, ac, ap)
        else: put(dname, tuple(x | y for x, y in zip(bc, bp)), Z); put(sname, tuple(x | y for x, y in zip(ac, ap)), Z)
    elif m in ('mov', 'movzx', 'movsx') and dname:               # from memory or an immediate
        put(dname, Z, Z)
        if m == 'mov' and o[1].type == X86_OP_IMM and PART[dname][2] == 32: consts['_set'] = (dname, o[1].imm & FULL)
    elif m == 'mov' and o and o[0].type == X86_OP_MEM and sname:   # a store: the bits escape
        reg, off, w = PART[sname]; escaped |= st.srcs(reg, part_mask(off, w))
    elif m in STORE:
        escaped |= touched()
    elif m == 'lea' and dname:
        acc = [0] * 4
        for r, scale in mem_regs(i):
            sh = {1: 0, 2: 1, 4: 2, 8: 3}[scale]
            for s_ in SRC: acc[s_] |= up((st.d(r)[s_] << sh) & FULL)
        put(dname, tuple(x & wm for x in acc), Z)
    elif (m in CARRY or m in BITWISE) and dname:
        dd = getd(dname)
        imm = o[1].imm if len(o) > 1 and o[1].type == X86_OP_IMM else None
        if imm is None and sname and PART[sname][2] == 32 and sname in st.k: imm = st.k[sname]   # a known constant
        sd = getd(sname) if sname else Z
        if sname == dname and m in ('xor', 'sub', 'sbb'):       # 0, 0, or -CF
            out = [wm if (m == 'sbb' and s in flags) else 0 for s in SRC]
            if m in ('xor', 'sub') and PART[dname][2] == 32: consts['_set'] = (dname, 0)
        elif strict and m in ('and', 'or') and imm is None and len(o) > 1 and o[1].type != X86_OP_IMM:
            out = [0, 0, 0, 0]                                    # which bits survive is not known
        elif m in BITWISE:
            out = [a | b for a, b in zip(dd, sd)]
            if imm is not None:
                if m == 'and': out = [x & imm & wm for x in out]
                elif m == 'or': out = [x & ~imm & wm for x in out]
        else:
            out = [up(a | b, width) for a, b in zip(dd, sd)]
            if m in ('adc', 'sbb'): out = [x | (wm if s in flags else 0) for x, s in zip(out, SRC)]
        put(dname, tuple(out), Z)
        new_flags = {s for s in SRC if out[s]}
    elif m in SHIFT and dname:
        dd = getd(dname)
        cnt = o[1] if len(o) > 1 else None
        n = (cnt.imm & 31) if cnt is not None and cnt.type == X86_OP_IMM else (1 if cnt is None else None)
        if n is None: examined |= st.srcs('ecx', 0xFF)              # a count taken from CL
        out = []
        for x in dd:
            if n is None:
                out.append((up(x, width) if m in ('shl', 'sal') else down(x, width) if m in ('shr', 'sar') else (wm if x else 0)))
            elif m in ('shl', 'sal'): out.append((x << n) & wm)
            elif m == 'shr': out.append(x >> n)
            elif m == 'sar': out.append((x >> n) | ((wm & ~(wm >> n)) if x & (1 << (width - 1)) else 0))
            elif m in ('rol', 'ror'):
                k = n % width if width else 0
                out.append((((x << k) | (x >> (width - k))) if m == 'rol' else ((x >> k) | (x << (width - k)))) & wm)
            else: out.append(wm if x else 0)                        # rcl/rcr: through the carry
        if m in ('rcl', 'rcr'): out = [x | (wm if s in flags else 0) for x, s in zip(out, SRC)]
        put(dname, tuple(out), Z)
        new_flags = {s for s in SRC if out[s]}
    elif m in EXAMINE:
        if m == 'test' and dname and len(o) > 1 and o[1].type == X86_OP_IMM:
            reg, off, w = PART[dname]; examined |= st.srcs(reg, (o[1].imm & ((1 << w) - 1)) << off)
        else:
            examined |= touched()
        new_flags = set()
        if m in ('mul', 'imul', 'div', 'idiv'):                  # their results: made from what was examined
            for r in wr:
                if r in PART and PART[r][2] == 32: clear(r)
                elif r in PART: put(r, Z, Z)
    else:
        t = touched()
        escaped |= t                                                # not modelled: its effect is not followed
        for r in wr:
            if r in PART:
                if PART[r][2] == 32: clear(r)
                else: put(r, Z, Z)
        new_flags = set()
    if 'eflags' in wr: flags = new_flags if new_flags is not None else set()
    keep_set = consts.pop('_set', None)
    for r in wr:                                                    # a register written forgets its constant
        if r in PART: consts.pop(PART[r][0], None)
    if keep_set: consts[keep_set[0]] = keep_set[1]
    return State(regs, flags, consts), examined, escaped | examined
