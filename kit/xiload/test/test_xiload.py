"""test_xiload.py -- xiload.exe under Wine: a stand-in pol.exe (target.c) starts, xiload --once loads marker.dll into
it; the target sees the module, the DLL's DllMain ran in the target's process, and xiload.log says so.

    python3 test_xiload.py        (after ../build.sh; needs wine)"""
import os, shutil, subprocess, sys, tempfile, time
H = os.path.dirname(os.path.abspath(__file__)); d = tempfile.mkdtemp(prefix='xiload_')
cc = lambda out, src, *a: subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-o', os.path.join(d, out), os.path.join(H, src), '-static-libgcc'] + list(a), check=True)
cc('pol.exe', 'target.c'); cc('marker.dll', 'marker.c', '-shared')
shutil.copy(os.path.join(H, '..', 'dist', 'xiload.exe'), d)
env = dict(os.environ, WINEDEBUG='-all')
ld = subprocess.Popen(['wine', 'xiload.exe', '--once', '--dll', 'marker.dll'], cwd=d, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
time.sleep(3)
tg = subprocess.run(['wine', 'pol.exe'], cwd=d, env=env, capture_output=True, text=True, timeout=120)
try: out = ld.communicate(timeout=60)[0]
except subprocess.TimeoutExpired: ld.kill(); out = ld.communicate()[0]
print(out.strip()); print(tg.stdout.strip())
log = open(os.path.join(d, 'xiload.log')).read() if os.path.exists(os.path.join(d, 'xiload.log')) else ''
mk = open(os.path.join(d, 'marker.txt')).read().strip() if os.path.exists(os.path.join(d, 'marker.txt')) else ''
ok = tg.returncode == 0 and 'marker.dll loaded' in tg.stdout and mk and ' loaded at ' in log and ld.returncode == 0
print('%s: xiload --once found pol.exe and loaded marker.dll into it (DllMain ran in process %s)' % ('ok' if ok else 'FAIL', mk or '-'))
shutil.rmtree(d, ignore_errors=True)
sys.exit(0 if ok else 1)
