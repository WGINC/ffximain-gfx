"""call_pushes.py -- how many bytes the caller pushes for each caller-cleanup call, from its own code and exact depths.

A call that pops nothing (its callee is caller-cleanup) and is followed directly by `add esp, N` (or by one
`pop r32`, N = 4) has its stack-argument area removed there. Which of those N bytes were pushed for that call is
settled where the walk back from the call, in the same straight run of code (no jump target, no other call, no
jump or ret in between), reaches the instruction before which the depth was N bytes lower: everything pushed
since then, and removed right after the call, was on the stack only for this call. The depths are
sp_solve.insn_depths: exact, every unknown pop solved, or the call is left out. Where the walk crosses a call or a
block boundary first (cleanup deferred over several calls, or pushes on several paths), the call is left out.

These are the bytes the caller passes. How many of them the callee reads is a different fact (cdecl_args.py, from
the callee's code); a slot pushed and never read is still pushed.

    FFXI_DLL=... python3 pipeline/call_pushes.py data/call_pushes.json [ADDR ...]
Output: {"0xCALL": {"func": "0xFUNC", "bytes": N, "callee": "0xCALLEE" | null}, ...,
         "_callees": {"0xCALLEE": N or null}}: a direct callee with one count over every settled call site, null where
         its sites disagree (variadic, or not one count).
"""
import json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as SP
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG


def func_pushes(im, ea, e, purges, starts, nr, iat):
    deps = SP.insn_depths(im, ea, e, purges, starts, nr=nr, iat=iat)
    if not deps: return {}
    ins = im.code(e)
    order = sorted(ins)
    labels = SP.labels_of(ins)
    joins = set(labels)                 # every instruction another path can reach: direct jump targets ...
    for k, a in enumerate(order):       # ... and switch targets; a switch whose targets are not bounded: no call
        i = ins[a]; op = i.operands[0] if i.operands else None
        if i.mnemonic == 'jmp' and op is not None and op.type == X86_OP_MEM:
            if not (op.mem.index and not op.mem.base and op.mem.scale == 4): continue   # an indirect tail jump: leaves
            prev = [ins[x] for x in order[max(0, k - 5):k]]
            while prev and prev[-1].address + prev[-1].size != a: prev = []
            t = SP.switch_targets(im, prev, i, labels)
            if t is None: return {}
            joins.update(t)
    out = {}
    for k, a in enumerate(order):
        i = ins[a]
        if i.mnemonic != 'call' or a not in deps or k + 1 >= len(order): continue
        nxt = ins[order[k + 1]]
        if nxt.address != a + i.size or nxt.address not in deps: continue
        dc = deps[a][0]
        if deps[nxt.address][0] != dc: continue                  # the callee pops: not caller cleanup
        if nxt.mnemonic == 'add' and len(nxt.operands) == 2 and nxt.operands[0].type == X86_OP_REG \
                and nxt.reg_name(nxt.operands[0].reg) == 'esp' and nxt.operands[1].type == X86_OP_IMM:
            n = nxt.operands[1].imm
        elif nxt.mnemonic == 'pop' and nxt.operands[0].type == X86_OP_REG and nxt.reg_name(nxt.operands[0].reg) != 'esp':
            n = 4
        else: continue
        if n <= 0 or n % 4: continue
        base = dc - n
        j, ok = k, False
        while j > 0:
            prev = ins[order[j - 1]]
            if prev.address + prev.size != order[j]: break            # not straight-line code
            if order[j] in joins: break                               # another path joins here
            if prev.mnemonic == 'call' or prev.mnemonic.startswith('j') or prev.mnemonic.startswith('ret'): break
            if prev.address not in deps: break
            j -= 1
            if deps[prev.address][0] == base: ok = True; break
            if deps[prev.address][0] < base: break
        if not ok: continue
        op = i.operands[0]
        callee = '0x%08X' % op.imm if op.type == X86_OP_IMM else None
        out['0x%08X' % a] = {'func': '0x%08X' % ea, 'bytes': n, 'callee': callee}
    return out


def main(out, *addrs):
    im = SP.Image(os.environ['FFXI_DLL'])
    D = SP.D
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    iat = SP.imports(im, os.environ['FFXI_DLL']); nr = SP.noreturn_funcs(im, ext, iat)
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}
    todo = [int(x, 16) for x in addrs] if addrs else sorted(starts)
    res = {}
    for ea in todo:
        e = ext.get('0x%x' % ea) or ext.get('0x%08X' % ea)
        if e: res.update(func_pushes(im, ea, e, purges, starts, nr, iat))
    per = {}
    for v in res.values():
        if v['callee']: per.setdefault(v['callee'], set()).add(v['bytes'])
    res['_callees'] = {c: (next(iter(s)) if len(s) == 1 else None) for c, s in sorted(per.items())}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    n = len(res) - 1
    print('%d caller-cleanup calls settled; %d direct callees, %d with one count' % (
        n, len(res['_callees']), sum(1 for v in res['_callees'].values() if v is not None)))


if __name__ == '__main__':
    main(*sys.argv[1:])
