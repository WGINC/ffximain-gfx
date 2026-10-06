"""test_skinmerge.py -- speedfix's fork of CYyModelDt::SkinAndDrawV2 (fork/CYyModelDt__SkinAndDrawV2.fork.cpp,
docs/SKINMERGE.md) against the game's function:

  1. merge off (xi_skinmerge_on 0, as speedfix leaves it with skin_merge=0): fork.py gate, no declarations -- the fork
     must be the game's function;
  2. merge on (a copy compiled with XI_SKINMERGE_GATE, which starts xi_skinmerge_on at 1): fork.py gate --declare
     fork/skinmerge.declare.json -- every difference must be one the declarations name. The fuzzed inputs do not
     reach a held record (no trial shows a declared difference), so:
  3. merge on, replayed from captured calls (pipeline/xcap.py; the owner's captures, which are game data and stay out
     of the repository: CAPTURES=dir of *.xcap), with Model_BeginOffscreenPass (0x10033800) run as the game's own code
     instead of the gate's stub. The stub answers with any value, also "pass active" in a trial whose memory makes its
     guard false (word 0x1045B77A, float 0x1045B80C < 4.0, [this+8]): the hold condition is that same guard, so with
     the stub the original takes a path the game's code cannot. Required: no FAIL, and at least one trial in which
     the declared differences were seen (a merged draw ran).

    FFXI_DLL=... CAPTURES=dir python3 test_skinmerge.py"""
import glob, json, os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); SF = os.path.join(H, '..'); ROOT = os.path.abspath(os.path.join(SF, '..', '..'))
FORK = os.path.join(SF, 'fork', 'CYyModelDt__SkinAndDrawV2.fork.cpp'); DECL = os.path.join(SF, 'fork', 'skinmerge.declare.json')
NAME = 'fork_CYyModelDt__SkinAndDrawV2'
bad = 0


def last(path):
    return [json.loads(l) for l in open(path)][-1]


def say(ok, what):
    global bad
    print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok


w = tempfile.mkdtemp(prefix='skinmerge_')
# 1. merge off
subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate', FORK, '--trials', '30', '--work', os.path.join(w, 'off')],
               capture_output=True, text=True)
r = last(os.path.join(w, 'off', NAME, 'gate.jsonl'))
say(r['verdict'] == 'PASS', 'merge off: fork.py gate, no declarations: %s %s' % (r['verdict'], r.get('original_outcomes')))
# 2. merge on: the copy keeps the fork's first line (fork.py reads its header there)
src = open(FORK).read().split('\n', 1)
on = os.path.join(w, 'on_src', os.path.basename(FORK)); os.makedirs(os.path.dirname(on))
open(on, 'w').write(src[0] + '\n#define XI_SKINMERGE_GATE 1\n' + src[1])
subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate', on, '--declare', DECL, '--trials', '30',
                '--work', os.path.join(w, 'on')], capture_output=True, text=True)
G = os.path.join(w, 'on', NAME); r = last(os.path.join(G, 'gate.jsonl'))
say(r['verdict'] == 'PASS', 'merge on: fork.py gate --declare: %s %s (trials with a declared difference: %d)' % (
    r['verdict'], r.get('original_outcomes'), r['declared']['trials_with_declared_differences']))
# 3. captures
caps = sorted(glob.glob(os.path.join(os.environ.get('CAPTURES', ''), '*2DC10*.xcap'))) if os.environ.get('CAPTURES') else []
if not caps:
    print('-- no captures of SkinAndDrawV2 (CAPTURES=dir of xikit_capture_*_2DC10_*.xcap): step 3 not run')
else:
    case = json.load(open(os.path.join(G, 'case.json')))
    for c in case if isinstance(case, list) else [case]: c['stubs'].pop('0x10033800', None)
    cj = os.path.join(w, 'case_beginreal.json'); json.dump(case, open(cj, 'w'))
    seen = 0
    for x in caps:
        out = os.path.join(w, 'cap_' + os.path.basename(x)[:-5])
        subprocess.run([sys.executable, os.path.join(ROOT, 'pipeline', 'xcap.py'), x, cj, os.path.join(G, 'gfx_recon_fork.dll'), out,
                        '--trials', '8', '--insn-limit', '5000000'], capture_output=True, text=True)
        rs = glob.glob(os.path.join(out, '*.gate.jsonl'))
        if not rs: say(False, '%s: no result' % os.path.basename(x)); continue
        r = last(rs[0]); n = r['declared']['trials_with_declared_differences']; seen += n
        say(r['verdict'] in ('PASS', 'INCONCLUSIVE'), '%s: %s %s, trials with a declared difference: %d%s' % (
            os.path.basename(x), r['verdict'], r.get('original_outcomes'), n,
            ' -- ' + r.get('first_mismatch', {}).get('why', '') if r['verdict'] == 'FAIL' else ''))
    say(seen > 0, 'a merged draw ran in %d captured trial(s) and every difference was declared' % seen)
shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
