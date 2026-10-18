"""reg_keeps.py -- which of EAX/ECX/EDX each function leaves untouched, for the whole DLL (reg_preserve.preserved).

A function keeps a register when none of its instructions writes any part of it and every function it calls or
tail-jumps to keeps it too; an indirect call or jump (other than a bounded in-function switch) makes it unknown,
so the function is listed as keeping nothing. A caller that reads the register after calling such a function reads
its own value from before the call (0x1002C5C0 reads EDX across 0x1002B5A0 `lea eax, [ecx+0x44]; ret`).

    FFXI_DLL=... python3 reg_keeps.py data/reg_keeps.json
Output {"0xFUNC": ["eax", "edx"]} for functions keeping at least one register."""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from reg_preserve import preserved

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    code = {int(k, 16): im.code(v) for k, v in ext.items()}
    keeps = preserved(im, code)
    res = {'0x%08X' % f: sorted(k) for f, k in sorted(keeps.items()) if k}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions keep at least one of EAX/ECX/EDX' % len(res))


if __name__ == '__main__':
    main(*sys.argv[1:])
