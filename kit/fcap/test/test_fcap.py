"""test_fcap.py -- the frame capture (kit/d3dprof/fcap.c) and its replayer (kit/fcap/replay.c), under Wine on Mesa's
software renderer in a virtual X server:
  1. fcap_host.exe draws game-like frames through Wine's d3d8 with d3dprof and fcap attached (as the kit attaches them
     to the game's device) and captures one frame;
  2. the replayer plays that capture through Wine's d3d8: its picture must equal the capture's own (the same device);
  3. the replayer plays it through kit/d3d8on11: its picture is compared with the capture's (compare.py: a pixel differs
     when a channel differs by more than 8; at most 1% may differ; the test's criteria);
  4. the host captures again with kit/d3d8on11 as its d3d8.dll (thread mode): the kit's hooks work on our device as on
     another d3d8.dll, and that capture replays through Wine's d3d8 to its own picture (the same criteria).
Every replay must run without a failed call, and the capture must carry no note (each note is something the capture
could not do).

    python3 test_fcap.py          (after kit/d3d8on11/build.sh; KEEP=1 keeps the folder)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); F = os.path.join(H, '..'); K = os.path.join(F, '..')
GCC = ['i686-w64-mingw32-gcc', '-O2', '-Wall', '-Wno-misleading-indentation', '-std=gnu99', '-static-libgcc']
subprocess.run(GCC + ['-DXIKIT', '-Wno-unused-function', '-o', os.path.join(H, 'fcap_host.exe'), os.path.join(H, 'fcap_host.c'),
                      os.path.join(K, 'd3dprof', 'd3dprof.c'), os.path.join(K, 'd3dprof', 'fcap.c'), '-ld3d8', '-lwinmm'], check=True)
subprocess.run(['bash', os.path.join(F, 'build.sh')], check=True, capture_output=True)
w = tempfile.mkdtemp(prefix='fcap_'); bad = 0


def wine(exe, args, cwd, ours=False):
    env = dict(os.environ, WINEDEBUG='-all')
    if ours: env['WINEDLLOVERRIDES'] = 'd3d8=n'
    r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', exe] + args, cwd=cwd, capture_output=True,
                       text=True, timeout=600, env=env)
    return '\n'.join(l for l in r.stdout.splitlines() if 'X connection' not in l)


def check(ok, what):
    global bad
    print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok


shutil.copy(os.path.join(H, 'fcap_host.exe'), w)
out = wine('fcap_host.exe', ['Z:' + w.replace('/', '\\') + '\\d3dprof.csv'], w)
print(out)
caps = [f for f in os.listdir(w) if f.endswith('.fcap')]
check(len(caps) == 1, 'the host wrote one capture')
if caps:
    cap = os.path.join(w, caps[0])
    for name, ours in (('wine', False), ('ours', True)):
        d = os.path.join(w, name); os.makedirs(d)
        shutil.copy(os.path.join(F, 'dist', 'fcap_replay.exe'), d); shutil.copy(cap, d)
        if ours: shutil.copy(os.path.join(K, 'd3d8on11', 'dist', 'd3d8.dll'), d)
        o = wine('fcap_replay.exe', [caps[0], name], d, ours)
        print('== replay through %s\n%s' % ("Wine's d3d8" if not ours else 'd3d8on11', o))
        check(' 0 failures' in o, 'replay through %s: no call failed' % name)
        check('capture note' not in o, 'the capture carries no note')
        if ours: check(os.path.exists(os.path.join(d, 'd3d8on11.log')), 'd3d8on11 was the d3d8.dll loaded')
        a, b = os.path.join(d, name + '_capture.pic'), os.path.join(d, name + '_replay.pic')
        if os.path.exists(a) and os.path.exists(b):
            args = [sys.executable, os.path.join(F, 'compare.py'), a, b, '--png', os.path.join(w, name + '.png')]
            if not ours: args += ['--tol', '0', '--maxfrac', '0']
            r = subprocess.run(args, capture_output=True, text=True)
            check(r.returncode == 0, 'replay through %s against the captured picture: %s' % (name, r.stdout.strip()))
        else: check(False, 'replay through %s: both pictures written' % name)
# 4. the kit's hooks on our device: capture through d3d8on11, replay through Wine's d3d8
o4 = os.path.join(w, 'ours_capture'); os.makedirs(o4)
shutil.copy(os.path.join(H, 'fcap_host.exe'), o4); shutil.copy(os.path.join(K, 'd3d8on11', 'dist', 'd3d8.dll'), o4)
print(wine('fcap_host.exe', ['Z:' + o4.replace('/', '\\') + '\\d3dprof.csv'], o4, True))
caps = [f for f in os.listdir(o4) if f.endswith('.fcap')]
check(len(caps) == 1 and os.path.exists(os.path.join(o4, 'd3d8on11.log')), 'the host captured one frame through d3d8on11')
if len(caps) == 1:
    d = os.path.join(o4, 'wine'); os.makedirs(d)
    shutil.copy(os.path.join(F, 'dist', 'fcap_replay.exe'), d); shutil.copy(os.path.join(o4, caps[0]), d)
    o = wine('fcap_replay.exe', [caps[0], 'x'], d)
    print('== a capture made through d3d8on11, replayed through Wine\'s d3d8\n%s' % o)
    check(' 0 failures' in o and 'capture note' not in o, 'that replay: no call failed, no note')
    a, b = os.path.join(d, 'x_capture.pic'), os.path.join(d, 'x_replay.pic')
    if os.path.exists(a) and os.path.exists(b):
        r = subprocess.run([sys.executable, os.path.join(F, 'compare.py'), a, b], capture_output=True, text=True)
        check(r.returncode == 0, 'captured through ours, replayed through Wine: %s' % r.stdout.strip())
    else: check(False, 'that replay: both pictures written')
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
