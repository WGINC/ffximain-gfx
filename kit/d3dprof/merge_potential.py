"""merge_potential.py -- how many of a frame's draws could be one draw: a trace (xikit trace=1) read for runs of
consecutive draws that run with the same device state and nothing changed between them.

    python3 merge_potential.py xikit_trace_<pid>_<n>.txt [--label 0] [--same-value] [--selftest]

A run is a sequence of draws of one method (DrawIndexedPrimitive, DrawPrimitive, DrawPrimitiveUP, ...) where each
draw after the first:
  * has the same state hash (` s=`: render states, texture-stage states, textures, vertex shader, index buffer,
    vertex streams, as d3dprof's shadow knows them) as the one before; a draw without a hash (state not known) starts
    a new run;
  * follows it with no call between them that changes what a draw sees but is not in the hash: SetTransform,
    MultiplyTransform, SetMaterial, SetLight, LightEnable, SetViewport, SetClipPlane, SetVertexShaderConstant,
    SetPixelShaderConstant, SetIndices with another base vertex index, a vertex or index buffer refilled (VB_Unlock,
    IB_Unlock), Clear, SetRenderTarget, a state block, BeginScene, EndScene, Present;
  * for DrawPrimitiveUP: has the same vertex stride.
Such draws read the same buffers under the same state, so one draw of the union of their primitives (strips and fans
turned into lists, which is what speedfix's batching and a merged index list do) draws what they drew. That is an
upper bound for merging in place: it does not say the merged order of triangles is the same where blending makes
order matter (within one run the order is kept), and it counts nothing across a state change.

--same-value: a SetTransform, SetLight, SetMaterial, LightEnable or SetViewport that sets the value its slot already
holds (the trace's payload, ` d=`, or its arguments, compared with the last set of that slot in the frame; a slot not
yet set in the frame breaks) does not break a run. The vertex streams stay in the hash, so draws of different
buffers still break: this is merging within one model's buffers, with no shared skinning buffer.

Reported per frame and label (0 = without swaps and mods, 1 = with): the draws of each method, the runs they form
(the draws merging would leave), the draws per run, and the primitives."""
import argparse, collections, sys

DRAWS = {'DrawPrimitive', 'DrawIndexedPrimitive', 'DrawPrimitiveUP', 'DrawIndexedPrimitiveUP'}
BREAK = {'SetTransform', 'MultiplyTransform', 'SetMaterial', 'SetLight', 'LightEnable', 'SetViewport', 'SetClipPlane',
         'SetVertexShaderConstant', 'SetPixelShaderConstant', 'VB_Unlock', 'IB_Unlock', 'Clear', 'SetRenderTarget',
         'ApplyStateBlock', 'CaptureStateBlock', 'BeginStateBlock', 'EndStateBlock', 'BeginScene', 'EndScene', 'Present',
         'Reset', 'CopyRects', 'UpdateTexture'}
PRIMS = {'DrawPrimitive': 2, 'DrawIndexedPrimitive': 4, 'DrawPrimitiveUP': 1, 'DrawIndexedPrimitiveUP': 3}   # the count's arg index


SAMEABLE = {'SetTransform', 'SetLight', 'SetMaterial', 'LightEnable', 'SetViewport'}
SLOTTED = {'SetTransform', 'SetLight', 'LightEnable'}     # the first argument names the slot (state, light index)


def records(lines, with_data=False):
    for l in lines:
        if l.startswith('#'): continue
        raw = l.split(); st = next((x[2:] for x in raw if x.startswith('s=')), None)
        d = next((x[2:] for x in raw if x.startswith('d=')), None)
        p = [x for x in raw if not x.startswith(('d=', 's='))]
        if len(p) < 7: continue
        np_ = int(p[5]); args = [int(x, 16) for x in p[6:6 + min(np_, 6)]]
        if with_data: yield int(p[0]), int(p[1]), p[3], int(p[4], 16), args, st, d
        else: yield int(p[0]), int(p[1]), p[3], int(p[4], 16), args, st


def set_value(name, args, d):
    """the slot a set writes and the value it writes; the value is None when the trace does not hold it"""
    slot = (name, args[0] if name in SLOTTED and args else None)
    if name == 'LightEnable': return slot, ('on', 1 if len(args) > 1 and args[1] else 0)
    if d is not None: return slot, ('d', d)
    return slot, None


def analyse(lines, label=None, same_value=False):
    fr = collections.defaultdict(lambda: {'label': None, 'n': collections.Counter(), 'runs': collections.Counter(),
                                           'prims': collections.Counter(), 'sizes': collections.defaultdict(list)})
    prev = {}                                  # frame -> (method, state, stride) of the last draw, None after a break
    held = collections.defaultdict(dict)       # frame -> slot -> the value last set (--same-value)
    base = {}; run_len = {}
    def close(f):                              # the run open in frame f (if any) ends here
        if prev.get(f) is not None and run_len.get(f): fr[f]['sizes'][prev[f][0]].append(run_len[f])
        prev[f] = None; run_len[f] = 0
    for lab, f, name, site, args, st, d in records(lines, True):
        if label is not None and lab != label: continue
        if same_value and name in SAMEABLE:
            slot, val = set_value(name, args, d)
            same = val is not None and held[f].get(slot) == val
            held[f][slot] = val
            if same: continue                  # the slot keeps its value: nothing a draw sees changed
            close(f); continue
        F = fr[f]; F['label'] = lab
        if name == 'SetIndices':
            b = args[1] if len(args) > 1 else 0
            if base.get(f) is not None and base[f] != b: close(f)
            base[f] = b; continue
        if name in BREAK: close(f); continue
        if name not in DRAWS: continue
        key = (name, st, args[3] if name == 'DrawPrimitiveUP' and len(args) > 3 else None)
        F['n'][name] += 1
        F['prims'][name] += args[PRIMS[name]] if len(args) > PRIMS[name] else 0
        if st is not None and prev.get(f) == key:
            run_len[f] += 1
        else:
            close(f); F['runs'][name] += 1; run_len[f] = 1
        prev[f] = key
    for f in list(fr): close(f)
    return fr


def report(fr):
    tot = collections.defaultdict(lambda: [0, 0, 0, 0])
    for f in sorted(fr):
        F = fr[f]
        for m in sorted(F['n']):
            t = tot[(F['label'], m)]; t[0] += F['n'][m]; t[1] += F['runs'][m]; t[2] += F['prims'][m]; t[3] += 1
    frames = collections.Counter(F['label'] for F in fr.values())
    print('per frame, averaged over the frames of each label (label 0: no swaps or mods; 1: with them):')
    for (lab, m), (n, r, p, _) in sorted(tot.items()):
        k = frames[lab]
        print('  label %s %-24s draws %8.1f  -> runs %8.1f  (%.1f draws a run; %.0f primitives)' % (lab, m, n / k, r / k, n / r if r else 0, p / k))
    return tot


def selftest():
    t = '''# d3d trace v1: selftest
0 0 34 BeginScene 0x10009736 0 00000000
0 0 85 SetIndices 0x1002DD60 2 00001000 00000000 00000000
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000000 00000004 00000000 s=AAAA0001
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000006 00000004 00000000 s=AAAA0001
0 0 61 SetTexture 0x1002E66C 2 00000000 00002000 00000000
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 0000000C 00000004 00000000 s=AAAA0002
0 0 37 SetTransform 0x1000BD9C 2 00000100 00000000 12345678
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000012 00000004 00000000 s=AAAA0002
0 0 72 DrawPrimitiveUP 0x1000CDE2 4 00000005 00000002 00000000 0000001C 11111111 s=BBBB0001
0 0 72 DrawPrimitiveUP 0x1000CDE2 4 00000005 00000002 00000000 0000001C 22222222 s=BBBB0001
0 0 72 DrawPrimitiveUP 0x1000CDE2 4 00000005 00000002 00000000 00000020 33333333 s=BBBB0001
0 0 35 EndScene 0x1000974C 0 00000000
'''.splitlines()
    fr = analyse(t, 0); F = fr[0]
    ok = F['n']['DrawIndexedPrimitive'] == 4 and F['runs']['DrawIndexedPrimitive'] == 3 and F['sizes']['DrawIndexedPrimitive'] == [2, 1, 1] \
        and F['n']['DrawPrimitiveUP'] == 3 and F['runs']['DrawPrimitiveUP'] == 2 and F['prims']['DrawIndexedPrimitive'] == 16
    print('%s: selftest (a texture change and a transform break runs; a stride change breaks UP runs)' % ('ok' if ok else 'FAIL'))
    # --same-value: a light set again to its value does not break; a new value does; a light's first set does
    t2 = '''# d3d trace v1: selftest 2
0 0 34 BeginScene 0x10009736 0 00000000
0 0 44 SetLight 0x1000C01B 2 00000000 00000000 11111111 d=0300AA
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000000 00000004 00000000 s=AAAA0001
0 0 44 SetLight 0x1000C01B 2 00000000 00000000 11111111 d=0300AA
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000006 00000004 00000000 s=AAAA0001
0 0 44 SetLight 0x1000C01B 2 00000000 00000000 22222222 d=0300BB
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 0000000C 00000004 00000000 s=AAAA0001
0 0 46 LightEnable 0x1000C0A0 2 00000001 00000001
0 0 71 DrawIndexedPrimitive 0x1000CCF1 5 00000005 00000000 00000010 00000012 00000004 00000000 s=AAAA0001
0 0 35 EndScene 0x1000974C 0 00000000
'''.splitlines()
    a = analyse(t2, 0)[0]['runs']['DrawIndexedPrimitive']; b = analyse(t2, 0, True)[0]['runs']['DrawIndexedPrimitive']
    ok2 = a == 4 and b == 3
    print('%s: selftest --same-value (4 runs without it; with it the repeated light joins, the new light and the first enable break: 3)' % ('ok' if ok2 else 'FAIL'))
    return ok and ok2


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('trace', nargs='?'); ap.add_argument('--label', type=int); ap.add_argument('--selftest', action='store_true')
    ap.add_argument('--same-value', action='store_true')
    o = ap.parse_args()
    if o.selftest: sys.exit(0 if selftest() else 1)
    if not o.trace: ap.error('a trace file, or --selftest')
    report(analyse(open(o.trace), o.label, o.same_value))


if __name__ == '__main__':
    main()
