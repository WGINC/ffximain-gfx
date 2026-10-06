"""test_xikit.py -- the kit end to end under Wine, with the real FFXiMain.dll ($FFXI_DLL) and the built dist/:
host.exe (host.c) maps the game DLL without running it, fakes the device, loads xikit.dll and plays the render
thread. Runs: the game DLL at its analysed base, loaded elsewhere (0x10000000 occupied first), at its base with
profile=1 (d3dprof built in), with a mod (mod_test.cpp, built with sdk/build_mod.sh; function and device hooks) at its
base and loaded elsewhere, the mod reloaded while running (mod_reload=1), capture= and facts=1.

    python3 test_xikit.py            (after ../build.sh; needs wine)"""
import json, os, re, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); DIST = os.path.join(H, '..', 'dist')
subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-Wall', '-o', os.path.join(H, 'host.exe'), os.path.join(H, 'host.c'), '-static-libgcc'], check=True)
bad = 0
# Wine maps images the Windows loader refuses, so check the Windows layout rules here: sections contiguous from
# SizeOfHeaders (rounded to SectionAlignment), SizeOfImage the end of the last one, raw data aligned and in the file
import pefile
for f in ('xikit.dll', 'gfx_recon_ingame.dll'):
    p = os.path.join(DIST, f); pe = pefile.PE(p); oh = pe.OPTIONAL_HEADER; al = oh.SectionAlignment
    up = lambda n: (n + al - 1) // al * al; s = sorted(pe.sections, key=lambda x: x.VirtualAddress); n = os.path.getsize(p)
    why = [] if s[0].VirtualAddress == up(oh.SizeOfHeaders) else ['first section does not follow the headers']
    why += ['gap or overlap after %s' % a.Name.rstrip(b'\0').decode() for a, b in zip(s, s[1:])
            if a.VirtualAddress + up(a.Misc_VirtualSize) != b.VirtualAddress]
    why += [] if oh.SizeOfImage == s[-1].VirtualAddress + up(s[-1].Misc_VirtualSize) else ['SizeOfImage']
    why += ['raw data of %s' % x.Name.rstrip(b'\0').decode() for x in s
            if x.SizeOfRawData and (x.PointerToRawData % oh.FileAlignment or x.PointerToRawData + x.SizeOfRawData > n)]
    print('%s: %s the Windows loader\'s layout rules%s' % ('ok' if not why else 'FAIL', f, '' if not why else ': ' + '; '.join(why)))
    bad += bool(why)
SDK = os.path.join(H, '..', '..', '..', 'sdk')
MODDLL = os.path.join(tempfile.mkdtemp(prefix='xikit_mod_'), 'mod_test.dll')
# the mod forks SetColor: fork.py new writes the tree's source of it, unedited
subprocess.run([sys.executable, os.path.join(SDK, 'fork.py'), 'new', 'CXiSkeletonActor::SetColor', os.path.dirname(MODDLL)], check=True)
FORK = os.path.join(os.path.dirname(MODDLL), 'CXiSkeletonActor__SetColor.fork.cpp')
subprocess.run([os.path.join(SDK, 'build_mod.sh'), MODDLL, os.path.join(H, 'mod_test.cpp'), FORK], check=True)
MODDLL2 = os.path.join(os.path.dirname(MODDLL), 'mod_test2.dll')          # the "rebuilt" mod of the reload run
subprocess.run([os.path.join(SDK, 'build_mod.sh'), MODDLL2, os.path.join(H, 'mod_test.cpp'), FORK, '--', '-DMOD_GEN=2'], check=True)
r = subprocess.run([sys.executable, os.path.join(SDK, 'fork.py'), 'gate', FORK, '--trials', '20'], capture_output=True, text=True)
print(r.stdout.strip(), r.stderr.strip()[-1500:])
ok = r.returncode == 0 and ': PASS;' in r.stdout
print('%s: fork.py gate: the unedited fork, compiled as the mod compiles it, PASSes against the game\'s SetColor' % ('ok' if ok else 'FAIL')); bad += not ok
for src in ('examples/overlay.cpp', 'template/my_mod.cpp'):     # the SDK's example and template build (not run here:
    subprocess.run([os.path.join(SDK, 'build_mod.sh'), os.path.join(os.path.dirname(MODDLL), os.path.basename(src) + '.dll'),
                    os.path.join(SDK, src)], check=True)          # the host's fake device has Present only)
FACTS = os.path.join(os.path.dirname(MODDLL), 'xikit_facts.txt')
subprocess.run([sys.executable, os.path.join(H, '..', '..', '..', 'pipeline', 'fact_sites.py'), 'sites', FACTS, '0x10009716', '0x10009736'], check=True)
for mode in ('base', 'relocated', 'profile', 'mod', 'modrel', 'reload', 'capture', 'facts', 'ab'):
    d = tempfile.mkdtemp(prefix='xikit_')
    for f in ('xikit.dll', 'gfx_recon_ingame.dll', 'xikit_manifest.txt', 'VERSION.txt'): shutil.copy(os.path.join(DIST, f), d)
    shutil.copy(os.environ['FFXI_DLL'], os.path.join(d, 'FFXiMain.dll')); shutil.copy(os.path.join(H, 'host.exe'), d)
    if mode.startswith('mod') or mode == 'reload': shutil.copy(MODDLL, d)
    if mode == 'reload': shutil.copy(MODDLL2, d)
    if mode == 'facts': shutil.copy(FACTS, d)
    env = dict(os.environ, WINEDEBUG='-all')
    r = subprocess.run(['wine', 'host.exe', mode], cwd=d, capture_output=True, text=True, timeout=600, env=env)
    print('== %s (exit %d)' % (mode, r.returncode)); print(r.stdout.strip())
    log = open(os.path.join(d, 'xikit.log')).read() if os.path.exists(os.path.join(d, 'xikit.log')) else ''
    if mode == 'base':
        ok = 'kit build: ' + open(os.path.join(DIST, 'VERSION.txt')).readline().strip() in log
        print('%s: xikit.log names the kit\'s build (VERSION.txt line 1)' % ('ok' if ok else 'FAIL')); bad += not ok
    print('-- xikit.log --'); print('\n'.join(l for l in log.splitlines() if 'swapped ' not in l and 'restored ' not in l)[-3000:])
    if r.returncode != 0 or 'ALL OK' not in r.stdout: bad += 1
    if mode == 'base':
        ok = re.search(r'cpu: CPUID vendor "\w+", brand ".*"; SSE 1', log) is not None and 'machine: ' in log
        print('%s: the environment is logged (CPUID vendor and features, machine and Windows build)' % ('ok' if ok else 'FAIL')); bad += not ok
        ok = re.search(r'compatibility: kernel version \d+\.\d+ build \d+ \(KUSER_SHARED_DATA; the game is told \d+\.\d+\); __COMPAT_LAYER .*; registry layers for .*host\.exe: user .*, machine ', log) is not None
        print('%s: the compatibility layers are logged (the kernel\'s own version, __COMPAT_LAYER, the registry)' % ('ok' if ok else 'FAIL')); bad += not ok
    if mode == 'profile':
        vs = [f for f in os.listdir(d) if f.startswith('xikit_vshader_')]
        r = subprocess.run([sys.executable, os.path.join(H, '..', '..', 'd3dprof', 'vshader.py')] + [os.path.join(d, f) for f in vs], capture_output=True, text=True)
        print(r.stdout.strip(), r.stderr.strip()[-500:])
        ok = len(vs) == 1 and 'v5 <- D3DCOLOR' in r.stdout and 'm4x4 oPos, v0, c0' in r.stdout and 'mov oD0, v5' in r.stdout
        print('%s: the vertex shader was saved and vshader.py decodes its declaration and code' % ('ok' if ok else 'FAIL')); bad += not ok
        sm = [f for f in os.listdir(d) if f.startswith('xikit_samples_')]
        if sm:
            r = subprocess.run([sys.executable, os.path.join(H, '..', '..', 'd3dprof', 'samples.py'), os.path.join(d, sm[0])], capture_output=True, text=True)
            ok = r.returncode == 0 and 'self: where the render thread was' in r.stdout
            print('\n'.join(r.stdout.splitlines()[:8])); print('%s: samples.py reads the samples' % ('ok' if ok else 'FAIL')); bad += not ok
    if mode == 'profile':
        pics = sorted(f for f in os.listdir(d) if f.endswith('.pic') and '_1_f' in f)
        r = subprocess.run([sys.executable, os.path.join(H, '..', '..', 'd3dprof', 'pictures.py')] + [os.path.join(d, f) for f in pics], capture_output=True, text=True)
        print(r.stdout.strip(), r.stderr.strip()[-800:])
        ok = len(pics) == 5 and "beyond what the control shows" in r.stdout and 'control  frame' in r.stdout and '(0.0000%)' in r.stdout
        print('%s: trace_pictures=1 saved frames 6..10 and pictures.py finds the swapped band in the swapped frames only' % ('ok' if ok else 'FAIL')); bad += not ok
    if mode == 'facts':
        fc = [f for f in os.listdir(d) if f.startswith('xikit_facts_')]
        out = os.path.join(d, 'game_sp.json')
        r = subprocess.run([sys.executable, os.path.join(H, '..', '..', '..', 'pipeline', 'fact_sites.py'), 'ingest'] + [os.path.join(d, f) for f in fc] + ['--out', out], capture_output=True, text=True)
        print(r.stdout.strip(), r.stderr.strip()[-800:])
        g = json.load(open(out)) if os.path.exists(out) else {}
        ok = g.get('0x10009716', {}).get('pops') == 4 and g.get('0x10009736', {}).get('pops') == 4
        print('%s: fact_sites.py ingest records both sites with 4 bytes popped' % ('ok' if ok else 'FAIL')); bad += not ok
    if mode == 'capture':
        # the capture replayed through the gate: the original and the tree's reconstruction run from that call
        xc = [f for f in os.listdir(d) if f.endswith('.xcap')]
        tree_dll = os.path.join(H, '..', '..', '..', 'tree', 'build', 'gfx_recon.dll')
        if not os.path.exists(tree_dll):                    # the gate's build of the tree (not the kit's in-game one)
            subprocess.run(['./build.sh'], cwd=os.path.join(H, '..', '..', '..', 'tree'), check=True, stdout=subprocess.DEVNULL)
        cases = os.path.join(H, '..', '..', '..', 'verification', 'cases_clean_final.json')
        if len(xc) != 2: print('FAIL: %d .xcap files written, not 2' % len(xc)); bad += 1
        for x in sorted(xc):
            r = subprocess.run([sys.executable, os.path.join(H, '..', '..', '..', 'pipeline', 'xcap.py'), os.path.join(d, x), cases,
                                tree_dll, os.path.join(d, 'replay')], capture_output=True, text=True)
            print(r.stdout.strip(), r.stderr.strip()[-1500:])
            ok = r.returncode == 0 and ': PASS;' in r.stdout and "'return': 8" in r.stdout and 'this 0x' in r.stdout
            if '_A4830_' in x: ok = ok and "args ['0x5566AABB']" in r.stdout          # SetColor's argument, read off the captured stack
            print('%s: %s replays through the gate: PASS on all 8 trials, from the captured this and arguments' % ('ok' if ok else 'FAIL', x))
            bad += not ok
    if os.environ.get("KEEP"): print("kept", d)
    else: shutil.rmtree(d, ignore_errors=True)
sys.exit(1 if bad else 0)
