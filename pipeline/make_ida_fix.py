"""make_ida_fix.py -- build ida/ffxi_ida_fix.py: the IDA pass with its data embedded (one file, runs from anywhere).

    python3 make_ida_fix.py
Embeds data/names_pending_idb.json, data/type_fixes.json, data/real_sp.json and the function list of
data/exports/gfx_export.jsonl into ida/ida_fix.template (not runnable itself).
"""
import json, os, re
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
D = os.path.join(R, 'data')
data = {'names': json.load(open(os.path.join(D, 'names_pending_idb.json'), encoding='utf-8')),
        'types': json.load(open(os.path.join(D, 'type_fixes.json'), encoding='utf-8')),
        'real_sp': json.load(open(os.path.join(D, 'real_sp.json'))),
        'revert': json.load(open(os.path.join(D, 'ida_sp_revert.json')))['calls'],
        'export': [json.loads(l)['ea'] for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))],
        'classes': json.load(open(os.path.join(D, 'ida_classes.json')))['classes'],
        'pre_class_types': json.load(open(os.path.join(D, 'pre_class_types.json')))['types']}     # make_ida_types.py
# names that state a fact (evidence_names.py), for functions with no other pending name
for k, v in json.load(open(os.path.join(D, 'evidence_names.json'))).items():
    if k.startswith('_') or k in data['names']: continue
    data['names'][k] = {'old': 'sub_%X' % int(k, 16), 'name': v['name'], 'confidence': 'proven',
                        'evidence': v['evidence'], 'note': '[name %s, evidence_names.py] %s' % (v['name'], v['evidence'])}
# the D3D8 types pipeline/xi_d3d8.hpp defines: the types header the IDA pass writes leaves these to it
_d3d = open(os.path.join(R, 'pipeline', 'xi_d3d8.hpp')).read()
data['types_external'] = sorted(set(re.findall(r'\b(?:struct|union|enum)\s+(\w+)\s*(?::\s*[\w\s]+)?\{', _d3d))
                                | set(re.findall(r'(?m)^\s*typedef\s+[^;{(]*?\b(\w+)\s*;', _d3d)))
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
    r['why'].setdefault(ck, "solved from this function's stack balance at every ret and join" + (
        '; UNCERTAIN: the balance uses D3D calls settled by an accepted, unproven declaration (%s, data/com_fields.json)'
        % ', '.join(v['rests_on_accepted']) if v.get('rests_on_accepted') else ''))
for ck, v in json.load(open(os.path.join(D, 'vcalls.json'))).items():
    if ck.startswith('_') or v['func'] not in want: continue
    r = sp_calls.setdefault(v['func'], {'calls': {}, 'why': {}})
    if ck in r['calls'] and r['calls'][ck] != v['pops']:
        raise SystemExit('vtable-proven and other source disagree at %s: %s vs %s' % (ck, r['calls'][ck], v['pops']))
    r['calls'][ck] = v['pops']; r['why'].setdefault(ck, 'every possible callee (by vtable) pops it')
# IDA's SP checked against the exact depths (ida_sp_check.py): the one correction explaining each difference,
# at a call (its known pop) or at the jump in front of a block that only jumps reach
for fk, rec in json.load(open(os.path.join(D, 'ida_sp_audit.json'))).items():
    if fk not in want: continue
    r = sp_calls.setdefault(fk, {'calls': {}, 'why': {}})
    for fx in rec['fixes']:
        if fx['at'] in r['calls'] and r['calls'][fx['at']] != fx['change']:
            raise SystemExit('ida_sp_audit and another source disagree at %s: %s vs %s' % (fx['at'], fx['change'], r['calls'][fx['at']]))
        r['calls'][fx['at']] = fx['change']; r['why'].setdefault(fx['at'], fx['why'])
data['sp_calls'] = sp_calls
# an SP point an earlier run set wrongly is removed only where no exact value is known now (fix_sp sets those)
known = {ck for r in sp_calls.values() for ck in r['calls']}
data['revert'] = [c for c in data['revert'] if c not in known]
# D3D8 method calls on a proven object (com_calls.py): the call type is the method's own, from the D3D8 header
data['com_types'] = {k: {'func': v['func'], 'targets': [], 'pops': v['pops'],
                         'decl': 'int __stdcall f(%s);' % ', '.join(['void *self'] + ['int a%d' % i for i in range(v['pops'] // 4 - 1)])}
                     for k, v in json.load(open(os.path.join(D, 'com_calls.json'))).items() if v['func'] in want}
# virtual calls proven by vtable (vcall_resolve.py) in the exported functions, with a known convention and an
# EAX return: their call type can be forced exactly
vc = json.load(open(os.path.join(D, 'vcalls.json')))
data['vcall_types'] = {k: {'func': v['func'], 'targets': v['targets'], 'pops': v['pops'], 'ecx': v['ecx']}
                       for k, v in vc.items() if not k.startswith('_') and v['func'] in want and v['ecx'] is not None and v['ret'] == 'eax'}
# functions declared __stdcall (in the export) that use their entry ECX: retyped __thiscall in IDA
ecx = json.load(open(os.path.join(D, 'ecx_use.json'))); purges = json.load(open(os.path.join(D, 'purges.json')))
decl = {}
for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
    r = json.loads(l)
    if r.get('cc'): decl['0x%08X' % r['ea']] = r['cc']
    for o in (r.get('objs') or {}).values():
        if o.get('func') and '__stdcall' in o['type']: decl.setdefault('0x%08X' % o['ea'], 'stdcall')
data['conv_fix'] = {k: purges[k] for k, cc in decl.items() if cc == 'stdcall' and ecx.get(k) and k in purges}
data['conv_why'] = {k: ecx[k] for k in data['conv_fix']}
blob = json.dumps(data, separators=(',', ':'), ensure_ascii=True)
assert "'''" not in blob
src = open(os.path.join(R, 'ida', 'ida_fix.template')).read().replace('__DATA__', blob, 1)
os.makedirs(os.path.join(R, 'ida'), exist_ok=True)
out = os.path.join(R, 'ida', 'ffxi_ida_fix.py')
open(out, 'w', newline='\n').write(src)
print('%s: %d names, %d prototypes, %d conventions, %d exact call effects in %d functions, %d vtable call types, %d D3D call types, %d SP points to remove, %d to export, %d KB'
      % (out, len(data['names']), len(data['types']), len(data['conv_fix']), sum(len(r['calls']) for r in sp_calls.values()),
         len(sp_calls), len(data['vcall_types']), len(data['com_types']), len(data['revert']), len(data['export']), len(src) // 1024))
