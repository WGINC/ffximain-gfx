"""phase0_report.py -- the phase 0 results (run_phase0.bat's folder) side by side: what a call and a plaza-like frame
cost through D3D11 from the emulated x86 process and from the native ARM64 one, and through Direct3D 8 (the folder's
d3d8.dll: dgVoodoo on the owner's machine); whether two processes can share the output. docs/D3D11.md, sections 3-4.

    python3 phase0_report.py PHASE0_FOLDER [--selftest]

Only what the files hold is printed; a ratio is printed only where both of its numbers were measured."""
import os, re, sys

def load(path):
    d = {}
    if not os.path.exists(path): return d
    for l in open(path, encoding='utf-8', errors='replace'):
        k, _, v = l.rstrip('\n').partition(',')
        d.setdefault(k, []).append(v)
    return d

def num(v, pat):
    m = re.search(pat, v or '')
    return float(m.group(1)) if m else None

# the same work through each API: (label, probe11 key, probe8 key)
CALLS = [
    ('blend / ALPHABLENDENABLE', 'call.OMSetBlendState', 'call.SetRenderState'),
    ('texture bind', 'call.PSSetShaderResources', 'call.SetTexture'),
    ('vertex buffer bind', 'call.IASetVertexBuffers', 'call.SetStreamSource'),
    ('per-draw constants (D3D11: Map DISCARD; D3D8: SetTransform)', 'call.Map constant buffer (DISCARD)', 'call.SetTransform (world)'),
    ('small indexed draw', 'call.DrawIndexed (2 triangles)', 'call.DrawIndexedPrimitive (2 triangles)'),
    ('small dynamic draw (D3D11: ring; D3D8: DrawPrimitiveUP)', 'call.ring Map NOOVERWRITE + Draw', 'call.DrawPrimitiveUP (2 triangles)'),
]
FRAMES = ['frame.plaza-like as D3D8 issues it (1600+900 draws)', 'frame.plaza-like merged (33 draws)']

def report(folder):
    x86, arm, d8 = (load(os.path.join(folder, f)) for f in ('probe11_x86.csv', 'probe11_arm64.csv', 'probe8_x86.csv'))
    out = []
    for name, d in (('x86', x86), ('arm64', arm)):
        for k in ('process', 'device.create', 'adapter', 'shader compiler'):
            for v in d.get(k, []): out.append('%-6s %-16s %s' % (name, k, v))
    for v in d8.get('d3d8.dll', []) + d8.get('adapter', []): out.append('d3d8   %s' % v)
    out.append('')
    out.append('%-62s %12s %12s %12s %10s %12s' % ('ns a call', 'D3D11 x86', 'D3D11 arm64', 'D3D8 x86', 'x86/arm64', 'D3D8/D3D11'))
    for label, k11, k8 in CALLS:
        a = num((x86.get(k11) or [''])[-1], r'([\d.]+) ns'); b = num((arm.get(k11) or [''])[-1], r'([\d.]+) ns'); c = num((d8.get(k8) or [''])[-1], r'([\d.]+) ns')
        f = lambda v: '%12.1f' % v if v is not None else '%12s' % '-'
        r1 = '%10.2f' % (a / b) if a and b else '%10s' % '-'
        r2 = '%12.2f' % (c / a) if a and c else '%12s' % '-'
        out.append('%-62s %s %s %s %s %s' % (label, f(a), f(b), f(c), r1, r2))
    out.append('')
    out.append('%-55s %16s %16s %16s' % ('frame, ms, means (submit / with Present[+GPU])', 'D3D11 x86', 'D3D11 arm64', 'D3D8 x86'))
    for k in FRAMES:
        cells = []
        for d in (x86, arm, d8):
            v = (d.get(k) or [''])[-1]
            s, t = num(v, r'means ([\d.]+) /'), num(v, r'means [\d.]+ / ([\d.]+)')   # means: a median can sit on a step of the D3D8 layer's waits
            if s is None: s, t = num(v, r'submit ([\d.]+)'), num(v, r'with Present[^\d]*([\d.]+)')
            cells.append('%16s' % ('%.2f / %.2f' % (s, t) if s is not None and t is not None else '-'))
        out.append('%-55s %s' % (k.replace('frame.', ''), ''.join(cells)))
    out.append('')
    for arch in ('x86', 'arm64'):
        xp = load(os.path.join(folder, 'probe11_xproc_%s.csv' % arch)); ch = load(os.path.join(folder, 'probe11_xchild_%s.csv' % ('arm64' if arch == 'x86' else 'x86')))
        for k in ('xproc.child present into this window', 'xproc.shared texture written by the child', 'xproc.child exit'):
            for v in xp.get(k, []): out.append('parent %-5s %-45s %s' % (arch, k.replace('xproc.', ''), v))
        for k in ('xchild.swap chain on the parent\'s window', 'xchild.present', 'xchild.open shared texture'):
            for v in ch.get(k, []): out.append('  child %-44s %s' % (k.replace('xchild.', ''), v))
    return '\n'.join(out)

def selftest():
    import tempfile
    d = tempfile.mkdtemp()
    open(os.path.join(d, 'probe11_x86.csv'), 'w').write('process,build x86\ncall.OMSetBlendState,400.0 ns a call (median)\nframe.plaza-like merged (33 draws),submit 0.11 ms (p95 0.15), with Present and the GPU 32.44 ms (p95 40.64); median\n')
    open(os.path.join(d, 'probe11_arm64.csv'), 'w').write('process,build arm64\ncall.OMSetBlendState,100.0 ns a call (median)\n')
    open(os.path.join(d, 'probe8_x86.csv'), 'w').write('call.SetRenderState,800.0 ns a call (median)\nframe.plaza-like merged (33 draws),submit 0.26 ms (p95 0.26), with Present 37.63 ms (p95 45.06); median\n')
    r = report(d)
    ok = re.search(r'blend / ALPHABLENDENABLE\s+400\.0\s+100\.0\s+800\.0\s+4\.00\s+2\.00', r) is not None and '0.11 / 32.44' in r and '0.26 / 37.63' in r \
        and re.search(r'texture bind\s+-\s+-\s+-\s+-\s+-', r) is not None
    print('%s: selftest (ratios only where both numbers exist; frames read with and without the GPU wait)' % ('ok' if ok else 'FAIL'))
    return ok

if __name__ == '__main__':
    if '--selftest' in sys.argv: sys.exit(0 if selftest() else 1)
    if len(sys.argv) < 2: print(__doc__); sys.exit(2)
    print(report(sys.argv[1]))
