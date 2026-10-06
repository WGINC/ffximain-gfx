"""vshader.py -- the vertex shaders the game created (xikit_vshader_<pid>_<handle>.bin, saved by d3dprof's
CreateVertexShader wrapper exactly as the game passed them), decoded: the declaration (which stream and vertex
register each input comes from, with its type) and the shader code (vs.1.x instructions).

    python3 vshader.py xikit_vshader_*.bin

Token formats from the DirectX 8 SDK (d3d8types.h): declaration tokens D3DVSD_* (type in bits 29-31), shader
instruction tokens D3DSIO_* with D3D8's fixed operand counts (vs.1.x tokens carry no length), parameter tokens
with the register type in bits 28-30, the write mask in 16-19 (destination), the swizzle in 16-23 and the
negate modifier in 24-27 (source). An opcode or register type outside that table is printed raw, not guessed."""
import struct, sys

OPS = {0: ('nop', 0), 1: ('mov', 2), 2: ('add', 3), 3: ('sub', 3), 4: ('mad', 4), 5: ('mul', 3), 6: ('rcp', 2), 7: ('rsq', 2),
       8: ('dp3', 3), 9: ('dp4', 3), 10: ('min', 3), 11: ('max', 3), 12: ('slt', 3), 13: ('sge', 3), 14: ('exp', 2), 15: ('log', 2),
       16: ('lit', 2), 17: ('dst', 3), 18: ('lrp', 4), 19: ('frc', 2), 20: ('m4x4', 3), 21: ('m4x3', 3), 22: ('m3x4', 3),
       23: ('m3x3', 3), 24: ('m3x2', 3), 78: ('expp', 2), 79: ('logp', 2)}
RASTOUT = {0: 'oPos', 1: 'oFog', 2: 'oPts'}
VTYPES = {0: 'FLOAT1', 1: 'FLOAT2', 2: 'FLOAT3', 3: 'FLOAT4', 4: 'D3DCOLOR', 5: 'UBYTE4', 6: 'SHORT2', 7: 'SHORT4'}


def reg(t):
    ty, n = (t >> 28) & 7, t & 0x7FF
    return {0: 'r%d' % n, 1: 'v%d' % n, 2: 'c%d' % n, 3: 'a%d' % n, 4: RASTOUT.get(n, 'rast%d' % n),
            5: 'oD%d' % n, 6: 'oT%d' % n}.get(ty, 'reg%d_%d' % (ty, n))


def dst(t):
    m = (t >> 16) & 0xF
    return reg(t) + ('' if m == 0xF else '.' + ''.join(c for i, c in enumerate('xyzw') if m >> i & 1))


def src(t):
    sw = (t >> 16) & 0xFF; mod = (t >> 24) & 0xF
    s = reg(t)
    if t & (1 << 13): s = s.replace('c', 'c[a0.x + ', 1) + ']'          # relative addressing (vs 1.1: a0.x)
    comp = ''.join('xyzw'[(sw >> (2 * i)) & 3] for i in range(4))
    if comp != 'xyzw': s += '.' + (comp[0] if len(set(comp)) == 1 else comp)
    return ('-' if mod == 1 else '' if mod == 0 else '(mod%d)' % mod) + s


def decl(tok):
    out = []; i = 0
    while i < len(tok):
        t = tok[i]; ty = t >> 29
        if t == 0xFFFFFFFF: out.append('end'); break
        if ty == 1: out.append('stream %d' % (t & 0xF) + (' (tessellator)' if t & (1 << 28) else ''))
        elif ty == 2 and t & (1 << 28): out.append('  skip %d dwords' % ((t >> 16) & 0xF))
        elif ty == 2: out.append('  v%d <- %s' % (t & 0x1F, VTYPES.get((t >> 16) & 0xF, 'type%d' % ((t >> 16) & 0xF))))
        elif ty == 4:
            n = (t >> 25) & 0xF; a = t & 0x7F
            out.append('  constants c%d..c%d set from the declaration' % (a, a + n - 1)); i += 4 * n
        elif ty == 0: out.append('  nop')
        else: out.append('  token %08X' % t)
        i += 1
    return out


def code(tok):
    out = []; i = 0
    if tok and tok[0] >> 16 == 0xFFFE: out.append('vs.%d.%d' % ((tok[0] >> 8) & 0xFF, tok[0] & 0xFF)); i = 1
    while i < len(tok):
        t = tok[i]; op = t & 0xFFFF
        if op == 0xFFFF: out.append('end'); break
        if op == 0xFFFE: n = (t >> 16) & 0x7FFF; out.append('; comment, %d dwords' % n); i += 1 + n; continue
        name, nops = OPS.get(op, (None, 0))
        if name is None: out.append('token %08X (opcode %d: not in the D3D8 vs.1.x table)' % (t, op)); i += 1; continue
        ops = tok[i + 1:i + 1 + nops]
        out.append('%s %s' % (name, ', '.join([dst(ops[0])] + [src(x) for x in ops[1:]]) if ops else '')); i += 1 + nops
    return out


def load(path):
    b = open(path, 'rb').read()
    magic, h, nd, nf = struct.unpack_from('<4I', b, 0)
    if magic != 0x48535658: raise ValueError('%s: not a saved vertex shader' % path)
    t = struct.unpack_from('<%dI' % (nd + nf), b, 16)
    return h, list(t[:nd]), list(t[nd:])


def main():
    for p in sys.argv[1:]:
        h, d, c = load(p)
        print('== handle %08X (%s): %d declaration tokens, %d code tokens' % (h, p, len(d), len(c)))
        for l in decl(d): print('  ' + l)
        for l in code(c): print('    ' + l)
        if not c: print('    (no code: a fixed-function vertex shader from the declaration)')


if __name__ == '__main__':
    main()
