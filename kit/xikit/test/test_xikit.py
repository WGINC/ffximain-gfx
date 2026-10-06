"""test_xikit.py -- the kit end to end under Wine, with the real FFXiMain.dll ($FFXI_DLL) and the built dist/:
host.exe (host.c) maps the game DLL without running it, fakes the device, loads xikit.dll and plays the render
thread. Run four times: with the game DLL at its analysed base, loaded elsewhere (0x10000000 occupied first), at its
base with profile=1 (d3dprof built in), and with a mod (mod_test.cpp, built with sdk/build_mod.sh) at its base and
loaded elsewhere.

    python3 test_xikit.py            (after ../build.sh; needs wine)"""
import os, shutil, subprocess, sys, tempfile
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
subprocess.run([os.path.join(SDK, 'build_mod.sh'), MODDLL, os.path.join(H, 'mod_test.cpp')], check=True)
for src in ('examples/overlay.cpp', 'template/my_mod.cpp'):     # the SDK's example and template build (not run here:
    subprocess.run([os.path.join(SDK, 'build_mod.sh'), os.path.join(os.path.dirname(MODDLL), os.path.basename(src) + '.dll'),
                    os.path.join(SDK, src)], check=True)          # the host's fake device has Present only)
for mode in ('base', 'relocated', 'profile', 'mod', 'modrel'):
    d = tempfile.mkdtemp(prefix='xikit_')
    for f in ('xikit.dll', 'gfx_recon_ingame.dll', 'xikit_manifest.txt'): shutil.copy(os.path.join(DIST, f), d)
    shutil.copy(os.environ['FFXI_DLL'], os.path.join(d, 'FFXiMain.dll')); shutil.copy(os.path.join(H, 'host.exe'), d)
    if mode.startswith('mod'): shutil.copy(MODDLL, d)
    env = dict(os.environ, WINEDEBUG='-all')
    r = subprocess.run(['wine', 'host.exe', mode], cwd=d, capture_output=True, text=True, timeout=600, env=env)
    print('== %s (exit %d)' % (mode, r.returncode)); print(r.stdout.strip())
    log = open(os.path.join(d, 'xikit.log')).read() if os.path.exists(os.path.join(d, 'xikit.log')) else ''
    print('-- xikit.log --'); print('\n'.join(l for l in log.splitlines() if 'swapped ' not in l and 'restored ' not in l)[-3000:])
    if r.returncode != 0 or 'ALL OK' not in r.stdout: bad += 1
    if os.environ.get("KEEP"): print("kept", d)
    else: shutil.rmtree(d, ignore_errors=True)
sys.exit(1 if bad else 0)
