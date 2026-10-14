"""test_d3d8on11.py -- d3d8on11 (kit/d3d8on11) offline, under Wine on Mesa's software renderer in a virtual X server:
  1. t8.exe basic through Wine's own d3d8 (the reference: Wine's implementation, itself tested against Windows) and
     through ours: every check must pass on both;
  2. t8.exe defaults through both: every render state and texture stage state of a fresh device, compared; each
     difference is listed (a difference is ours to explain against the DirectX 8 SDK reference, not to copy from Wine);
  3. t8.exe scenes through both: each scene's 64x64 picture compared pixel by pixel. A pixel differs when a channel
     differs by more than TOL (rasterizers may round differently at the last bit). A scene passes when at most
     MAXFRAC of its pixels differ. TOL and MAXFRAC are this test's criteria, chosen here, not facts about either
     device; every scene's numbers are printed. With KEEP=1 each scene is written as a PNG (Wine | ours | difference).
Ours runs in each executor mode (README.md: direct, thread, remote -- the last with dist/d8x.exe, the x86 build of the
executor process; MODES="direct thread remote" by default): 1 and 3 must pass in each, and each mode's pictures must
equal direct mode's exactly (the same commands reach the same executor code; only the transport differs).

    python3 test_d3d8on11.py          (after build.sh; needs wine, xvfb-run)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); D8 = os.path.join(H, '..')
bad = 0
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-Wno-misleading-indentation', '-o', os.path.join(H, 't8.exe'), os.path.join(H, 't8.c'), '-ld3d8', '-static-libgcc'], check=True)
w = tempfile.mkdtemp(prefix='d3d8on11_')
ours = os.path.join(w, 'ours'); ref = os.path.join(w, 'ref'); os.makedirs(ours); os.makedirs(ref)
for d in (ours, ref): shutil.copy(os.path.join(H, 't8.exe'), d)
shutil.copy(os.path.join(D8, 'dist', 'd3d8.dll'), ours); shutil.copy(os.path.join(D8, 'dist', 'd8x.exe'), ours)
MODES = os.environ.get('MODES', 'direct thread remote').split()


def run(where, mode, override, xmode=None):
    env = dict(os.environ, WINEDEBUG='-all')
    if override: env['WINEDLLOVERRIDES'] = 'd3d8=n'
    if xmode: env['D8ON11_MODE'] = xmode
    r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', mode], cwd=where,
                       capture_output=True, text=True, timeout=600, env=env)
    return r.stdout


log = os.path.join(ours, 'd3d8on11.log')
for name, where, ov, xm in [('Wine\'s d3d8', ref, False, None)] + [('d3d8on11, %s mode' % m, ours, True, m) for m in MODES]:
    if xm and os.path.exists(log): os.remove(log)
    out = run(where, 'basic', ov, xm)
    print('== basic, %s' % name); print(out.strip())
    if 'ALL OK' not in out: bad += 1
    if not xm: continue
    t = open(log).read() if os.path.exists(log) else ''
    ok = 'Direct3DCreate8' in t and ('executor: %s' % xm) in t
    print('%s: d3d8on11 was the d3d8.dll loaded, in %s mode (its log says so)' % ('ok' if ok else 'FAIL', xm)); bad += not ok
    print('-- d3d8on11.log --'); print(t.strip())

a = dict(l.rsplit(' ', 1) for l in run(ref, 'defaults', False).splitlines() if l[:3] in ('rs ', 'tss'))
b = dict(l.rsplit(' ', 1) for l in run(ours, 'defaults', True).splitlines() if l[:3] in ('rs ', 'tss'))
diff = sorted(k for k in set(a) | set(b) if a.get(k) != b.get(k))
print('== defaults: %d states read on Wine\'s d3d8, %d on ours, %d differ' % (len(a), len(b), len(diff)))
for k in diff: print('  %-10s Wine %s  ours %s' % (k, a.get(k, '(fails)'), b.get(k, '(fails)')))

TOL, MAXFRAC = 8, 0.01


def scenes(where, ov, xm=None):
    out = {}
    for l in run(where, 'scenes', ov, xm).splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2)
            out[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    return out


sa = scenes(ref, False)
first = None
for m in MODES:
  sb = scenes(ours, True, m)
  if first is None: first = (m, sb)
  else:
    same = sb == first[1]; bad += not same
    print('%s: %s mode drew exactly what %s mode drew (%d scenes)' % ('ok' if same else 'FAIL', m, first[0], len(sb)))
    for n in sorted(set(sb) | set(first[1])):
        if sb.get(n) != first[1].get(n): print('  differs: %s' % n)
    continue
  print('== scenes: Wine\'s d3d8 against ours, %s mode (a pixel differs when a channel differs by more than %d)' % (m, TOL))
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
