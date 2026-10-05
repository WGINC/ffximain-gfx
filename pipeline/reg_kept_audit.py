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
entry (data/ecx_use.json, data/edx_use.json). A callee whose use "cannot tell" is not counted as a reader.

    FFXI_DLL=... python3 reg_kept_audit.py data/reg_kept_audit.json
Output {"sites": [{"function", "use", "register", "instruction", "defined", "kept_across", "kind"}], "_summary"};
kind (slot_kind): for `push REG`, "overwritten" (MSVC's stack-slot reservation, written before anything can read
it: the kept value is not used) or "address-passed" (the slot's address goes to a callee first: not decided here);
otherwise "use", the case a translation must reproduce."""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from vcall_resolve import written_exact, r32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
V = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'verification')
VOL = ('eax', 'ecx', 'edx')


def table(im, ins, a, order):
    """the targets of `jmp [r*4+T]` when its index is bound by the two instructions before it, else None"""
    i = ins[a]; o = i.operands[0] if i.operands else None
    if i.mnemonic != 'jmp' or o is None or o.type != X86_OP_MEM or o.mem.base or not o.mem.index or o.mem.scale != 4:
        return None
    k = order.index(a)
    if k < 2: return None
    c, j = ins[order[k - 2]], ins[order[k - 1]]
    if c.mnemonic != 'cmp' or j.mnemonic != 'ja' or c.address + c.size != j.address or j.address + j.size != a: return None
    if c.operands[0].type != X86_OP_REG or c.operands[1].type != X86_OP_IMM or c.operands[0].reg != o.mem.index: return None
    landing = {x.operands[0].imm for x in ins.values()
               if x.mnemonic.startswith('j') and x.operands and x.operands[0].type == X86_OP_IMM}
    if j.address in landing or a in landing or not 0 <= c.operands[1].imm <= 4096: return None
    return [im.u32((o.mem.disp & 0xFFFFFFFF) + 4 * n) for n in range(c.operands[1].imm + 1)]


def slot_kind(ins, order, a):
    """for `push REG` at a: "overwritten" when, straight after it, a 4-byte write to [esp] comes before anything
    that could read the slot (an access overlapping [esp, esp+4), another use of ESP, a call, push, pop, jump, or a
    jump landing); "address-passed" when the slot's address is taken first (`mov r, esp` / `lea r, [esp]`: MSVC's
    by-value temporary, built in place by the callee); otherwise "use" (the pushed value is an argument or is read)"""
    landing = {x.operands[0].imm for x in ins.values()
               if x.mnemonic.startswith('j') and x.operands and x.operands[0].type == X86_OP_IMM}
    k = order.index(a)
    for b in order[k + 1:k + 12]:
        i = ins[b]; m = i.mnemonic; o = i.operands
        if b in landing or m in ('call', 'push', 'pop') or m.startswith(('j', 'ret', 'loop')): return 'use'
        if m in ('mov', 'lea') and len(o) == 2 and o[0].type == X86_OP_REG and \
                ((o[1].type == X86_OP_REG and i.reg_name(o[1].reg) == 'esp') or
                 (o[1].type == X86_OP_MEM and o[1].mem.base and i.reg_name(o[1].mem.base) == 'esp' and not o[1].mem.index and o[1].mem.disp == 0)):
            return 'address-passed'
        for n, x in enumerate(o):
            if x.type == X86_OP_REG and i.reg_name(x.reg) == 'esp': return 'use'
            if x.type != X86_OP_MEM or not x.mem.base or i.reg_name(x.mem.base) != 'esp': continue
            if x.mem.index: return 'use'
            if m == 'lea':
                if 0 <= x.mem.disp < 4: return 'address-passed'
                continue                                                        # another stack address
            if not (x.mem.disp < 4 and x.mem.disp + x.size > 0): continue           # does not touch the slot
            if n == 0 and x.mem.disp == 0 and x.size == 4 and m in ('mov', 'fstp', 'fistp', 'fst', 'fist'): return 'overwritten'
            return 'use'
    return 'use'


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    ecxu = json.load(open(os.path.join(D, 'ecx_use.json'))); edxu = json.load(open(os.path.join(D, 'edx_use.json')))
    gated = {int(json.loads(l)['original'], 16) for l in open(os.path.join(V, 'gate_clean_final.jsonl'))}
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

    # which of EAX/ECX/EDX each function leaves untouched
    own = {}
    for f, ins in code.items():
        w, callees, unknown = set(), set(), False
        for a, i in ins.items():
            m = i.mnemonic; o = i.operands
            if m == 'call' or m == 'jmp':
                if o and o[0].type == X86_OP_IMM:
                    if m == 'call' or o[0].imm not in ins: callees.add(o[0].imm)
                elif not (m == 'jmp' and (lambda t: t is not None and all(x in ins for x in t))(table(im, ins, a, orders[f]))):
                    unknown = True
                continue
            w |= {r for r in written_exact(i) if r in VOL}
        own[f] = (w, callees, unknown)
    keeps = {f: set() if u else set(VOL) - w for f, (w, c, u) in own.items()}
    changed = True
    while changed:
        changed = False
        for f, (w, callees, u) in own.items():
            k = set(keeps[f])
            for c in callees: k &= keeps.get(c, set())
            if k != keeps[f]: keeps[f] = k; changed = True

    sites = []
    for f in sorted(gated):
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
            if i.mnemonic == 'call' and o and o[0].type == X86_OP_IMM:
                c = '0x%08X' % o[0].imm
                if ecxu.get(c): rd.add('ecx')
                if edxu.get(c): rd.add('edx')
            for r, d, k in before[a]:
                if k is not None and r in rd:
                    kind = slot_kind(ins, order, a) if i.mnemonic == 'push' else 'use'
                    sites.append({'function': '0x%08X' % f, 'use': '0x%08X' % a, 'register': r,
                                  'instruction': '%s %s' % (i.mnemonic, i.op_str), 'defined': d, 'kept_across': k, 'kind': kind})
    sites.sort(key=lambda s: (s['function'], s['use'], s['register'], s['defined'], s['kept_across']))
    summ = {'sites': len(sites), 'functions_with_use': len({s['function'] for s in sites if s['kind'] == 'use'})}
    for s in sites: summ[s['kind']] = summ.get(s['kind'], 0) + 1
    json.dump({'sites': sites, '_summary': summ}, open(out, 'w'), indent=0)
    print(json.dumps(summ))


if __name__ == '__main__':
    main(sys.argv[1])
