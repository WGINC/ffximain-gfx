"""random_cases.py -- cases for t8 random: each a draw with Direct3D 8 state picked at random, value by value, from
the values the game was seen setting (data/d3d8_scope.json, traces: render_states, texture_stage_states, fvf_codes,
primitive_types). Each state is left at the case's starting state (t8.c reset_state, then depth on) with probability
1/3, else set to one of the game's values. The combinations are random: a case may combine values the game never
sets together. Additions not from the traces, each said here:
  - FOGEND also from 6.0 and 12.0 and FOGSTART from 1.0, as the test scene's depths are 1 to 10 and the game's ends
    (about 3,240) would leave everything unfogged;
  - texture transforms (for TEXTURETRANSFORMFLAGS 2 and 3), random 2D affine matrices;
  - a texture for each stage whose op reads one: the three 2D test textures, or a cube texture when the stage's
    TEXCOORDINDEX generates coordinates (the game's 0x10001 and 0x30001);
  - lights 0 to 2 (directional, point, spot) each enabled at random, and a random material.

    python3 random_cases.py SEED COUNT > cases.txt"""
import json, os, random, struct, sys
H = os.path.dirname(os.path.abspath(__file__))
scope = json.load(open(os.path.join(H, '..', '..', '..', 'data', 'd3d8_scope.json')))['traces']
f2h = lambda x: '%08X' % struct.unpack('<I', struct.pack('<f', x))[0]
# the scope lists at most 24 values a state and ends a longer list with a "..." key ("N more values"): not a value
vals = lambda v: [x for x in v if x != '...']
RS = {int(k.split()[0]): vals(v) for k, v in scope['render_states'].items()}
RS[37] = RS[37] + [f2h(6.0), f2h(12.0)]
RS[36] = RS[36] + [f2h(1.0)]
TSS = {}
for k, v in scope['texture_stage_states'].items():
    st, t = k.split()[0].split('.'); TSS[(int(st), int(t))] = vals(v)
FVF = list(scope['fvf_codes'])
PRIM = {'TRIANGLESTRIP': 5, 'TRIANGLELIST': 4}
PRIMS = [PRIM[p] for p in scope['primitive_types']]
READS_TEX = lambda a: (a & 0xF) == 2                                    # D3DTA_TEXTURE


def case(r, n):
    fvf = r.choice(FVF); out = ['case r%04d %s %d %d' % (n, fvf, r.choice(PRIMS), r.randrange(3))]
    for s, vals in sorted(RS.items()):
        if r.random() < 2 / 3: out.append('rs %d %s' % (s, r.choice(vals)))
    tss = {}
    for (st, t), vals in sorted(TSS.items()):
        if r.random() < 2 / 3: tss[(st, t)] = int(r.choice(vals), 16); out.append('tss %d %d %X' % (st, t, tss[(st, t)]))
    for st in range(4):
        arg = [tss.get((st, t), 2 if t in (2, 5) else 1) for t in (2, 3, 5, 6)]
        if any(READS_TEX(a) for a in arg) or r.random() < 0.3:
            gen = tss.get((st, 11), st) >> 16
            out.append('tex %d %d' % (st, 4 if gen else r.choice((1, 2, 3))))
        if tss.get((st, 24), 0):
            out.append('tt %d %s' % (st, ' '.join('%.3f' % r.uniform(-1.5, 1.5) for _ in range(6))))
    for i in range(3): out.append('light %d %d' % (i, r.random() < 0.5))
    out.append('mat %s' % ' '.join('%.3f' % r.random() for _ in range(4)))
    out.append('draw')
    return out


# the classes of test_d3d8on11.py (its comment above the random cases says what each is) of a case's lines
def rclass(lines, fix=None):
    fvf, tss, tex, cube = int(lines[0].split()[2], 16), {}, set(), set()
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
        if p[0] == 'tex' and p[2] != '0': tex.add(int(p[1]))
        if p[0] == 'tex': (cube.add if p[2] in ('4', '5', '6') else cube.discard)(int(p[1]))
    rhw, nsets, cls = (fvf & 0xE) == 4, (fvf >> 8) & 15, set()
    for st in range(4):
        if tss.get((st, 1), 4 if st == 0 else 1) == 1: break               # D3DTOP_DISABLE ends the stages
        if st not in tex: continue
        tci = tss.get((st, 11), st)
        # the change that takes the class away must not make another undefined case: a cube stage reading a 2D set
        # (what a missing third component reads as is not in the reference) gets a 2D texture
        two_d = lambda: ['tex %d 1' % st] if st in cube else []
        # with no set to read, the stage is turned off (COLOROP DISABLE: "Disables output from this texture stage and all
        # stages with a higher index", D3DTEXTUREOP), not left without a texture: a stage whose args read an unset
        # texture is drawn by the DirectX 8.1 rule (d8_ff.cpp, Texture Blending) by the reference device and ours, by
        # another by Wine 9.0 (is_invalid_op; measured, 14 Oct)
        off = lambda: ['tss %d 1 1' % st]
        if rhw and tci >> 16:
            cls.add('A')
            if fix is not None: fix.extend(['tss %d 11 %X' % (st, tci & 7)] + two_d())
        if (rhw or not tci >> 16) and (tci & 7) >= nsets:
            cls.add('C')
            if fix is not None: fix.extend(['tss %d 11 0' % st] + two_d() if nsets else off())
        # N: coordinates from the vertex normal (CAMERASPACENORMAL, CAMERASPACEREFLECTIONVECTOR) of a vertex format
        # with no normal. The DirectX 8.1 text says only "Use the vertex normal" (D3DTEXTURESTAGESTATETYPE); Microsoft's
        # reference device and ours differ there (seed 1 r0003, 14 Oct). The game generates only through its vertex
        # shader declarations, and each of the 2,260 the kit captured (6 to 13 Oct) has a normal (register 3)
        if not rhw and not fvf & 0x10 and tci >> 16 in (1, 3):
            cls.add('N')
            if fix is not None: fix.extend(['tss %d 11 0' % st] + two_d() if nsets else off())
    return cls
def alpha_disabled(lines, fix=None):
    """a stage whose colour op is on and whose alpha op is DISABLE (class D): "Setting the alpha operation to
    D3DTOP_DISABLE when color blending is enabled causes undefined behavior" (D3DTEXTUREOP, DirectX 8.1). reset_state
    leaves stages 1 to 3 so (the D3D8 defaults), so a case that turns a stage's colour on without its alpha op is one.
    Microsoft's reference device makes alpha 1 there, ours and Wine pass the previous stage's alpha on (scenes
    alphaop_disabled_stage0/1); the game does it (0x1000CBF3). The change: the stage's alpha op SELECTARG1 of CURRENT,
    the pass-through the DirectX 8.1 text defines"""
    tss = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
    found = False
    for st in range(4):
        if tss.get((st, 1), 4 if st == 0 else 1) == 1: break
        if tss.get((st, 4), 2 if st == 0 else 1) == 1:
            found = True
            if fix is not None: fix.extend(['tss %d 4 2' % st, 'tss %d 5 1' % st])
    return found
def linear_stages(lines):
    """the stages with a texture filtered other than by point (class P). The DirectX 8.1 text gives no precision for
    linear filtering ("Linear Texture Filtering"); Microsoft's reference device departs from the exact weighted average
    of the documented texel mapping ("Directly Mapping Texels to Pixels") by up to about 10 per channel, ours by 1
    (exact_filter.py, 14 Oct). Counted only against a device of other filtering hardware (the reference device), and
    only if the case with every stage point-filtered then draws the same"""
    tss, tex = {}, set()
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
        if p[0] == 'tex': (tex.add if p[2] != '0' else tex.discard)(int(p[1]))
    return [st for st in sorted(tex) if tss.get((st, 16), 1) >= 2 or tss.get((st, 17), 1) >= 2 or tss.get((st, 18), 0) >= 1]
def classes(lines, ref=False):
    """the class letters of a case (test_d3d8on11.py says what each is); P only with ref (against the reference device)"""
    return ''.join(sorted(rclass(lines))) + ('D' if alpha_disabled(lines) else '') + ('S' if cube_linear_stages(lines) else '') + \
        ('L' if lit_unnormalized(lines) else '') + ('F' if fog_reversed(lines) else '') + ('W' if fog_rhw_table(lines) else '') + \
        ('P' if ref and linear_stages(lines) else '') + ('T' if not ref and alpha_test_wine(lines) else '')
def changes(lines, ref=False):
    """the lines that take a case out of its classes (each class's change, said where the class is defined)"""
    fx = []; rclass(lines, fx); alpha_disabled(lines, fx); fog_reversed(lines, fx); fog_rhw_table(lines, fx)
    pt = linear_stages(lines) if ref else cube_linear_stages(lines)
    return fx + ['tss %d 16 1\ntss %d 17 1\ntss %d 18 0' % (k, k, k) for k in pt] + (['rs 143 1'] if lit_unnormalized(lines) else []) + \
        (['rs 15 0'] if not ref and alpha_test_wine(lines) else [])
def alpha_test_wine(lines):
    """the alpha test on, with a function other than NEVER or ALWAYS (class T). The DirectX 8.1 text does not say how
    the pixel's alpha is made 8-bit for the comparison with ALPHAREF; Microsoft's reference device compares
    floor(alpha x 255) (measured, 14 Oct), and ours does the same; Wine 9.0 compares the float alpha with
    ALPHAREF / 255 (wined3d_private.h:3117, glsl_shader.c:7529), so the two differ where alpha x 255 falls between
    two integers. Counted only against Wine. The change: the alpha test off"""
    rs = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'rs': rs[int(p[1])] = int(p[2], 16)
    return bool(rs.get(15, 0)) and rs.get(25, 8) not in (1, 8)
def lit_unnormalized(lines):
    rs = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'rs': rs[int(p[1])] = int(p[2], 16)
    return (int(lines[0].split()[2], 16) & 0x10) and rs.get(137, 1) and not rs.get(143, 0)
def cube_linear_stages(lines):
    tss, cube = {}, set()
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'tss': tss[(int(p[1]), int(p[2]))] = int(p[3], 16)
        if p[0] == 'tex' and p[2] in ('4', '5', '6'): cube.add(int(p[1]))
    return [st for st in cube if tss.get((st, 16), 1) >= 2 or tss.get((st, 17), 1) >= 2]

def fog_reversed(lines, fix=None):
    """linear fog in use with FOGSTART >= FOGEND (class F). The DirectX 8.1 text defines start as where fog "begins" and
    end as where it "no longer increases" (Fog Formulas), which contradict each other when start >= end. Microsoft's
    reference device then fogs fully from end on and not before (measured, 14 Oct); Wine 9.0 computes the formula;
    the game never sets start >= end (traces 5180, 6080, 6796). The change: FOGEND moved to FOGSTART + 4"""
    rs = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'rs': rs[int(p[1])] = int(p[2], 16)
    fl = lambda v: struct.unpack('<f', struct.pack('<I', v))[0]
    rhw = (int(lines[0].split()[2], 16) & 0xE) == 4
    t, v = rs.get(35, 0), rs.get(140, 0)                                 # reset_state sets both NONE
    linear = rs.get(28, 0) and (t == 3 or (t == 0 and v == 3 and not rhw))
    s, e = fl(rs.get(36, 0)), fl(rs.get(37, 0x3F800000))                   # the D3D8 defaults: start 0.0, end 1.0
    if linear and s >= e:
        if fix is not None: fix.append('rs 37 %s' % f2h(s + 4.0))
        return True
    return False

def fog_rhw_table(lines, fix=None):
    """table fog on pre-transformed vertices (class W): Wine 9.0 fogs by the vertex z (glsl_shader.c, transformed:
    ffp_varying_fogcoord = ec_pos.z), Microsoft's runtime and ours by 1/RHW ("Pixel Fog", DirectX 8.1). The change:
    fog off"""
    rs = {}
    for l in lines[1:]:
        p = l.split()
        if p[0] == 'rs': rs[int(p[1])] = int(p[2], 16)
    if (int(lines[0].split()[2], 16) & 0xE) == 4 and rs.get(28, 0) and rs.get(35, 0):
        if fix is not None: fix.append('rs 28 0')
        return True
    return False


if __name__ == '__main__':
    seed, count = int(sys.argv[1]), int(sys.argv[2]); r = random.Random(seed)
    for n in range(count): print('\n'.join(case(r, n)))
