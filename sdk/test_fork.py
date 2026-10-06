"""test_fork.py -- fork.py and the declared-differences gate (pipeline/declared.py) on CDx::DrawFadeQuad (0x1000A820,
which sets render states through the device): the unedited fork PASSes; an edit FAILs where it changed the game's
behaviour; the same edit PASSes with its difference declared, and FAILs with a declaration that does not cover it.
The same for a mod's own replacement hooked with XI_HOOK (fork.py gate-hook).

    python3 sdk/test_fork.py            (uses $FFXI_DLL and MinGW; builds the in-game tree once into runs/fork)"""
import os, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__))
D = tempfile.mkdtemp(prefix='forktest_')
bad = 0


def fork(*a):
    return subprocess.run([sys.executable, os.path.join(H, 'fork.py')] + list(a), capture_output=True, text=True)


def check(ok, what):
    global bad
    print('%s: %s' % ('ok' if ok else 'FAIL', what)); bad += not ok


r = fork('new', 'CDx::DrawFadeQuad', D); src = os.path.join(D, 'CDx__DrawFadeQuad.fork.cpp')
check(r.returncode == 0 and os.path.exists(src), 'fork.py new wrote the fork')
check('no change' in fork('diff', src).stdout, 'fork.py diff: an unedited fork is the tree\'s source')
r = fork('gate', src, '--trials', '30', '--work', os.path.join(D, 'w')); print(r.stdout.strip())
check(r.returncode == 0 and ': PASS;' in r.stdout, 'the unedited fork PASSes against the game\'s function')

text = open(src).read()
def variant(name, old, new):
    assert old in text
    p = os.path.join(D, name + '.fork.cpp'); open(p, 'w').write(text.replace(old, new, 1)); return p
def decl(name, body):
    p = os.path.join(D, name + '.json'); open(p, 'w').write(body); return p

fog = variant('fog', 'D3DRS_FOGENABLE, 0);', 'D3DRS_FOGENABLE, 1);')
r = fork('gate', fog, '--trials', '30', '--work', os.path.join(D, 'w')); print(r.stdout.strip())
check(r.returncode == 1 and 'stubbed call #7 differs' in r.stdout and '0x0000001C, 0x00000001' in r.stdout,
      'an edit (FOGENABLE 0 -> 1) FAILs at the call it changed')
r = fork('gate', fog, '--trials', '30', '--work', os.path.join(D, 'w'), '--declare',
         decl('fog', '{"calls": [{"may": "change", "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x1C"}, "args": [1]}]}'))
print(r.stdout.strip())
check(r.returncode == 0 and ': PASS;' in r.stdout and 'declared differences: in ' in r.stdout, 'the same edit, its change declared: PASS, the declaration counted')

drop = variant('drop', '->SetRenderState((*(IDirect3DDevice8 * *)((char *)this + 0xC)), D3DRS_LIGHTING, 0);', '->AddRef((*(IDirect3DDevice8 * *)((char *)this + 0xC)));')
r = fork('gate', drop, '--trials', '30', '--work', os.path.join(D, 'w'), '--declare',
         decl('lighting', '{"calls": [{"may": "drop", "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x89"}}]}'))
print(r.stdout.strip())
check(r.returncode == 1 and 'undeclared: the change calls IDirect3DDevice8::AddRef' in r.stdout,
      'a drop declared but an added call not: FAIL, naming the added call')
r = fork('gate', drop, '--trials', '30', '--work', os.path.join(D, 'w'), '--declare',
         decl('both', '{"calls": [{"may": "drop", "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x89"}},'
                      ' {"may": "add", "call": "IDirect3DDevice8::AddRef"}]}'))
print(r.stdout.strip())
check(r.returncode == 0 and ': PASS;' in r.stdout, 'the drop and the added call both declared: PASS')
r = fork('gate', drop, '--trials', '30', '--work', os.path.join(D, 'w'), '--declare',
         decl('wrong', '{"calls": [{"may": "drop", "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x7"}},'
                       ' {"may": "add", "call": "IDirect3DDevice8::AddRef"}]}'))
print(r.stdout.strip())
check(r.returncode == 1 and 'the original calls IDirect3DDevice8::SetRenderState(this=0x' in r.stdout and 'never: ' in r.stdout,
      'a declaration for another state (ZENABLE) does not cover the dropped LIGHTING: FAIL, and it is reported never seen')
# a mod's own replacement (XI_HOOK, not a fork): fork.py gate-hook
HOOK = r"""#include "xikit_sdk.hpp"
XIKIT_MOD_GLOBALS
static xikit::fn<decltype(&CDx::DrawFadeQuad)> prev;
static int __thiscall my_DrawFadeQuad(CDx *self, int a1, int a2) { BODY }
extern "C" __declspec(dllexport) int xikit_mod_init(const XikitHost *host) { return XI_HOOK(host, CDx::DrawFadeQuad, my_DrawFadeQuad, prev); }
"""
def hook(name, body):
    p = os.path.join(D, name + '.cpp'); open(p, 'w').write(HOOK.replace('BODY', body)); return p
def gate_hook(src, *extra):
    r = fork('gate-hook', 'CDx::DrawFadeQuad', src, '--trials', '30', '--work', os.path.join(D, 'w'), *extra)
    print(r.stdout.strip(), r.stderr.strip()[-1500:]); return r
r = gate_hook(hook('pass', 'return prev(self, a1, a2);'))
check(r.returncode == 0 and ': PASS;' in r.stdout, 'gate-hook: a replacement that calls previous (the game\'s code) PASSes')
r = gate_hook(hook('seven', 'return 7;'))
check(r.returncode == 1 and 'stubbed call #1 differs' in r.stdout and 'reconstruction no call' in r.stdout,
      'gate-hook: a replacement that does nothing FAILs at the first call it does not make')
w = hook('write', 'int r = prev(self, a1, a2); *(volatile int *)((char *)self + 0x908) = 5; return r;')
r = gate_hook(w)
check(r.returncode == 1 and 'memory differs at 4 byte(s)' in r.stdout, 'gate-hook: an extra write to this+0x908 FAILs')
r = gate_hook(w, '--declare', decl('write', '{"memory": [{"base": "this", "offset": "0x908", "size": 4}]}'))
check(r.returncode == 0 and ': PASS;' in r.stdout and '"offset": "0x908"' in r.stdout, 'gate-hook: the same write declared: PASS')
print('ALL OK' if not bad else 'FAILED')
sys.exit(1 if bad else 0)
