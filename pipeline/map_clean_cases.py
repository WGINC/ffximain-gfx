"""map_clean_cases.py -- point the raw gate cases at the clean DLL's exports (demangled names)."""
import json, re, subprocess, sys
dll, placement, raw_cases, out = sys.argv[1:5]
P = json.load(open(placement))
exp = subprocess.run(['i686-w64-mingw32-objdump', '-p', dll], capture_output=True, text=True).stdout
tbl = exp.split('[Ordinal/Name Pointer] Table', 1)[1]
names = re.findall(r'^\s*\[\s*\d+\]\s+(\S+)\s*$', tbl, re.M)
dem = subprocess.run(['i686-w64-mingw32-c++filt', '-n'], input='\n'.join(names), capture_output=True, text=True).stdout.split('\n')
by = {}
for n, d in zip(names, dem):
    key = d.split('(')[0].strip()
    by.setdefault(key, n)
cases = []; missing = []
for c in json.load(open(raw_cases)):
    ea = hex(int(c['original'], 16)); p = P.get(ea)
    if not p: continue
    key = '%s::%s' % (p['class'], p['method']) if p['class'] else None
    if key is None:
        cands = [k for k in by if '::' not in k and k.split()[-1] in (c['name'].split()[0], 'fn_%08X' % int(ea, 16), 'fn_%08x' % int(ea, 16))]
        key = cands[0] if cands else None
    if key and key in by:
        c = dict(c); c['candidate'] = by[key]; cases.append(c)
    else:
        missing.append(ea)
json.dump(cases, open(out, 'w'), indent=0)
print(len(cases), 'mapped;', len(missing), 'missing', missing[:5])
