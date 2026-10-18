"""tail_purges.py -- the pop count of functions that end in a tail jump instead of a `ret`, from their code.

purges.py reads `ret N`; a function with no `ret` (0x10271800 ends `add esp, 0x40; jmp 0x1028BBD0`) returns
through the `ret` of the function it jumps to. When the stack depth at that jump is exactly 0 -- every byte
the function pushed is gone, solved by sp_solve.insn_depths with every call's pop known or solved -- the
target's `ret N` pops N bytes of this function's caller's arguments, so this function pops N.

Taken only when every exit the walk reaches is such a jump, at depth exactly 0, to targets whose pops are
known and agree, and no `ret` and no indirect jump is reachable; a jump at any other depth (shared epilogue
code) or an unknown target leaves the function out. Repeated until nothing changes, since a target's own
count may come from this rule.

    python3 tail_purges.py data/tail_purges.json      uses $FFXI_DLL, data/purges.json, data/func_extents.json
Output: {"0xADDR": {"pops": N, "via": ["0xTARGET", ...]}}; readers merge it under purges.json.
"""
import json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve

D = sp_solve.D


def tail_exits(im, ea, ext, pur, starts, nr, iat):
    """[(target, depth)] for every exit of the function, or None when it has a `ret`, an indirect jump out,
    or an instruction whose depth is not exact"""
    ins = im.code(ext)
    dep = sp_solve.insn_depths(im, ea, ext, pur, starts, nr=nr, iat=iat)
    every = getattr(sp_solve.func_calls, 'every', {})
    if not dep or set(every) != set(dep): return None
    out = []
    for a in every:
        i = ins[a]; m = i.mnemonic
        if m.startswith('ret'): return None
        if m != 'jmp': continue
        op = i.operands[0]
        if op.type == X86_OP_IMM:
            if op.imm not in ins: out.append((op.imm, dep[a][0]))
        elif op.type == X86_OP_MEM and op.mem.scale == 4 and op.mem.index and not op.mem.base:
            continue                                  # a switch inside the function (sp_solve follows it)
        else: return None
    return out


def main(out):
    im = sp_solve.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    iat = sp_solve.imports(im, os.environ['FFXI_DLL']); nr = sp_solve.noreturn_funcs(im, ext, iat)
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}
    got = {}
    while True:
        pur = dict(purges, **{k: v['pops'] for k, v in got.items()})
        new = 0
        for k, e in ext.items():
            ea = int(k, 16); key = '0x%08X' % ea
            if key in pur: continue
            ex = tail_exits(im, ea, e, pur, starts, nr, iat)
            if not ex: continue
            pops = {pur.get('0x%08X' % t) for t, d in ex}
            if any(d != 0 for t, d in ex) or len(pops) != 1 or None in pops: continue
            got[key] = {'pops': pops.pop(), 'via': sorted({'0x%08X' % t for t, d in ex})}; new += 1
        if not new: break
    json.dump(got, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions pop what their tail-jump target pops' % len(got))


if __name__ == '__main__':
    main(*sys.argv[1:])
