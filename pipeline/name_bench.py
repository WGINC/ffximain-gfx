"""name_bench.py -- measure the naming process against names that are known, before trusting it on unknown ones.

Ground truth, two kinds, reported apart:
  * `ps2`: the PS2 build's own symbol for a PC function matched to it on a unique literal (data/ps2_matches.json,
    kinds string/strings). These are the developers' names.
  * `dm`: DancingMad's name (data/dm_names.json). Earlier sessions' work, reviewed by the owner but not proven:
    agreement with it is a softer measure.
Each target's dossier is the one a naming session would get (batch.py, list scope), with the target's own known
names removed: every spelling of them (Class_Method, Class::Method, the tree's definition name) becomes the
placeholder fn_XXXXXXXX, and its PS2 match line is dropped. The answer key is written apart from the dossiers.

    FFXI_DLL=... python3 name_bench.py TREE OUT_DIR KEY.json [N_DM] [SEED]
Writes OUT_DIR/bench_NN.md (10 functions each) and KEY.json ({addr: {truth, source}}); prints any target name still
found in a dossier (a leak), which must be none."""
import glob, json, os, random, re, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data'); sys.path.insert(0, P)
import batch as B


def main(tree, outdir, keyf, n_dm='77', seed='1'):
    tf = B.tree_functions(tree)
    fs = json.load(open(os.path.join(D, 'frame_set.json')))['functions']
    m = json.load(open(os.path.join(D, 'ps2_matches.json')))
    dm = {int(k, 16): v[0] for k, v in json.load(open(os.path.join(D, 'dm_names.json'))).items() if not k.startswith('_')}
    key = {}
    for k, v in m.items():
        if not k.startswith('_') and v['kind'] in ('string', 'strings') and int(k, 16) in tf:
            key[int(k, 16)] = {'truth': v['name'], 'source': 'ps2', 'ps2_symbol': v['ps2']}
    pool = sorted(a for a in dm if a in tf and '0x%08X' % a in fs and a not in key)
    for a in random.Random(int(seed)).sample(pool, min(int(n_dm), len(pool))):
        key[a] = {'truth': dm[a], 'source': 'dm'}
    os.makedirs(outdir, exist_ok=True)
    lst = os.path.join(outdir, '_targets.json')
    json.dump(['0x%08X' % a for a in sorted(key)], open(lst, 'w'))
    tmp = os.path.join(outdir, '_raw')
    subprocess.run([sys.executable, os.path.join(P, 'batch.py'), tree, tmp, '10', 'list:' + lst, 'bench_'], check=True)
    # every spelling of each target's known names
    spell = {}
    for a, v in key.items():
        names = {v['truth'], dm.get(a, ''), tf[a][1], (m.get('0x%08X' % a) or {}).get('name', '')}
        for n in list(names):
            if '::' in n: names.add(n.replace('::', '_'))
            if '_' in n and n.count('_') == 1 and not n.startswith(('fn_', 'sub_', 'FUN_')): names.add(n.replace('_', '::'))
        spell[a] = sorted((n for n in names if n and not B.placeholder(n)), key=len, reverse=True)
    idx = json.load(open(os.path.join(tmp, 'index.json')))['batches']
    leaks = []
    for n, b in enumerate(idx, 1):
        t = open(os.path.join(tmp, b['batch'] + '.md')).read()
        t = re.sub(r'^# Batch .*$', '# Benchmark batch %02d: %d functions' % (n, len(b['functions'])), t, count=1, flags=re.M)
        t = '\n'.join(l for l in t.split('\n') if not l.startswith('- **PS2 match**'))
        for a in (int(x, 16) for x in b['functions']):
            for s in spell[a]:
                t = re.sub(r'(?:\borig::)?(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])' % re.escape(s), 'fn_%08X' % a, t)
        for a in key:
            for s in spell[a]:
                if re.search(r'(?<![A-Za-z0-9_])%s(?![A-Za-z0-9_])' % re.escape(s), t) and a in (int(x, 16) for x in b['functions']):
                    leaks.append((b['batch'], '0x%08X' % a, s))
        open(os.path.join(outdir, 'bench_%02d.md' % n), 'w').write(t)
    json.dump({'0x%08X' % a: v for a, v in sorted(key.items())}, open(keyf, 'w'), indent=1)
    for f in glob.glob(os.path.join(tmp, '*')): os.remove(f)
    os.rmdir(tmp)
    print('%d targets (%d ps2, %d dm) in %d batches; leaks: %s' % (
        len(key), sum(v['source'] == 'ps2' for v in key.values()), sum(v['source'] == 'dm' for v in key.values()),
        len(idx), leaks or 'none'))


if __name__ == '__main__':
    main(*sys.argv[1:6])
