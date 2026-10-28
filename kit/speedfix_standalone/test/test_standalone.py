"""test_standalone.py -- speedfix_standalone.dll under Wine with the real FFXiMain.dll, no xikit.

kit/speedfix/test/sf_host.c (a stand-in for pol.exe: the game DLL mapped without running it, a fake CDx whose device
records what reaches it, the render thread played) built with the standalone DLL and its log in place of xikit.dll and
xikit.log; each of speedfix's test modes run, every "FAIL:" line it prints counted, and the log's lines checked.

    FFXI_DLL=... python3 kit/speedfix_standalone/test/test_standalone.py      (after ../build.sh; needs wine)"""
import os, re, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); SA = os.path.dirname(H); ROOT = os.path.join(SA, '..', '..')
src = open(os.path.join(ROOT, 'kit', 'speedfix', 'test', 'sf_host.c')).read()
src = src.replace('"xikit.dll"', '"speedfix_standalone.dll"').replace('"xikit.log"', '"speedfix_standalone.log"')
src = src.replace('"speedfix.dll"', '"speedfix_standalone.dll"').replace('methods hooked by mods', 'methods hooked by speedfix')
d = tempfile.mkdtemp(prefix='sfs_test_', dir=os.environ.get('SFS_TMP'))
open(os.path.join(d, 'sfs_host_test.c'), 'w').write(src)
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-Wno-unused-parameter', '-o', os.path.join(d, 'sf_host.exe'),
                os.path.join(d, 'sfs_host_test.c'), '-static-libgcc', '-lwinmm', '-w'], check=True)
for f in os.listdir(os.path.join(SA, 'dist')): shutil.copy(os.path.join(SA, 'dist', f), d)
shutil.copy(os.environ['FFXI_DLL'], os.path.join(d, 'FFXiMain.dll'))
bad = 0
for mode in ('minimal', 'full', 'lazy', 'blocks', 'pacing', 'step', 'group'):
    for f in ('speedfix_standalone.log', 'speedfix.ini'):
        if os.path.exists(os.path.join(d, f)): os.remove(os.path.join(d, f))
    r = subprocess.run(['wine', 'sf_host.exe', mode], cwd=d, capture_output=True, text=True, timeout=900, env=dict(os.environ, WINEDEBUG='-all'))
    log = open(os.path.join(d, 'speedfix_standalone.log')).read() if os.path.exists(os.path.join(d, 'speedfix_standalone.log')) else ''
    fails = [l for l in r.stdout.splitlines() if l.startswith('FAIL')]
    oks = sum(1 for l in r.stdout.splitlines() if l.startswith('ok'))
    print('%s: %s: %d checks ok, %d FAIL, exit %d' % ('ok' if not fails and r.returncode == 0 else 'FAIL', mode, oks, len(fails), r.returncode))
    for l in fails[:10]: print('   ', l)
    bad += bool(fails) or r.returncode != 0
    if mode == 'minimal':
        ok = 'speedfix: 20 device methods hooked, 3 flush guards' in log and 'speedfix init returned 0' in log
        print('%s: minimal: speedfix started by the standalone host, its 20 device methods hooked' % ('ok' if ok else 'FAIL')); bad += not ok
    if mode == 'step':
        ok = all(('hooked %s' % n) in log for n in ('Camera_GetFrameScale', 'Camera_GetClampedField28', 'Frame_Render')) and re.search(r'function hooks: ([3-9]|\d\d) written, 0 refused', log)
        print('%s: step: the three frame_step forks hooked through trampolines and written at the first frame' % ('ok' if ok else 'FAIL')); bad += not ok
print('-- speedfix_standalone.log (last run) --'); print(log[-2500:])
print('ALL OK' if not bad else '%d FAILED' % bad)
shutil.rmtree(d, ignore_errors=True)
sys.exit(1 if bad else 0)
