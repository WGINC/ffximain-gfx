#!/usr/bin/env python3
"""test_pdb.py -- FFXiMain.pdb read by a second reader: Windows' dbghelp API (Wine's implementation here).

    FFXI_DLL=/path/FFXiMain.dll python3 sdk/pdb/test_pdb.py      (needs make_pdb.py's tools, i686-w64-mingw32-gcc, wine)

make_pdb.py itself reads every symbol back with llvm-pdbutil. This loads the PDB the way a debugger does (through the
throwaway image whose debug record names it; the game's own DLL has no usable record) and asks dbghelp for: the
function at an address and inside a function, a function by name, a global by address, and a class's size. The
expected answers come from the repository's own data (the manifest, func_extents, the tree's static_assert on CDx).
"""
import json, os, re, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); R = os.path.join(H, '..', '..')
w = tempfile.mkdtemp(prefix='xipdb_'); bad = 0
def check(ok, what):
    global bad; print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok
r = subprocess.run([sys.executable, os.path.join(H, 'make_pdb.py'), w], env=dict(os.environ, XI_PDB_KEEP='1'), capture_output=True, text=True)
check(r.returncode == 0, 'make_pdb.py: ' + (r.stdout.strip().splitlines()[0] if r.stdout.strip() else r.stderr.strip()[-300:]))
if r.returncode == 0:
    syms = json.load(open(os.path.join(w, 'FFXiMain.pdb.json')))['symbols']
    fn = next(s for s in syms if s['name'] == 'CApp::RunMainLoop'); gl = next(s for s in syms if s['name'] == 'g_pCDx')
    fa = int(fn['address'], 16); ga = int(gl['address'], 16)
    m = re.search(r'static_assert\(sizeof\(CDx\) == (0x[0-9A-Fa-f]+|\d+)', open(os.path.join(R, 'tree/include/CDx.hpp')).read())
    subprocess.run(['i686-w64-mingw32-gcc', '-O1', '-o', os.path.join(w, 'pdb_check.exe'), os.path.join(H, 'test', 'pdb_check.c'), '-ldbghelp'], check=True)
    out = subprocess.run(['wine', 'pdb_check.exe', 'layout_throwaway.dll', '%X' % fa, '%X' % (fa + fn['size'] - 1), '%X' % ga], cwd=w,
                         env=dict(os.environ, WINEDEBUG='-all'), capture_output=True, text=True, timeout=600).stdout
    check('symbol type 3' in out, 'dbghelp loads the PDB (symbol type 3 = PDB)')
    check('%x = CApp::RunMainLoop+0x0 (size %d)' % (fa, fn['size']) in out, 'the function at 0x%X is CApp::RunMainLoop, %d bytes' % (fa, fn['size']))
    check('%x = CApp::RunMainLoop+0x%x' % (fa + fn['size'] - 1, fn['size'] - 1) in out, 'its last byte is still CApp::RunMainLoop')
    check('%x = g_pCDx+0x0' % ga in out, 'the global at 0x%X is g_pCDx' % ga)
    check('CApp::RunMainLoop = %x' % fa in out, 'CApp::RunMainLoop is found by name')
    if m: check('type CDx:' in out and ', %d bytes' % int(m.group(1), 0) in out, 'class CDx is %d bytes, as the tree\'s static_assert says' % int(m.group(1), 0))
    else: check('type CDx:' in out, 'class CDx is in the type stream (the tree header has no sizeof static_assert to compare)')
    if bad: print(out)
shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED'); sys.exit(1 if bad else 0)
