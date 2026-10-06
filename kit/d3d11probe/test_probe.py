"""test_probe.py -- phase 0's probes (kit/d3d11probe) offline: the x86 builds under Wine, on Mesa's software
renderer in a virtual X server (xvfb-run), and the report script. What this shows: the programs run, write every
result line, and the report reads them. What it cannot show: any number for the owner's machine (Wine's D3D on
llvmpipe), the ARM64 build (no ARM64 Windows here), or the cross-process pixel checks (Wine's PrintWindow does not see
OpenGL output, and Wine has no shared D3D11 textures).

    python3 test_probe.py          (after build.sh; needs wine, xvfb-run, i386 Mesa)"""
import os, shutil, subprocess, sys, tempfile
H = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(H, 'dist')
w = tempfile.mkdtemp(prefix='probe_')
for f in ('probe11_x86.exe', 'probe11_arm64.exe', 'probe8_x86.exe'): shutil.copy(os.path.join(D, f), w)
cmd = 'wine probe11_x86.exe caps o; wine probe11_x86.exe bench o; wine probe8_x86.exe bench o; wine probe11_x86.exe xproc o probe11_x86.exe'
r = subprocess.run(['xvfb-run', '-a', '-s', '-screen 0 1600x900x24', 'sh', '-c', cmd], cwd=w, capture_output=True, text=True,
                   timeout=1800, env=dict(os.environ, WINEDEBUG='-all'))
o = os.path.join(w, 'o'); bad = 0
def has(f, keys):
    p = os.path.join(o, f); s = open(p).read() if os.path.exists(p) else ''
    miss = [k for k in keys if '\n' + k + ',' not in '\n' + s]
    return miss
checks = [
    ('probe11_x86.csv', ['process', 'module', 'device.create', 'adapter', 'format', 'shader compiler', 'call.OMSetBlendState', 'call.PSSetShaderResources',
                         'call.IASetVertexBuffers', 'call.Map constant buffer (DISCARD)', 'call.DrawIndexed (2 triangles)', 'call.ring Map NOOVERWRITE + Draw',
                         'frame.plaza-like as D3D8 issues it (1600+900 draws)', 'frame.plaza-like merged (33 draws)'], 'probe11 caps and bench: every result line'),
    ('probe8_x86.csv', ['d3d8.dll', 'device.create', 'call.SetRenderState', 'call.DrawPrimitiveUP (2 triangles)', 'call.VB Lock DISCARD + Unlock',
                        'frame.plaza-like as D3D8 issues it (1600+900 draws)', 'frame.plaza-like merged (33 draws)'], 'probe8 bench: every result line'),
    ('probe11_xproc_x86.csv', ['xproc.shared texture', 'xproc.child exit', 'xproc.child present into this window', 'xproc.shared texture written by the child'], 'xproc: the parent\'s lines'),
    ('probe11_xchild_x86.csv', ['xchild.swap chain on the parent\'s window', 'xchild.present'], 'xproc: the child ran on the parent\'s window and wrote its log into the run folder'),
]
for f, keys, what in checks:
    miss = has(f, keys); print('%s: %s%s' % ('ok' if not miss else 'FAIL', what, '' if not miss else ' (missing: %s)' % ', '.join(miss))); bad += bool(miss)
s = open(os.path.join(o, 'probe11_x86.csv')).read() if os.path.exists(os.path.join(o, 'probe11_x86.csv')) else ''
ok = 'device.create,hr 0x00000000' in s
print('%s: the D3D11 device was made (Wine on llvmpipe)' % ('ok' if ok else 'FAIL')); bad += not ok
rp = subprocess.run([sys.executable, os.path.join(H, 'phase0_report.py'), o], capture_output=True, text=True)
print(rp.stdout)
ok = rp.returncode == 0 and 'plaza-like merged (33 draws)' in rp.stdout and 'blend / ALPHABLENDENABLE' in rp.stdout
print('%s: phase0_report.py reads the run' % ('ok' if ok else 'FAIL')); bad += not ok
rs = subprocess.run([sys.executable, os.path.join(H, 'phase0_report.py'), '--selftest'], capture_output=True, text=True)
print(rs.stdout.strip()); bad += rs.returncode != 0
for f in ('probe11_arm64.exe',):
    b = open(os.path.join(D, f), 'rb').read(); pe = b.find(b'PE\0\0')
    ok = pe > 0 and b[pe + 4:pe + 6] == b'\x64\xaa'           # IMAGE_FILE_MACHINE_ARM64 0xAA64
    print('%s: %s is an ARM64 image' % ('ok' if ok else 'FAIL', f)); bad += not ok
if os.environ.get('KEEP'): print('kept', w)
else: shutil.rmtree(w, ignore_errors=True)
print('ALL OK' if not bad else 'FAILED'); sys.exit(1 if bad else 0)
