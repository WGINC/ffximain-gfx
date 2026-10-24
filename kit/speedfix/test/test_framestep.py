"""test_framestep.py -- speedfix's frame_step and uncap forks against the game's functions (fork/Frame_Render.fork.cpp,
fork/Camera_GetFrameScale.fork.cpp, fork/Camera_GetClampedField28.fork.cpp; all three from the verified tree):

  1. switches off (xi_step_mode 0, xi_uncap 0, as speedfix leaves them with frame_step=0 and uncap=0): fork.py gate,
     no declarations -- each fork must be the game's function;
  2. switches on (a copy compiled with XI_FRAMESTEP_GATE, which starts them on): fork.py gate --declare, every
     difference one the declarations name (fork/framestep.declare.json for Frame_Render,
     fork/framestep_getter.declare.json for the getters). The getters' declared difference (the return) must be
     seen; Frame_Render's top-of-frame step (its call of 0x1000D3F0) must be seen;
  3. Frame_Render with the switches on, replayed from captured calls (pipeline/xcap.py; the owner's captures stay out of
     the repository: CAPTURES=dir of xikit_capture_*_11A00_*.xcap): the wait and the step on the game's own state.
     Until the timer calls in the wait (0x10012B28, 0x10012B86, 0x10012B91, 0x10012BCB, 0x10012BF5) have measured
     facts (xikit facts=1), a trial reaching them is UNRESOLVED and says so.

    FFXI_DLL=... [CAPTURES=dir [CAPTURE_BUILD=kit/xikit/builds/<stamp>.json]] python3 test_framestep.py
  (CAPTURE_BUILD: the captures were taken on that translated build, as xikit.log names it; pipeline/xcap.py --build)"""
import glob, json, os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); SF = os.path.join(H, '..'); ROOT = os.path.abspath(os.path.join(SF, '..', '..'))
FORKS = [('Frame_Render', 'framestep.declare.json'), ('Camera_GetFrameScale', 'framestep_getter.declare.json'),
         ('Camera_GetClampedField28', 'framestep_getter.declare.json')]
bad = 0


def last(path):
    return [json.loads(l) for l in open(path)][-1]


def say(ok, what):
    global bad
    print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok


def gate(src, work, *extra):
    subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate', src, '--trials', '100', '--work', work] + list(extra),
                   capture_output=True, text=True)
    g = glob.glob(os.path.join(work, 'fork_*', 'gate.jsonl'))
    return last(g[0]) if g else {'verdict': 'no result'}, (os.path.dirname(g[0]) if g else None)


w = tempfile.mkdtemp(prefix='framestep_')
gdir = {}
for name, decl in FORKS:
    fork = os.path.join(SF, 'fork', name + '.fork.cpp')
    r, _ = gate(fork, os.path.join(w, 'off_' + name))
    say(r['verdict'] == 'PASS', '%s, switches off: fork.py gate, no declarations: %s %s' % (name, r['verdict'], r.get('original_outcomes')))
    # the copy keeps the fork's first line (fork.py reads its header there); its includes are made absolute
    src = open(fork).read().replace('"../sf_step_sites.h"', '"%s"' % os.path.join(SF, 'sf_step_sites.h')).split('\n', 1)
    on = os.path.join(w, 'on_src_' + name, os.path.basename(fork)); os.makedirs(os.path.dirname(on))
    open(on, 'w').write(src[0] + '\n#define XI_FRAMESTEP_GATE 1\n' + src[1])
    r, gdir[name] = gate(on, os.path.join(w, 'on_' + name), '--declare', os.path.join(SF, 'fork', decl))
    seen = (r.get('declared') or {}).get('seen', {})
    n = (r.get('declared') or {}).get('trials_with_declared_differences', 0)
    say(r['verdict'] == 'PASS' and n > 0, '%s, switches on: fork.py gate --declare: %s %s (trials with a declared difference: %d)' % (
        name, r['verdict'], r.get('original_outcomes'), n))
caps = sorted(glob.glob(os.path.join(os.environ.get('CAPTURES', ''), '*_11A00_*.xcap'))) if os.environ.get('CAPTURES') else []
if not caps:
    print('-- no captures of Frame_Render (CAPTURES=dir of xikit_capture_*_11A00_*.xcap): step 3 not run')
else:
    G = gdir['Frame_Render']; case = os.path.join(G, 'case.json')
    for x in caps:
        out = os.path.join(w, 'cap_' + os.path.basename(x)[:-5])
        subprocess.run([sys.executable, os.path.join(ROOT, 'pipeline', 'xcap.py'), x, case, os.path.join(G, 'gfx_recon_fork.dll'), out,
                        '--trials', '8', '--insn-limit', '20000000'] + (['--build', os.environ['CAPTURE_BUILD']] if os.environ.get('CAPTURE_BUILD') else []),
                       capture_output=True, text=True)
        rs = glob.glob(os.path.join(out, '*.gate.jsonl'))
        if not rs: say(False, '%s: no result' % os.path.basename(x)); continue
        r = last(rs[0])
        say(r['verdict'] in ('PASS', 'UNRESOLVED', 'INCONCLUSIVE'), '%s: %s %s, unresolved %s%s' % (
            os.path.basename(x), r['verdict'], r.get('original_outcomes'), r.get('unresolved'),
            ' -- ' + (r.get('first_mismatch') or {}).get('why', '') if r['verdict'] == 'FAIL' else ''))
shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
