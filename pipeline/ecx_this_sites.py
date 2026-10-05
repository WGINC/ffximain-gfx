"""ecx_this_sites.py -- virtual calls where the caller passes the object in ECX, read from the caller's code.

At `call [R+4k]`, if R was loaded from [O+0] (the object's vtable pointer) and ECX holds that same object O at
the call, the original enters the callee with ECX = the object. When the object is also pushed as the call's
first stack argument (`push ecx` / `push O`, the COM convention), ECX is only where the compiler kept it, and the
site is marked `pushed`. A reconstruction that does not pass the object in ECX does not reproduce the original
at such a call: the callee runs with whatever ECX holds (pol.exe crashed in CXiTimer::GetFps, `fld [ecx+8]`,
called from a reconstruction of 0x10011A00 that typed the call without `this`).

The analysis follows the function's control flow. Each register holds a symbol; `mov r, r2` copies r2's
symbol; `mov r, [B+d]` (B not ESP, no index) gives a symbol that records it was loaded from [sym(B)+d]; any other
write to a register (vcall_resolve.written_exact, partial registers included) gives a new symbol. A call gives
EAX, ECX and EDX new symbols and forgets the pushes since the previous call. Where paths join, a register keeps
its symbol only when every path into the join gives it the same one (otherwise it gets a new symbol for the
join), and the pushes only when every path has the same ones, so a claim made after a join holds on every path.
Code reached by no direct edge from the entry (a switch-table target) starts with nothing known. A site is
listed only when the vtable pointer's origin is known, so every claim is about the value the original's ECX
holds.

    FFXI_DLL=... python3 ecx_this_sites.py data/ecx_this_sites.json
Output {"0xCALL": {"func", "slot", "ecx": true|false, "pushed": true|false}}; "ecx": ECX holds the object at the
call; "pushed": the object is also the first stack argument."""
import json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from vcall_resolve import written_exact, r32

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
GPR = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp', 'esp')


def sites(ins):
    """ins: {address: capstone insn} of one function -> {call address: record}"""
    order = sorted(ins)

    def succ(a):
        i = ins[a]; m = i.mnemonic; ops = i.operands
        if m.startswith('ret'): return []
        if m.startswith('j') or m == 'loop':
            if not (ops and ops[0].type == X86_OP_IMM): return []                  # an indirect jump
            t = [ops[0].imm] if ops[0].imm in ins else []
            return t if m == 'jmp' else t + ([a + i.size] if a + i.size in ins else [])
        return [a + i.size] if a + i.size in ins else []

    preds = {a: [] for a in order}
    for a in order:
        for b in succ(a): preds[b].append(a)
    # symbols are named by where they are made, so every pass over a loop makes the same ones
    loaded = {}

    def step(a, st):
        """the state after instruction a, given the state before it: (registers {reg: sym}, pushes tuple)"""
        i = ins[a]; m = i.mnemonic; ops = i.operands
        sym, pushes = dict(st[0]), st[1]
        get = lambda r: sym[r]
        if m == 'call':
            for r in ('eax', 'ecx', 'edx'): sym[r] = ('call', a, r)
            return sym, ()
        if m == 'push':
            if ops and ops[0].type == X86_OP_REG and r32(i.reg_name(ops[0].reg)) in GPR:
                return sym, pushes + (get(r32(i.reg_name(ops[0].reg))),)
            return sym, pushes + (('push', a),)
        if m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) in GPR:
            d = i.reg_name(ops[0].reg); s = ops[1]
            if s.type == X86_OP_REG and i.reg_name(s.reg) in GPR:
                sym[d] = get(i.reg_name(s.reg))
            elif s.type == X86_OP_MEM and s.mem.base and not s.mem.index and s.mem.segment == 0 \
                    and i.reg_name(s.mem.base) in GPR and i.reg_name(s.mem.base) != 'esp':
                v = ('ld', a); loaded[v] = (get(i.reg_name(s.mem.base)), s.mem.disp); sym[d] = v
            else:
                sym[d] = ('def', a, d)
            return sym, pushes
        for r in written_exact(i):
            if r in GPR: sym[r] = ('def', a, r)
        return sym, pushes

    def join(a, states):
        regs = {}
        for r in set().union(*(s[0] for s in states)):
            v = {s[0].get(r) for s in states}
            regs[r] = v.pop() if len(v) == 1 and None not in v else ('phi', a, r)
        p = {s[1] for s in states}
        return regs, (p.pop() if len(p) == 1 else (('phi', a, 'push'),))

    out_st = {}
    entries = {a for a in order if not preds[a]}                  # the entry, and code only an indirect jump reaches
    before = {}
    work = sorted(entries, reverse=True)
    for a in entries: before[a] = ({r: ('in', a, r) for r in GPR}, ())
    while work:
        a = work.pop()
        st = before[a] if a in entries else join(a, [out_st[p] for p in preds[a] if p in out_st])
        before[a] = st
        new = step(a, st)
        if out_st.get(a) == new: continue
        out_st[a] = new
        for b in succ(a):
            if b not in work: work.append(b)
    out = {}
    for a in order:
        i = ins[a]
        if i.mnemonic != 'call' or a not in before: continue
        o = i.operands[0] if i.operands else None
        if o is None or o.type != X86_OP_MEM or not o.mem.base or o.mem.index or o.mem.disp < 0 or o.mem.disp % 4: continue
        sym, pushes = before[a]
        src = loaded.get(sym.get(r32(i.reg_name(o.mem.base))))
        if src is not None and src[1] == 0:
            obj = src[0]
            out['0x%08X' % a] = {'slot': o.mem.disp // 4, 'ecx': sym.get('ecx') == obj,
                                 'pushed': bool(pushes) and pushes[-1] == obj}
    return out


def main(out_path):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    res = {}
    for k, e in ext.items():
        ins = im.code(e)
        for a, r in sites(ins).items():
            r['func'] = '0x%08X' % int(k, 16); res[a] = r
    res = dict(sorted(res.items()))
    n_ecx = sum(r['ecx'] for r in res.values()); n_this = sum(r['ecx'] and not r['pushed'] for r in res.values())
    res['_summary'] = {'sites': len(res), 'ecx_holds_object': n_ecx, 'ecx_holds_object_not_pushed': n_this}
    json.dump(res, open(out_path, 'w'), indent=0)
    print(json.dumps(res['_summary']))


if __name__ == '__main__':
    main(sys.argv[1])
