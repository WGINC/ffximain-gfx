"""effect_drain.py TRACE [FRAME] -- the effect drain in one label-0 frame of an xikit trace: each queued effect element
from YmElem_SetupDevice (0x100467E0) to YmElem_RestoreDevice's SetTransform (0x100473B4), and how many device calls of
the frame would reach the device under speedfix's settings, simulated on the trace's own arguments with the rules of
kit/speedfix/sf_device.cpp:
  raw      every call;
  default  state_cache and vertex_shader_cache; the cache forgotten at BeginScene, EndScene and ApplyStateBlock;
  ext      + extended_state_cache (Transform, Viewport, Light, LightEnable, Stream, Indices; never Material);
  blkext   ext, with ApplyStateBlock of a known block setting the shadow to the block's values instead of forgetting;
  lazy     ext + lazy_state (RS/TSS/Tex held until a method outside other_state());
  blk      lazy, with known blocks as in blkext;
  once     blk, with the restore block applied only at the end of a run of consecutive elements (a game-side change).
Known blocks: tokens 8 and 0xA, the drain's own. Their contents are the game's tables (StateBlock_ApplyTSS 0x1006AF90
called with 0x10353598 from YmElem_SetupDevice at 0x10047207; StateBlockHolder_ApplyRenderStates 0x1006AF00 called with
0x10353668 from YmElem_RestoreDevice at 0x10047371). Every other token is forgotten, as speedfix does. docs/HISTORY.md,
11 Oct, has the numbers and the evidence for the token-table link."""
import sys, json, bisect, collections
R = '/home/user/ffximain-gfx'
FR = sys.argv[2] if len(sys.argv) > 2 else '2'
ex = json.load(open(R + '/data/func_extents.json'))
iv = sorted((s, e, int(k, 16)) for k, v in ex.items() for s, e in v); st = [x[0] for x in iv]
def fn(a):
    i = bisect.bisect_right(st, a) - 1
    return iv[i][2] if i >= 0 and a < iv[i][1] else None
recs = [p for p in (l.split() for l in open(sys.argv[1], errors='replace')) if len(p) > 5 and p[0] == '0' and p[1] == FR]
LAZY = {'SetRenderState': 1, 'SetTextureStageState': 2, 'SetTexture': 1}
EXT = {'SetTransform': 1, 'SetViewport': 0, 'SetLight': 1, 'LightEnable': 1, 'SetStreamSource': 1, 'SetIndices': 0}
OTHER = {'SetVertexShader': 0, 'SetMaterial': 0, 'GetTransform': None, 'GetViewport': None, 'MultiplyTransform': None,
         'CreateVertexShader': None, 'SetVertexShaderConstant': None, 'SetPixelShader': None}   # other_state(): no flush
def kv(p):
    m = p[3]; n = int(p[5]); a = p[6:6 + n]; d = tuple(x for x in p[6 + n:] if x.startswith('d='))
    k = LAZY.get(m, EXT.get(m, OTHER.get(m)))
    if k is None: return None, None
    key = (m,) + tuple(a[:k]); val = d if m in ('SetTransform', 'SetLight', 'SetMaterial', 'SetViewport') and d else tuple(a[k:]) + d
    return key, val
BLK = {'00000008': [('SetTextureStageState', '%08X' % st, '%08X' % ty, v) for st in range(4) for ty, v in ((0x18, 0), (0xB, 0), (0x10, 2), (0x11, 2))],
       '0000000A': [('SetRenderState', '%08X' % k, v) for k, v in ((0x89,1),(0x1C,0),(0x30,0),(0x16,1),(0xAB,1),(0x13,2),(0x14,1),(0x1B,0),(0xF,1),(0x19,8),(0x18,0x60),(0x7,1),(0xE,1),(0x17,4),(0x1D,0),(0x8D,1))]}
def blk_items(tok):
    return [((b[0],) + b[1:-1], ('%08X' % b[-1],)) for b in BLK[tok]]
DROP = set()
# spans
SETUP, END = 0x100467E0, 0x100473B4
span = [None] * len(recs); cur = None; n = 0; starts = []
for i, p in enumerate(recs):
    a = int(p[4], 16)
    if cur is None and fn(a) == SETUP: cur = n; n += 1; starts.append(i)
    span[i] = cur
    if a == END: cur = None
print('frame', FR, 'records', len(recs), 'elements', n, 'calls in spans', sum(s is not None for s in span))
def sim(mode):
    dev = {}; pend = {}; sent = [False] * len(recs)
    def forget(): dev.clear()
    for i, p in enumerate(recs):
        m = p[3]; key, val = kv(p)
        if mode == 'raw': sent[i] = True; continue
        if i in DROP: continue
        if m == 'ApplyStateBlock' and mode in ('blk', 'once', 'blkext') and p[6] in BLK:
            for k, v in list(pend.items()):
                if dev.get(k) != v:
                    dev[k] = v; sent_extra[mode][i] += 1
                    if span[i] is not None: by_sent[mode][k[0]] += 1
            pend.clear(); sent[i] = True
            for k, v in blk_items(p[6]): dev[k] = v
            continue
        if m in ('BeginScene', 'EndScene', 'ApplyStateBlock', 'Reset'):
            # flush pending first (these read), then forget
            for k, v in list(pend.items()):
                if dev.get(k) != v:
                    dev[k] = v; sent_extra[mode][i] += 1
                    if span[i] is not None: by_sent[mode][k[0]] += 1
            pend.clear(); sent[i] = True; forget(); continue
        lazyable = (mode in ('lazy', 'blk', 'once') and m in LAZY) or (mode in ('floor',) and key is not None and m.startswith(('Set', 'Light')))
        if lazyable:
            if dev.get(key) == val: pend.pop(key, None)
            else: pend[key] = val
            continue
        cacheable = m in LAZY or m == 'SetVertexShader' or (mode != 'default' and m in EXT)
        if key is not None and m.startswith(('Set', 'Light')):
            if cacheable and dev.get(key) == val: continue
            sent[i] = True; dev[key] = val
            if m == 'SetRenderTarget': dev.pop(('SetViewport',), None)
            continue
        # a reading method: pending goes out, attributed to this record's position
        if mode == 'floor' and m == 'GetTransform':          # reads one transform only
            k = ('SetTransform', p[6]) ; v = pend.pop(k, None)
            if v is not None and dev.get(k) != v:
                dev[k] = v; sent_extra[mode][i] += 1
                if span[i] is not None: by_sent[mode][k[0]] += 1
            sent[i] = True; continue
        if mode not in ('ext', 'default', 'blkext') and (mode not in ('lazy', 'blk', 'once') or m not in OTHER):
            out = [k for k, v in pend.items() if dev.get(k) != v]
            for k in out:
                if span[i] is not None: by_sent[mode][k[0]] += 1
            for k in out: dev[k] = pend[k]
            sent_extra[mode][i] += len(out); pend.clear()
        sent[i] = True
        if m == 'SetRenderTarget': dev.pop(('SetViewport',), None)
    return sent
by_direct = collections.defaultdict(collections.Counter)
by_sent = collections.defaultdict(collections.Counter)
sent_extra = collections.defaultdict(collections.Counter)
res = {}
sims = {}
last_restore = {}
for i, p in enumerate(recs):
    if span[i] is not None and p[3] == 'ApplyStateBlock' and p[6] == '0000000A': last_restore[span[i]] = i
for mode in ('raw', 'default', 'ext', 'blkext', 'lazy', 'blk', 'once'):
    if mode == 'once':     # the restore block applied after each run of consecutive elements only (a game-side change)
        ends = set(); prev = None
        for e in sorted(last_restore):
            nxt = starts[e + 1] if e + 1 < len(starts) else None
            # the run continues when the next element starts with no device call outside the drain between
            # (OnDraw's SetRenderState 0xAB and the SetTransform it makes through 0x1000BD50 do not read state)
            gapf = {fn(int(recs[j][4], 16)) for j in range(last_restore[e] + 1, nxt) if span[j] is None} if nxt else None
            if nxt is None or not gapf <= {0x10046070, 0x1000BD50}: ends.add(last_restore[e])
        DROP.clear(); DROP.update(v for v in last_restore.values() if v not in ends)
        print('restore blocks kept at run ends:', len(ends), 'of', len(last_restore))
    s = sim(mode); sims[mode] = s
    tot = sum(s) + sum(sent_extra[mode].values())
    ins = sum(1 for i, x in enumerate(s) if x and span[i] is not None) + sum(v for i, v in sent_extra[mode].items() if span[i] is not None)
    res[mode] = (tot, ins)
    print('%-8s frame %6d   in the drain %6d' % (mode, tot, ins))
by = collections.Counter(recs[i][3] for i in range(len(recs)) if span[i] is not None)
print('drain calls by method:', by.most_common())
per = collections.Counter(span[i] for i in range(len(recs)) if span[i] is not None)
v = sorted(per.values()); print('per element min/median/max', v[0], v[len(v) // 2], v[-1], 'mean %.1f' % (sum(v) / len(v)))
byf = collections.Counter((hex(fn(int(recs[i][4], 16)) or 0)) for i in range(len(recs)) if span[i] is not None)
print('by containing function:', byf.most_common(12))
for mode in ('lazy', 'blk', 'once'):
    c = collections.Counter(by_sent[mode])
    for i in range(len(recs)):
        if span[i] is not None and sims[mode][i]: c[recs[i][3]] += 1
    print(mode, 'sent in the drain by method:', sorted(c.items(), key=lambda x: -x[1]))
gap = collections.Counter(); gaplen = collections.Counter()
for e in sorted(last_restore):
    if e + 1 >= len(starts): continue
    seg = recs[last_restore[e] + 2:starts[e + 1]]     # after restore's SetTransform
    gaplen[len(seg)] += 1
    gap[tuple((hex(fn(int(q[4], 16)) or 0), q[3]) for q in seg)] += 1
print('calls between elements (count of gaps by length):', sorted(gaplen.items()))
for g, c in gap.most_common(6): print(c, g[:8])
