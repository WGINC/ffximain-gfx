"""test_xikit.py -- the kit end to end under Wine, with the real FFXiMain.dll ($FFXI_DLL) and the built dist/:
host.exe (host.c) maps the game DLL without running it, fakes the device, loads xikit.dll and plays the render
thread. Run twice: with the game DLL at its analysed base, and loaded elsewhere (0x10000000 occupied first).

    python3 test_xikit.py            (after ../build.sh; needs wine)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); DIST = os.path.join(H, '..', 'dist')
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-o', os.path.join(H, 'host.exe'), os.path.join(H, 'host.c'), '-static-libgcc'], check=True)
bad = 0
for mode in ('base', 'relocated'):
    d = tempfile.mkdtemp(prefix='xikit_')
    for f in ('xikit.dll', 'gfx_recon_ingame.dll', 'xikit_manifest.txt'): shutil.copy(os.path.join(DIST, f), d)
    shutil.copy(os.environ['FFXI_DLL'], os.path.join(d, 'FFXiMain.dll')); shutil.copy(os.path.join(H, 'host.exe'), d)
    env = dict(os.environ, WINEDEBUG='-all')
    r = subprocess.run(['wine', 'host.exe', mode], cwd=d, capture_output=True, text=True, timeout=600, env=env)
    print('== %s (exit %d)' % (mode, r.returncode)); print(r.stdout.strip())
    log = open(os.path.join(d, 'xikit.log')).read() if os.path.exists(os.path.join(d, 'xikit.log')) else ''
    print('-- xikit.log --'); print('\n'.join(l for l in log.splitlines() if 'swapped ' not in l and 'restored ' not in l)[-3000:])
    if r.returncode != 0 or 'ALL OK' not in r.stdout: bad += 1
    shutil.rmtree(d, ignore_errors=True)
sys.exit(1 if bad else 0)
