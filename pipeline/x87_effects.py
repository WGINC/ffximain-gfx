"""x87_effects.py -- what each function does to its caller's x87 register stack, read from the game's code.

For every function (data/func_extents.json) the x87 stack depth is followed along every path from entry, relative
to the depth on entry. Slots are numbered from the entry: the caller's values sit at -1 (its ST0), -2, ...; a push
at depth d fills slot d. An instruction touching st(i) at depth d touches slot d-1-i; a pop at depth d removes
slot d-1 (Intel SDM vol. 1 ch. 8 for every push/pop/register operand rule used). From that:

  k  inputs: the caller's slots the function touches, -(lowest slot touched); its arguments on the x87 stack
  r  outputs: what it leaves at or above slot -k on return (depth at `ret` + k), the same on every `ret`
  m  the deepest it pushes above its entry depth (overflow: a caller at depth D needs D + m <= 8)

The caller's slots below -k are never touched: a function with a verdict leaves them as they were, by construction.

A call inside a function applies the callee's own verdict (computed first; mutual recursion is solved to a fixpoint,
starting from "touches nothing" and accepted only when every function's verdict reproduces itself). A direct callee
with no verdict refuses the caller.

A call whose target the code does not fix (an import, an indirect call, a jump through a pointer) is taken only at
an empty x87 stack (depth 0, nothing touched): the Microsoft x86 conventions leave the x87 stack empty on entry and
on return except a float return in ST0 (Microsoft C++ documentation, x86 calling conventions / "__cdecl": floating
point values returned in ST0). What it returns is then established from the caller's own code, never assumed:
  - vcalls.json `ret: st0` for that call site: a float;
  - the caller then reads an x87 value below everything it pushed since (a "fence" names that call): the only
    value there is that call's float return; the function is analysed again with the call returning one, and the
    verdict holds only if that analysis is consistent ("float_shown_by_caller" on the site);
  - the caller returns at depth 0 with nothing read since (or tail-jumps through a pointer): it returns what that
    call returned. A C function has one return type (ISO C 6.9.1), so that return must match the function's other
    returns; with no other return its x87 return is not known and the verdict carries u = true.
A function with u = true is itself only taken at an empty x87 stack.

A jump to another function's entry (conditional or not) is a tail call: its verdict applies, then the function
returns. Control flow, not IDA's extents, decides what code a function runs: code past an extent is decoded from
the DLL. A switch `jmp [r*4 + T]` follows the table entries for the index values the code establishes on every
path into the jump (sp_solve.switch_targets, else index_values: bound checks `cmp`/`ja`/`jb`/`jbe`, a borrow
`sub; jb`, `and`, `mov`, `neg`, `add`/`sub` imm, a copy from another register; switch targets and only reachable
code as predecessors). A table entry outside the image is not DLL code and is dropped (no execution that does not
fault selects it). Any other indirect jump refuses the function, except the MSVC CRT intrinsic dispatch at
0x1031AF77 (crt_dispatch), whose targets are read from the DLL's own tables for the EDX the caller set.

`fnstenv [esp]` ... `fldenv [esp]` is followed through when the code between can only leave the saved TOP field as
it was (env_pair); `ffree`, `fincstp`, `fdecstp`, `finit`, `fsave`/`frstor` refuse the function.

Output data/x87_effects.json:
  {"funcs": {ea: {"k", "r", "m", "u"} | {"why": its own step, "root": the cause at the bottom of the chain}},
   "sites": {call address: {"func": caller, "callee": ea|null, "d": depth before the call relative to the caller's
             entry, "low": lowest slot the caller touched before it, "k", "r", "u": the callee's verdict}}}
Usage: python3 x87_effects.py GAME.dll data/func_extents.json OUT.json [--selftest]
"""
import json, os, struct, sys
import capstone, pefile
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sp_solve import switch_targets, labels_of, SWITCH_RUN   # noqa: E402

PUSH = {'fld', 'fild', 'fldz', 'fld1', 'fldpi', 'fldl2e', 'fldln2', 'fldlg2', 'fldl2t', 'fbld'}
PUSH_AFTER = {'fsincos', 'fptan', 'fxtract'}     # replace ST0, then push one more (touch st(0) first)
POP1 = {'fstp', 'fistp', 'faddp', 'fsubp', 'fsubrp', 'fmulp', 'fdivp', 'fdivrp', 'fcomp', 'fucomp',
        'fbstp', 'fcomip', 'fucomip', 'fisttp', 'fpatan', 'fyl2x', 'fyl2xp1', 'fistpll', 'ficomp'}
POP2 = {'fcompp', 'fucompp'}
# instructions that read or write st(1) besides st(0) without a register operand naming it
USES_ST1 = {'fprem', 'fprem1', 'fscale', 'fpatan', 'fyl2x', 'fyl2xp1', 'fcompp', 'fucompp'}
CTRL = {'fnstsw', 'fstsw', 'fnstcw', 'fstcw', 'fldcw', 'fwait', 'wait', 'fnclex', 'fclex', 'fninit', 'finit',
        'femms', 'fnstenv', 'fstenv', 'fldenv', 'fnsave', 'fsave', 'frstor', 'fnop', 'ffree', 'fdecstp', 'fincstp'}
REFUSE = {'ffree', 'fdecstp', 'fincstp', 'fninit', 'finit', 'fnsave', 'fsave', 'frstor', 'fldenv'}   # stack reset/rotate


class Image:
    def __init__(self, path):
        pe = pefile.PE(path, fast_load=True); pe.parse_data_directories()
        self.base = pe.OPTIONAL_HEADER.ImageBase; self.img = bytes(pe.get_memory_mapped_image())
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); self.md.detail = True
        self.iat = {}
        for d in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []):
            for e in d.imports:
                self.iat[e.address] = (e.name or b'#%d' % e.ordinal).decode()
        self.cache = {}

    def u32(self, a): return struct.unpack('<I', self.img[a - self.base:a - self.base + 4])[0]
    def u8(self, a): return self.img[a - self.base]

    def insn(self, a):
        if a not in self.cache:
            self.cache[a] = next(self.md.disasm(self.img[a - self.base:a - self.base + 16], a, 1), None)
        return self.cache[a]

    def code(self, ext):
        """{address: insn} for a function's chunks, as sp_solve's Image.code does"""
        out = {}
        for s, e in ext:
            for i in self.md.disasm(self.img[s - self.base:e - self.base], s):
                out[i.address] = i
        return out


# 0x1031AF77 (MSVC CRT intrinsic dispatch, the `_trandisp`-style table walk): with EDX = a function table, it
# classifies ST0 and ST1 (fxam), maps each class through the 16-byte table at 0x103D099C (xlatb) and jumps to the
# dword at EDX + 0x10 + (cls(ST0) | cls(ST1) << 2). These are its bytes; the targets are read for the EDX a caller
# set, for every pair of table classes -- a superset of what runs, so a verdict holds for every operand class.
CRT_DISPATCH = 0x1031AF77
CRT_DISPATCH_BYTES = bytes.fromhex(
    '807a0e057511668b9d5cffffff80cf0280e7feb33feb0466bb3f1366899d5effffffd9ad5effffffbb9c093d10d9e589'
    '956cffffff9bddbd60ffffffc68570ffffff00d9c98a8d61ffffffd9e59bddbd60ffffffd9c98aad61ffffffd0e5d0fd'
    'd0c58ac5240fd78ae0d0e1d0f9d0c18ac1240fd7d0e4d0e40ac40fbec081e1040400008bda03d883c310ff23')
CLASS_TABLE = 0x103D099C


def crt_dispatch_targets(im, edx):
    if edx is None: return None
    n = len(CRT_DISPATCH_BYTES)
    if im.img[CRT_DISPATCH - im.base:CRT_DISPATCH - im.base + n] != CRT_DISPATCH_BYTES: return None
    cls = [im.u8(CLASS_TABLE + q) for q in range(16)]
    idx = set()
    for a in cls:
        for b in cls:
            v = (a | ((b << 2) & 0xFF)) & 0xFF
            idx.add(v - 256 if v >= 128 else v)                  # movsx eax, al
    return sorted({im.u32(edx + 0x10 + v) for v in idx})


def env_pair(ins, i):
    """`fnstenv [esp]` ... `fldenv [esp]` that restores the stack top it saved: the address after the fldenv, or None.
    In the 32-bit protected-mode environment (Intel SDM vol. 1, 8.1.10) the status word is the dword at offset 4 and
    TOP is its bits 11-13 (0x3800). Between the two, straight-line code may only AND that dword with a mask keeping
    0x3800 or OR it with a register last set by `and reg, imm` with imm & 0x3800 == 0 (or not written in between)."""
    o = i.operands[0]
    if o.type != X86_OP_MEM or i.reg_name(o.mem.base) != 'esp' or o.mem.index or o.mem.disp: return None
    masked = {}                                      # register -> mask of its last `and reg, imm`
    a = i.address + i.size
    p = ins.get(i.address)
    # look back for `and reg, imm` setting the OR source (straight line before the fnstenv, within 4 instructions)
    back = sorted(x for x in ins if x < i.address)[-4:]
    for x in back:
        q = ins[x]
        if q.mnemonic == 'and' and q.operands[0].type == X86_OP_REG and q.operands[1].type == X86_OP_IMM:
            masked[q.reg_name(q.operands[0].reg)] = q.operands[1].imm
        elif q.operands and q.operands[0].type == X86_OP_REG and q.reg_name(q.operands[0].reg) in masked and q.mnemonic not in ('cmp', 'test', 'push'):
            masked.pop(q.reg_name(q.operands[0].reg))
    for _ in range(8):
        q = ins.get(a)
        if q is None: return None
        ops = q.operands
        if q.mnemonic == 'fldenv':
            o2 = ops[0]
            return a + q.size if (o2.type == X86_OP_MEM and q.reg_name(o2.mem.base) == 'esp' and not o2.mem.index and not o2.mem.disp) else None
        if len(ops) != 2 or ops[0].type != X86_OP_MEM or q.reg_name(ops[0].mem.base) != 'esp' or ops[0].mem.index or ops[0].mem.disp != 4:
            return None
        if q.mnemonic == 'and' and ops[1].type == X86_OP_IMM and (ops[1].imm & 0x3800) == 0x3800: pass
        elif q.mnemonic == 'or' and ops[1].type == X86_OP_REG and (masked.get(q.reg_name(ops[1].reg), 0xFFFF) & 0x3800) == 0: pass
        else: return None
        a += q.size
    return None


_REG_SAVED = None


def call_keeps(q, reg):
    """a direct call hands `reg` back unchanged: an EBX/ESI/EDI/EBP its callee restores (data/reg_saved.json).
    Capstone lists only ESP as written by a call; what the callee writes is the callee's code, so every other
    register is written unless that fact says otherwise (EAX/ECX/EDX: no fact keeps them)"""
    global _REG_SAVED
    if _REG_SAVED is None:
        f = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'reg_saved.json')
        _REG_SAVED = json.load(open(f)) if os.path.exists(f) else {}
    o = q.operands
    if not (o and o[0].type == X86_OP_IMM): return False
    return reg in _REG_SAVED.get('0x%08X' % (o[0].imm & 0xFFFFFFFF), ())


def writes(q, reg):
    if q.mnemonic == 'call': return not call_keeps(q, reg)
    try: _, w = q.regs_access()
    except capstone.CsError: return True
    fam = {'eax': ('eax', 'ax', 'al', 'ah'), 'ebx': ('ebx', 'bx', 'bl', 'bh'), 'ecx': ('ecx', 'cx', 'cl', 'ch'),
           'edx': ('edx', 'dx', 'dl', 'dh'), 'esi': ('esi', 'si'), 'edi': ('edi', 'di'), 'ebp': ('ebp', 'bp')}.get(reg, (reg,))
    return any(q.reg_name(x) in fam for x in w)


def flag_setter(labels, q, reg):
    """the instruction that set the flags branch q tests: walking back over straight-line instructions that write
    neither the flags nor reg (no jump may land between); None otherwise"""
    c = labels.before(q.address)
    for _ in range(6):
        if c is None or c.mnemonic == 'call': return None       # the callee leaves the flags as its code does
        try: _, w = c.regs_access()
        except capstone.CsError: return None
        names = {c.reg_name(x) for x in w}
        if 'eflags' in names: return c
        if writes(c, reg) or c.address in labels: return None
        nxt = c; c = labels.before(c.address)
        if nxt.address in labels: return None
    return None


def index_values(ins, labels, reg, at, depth=0, seen=frozenset(), memo=None, entry=None):
    """the values the register can hold on reaching address `at`, as a set of 32-bit ints, from the code on every
    path into it; None when a path does not bound it. Walking back over instructions that do not write it:
      `and reg, imm`                         -> 0..imm
      reached by `jb X` after `cmp reg, N`   -> 0..N-1;  `jbe` -> 0..N
      reached by `jb X` after `sub reg, N`   -> the borrow: reg was 0..N-1, now 2**32-N .. 2**32-1
      fall-through of `ja` after `cmp reg, N` -> 0..N
    A point other code jumps to needs every predecessor bounded (union); an unconditional jump or ret before it is
    not a predecessor."""
    if memo is None: memo = {}
    if (reg, at) in memo: return memo[(reg, at)]
    if depth > 40 or at in seen: return None
    seen = seen | {at}
    memo[(reg, at)] = None                         # a cycle back here bounds nothing new
    out = set()
    preds = []                                   # (edge kind, branch instruction or None, previous address)
    for b in labels.get(at, ()):
        preds.append(('taken', ins[b]))
    p = labels.before(at)
    if p is not None and not (p.mnemonic == 'jmp' or p.mnemonic.startswith('ret')):
        preds.append(('fall', p))
    if not preds:
        # nothing reaches here but the function's entry (unknown values) or nothing at all (unreachable: no values)
        return None if at == entry else set()
    for kind, q in preds:
        mn = q.mnemonic
        if mn.startswith('j') and mn != 'jmp':
            c = flag_setter(labels, q, reg)
            taken = kind == 'taken' and q.operands[0].type == X86_OP_IMM and q.operands[0].imm == at
            if c is not None and len(c.operands) == 2 and c.operands[0].type == X86_OP_REG \
                    and c.reg_name(c.operands[0].reg) == reg and c.operands[1].type == X86_OP_IMM:
                N = c.operands[1].imm
                if N > 1024: return None
                if c.mnemonic == 'cmp' and taken and mn == 'jb': out |= set(range(N)); continue
                if c.mnemonic == 'cmp' and taken and mn == 'jbe': out |= set(range(N + 1)); continue
                if c.mnemonic == 'cmp' and not taken and mn == 'ja': out |= set(range(N + 1)); continue
                if c.mnemonic == 'sub' and taken and mn == 'jb': out |= {(v - N) & 0xFFFFFFFF for v in range(N)}; continue
            # a branch that says nothing about reg: keep walking back from the branch
            if writes(q, reg): return None
            v = index_values(ins, labels, reg, q.address, depth + 1, seen, memo, entry)
        else:
            if writes(q, reg):
                o = q.operands
                full = len(o) == 2 and o[0].type == X86_OP_REG and q.reg_name(o[0].reg) == reg
                if full and q.mnemonic == 'and' and o[1].type == X86_OP_IMM and o[1].imm <= 1024:
                    out |= set(range(o[1].imm + 1)); continue
                if full and q.mnemonic == 'mov' and o[1].type == X86_OP_IMM:
                    out.add(o[1].imm & 0xFFFFFFFF); continue
                if full and q.mnemonic == 'mov' and o[1].type == X86_OP_REG and q.reg_name(o[1].reg) in \
                        ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp'):     # the source register's values there
                    v = index_values(ins, labels, q.reg_name(o[1].reg), q.address, depth + 1, seen, memo, entry)
                    if v is None: return None
                    out |= v; continue
                one = len(o) == 1 and o[0].type == X86_OP_REG and q.reg_name(o[0].reg) == reg
                if (one and q.mnemonic == 'neg') or (full and q.mnemonic in ('add', 'sub') and o[1].type == X86_OP_IMM):
                    v = index_values(ins, labels, reg, q.address, depth + 1, seen, memo, entry)
                    if v is None: return None
                    if q.mnemonic == 'neg': out |= {(-x) & 0xFFFFFFFF for x in v}
                    else: out |= {(x + (o[1].imm if q.mnemonic == 'add' else -o[1].imm)) & 0xFFFFFFFF for x in v}
                    continue
                if full and q.mnemonic == 'and':            # x & y <= x (unsigned): bounded by reg's values before
                    v = index_values(ins, labels, reg, q.address, depth + 1, seen, memo, entry)
                    if v is None or max(v) > 1024: return None
                    out |= set(range(max(v) + 1)); continue
                return None
            v = index_values(ins, labels, reg, q.address, depth + 1, seen, memo, entry)
        if v is None: return None
        out |= v
    memo[(reg, at)] = out
    return out


def reads_st0_next(ins, a):
    """the first x87 instruction on the straight-line path from a reads ST0 without pushing first"""
    for _ in range(12):
        i = ins.get(a)
        if i is None: return False
        mn = i.mnemonic
        if mn == 'call' or mn.startswith(('j', 'ret', 'loop')): return False
        if mn.startswith('f') and mn not in CTRL: return mn not in PUSH
        a += i.size
    return False


def reachable(im, ins, f, sw, starts):
    """addresses reached from the entry by control flow (resolved switches included; calls return), never decoding
    past an unconditional jump, a return or an unresolved indirect jump"""
    R = set(); work = [f]
    while work:
        a = work.pop()
        if a in R: continue
        i = ins.get(a)
        if i is None:                      # code control flow reaches outside IDA's extents: decode it from the DLL
            i = im.insn(a)
            if i is None: continue
            ins[a] = i
        R.add(a); mn = i.mnemonic; ops = i.operands
        if mn.startswith('ret') or mn in ('int3', 'hlt', 'ud2'): continue
        if mn == 'jmp':
            if ops[0].type == X86_OP_IMM:
                t = ops[0].imm & 0xFFFFFFFF
                if t not in starts or t == f: work.append(t)
            else: work.extend(sw.get(a, ()))
            continue
        if mn.startswith(('j', 'loop')) and ops[0].type == X86_OP_IMM:
            t = ops[0].imm & 0xFFFFFFFF
            if t not in starts or t == f: work.append(t)
        work.append(a + i.size)
    return R


def switch_targets_x(im, ins, labels, prev, jmp, entry=None):
    """sp_solve.switch_targets, else the targets for the index values the paths into the jump establish
    (index_values); entries are read from the table, every one of them code"""
    t = switch_targets(im, list(prev), jmp, labels)
    if t is not None: return t
    o = jmp.operands[0]
    if o.type != X86_OP_MEM or o.mem.scale != 4 or not o.mem.index or o.mem.base: return None
    vals = index_values(ins, labels, jmp.reg_name(o.mem.index), jmp.address, entry=entry)
    if vals is None or len(vals) > 1024: return None
    ts = [im.u32((o.mem.disp + 4 * v) & 0xFFFFFFFF) for v in sorted(vals)]
    # an entry outside the image is no DLL code: the index that selects it is one no execution that does not fault
    # takes (the bound walk does not see every test on the way, e.g. memcpy's `test edi, 3`)
    return [t for t in ts if im.base <= t < im.base + len(im.img)]


def main(game, extf, out, passes=12):
    im = Image(game)
    ext = {int(k, 16): v for k, v in json.load(open(extf)).items()}
    D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
    vc = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'vcalls.json'))).items() if not k.startswith('_')}
    starts = set(ext)
    est = {}                                   # the previous pass's verdicts: recursion is solved to a fixpoint
    sites = {}

    def run_pass():
        memo = {}; busy = set()

        def summary(f, edx=None):
            key = (f, edx)
            if key in memo: return memo[key]
            if key in busy:                    # inside its own recursion: the previous pass's verdict (optimistic
                return est.get(key, {'k': 0, 'r': 0, 'm': 0, 'u': False})   # first), checked by the fixpoint
            busy.add(key)
            res = analyse(f, edx, record=(edx is None))
            busy.discard(key); memo[key] = res
            return res

        def unknown(d, lo):
            return d == 0 and lo == 0

        def effect(t, d, lo, edx, what):
            """(k, r, m, u) of a call or tail jump to t (None: a target the code does not fix), or a reason"""
            if t is not None and t in starts:
                s = summary(t, edx)
                if 'why' in s: return ('%s 0x%08X' % (what, t), s.get('root', s['why']))
                if s['u'] and not unknown(d, lo):
                    return '%s 0x%08X, whose x87 return is not known, with the x87 stack not empty' % (what, t)
                return s['k'], s['r'], s['m'], s['u']
            if not unknown(d, lo): return '%s with the x87 stack not empty' % what
            return 0, 0, 0, True

        def analyse(f, edx0, record):
            # a call whose x87 return is not known, at an empty x87 stack: what the caller then reads below its own
            # values can only be that call's float return (ABI): the function is analysed again with that call
            # returning one in ST0, and the verdict stands only if that analysis is consistent
            fsites = set()
            for _ in range(10):
                res, retry = analyse1(f, edx0, record, fsites)
                if retry is None: return res
                fsites.add(retry)
            return {'why': 'too many float returns inferred', 'root': 'too many float returns inferred'}

        def analyse1(f, edx0, record, fsites):
            retry = None
            ins = im.code(ext[f])
            if f not in ins: return {'why': 'no code at entry'}, None
            labels = labels_of(ins)
            # switch targets are predecessors too (a table jump into the middle of a block), and only code reachable
            # from the entry is a predecessor (bytes decoded inside a table are not): reachability and switch
            # resolution are iterated together to a fixpoint
            sw = {}; lab0 = labels
            for _ in range(8):
                R = reachable(im, ins, f, sw, starts)
                labels = labels_of({x: ins[x] for x in R if x in ins})
                for ja, ts in sw.items():
                    for t in ts: labels.setdefault(t, []).append(ja)
                grew = False
                for ja in sorted(R):
                    ji = ins.get(ja)
                    o0 = ji.operands[0] if ji is not None and ji.mnemonic == 'jmp' and ji.operands else None
                    if ja in sw or o0 is None or o0.type != X86_OP_MEM or o0.mem.scale != 4 or not o0.mem.index or o0.mem.base: continue
                    pv = []; q = ji
                    for _k in range(SWITCH_RUN):
                        q = labels.before(q.address)
                        if q is None: break
                        pv.insert(0, q)
                    ts = switch_targets_x(im, ins, labels, pv, ji, f)
                    if ts is None: continue
                    sw[ja] = ts; grew = True
                if not grew: break
            # state: address, depth, lowest slot touched, EDX constant, fence (a call whose x87 return is not known
            # happened at depth 0 with nothing touched: touching a slot below 0 now would read that return)
            work = [(f, 0, 0, edx0, False, ())]; seen = set()
            m = 0; rets = set(); wild = set(); why = None; mysites = {}
            while work and why is None:
                a, d, lo, edx, fence, prev = work.pop()
                st = (a, d, lo, edx, fence)
                if st in seen: continue
                if len(seen) > 400000: why = 'too many states'; break
                seen.add(st)
                i = ins.get(a)
                if i is None:
                    i = im.insn(a)
                    if i is None: why = 'no instruction at 0x%08X' % a; break
                    ins[a] = i
                mn = i.mnemonic; ops = i.operands; nd = d; nlo = lo; nfence = fence
                if mn == 'fnstenv':
                    j = env_pair(ins, i)
                    if j is None: why = 'fnstenv at 0x%08X without a TOP-keeping fldenv' % a; break
                    work.append((j, d, lo, edx, fence, ())); continue
                if mn in REFUSE: why = '%s at 0x%08X' % (mn, a); break
                if mn.startswith('f') and mn not in CTRL:
                    regs = [o for o in ops if o.type == X86_OP_REG and i.reg_name(o.reg).startswith('st')]
                    for o in regs: nlo = min(nlo, d - 1 - int(i.reg_name(o.reg)[3:-1]))
                    if mn in PUSH: nd = d + 1
                    elif mn in PUSH_AFTER: nlo = min(nlo, d - 1); nd = d + 1
                    else:
                        if not regs or mn in USES_ST1 or mn in POP1 or mn in POP2: nlo = min(nlo, d - 1)
                        if mn in USES_ST1: nlo = min(nlo, d - 2)
                        if mn in POP1: nd = d - 1
                        elif mn in POP2: nlo = min(nlo, d - 2); nd = d - 2
                elif mn == 'call':
                    o = ops[0]; t = o.imm & 0xFFFFFFFF if o.type == X86_OP_IMM else None
                    what = 'callee' if t is not None else (
                        'import %s' % im.iat[o.mem.disp] if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index
                        and o.mem.disp in im.iat else 'indirect call at 0x%08X' % a)
                    if t is None and a in vc and vc[a].get('ret') == 'st0' and unknown(d, lo): e = (0, 1, 1, False)
                    else: e = effect(t, d, lo, edx, what)
                    if not isinstance(e, str) and len(e) == 4 and e[3] and e[:2] == (0, 0) and (a in fsites or reads_st0_next(ins, a + i.size)):
                        # at an empty x87 stack (the only place such a call is taken) the ABI leaves nothing but a float
                        # return in ST0: the caller's own first x87 instruction reading ST0 shows the call returned one
                        e = (0, 1, max(e[2], 1), False)
                    if isinstance(e, (str, tuple)) and len(e) != 4: why = e; break
                    k, r, mm, u = e
                    if record: mysites[a] = {'callee': '0x%08X' % t if t is not None else None, 'd': d, 'low': lo,
                                             'k': k, 'r': r, 'u': u}
                    nlo = min(lo, d - k); m = max(m, d + mm); nd = d - k + r
                    if u: nfence = a                       # the latest call whose x87 return is not known
                    edx = None                         # EDX is the callee's to lose (every MSVC convention)
                if nfence and nlo < 0:
                    if nfence in fsites or nfence is True:
                        why = 'reads the x87 stack after a call whose x87 return is not known (0x%08X)' % a; break
                    retry = nfence; why = 'retry'; break
                if nd > 8 or nd - nlo > 8: why = 'more than 8 x87 values at 0x%08X' % a; break
                m = max(m, nd)
                if mn != 'call':
                    if mn == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'edx' and ops[1].type == X86_OP_IMM:
                        edx = ops[1].imm & 0xFFFFFFFF
                    elif edx is not None:
                        try:
                            if any(i.reg_name(x) in ('edx', 'dx', 'dl', 'dh') for x in i.regs_access()[1]): edx = None
                        except capstone.CsError: edx = None
                nxt = []
                if mn in ('ret', 'retn'):
                    if nfence is not False and nd == 0 and nlo == 0:
                        # nothing read since a call whose x87 return is not known: the function returns what it left
                        wild.add((nd, nlo))
                    else: rets.add((nd, nlo, nfence))
                    continue
                if mn == 'jmp':
                    o = ops[0]
                    if o.type == X86_OP_IMM and (o.imm & 0xFFFFFFFF) not in starts: nxt = [o.imm & 0xFFFFFFFF]
                    elif o.type == X86_OP_MEM and o.mem.scale == 4 and o.mem.index and not o.mem.base:
                        ts = sw.get(a)
                        if ts is None: why = 'unresolved switch at 0x%08X' % a; break
                        nxt = ts
                    elif f == CRT_DISPATCH and o.type == X86_OP_MEM and o.mem.base and not o.mem.index and not o.mem.disp:
                        ts = crt_dispatch_targets(im, edx0)
                        if ts is None: why = 'CRT dispatch with no table (EDX not known)'; break
                        for t in ts:
                            if t not in starts: work.append((t, nd, nlo, edx, nfence, ())); continue
                            e = effect(t, nd, nlo, None, 'CRT dispatch target')
                            if isinstance(e, (str, tuple)) and len(e) != 4: why = e; break
                            k, r, mm, u = e; m = max(m, nd + mm); rets.add((nd - k + r, min(nlo, nd - k), nfence or u))
                        continue
                    else:                              # a tail call: direct, through the import table or a pointer
                        t = o.imm & 0xFFFFFFFF if o.type == X86_OP_IMM else None
                        what = 'tail call' if t is not None else (
                            'tail jump to import %s' % im.iat[o.mem.disp] if o.type == X86_OP_MEM and not o.mem.base
                            and not o.mem.index and o.mem.disp in im.iat else 'indirect tail jump at 0x%08X' % a)
                        e = effect(t, nd, nlo, edx, what)
                        if isinstance(e, (str, tuple)) and len(e) != 4: why = e; break
                        k, r, mm, u = e; m = max(m, nd + mm)
                        if t is None or (u and (k, r) == (0, 0)):
                            # through a pointer (or to a function whose x87 return is not known): it returns what the
                            # target returns, nothing or a float in ST0 -- fixed below by the function's other returns
                            wild.add((nd, min(nlo, nd)))
                        else: rets.add((nd - k + r, min(nlo, nd - k), nfence or u))
                        continue
                elif mn.startswith(('j', 'loop')):
                    nxt = [a + i.size] + ([ops[0].imm & 0xFFFFFFFF] if ops[0].type == X86_OP_IMM else [])
                elif mn in ('int3', 'hlt', 'ud2'):
                    continue
                else:
                    nxt = [a + i.size]
                np = (tuple(prev) + (i,))[-5:]
                for n in nxt:
                    if n in starts and n != f:      # a (conditional) tail call into another function's entry
                        e = effect(n, nd, nlo, edx, 'tail branch')
                        if isinstance(e, (str, tuple)) and len(e) != 4: why = e; break
                        k, r, mm, u = e; m = max(m, nd + mm); rets.add((nd - k + r, min(nlo, nd - k), nfence or u))
                        continue
                    work.append((n, nd, nlo, edx, nfence, np if n == a + i.size else ()))
            if why == 'retry': return None, retry
            if why:   # a refusal names its own step and the root cause (a callee's refusal: that callee's root)
                return ({'why': why[0], 'root': why[1]} if isinstance(why, tuple) else {'why': why, 'root': why}), None
            if not rets and not wild: return {'why': 'no return'}, None
            # a C function has one return type (ISO C 6.9.1): a tail jump whose target's x87 return is not known
            # returns what the function's other returns return; with no other return, nothing is known (u)
            k = max(0, -min([lo for _, lo, _ in rets] + [lo for _, lo in wild]))
            known = {d + k for d, _, _ in rets}
            if wild:
                if len(known) == 1:
                    r0 = next(iter(known))
                    for d, lo in wild:
                        if d + k not in (r0, r0 - 1): return {'why': 'a pointer tail jump at depth %d against returns of %d' % (d + k, r0)}, None
                        rets.add((r0 - k, lo, True))
                else:
                    for d, lo in wild: rets.add((d, lo, True))
            rs = {d + k for d, _, _ in rets}
            if len(rs) != 1: return {'why': 'returns with different x87 depths %s' % sorted({d for d, _, _ in rets})}, None
            if record:
                for s_, v in mysites.items():
                    v['func'] = '0x%08X' % f
                    if s_ in fsites: v['float_shown_by_caller'] = True
                    sites['0x%08X' % s_] = v
            return {'k': k, 'r': rs.pop(), 'm': m, 'u': any(fz is not False for _, _, fz in rets)}, None

        out = {}
        for f in sorted(ext): out[(f, None)] = summary(f)
        for key, v in memo.items(): out[key] = v
        return out

    for n in range(passes):
        sites.clear()
        new = run_pass()
        if new == est: break
        diff = [k for k in set(new) | set(est) if new.get(k) != est.get(k)]
        print('pass', n + 1, 'changed', len(diff))
        est.clear(); est.update(new)
    else:
        print('no fixpoint after', passes, 'passes')
    funcs = {'0x%08X' % f: v for (f, e), v in sorted(est.items(), key=lambda x: (x[0][0], x[0][1] or 0)) if e is None}
    json.dump({'_doc': __doc__.split('\n\n')[0], 'passes': n + 1, 'funcs': funcs, 'sites': dict(sorted(sites.items()))},
              open(out, 'w'), indent=0)
    ok = [v for v in funcs.values() if 'why' not in v]
    import collections
    print('passes', n + 1, 'functions', len(funcs), 'with a verdict', len(ok), 'refused', len(funcs) - len(ok),
          '; u (return may hold an unknown x87 value):', sum(v['u'] for v in ok))
    print('verdicts (k, r):', collections.Counter((v['k'], v['r']) for v in ok).most_common(8))
    return funcs, sites


def selftest(game, extf, out):
    funcs, sites = main(game, extf, out)
    im = Image(game)
    ext = json.load(open(extf))
    fp = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'fpret.json')))
    bad = []
    # read from the instructions (see HISTORY, 25 Oct): __ftol pops one value and leaves none; the fmod intrinsic's
    # normal-operand entry `fxch st(1); fprem ...; fstp st(1); ret` takes two and leaves one; the fmod stub itself has
    # no verdict, its error path calling RaiseException with the operands on the x87 stack
    for a, kr in {'0x10311C2C': (1, 0), '0x103138C4': (2, 1)}.items():
        v = funcs.get(a, {})
        if (v.get('k'), v.get('r')) != kr: bad.append((a, v, kr))
    if 'why' not in funcs.get('0x103138BA', {}): bad.append(('0x103138BA', funcs.get('0x103138BA'), 'refused'))
    # agreement with fpret.json (an independent depth count). It has two blind spots, each checked here from the
    # code: a tail jump to a function that pops a value (it counts the jump as returning the value), and a call to a
    # function returning a float it has no verdict for (it counts the call as pushing nothing)
    def last_jump(a):
        last = None
        for s_, e in ext[a]:
            for x in im.md.disasm(im.img[s_ - im.base:e - im.base], s_): last = x
        return last
    unexplained = []
    for a, v in fp.items():
        A = '0x%08X' % int(a, 16); s = funcs.get(A)
        if not s or 'why' in s: continue
        ok = {'float': (s['k'], s['r']) == (0, 1), 'none': (s['k'], s['r']) == (0, 0) or s['k'] > 0,
              'consumes': s['r'] - s['k'] == -1}.get(v, True)   # fpret: the first op pops, net -1
        if ok: continue
        why = None
        lj = last_jump(a)
        if v == 'float' and lj is not None and lj.mnemonic == 'jmp' and lj.op_str.startswith('0x'):
            t = funcs.get('0x%08X' % int(lj.op_str, 16), {})
            if t.get('k') == 1 and t.get('r') == 0: why = 'tail jump to a consumer'
        if v == 'none':
            cs = [c for c in sites.values() if c['func'] == A and c['r'] >= 1 and c['callee']
                  and fp.get('0x%x' % int(c['callee'], 16)) != 'float']
            tails = lj is not None and lj.mnemonic == 'jmp' and lj.op_str.startswith('0x') and \
                funcs.get('0x%08X' % int(lj.op_str, 16), {}).get('r', 0) >= 1
            if cs or tails: why = 'calls a float-returning function fpret.json does not list'
        if why is None: unexplained.append((A, v, s))
    print('selftest: known', 'OK' if not bad else bad, '; disagreements with fpret.json not explained by its '
          'blind spots:', len(unexplained), unexplained[:5])
    return not bad and not unexplained


if __name__ == '__main__':
    a = [x for x in sys.argv[1:] if not x.startswith('--')]
    if '--selftest' in sys.argv: sys.exit(0 if selftest(*a) else 1)
    main(*a)
