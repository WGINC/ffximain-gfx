"""gen_residue.py -- document every graphics-set function that is NOT in the verified clean tree:
what it is, why it isn't reconstructed, and the route for it.
Usage: python3 gen_residue.py EXPORT META GATE LOCALIZE BYTE_RETURNS CLEAN_GATE OUT.md"""
import json, os, re, sys, collections
DATA = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
exp, metaf, gatef, locf, brf, cleanf, out = sys.argv[1:8]
R = {json.loads(l)['ea']: json.loads(l) for l in open(exp)}
meta = json.load(open(metaf))
G = {int(json.loads(l)['original'], 16): json.loads(l) for l in open(gatef)}
L = {int(k, 16): v for k, v in json.load(open(locf)).items()} if os.path.exists(locf) else {}
BR = {int(k, 16): v for k, v in json.load(open(brf)).items()} if os.path.exists(brf) else {}
C = {int(json.loads(l)['original'], 16): json.loads(l) for l in open(cleanf)} if os.path.exists(cleanf) else {}
DM = json.load(open(os.path.join(DATA, 'dm_names.json')))
D3DX = json.load(open(os.path.join(DATA, 'd3dx', 'd3dx81_manifest.json')))
SP_AUDIT = json.load(open(os.path.join(DATA, 'ida_sp_audit.json'))) if os.path.exists(os.path.join(DATA, 'ida_sp_audit.json')) else {}
TYPE_FIXES = {k.upper().replace('0X', '0x'): v for k, v in json.load(open(os.path.join(DATA, 'type_fixes.json'))).items()}
def dmc(ea):
    d = DM.get(hex(ea)); return (d[1][:110] + ('...' if len(d[1]) > 110 else '')) if d and d[1] else ''
rows = collections.defaultdict(list)
for k, v in meta.items():
    ea = int(k, 16); r = R[ea]; nm = r['name']
    if v['status'] == 'library':
        d = D3DX.get(hex(ea), {})
        rows['library'].append((ea, nm, d.get('symbol', v['why']), d.get('object', ''), '%s bytes, %s' % (d.get('bytes_compared', '?'), d.get('how', '?'))))
    elif v['status'] in ('skipped', 'untranslatable'):
        why = v['why']
        if '0x%08X' % ea in SP_AUDIT: why += '; IDA SP corrected in ida/ffxi_ida_fix.py (%s), re-export pending' % ', '.join(
            '%s %+d' % (x['at'], x['change']) for x in SP_AUDIT['0x%08X' % ea]['fixes'])
        norm = lambda t: re.sub(r'\s+', '', re.sub(r'\b\w+\s*(?=[,)])', '', t))       # parameter names dropped
        fixed = sorted(n for n, o in r.get('objs', {}).items() if o.get('func') and '0x%08X' % o['ea'] in TYPE_FIXES
                       and '__asm' in v['why'] and norm(o['type']) != norm(TYPE_FIXES['0x%08X' % o['ea']]['type']))
        if fixed: why += '; callee types corrected in data/type_fixes.json (%s), re-export pending' % ', '.join(fixed)
        rows['construct'].append((ea, nm, why, dmc(ea)))
    elif v['status'] == 'translated':
        if ea in C and C[ea]['verdict'] == 'PASS': continue
        g = G.get(ea)
        if g is None:
            built = os.path.exists(os.path.join(os.path.dirname(metaf), 'obj', 'fn_%08X.o' % ea))
            rows['compile'].append((ea, nm, ('compiles; no gate case (calling convention %s)' % R[ea].get('cc')) if built
                                    else 'did not compile as C++: ' + (open(os.path.join(os.path.dirname(metaf), 'err', 'fn_%08X.txt' % ea)).read().split('error: ')[1].split('\n')[0][:90] if os.path.exists(os.path.join(os.path.dirname(metaf), 'err', 'fn_%08X.txt' % ea)) else '?'), dmc(ea)))
        elif ea in BR and BR[ea].get('verdict') == 'PASS':
            continue
        elif g['verdict'] == 'FAIL':
            l = L.get(ea, {})
            why = g['first_mismatch']['why'][:110]
            if '0x%08X' % ea in SP_AUDIT: why += '; cause: IDA SP, corrected in ida/ffxi_ida_fix.py (%s), re-export pending' % ', '.join(
                '%s %+d' % (x['at'], x['change']) for x in SP_AUDIT['0x%08X' % ea]['fixes'])
            rows['fail'].append((ea, nm, why, (l.get('recon_line') or [None, ''])[1][:90], dmc(ea)))
        elif g['verdict'] == 'INCONCLUSIVE':
            rows['inconclusive'].append((ea, nm, ', '.join('%s %d' % kv for kv in g['original_outcomes'].items()), dmc(ea)))
        elif g['verdict'] == 'UNRESOLVED':
            rows['unresolved'].append((ea, nm, '; '.join('%s (%d trial%s)' % (kv[0], kv[1], '' if kv[1] == 1 else 's') for kv in sorted(g['unresolved'].items(), key=lambda kv: -kv[1])), dmc(ea)))
        elif g['verdict'] == 'PASS' and ea in C:
            rows['clean_regress'].append((ea, nm, C[ea].get('verdict'), dmc(ea)))
        elif g['verdict'] != 'PASS':
            raise SystemExit('gen_residue.py: 0x%08X has gate verdict %r, which no group covers' % (ea, g['verdict']))
        elif g['verdict'] == 'PASS':
            rows['unplaced'].append((ea, nm, 'passes as decompiled; the clean stage has no export for it to gate', dmc(ea)))
w = []
w.append('# Graphics functions not in the verified tree (build 2026-09-03)\n')
w.append('Every function in the 768-function graphics set that is not a gate-verified member of `tree/` is listed here, with why and what to do with it. Descriptions in the last column are DancingMad\'s.\n')
w.append('| Group | Count | Route |\n|---|---|---|')
w.append('| Microsoft D3DX 8.1 (statically linked) | %d | Not game code. Byte-identical to `d3dx8.lib` from the DirectX 8.1 SDK; a standalone build links that library and includes `d3dx8.h`. In a hybrid build they keep running from the game image. |' % len(rows['library']))
w.append('| Constructs C++ cannot express | %d | Stays original code (`orig::`). Listed below with the exact construct. |' % len(rows['construct']))
w.append('| Did not compile, or compiles with no gate case | %d | Translator gaps; listed below. |' % len(rows['compile']))
w.append('| Gate FAIL (decompiled code is wrong) | %d | Hex-Rays output differs from the original; the first divergence is given. Fix in IDA (types) and re-run, or keep `orig::`. |' % len(rows['fail']))
w.append('| Verified as decompiled, not in the clean tree | %d | Clean-stage gap (no exported symbol to gate); the raw translation is verified. |' % len(rows['unplaced']))
w.append('| Gate INCONCLUSIVE (no comparable run) | %d | The original never returned under fuzzed inputs; needs better input construction or a live memory overlay. |' % len(rows['inconclusive']))
w.append('| Gate UNRESOLVED (no definitive fact for a call) | %d | A call the original makes has no definitive pop or argument count (`CLAUDE.md`), so no verdict is given. The call site is named; settle it in IDA (type the callee or the call) or from a live run, then re-gate. |' % len(rows['unresolved']))
w.append('| Passes as decompiled, fails in the clean tree | %d | Clean-stage regression; listed below. |' % len(rows['clean_regress']))
w.append('')
listed = sum(len(v) for v in rows.values())
if listed != len(meta) - sum(1 for k in meta if int(k, 16) in C and C[int(k, 16)]['verdict'] == 'PASS'):
    raise SystemExit('gen_residue.py: %d functions listed, but %d are not verified in the clean tree' % (listed, len(meta) - sum(1 for k in meta if int(k, 16) in C and C[int(k, 16)]['verdict'] == 'PASS')))
def table(title, head, rs, fmt):
    if not rs: return
    w.append('## %s (%d)\n' % (title, len(rs)))
    w.append('| ' + ' | '.join(head) + ' |'); w.append('|' + '---|' * len(head))
    for r in sorted(rs): w.append('| ' + ' | '.join(fmt(r)) + ' |')
    w.append('')
esc = lambda s: str(s).replace('|', '\\|').replace('\n', ' ')
table('Microsoft D3DX 8.1', ['Address', 'IDB name', 'Library symbol', 'Object', 'Match'], rows['library'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), '`%s`' % esc(r[2]), esc(r[3]), esc(r[4])])
table('Constructs C++ cannot express', ['Address', 'Name', 'Construct', 'What it does'], rows['construct'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
table('Did not compile / no gate case', ['Address', 'Name', 'Reason', 'What it does'], rows['compile'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
table('Verified as decompiled, not in the clean tree', ['Address', 'Name', 'Why', 'What it does'], rows['unplaced'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
table('Gate FAIL', ['Address', 'Name', 'First difference', 'Reconstruction line at the divergence', 'What it does'], rows['fail'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), '`%s`' % esc(r[3]) if r[3] else '', esc(r[4])])
table('Gate INCONCLUSIVE', ['Address', 'Name', 'Original outcomes', 'What it does'], rows['inconclusive'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
table('Gate UNRESOLVED', ['Address', 'Name', 'Call without a definitive fact', 'What it does'], rows['unresolved'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
table('Passes as decompiled, fails in the clean tree', ['Address', 'Name', 'Clean verdict', 'What it does'], rows['clean_regress'],
      lambda r: ['`0x%08X`' % r[0], esc(r[1]), esc(r[2]), esc(r[3])])
open(out, 'w').write('\n'.join(w) + '\n')
print({k: len(v) for k, v in rows.items()})
