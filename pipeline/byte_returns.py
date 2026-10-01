"""byte_returns.py -- failures where only the upper bytes of EAX differ: re-gate comparing AL only.
A pass means the function returns a byte (char/bool) and IDA's `int` return type is wrong; the
function's behaviour is otherwise equivalent. Output: list of addresses + their re-gate records.
Usage: python3 byte_returns.py GAME DLL CASES GATE... OUT"""
import json, re, sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import diffemu_gfx as DG
game, dll, casesf, *gates, out = sys.argv[1:]
cases = {c['original']: c for c in json.load(open(casesf))}
res = {}
for gf in gates:
    for l in open(gf):
        g = json.loads(l)
        if g['verdict'] != 'FAIL': continue
        m = re.match(r'EAX 0x([0-9A-F]{8}) vs 0x([0-9A-F]{8})', g['first_mismatch']['why'])
        if not m or m.group(1)[-2:] != m.group(2)[-2:]: continue
        c = dict(cases[g['original']]); c['returns'] = 'bool'
        mm = DG.GfxMachine(game, dll, c['stubs'], [])
        r = DG.run_case(mm, c, g['trials'], 11)
        res[g['original']] = r
        print(g['original'], g['name'][:30], '-> AL-only:', r['verdict'], flush=True)
json.dump(res, open(out, 'w'), indent=0)
