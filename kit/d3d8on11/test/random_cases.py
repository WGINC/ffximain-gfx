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
RS = {int(k.split()[0]): list(v) for k, v in scope['render_states'].items()}
RS[37] = RS[37] + [f2h(6.0), f2h(12.0)]
RS[36] = RS[36] + [f2h(1.0)]
TSS = {}
for k, v in scope['texture_stage_states'].items():
    st, t = k.split()[0].split('.'); TSS[(int(st), int(t))] = list(v)
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


if __name__ == '__main__':
    seed, count = int(sys.argv[1]), int(sys.argv[2]); r = random.Random(seed)
    for n in range(count): print('\n'.join(case(r, n)))
