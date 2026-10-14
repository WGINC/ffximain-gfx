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


def run(where, mode, override, xmode=None, extra=None):
    env = dict(os.environ, WINEDEBUG='-all', **(extra or {}))
    if override: env['WINEDLLOVERRIDES'] = 'd3d8=n'
    if xmode: env['D8ON11_MODE'] = xmode
    r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe'] + mode.split(), cwd=where,
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


def scenes(where, ov, xm=None, extra=None):
    out = {}
    for l in run(where, 'scenes', ov, xm, extra).splitlines():
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
# the ring wrapping: the scenes through a 64 KB ring (D8ON11_RING_KB) must draw what direct draws. They send about 165 KB,
# so the ring wraps twice (two X_PAD records, counted with a -DD8_TRACE build on 14 Oct). The byte counters start at
# 0xFFFF0100 (D8ON11_RING_START), so they also pass 2^32 on the way, as they do every few minutes in the game
for m in [m for m in MODES if m != 'direct']:
    sb = scenes(ours, True, m, {'D8ON11_RING_KB': '64', 'D8ON11_RING_START': 'FFFF0100'})
    t = open(log).read() if os.path.exists(log) else ''
    same = first is not None and sb == first[1] and 'ring: 64 KB' in t and 'counters start at 0xFFFF0100' in t; bad += not same
    print('%s: %s mode through a 64 KB ring, its counters passing 2^32, drew exactly what %s mode drew (%d scenes)' % ('ok' if same else 'FAIL', m, first[0] if first else '?', len(sb)))
# randomized cases (random_cases.py: each state drawn from the values the game sets, data/d3d8_scope.json), through
# Wine's d3d8 and ours (direct mode; the other modes draw the same, above). Same criteria as the scenes. A difference
# fails unless the case is in one of two classes where the reference behaviour is not established and the game was
# not seen in them (its traces, 14 Oct); those are listed with their class, not hidden:
#   A  pre-transformed vertices with a stage generating coordinates (Wine generates from the screen position; ours,
#      by the DirectX 8 era text that such vertices are not modified, reads the set as given: d8_exec.cpp)
#   C  a stage reading a texture through a coordinate set the vertex format does not have
#   L  lighting with normals longer than 1 and NORMALIZENORMALS off: Wine clamps each light's N.L at 1, the reference's
#      equation does not ("Diffuse Lighting", Direct3D 9; d8_exec.cpp). Counts as L only if the same case with
#      NORMALIZENORMALS on then draws the same on both
#   S  a cube texture filtered linearly: Wine 9.0's d3d8 filters each face apart (WINED3D_LEGACY_CUBEMAP_FILTERING,
#      dlls/d3d8/d3d8_private.h), D3D11 always across faces. Checked, not assumed: such a case counts as S only if the
#      same case with that stage's filters set to point then draws the same on both (a one-colour cube and point
#      filtering never differ; with a colour per face the differences lie on the face boundaries: 14 Oct)
RSEED, RCOUNT = int(os.environ.get('RANDOM_SEED', '1')), int(os.environ.get('RANDOM_CASES', '200'))
cases_txt = subprocess.run([sys.executable, os.path.join(H, 'random_cases.py'), str(RSEED), str(RCOUNT)], capture_output=True, text=True, check=True).stdout
for d in (ref, ours): open(os.path.join(d, 'cases.txt'), 'w').write(cases_txt)
def rscenes(where, ov, xm=None):
    out = {}
    for l in run(where, 'random cases.txt', ov, xm).splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2)
            out[n] = None if hx.strip() == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx.strip()), 6)]
    return out
def rclass(lines, fix=None):
    fvf, tss, tex, cube = int(lines[0].split()[2], 16), {}, set(), set()
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
        if p[0] == 'tex' and p[2] != '0': tex.add(int(p[1]))
        if p[0] == 'tex': (cube.add if p[2] in ('4', '5', '6') else cube.discard)(int(p[1]))
    rhw, nsets, cls = (fvf & 0xE) == 4, (fvf >> 8) & 15, set()
    for st in range(4):
        if tss.get((st, 1), 4 if st == 0 else 1) == 1: break               # D3DTOP_DISABLE ends the stages
        if st not in tex: continue
        tci = tss.get((st, 11), st)
        # the change that takes the class away must not make another undefined case: a cube stage reading a 2D set
        # (what a missing third component reads as is not in the reference) gets a 2D texture
        two_d = lambda: ['tex %d 1' % st] if st in cube else []
        if rhw and tci >> 16:
            cls.add('A')
            if fix is not None: fix.extend(['tss %d 11 %X' % (st, tci & 7)] + two_d())
        if (rhw or not tci >> 16) and (tci & 7) >= nsets:
            cls.add('C')
            if fix is not None: fix.extend(['tss %d 11 0' % st] + two_d() if nsets else ['tex %d 0' % st])
    return cls
def lit_unnormalized(lines):
    rs = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'rs': rs[int(p[1])] = int(p[2], 16)
    return (int(lines[0].split()[2], 16) & 0x10) and rs.get(137, 1) and not rs.get(143, 0)
def cube_linear_stages(lines):
    tss, cube = {}, set()
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
        if p[0] == 'tex' and p[2] in ('4', '5', '6'): cube.add(int(p[1]))
    return [st for st in cube if tss.get((st, 16), 1) >= 2 or tss.get((st, 17), 1) >= 2]
blocks = {}
for blk in cases_txt.split('case ')[1:]:
    ls = ('case ' + blk).strip().splitlines(); blocks[ls[0].split()[1]] = ls
ra, rb = rscenes(ref, False), rscenes(ours, True, 'direct')
fails, known, cands = [], {}, []
for n in blocks:
    a, b = ra.get(n), rb.get(n)
    if not a or not b or len(a) != len(b): fails.append('%s: no picture from %s' % (n, 'Wine' if not a else 'ours')); continue
    dif = [max(abs(((x >> s_) & 255) - ((y >> s_) & 255)) for s_ in (0, 8, 16)) for x, y in zip(a, b)]
    nd = sum(1 for v in dif if v > TOL)
    if nd / len(dif) <= MAXFRAC: continue
    if rclass(blocks[n]) or cube_linear_stages(blocks[n]) or lit_unnormalized(blocks[n]): cands.append((n, nd, max(dif)))
    else: fails.append('%s: %d of %d pixels differ, worst channel difference %d' % (n, nd, len(dif), max(dif)))
# the candidates again, changed in every way their classes name (A: the stage stops generating; C: the stage reads
# set 0, or no texture if there is no set; S: the cube stages point-filtered; L: NORMALIZENORMALS on): they must then draw the same, or the difference is not explained by those classes
if cands:
    def change(ls):
        fx = []; rclass(ls, fx)
        return fx + ['tss %d 16 1\ntss %d 17 1' % (k, k) for k in cube_linear_stages(ls)] + (['rs 143 1'] if lit_unnormalized(ls) else [])
    txt = ''.join('\n'.join([blocks[n][0]] + blocks[n][1:-1] + change(blocks[n]) + ['draw']) + '\n' for n, _, _ in cands)
    for d in (ref, ours): open(os.path.join(d, 'cases.txt'), 'w').write(txt)
    pa, pb = rscenes(ref, False), rscenes(ours, True, 'direct')
    for n, nd0, w0 in cands:
        a, b = pa.get(n), pb.get(n)
        dif = [max(abs(((x >> s_) & 255) - ((y >> s_) & 255)) for s_ in (0, 8, 16)) for x, y in zip(a or [], b or [])]
        nd = sum(1 for v in dif if v > TOL)
        cls = ''.join(sorted(rclass(blocks[n]))) + ('S' if cube_linear_stages(blocks[n]) else '') + ('L' if lit_unnormalized(blocks[n]) else '')
        if a and b and nd / len(dif) <= MAXFRAC: known.setdefault(cls, []).append('%s (%d px, worst %d; changed: %d px)' % (n, nd0, w0, nd))
        else: fails.append('%s: differs, and still with the changes of class %s (%d px)' % (n, cls, nd))
print('%s: %d random cases (seed %d): %d differ outside the known classes' % ('ok' if not fails else 'FAIL', len(blocks), RSEED, len(fails)))
for f in fails: print('   ', f)
print('    classes: A pre-transformed vertices generating coordinates, C a coordinate set the format lacks (reference not\n'
      '    established for either, not seen in the game); S cube seams (Wine filters faces apart, D3D11 across them);\n'
      '    L N.L above 1 with long normals (Wine clamps it, the reference equation does not). Each case below drew the same\n'
      '    on both once changed as its classes name ("changed: N px")')
for c, l in sorted(known.items()): print('    class %s: %s' % (c, ', '.join(l)))
bad += len(fails) > 0
# 600 frames of Clear, Present and (from frame 30) a read-back, in each mode (t8 frames; a 64 KB ring): all 600 must run and the
# stats lines must be written (d8_link.cpp, every 600 Presents). Then in remote mode t8 orphan ends the executor
# process at frame 30: the device must end the game with a log line, not hang.
for m, run_ in [(m, 'frames') for m in MODES] + ([('remote', 'orphan')] if 'remote' in MODES else []):
    for f in os.listdir(ours):
        if f.endswith('.log'): os.remove(os.path.join(ours, f))
    env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE=m, D8ON11_RING_KB='64')
    try:
        r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', run_], cwd=ours,
                           capture_output=True, text=True, timeout=300, env=env)
        code, out = r.returncode, r.stdout
    except subprocess.TimeoutExpired: code, out = None, '(timed out after 300 s)'
    t = ''.join(open(os.path.join(ours, f)).read() for f in sorted(os.listdir(ours)) if f.endswith('.log'))
    if run_ == 'orphan':
        ok = code == 3 and 'ended 1 executor' in out and 'the executor process ended' in t
        what = 'with its executor process ended at frame 30, the game ends (exit code %s), not hangs' % code
    else:
        need = ['executor stats, 600 frames'] + (['ring stats, 600 frames'] if m != 'direct' else [])
        ok = code == 0 and 'still running after 600 frames' in out and all(n in t for n in need)
        what = '600 frames run (exit code %s) and the stats lines are written' % code
    print('%s: %s mode: %s' % ('ok' if ok else 'FAIL', m, what)); bad += not ok
    for l in t.splitlines():
        if 'stats,' in l or 'executor process ended' in l: print('   ', l)
# a second thread making objects while this one does too (t8 threads), and 5,000 rounds of objects made and released
# (t8 churn), in each mode: every check passes, the executor's log has no stream error (an object created twice or
# destroyed unknown: what ids shared between threads would cause; a counter that is not atomic makes them, checked on
# 14 Oct), and once the device is released the executor holds nothing but the back buffer
for m in MODES:
    for run_ in ('threads', 'churn'):
        for f in os.listdir(ours):
            if f.endswith('.log'): os.remove(os.path.join(ours, f))
        env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE=m)
        try:
            r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', run_], cwd=ours,
                               capture_output=True, text=True, timeout=900, env=env)
            code, out = r.returncode, r.stdout
        except subprocess.TimeoutExpired: code, out = None, '(timed out)'
        t = ''.join(open(os.path.join(ours, f)).read() for f in sorted(os.listdir(ours)) if f.endswith('.log'))
        ok = code == 0 and 'ALL OK' in out and 'stream error' not in t and 'executor shutdown: 0 objects still held' in t
        print('%s: %s mode: t8 %s (exit %s; %d stream errors; %s)' % ('ok' if ok else 'FAIL', m, run_, code, t.count('stream error'),
              ([l for l in t.splitlines() if 'objects still held' in l] or ['no shutdown line'])[0])); bad += not ok
        for l in out.splitlines():
            if l.startswith('FAIL'): print('   ', l)
# the executor process starts but cannot make the device (D8ON11_TEST_EXEC_FAIL_INIT): the device falls back to the
# thread mode and works
if 'remote' in MODES:
    for f in os.listdir(ours):
        if f.endswith('.log'): os.remove(os.path.join(ours, f))
    env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE='remote', D8ON11_TEST_EXEC_FAIL_INIT='1')
    out = run(ours, 'basic', True, 'remote', {'D8ON11_TEST_EXEC_FAIL_INIT': '1'})
    t = open(log).read() if os.path.exists(log) else ''
    ok = 'ALL OK' in out and 'could not start the device: thread instead' in t and 'executor: thread' in t
    print('%s: remote mode whose executor cannot make the device falls back to thread mode, and basic passes' % ('ok' if ok else 'FAIL')); bad += not ok
if os.path.exists(log):
    t = open(log).read()
    for l in t.splitlines():
        if 'not implemented' in l or 'failed' in l or 'skipped' in l: print('  log:', l)
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
