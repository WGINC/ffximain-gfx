"""import_pops.py -- the stack bytes each import's call pops, from the game's own call sites.

An import is code in another DLL, so its `ret N` is not in this image. The callers' stack balance is: sp_solve.py
solves the bytes each call pops (data/sp_solved.json, exact). For every solved call whose target is an import
(`call dword ptr [slot]`, or a call of a thunk that is one `jmp dword ptr [slot]`), the pops are recorded under the
import's name. An import's convention is the same at every call, so an import is listed only when all its solved
calls agree (one that disagrees is reported, not listed).

diffemu_gfx.py uses it for a case with "imports": "stub": an import with no fake there answers like a stubbed game
callee (its arguments logged, EAX from the trial's stream, nothing written), and returns popping these bytes.

    FFXI_DLL=... python3 import_pops.py OUT.json
Output {"Name": bytes popped}."""
import collections, json, os, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_IMM, X86_OP_MEM
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S


def main(out):
    im = S.Image(os.environ['FFXI_DLL']); iat = S.imports(im, os.environ['FFXI_DLL'])
    sp = json.load(open(os.path.join(P, '..', 'data', 'sp_solved.json')))
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    first = lambda a: next(md.disasm(bytes(im.img[a - im.base:a - im.base + 8]), a), None)
    def slot_of(o):
        if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index and (o.mem.disp & 0xFFFFFFFF) in iat:
            return o.mem.disp & 0xFFFFFFFF
    pops = collections.defaultdict(set)
    for k, v in sp.items():
        if k.startswith('_') or not isinstance(v, dict) or 'pops' not in v: continue
        i = first(int(k, 16))
        if i is None or i.mnemonic != 'call': continue
        s = slot_of(i.operands[0])
        if s is None and i.operands[0].type == X86_OP_IMM:
            j = first(i.operands[0].imm)
            if j is not None and j.mnemonic == 'jmp': s = slot_of(j.operands[0])
        if s is not None: pops[iat[s].split('!')[-1]].add(v['pops'])
    bad = sorted(k for k, v in pops.items() if len(v) > 1)
    json.dump({k: next(iter(v)) for k, v in sorted(pops.items()) if len(v) == 1}, open(out, 'w'), indent=0)
    print('%d imports with every solved call agreeing; %d disagree: %s' % (len(pops) - len(bad), len(bad), bad))


if __name__ == '__main__':
    main(*sys.argv[1:])
