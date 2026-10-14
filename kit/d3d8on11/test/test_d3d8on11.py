"""test_d3d8on11.py -- d3d8on11 (kit/d3d8on11) offline, under Wine on Mesa's software renderer in a virtual X server:
  1. t8.exe basic through Wine's own d3d8 (the reference: Wine's implementation, itself tested against Windows) and
     through ours: every check must pass on both;
  2. t8.exe defaults through both: every render state and texture stage state of a fresh device, compared; each
     difference is listed (a difference is ours to explain against the DirectX 8 SDK reference, not to copy from Wine).

    python3 test_d3d8on11.py          (after build.sh; needs wine, xvfb-run)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); D8 = os.path.join(H, '..')
bad = 0
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-o', os.path.join(H, 't8.exe'), os.path.join(H, 't8.c'), '-ld3d8', '-static-libgcc'], check=True)
w = tempfile.mkdtemp(prefix='d3d8on11_')
ours = os.path.join(w, 'ours'); ref = os.path.join(w, 'ref'); os.makedirs(ours); os.makedirs(ref)
for d in (ours, ref): shutil.copy(os.path.join(H, 't8.exe'), d)
shutil.copy(os.path.join(D8, 'dist', 'd3d8.dll'), ours)


def run(where, mode, override):
    env = dict(os.environ, WINEDEBUG='-all')
    if override: env['WINEDLLOVERRIDES'] = 'd3d8=n'
    r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', mode], cwd=where,
                       capture_output=True, text=True, timeout=600, env=env)
    return r.stdout


for name, where, ov in (('Wine\'s d3d8', ref, False), ('d3d8on11', ours, True)):
    out = run(where, 'basic', ov)
    print('== basic, %s' % name); print(out.strip())
    if 'ALL OK' not in out: bad += 1
log = os.path.join(ours, 'd3d8on11.log')
ok = os.path.exists(log) and 'Direct3DCreate8' in open(log).read()
print('%s: d3d8on11 was the d3d8.dll loaded (its log exists)' % ('ok' if ok else 'FAIL')); bad += not ok
if os.path.exists(log): print('-- d3d8on11.log --'); print(open(log).read().strip())

a = dict(l.rsplit(' ', 1) for l in run(ref, 'defaults', False).splitlines() if l[:3] in ('rs ', 'tss'))
b = dict(l.rsplit(' ', 1) for l in run(ours, 'defaults', True).splitlines() if l[:3] in ('rs ', 'tss'))
diff = sorted(k for k in set(a) | set(b) if a.get(k) != b.get(k))
print('== defaults: %d states read on Wine\'s d3d8, %d on ours, %d differ' % (len(a), len(b), len(diff)))
for k in diff: print('  %-10s Wine %s  ours %s' % (k, a.get(k, '(fails)'), b.get(k, '(fails)')))
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
