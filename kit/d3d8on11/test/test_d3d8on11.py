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
    env.pop('D8ON11_EXEC_FILTER')                 # with nothing set, the filter must be on
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
        need = ['executor stats, 600 frames', 'executor filter: 1'] + (['ring stats, 600 frames'] if m != 'direct' else [])
        snap = re.search(r'readback snapshots, \d+ frames: (\d+) locks .* (\d+) read it back', t)
        ok = (code == 0 and 'still running after 600 frames' in out and all(n in t for n in need) and
              'ok: a frame-late lock' in out and snap is not None and int(snap.group(1)) > 0)
        what = ('600 frames run (exit code %s), the filter on by default, the stats lines are written, and frame-late locks '
                'of a copied render target read the right colour, some from their snapshot' % code)
    print('%s: %s mode: %s' % ('ok' if ok else 'FAIL', m, what)); bad += not ok
    for l in t.splitlines():
        if 'stats,' in l or 'snapshots,' in l or 'executor process ended' in l: print('   ', l)
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
