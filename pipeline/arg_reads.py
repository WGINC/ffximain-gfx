"""arg_reads.py -- which bytes of each stack argument a function's own code reads, from its code.

A caller passes a byte argument as `push eax` with AL set: the upper 24 bits are whatever EAX held. The gate
logs every stubbed call's arguments as dwords, so two callers that pass the same byte compare unequal when their
leftover bits differ. The bits the callee never reads are not part of the call, and this gives, per function,
the mask of bytes it reads in each argument slot. gfx_cases.py uses it as the stub's argument masks.

Stack depths are sp_solve.insn_depths: exact, every unknown pop solved. At an instruction with depth d, the
operand [esp + x] is entry ESP + x - d; with EBP a copy of ESP at depth e, [ebp + x] is entry ESP + x - e. Entry
ESP + 4 + 4k is argument k. A memory operand there that the instruction reads (capstone's operand access)
reads those bytes; a write alone does not.

A function is left out, because its argument reads cannot all be seen, when:
  * an instruction with an ESP- or EBP-based memory operand has no exact depth, or EBP is not a copy of ESP there;
  * an argument's address is taken (`lea` of entry ESP + 4 or above), or ESP or EBP is copied into another
    register other than `mov ebp, esp`;
  * it leaves by a jump (conditional or not) to another function at a depth that is not exact or not 0, or to a
    function that is itself left out (or reached again through such jumps: a cycle is not followed).
A jump to another function at depth 0 (ESP at the return address, as at the entry) passes the frame: the target
reads the same argument slots, so its reads count as the function's, except bytes of a slot the instructions
straight before the jump store (each ending where the next begins, no jump landing after it, no call between),
which the target reads as stored, not as passed.

A load of a whole argument into a 32-bit register (`mov r32, dword ptr [slot]`) reads the four bytes from memory,
but what the code then does with them decides which of them are read. The register is followed from the next
instruction over the function's control flow (vcall_resolve.succs), bit by bit (byte_flow.py, its may-walk): a
byte counts as read where data made from it is examined or escapes (stored, pushed, an instruction byte_flow does
not model). Where the walk does not follow the value, every byte still held counts as read: a call through a
pointer, a jump out of the function, an indirect jump, an address outside the function. A direct call or an import
call is followed as caller_reads.py follows one (after_call: the callee's reads of ECX/EDX at entry, the registers
it never writes or restores). At a `ret`, the bytes go to the function's callers: those in EAX count as read as
caller_reads.json says its callers read EAX; those in ECX or EDX only where some caller reads the register after
the call (regs_read_after: every caller known, a fixpoint over callers that return it on); in any other register
or the flags, all. 0x100EB490 loads its second argument into ECX, reads CL, and on one path returns with the rest
still in ECX; some callers return it on to callers that are not all known (virtual methods), so all four bytes stay
counted.

    FFXI_DLL=... python3 arg_reads.py OUT.json
Output {"0xFUNC": [mask of argument 0, mask of argument 1, ...]}, up to the last argument read; listed only
where some mask is not 0xFFFFFFFF. A mask of 0 means the code never reads that argument."""
import json, os, struct, sys
from capstone import CS_AC_READ, CS_AC_WRITE
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V
import byte_flow as BF

NSLOT = 16


def arg_bytes(i, op, d):
    """the argument-area bytes (offsets past the return address) an ESP/EBP-based memory operand covers, or None"""
    base = i.reg_name(op.mem.base)
    if base == 'esp': off = op.mem.disp - d[0]
    else:
        if d[1] is None: return None
        off = op.mem.disp - d[1]
    return range(off - 4, off - 4 + op.size)


def stored_before(ins, labels, jmp, depths):
    """bytes of the argument area the straight-line run ending at the jump stores, nothing landing after them"""
    out = set(); x = jmp
    while True:
        if x.address in labels: break
        p = labels.before(x.address)
        if p is None or p.mnemonic == 'call' or p.mnemonic.startswith(('j', 'ret', 'loop')): break
        d = depths.get(p.address)
        if d is None: break
        for op in p.operands:
            if op.type == X86_OP_MEM and op.mem.base and not op.mem.index and p.reg_name(op.mem.base) in ('esp', 'ebp') \
                    and (op.access & CS_AC_WRITE) and not (op.access & CS_AC_READ) and p.mnemonic == 'mov':
                r = arg_bytes(p, op, d)
                if r is not None: out.update(b for b in r if b >= 0)
        x = p
    return out


def follow(im, ins, labels, start, reg, at_ret):
    """the bytes (0-3) of a value held in reg at start that the code from there reads (byte_flow's may-walk);
    at_ret(state) gives the bytes a `ret` reads"""
    got = set()
    state_at, work = {start: BF.entry_state(reg)}, [start]
    while work:
        x = work.pop(); st = state_at[x]
        j = ins.get(x)
        if j is None: got |= held(st); continue
        m, o = j.mnemonic, j.operands
        if m == 'call':
            t = o[0]
            if t.type == X86_OP_MEM:                    # the bits used to reach the callee: an address
                for r_, _ in BF.mem_regs(j): got |= st.srcs(r_)
            elif t.type == X86_OP_REG and j.reg_name(t.reg) in BF.PART: got |= st.srcs(BF.PART[j.reg_name(t.reg)][0])
            imp = (t.type == X86_OP_MEM and not t.mem.base and not t.mem.index and (t.mem.disp & 0xFFFFFFFF) in IAT) \
                or (t.type == X86_OP_IMM and import_thunk(im, t.imm))
            if not imp and not (t.type == X86_OP_IMM and t.imm in FACTS['starts']):
                got |= held(st); continue               # a call through a pointer: not followed
            nst, r = after_call(st, None if imp else t.imm)
            got |= r | set(st.flags)
            nx = [x + j.size]
            if not nst.any(): continue
            for n in nx:
                if n not in ins: got |= held(nst); continue
                old = state_at.get(n)
                new = nst if old is None else BF.join(old, nst)
                if old is None or new.key() != old.key(): state_at[n] = new; work.append(n)
            continue
        if m.startswith('ret'): got |= at_ret(st); continue
        if m == 'jmp' and o and o[0].type == X86_OP_IMM and o[0].imm not in ins: got |= held(st) | set(st.flags); continue
        if m.startswith('j') or m.startswith('loop'):
            try: reads_flags = 'eflags' in {j.reg_name(r) for r in j.regs_access()[0]}
            except Exception: reads_flags = True
            if reads_flags: got |= set(st.flags)
            if m in ('jecxz', 'jcxz') or m.startswith('loop'): got |= st.srcs('ecx')
            if o and o[0].type != X86_OP_IMM: got |= held(st); continue     # an indirect jump: not followed
            nst = st
        else:
            nst, ex, es = BF.step(j, st)
            got |= es
        nx = V.succs(im, ins, x, j, labels)
        if nx is None: got |= held(nst); continue
        if not nst.any() and not nst.flags: continue
        for n in nx:
            if n not in ins: got |= held(nst); continue
            old = state_at.get(n)
            new = nst if old is None else BF.join(old, nst)
            if old is None or new.key() != old.key(): state_at[n] = new; work.append(n)
    return got


IAT, FACTS, _THUNK = {}, {'starts': set()}, {}


def load_facts(im, starts):
    """the callee facts after_call uses (data/): ecx_use, edx_use (a callee's reads of ECX / EDX at entry),
    reg_keeps (registers it never writes), reg_saved (callee-saved ones it restores), entry_regs (registers it
    reads at entry)"""
    IAT.update(S.imports(im, os.environ['FFXI_DLL']))
    dj = lambda n: json.load(open(os.path.join(P, '..', 'data', n)))
    FACTS.update(starts=starts, ecx=dj('ecx_use.json'), edx=dj('edx_use.json'),
                 keeps={int(k, 16): set(v) for k, v in dj('reg_keeps.json').items()},
                 saved={int(k, 16): set(v) for k, v in dj('reg_saved.json').items()},
                 entry={int(k, 16): set(v) for k, v in dj('entry_regs.json').items()})


def import_thunk(im, t):
    """whether t is a thunk that is one `jmp dword ptr [import slot]`"""
    if t not in _THUNK:
        from capstone import Cs, CS_ARCH_X86, CS_MODE_32
        md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
        try:
            i = next(md.disasm(bytes(im.img[t - im.base:t - im.base + 8]), t)); o = i.operands
            _THUNK[t] = i.mnemonic == 'jmp' and len(o) == 1 and o[0].type == X86_OP_MEM and not o[0].mem.base \
                and not o[0].mem.index and (o[0].mem.disp & 0xFFFFFFFF) in IAT
        except StopIteration: _THUNK[t] = False
    return _THUNK[t]


def after_call(st, t):
    """(the state after a direct call of t, or of an import when t is None; the source bytes the call reads), as
    caller_reads.py's after_callee: EAX is read (an argument in a register is not excluded) and not held after;
    ECX/EDX are read where the callee's code reads them at entry or that is not known (ecx_use / edx_use not
    False), and held after only where it never writes them (reg_keeps); EBX, ESI, EDI, EBP are held where the
    callee restores them (reg_saved), read where it reads them at entry (entry_regs), else read. An import reads
    no register (the SDK's stdcall/cdecl prototypes take their arguments on the stack), may keep ECX/EDX
    (counted read: not followed) and keeps EBX, ESI, EDI, EBP (Microsoft's x86 conventions)"""
    r, regs = set(), {}
    for reg, v in st.regs.items():
        if reg == 'eax':
            if t is not None: r |= st.srcs(reg)
        elif reg in ('ecx', 'edx'):
            if t is None: r |= st.srcs(reg); continue
            if FACTS[reg].get('0x%08X' % t) is not False: r |= st.srcs(reg)
            if reg in FACTS['keeps'].get(t, ()): regs[reg] = v
        elif t is None: regs[reg] = v
        elif reg in FACTS['saved'].get(t, ()):
            regs[reg] = v
            if reg in FACTS['entry'].get(t, ()): r |= st.srcs(reg)
        else: r |= st.srcs(reg)
    return BF.State(regs), r


def held(st):
    """every source byte some register (or the flags) may hold"""
    return (set().union(*[st.srcs(r) for r in st.regs]) if st.regs else set()) | set(st.flags)


EAX_READ = {0: 0, 1: 0xFF, 2: 0xFFFF}            # caller_reads.json: no byte, only AL, only AX


def ret_reads(f, eax_read, after):
    """at_ret for function f: the bytes a `ret` hands to f's callers that they read. EAX: the bytes caller_reads.json
    says f's callers read (all where it does not list f). ECX, EDX: none where no caller reads the register after
    the call (after); every other register and the flags: all, since no fact says the caller does not read them"""
    em = EAX_READ.get(eax_read.get('0x%08X' % f), 0xFFFFFFFF)
    def at_ret(st):
        out = set(st.flags)
        for r in st.regs:
            if r == 'eax': out |= st.srcs('eax', em)
            elif r in ('ecx', 'edx') and not after.get(f, {}).get(r, True): continue
            else: out |= st.srcs(r)
        return out
    return at_ret


def regs_read_after(im, code, starts, eax_read):
    """{f: {'ecx': bool, 'edx': bool}}: whether some caller of f reads that register's value as f leaves it, for the
    f whose callers are all known: every reference to f is a direct `call f` (an address of f anywhere else, a jump
    to it from another function, or the dword in the image: not all known). From each return address the
    caller's code is followed (follow()); the caller's own `ret` hands what it still holds to its callers
    (ret_reads with this fact for the caller), a fixpoint growing from nothing: a value that only goes round a
    cycle of calls and returns is read by nobody"""
    sites, taken = {}, set()
    for g, ins in code.items():
        for a, i in ins.items():
            for o in i.operands:
                v = o.imm & 0xFFFFFFFF if o.type == X86_OP_IMM else \
                    o.mem.disp & 0xFFFFFFFF if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index else None
                if v not in starts: continue
                if i.mnemonic == 'call' and o.type == X86_OP_IMM: sites.setdefault(v, []).append((g, a + i.size))
                elif not (i.mnemonic.startswith('j') and o.type == X86_OP_IMM and v in ins): taken.add(v)
    for v in starts:
        if im.img.find(struct.pack('<I', v)) != -1: taken.add(v)
    known = [f for f in starts if f not in taken]
    after = {f: {'ecx': False, 'edx': False} for f in known}
    callees = {}                                   # g -> the known f it calls (recomputed when after[g] changes)
    for f in known:
        for g, _ in sites.get(f, []): callees.setdefault(g, set()).add(f)
    labels, work, rounds = {}, set(known), 0
    while work:
        rounds += 1; nxt = set()
        for f in work:
            for reg in ('ecx', 'edx'):
                if after[f][reg]: continue
                for g, ra in sites.get(f, []):
                    if g not in labels: labels[g] = S.labels_of(code[g])
                    if follow(im, code[g], labels[g], ra, reg, ret_reads(g, eax_read, after)):
                        after[f][reg] = True; nxt |= callees.get(f, set()); break
        work = nxt
    print('callers\' register reads after a call: %d rounds' % rounds)
    return after


def reads_of(ea, ins, depths, im=None, at_ret=None):
    """([mask per slot], [(tail-jump target, argument bytes stored before it)]) or None"""
    masks = [0] * NSLOT; tails = []; labels = S.labels_of(ins)
    for a, i in ins.items():
        m, o = i.mnemonic, i.operands
        if (m == 'jmp' or m.startswith('j')) and o and o[0].type == X86_OP_IMM and o[0].imm not in ins:
            d = depths.get(a)
            if d is None or d[0] != 0: return None
            tails.append((o[0].imm, stored_before(ins, labels, i, depths)))
        for n, op in enumerate(o):
            if op.type == X86_OP_REG and i.reg_name(op.reg) in ('esp', 'ebp') and m in ('mov', 'lea', 'xchg') and n == 1:
                if not (m == 'mov' and o[0].type == X86_OP_REG and i.reg_name(o[0].reg) == 'ebp' and i.reg_name(op.reg) == 'esp'):
                    return None
            if op.type != X86_OP_MEM or not op.mem.base: continue
            base = i.reg_name(op.mem.base)
            if base not in ('esp', 'ebp'): continue
            if op.mem.index: return None
            d = depths.get(a)
            if d is None: return None
            r = arg_bytes(i, op, d)
            if r is None: return None
            off = r.start                                    # past the return address: argument area from 0
            if m == 'lea':
                if off >= 0: return None
                continue
            if off + op.size <= 0 or not (op.access & CS_AC_READ): continue
            if im is not None and m == 'mov' and n == 1 and op.size == 4 and off >= 0 and off % 4 == 0 and off // 4 < NSLOT \
                    and o[0].type == X86_OP_REG and i.reg_name(o[0].reg) in BF.REG32:
                for b in follow(im, ins, labels, a + i.size, i.reg_name(o[0].reg), at_ret): masks[off // 4] |= 0xFF << (8 * b)
                continue
            for b in range(max(off, 0), off + op.size):
                k = b // 4
                if k < NSLOT: masks[k] |= 0xFF << (8 * (b % 4))
    return masks, tails


def trim(masks):
    last = max((k for k in range(NSLOT) if masks[k]), default=-1)
    return masks[:last + 1]


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    purges = json.load(open(os.path.join(P, '..', 'data', 'purges.json')))
    iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    starts = {int(k, 16) for k in ext}
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    eax_read = json.load(open(os.path.join(P, '..', 'data', 'caller_reads.json')))
    load_facts(im, starts)
    after = regs_read_after(im, code, starts, eax_read)
    print('%d functions whose callers are all known; %d with no caller reading ECX after the call, %d EDX'
          % (len(after), sum(not v['ecx'] for v in after.values()), sum(not v['edx'] for v in after.values())))
    own = {}
    for k, e in ext.items():
        ea = int(k, 16)
        depths = S.insn_depths(im, ea, e, purges, starts, nr=nr, iat=iat)
        if not depths: continue
        r = reads_of(ea, code[ea], depths, im, ret_reads(ea, eax_read, after))
        if r is not None: own[ea] = r
    # tail jumps: a function is decided once every target is (cycles stay undecided)
    done = {ea: m for ea, (m, tails) in own.items() if not tails}
    changed = True
    while changed:
        changed = False
        for ea, (m, tails) in own.items():
            if ea in done or not all(t in done for t, _ in tails): continue
            m = list(m)
            for t, stored in tails:
                for k, tm in enumerate(done[t]):
                    keep = sum(0xFF << (8 * b) for b in range(4) if 4 * k + b not in stored)
                    m[k] |= tm & keep
            done[ea] = m; changed = True
    res, n = {}, len(done)
    for ea, m in done.items():
        r = trim(m)
        if any(x != 0xFFFFFFFF for x in r): res['0x%08X' % ea] = r
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions whose argument reads are all seen; %d read less than whole dwords of some argument' % (n, len(res)))


if __name__ == '__main__':
    main(*sys.argv[1:])
