"""reg_kept_audit.py -- EAX/ECX/EDX values the original keeps live across a call and then uses, read from the code.

Hex-Rays assumes a call clobbers EAX, ECX and EDX. The compiler did not always: where it knew the callee leaves a
register untouched, it kept a value there across the call and used it afterwards. Hex-Rays then shows that use as
a never-assigned `// ecx` or `// edx` variable, and a translation of it passes garbage. Two such defects were found
this way (0x1006AA30: the device in EDX across Gfx_CalcFVFStride; 0x10040670: the element in ECX across
0x1003FCE0); data/hx_patches has the corrections.

A function leaves a register untouched when none of its instructions writes any part of it
(vcall_resolve.written_exact) and every function it calls or tail-jumps to leaves it untouched too (a fixpoint);
an indirect call or jump makes it unknown, except a jump through a table whose index the two instructions before
it bound (`cmp r, N` / `ja`, falling through, no jump landing between them) with every entry inside the function.
Then, per gated function, over its control flow: a register's value is "kept" once it has survived a call to a
callee that leaves it untouched. A kept value is listed where it is used: an instruction that reads it (capstone
regs_access; `xor r, r` / `or r, -1` are not reads), or a direct call to a callee that reads that register on
entry (data/ecx_use.json, data/edx_use.json), or the `ret` of a function whose prototype returns a value (EAX). A
callee whose use "cannot tell" is not counted as a reader.

    FFXI_DLL=... python3 reg_kept_audit.py data/reg_kept_audit.json
Output {"sites": [{"function", "use", "register", "instruction", "defined", "kept_across", "kind"}], "_summary"};
kind (slot_kind): for `push REG`, "overwritten" (MSVC's stack-slot reservation, written before anything can read
it: the kept value is not used) or "address-passed" (the slot's address goes to a callee first: not decided here);
"return" for a `ret` of a function declared to return a value (EAX is its result: Hex-Rays writes `return
callee(...)` there, the callee's EAX, when the original returns the value it kept); otherwise "use", the case a
translation must reproduce."""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from vcall_resolve import written_exact, r32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
V = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'verification')
VOL = ('eax', 'ecx', 'edx')


from reg_preserve import table, preserved, slot_kind


def main(out, everyone=False):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    ecxu = json.load(open(os.path.join(D, 'ecx_use.json'))); edxu = json.load(open(os.path.join(D, 'edx_use.json')))
    gated = {int(json.loads(l)['original'], 16) for l in open(os.path.join(V, 'gate_clean_final.jsonl'))}
    # a function declared to return a value reads EAX at its `ret` (the IDB's prototype, as type-fixed)
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import hx_patches
    returns = set()
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = hx_patches.fix_types(json.loads(l))
        if r.get('ret', '').strip() not in ('void', ''): returns.add(r['ea'])
    code = {int(k, 16): im.code(v) for k, v in ext.items()}
    orders = {f: sorted(ins) for f, ins in code.items()}

    def succ(f, a):
        ins = code[f]; i = ins[a]; m = i.mnemonic; o = i.operands
        if m.startswith('ret'): return []
        if m.startswith('j') or m == 'loop':
            if not (o and o[0].type == X86_OP_IMM):
                t = table(im, ins, a, orders[f]) if m == 'jmp' else None
                return [x for x in t if x in ins] if t and all(x in ins for x in t) else []
            t = [o[0].imm] if o[0].imm in ins else []
            return t if m == 'jmp' else t + ([a + i.size] if a + i.size in ins else [])
        return [a + i.size] if a + i.size in ins else []

    keeps = preserved(im, code, orders)                 # which of EAX/ECX/EDX each function leaves untouched

    callers = sorted(code) if everyone else sorted(gated)
    sites = []
    for f in callers:
        ins = code.get(f)
        if not ins: continue
        order = orders[f]
        preds = {a: 0 for a in order}
        for a in order:
            for b in succ(f, a): preds[b] += 1
        before = {a: frozenset() for a in order if not preds[a]}
        work = list(before)
        while work:                                   # may-analysis: (reg, defined at, kept across call at | None)
            a = work.pop(); st = set(before[a]); i = ins[a]
            if i.mnemonic == 'call':
                t = i.operands[0].imm if i.operands and i.operands[0].type == X86_OP_IMM else None
                kept = keeps.get(t, set()) if t is not None else set()
                st = {(r, d, '0x%08X' % a) for r, d, _ in st if r in kept}
            else:
                w = {r for r in written_exact(i) if r in VOL}
                st = {s for s in st if s[0] not in w} | {(r, '0x%08X' % a, None) for r in w}
            for b in succ(f, a):
                old = before.get(b); new = frozenset(st) if old is None else old | frozenset(st)
                if new != old: before[b] = new; work.append(b)
        for a in order:
            if a not in before: continue
            i = ins[a]; o = i.operands
            rd = {r32(i.reg_name(r)) for r in i.regs_access()[0]}
            if len(o) == 2 and o[0].type == X86_OP_REG and ((i.mnemonic in ('xor', 'sub') and o[1].type == X86_OP_REG and o[0].reg == o[1].reg)
                                                           or (i.mnemonic == 'or' and o[1].type == X86_OP_IMM and o[1].imm & 0xFFFFFFFF == 0xFFFFFFFF)):
                rd = set()
            if i.mnemonic.startswith('ret') and f in returns: rd.add('eax')
            if i.mnemonic == 'call' and o and o[0].type == X86_OP_IMM:
                c = '0x%08X' % o[0].imm
                if ecxu.get(c): rd.add('ecx')
                if edxu.get(c): rd.add('edx')
            for r, d, k in before[a]:
                if k is not None and r in rd:
                    kind = slot_kind(ins, order, a) if i.mnemonic == 'push' else 'return' if i.mnemonic.startswith('ret') else 'use'
                    sites.append({'callee': '0x%08X' % ins[int(k, 16)].operands[0].imm, 'function': '0x%08X' % f, 'use': '0x%08X' % a, 'register': r,
                                  'instruction': '%s %s' % (i.mnemonic, i.op_str), 'defined': d, 'kept_across': k, 'kind': kind})
    sites.sort(key=lambda s: (s['function'], s['use'], s['register'], s['defined'], s['kept_across']))
    summ = {'sites': len(sites), 'functions_with_use': len({s['function'] for s in sites if s['kind'] == 'use'})}
    for s in sites: summ[s['kind']] = summ.get(s['kind'], 0) + 1
    json.dump({'sites': sites, '_summary': summ}, open(out, 'w'), indent=0)
    print(json.dumps(summ))


if __name__ == '__main__':
    main(sys.argv[1], '--all-callers' in sys.argv)
