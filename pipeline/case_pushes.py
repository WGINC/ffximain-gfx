"""case_pushes.py -- apply gfx_cases.pushed_args to an existing cases file: stubs log the slots callers push.

verification/cases_raw_final.json holds cases built by earlier gfx_cases.py runs (several exports); their stubs'
`args` predate data/call_pushes.json. Each stub with `pops` 0 gets `args_may`, the slots gfx_cases.pushed_args gives
(the same rule new cases are built with), and nothing else changes. Prints how many stubs and cases changed.

    FFXI_DLL=... python3 pipeline/case_pushes.py CASES.json OUT.json
"""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gfx_cases as G


def main(src, out):
    cases = json.load(open(src))
    nst = ncase = 0
    for c in cases:
        hit = False
        for a, sp in (c.get('stubs') or {}).items():
            if not isinstance(sp, dict) or 'args' not in sp: continue
            n = G.pushed_args(int(a, 16), sp.get('pops', 0), sp['args'])
            if n and sp.get('args_may') != n: sp['args_may'] = n; nst += 1; hit = True
        ncase += hit
    json.dump(cases, open(out, 'w'))
    print('%d stubs in %d cases now log the slots their callers push past those compared' % (nst, ncase))


if __name__ == '__main__':
    main(*sys.argv[1:3])
