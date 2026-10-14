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
        if rhw and tci >> 16:
            cls.add('A')
            if fix is not None: fix.extend(['tss %d 11 %X' % (st, tci & 7)] + two_d())
        if (rhw or not tci >> 16) and (tci & 7) >= nsets:
            cls.add('C')
            if fix is not None: fix.extend(['tss %d 11 0' % st] + two_d() if nsets else ['tex %d 0' % st])
    return cls
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
