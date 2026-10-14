"""test_d3d8on11.py -- d3d8on11 (kit/d3d8on11) offline, under Wine on Mesa's software renderer in a virtual X server:
  1. t8.exe basic through Wine's own d3d8 (the reference: Wine's implementation, itself tested against Windows) and
     through ours: every check must pass on both;
  2. t8.exe defaults through both: every render state and texture stage state of a fresh device, compared; each
     difference is listed (a difference is ours to explain against the DirectX 8 SDK reference, not to copy from Wine);
  3. t8.exe scenes through both: each scene's 64x64 picture compared pixel by pixel. A pixel differs when a channel
     differs by more than TOL (rasterizers may round differently at the last bit). A scene passes when at most
     MAXFRAC of its pixels differ. TOL and MAXFRAC are this test's criteria, chosen here, not facts about either
     device; every scene's numbers are printed. With KEEP=1 each scene is written as a PNG (Wine | ours | difference).

    python3 test_d3d8on11.py          (after build.sh; needs wine, xvfb-run)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); D8 = os.path.join(H, '..')
bad = 0
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-Wno-misleading-indentation', '-o', os.path.join(H, 't8.exe'), os.path.join(H, 't8.c'), '-ld3d8', '-static-libgcc'], check=True)
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

TOL, MAXFRAC = 8, 0.01


def scenes(where, ov):
    out = {}
    for l in run(where, 'scenes', ov).splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2)
            out[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    return out


sa, sb = scenes(ref, False), scenes(ours, True)
print('== scenes: Wine\'s d3d8 against ours (a pixel differs when a channel differs by more than %d)' % TOL)
for n in sa:
    a, b = sa[n], sb.get(n)
    if not a or not b or len(a) != len(b):
        print('FAIL %-32s %s' % (n, 'no picture from ' + ('Wine' if not a else 'ours'))); bad += 1; continue
    dif = [max(abs(((x >> s_) & 255) - ((y >> s_) & 255)) for s_ in (0, 8, 16)) for x, y in zip(a, b)]
    nd = sum(1 for v in dif if v > TOL); frac = nd / len(dif)
    ok = frac <= MAXFRAC; bad += not ok
    print('%-4s %-32s %4d of %d pixels differ, worst channel difference %d' % ('ok' if ok else 'FAIL', n, nd, len(dif), max(dif)))
    if os.environ.get('KEEP'):
        import zlib, struct
        W = 64 * 3 + 8
        rows = []
        for y in range(64):
            r = bytearray()
            for img in (a, b, [0xFF0000 if v > TOL else (v * 8 << 8) for v in dif]):
                for x in range(64): v = img[y * 64 + x]; r += bytes(((v >> 16) & 255, (v >> 8) & 255, v & 255))
                r += b'\x40\x40\x40' * 4
            rows.append(b'\0' + bytes(r[:W * 3]))
        def chunk(t, d): return struct.pack('>I', len(d)) + t + d + struct.pack('>I', zlib.crc32(t + d))
        png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', W, 64, 8, 2, 0, 0, 0)) + chunk(b'IDAT', zlib.compress(b''.join(rows))) + chunk(b'IEND', b'')
        open(os.path.join(w, 'scene_%s.png' % n), 'wb').write(png)
for n in sb:
    if n not in sa: print('FAIL %-32s only ours drew it' % n); bad += 1
if os.path.exists(log):
    t = open(log).read()
    for l in t.splitlines():
        if 'not implemented' in l or 'failed' in l or 'skipped' in l: print('  log:', l)
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
