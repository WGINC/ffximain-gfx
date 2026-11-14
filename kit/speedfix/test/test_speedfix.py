"""test_speedfix.py -- the speedfix mod (kit/speedfix) offline:
  1. sf_host.exe under Wine with the real FFXiMain.dll ($FFXI_DLL), the kit (kit/xikit/dist) and speedfix.dll: the
     caches, batching, the thread rule and every game-code change (sf_host.c lists the checks); in 'pacing' mode the
     game's own frame limiter code runs with precise_pacing on and off, in 'step' mode it and the game's step code with
     uncap and frame_step;
  2. the gate on its hook of Occlusion_QueryScreenRect (sdk/fork.py gate-hook): with its switches off it must be the
     game's function.

    python3 test_speedfix.py          (after kit/xikit/build.sh and kit/speedfix/build.sh; needs wine)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); SF = os.path.join(H, '..'); ROOT = os.path.join(SF, '..', '..')
KIT = os.path.join(ROOT, 'kit', 'xikit', 'dist')
bad = 0
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-Wno-unused-parameter', '-o', os.path.join(H, 'sf_host.exe'),
                os.path.join(H, 'sf_host.c'), '-static-libgcc', '-lwinmm'], check=True)
d = tempfile.mkdtemp(prefix='speedfix_')
for f in ('xikit.dll', 'gfx_recon_ingame.dll', 'xikit_manifest.txt', 'VERSION.txt'): shutil.copy(os.path.join(KIT, f), d)
for f in os.listdir(KIT):                                   # the manifests of other builds (build translation)
    if f.startswith('xikit_manifest_'): shutil.copy(os.path.join(KIT, f), d)
shutil.copy(os.path.join(SF, 'dist', 'speedfix.dll'), d); shutil.copy(os.path.join(H, 'sf_host.exe'), d)
shutil.copy(os.environ['FFXI_DLL'], os.path.join(d, 'FFXiMain.dll'))
for mode in ('minimal', 'full', 'lazy', 'blocks', 'pacing', 'step', 'group'):
    for f in ('xikit.log', 'speedfix.ini'):
        if os.path.exists(os.path.join(d, f)): os.remove(os.path.join(d, f))
    r = subprocess.run(['wine', 'sf_host.exe', mode], cwd=d, capture_output=True, text=True, timeout=900, env=dict(os.environ, WINEDEBUG='-all'))
    print('== %s' % mode); print(r.stdout.strip())
    log = open(os.path.join(d, 'xikit.log')).read() if os.path.exists(os.path.join(d, 'xikit.log')) else ''
    if r.returncode != 0 or 'ALL OK' not in r.stdout: bad += 1
    if mode == 'minimal':
        ok = 'speedfix: 20 device methods hooked, 3 flush guards' in log
        print('%s: minimal: 20 device methods hooked (the four draws for null_draws), 3 flush guards (CopyRects, UpdateTexture, GetFrontBuffer: merge_scenes)' % ('ok' if ok else 'FAIL')); bad += not ok
    import re
    if mode == 'full':
        ok = 'speedfix: cpu tier in use: x87' in log
        print('%s: full: the tier the game chose is logged (x87 class vtable)' % ('ok' if ok else 'FAIL')); bad += not ok
        mv = re.findall(r'motion: camera moving: (\d+) frames', log); st = re.findall(r'motion: camera still : (\d+) frames', log)
        ok = sum(map(int, mv)) >= 29 and sum(map(int, st)) >= 30 and 'merge_scenes: ' in log
        print('%s: full: motion_stats splits still and moving frames (still %s, moving %s); merge_scenes counted its drops' % ('ok' if ok else 'FAIL', st, mv)); bad += not ok
    if mode == 'lazy':
        import re
        ab = re.search(r'speedfix: ab 1 \(lazy_state, bogus\): A \(off\) [0-9.]+ ms, B \(on\) [0-9.]+ ms', log)
        ok = bool(ab) and 'phase 1 of 2: A (the levers off)' in log and 'phase 2 of 2: B' in log and 'bogus (not a lever)' in log
        print('%s: lazy: the lever A/B ran A then B and compared them (an unknown name left out and logged)' % ('ok' if ok else 'FAIL')); bad += not ok
        # (24 Nov) each phase and the end leave a mark for the measuring kit (xikit_mark.txt, as mark.bat writes it); no kit
        # reads it here, so the last one written stays: the end
        mk = os.path.join(d, 'xikit_mark.txt'); mt = open(mk).read().strip() if os.path.exists(mk) else ''
        ok = mt == 'ab end'
        print('%s: lazy: the A/B left its marks for the measuring kit (the last: %r)' % ('ok' if ok else 'FAIL', mt)); bad += not ok
        if os.path.exists(mk): os.remove(mk)
# shadow_fit=1 (16 Nov): the hook of Shadow_DrawByMode goes in and says so (the minimal host run with the setting)
for f in ('xikit.log', 'speedfix.ini'):
    if os.path.exists(os.path.join(d, f)): os.remove(os.path.join(d, f))
r = subprocess.run(['wine', 'sf_host.exe', 'minimal'], cwd=d, capture_output=True, text=True, timeout=900,
                   env=dict(os.environ, WINEDEBUG='-all', SF_HOST_INI='[speedfix]\nshadow_fit=1\nname_smooth=1\nstats_every=1\n'))
slog = open(os.path.join(d, 'xikit.log')).read() if os.path.exists(os.path.join(d, 'xikit.log')) else ''
ok = r.returncode == 0 and 'speedfix: shadow_fit=1: simple shadows sized by each character' in slog
print('%s: shadow_fit=1: Shadow_DrawByMode hooked (logged)' % ('ok' if ok else 'FAIL')); bad += not ok
ok = r.returncode == 0 and 'speedfix: name_smooth=1: names sized from their unrounded depth' in slog
print('%s: name_smooth=1: CXiActor::NameCalc hooked (logged)' % ('ok' if ok else 'FAIL')); bad += not ok
# sound_retry (14 Nov): on by default, the two hooks go in and say so (this run), and the counts line is written (the last
# mode's run, whose statistics lines are on)
ok = r.returncode == 0 and 'speedfix: sound_retry=1: a sound the game keeps when unheard is kept when refused a voice' in slog and \
     re.search(r'speedfix: sound_retry=1: \d+ sound restarts asked for a voice since the last line; refused a voice: \d+ kept to ask again, \d+ deleted', log) is not None
print('%s: sound_retry=1 (the default): the voice request and the element update hooked, the counts logged' % ('ok' if ok else 'FAIL')); bad += not ok
print('-- xikit.log (speedfix lines) --'); print('\n'.join(l for l in log.splitlines() if 'speedfix' in l or 'mod ' in l)[-4000:])
ok = 'speedfix.dll (the speedfix mod) is loaded' in log
print('%s: the kit leaves the functions speedfix changes inside to the game\'s code (respect_speedfix)' % ('ok' if ok else 'FAIL')); bad += not ok
g = subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate-hook', 'Occlusion_QueryScreenRect',
                    os.path.join(SF, 'sf_game.cpp'), os.path.join(H, 'gate_stub.cpp'), '--trials', '30',
                    '--work', os.path.join(d, 'w')], capture_output=True, text=True)
print(g.stdout.strip(), g.stderr.strip()[-2000:])
ok = g.returncode == 0 and ': PASS;' in g.stdout
print('%s: the gate: speedfix\'s Occlusion_QueryScreenRect hook, switches off, is the game\'s function' % ('ok' if ok else 'FAIL')); bad += not ok
g = subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate-hook', 'Shadow_DrawByMode',
                    os.path.join(SF, 'sf_game.cpp'), os.path.join(H, 'gate_stub.cpp'), '--trials', '30',
                    '--work', os.path.join(d, 'w2')], capture_output=True, text=True)
print(g.stdout.strip(), g.stderr.strip()[-2000:])
ok = g.returncode == 0 and ': PASS;' in g.stdout
print('%s: the gate: speedfix\'s Shadow_DrawByMode hook, shadow_fit off, is the game\'s function' % ('ok' if ok else 'FAIL')); bad += not ok
g = subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate-hook', 'CXiActor::NameCalc',
                    os.path.join(SF, 'sf_game.cpp'), os.path.join(H, 'gate_stub.cpp'), '--trials', '30',
                    '--work', os.path.join(d, 'w3')], capture_output=True, text=True)
print(g.stdout.strip(), g.stderr.strip()[-2000:])
ok = g.returncode == 0 and ': PASS;' in g.stdout
print('%s: the gate: speedfix\'s CXiActor::NameCalc hook, name_smooth off, is the game\'s function' % ('ok' if ok else 'FAIL')); bad += not ok
for fn, what in (('Unit10036B90_SetVolumeVector', 'the voice request'), ('CYySoundElem::UpdateTimerAndPosition', 'the sound element update')):
    g = subprocess.run([sys.executable, os.path.join(ROOT, 'sdk', 'fork.py'), 'gate-hook', fn,
                        os.path.join(SF, 'sf_game.cpp'), os.path.join(H, 'gate_stub.cpp'), '--trials', '30',
                        '--work', os.path.join(d, 'w_' + fn.replace(':', '_'))], capture_output=True, text=True)
    print(g.stdout.strip(), g.stderr.strip()[-2000:])
    ok = g.returncode == 0 and ': PASS;' in g.stdout
    print('%s: the gate: speedfix\'s %s hook (%s), sound_retry off, is the game\'s function' % ('ok' if ok else 'FAIL', fn, what)); bad += not ok
if os.environ.get('KEEP'): print('kept', d)
else: shutil.rmtree(d, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED'); sys.exit(1 if bad else 0)
