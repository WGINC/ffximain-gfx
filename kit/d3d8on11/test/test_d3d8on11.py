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
Where Wine is known to differ from Microsoft's own runtime (WINE_DIFFERS), a scene's difference from Wine is listed,
not failed, and that scene is held to the next stage instead:
  4. with D3D8REF_DIR (the DirectX 8.1 SDK's retail d3d8.dll, d3dref8.dll and d3d8thk.dll) and WDK_DDI (the WDK 7.1
     DDI headers) set: Microsoft's runtime with its reference device (msref/build.sh: our stand-in for the kernel
     thunks), t8 scenes, defaults and sbtypes (what each state block type captures), against ours (direct mode). A pixel passes when within REF_TOL of the reference's
     pixel or, both ways, of one of its 3x3 neighbours: a pixel centre exactly on (or within about 0.01 px of) an
     edge, or a texture coordinate on a texel boundary, is a tie each rasterizer's rounding decides (every such
     pixel of 14 Oct was computed to be one: docs/HISTORY.md). REF_SKIP lists the scenes not compared, and why.

The executor's filter (exec_filter=1) is run in each mode too, and must draw exactly what direct mode draws without it.

    python3 test_d3d8on11.py          (after build.sh; needs wine, xvfb-run)"""
import os, re, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); D8 = os.path.join(H, '..')
bad = 0
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-Wno-misleading-indentation', '-o', os.path.join(H, 't8.exe'), os.path.join(H, 't8.c'), '-ld3d8', '-lversion', '-static-libgcc'], check=True)
w = tempfile.mkdtemp(prefix='d3d8on11_')
ours = os.path.join(w, 'ours'); ref = os.path.join(w, 'ref'); os.makedirs(ours); os.makedirs(ref)
for d in (ours, ref): shutil.copy(os.path.join(H, 't8.exe'), d)
shutil.copy(os.path.join(D8, 'dist', 'd3d8.dll'), ours); shutil.copy(os.path.join(D8, 'dist', 'd8x.exe'), ours)
MODES = os.environ.get('MODES', 'direct thread remote').split()
# the scenes are drawn without the executor filter (its default is 1, d8_device.cpp) unless a check asks for it
os.environ['D8ON11_EXEC_FILTER'] = '0'
# ... and without the executor's batches (exec_batch, default 1), unless a check asks for them
os.environ['D8ON11_EXEC_BATCH'] = '0'


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
    # the presentation parameters t8 passes (windowed, DISCARD, interval DEFAULT, BackBufferCount 0) are logged at
    # CreateDevice and Reset with the SyncInterval they map to (0: windowed, d8_device.cpp Dev::present_interval); no
    # reply was cut (basic reads back an 18 MB render target, larger than the reply area)
    ok = all('%s: the game asked SwapEffect 1 (DISCARD), FullScreen_PresentationInterval 0x0, BackBufferCount 0, Flags 0x0, '
             'windowed; Present\'s SyncInterval 0 (windowed)' % at in t for at in ('CreateDevice', 'Reset')) and 'larger than the reply area' not in t
    print('%s: %s mode: the game\'s presentation parameters and the SyncInterval are logged at CreateDevice and Reset; no reply cut' % ('ok' if ok else 'FAIL', xm)); bad += not ok
    print('-- d3d8on11.log --'); print(t.strip())

# present_interval=1 in d3d8on11.ini: Present's SyncInterval is 1 whatever the game asked (the log says so; whether the
# executor's Present then waits for a vertical blank is not observable under Xvfb). The option is read on the D3D8
# side and sent in X_INIT, so one mode is run: remote, where it crosses to another process
ini = os.path.join(ours, 'd3d8on11.ini')
for m in (['remote'] if 'remote' in MODES else MODES[:1]):
    if os.path.exists(log): os.remove(log)
    open(ini, 'w').write('[d3d8on11]\npresent_interval=1\n')
    out = run(ours, 'basic', True, m)
    os.remove(ini)
    t = open(log).read() if os.path.exists(log) else ''
    ok = 'ALL OK' in out and all("%s: the game asked SwapEffect 1 (DISCARD), FullScreen_PresentationInterval 0x0, BackBufferCount 0, Flags 0x0, "
                                 "windowed; Present's SyncInterval 1 (present_interval in d3d8on11.ini)" % at in t for at in ('CreateDevice', 'Reset'))
    print('%s: %s mode with present_interval=1: basic passes and the log gives SyncInterval 1 from the ini' % ('ok' if ok else 'FAIL', m)); bad += not ok

# the flip-model swap chain and the GPU's time (6 Nov): t8 frames 3000 (3000 frames, frame-late locks of a copied
# render target) in thread mode (1) with swap_flip=1 and gpu_time=1 from the start: every frame-late lock must still read
# the right colour and the GPU lines must be written; (2) exec_ab_lever=flip (7 Nov: the switch while running froze the
# owner's game): refused, the A/B off, the swap chain never made again
for label, text in (('swap_flip=1 gpu_time=1', '[d3d8on11]\nswap_flip=1\ngpu_time=1\n'),
                    ('frame_latency=1 gpu_time=1', '[d3d8on11]\nframe_latency=1\ngpu_time=1\n'),
                    ('exec_ab_lever=flip', '[d3d8on11]\ngpu_time=1\nexec_ab=1\nexec_ab_lever=flip\nexec_ab_after=0\n'
                                           'exec_ab_phase=1\nexec_ab_rounds=3\nexec_ab_settle=1\n')):
    if os.path.exists(log): os.remove(log)
    open(ini, 'w').write(text)
    out = run(ours, 'frames 3000', True, 'thread', {'D8ON11_RING_KB': '64'})
    os.remove(ini)
    t = open(log).read() if os.path.exists(log) else ''
    ok = 'still running after 3000 frames' in out and 'ok: a frame-late lock' in out and 'FAIL' not in out and 'gpu_time, 600 frames' in t \
         and 'the blt model instead' not in t and 'swap chain now' not in t
    if 'lever' in label: ok = ok and 'exec_ab_lever=flip: refused' in t and 'executor ab: phase' not in t
    elif 'latency' in label: ok = ok and 'frame_latency=1: SetMaximumFrameLatency 0x00000000' in t
    else: ok = ok and 'swap_flip=1: the flip-model swap chain' in t
    print('%s: thread mode, %s: 3000 frames run, frame-late locks right, the GPU lines written%s' % (
        'ok' if ok else 'FAIL', label, '; the lever refused, the swap chain not switched' if 'lever' in label else ''))
    bad += not ok
    for l in t.splitlines():
        if 'gpu_time' in l or 'swap chain now' in l or 'swap_flip' in l or 'executor ab' in l or 'blt model instead' in l or 'lever' in l or 'frame_latency' in l: print('   ', l)

a = dict(l.rsplit(' ', 1) for l in run(ref, 'defaults', False).splitlines() if l[:3] in ('rs ', 'tss'))
b = dict(l.rsplit(' ', 1) for l in run(ours, 'defaults', True).splitlines() if l[:3] in ('rs ', 'tss'))
bdef = b
diff = sorted(k for k in set(a) | set(b) if a.get(k) != b.get(k))
print('== defaults: %d states read on Wine\'s d3d8, %d on ours, %d differ' % (len(a), len(b), len(diff)))
for k in diff: print('  %-10s Wine %s  ours %s' % (k, a.get(k, '(fails)'), b.get(k, '(fails)')))

TOL, MAXFRAC = 8, 0.01
# scenes where Wine's d3d8 is known to draw otherwise than Microsoft's runtime with its reference device (stage 4)
WINE_DIFFERS = {'fog_vertex_linear': 'vertex fog: Microsoft\'s runtime makes the factor per vertex, clamps it and '
                'interpolates it ("Vertex Fog", DirectX 8.1); Wine interpolates the distance',
                'fog_table_rhw': 'table fog on pre-transformed vertices: the depth is 1/RHW ("Pixel Fog", DirectX 8.1: '
                '"to reproduce true w"), as Microsoft\'s runtime does; Wine 9.0 reads the vertex z (glsl_shader.c, '
                'transformed: ffp_varying_fogcoord = ec_pos.z)'}


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
      ok = frac <= MAXFRAC
      if not ok and n in WINE_DIFFERS:
          print('note %-32s %4d of %d pixels differ from Wine, as known: %s' % (n, nd, len(dif), WINE_DIFFERS[n])); continue
      bad += not ok
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
# the executor's filter (exec_filter, D8ON11_EXEC_FILTER: binds that repeat what is set are skipped) must draw exactly
# what it draws without the filter, in each mode
for m in MODES:
    if not first: break
    sf = scenes(ours, True, m, {'D8ON11_EXEC_FILTER': '1'})
    same = sf == first[1]; bad += not same
    print('%s: %s mode with the executor filter drew exactly what %s mode drew without it (%d scenes)' % ('ok' if same else 'FAIL', m, first[0], len(sf)))
    for n in sorted(set(sf) | set(first[1])):
        if sf.get(n) != first[1].get(n): print('  differs: %s' % n)
# the executor's batches (exec_batch, D8ON11_EXEC_BATCH: records held and run together, constants and ...UP data
# written with one Map of each arena) must draw exactly what it draws without them, in each mode, with the filter off
# and on
for m in MODES:
    if not first: break
    for fl in ('0', '1'):
        sf = scenes(ours, True, m, {'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': fl})
        same = sf == first[1]; bad += not same
        print('%s: %s mode with batches (filter %s) drew exactly what %s mode drew without them (%d scenes)' % ('ok' if same else 'FAIL', m, fl, first[0], len(sf)))
        for n in sorted(set(sf) | set(first[1])):
            if sf.get(n) != first[1].get(n): print('  differs: %s' % n)
# 4. Microsoft's DirectX 8.1 runtime with its reference device (D3DDEVTYPE_REF), against ours in direct mode
REF_TOL = 3
REF_SKIP = {'zbias': 'the reference device does not report D3DPRASTERCAPS_ZBIAS (RasterCaps 0x00772199) and ignores it',
            'decl_short2': 'SHORT2 texture coordinates are not fixed-function input (Microsoft\'s debug runtime: "Invalid '
                           'data type set for texture register")',
            'decl_short4': 'as decl_short2'}
# scenes whose state the reference leaves undefined, where the reference device and ours differ: listed with what each
# does, not passed or failed. Which one the owner's setup does is measured by test/owner_run (run_t8.bat)
REF_UNDEFINED = {n: 'an alpha op D3DTOP_DISABLE on a stage whose colour op is on "causes undefined behavior" (D3DTEXTUREOP); '
                    'the reference device takes alpha 1, ours (and Wine) the previous stage\'s alpha; the game does it '
                    '(draws from 0x1000CBF3)' for n in ('alphaop_disabled_stage1', 'alphaop_disabled_stage0')}
# pixels no neighbour matches, each a tie computed from the scene's geometry (14 Oct, docs/HISTORY.md): which texel a
# point sample takes where the coordinate is within a hundredth of a texel of a boundary, or the cube face where two
# components of the lookup vector are within 0.2% of each other, is the rasterizer's rounding
REF_TIES = {('texgen_reflection_cube', 44, 39): 'lookup (-1.593, 1.596, -1.005): |x| and |y| within 0.2% (face edge)',
            ('texgen_reflection_cube', 55, 40): 'face -Z, u = 4.0024 texels (the checker changes at 4)',
            ('texgen_reflection_cube', 54, 42): 'face -Z, v = 2.0052 texels (the checker changes at 2)',
            ('three_stream_declaration', 19, 50): 'v = 25.9940 texels, 0.006 from the row boundary at 26'}
MSD, DDI = os.environ.get('D3D8REF_DIR'), os.environ.get('WDK_DDI')
msrun = None
def ref_unmatched(a, b):
    """pixels of b with no match in a by the criterion above (within REF_TOL, or both ways of a 3x3 neighbour)"""
    near = lambda x, y: all(abs(((x >> s_) & 255) - ((y >> s_) & 255)) <= REF_TOL for s_ in (0, 8, 16))
    bad_ = 0
    for i in range(len(a)):
        if near(a[i], b[i]): continue
        x, y = i % 64, i // 64
        nbr = [(x + dx) + 64 * (y + dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1) if 0 <= x + dx < 64 and 0 <= y + dy < 64]
        if not (any(near(a[i], b[j]) for j in nbr) and any(near(b[i], a[j]) for j in nbr)): bad_ += 1
    return bad_
if not (MSD and DDI):
    print('== Microsoft\'s reference device: not run (set D3D8REF_DIR and WDK_DDI); not checked: %s' % ', '.join(sorted(WINE_DIFFERS)))
elif first:
    msr = os.path.join(w, 'msref')
    subprocess.run(['bash', os.path.join(H, 'msref', 'build.sh'), MSD, DDI, msr], check=True, stdout=subprocess.DEVNULL)
    shutil.copy(os.path.join(H, 't8.exe'), msr)
    mx = {'T8_REF': '1', 'WINEDLLOVERRIDES': 'd3d8=n;d3d8thk=n'}
    def msrun(mode):
        env = dict(os.environ, WINEDEBUG='-all', **mx)
        return subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe'] + mode.split(), cwd=msr,
                              capture_output=True, text=True, timeout=900, env=env).stdout
    sm = {}
    for l in msrun('scenes').splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2)
            sm[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    def near(x, y): return all(abs(((x >> s_) & 255) - ((y >> s_) & 255)) <= REF_TOL for s_ in (0, 8, 16))
    print('== scenes: Microsoft\'s reference device against ours, direct mode (a pixel passes within %d of the reference '
          'pixel, or both ways of one of its 3x3 neighbours)' % REF_TOL)
    ours_d = first[1]
    for n in ours_d:
        if n in REF_SKIP: print('skip %-32s %s' % (n, REF_SKIP[n])); continue
        if n in REF_UNDEFINED: print('undefined %-27s %s' % (n, REF_UNDEFINED[n])); continue
        a, b = sm.get(n), ours_d[n]
        if not a or not b or len(a) != len(b): print('FAIL %-32s no picture from %s' % (n, 'the reference' if not a else 'ours')); bad += 1; continue
        nb_bad = exact = 0; ties = []
        for i in range(len(a)):
            if near(a[i], b[i]): exact += 1; continue
            x, y = i % 64, i // 64
            nbr = [(x + dx) + 64 * (y + dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1) if 0 <= x + dx < 64 and 0 <= y + dy < 64]
            if not (any(near(a[i], b[j]) for j in nbr) and any(near(b[i], a[j]) for j in nbr)):
                if (n, x, y) in REF_TIES: ties.append('(%d,%d) %s' % (x, y, REF_TIES[(n, x, y)])); continue
                nb_bad += 1; print('     (%d,%d) reference %06X, ours %06X' % (x, y, a[i], b[i]))
        ok = nb_bad == 0; bad += not ok
        print('%-4s %-32s %4d pixels beyond %d, %d of them with no match in their neighbourhood%s' % ('ok' if ok else 'FAIL', n, len(a) - exact, REF_TOL, nb_bad,
              ''.join('\n     tie %s' % t for t in ties)))
    for n in sm:
        if n not in ours_d: print('FAIL %-32s only the reference drew it' % n); bad += 1
    # defaults: the states Microsoft's runtime answers (it fails the retired ones) up to 173, the last d3d8types.h
    # defines, but D3DRS_SOFTWAREVERTEXPROCESSING (153), which follows the device's creation flags (the reference
    # device is made with software vertex processing), and those it answers 0xBAADCAFE, its fill for a state it never
    # set (all numbers d3d8types.h does not define, and D3DRS_DEBUGMONITORTOKEN, 165, which ours answers so too)
    md = dict(l.rsplit(' ', 1) for l in msrun('defaults').splitlines() if l[:3] in ('rs ', 'tss'))
    keys = [k for k in md if not (k.startswith('rs ') and (int(k.split()[1]) > 173 or int(k.split()[1]) == 153))]
    cafe = [k for k in keys if md[k] == 'BAADCAFE']; keys = [k for k in keys if k not in cafe]
    dd = [k for k in keys if md[k] != bdef.get(k)]
    print('%s: defaults: %d states compared with Microsoft\'s runtime (%d it answers 0xBAADCAFE left out), %d differ' % (
          'ok' if not dd else 'FAIL', len(keys), len(cafe), len(dd))); bad += len(dd) > 0
    for k in dd: print('  %-10s Microsoft %s  ours %s' % (k, md[k], bdef.get(k, '(fails)')))
    # state blocks: what each D3DSTATEBLOCKTYPE captures (t8 sbtypes), on every row Microsoft's runtime answers. Rows ours
    # cannot measure ("unset": its pixel shaders and shader constants are not implemented) are listed, not failed
    def sbrows(out): return {' '.join(l.split()[1:-1]): l.split()[-1] for l in out.splitlines() if l.startswith('sb ')}
    ms_out = msrun('sbtypes'); ms_sb, ours_sb = sbrows(ms_out), sbrows(run(ours, 'sbtypes', True, 'direct'))
    # complete: t8 ends each type with its own count of rows ("sbtotal TYPE captured N not N unmeasured N")
    tot = sum(int(p_[3]) + int(p_[5]) + int(p_[7]) for p_ in (l.split() for l in ms_out.splitlines() if l.startswith('sbtotal ')))
    complete = ms_out.count('sbtotal ') == 3 and tot == len(ms_sb)
    gap = sorted(k for k in ms_sb if ours_sb.get(k) in (None, 'unset'))
    sbd = sorted(k for k in ms_sb if k not in gap and ms_sb[k] != ours_sb[k])
    print('%s: state blocks: %d rows Microsoft\'s runtime answers, %d compared, %d differ; not measured on ours: %s' % (
          'ok' if not sbd and complete else 'FAIL', len(ms_sb), len(ms_sb) - len(gap), len(sbd), ', '.join(gap) or '-'))
    bad += bool(sbd) or not complete
    if not complete: print('  the runtime\'s output is incomplete: %d rows, its totals say %d' % (len(ms_sb), tot))
    for k in sbd: print('  %-24s Microsoft %s  ours %s' % (k, ms_sb[k], ours_sb[k]))
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
# fails unless the case is in one of the classes below, where the reference behaviour is not established and the game was
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
#   F  linear fog with FOGSTART >= FOGEND, which the DirectX 8.1 text leaves undefined (random_cases.py fog_reversed;
#      Microsoft's reference device and Wine differ there too). Counts as F only if the same case with FOGEND moved
#      above FOGSTART then draws the same on both
#   W  table fog on pre-transformed vertices: Wine 9.0 fogs by the vertex z, Microsoft's runtime and ours by 1/RHW
#      (scene fog_table_rhw, WINE_DIFFERS). Counts as W only if the same case with fog off then draws the same on both
#   D  a stage with its colour op on and its alpha op DISABLE, undefined by the DirectX 8.1 text (random_cases.py
#      alpha_disabled). Counts as D only if the same case with that alpha op SELECTARG1 of CURRENT then draws the same
#   N  a stage reading a texture it was not given (random_cases.py rclass; ours draws it by the DirectX 8.1 "Texture
#      Blending" rule, Wine otherwise). Counts as N only if the same case with that stage off then draws the same
#   T  the alpha test, compared by Wine on the float alpha and by ours on floor(alpha x 255) as Microsoft's reference
#      device does (random_cases.py alpha_test_wine). Counts as T only if the same case with the alpha test off then
#      draws the same
RSEED, RCOUNT = int(os.environ.get('RANDOM_SEED', '1')), int(os.environ.get('RANDOM_CASES', '200'))
sys.path.insert(0, H)
from random_cases import classes, changes   # the classes below
cases_txt = subprocess.run([sys.executable, os.path.join(H, 'random_cases.py'), str(RSEED), str(RCOUNT)], capture_output=True, text=True, check=True).stdout
for d in (ref, ours): open(os.path.join(d, 'cases.txt'), 'w').write(cases_txt)
def rscenes(where, ov, xm=None):
    out = {}
    for l in run(where, 'random cases.txt', ov, xm).splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2)
            out[n] = None if hx.strip() == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx.strip()), 6)]
    return out
blocks = {}
for blk in cases_txt.split('case ')[1:]:
    ls = ('case ' + blk).strip().splitlines(); blocks[ls[0].split()[1]] = ls
ra, rb = rscenes(ref, False), rscenes(ours, True, 'direct')
fails, known, cands, variant = [], {}, [], {}
for n in blocks:
    a, b = ra.get(n), rb.get(n)
    if not a or not b or len(a) != len(b): fails.append('%s: no picture from %s' % (n, 'Wine' if not a else 'ours')); continue
    dif = [max(abs(((x >> s_) & 255) - ((y >> s_) & 255)) for s_ in (0, 8, 16)) for x, y in zip(a, b)]
    nd = sum(1 for v in dif if v > TOL)
    if nd / len(dif) <= MAXFRAC: continue
    if classes(blocks[n]): cands.append((n, nd, max(dif)))
    else: fails.append('%s: %d of %d pixels differ, worst channel difference %d' % (n, nd, len(dif), max(dif)))
# the candidates again, changed in every way their classes name (A: the stage stops generating; C: the stage reads
# set 0, or no texture if there is no set; S: the cube stages point-filtered; L: NORMALIZENORMALS on; F: FOGEND above FOGSTART; W: fog off;
# D: the alpha op SELECTARG1 of CURRENT; N: the stage off; T: the alpha test off): they must then draw the same, or the difference is not
# explained by those classes
if cands:
    chg = {n: '\n'.join([blocks[n][0]] + blocks[n][1:-1] + changes(blocks[n]) + ['draw']) + '\n' for n, _, _ in cands}
    txt = ''.join(chg[n] for n, _, _ in cands)
    for d in (ref, ours): open(os.path.join(d, 'cases.txt'), 'w').write(txt)
    pa, pb = rscenes(ref, False), rscenes(ours, True, 'direct')
    for n, nd0, w0 in cands:
        a, b = pa.get(n), pb.get(n)
        dif = [max(abs(((x >> s_) & 255) - ((y >> s_) & 255)) for s_ in (0, 8, 16)) for x, y in zip(a or [], b or [])]
        nd = sum(1 for v in dif if v > TOL)
        cls = classes(blocks[n])
        if a and b and nd / len(dif) <= MAXFRAC: known.setdefault(cls, []).append('%s (%d px, worst %d; changed: %d px)' % (n, nd0, w0, nd))
        else: fails.append('%s: differs, and still with the changes of class %s (%d px)' % (n, cls, nd)); variant[n] = (chg[n], b)
# a case that still fails is drawn by Microsoft's runtime and reference device too (when there), as it failed (with its
# class's changes if it failed with them): where that matches ours by the reference criterion (above), the difference
# is Wine's alone and is listed so, not failed. Without the reference such a case fails: Wine is then the only one
alone = []
if msrun and fails:
    fn = [f.split(':')[0] for f in fails]
    open(os.path.join(msr, 'cases.txt'), 'w').write(''.join(variant[n][0] if n in variant else '\n'.join(blocks[n]) + '\n' for n in fn if n in blocks))
    pm = {}
    for l in msrun('random cases.txt').splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2); hx = hx.strip()
            pm[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    for f in list(fails):
        n = f.split(':')[0]; a, b = pm.get(n), variant[n][1] if n in variant else rb.get(n)
        if a and b and len(a) == len(b) and ref_unmatched(a, b) == 0: fails.remove(f); alone.append(n)
print('%s: %d random cases (seed %d): %d differ outside the known classes' % ('ok' if not fails else 'FAIL', len(blocks), RSEED, len(fails)))
if alone: print('    Wine alone differs (Microsoft\'s reference device draws them as ours does): %s' % ', '.join(alone))
for f in fails: print('   ', f)
print('    classes: A pre-transformed vertices generating coordinates, C a coordinate set the format lacks (reference not\n'
      '    established for either, not seen in the game); S cube seams (Wine filters faces apart, D3D11 across them);\n'
      '    L N.L above 1 with long normals (Wine clamps it, the reference equation does not); F linear fog with start >= end\n'
      '    (undefined; the game never sets it); W table fog on pre-transformed vertices (Wine fogs by z, the reference by\n'
      '    1/RHW); D an alpha op DISABLE on an enabled stage (undefined); N a stage without its texture (ours by the\n'
      '    DirectX 8.1 text, Wine otherwise); T the alpha test (Wine compares the float alpha, ours and the reference\n'
      '    floor(alpha x 255)). Each case below drew the same\n'
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
    env.pop('D8ON11_EXEC_FILTER'); env.pop('D8ON11_EXEC_BATCH')   # with nothing set, the filter and the batches must be on
    sites = m == 'thread' and run_ == 'frames'                       # draw_sites on in one mode: the line, its draws a mod's (t8 is not FFXiMain.dll)
    env.pop('D8ON11_DRAW_SITES', None)
    if sites: env['D8ON11_DRAW_SITES'] = '1'
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
        need = ['executor stats, 600 frames', 'executor filter: 1, batch: 1'] + (['ring stats, 600 frames'] if m != 'direct' else [])
        snap = re.search(r'readback snapshots, \d+ frames: (\d+) locks .* (\d+) read it back', t)
        st = re.search(r'executor draws by game call site, 600 frames \(.*?a frame\): mod [\d.]+/([\d.]+);', t)
        ok = (code == 0 and 'still running after 600 frames' in out and all(n in t for n in need) and
              'ok: a frame-late lock' in out and snap is not None and int(snap.group(1)) > 0 and
              ((st is not None and float(st.group(1)) > 0) if sites else 'executor draws by game call site' not in t))
        what = ('600 frames run (exit code %s), the filter and batches on by default, the stats lines are written, and frame-late locks '
                'of a copied render target read the right colour, some from their snapshot; %s' % (code,
                'draw_sites=1: the draw time by call site logged (all a mod\'s: t8 is not FFXiMain.dll)' if sites else 'draw_sites off: no such line'))
    print('%s: %s mode: %s' % ('ok' if ok else 'FAIL', m, what)); bad += not ok
    for l in t.splitlines():
        if 'stats,' in l or 'snapshots,' in l or 'executor process ended' in l or 'by game call site' in l: print('   ', l)
# the do-not-wait lock (t8 nowait; D8ON11_LOCK_DONOTWAIT, speedfix's occlusion_late uses it): in each mode every lock that
# succeeds reads the copied frame's colour, the others are told "not yet" without a read-back, and answers keep coming
for m in MODES:
    for f in os.listdir(ours):
        if f.endswith('.log'): os.remove(os.path.join(ours, f))
    out = run(ours, 'nowait 1200', True, m)
    t = ''.join(open(os.path.join(ours, f)).read() for f in sorted(os.listdir(ours)) if f.endswith('.log'))
    sn = re.search(r'readback snapshots, \d+ frames: (\d+) locks .*?(\d+) read it\s+back .*?(\d+) do-not-wait', t, re.S)
    ok = out.count('ok: ') == 3 and 'FAIL' not in out and sn is not None and int(sn.group(2)) == 0
    print('%s: %s mode: do-not-wait locks: %s; the log: %s' % ('ok' if ok else 'FAIL', m,
          next((l for l in out.splitlines() if l.startswith('nowait:')), '(no line)'),
          ('%s from a snapshot, %s read back, %s told not yet' % sn.groups()) if sn else '(no snapshot line)')); bad += not ok
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
# dynamic_buffers=1 (7 Nov; the default since 9 Nov, against dynamic_buffers=0, UpdateSubresource): the game's dynamic
# vertex and index buffers written by Map with its lock flags. Every
# scene, the dynamic_buffers scene among them (DISCARD, NOOVERWRITE, a lock with no flags, a second DISCARD of a part,
# a dynamic index buffer), must draw exactly what the default (UpdateSubresource) draws, in each mode, and the executor
# must have written the buffers by Map both ways (its log: the first DISCARD and the first NOOVERWRITE Map)
for m in MODES:
    xlog = os.path.join(ours, 'd3d8on11_exec.log')      # remote mode: the executor's own log (d8x.exe)
    base = scenes(ours, True, m, {'D8ON11_DYNAMIC_BUFFERS': '0'})
    for f in (log, xlog):
        if os.path.exists(f): os.remove(f)
    dyn = scenes(ours, True, m, {'D8ON11_DYNAMIC_BUFFERS': '1'})
    t = ''.join(open(f).read() for f in (log, xlog) if os.path.exists(f))
    same = base == dyn and 'dynamic_buffers' in base and base['dynamic_buffers'] is not None
    ok = same and 'dynamic_buffers=1: the game' in t and 'the first DISCARD Map' in t and 'the first NOOVERWRITE Map' in t
    # the draw-caller trace (9 Nov): t8's own draws return into t8.exe, no mod hands a site over
    ok = ok and ('draw callers: a draw method returns into t8.exe+0x' in t and 'the call site a mod handed over: none' in t or m == 'remote')
    print('%s: %s mode, dynamic_buffers=1 draws exactly what dynamic_buffers=0 draws (%d scenes, the dynamic_buffers scene among them); '
          'Maps with DISCARD and NOOVERWRITE made' % ('ok' if ok else 'FAIL', m, len(base))); bad += not ok
    for n in sorted(set(base) | set(dyn)):
        if base.get(n) != dyn.get(n): print('  differs: %s' % n)
    for l in t.splitlines():
        if 'dynamic buffers' in l: print('   ', l)

# buffer_arena (10 Nov): the dynamic buffers' contents in one shared Direct3D 11 buffer, a batch's uploads written under
# one Map before its draws. Every scene (dynamic_buffers and buffer_arena among them: a buffer written again before each
# of 320 draws, one written once and drawn after them, a dynamic index buffer, buffers made and released in between)
# must draw exactly what it draws without the arena: in each mode, records run one by one and in batches (with the
# filter), with the arena large and with an 8 KB test arena that starts again several times.
for m in MODES:
    xlog = os.path.join(ours, 'd3d8on11_exec.log')
    base = scenes(ours, True, m, {'D8ON11_BUFFER_ARENA': '0'})
    for name, extra in [('one by one', {}), ('batches and filter', {'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': '1'}),
                        ('one by one, an 8 KB arena', {'D8ON11_BUFFER_ARENA_TEST_KB': '8'}),
                        ('batches and filter, an 8 KB arena', {'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': '1', 'D8ON11_BUFFER_ARENA_TEST_KB': '8'})]:
        for f in (log, xlog):
            if os.path.exists(f): os.remove(f)
        got = scenes(ours, True, m, dict(extra, D8ON11_BUFFER_ARENA='1'))
        t = ''.join(open(f).read() for f in (log, xlog) if os.path.exists(f))
        tot = re.search(r'buffer arena totals: (\d+) buffers placed, (\d+) ranges written in place, (\d+) Maps of an arena, (\d+) times an arena started again, (\d+) buffers back', t)
        small = 'TEST_KB' in ''.join(extra)
        ok = (base == got and base.get('buffer_arena') is not None and base.get('dynamic_buffers') is not None and tot is not None
              and int(tot.group(1)) > 400 and (int(tot.group(4)) > 2 and int(tot.group(5)) > 0 if small else int(tot.group(2)) > 0)
              # batches: fewer Maps than buffers placed (how many fewer depends on how far the executor's thread is behind;
              # direct mode's batches are whole frames, so there a quarter at most)
              and ('BATCH' not in ''.join(extra) or int(tot.group(3)) < int(tot.group(1)) // (4 if m == 'direct' else 1)))
        print('%s: %s mode, buffer_arena=1, %s: draws exactly what it draws without the arena (%d scenes); %s' % ('ok' if ok else 'FAIL', m, name, len(base),
              ('%s placed, %s ranges, %s Maps, %s restarts, %s back to their own' % tot.groups()) if tot else 'no totals line')); bad += not ok
        for n in sorted(set(base) | set(got)):
            if base.get(n) != got.get(n): print('  differs: %s' % n)

# draw_merge (11 Nov): consecutive indexed list draws equal in all but their buffers and index range drawn as one, from
# slot arenas. Every scene (draw_merge among them: five character-like parts, each range drawn from two buffer sets,
# overlapping, with a base vertex, a material change and a buffer written again between draws) must draw exactly what
# it draws without it, in each mode, with large arenas and with 512-byte ones that fill and start again; and draws must have merged.
for m in MODES:
    xlog = os.path.join(ours, 'd3d8on11_exec.log')
    common = {'D8ON11_BUFFER_ARENA': '1', 'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': '1'}
    base = scenes(ours, True, m, dict(common, D8ON11_DRAW_MERGE='0'))
    for name, extra in [('16 MB arenas', {}), ('512-byte slot arenas', {'D8ON11_BUFFER_ARENA_TEST_KB': '2'})]:
        for f in (log, xlog):
            if os.path.exists(f): os.remove(f)
        got = scenes(ours, True, m, dict(common, D8ON11_DRAW_MERGE='1', **extra))
        t = ''.join(open(f).read() for f in (log, xlog) if os.path.exists(f))
        tot = re.search(r'draw merge totals: (\d+) merged draws made, (\d+) draws they took in besides their own, (\d+) times', t)
        ok = base == got and base.get('draw_merge') is not None and tot is not None and (int(tot.group(1)) >= 1 and int(tot.group(2)) >= 3 and int(tot.group(3)) >= 1 if '512' in name else int(tot.group(1)) >= 2 and int(tot.group(2)) >= 12)
        print('%s: %s mode, draw_merge=1, %s: draws exactly what it draws without it (%d scenes); %s' % ('ok' if ok else 'FAIL', m, name, len(base),
              ('%s merged draws took in %s others, the arenas started again %s times' % tot.groups()) if tot else 'no totals line')); bad += not ok
        for n in sorted(set(base) | set(got)):
            if base.get(n) != got.get(n): print('  differs: %s' % n)

# draw_delta (11 Nov): a buffer draw made while no state has changed since the last sends only its packet's tail; the
# executor puts it after the last packet's head. Every scene must draw exactly what it draws without it: in each mode,
# records run one by one, and in batches with the arena and draw_merge; and deltas must have been sent.
for m in MODES:
    for name, common in [('one by one', {}), ('batches, arena, merge', {'D8ON11_BUFFER_ARENA': '1', 'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': '1', 'D8ON11_DRAW_MERGE': '1'})]:
        base = scenes(ours, True, m, dict(common, D8ON11_DRAW_DELTA='0'))
        if os.path.exists(log): os.remove(log)
        got = scenes(ours, True, m, dict(common, D8ON11_DRAW_DELTA='1'))
        t = open(log).read() if os.path.exists(log) else ''
        tot = re.search(r'draw delta totals: (\d+) draws sent as deltas, (\d+) in full', t)
        ok = base == got and len(base) > 25 and tot is not None and int(tot.group(1)) > 300 and 'stream error' not in t
        print('%s: %s mode, draw_delta=1, %s: draws exactly what it draws without it (%d scenes); %s' % ('ok' if ok else 'FAIL', m, name, len(base),
              ('%s draws sent as deltas, %s in full' % tot.groups()) if tot else 'no totals line')); bad += not ok
        for n in sorted(set(base) | set(got)):
            if base.get(n) != got.get(n): print('  differs: %s' % n)

# gpu skinning (11 Nov, d8_skin.h): scenes skin_cpu and skin_gpu draw one mesh in the game's skinned layout (one-bone and
# two-bone vertices, duplicates, a bone list naming bone 130, the mirrored variant, a lock that writes the buffers again
# after a job): skinned by t8 on the CPU, and handed to D8ON11_Skin. With gpu_skin=1 the jobs must be taken and every
# scene must equal the run without it (so skin_gpu, drawn by the vertex shader, equals the CPU's picture exactly on this
# rasterizer); in each mode, one by one and with batches, the arena, draw_merge and draw_delta.
for m in MODES:
    base = scenes(ours, True, m, {'D8ON11_GPU_SKIN': '0'})
    for name, common in [('one by one', {}), ('batches, arena, merge, delta', {'D8ON11_BUFFER_ARENA': '1', 'D8ON11_EXEC_BATCH': '1', 'D8ON11_EXEC_FILTER': '1', 'D8ON11_DRAW_MERGE': '1', 'D8ON11_DRAW_DELTA': '1'})]:
        xlog = os.path.join(ours, 'd3d8on11_exec.log')
        for f in (log, xlog):
            if os.path.exists(f): os.remove(f)
        out = run(ours, 'scenes', True, m, dict(common, D8ON11_GPU_SKIN='1'))
        got = {}
        for l in out.splitlines():
            if l.startswith('scene '):
                _, n, hx = l.split(' ', 2); got[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
        t = ''.join(open(f).read() for f in (log, xlog) if os.path.exists(f))
        tot = re.search(r'gpu skin totals: (\d+) jobs, (\d+) palettes sent, (\d+) sources made, (\d+) refused, (\d+) draws not made', t)
        ok = (base == got and base.get('skin_gpu') is not None and base.get('skin_gpu') == base.get('skin_cpu') and 'skin: 3 jobs taken' in out and
              tot is not None and tot.group(1) == '3' and tot.group(4) == '0' and tot.group(5) == '0' and 'stream error' not in t and 'not drawn' not in t and 'skipped' not in t.replace('draw skipped', ''))
        print('%s: %s mode, gpu_skin=1, %s: 3 jobs taken; every scene equals the run without it and skin_gpu equals skin_cpu (%d scenes); %s' % (
              'ok' if ok else 'FAIL', m, name, len(base), ('%s jobs, %s palettes, %s sources, %s refused, %s draws not made' % tot.groups()) if tot else 'no totals line')); bad += not ok
        for n in sorted(set(base) | set(got)):
            if base.get(n) != got.get(n): print('  differs: %s' % n)

# what a second executor thread could take (12 Nov): t8 frames 700 draw (40 quads a frame), thread mode: the log has the
# driver's threading capabilities (D3D11_FEATURE_THREADING) and, after 600 frames, the planning time and the sampled
# draw split (preparing / submitting, 1 draw in 32)
if 'thread' in MODES:
    if os.path.exists(log): os.remove(log)
    out = run(ours, 'frames 700 draw', True, 'thread')
    t = open(log).read() if os.path.exists(log) else ''
    m = re.search(r'executor split, 600 frames: planning ([0-9.]+) ms a frame; draws \((\d+) sampled\): preparing ([0-9.]+) us, submitting ([0-9.]+) us each', t)
    ok = m is not None and int(m.group(2)) > 0 and 'D3D11 threading: the driver\'s concurrent creates' in t and 'presented 699' in out
    print('%s: executor split logged (%s) and the driver\'s threading capabilities' % ('ok' if ok else 'FAIL', m.group(0)[27:] if m else 'no line')); bad += not ok

# the D3D12 probe (12 Nov): once, after the first Present, in the executor's process; each mode runs it and it ends
# (whatever this machine answers: Wine's d3d12 needs Vulkan), t8 still passes; d3d12_probe=0 turns it off
for m in [x for x in ('thread', 'remote') if x in MODES]:
    for on in ('1', '0'):
        for f in (log, xlog):
            if os.path.exists(f): os.remove(f)
        out = run(ours, 'frames 40', True, m, {'D8ON11_D3D12_PROBE': on})
        t = ''.join(open(f).read() for f in (log, xlog) if os.path.exists(f))
        lines = [l for l in t.splitlines() if 'D3D12 probe:' in l]
        ended = any(('ended, everything' in l) or ('no D3D12' in l) or ('has no D3D12CreateDevice' in l) for l in lines)
        ok = 'presented 39' in out and ((on == '1' and lines and 'starting' in lines[0] and ended) or (on == '0' and not lines))
        print('%s: %s mode, d3d12_probe=%s: %s' % ('ok' if ok else 'FAIL', m, on, ' | '.join(l.split('D3D12 probe: ', 1)[1] for l in lines[1:]) or 'no probe lines'))
        bad += not ok

# the hang watch (12 Nov): the executor stalled 8 s before its 3rd Present (D8ON11_TEST_STALL=3, thread mode); the log must
# say after 5 s what the game waits for and what the executor runs and where (op 12 = X_PRESENT, d8_proto.h; its thread
# in d3d8.dll), then that the wait ended; and t8's checks must pass as without the stall
if 'thread' in MODES:
    if os.path.exists(log): os.remove(log)
    out = run(ours, '', True, 'thread', {'D8ON11_TEST_STALL': '3'})
    t = open(log).read() if os.path.exists(log) else ''
    rep = re.search(r'hang watch: the game has waited (\d+) ms for (.*?) \(.*?the executor runs op (\d+) since (\d+) ms.*?executor thread at (\S+), stack:(.*)', t)
    ok = (rep is not None and rep.group(3) == '12' and 'd3d8.dll+' in rep.group(6) and re.search(r'hang watch: the wait for .* ended after \d+ ms', t) is not None
          and 'ALL OK' in out)
    print('%s: hang watch: an 8 s executor stall is reported after 5 s (%s), the executor thread\'s stack in d3d8.dll, then the wait\'s end; t8 passes' % (
          'ok' if ok else 'FAIL', ('%s, op %s' % (rep.group(2), rep.group(3))) if rep else 'no report')); bad += not ok

# ---- the Direct3D 12 executor (12 Nov, d8_exec12.h; backend=d3d12, D8ON11_BACKEND) ----------------------------------------
# Under Wine, Direct3D 12 is vkd3d on Vulkan (Mesa lavapipe) and Direct3D 11 is wined3d on OpenGL (Mesa llvmpipe): two
# rasterizers. So a D3D12 scene is compared with the D3D11 executor's: every pixel equal, or a point-sampling tie (the
# test switch D8ON11_TEST_TIES=0.01 marks the pixels that sample within 0.01 texel of a texel boundary, where Direct3D
# lets the driver read either texel: D3D11.3 functional spec 7.18.7, d8_exec.cpp gen_ps). Measured 12 Nov: 28 of 33
# scenes identical, the other 5 differ only at marked ties.
D12 = {'D8ON11_BACKEND': 'd3d12'}
def logs():
    return ''.join(open(os.path.join(ours, f)).read() for f in sorted(os.listdir(ours)) if f.endswith('.log'))
def clear_logs():
    for f in os.listdir(ours):
        if f.endswith('.log'): os.remove(os.path.join(ours, f))
for m in MODES:
    clear_logs()
    out = run(ours, 'basic', True, m, D12); t = logs()
    # remote mode: the executor process makes its swap chain on the game process's window; Wine's Direct3D 12 refuses
    # that (CreateSwapChainForHwnd 0x8007000E, 12 Nov), so there the stream falls back to Direct3D 11, which is checked
    # instead (whether Windows allows it is not known: not run there)
    fell = m == 'remote' and 'CreateSwapChainForHwnd' in t and 'Direct3D 11 runs the stream' in t
    # (13 Nov) the sampler check at the start: a linear sample between four texels blends, a clamped one clamps
    smp = re.search(r'D3D12 sampler check: samplers copied into the shader heap: linear sample (\d+) .*?clamped sample (\d+) .*?: (\w+)', t)
    ok = ('ALL OK' in out and 'executor shutdown: 0 objects still held' in t and t.count('executor shutdown') == 1 and 'Nothing more is drawn' not in t and
          (('D3D12 executor: device made' in t and 'Direct3D 11 runs the stream' not in t and smp is not None and smp.group(3) == 'right') or fell))
    print('%s: d3d12, %s mode: t8 basic (Reset included) passes %s; it ends holding nothing%s' % ('ok' if ok else 'FAIL', m,
          'after the fallback to Direct3D 11 (Wine makes no Direct3D 12 swap chain on another process\'s window)' if fell else 'on the Direct3D 12 executor',
          '; the sampler check: linear %s, clamped %s, %s' % smp.groups() if smp else '')); bad += not ok
    s11 = scenes(ours, True, m)
    clear_logs(); s12 = scenes(ours, True, m, D12); t = logs()
    st = scenes(ours, True, m, dict(D12, D8ON11_TEST_TIES='0.01'))
    nsame = 0; unmarked = 0; worst = []
    for n in sorted(set(s11) | set(s12)):
        a, b, c = s11.get(n), s12.get(n), st.get(n)
        if not a or not b or len(a) != len(b): unmarked += 1; worst.append('%s: no picture' % n); continue
        if a == b: nsame += 1; continue
        um = [i for i in range(len(a)) if a[i] != b[i] and not (c and c[i] == 0xFF00FF)]
        unmarked += len(um)
        if um: worst.append('%s: %d pixels differ and are not ties' % (n, len(um)))
    ok = unmarked == 0 and len(s12) == len(s11) and 'stream error' not in t
    print('%s: d3d12, %s mode: %d scenes; %d identical to the Direct3D 11 executor\'s, the rest differ only at point-sampling ties%s%s' % (
          'ok' if ok else 'FAIL', m, len(s12), nsame, ' (drawn by Direct3D 11 after the fallback)' if 'Direct3D 11 runs the stream' in t else '',
          ''.join('\n    ' + x for x in worst))); bad += not ok
    for run_, arg, want in (('frames', 'frames 600', 'presented 599'), ('nowait', 'nowait 1200', None)):
        clear_logs(); out = run(ours, arg, True, m, D12); t = logs()
        if want: ok = want in out and 'FAIL' not in out
        else:
            sn = re.search(r'readback snapshots, \d+ frames: (\d+) locks .*?(\d+) read it\s+back .*?(\d+) do-not-wait', t, re.S)
            ok = out.count('ok: ') == 3 and 'FAIL' not in out and sn is not None and int(sn.group(2)) == 0
        print('%s: d3d12, %s mode: t8 %s' % ('ok' if ok else 'FAIL', m, arg)); bad += not ok
    for run_ in ('threads', 'churn'):
        clear_logs()
        env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE=m, **D12)
        try:
            r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', run_], cwd=ours, capture_output=True, text=True, timeout=900, env=env)
            code, out = r.returncode, r.stdout
        except subprocess.TimeoutExpired: code, out = None, '(timed out)'
        t = logs()
        ok = code == 0 and 'ALL OK' in out and 'stream error' not in t and 'executor shutdown: 0 objects still held' in t
        print('%s: d3d12, %s mode: t8 %s (exit %s)' % ('ok' if ok else 'FAIL', m, run_, code)); bad += not ok
    # gpu skinning on Direct3D 12: the jobs taken, and every scene as without it on Direct3D 12
    b12 = scenes(ours, True, m, dict(D12, D8ON11_GPU_SKIN='0'))
    clear_logs(); out = run(ours, 'scenes', True, m, dict(D12, D8ON11_GPU_SKIN='1')); t = logs()
    g12 = {}
    for l in out.splitlines():
        if l.startswith('scene '):
            _, n, hx = l.split(' ', 2); g12[n] = None if hx == 'none' else [int(hx[i:i + 6], 16) for i in range(0, len(hx), 6)]
    tot = re.search(r'gpu skin totals: (\d+) jobs, (\d+) palettes sent, (\d+) sources made, (\d+) refused, (\d+) draws not made', t)
    ok = (g12 == b12 and b12.get('skin_gpu') is not None and b12.get('skin_gpu') == b12.get('skin_cpu') and tot is not None and tot.group(1) == '3'
          and tot.group(5) == '0' and 'skin: 3 jobs taken' in out)
    print('%s: d3d12, %s mode, gpu_skin=1: every scene equals the run without it, skin_gpu equals skin_cpu (%s)' % (
          'ok' if ok else 'FAIL', m, ('%s jobs, %s draws not made' % (tot.group(1), tot.group(5))) if tot else 'no totals line')); bad += not ok
    # draw merging on Direct3D 12 (13 Nov): runs of delta draws drawn as one must draw exactly what the run draws without it;
    # also with the slot buffers forced to start again every second run (D8ON11_TEST_SLOT_WRAP=2), and with gpu skinning
    for name, extra in [('merge', {}), ('merge, slots restarting', {'D8ON11_TEST_SLOT_WRAP': '2'}), ('merge, gpu_skin', {'D8ON11_GPU_SKIN': '1'})]:
        sk = extra.get('D8ON11_GPU_SKIN', '0')
        base12 = scenes(ours, True, m, dict(D12, D8ON11_GPU_SKIN=sk))
        clear_logs(); sm = scenes(ours, True, m, dict(D12, D8ON11_DRAW_MERGE='1', D8ON11_DRAW_DELTA='1', **extra)); t = logs()
        mm = re.search(r'draw_merge: (\d+) merged draws took in (\d+) others, the slots started again (\d+) times', t)
        fell = 'Direct3D 11 runs the stream' in t
        ok = sm == base12 and (fell or (mm is not None and int(mm.group(1)) > 0 and (name != 'merge, slots restarting' or int(mm.group(3)) > 0)))
        print('%s: d3d12, %s mode, %s: every scene as without merging (%s)' % ('ok' if ok else 'FAIL', m, name,
              'Direct3D 11 after the fallback' if fell else ('%s merged draws took in %s others, the slots started again %s times' % mm.groups()) if mm else 'no merge line')); bad += not ok
# the GPU's time and the binds (13 Nov): logged every 600 frames by the Direct3D 12 executor; and the shader cache on disk: a
# second start compiles nothing it compiled before (Wine's Direct3D 12 has no pipeline library: that half is logged as none)
if 'thread' in MODES:
    for f in os.listdir(ours):
        if f.startswith('d3d8on11_d3d12_') and f.endswith('.bin'): os.remove(os.path.join(ours, f))
    got = []
    for k in range(2):
        clear_logs(); out = run(ours, 'frames 700 draw', True, 'thread', D12); t = logs()
        gt = re.search(r'D3D12 gpu time, 600 frames: a frame\'s GPU work ([0-9.]+) ms on average.*?by render target: (.*)', t)
        bl = re.search(r'D3D12 binds, 600 frames: .*?(\d+) shaders compiled \((\d+) from the disk cache\)', t)
        sv = 'D3D12 caches written' in t or os.path.exists(os.path.join(ours, 'd3d8on11_d3d12_shaders.bin'))
        got.append((gt, bl, sv, 'presented 699' in out))
    (g1, b1, s1, p1), (g2, b2, s2, p2) = got
    # (700 frames is under the 1,800 Presents after which the cache is written while running; the first run writes it at
    # shutdown only if its device is released, which t8 frames does not do: so the first run's file is made here by a
    # longer run when it is missing)
    ok = p1 and p2 and g1 is not None and float(g1.group(1)) > 0 and b1 is not None
    print('%s: d3d12: the GPU\'s time per frame and by render target is logged (%s), and the binds line' % ('ok' if ok else 'FAIL', (g1.group(1) + ' ms; ' + g1.group(2)[:80]) if g1 else 'no line')); bad += not ok
    clear_logs(); run(ours, 'frames 2000 draw', True, 'thread', D12)
    clear_logs(); out = run(ours, 'frames 700 draw', True, 'thread', D12); t = logs()
    bl = re.search(r'D3D12 binds, 600 frames: .*?(\d+) shaders compiled \((\d+) from the disk cache\)', t)
    ok = 'presented 699' in out and bl is not None and bl.group(1) == '0' and int(bl.group(2)) > 0 and re.search(r'D3D12 caches: [1-9]\d* compiled shaders from the last starts', t) is not None
    print('%s: d3d12: a start after one that ran 2,000 frames compiles no shader: %s' % ('ok' if ok else 'FAIL', bl.group(0)[bl.group(0).find('frame;') + 7:] if bl else 'no binds line')); bad += not ok
# lazy depth (18 Nov, d8_exec12.cpp bind_targets): every scene and the random cases identical with it on and off, and
# the scene lazy_depth makes it bind passes without their depth surface and bind it later in one (the shutdown line)
if 'thread' in MODES:
    clear_logs(); s_on = scenes(ours, True, 'thread', D12); t = logs()
    s_off = scenes(ours, True, 'thread', dict(D12, D8ON11_D3D12_LAZY_DEPTH='0'))
    pick = lambda o: [l for l in o.splitlines() if l.startswith('scene ')]
    r_on = pick(run(ours, 'random cases.txt', True, 'thread', D12)); r_off = pick(run(ours, 'random cases.txt', True, 'thread', dict(D12, D8ON11_D3D12_LAZY_DEPTH='0')))
    m = re.findall(r'lazy depth on: ([0-9.]+) passes a frame bound without their depth surface, ([0-9.]+) bound it later', t)
    used = bool(m) and float(m[-1][0]) > 0 and float(m[-1][1]) > 0
    ok = s_on == s_off and 'lazy_depth' in s_on and r_on == r_off and len(r_on) > 0 and used
    print('%s: d3d12 lazy depth: %d scenes and %d random cases identical on and off; passes without depth and a later bind seen (%s)' %
          ('ok' if ok else 'FAIL', len(s_on), len(r_on), m[-1] if m else 'no line')); bad += not ok
# mipmaps (19 Nov, d8_mip.cpp, d8_res.cpp, d8_ff.cpp): the level builder's own test (native: box averages exact, DXT
# levels the encoder's output of the averaged level, cutout coverage kept); then in t8, the scene mipmaps_one (a one-level
# texture, the game's POINT/NONE sampling) with mipmaps=1 must equal mipmaps_ref (the same texels with a mip chain t8
# makes by the same 2x2 rule, sampled LINEAR/LINEAR), on both executors; the reference scene and the screen-space strip
# drawn over mipmaps_one are unchanged; after a LockRect with a rectangle rewrites part of level 0 (the levels made
# again in part, mip_update), mipmaps_part_one equals mipmaps_part_ref (t8's whole chain of the new texels); anisotropy=8 runs. Other scenes that minify a one-level texture change on
# purpose and are listed
mt = os.path.join(w, 'mip_test')
r = subprocess.run(['g++', '-O1', '-Wall', '-DD8_MIP_STANDALONE', '-o', mt, os.path.join(H, 'mip_test.cpp'), os.path.join(D8, 'd8_mip.cpp')], capture_output=True, text=True)
out = subprocess.run([mt], capture_output=True, text=True).stdout if r.returncode == 0 else r.stderr
ok = 'ALL OK' in out
print('%s: mip_test (native): %s' % ('ok' if ok else 'FAIL', out.strip().splitlines()[-1] if out.strip() else 'no output')); bad += not ok
if 'thread' in MODES:
    def px(a, b, rows=range(64)): return sum(1 for i, (x, y) in enumerate(zip(a, b)) if i // 64 in rows and x != y)
    for be in ('d3d11', 'd3d12'):
        off = scenes(ours, True, 'thread', dict(D8ON11_BACKEND=be, D8ON11_MIPMAPS='0'))
        clear_logs(); on = scenes(ours, True, 'thread', dict(D8ON11_BACKEND=be, D8ON11_MIPMAPS='1')); t = logs()
        an = scenes(ours, True, 'thread', dict(D8ON11_BACKEND=be, D8ON11_MIPMAPS='1', D8ON11_ANISOTROPY='8'))
        pm = scenes(ours, True, 'thread', dict(D8ON11_BACKEND=be, D8ON11_MIPMAPS='1', D8ON11_MIPMAPS_POINT_MAG='1'))
        ok = bool(on.get('mipmaps_pointmag')) and on.get('mipmaps_pointmag') == off.get('mipmaps_pointmag') and pm.get('mipmaps_pointmag') != off.get('mipmaps_pointmag')
        print('%s: %s: a draw that magnifies POINT (as the names are drawn) keeps the game\'s sampling with mipmaps=1, and uses the levels with mipmaps_point_mag=1' % ('ok' if ok else 'FAIL', be)); bad += not ok
        a, b = on.get('mipmaps_one'), on.get('mipmaps_ref')
        pa, pb = on.get('mipmaps_part_one'), on.get('mipmaps_part_ref')
        ok = bool(a) and a == b and bool(pa) and pa == pb and pa != a and off.get('mipmaps_ref') == b and px(a, off.get('mipmaps_one') or [], range(8)) == 0 and \
             off.get('mipmaps_one') != a and bool(an.get('mipmaps_one')) and 'mipmaps=1: one-level' in t
        print('%s: %s: mipmaps=1 draws the one-level texture as its mip chain sampled trilinear (%s pixels differ), and again after a write to part of it (%s differ); the strip and the reference unchanged; anisotropy=8 draws' %
              ('ok' if ok else 'FAIL', be, px(a, b) if a and b else 'no picture', px(pa, pb) if pa and pb else 'no picture')); bad += not ok
        ch = sorted(n for n in off if not n.startswith('mipmaps') and off[n] != on.get(n))
        print('note: %s: scenes that minify a one-level texture in a 3D draw, changed by mipmaps=1: %s' % (be, ', '.join(ch) or 'none'))
# interp (17 Nov, d8_device.cpp): t8 interp is the step clock and draws a skinned mesh through D8ON11_Skin; each case's
# picture must equal its reference, drawn with the clock off and the palette and world matrix the case expects
# (blended between ticks, on a tick frame, over a frame of two ticks; drawn as handed when the game changed it between
# ticks, when a bone moved 10 units, with no clock). With both executors, with and without draw_delta
if 'thread' in MODES:
    for be in ('d3d11', 'd3d12'):
        for dd in ('0', '1'):
            out = run(ours, 'interp', True, 'thread', dict(D8ON11_BACKEND=be, D8ON11_GPU_SKIN='1', D8ON11_INTERP='1', D8ON11_DRAW_DELTA=dd))
            pics = {l.split(' ', 2)[1]: l.split(' ', 2)[2].strip() for l in out.splitlines() if l.startswith('scene ') and len(l.split(' ', 2)) == 3}
            cases = ('interp_blend', 'interp_tick', 'interp_span2', 'interp_changed', 'interp_warp', 'interp_off')
            bad_c = [c for c in cases if c not in pics or pics.get(c) != pics.get(c + '_ref')]
            disc = pics.get('interp_blend') != pics.get('interp_off_ref')       # the blend is not the unblended picture
            ok = not bad_c and disc and 'interp: a job refused' not in out
            print('%s: interp (%s, draw_delta=%s): every case equals its reference%s' % ('ok' if ok else 'FAIL', be, dd,
                  '' if ok else ' -- differ: %s, blend differs from unblended: %s' % (bad_c, disc))); bad += not ok
# when Direct3D 12 cannot start (D8ON11_TEST_D3D12_FAIL), the stream runs on Direct3D 11: logged, and the scenes are the
# Direct3D 11 executor's exactly
if 'thread' in MODES:
    clear_logs(); sf = scenes(ours, True, 'thread', dict(D12, D8ON11_TEST_D3D12_FAIL='1')); t = logs()
    ok = sf == scenes(ours, True, 'thread') and 'Direct3D 11 runs the stream' in t
    print('%s: d3d12 that cannot start: the stream runs on Direct3D 11 (logged), the scenes exactly the Direct3D 11 executor\'s' % ('ok' if ok else 'FAIL')); bad += not ok
    # a driver whose samplers are wrong by both ways (D8ON11_TEST_D3D12_SAMPLERS makes the check count them wrong): the
    # start runs on Direct3D 11, logged, and draws exactly what the Direct3D 11 executor draws
    clear_logs(); sf = scenes(ours, True, 'thread', dict(D12, D8ON11_TEST_D3D12_SAMPLERS='1')); t = logs()
    ok = sf == scenes(ours, True, 'thread') and 'does not sample as the stream asks by either way: Direct3D 11 runs the stream' in t
    print('%s: d3d12 whose sampler check fails: the stream runs on Direct3D 11 (logged), the scenes exactly the Direct3D 11 executor\'s' % ('ok' if ok else 'FAIL')); bad += not ok
    # sampler tables each in a heap of their own (13 Nov: the owner's Parallels reads a sampler table only at its heap's
    # start; D8ON11_D3D12_DIAG=heaps chooses it here, where the place check is right): the heap check right, logged, and
    # the scenes and 150 random cases exactly the usual Direct3D 12 executor's
    clear_logs(); sf = scenes(ours, True, 'thread', dict(D12, D8ON11_D3D12_DIAG='heaps')); t = logs()
    open(os.path.join(ours, 'cases_heaps.txt'), 'w').write(subprocess.run([sys.executable, os.path.join(H, 'random_cases.py'), '11', '150'], capture_output=True, text=True, check=True).stdout)
    ra = run(ours, 'random cases_heaps.txt', True, 'thread', D12); rb = run(ours, 'random cases_heaps.txt', True, 'thread', dict(D12, D8ON11_D3D12_DIAG='heaps'))
    pick = lambda o: [l for l in o.splitlines() if l.startswith('scene ')]
    ok = sf == scenes(ours, True, 'thread', D12) and 'each in a heap of their own' in t and re.search(r'sampler heap check .*: 128, (0|255): right', t) is not None \
         and len(pick(ra)) == 150 and pick(ra) == pick(rb)
    print('%s: d3d12 with sampler tables in heaps of their own: heap check right, the scenes and %d random cases exactly the usual executor\'s' % ('ok' if ok else 'FAIL', len(pick(rb)))); bad += not ok
    # many tables (fresh: a new one each draw): the heaps are released past 1,024 and made again, and the run goes on
    clear_logs(); out = run(ours, 'frames 600 draw', True, 'thread', dict(D12, D8ON11_D3D12_DIAG='heaps,fresh')); t = logs()
    ok = 'presented 599' in out and 'sampler heaps made: all released and made again' in t
    print('%s: d3d12 with heaps of their own and a new table each draw: past 1,024 heaps all released, t8 frames 600 runs to its end' % ('ok' if ok else 'FAIL')); bad += not ok
    # a heap check that fails (D8ON11_TEST_D3D12_HEAPS_WRONG): Direct3D 11 runs the stream, its scenes exactly
    clear_logs(); sf = scenes(ours, True, 'thread', dict(D12, D8ON11_D3D12_DIAG='heaps', D8ON11_TEST_D3D12_HEAPS_WRONG='1')); t = logs()
    ok = sf == scenes(ours, True, 'thread') and 'reads no sampler table as the stream asks: Direct3D 11 runs the stream' in t
    print('%s: d3d12 whose sampler heap check fails: the stream runs on Direct3D 11 (logged), the scenes exactly the Direct3D 11 executor\'s' % ('ok' if ok else 'FAIL')); bad += not ok
    # a lost device (D8ON11_TEST_D3D12_LOST=50: from the 50th Present the device counts as removed): logged with the
    # device's reason, and nothing waits for the GPU after it, so the game goes on to its end instead of hanging
    clear_logs()
    env = dict(os.environ, WINEDEBUG='-all', WINEDLLOVERRIDES='d3d8=n', D8ON11_MODE='thread', D8ON11_TEST_D3D12_LOST='50', **D12)
    try:
        r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1024x768x24', 'wine', 't8.exe', 'frames', '600'], cwd=ours, capture_output=True, text=True, timeout=300, env=env)
        code, out = r.returncode, r.stdout
    except subprocess.TimeoutExpired: code, out = None, '(timed out)'
    t = logs()
    ok = code is not None and 'presented 599' in out and 'D8ON11_TEST_D3D12_LOST (a test)' in t and 'Nothing more is drawn' in t
    print('%s: d3d12, a device lost at the 50th Present: logged, and t8 frames 600 runs to its end (exit %s) instead of hanging' % ('ok' if ok else 'FAIL', code)); bad += not ok

if os.path.exists(log):
    t = open(log).read()
    for l in t.splitlines():
        if 'not implemented' in l or 'failed' in l or 'skipped' in l: print('  log:', l)
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED (%d)' % bad)
sys.exit(1 if bad else 0)
