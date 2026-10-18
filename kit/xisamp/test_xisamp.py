"""test_xisamp.py -- the native sampler offline: dist/xisamp_x64.exe under Wine (64-bit) sampling test/target.exe,
an x86 process with a known call chain (target.c: main -> chain_outer -> chain_inner -> spin, then -> nap).
What this shows: the sampler opens, suspends and reads a WOW64 thread; the x86 context, the x86 stack scan and the
module table and export labels are right (each return address checked against the target's own disassembly), the
file is written each window and at the target's exit, and read.py reads it. What it cannot show: the ARM64 build
(no ARM64 Windows here: only that it is an ARM64 image) and its native frame-pointer chain -- Wine's WOW64 runs the
x86 code natively, so the native pc there is the x86 eip.

    python3 test_xisamp.py          (after build.sh; needs wine with 64-bit support)"""
import os, re, shutil, subprocess, sys, tempfile, time
H = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(H, 'dist')
sys.path.insert(0, H)
import read   # noqa: E402
w = tempfile.mkdtemp(prefix='xisamp_')
env = dict(os.environ, WINEDEBUG='-all', WINEPREFIX=os.path.join(w, 'pfx'))
for f in (os.path.join(D, 'xisamp_x64.exe'), os.path.join(H, 'test', 'target.exe')): shutil.copy(f, w)
bad = 0
def check(ok, what): global bad; print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok
subprocess.run(['wineboot', '-i'], env=env, capture_output=True, timeout=600)
# the return addresses the chain must show: the instruction after each call, from the target's own disassembly
dis = subprocess.run(['i686-w64-mingw32-objdump', '-d', os.path.join(w, 'target.exe')], capture_output=True, text=True).stdout
lines = dis.splitlines(); sym = {}
for l in lines:
    m = re.match(r'([0-9a-f]+) <_(\w+)>:', l)
    if m: sym[m.group(2)] = int(m.group(1), 16)
def after(caller, callee):
    fn = None
    for i, l in enumerate(lines):
        m = re.match(r'[0-9a-f]+ <_(\w+)>:', l)
        if m: fn = m.group(1)
        if fn == caller and re.search(r'call\s+[0-9a-f]+ <_%s>' % callee, l):
            return int(lines[i + 1].split(':')[0], 16)
ra_spin, ra_inner, ra_outer = after('chain_inner', 'spin'), after('chain_outer', 'chain_inner'), after('main', 'chain_outer')
tgt = subprocess.Popen(['wine', 'target.exe', 'pt.txt', '6000'], cwd=w, env=env)
for _ in range(300):
    p = os.path.join(w, 'pt.txt')
    if os.path.exists(p) and open(p).read().strip(): break
    time.sleep(0.1)
pid, tid = open(os.path.join(w, 'pt.txt')).read().split()
r = subprocess.run(['wine', 'xisamp_x64.exe', pid, tid, '1', '2', '.', '0x0', '10001000:10002000:0x20'], cwd=w, env=env, capture_output=True, text=True, timeout=120)
tgt.wait(timeout=60)
check(r.returncode == 0, 'xisamp ran and stopped at the target\'s exit (exit code %d)' % r.returncode)
csv = os.path.join(w, 'xikit_native_%s.csv' % pid)
head, wins = read.load(csv) if os.path.exists(csv) else ([], [])
check(len(wins) >= 3, '%d windows written (6 s, a window every 2 s, and the last at exit)' % len(wins))
check(any(h == '# rel 0x0; shift 10001000:10002000:0x20' for h in head), 'the header carries rel and the shift table')
rows = [(w_, r_) for w_ in wins for r_ in w_.rows]; total = sum(r_[0] for _, r_ in rows)
check(total > 2000 and sum(w_.missed + w_.xfail for w_ in wins) == 0, '%d samples, none missed, every x86 context read' % total)
spin_lo = sym['spin']; spin_hi = min(v for v in sym.values() if v > spin_lo)
in_spin = [(w_, r_) for w_, r_ in rows if spin_lo <= r_[1] < spin_hi]
n_spin = sum(r_[0] for _, r_ in in_spin)
check(n_spin > total * 0.3, 'samples in spin (the busy half): %d of %d' % (n_spin, total))
good = sum(r_[0] for _, r_ in in_spin if r_[2][:3] == [ra_spin, ra_inner, ra_outer])
check(good == n_spin, 'every spin sample\'s x86 chain begins with the three return addresses the disassembly gives (0x%X 0x%X 0x%X): %d of %d' % (ra_spin, ra_inner, ra_outer, good, n_spin))
napped = sum(r_[0] for w_, r_ in rows if ra_inner in r_[2] and not (spin_lo <= r_[1] < spin_hi) and w_.mod(r_[1]) and w_.mod(r_[1])[2].lower() != 'target.exe')
check(napped > total * 0.2, 'samples waiting in Windows under chain_inner (nap: Sleep): %d' % napped)
labs = [l for w_ in wins for l in w_.labels.values()]
check(any(l[0].lower() == 'kernel32.dll' and l[2] for l in labs), 'an address in kernel32.dll is labelled with its nearest export')
check(any(w_.mod(sym['main']) and w_.mod(sym['main'])[1] == 0x14C for w_ in wins), 'the module table has target.exe as an x86 image')
rp = subprocess.run([sys.executable, os.path.join(H, 'read.py'), csv], capture_output=True, text=True)
check(rp.returncode == 0 and 'x86: the module the x86 code was in' in rp.stdout and 'target.exe' in rp.stdout, 'read.py reads the file')
b = open(os.path.join(D, 'xisamp_arm64.exe'), 'rb').read(); pe = b.find(b'PE\0\0')
check(pe > 0 and b[pe + 4:pe + 6] == b'\x64\xaa', 'xisamp_arm64.exe is an ARM64 image')
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED'); sys.exit(1 if bad else 0)
