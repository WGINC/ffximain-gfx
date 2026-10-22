"""annotate_gate.py -- put each function's clean-build gate result under its FUNCTION line.
Usage: python3 annotate_gate.py CLEAN_DIR GATE_CLEAN.jsonl   (the clean fuzz gate, or combine_gates.py's output)"""
import json, os, re, sys
d, gf = sys.argv[1:3]
G = {int(json.loads(l)['original'], 16): json.loads(l) for l in open(gf)}
n = {'pass': 0, 'unverified': 0}
for f in sorted(os.listdir(os.path.join(d, 'src'))):
    p = os.path.join(d, 'src', f); out = []
    for line in open(p).read().split('\n'):
        if line.startswith('// GATE:') or line.startswith('// NOT VERIFIED'): continue
        out.append(line)
        m = re.match(r'// FUNCTION: FFXIMAIN 0x([0-9A-F]{8})', line)
        if not m: continue
        g = G.get(int(m.group(1), 16))
        if g and g['verdict'] == 'PASS':
            r = g['original_outcomes'].get('return', 0)
            s = '// GATE: PASS -- %d trials, %d returned normally, %d D3D calls compared (diffemu_gfx, clean build)' % (
                g['trials'], r, g.get('d3d_calls_checked', 0))
            if r < 10: s += '; thin evidence: few trials returned'
            if g.get('x87_rounded_trials'): s += '; float result equal at its declared precision (the original returns ST0 unrounded)'
            if g.get('inputs_limited'): s += '; ONLY UNDER FIXED INPUTS (data/case_inputs.json): ' + g['inputs_limited']
            if g.get('gates'):            # combine_gates.py: every gate's verdict and returning trials (fuzz, each dump)
                s += '; gates: ' + ', '.join('%s %s %d' % (k, v, r) for k, (v, r) in g['gates'].items())
            out.append(s); n['pass'] += 1
        else:
            out.append('// NOT VERIFIED -- %s' % (g['verdict'] if g else 'not gated in the clean build')); n['unverified'] += 1
    open(p, 'w').write('\n'.join(out))
print(n)
