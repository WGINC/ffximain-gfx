"""case_pushes.py -- apply gfx_cases.pushed_args to an existing cases file: stubs log the slots callers push.

verification/cases_raw_final.json holds cases built by earlier gfx_cases.py runs (several exports); their stubs'
`args` predate data/call_pushes.json. Each stub with `pops` 0 gets `args_may`, the slots gfx_cases.pushed_args gives
(the same rule new cases are built with). Each stub of a function data/arg_reads.json lists gets that function's
masks, as gfx_cases.stub_masks gives them to new cases (the bytes its code reads; arg_reads.py can narrow them
as its analysis improves), keeping the stub's own mask for an argument the code may read whole; the others keep
theirs. Nothing else changes. Prints how many stubs and cases changed.

    FFXI_DLL=... python3 pipeline/case_pushes.py CASES.json OUT.json
"""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import gfx_cases as G


def main(src, out):
    cases = json.load(open(src))
    nst = ncase = nm = nmc = 0
    for c in cases:
        hit = mhit = False
        for a, sp in (c.get('stubs') or {}).items():
            if not isinstance(sp, dict) or 'args' not in sp: continue
            n = G.pushed_args(int(a, 16), sp.get('pops', 0), sp['args'])
            if n and sp.get('args_may') != n: sp['args_may'] = n; nst += 1; hit = True
            if '0x%08X' % int(a, 16) in G.ARG_READS:
                m = G.stub_masks(int(a, 16), None, None, sp.get('masks'))
                if sp.get('masks') != m: sp['masks'] = m; nm += 1; mhit = True
        ncase += hit; nmc += mhit
    json.dump(cases, open(out, 'w'))
    print('%d stubs in %d cases now log the slots their callers push past those compared' % (nst, ncase))
    print('%d stubs in %d cases take their masks from arg_reads.json' % (nm, nmc))


if __name__ == '__main__':
    main(*sys.argv[1:3])
