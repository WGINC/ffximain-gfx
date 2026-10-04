"""sp_solve.py -- how many bytes each call really pops, solved from the caller's own code: no guesses.

MSVC keeps the stack balanced: wherever two paths meet, ESP is the same on both, and at `ret` (or a
tail-call `jmp`) every byte the function pushed is gone. A call whose callee is unknown (a virtual or
COM call, an import, a function pointer) pops an unknown p_i bytes; the depth along each path is then
const + sum(c_i * p_i), and every meeting point and every `ret` gives one linear equation. Solved
exactly (rational Gaussian elimination), p_i is *determined* when the equations pin it to one value;
otherwise it is left out, never guessed. A function whose equations contradict each other (a call that
never returns, code the extents got wrong) yields nothing.

Known pops: a direct call's callee `ret N` (data/purges.json); a virtual call whose every possible callee
is proven and agrees (data/vcalls.json, vcall_resolve.py); the stack probe _chkstk moves ESP by
EAX. A frame pointer is followed: `mov ebp, esp` / `mov esp, ebp` / `leave` / `lea esp, [ebp-X]`.

    python3 sp_solve.py data/sp_solved.json [ADDR ...]      uses $FFXI_DLL; all functions if none given
Output: {"0xCALL": {"pops": bytes, "func": "0xFUNC"}} for determined calls,
        plus "_inconsistent": [functions whose equations contradict], "_undetermined": count.
"""
import bisect, json, os, sys, struct
from fractions import Fraction
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
_vc = os.path.join(D, 'vcalls.json')
VCALLS = {k: v['pops'] for k, v in (json.load(open(_vc)).items() if os.path.exists(_vc) else ()) if not k.startswith('_')}
_cc = os.path.join(D, 'com_calls.json')
COMCALLS = {k: v['pops'] for k, v in (json.load(open(_cc)).items() if os.path.exists(_cc) else ())}
# COM calls settled by an accepted (unproven) declaration in com_fields.json: a balance using one says so
ACCEPTED = {k: v['field'] for k, v in (json.load(open(_cc)).items() if os.path.exists(_cc) else ()) if 'accepted' in v}
CHKSTK = {0x10312150}           # _chkstk: `mov eax, N; call _chkstk` reserves N bytes
ALIGN = -1                      # the variable for the padding of `and esp, -N` (see func_calls)


class Lin:
    """const + sum(coef * var)"""
    __slots__ = ('c', 'v')

    def __init__(self, c=0, v=None): self.c = c; self.v = v or {}

    def add(self, k, var=None, coef=1):
        v = dict(self.v)
        if var is not None:
            v[var] = v.get(var, 0) + coef
            if v[var] == 0: del v[var]
        return Lin(self.c + k, v)

    def sub(self, o):
        v = dict(self.v)
        for k, x in o.v.items():
            v[k] = v.get(k, 0) - x
            if v[k] == 0: del v[k]
        return Lin(self.c - o.c, v)

    def __eq__(self, o): return self.c == o.c and self.v == o.v


def solve(eqs, nvars):
    """eqs: [Lin == 0]. Returns ({var: value} for determined vars, consistent)"""
    rows = [({k: Fraction(x) for k, x in e.v.items()}, Fraction(-e.c)) for e in eqs if e.v or e.c]
    piv = {}                                    # var -> row (coefs, rhs) with that pivot
    for coefs, rhs in rows:
        coefs = dict(coefs)
        for p, (pc, pr) in piv.items():         # reduce by existing pivots
            if p in coefs:
                f = coefs[p]
                for k, x in pc.items():
                    coefs[k] = coefs.get(k, 0) - f * x
                    if coefs[k] == 0: del coefs[k]
                rhs -= f * pr
        if not coefs:
            if rhs != 0: return {}, False
            continue
        p = min(coefs); f = coefs[p]
        coefs = {k: x / f for k, x in coefs.items()}; rhs /= f
        for q in list(piv):                     # keep reduced
            qc, qr = piv[q]
            if p in qc:
                g = qc[p]; qc = dict(qc)
                for k, x in coefs.items():
                    qc[k] = qc.get(k, 0) - g * x
                    if qc[k] == 0: del qc[k]
                piv[q] = (qc, qr - g * rhs)
        piv[p] = (coefs, rhs)
    return {p: r for p, (c, r) in piv.items() if len(c) == 1}, True


class Image:
    def __init__(self, path):
        pe = pefile.PE(path, fast_load=True)
        self.base = pe.OPTIONAL_HEADER.ImageBase; self.img = pe.get_memory_mapped_image()
        self.md = Cs(CS_ARCH_X86, CS_MODE_32); self.md.detail = True

    def u32(self, a): return struct.unpack('<I', self.img[a - self.base:a - self.base + 4])[0]

    def code(self, ext):
        ins = {}
        for a, b in ext:
            for i in self.md.disasm(self.img[a - self.base:b - self.base], a): ins[i.address] = i
        return ins


NORETURN_API = {'ExitProcess', 'ExitThread', 'FatalAppExitA'}     # documented never to return


def imports(im, path):
    pe = pefile.PE(path)
    return {i.address: i.name.decode() for d in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []) for i in d.imports if i.name}


def noreturn_funcs(im, ext, iat):
    """functions from whose entry no `ret` (or tail call into a returning function) is reachable, when calls
    into the never-returning ones end the path: a fixpoint from the Windows APIs that never return.
    A tail jump or call through a pointer is taken to return (it adds the function to no set)."""
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    def target(i):
        op = i.operands[0]
        if op.type == X86_OP_IMM: return op.imm
        if op.type == X86_OP_MEM and not op.mem.base and not op.mem.index and op.mem.disp in iat: return 'imp:' + iat[op.mem.disp]
        return None
    nr = set()
    def dead(t): return t in nr or (isinstance(t, str) and t[4:] in NORETURN_API)
    changed = True
    while changed:
        changed = False
        for ea, ins in code.items():
            if ea in nr or ea not in ins: continue
            seen, work, rets = {ea}, [ea], False
            while work and not rets:
                a = work.pop(); i = ins[a]; m = i.mnemonic; nxt = [a + i.size]
                if m.startswith('ret'): rets = True; break
                if m == 'call' and dead(target(i)): nxt = []
                elif m == 'jmp':
                    t = target(i)
                    if t in ins: nxt = [t]
                    else:
                        if not dead(t): rets = True
                        nxt = []
                        if i.operands[0].type == X86_OP_MEM and i.operands[0].mem.index: rets = True   # a switch: taken to return
                elif m.startswith(('j', 'loop')) and i.operands[0].type == X86_OP_IMM: nxt.append(i.operands[0].imm)
                elif m in ('int3', 'hlt', 'ud2'): nxt = []
                for n in nxt:
                    if n in ins and n not in seen: seen.add(n); work.append(n)
            if not rets: nr.add(ea); changed = True
    return nr


def switch_bound(prev, jmp):
    """entries of the jump table `jmp [idx*4 + T]` when the two instructions before it are `cmp idx, N` and
    `ja` (unsigned: idx in 0..N), in straight-line code; None otherwise"""
    if len(prev) < 2: return None
    c, j = prev
    idx = jmp.reg_name(jmp.operands[0].mem.index)
    if j.mnemonic != 'ja' or c.mnemonic != 'cmp' or len(c.operands) != 2: return None
    if c.operands[0].type != X86_OP_REG or c.reg_name(c.operands[0].reg) != idx or c.operands[1].type != X86_OP_IMM: return None
    return c.operands[1].imm + 1


class Labels(dict):
    """{target: [addresses of the direct jumps and branches to it]}, with the instructions to look back from"""
    def __init__(self, ins):
        super().__init__(); self.ins = ins; self.keys = sorted(ins)
        for a, i in ins.items():
            if (i.mnemonic == 'jmp' or i.mnemonic.startswith(('j', 'loop'))) and i.operands and i.operands[0].type == X86_OP_IMM:
                self.setdefault(i.operands[0].imm, []).append(a)

    def before(self, a):
        """the instruction that ends exactly where a begins, or None"""
        k = bisect.bisect_left(self.keys, a)
        if k == 0: return None
        p = self.ins[self.keys[k - 1]]
        return p if p.address + p.size == a else None


def labels_of(ins): return Labels(ins)


def entry_bound(labels, jmp, idx):
    """every jump straight to the dispatch must itself bound the index (`cmp idx, M; jbe dispatch`): the
    largest M, or None when one does not"""
    if not isinstance(labels, Labels): return None
    hi = -1
    for a in labels.get(jmp.address, ()):
        j = labels.ins[a]; c = labels.before(a)
        if j.mnemonic != 'jbe' or c is None or c.mnemonic != 'cmp' or len(c.operands) != 2 or c.operands[0].type != X86_OP_REG \
                or c.reg_name(c.operands[0].reg) != idx or c.operands[1].type != X86_OP_IMM: return None
        hi = max(hi, c.operands[1].imm)
    return hi


def switch_targets(im, prev, jmp, labels=frozenset()):
    """the targets of a switch `jmp [r*4 + T]`, read from its tables, when the straight-line code before it
    bounds the index; None otherwise. Two forms (MSVC):
      cmp idx, N; ja default; jmp [idx*4 + T]                          T[0..N]
      cmp idx, N; ja default; [xor r, r;] mov r8|movzx r, byte [idx + B]; jmp [r*4 + T]
                                                                       T[b] for every byte b in B[0..N]
    prev: the instructions directly before the jump, in order, each ending where the next begins. labels: the
    function's jump targets -- a jump into the sequence after its `cmp` would skip the bound, so none may"""
    o = jmp.operands[0]
    if o.type != X86_OP_MEM or o.mem.scale != 4 or not o.mem.index or o.mem.base: return None
    if any(prev[k].address + prev[k].size != (prev[k + 1].address if k + 1 < len(prev) else jmp.address) for k in range(len(prev))):
        return None
    r = jmp.reg_name(o.mem.index); T = o.mem.disp
    n = switch_bound(prev[-2:], jmp) if len(prev) >= 2 else None
    if n is not None:
        if prev[-1].address in labels: return None                  # a jump onto the `ja` skips the `cmp`
        if jmp.address in labels:                                     # other paths reach the dispatch directly
            m = entry_bound(labels, jmp, r)
            if m is None: return None
            n = max(n, m + 1)
        return [im.u32(T + 4 * k) for k in range(n)]
    # two-level: the index register is a byte read from a table at [idx + B]
    k = len(prev) - 1
    if k < 0: return None
    ld = prev[k]; lo = ld.operands
    if ld.mnemonic not in ('mov', 'movzx') or len(lo) != 2 or lo[0].type != X86_OP_REG or lo[1].type != X86_OP_MEM or lo[1].size != 1:
        return None
    dst = ld.reg_name(lo[0].reg)
    if not (dst == r or (ld.mnemonic == 'mov' and dst == {'eax': 'al', 'ebx': 'bl', 'ecx': 'cl', 'edx': 'dl'}.get(r))): return None
    if ld.mnemonic == 'mov':        # an 8-bit load leaves the upper 24 bits: they must have been zeroed just before
        k -= 1
        if k < 0: return None
        z = prev[k]
        if not (z.mnemonic == 'xor' and len(z.operands) == 2 and all(x.type == X86_OP_REG and z.reg_name(x.reg) == r for x in z.operands)):
            return None
    if lo[1].mem.base == 0 or lo[1].mem.index: return None
    idx = ld.reg_name(lo[1].mem.base); B = lo[1].mem.disp
    if k < 2: return None
    c, j = prev[k - 2], prev[k - 1]
    if j.mnemonic != 'ja' or c.mnemonic != 'cmp' or len(c.operands) != 2 or c.operands[0].type != X86_OP_REG \
            or c.reg_name(c.operands[0].reg) != idx or c.operands[1].type != X86_OP_IMM: return None
    if any(x.address in labels for x in prev[k - 1:] + [jmp]): return None
    cases = sorted({im.img[B - im.base + q] for q in range(c.operands[1].imm + 1)})
    return [im.u32(T + 4 * b) for b in cases]


def func_calls(im, ea, ext, purges, starts, hide=False, nr=frozenset(), iat=None):
    """({call address: pops}, consistent, n unknown) for one function"""
    ins = im.code(ext)
    if ea not in ins: return {}, True, 0
    labels = labels_of(ins)
    var, eqs, src = {}, [], []
    reg = lambda i, op: i.reg_name(op.reg) if op.type == X86_OP_REG else None

    def pop_of(i, eax):
        op = i.operands[0]
        if op.type == X86_OP_MEM and not op.mem.base and not op.mem.index and iat and iat.get(op.mem.disp) in NORETURN_API:
            return 'noreturn'
        if op.type == X86_OP_IMM and op.imm in nr: return 'noreturn'
        if op.type != X86_OP_IMM and '0x%08X' % i.address in VCALLS and not hide:
            return VCALLS['0x%08X' % i.address], None          # a virtual call whose callees are proven (vcall_resolve.py)
        if op.type != X86_OP_IMM and '0x%08X' % i.address in COMCALLS and not hide:
            return COMCALLS['0x%08X' % i.address], None        # a D3D8 method on a proven object (com_calls.py)
        if op.type == X86_OP_IMM:
            if op.imm in CHKSTK: return (-eax, None) if eax is not None else None
            k = purges.get('0x%08X' % op.imm)
            if k is not None and not hide: return k, None
            if k is not None: return 0, var.setdefault(i.address, len(var))   # self-test: solve it as unknown
            if op.imm in starts: return 'noreturn'
        return 0, var.setdefault(i.address, len(var))   # unknown: p_i

    state = {ea: (Lin(), None)}; work = [ea]; every = {}
    while work:
        a = work.pop(); d, ebp = state[a]; eax = None; prev = []
        while a in ins:
            i = ins[a]; m = i.mnemonic; ops = i.operands; nd, nebp = d, ebp; nxt = [a + i.size]
            every[a] = (d, ebp)
            r0 = reg(i, ops[0]) if ops else None
            if m == 'call':
                p = pop_of(i, eax)
                if p is None: break
                if p == 'noreturn': nxt = []
                else: k, v = p; nd = d.add(-k, v, -1)
            elif any(o.type == X86_OP_REG and i.reg_name(o.reg) in ('esp', 'sp') for o in ops[1:]) and m == 'xchg': break
            elif m == 'push': nd = d.add(4)
            elif m == 'pop': nd = d.add(-4); nebp = None if r0 == 'ebp' else ebp
            elif m == 'pushal': nd = d.add(32)
            elif m == 'popal': nd = d.add(-32)
            elif m in ('pushfd', 'pushf'): nd = d.add(4)
            elif m in ('popfd', 'popf'): nd = d.add(-4)
            elif r0 == 'esp' and m in ('sub', 'add') and ops[1].type == X86_OP_IMM:
                nd = d.add(ops[1].imm if m == 'sub' else -ops[1].imm)
            elif r0 == 'esp' and m == 'mov' and reg(i, ops[1]) == 'ebp':
                if ebp is None: break
                nd = ebp
            elif m == 'leave':
                if ebp is None: break
                nd = ebp.add(-4); nebp = None
            elif r0 == 'esp' and m == 'lea' and ops[1].type == X86_OP_MEM and i.reg_name(ops[1].mem.base) == 'ebp' and not ops[1].mem.index:
                if ebp is None: break
                nd = ebp.add(-ops[1].mem.disp)
            elif r0 == 'esp' and m == 'and' and ops[1].type == X86_OP_IMM and ALIGN not in d.v:
                # `and esp, -16`: the real depth grows by a padding A >= 0 that differs between runs; it is a
                # variable no equation may pin (a ret or join that does not cancel it refuses the function).
                # Depths after it are exact relative to this point, which is how IDA counts them (A = 0)
                nd = d.add(0, ALIGN, 1)
            elif r0 == 'esp': break                 # `mov esp, reg`, a second alignment: the depth is lost here
            elif r0 == 'ebp' and m == 'mov' and reg(i, ops[1]) == 'esp': nebp = d
            elif r0 == 'ebp' and m not in ('cmp', 'test', 'push'): nebp = None
            if r0 == 'eax' and m == 'mov' and ops[1].type == X86_OP_IMM: eax = ops[1].imm
            elif r0 in ('eax', 'ax', 'al') and m not in ('cmp', 'test', 'push'): eax = None
            if m.startswith('ret'):
                eqs.append(d); src.append(('ret', a)); nxt = []
            elif m == 'jmp':
                if ops[0].type == X86_OP_IMM:
                    t = ops[0].imm
                    if t in ins: nxt = [t]
                    else: nxt = []                  # out of the function: no equation (a jump into shared
                                                    # code can leave at any depth, e.g. 0x102BCE07 at 104)
                elif ops[0].type == X86_OP_MEM and ops[0].mem.scale == 4 and ops[0].mem.index and not ops[0].mem.base:
                    # a switch: its targets, as far as the bound check in front of it fixes them (switch_targets)
                    nxt = [x for x in (switch_targets(im, prev, i, labels) or []) if x in ins]
                else: nxt = []
            elif m.startswith(('j', 'loop')):
                nxt = [a + i.size] + ([ops[0].imm] if ops[0].type == X86_OP_IMM else [])
            elif m in ('int3', 'hlt', 'ud2'): nxt = []
            prev = (prev + [i])[-5:]
            cont = None
            for n in nxt:
                if n not in ins: continue
                if n in state:
                    eqs.append(state[n][0].sub(nd)); src.append(('join', n, a)); continue
                state[n] = (nd, nebp)
                if cont is None and n == a + i.size: cont = n
                else: work.append(n)
            if cont is None: break
            a, d, ebp = cont, nd, nebp
    func_calls.debug = (var, eqs, src)
    if any(ALIGN in e.v for e in eqs): return {}, False, len(var)       # the padding would be pinned
    vals, ok = solve(eqs, len(var))
    func_calls.state = (state, vals if ok else None); func_calls.every = every
    if not ok: return {}, False, len(var)
    out = {}
    for addr, k in var.items():
        x = vals.get(k)
        if x is not None and x.denominator == 1 and 0 <= x <= 0x100 and x % 4 == 0: out[addr] = int(x)
    return out, True, len(var)


def head_depths(im, ea, ext, purges, starts, nr=frozenset(), iat=None):
    """{address: depth} at every block head of the function (each jump target and fall-through after a
    branch) whose depth is exact: every unknown pop on the way solved; after `and esp, -N` the depth is
    counted from the aligned point as IDA counts it (the padding left out). {} for an inconsistent function"""
    calls, ok, _ = func_calls(im, ea, ext, purges, starts, nr=nr, iat=iat)
    if not ok or not hasattr(func_calls, 'state'): return {}
    state, vals = func_calls.state
    if vals is None: return {}
    out = {}
    for a, (d, _) in state.items():
        x = Fraction(d.c)
        if any(k != ALIGN and k not in vals for k in d.v): continue
        for k, c in d.v.items():
            if k != ALIGN: x += c * vals[k]
        if x.denominator == 1: out[a] = int(x)
    return out


def insn_depths(im, ea, ext, purges, starts, nr=frozenset(), iat=None):
    """{address: (depth, ebp depth or None)} before every instruction the walk reached, where exact (as
    head_depths: every unknown pop on the way solved). The address ESP holds there is entry ESP - depth;
    EBP, when it holds a copy of ESP, is entry ESP - its depth"""
    calls, ok, _ = func_calls(im, ea, ext, purges, starts, nr=nr, iat=iat)
    if not ok or not hasattr(func_calls, 'state'): return {}
    state, vals = func_calls.state
    if vals is None: return {}
    def ev(d):
        if d is None or ALIGN in d.v or any(k not in vals for k in d.v): return None
        x = Fraction(d.c) + sum(c * vals[k] for k, c in d.v.items())
        return int(x) if x.denominator == 1 else None
    out = {}
    for a, (d, ebp) in func_calls.every.items():
        x = ev(d)
        if x is not None: out[a] = (x, ev(ebp))
    return out


def known_pop(im, purges, a):
    i = next(im.md.disasm(im.img[a - im.base:a - im.base + 8], a)); op = i.operands[0]
    return purges.get('0x%08X' % op.imm) if op.type == X86_OP_IMM else None


def selftest(im, purges, ext, starts, nr, iat):
    """hide every direct call's known `ret N`, solve, and compare: the solver's own error rate"""
    ok = bad = 0; ex = []
    for k, e in ext.items():
        calls, cons, _ = func_calls(im, int(k, 16), e, purges, starts, hide=True, nr=nr, iat=iat)
        for a, p in calls.items():
            i = next(im.md.disasm(im.img[a - im.base:a - im.base + 8], a)); op = i.operands[0]
            if op.type != X86_OP_IMM: continue
            want = purges.get('0x%08X' % op.imm)
            if want is None: continue
            if want == p: ok += 1
            else: bad += 1; ex.append(('0x%08X' % a, p, want))
    print('self-test: %d direct calls solved with their ret N hidden: %d right, %d wrong %s' % (ok + bad, ok, bad, ex[:10]))
    # a miss is harmless only if normal use rejects its function (inconsistent, or failing its own check)
    leak = []
    for a, p, want in ex:
        f = next(int(k, 16) for k, e in ext.items() if any(x <= int(a, 16) < y for x, y in e))
        calls, cons, _ = func_calls(im, f, ext['0x%x' % f], purges, starts, nr=nr, iat=iat)
        if cons:
            chk, ok2, _ = func_calls(im, f, ext['0x%x' % f], purges, starts, hide=True, nr=nr, iat=iat)
            if ok2 and not [c for c, q in chk.items() if c not in calls and known_pop(im, purges, c) not in (None, q)]:
                leak.append(a)
    print('self-test: %d of %d misses reach the output (the rest are in functions normal use rejects)%s' % (len(leak), bad, (': %s' % leak) if leak else ''))
    return len(leak)


def main(out, *addrs):
    im = Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    iat = imports(im, os.environ['FFXI_DLL']); nr = noreturn_funcs(im, ext, iat)
    print('%d functions never return' % len(nr))
    if out == '--selftest':
        sys.exit(1 if selftest(im, json.load(open(os.path.join(D, 'purges.json'))), ext, {int(k, 16) for k in ext}, nr, iat) else 0)
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}
    todo = [int(x, 16) for x in addrs] if addrs else sorted(starts)
    res, bad, und, tot, suspect = {}, [], 0, 0, []
    for ea in todo:
        e = ext.get('0x%x' % ea)
        if not e: continue
        calls, ok, n = func_calls(im, ea, e, purges, starts, nr=nr, iat=iat)
        if not ok: bad.append('0x%08X' % ea); continue
        # the function's equations checked against its own known calls: solved again with their `ret N`
        # hidden, every one that comes out determined must come out right, else none of its results is used
        # (a call that does not return, which the code alone cannot show, leaves a false equation behind)
        chk, ok2, _ = func_calls(im, ea, e, purges, starts, hide=True, nr=nr, iat=iat)
        wrong = [a for a, p in chk.items() if a not in calls and known_pop(im, purges, a) not in (None, p)]
        if not ok2 or wrong: suspect.append('0x%08X' % ea); und += len(calls); tot += n; continue
        tot += n; und += n - len(calls)
        acc = sorted({ACCEPTED[k] for k in ACCEPTED if any(x <= int(k, 16) < y for x, y in e)})
        for a, p in calls.items():
            res['0x%08X' % a] = {'pops': p, 'func': '0x%08X' % ea}
            if acc: res['0x%08X' % a]['rests_on_accepted'] = acc
    res['_inconsistent'] = bad; res['_suspect'] = suspect; res['_undetermined'] = und
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d unknown calls: %d solved, %d undetermined; %d functions inconsistent, %d failed their own check'
          % (tot, tot - und, und, len(bad), len(suspect)))


if __name__ == '__main__':
    main(*sys.argv[1:])
