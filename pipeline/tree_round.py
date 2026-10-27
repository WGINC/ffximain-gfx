"""tree_round.py -- one round of the clean tree, from tree/source.json: regenerate, gate under both cases, install.

The steps the 27 Oct rounds (r12, r13) ran by hand, in one place. Each step is a subcommand; run them in order.

    export FFXI_DLL=... XI_REGATE=verification/gate_raw_regate.jsonl
    python3 pipeline/tree_round.py regen OUT [--first fsA ...] [--last fsB ...] [--drop 0xADDR ...]
    python3 pipeline/tree_round.py cases OUT [--first fsA ...] [--last fsB ...]
    python3 pipeline/tree_round.py gate OUT [--jobs 2]
    python3 pipeline/tree_round.py install OUT

regen    the export list is tree/source.json's (IDA export, then `ghidra_exports` in order); --first puts the named
         exports (data/exports/ghidra_frameset<N>.jsonl with N from fsN, raw gate verification/gate_raw_frameset<N>.jsonl)
         right after the IDA export, --last after the others. hx2clean.py takes each function from the first export
         whose raw gate passed it, so --first is how a re-exported record replaces an older one. --drop ADDR...
         --why TEXT: left out (XI_DROP, the file OUT.given_drop.json), each listed in left_out.json with TEXT.
cases    the raw cases: verification/cases_raw_final.json; for a function a --first export passes raw, that export's
         case (runs/fsN/cases.json, made with `./run.sh translate|cases` if missing), the older one kept as a second
         case when it differs (verification/cases_raw_alt.json); a function with no case takes the --last export's.
         Both mapped to OUT's DLL (map_clean_cases.py).
gate     every function under its case, and every function with a second case under that one too; the stricter
         verdict stands (FAIL > UNRESOLVED > INCONCLUSIVE > PASS). A function that stopped at an import under an older
         case without "imports": "stub" is gated again with the stub added (the fs19 rule: imports answered like stubbed
         game callees, data/import_pops.json).
install  verification/gate_clean_final.jsonl, cases_clean_final.json, cases_raw_final.json, cases_raw_alt.json and
         gate_clean_altcases_tree.jsonl from OUT; the tree copied in, tree/source.json kept (and given the export order
         the round used). Then run: regen_facts.py, frame_status.py, verify_all.py --quick --facts-from-state.
"""
import argparse, collections, hashlib, json, os, shutil, subprocess, sys
P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')
R = lambda *p: os.path.join(ROOT, *p)
n = lambda a: '0x%08X' % int(a, 16)
RANK = {'PASS': 0, 'INCONCLUSIVE': 1, 'UNRESOLVED': 2, 'FAIL': 3}


def export_of(fs): return 'data/exports/ghidra_frameset%s.jsonl' % fs[2:], 'verification/gate_raw_frameset%s.jsonl' % fs[2:]


def order(o):
    s = json.load(open(R('tree', 'source.json')))
    first = [dict(zip(('path', 'raw_gate'), export_of(f))) for f in o.first]
    last = [dict(zip(('path', 'raw_gate'), export_of(f))) for f in o.last]
    have = {e['path'] for e in first + last}
    mid = [e for e in s['ghidra_exports'] if e['path'] not in have]
    exps = [('data/exports/gfx_export.jsonl', s['raw_gate'])]
    exps += [(e['path'], e.get('raw_gate') or s['ghidra_raw_gate']) for e in first + mid + last]
    return s, first + mid + last, exps


def regen(o):
    _, _, exps = order(o)
    env = dict(os.environ)
    if o.drop:                     # XI_DROP is a file {ea: why} (hx2clean.py, drop_failed.py); OUT itself is rebuilt
        if not o.why: sys.exit('--drop needs --why (the reason left_out.json gives)')
        f = o.out.rstrip('/') + '.given_drop.json'
        json.dump({n(a): 'left out: ' + o.why for a in o.drop}, open(f, 'w'), indent=1); env['XI_DROP'] = f
    env.setdefault('XI_REGATE', R('verification', 'gate_raw_regate.jsonl'))
    cmd = [R('pipeline', 'regen_clean.sh'), ','.join(e for e, _ in exps), ','.join(g for _, g in exps), o.out]
    print(' '.join(cmd)); sys.exit(subprocess.call(cmd, cwd=ROOT, env=env))


def run_cases(fs):
    c = R('runs', fs + 'c', 'cases.json')
    if not os.path.exists(c):
        e, _ = export_of(fs)
        for step in ('translate', 'cases'):
            subprocess.check_call([R('run.sh'), step, e, fs + 'c'], cwd=ROOT)
    return {n(x['original']): x for x in json.load(open(c))}


def cases(o):
    raw = {n(c['original']): c for c in json.load(open(R('verification', 'cases_raw_final.json')))}
    alt = {n(c['original']): c for c in json.load(open(R('verification', 'cases_raw_alt.json')))}
    place = {n(k) for k in json.load(open(os.path.join(o.out, 'placement.json'))) if k.startswith('0x')}
    passes = lambda fs: {n(json.loads(l)['original']) for l in open(R(export_of(fs)[1])) if json.loads(l)['verdict'] == 'PASS'}
    for fs in o.first:
        cc, ok = run_cases(fs), passes(fs)
        for k in place & ok & set(cc):
            if k in raw and json.dumps(raw[k], sort_keys=True) != json.dumps(cc[k], sort_keys=True): alt[k] = raw[k]
            raw[k] = cc[k]
    for fs in o.last:
        cc, ok = run_cases(fs), passes(fs)
        for k in place & ok & set(cc):
            raw.setdefault(k, cc[k])
    miss = sorted(place - set(raw))
    if miss: print('no case for %d: %s' % (len(miss), miss[:20]))
    for name, d in (('cases_raw.json', raw), ('alt_raw.json', {k: v for k, v in alt.items() if k in place})):
        json.dump(list(d.values()), open(os.path.join(o.out, name), 'w'))
    dll = os.path.join(o.out, 'build', 'gfx_recon.dll')
    for a, b in (('cases_raw.json', 'cases_clean.json'), ('alt_raw.json', 'alt_clean.json')):
        subprocess.check_call([sys.executable, R('pipeline', 'map_clean_cases.py'), dll, os.path.join(o.out, 'placement.json'),
                               os.path.join(o.out, a), os.path.join(o.out, b)])


def gate_one(o, cases_file, name):
    d = os.path.join(o.out, name); shutil.rmtree(d, ignore_errors=True); os.makedirs(d)
    subprocess.check_call([sys.executable, R('pipeline', 'gate_some.py'), d, 'all', '--dll', os.path.join(o.out, 'build', 'gfx_recon.dll'),
                           '--cases', cases_file, '--out', 'gate', '--jobs', str(o.jobs)])
    return {n(x['original']): x for x in map(json.loads, open(os.path.join(d, 'gate.jsonl')))}


def gate(o):
    a = gate_one(o, os.path.join(o.out, 'cases_clean.json'), 'g_main')
    b = gate_one(o, os.path.join(o.out, 'alt_clean.json'), 'g_alt')
    fin = {k: (b[k] if k in b and RANK[b[k]['verdict']] > RANK[x['verdict']] else x) for k, x in a.items()}
    cc = json.load(open(os.path.join(o.out, 'cases_clean.json')))
    stub = [dict(c, imports='stub') for c in cc if 'imports' not in c and fin.get(n(c['original']), {}).get('verdict') == 'INCONCLUSIVE'
            and 'import' in (fin[n(c['original'])].get('original_outcomes') or {})]
    if stub:
        json.dump(stub, open(os.path.join(o.out, 'stub_clean.json'), 'w'))
        for k, x in gate_one(o, os.path.join(o.out, 'stub_clean.json'), 'g_stub').items(): fin[k] = x
        json.dump(sorted(n(c['original']) for c in stub), open(os.path.join(o.out, 'stubbed.json'), 'w'))
    json.dump(fin, open(os.path.join(o.out, 'final.json'), 'w'))
    old = {n(json.loads(l)['original']): json.loads(l)['verdict'] for l in open(R('verification', 'gate_clean_final.jsonl'))}
    print(len(fin), dict(collections.Counter(x['verdict'] for x in fin.values())))
    for k in sorted(fin):
        if fin[k]['verdict'] != old.get(k): print('  %s %s -> %s' % (k, old.get(k), fin[k]['verdict']))


def install(o):
    fin = json.load(open(os.path.join(o.out, 'final.json')))
    open(R('verification', 'gate_clean_final.jsonl'), 'w').write(''.join(json.dumps(fin[k]) + '\n' for k in sorted(fin)))
    stub = set(json.load(open(os.path.join(o.out, 'stubbed.json')))) if os.path.exists(os.path.join(o.out, 'stubbed.json')) else set()
    for src, dst in (('cases_clean.json', 'cases_clean_final.json'), ('cases_raw.json', 'cases_raw_final.json')):
        cc = json.load(open(os.path.join(o.out, src)))
        for c in cc:
            if n(c['original']) in stub: c['imports'] = 'stub'
        json.dump(cc, open(R('verification', dst), 'w'))
    shutil.copy(os.path.join(o.out, 'alt_raw.json'), R('verification', 'cases_raw_alt.json'))
    shutil.copy(os.path.join(o.out, 'g_alt', 'gate.jsonl'), R('verification', 'gate_clean_altcases_tree.jsonl'))
    s, exps, _ = order(o)
    old = {e['path']: e for e in s['ghidra_exports']}
    s['ghidra_exports'] = [dict(old.get(e['path'], {'_doc': 'added by tree_round.py; see docs/HISTORY.md'}), path=e['path'], **({'raw_gate': e['raw_gate']} if e.get('raw_gate') else {}),
                                sha1=hashlib.sha1(open(R(e['path']), 'rb').read()).hexdigest()) for e in exps]
    subprocess.check_call([sys.executable, R('pipeline', 'annotate_gate.py'), o.out, R('verification', 'gate_clean_final.jsonl')])
    keep = json.dumps(s, indent=1)
    shutil.rmtree(R('tree')); shutil.copytree(o.out, R('tree'), ignore=shutil.ignore_patterns('build', 'b', 'g_*', '*.json.new', 'cases_*.json',
                                                                                       'alt_*.json', 'stub*.json', 'final.json', 'drop_reasons.json'))
    open(R('tree', 'source.json'), 'w').write(keep)
    print('installed; now: regen_facts.py, frame_status.py verification/frame_set_status.json, verify_all.py --quick --facts-from-state')


if __name__ == '__main__':
    ap = argparse.ArgumentParser(); ap.add_argument('step', choices=('regen', 'cases', 'gate', 'install')); ap.add_argument('out')
    ap.add_argument('--first', nargs='*', default=[]); ap.add_argument('--last', nargs='*', default=[])
    ap.add_argument('--drop', nargs='*', default=[]); ap.add_argument('--why', default='')
    ap.add_argument('--jobs', type=int, default=2)
    o = ap.parse_args(); o.out = os.path.abspath(o.out)
    {'regen': regen, 'cases': cases, 'gate': gate, 'install': install}[o.step](o)
