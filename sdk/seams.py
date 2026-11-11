#!/usr/bin/env python3
"""seams.py -- where the graphics system ends: sdk/seams.json and the table in docs/DEVKIT.md.

    python3 sdk/seams.py            (writes sdk/seams.json, prints the table)

Nothing here is a judgement of what "is graphics". Each column is a fact with its source:
  unit        the source file a function belongs to                      data/units.json (unit evidence per function)
  device      the function has a Direct3D call site proven in the game   data/render_map.json "sites" (the site's func)
  frame       the function is reachable from the main loop each frame    data/frame_set.json
  tree        the function is reconstructed and gate-verified            kit/xikit/dist/xikit_manifest.txt
  hookable    a mod may hook it with XI_HOOK (not flag 8: register input; flag 2: a speedfix site)   the manifest's flags
  depth       the fewest direct calls (call rel32, read from the game's code inside the function's extents,
              data/func_extents.json) from the function to one with a device call: 0 = it has one, 1 = it calls such
              a function, ... ; none = no chain of direct calls reaches the device. Calls through pointers and
              vtables are not followed, so "none" does not prove a function never draws.
A unit is listed when one of its functions has depth 0 or 1. The table is sorted by how many do; there is no
"graphics / not graphics" label: the seam is per function (sdk/seams.json), and xihook:: lists what may be hooked.
Needs FFXI_DLL (the analysed build) and capstone.
"""
import collections, json, os
H = os.path.dirname(os.path.abspath(__file__)); R = os.path.join(H, '..')
units = json.load(open(os.path.join(R, 'data/units.json')))
rm = json.load(open(os.path.join(R, 'data/render_map.json')))['sites']
fs = json.load(open(os.path.join(R, 'data/frame_set.json')))['functions']
dev = {int(s['func'], 16) for s in rm.values() if s.get('func')}
frame = {int(a, 16) for a in fs}
man = {}
for l in open(os.path.join(R, 'kit/xikit/dist/xikit_manifest.txt')):
    if l[0] in '#!' or not l.strip(): continue
    p = l.split(); man[0x10000000 + int(p[0], 16)] = (int(p[1]), p[5])
import struct
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
dll = open(os.environ['FFXI_DLL'], 'rb').read(); pe = struct.unpack_from('<I', dll, 0x3C)[0]
assert struct.unpack_from('<I', dll, pe + 8)[0] == 0x6A995428, 'FFXI_DLL must be the analysed build'
o = pe + 24 + struct.unpack_from('<H', dll, pe + 20)[0]; tva, traw, tsz = struct.unpack_from('<I', dll, o + 12)[0], struct.unpack_from('<I', dll, o + 20)[0], struct.unpack_from('<I', dll, o + 8)[0]
ext = json.load(open(os.path.join(R, 'data/func_extents.json'))); starts = {int(a, 16) for a in ext}
md = Cs(CS_ARCH_X86, CS_MODE_32); calls = collections.defaultdict(set)
for a_, rs in ext.items():
    a = int(a_, 16)
    for lo, hi in rs:
        if not (0x10000000 + tva <= lo < hi <= 0x10000000 + tva + tsz): continue
        code = dll[traw + lo - 0x10000000 - tva: traw + hi - 0x10000000 - tva]
        for ins in md.disasm_lite(code, lo):
            if ins[2] == 'call' and ins[3].startswith('0x'):
                t = int(ins[3], 16)
                if t in starts: calls[a].add(t)
callers = collections.defaultdict(set)
for a, ts in calls.items():
    for t in ts: callers[t].add(a)
depth = {a: 0 for a in dev}; front = set(dev); d_ = 0
while front:
    d_ += 1; nxt = set()
    for t in front:
        for c in callers[t]:
            if c not in depth: depth[c] = d_; nxt.add(c)
    front = nxt
per = collections.defaultdict(lambda: collections.Counter()); funcs = {}
for a_, u in units.items():
    a = int(a_, 16); un = u['unit']; c = per[un]; c['functions'] += 1
    f, t = a in frame, a in man; dp = depth.get(a)
    hook = t and not (man[a][0] & 8) and not (man[a][0] & 2)
    c['frame'] += f; c['tree'] += t; c['hookable'] += hook; c['depth0'] += dp == 0; c['depth1'] += dp == 1; c['reaches'] += dp is not None
    if dp is not None and dp <= 1 or t: funcs['0x%08X' % a] = {'unit': un, 'name': man[a][1] if t else None, 'depth': dp, 'frame': f, 'tree': t, 'hookable': hook}
rows = [(un, c) for un, c in per.items() if c['depth0'] + c['depth1']]
rows.sort(key=lambda r: -(r[1]['depth0'] + r[1]['depth1']))
json.dump({'_about': 'sdk/seams.py: per function, depth = fewest direct calls to a function with a proven Direct3D call site (0 = has one; null = none by direct calls); units = the same counted per source unit',
           'units': {un: dict(c) for un, c in rows}, 'functions': funcs}, open(os.path.join(H, 'seams.json'), 'w'), indent=0, sort_keys=True)
print('| Unit | Functions | Device call (depth 0) | Calls one (depth 1) | Reach the device by direct calls | In the frame set | In the tree | Hookable |\n|---|---|---|---|---|---|---|---|')
for un, c in rows[:int(os.environ.get('SEAMS_ROWS', '40'))]:
    print('| %s | %d | %d | %d | %d | %d | %d | %d |' % (un, c['functions'], c['depth0'], c['depth1'], c['reaches'], c['frame'], c['tree'], c['hookable']))
n0 = sum(1 for v in depth.values() if v == 0); n1 = sum(1 for v in depth.values() if v == 1)
print('\n%d functions have a proven device call; %d more call one of those directly; %d of the game\'s %d functions reach the device by some chain of direct calls. %d units have a function of depth 0 or 1.' % (n0, n1, len(depth), len(ext), len(rows)))
hk = lambda dmax: sum(1 for f in funcs.values() if f['depth'] is not None and f['depth'] <= dmax and f['hookable'])
print('Hookable: %d of the depth-0 functions, %d of depth 0 and 1 together.' % (hk(0), hk(1)))
