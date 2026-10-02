"""make_ida_fix.py -- build ida/ffxi_ida_fix.py: the IDA pass with its data embedded (one file, runs from anywhere).

    python3 make_ida_fix.py
Embeds data/names_pending_idb.json, data/type_fixes.json, data/real_sp.json and the function list of
data/exports/gfx_export_current.jsonl into pipeline/ida_fix.template (not runnable itself).
"""
import json, os
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
D = os.path.join(R, 'data')
data = {'names': json.load(open(os.path.join(D, 'names_pending_idb.json'), encoding='utf-8')),
        'types': json.load(open(os.path.join(D, 'type_fixes.json'), encoding='utf-8')),
        'real_sp': json.load(open(os.path.join(D, 'real_sp.json'))),
        'revert': json.load(open(os.path.join(D, 'ida_sp_revert.json')))['calls'],
        'export': [json.loads(l)['ea'] for l in open(os.path.join(D, 'exports', 'gfx_export_current.jsonl'))]}
# every call whose stack effect is known exactly, per exported function: measured (real_sp) or solved (sp_solve)
solved = json.load(open(os.path.join(D, 'sp_solved.json')))
want = {'0x%08X' % e for e in data['export']}; sp_calls = {}
for fk, rec in data['real_sp'].items():
    for ck, p in rec['calls'].items():
        sp_calls.setdefault(fk, {'calls': {}, 'why': {}})['calls'][ck] = p
        sp_calls[fk]['why'][ck] = "the callee's own ret, measured by running the original"
for ck, v in solved.items():
    if ck.startswith('_') or v['func'] not in want: continue
    r = sp_calls.setdefault(v['func'], {'calls': {}, 'why': {}})
    if ck in r['calls'] and r['calls'][ck] != v['pops']:
        raise SystemExit('measured and solved disagree at %s: %s vs %s' % (ck, r['calls'][ck], v['pops']))
    r['calls'][ck] = v['pops']
    r['why'].setdefault(ck, "solved from this function's stack balance at every ret and join")
data['sp_calls'] = sp_calls
blob = json.dumps(data, separators=(',', ':'), ensure_ascii=True)
assert "'''" not in blob
src = open(os.path.join(R, 'pipeline', 'ida_fix.template')).read().replace('__DATA__', blob, 1)
os.makedirs(os.path.join(R, 'ida'), exist_ok=True)
out = os.path.join(R, 'ida', 'ffxi_ida_fix.py')
open(out, 'w', newline='\n').write(src)
print('%s: %d names, %d prototypes, %d exact call effects in %d functions, %d to export, %d KB'
      % (out, len(data['names']), len(data['types']), sum(len(r['calls']) for r in sp_calls.values()), len(sp_calls),
         len(data['export']), len(src) // 1024))
