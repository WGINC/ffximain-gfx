"""fp_callsites.py -- which calls in each graphics function take a result from the x87 stack.

fpret.py finds callees whose every path returns with one value on the x87 stack, but that analysis
has false positives (a function returning a pointer in EAX that leaves a value in ST0). What a
caller does right after the call decides it: MSVC code consumes a float result at once (fstp,
fcomp, fmul ...). A gate stub pushes a value only at calls the original caller consumes, so a
stub never leaves the reconstruction with a value the original never had.

    python3 fp_callsites.py FFXiMain_unpacked.dll EXPORT.jsonl data/fp_callsites.json
Output: {caller: [callee, ...]} -- callees whose ST0 result this caller consumes at every call site, plus
"indirect" when some virtual call's result is consumed (a vtable-slot stub of a float function then pushes).
"""
import json, os, sys, collections
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
PUSH = ('fld', 'fild', 'fldz', 'fld1', 'fldpi', 'fldl2e', 'fldln2', 'fldlg2', 'fldl2t', 'fbld')


def consumes(ins, k, at, caller_fp, fpret):
    """after the call at ins[k]: does the caller read the value it left in ST0, on every path?

    A path reads it when its first x87 instruction reads ST0 rather than pushing, or when it reaches
    the caller's own `ret` with no x87 instruction and the caller itself returns a float (the value
    passes through as its result). Both successors of a branch are followed."""
    todo, seen = [k + 1], set()
    while todo:
        j = todo.pop()
        while True:
            if j in seen or j >= len(ins) or len(seen) > 400: break
            seen.add(j)
            i = ins[j]; m = i.mnemonic
            if m in ('ret', 'retn'):
                if not caller_fp: return False
                break
            if m == 'call' and i.operands and i.operands[0].type == X86_OP_IMM and i.operands[0].imm in fpret:
                return False                    # another float result is pushed first
            if m.startswith('j'):
                t = i.operands[0].imm if i.operands and i.operands[0].type == X86_OP_IMM else None
                if t not in at: return False
                todo.append(at[t])
                if m == 'jmp': break
                j += 1; continue
            if m.startswith('f') and m not in ('fnstsw', 'fstsw', 'fnstcw', 'fldcw', 'fwait', 'fnclex', 'fninit'):
                if m.startswith(PUSH) and not (m == 'fld' and 'st' in i.op_str): return False
                break
            j += 1
    return True


def main(dll, export, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    fpret = {int(k, 16) for k, v in json.load(open(os.path.join(D, 'fpret.json'))).items() if v in ('float', 'double')}
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    res = {}
    for line in open(export):
        ea = json.loads(line)['ea']
        ins = []
        for a, b in ext.get('0x%x' % ea, []):
            ins += list(md.disasm(img[a - base:b - base], a))
        at = {i.address: k for k, i in enumerate(ins)}
        seen = collections.defaultdict(list)
        for k, i in enumerate(ins):
            if i.mnemonic != 'call' or not i.operands: continue
            if i.operands[0].type == X86_OP_IMM:
                if i.operands[0].imm in fpret: seen['0x%08X' % i.operands[0].imm].append(consumes(ins, k, at, ea in fpret, fpret))
            else:                               # a virtual call: the target is resolved by the gate (vslots)
                seen['indirect'].append(consumes(ins, k, at, ea in fpret, fpret))
        keep = sorted(c for c, v in seen.items() if (all(v) if c != 'indirect' else any(v)))
        if keep: res['0x%08X' % ea] = keep
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d callers, %d consumed float callees' % (len(res), sum(len(v) for v in res.values())))


if __name__ == '__main__':
    main(*sys.argv[1:4])
