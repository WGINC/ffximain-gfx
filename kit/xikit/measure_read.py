"""measure_read.py -- reads one start's measurement files (xikit's frames=, imports=, callsites=, sample=3, sysstats=,
env=; docs/KIT.md, Measuring) and writes report.md and summary.json into that folder.

    python3 measure_read.py LOG_FOLDER [--presentmon PM.csv] [--measure xikit_measure.txt]

What each number is, and where it comes from:
  * frame times: the kit's own QPC times at Present (xikit_frames_<pid>.csv);
  * the game's pacing fields: read raw each frame, decoded as docs/RENDERER.md 3 says the code uses each
    (this+0x28, +0x2C, the ring and [timer+4] are floats; +0x30 and +0x38 are integers; +0x3C a float);
  * call timing: the wrapped calls' own TSC times (exact for the calls wrapped; each wrapped call adds the cost the
    kit measured at the start, xikit.log);
  * sample=3: where each thread was when it was looked at -- a share of time, an estimate. The render thread's samples
    are charged to the call in Frame_Render (0x10011A00, data/func_extents.json) whose return address is the outermost
    one on the stack inside Frame_Render, and to the next return address inward (the level below). A return address
    left in a dead stack slot can be counted: these are attributions, not measurements.
Game addresses are given in the analysed build's numbering (the kit's rebasing and build translation undone); the
names are labels (pipeline/disasm.py's), never evidence."""
import bisect, collections, glob, json, math, os, re, struct, sys

H = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(H, '..', '..')
FRAME_RENDER = 0x10011A00


def load_names():
    try:
        sys.path.insert(0, os.path.join(ROOT, 'pipeline')); from disasm import names
        return names()
    except Exception:
        return {}


class Funcs:
    """the analysed build's functions (data/func_extents.json): which function an address is in"""
    def __init__(self):
        ext = json.load(open(os.path.join(ROOT, 'data', 'func_extents.json')))
        iv = sorted((s, e, int(k, 16)) for k, ch in ext.items() for s, e in ch)
        self.lo = [x[0] for x in iv]; self.iv = iv; self.ext = {int(k, 16): ch for k, ch in ext.items()}

    def of(self, a):
        i = bisect.bisect_right(self.lo, a) - 1
        if i >= 0 and self.iv[i][0] <= a < self.iv[i][1]: return self.iv[i][2]
        return None

    def inside(self, f, a):
        return any(s <= a < e for s, e in self.ext.get(f, []))


def f32(h):
    try: return struct.unpack('<f', struct.pack('<I', int(h, 16)))[0]
    except Exception: return None


def i32(h):
    try: v = int(h, 16); return v - (1 << 32) if v >= 1 << 31 else v
    except Exception: return None


def pct(xs, p):
    if not xs: return float('nan')
    s = sorted(xs); k = min(len(s) - 1, max(0, int(round(p / 100.0 * (len(s) - 1))))); return s[k]


def stats(xs):
    xs = [x for x in xs if x is not None and not (isinstance(x, float) and math.isnan(x))]
    if not xs: return {}
    return {'n': len(xs), 'mean': sum(xs) / len(xs), 'p50': pct(xs, 50), 'p90': pct(xs, 90), 'p95': pct(xs, 95), 'p99': pct(xs, 99), 'max': max(xs)}


def read_csv(path):
    rows, head, comments = [], None, []
    for line in open(path, errors='replace'):
        line = line.rstrip('\n')
        if line.startswith('#'): comments.append(line); continue
        if head is None: head = line.split(','); continue
        p = line.split(',')
        if len(p) >= len(head): rows.append(dict(zip(head, p)))
    return head, rows, comments


class Raw:
    """xikit_raw_<pid>.bin (measure.c, sample=3)"""
    def __init__(self, path):
        b = open(path, 'rb').read()
        if b[:6] != b'XIRAW1': raise ValueError('not an xikit raw file')
        (self.ver, self.pid, self.rel, self.glo, self.ghi, self.rlo, self.rhi, self.mland) = struct.unpack_from('<8I', b, 8)
        self.qpf = struct.unpack_from('<q', b, 40)[0]
        sh = struct.unpack_from('<20i', b, 48); self.shift = [(sh[1 + 3 * i] & 0xFFFFFFFF, sh[2 + 3 * i] & 0xFFFFFFFF, sh[3 + 3 * i]) for i in range(min(sh[0], 6))]
        self.samples = []; self.repeats = collections.Counter(); self.threads = {}; self.mods = {}
        o = 128; n = len(b)
        while o + 4 <= n:
            t = b[o:o + 1]
            if t == b'S':
                if o + 24 > n: break
                fl, nra, tid, fr, q, eip = struct.unpack_from('<BHIIqI', b, o + 1)
                if o + 24 + 4 * nra > n: break
                ras = struct.unpack_from('<%dI' % nra, b, o + 24); o += 24 + 4 * nra
                self.samples.append((tid, fr, q, eip, ras, fl))
            elif t == b'R':
                if o + 20 > n: break
                tid, fr, q = struct.unpack_from('<IIq', b, o + 4); o += 20
                self.repeats[tid] += 1; self.samples.append((tid, fr, q, None, None, 0))
            elif t == b'T':
                if o + 20 > n: break
                tid, start, q = struct.unpack_from('<IIq', b, o + 4); o += 20; self.threads[tid] = start
            elif t == b'M':
                if o + 12 > n: break
                ln, base, size = struct.unpack_from('<HII', b, o + 2)
                name = b[o + 12:o + 12 + ln].decode('latin-1'); o += 12 + ln; self.mods[base] = (size, name)
            else: break
        self.mbase = sorted(self.mods)

    def analysed(self, a):
        ea = (a - self.rel) & 0xFFFFFFFF
        for lo, hi, d in self.shift:
            if lo + d <= ea < hi + d: return ea - d
        return ea

    def where(self, a):
        """('game', analysed) | ('recon', offset) | ('module', name, offset) | ('memory', a)"""
        if self.glo <= a < self.ghi: return ('game', self.analysed(a))
        if self.rlo <= a < self.rhi: return ('recon', a - self.rlo)
        i = bisect.bisect_right(self.mbase, a) - 1
        if i >= 0:
            base = self.mbase[i]; size, name = self.mods[base]
            if a < base + size: return ('module', os.path.basename(name.replace('\\', '/')), a - base)
        return ('memory', a)


def main(argv):
    d = argv[0]; pm_path = None; mtab = None
    if '--presentmon' in argv: pm_path = argv[argv.index('--presentmon') + 1]
    if '--measure' in argv: mtab = argv[argv.index('--measure') + 1]
    N = load_names(); F = Funcs()
    def fname(ea):
        f = F.of(ea)
        if f is None: return '0x%08X' % ea
        return '%s+0x%X' % (N.get(f, 'sub_%X' % f), ea - f)
    out, summ = [], {}
    def w(s=''): out.append(s)
    w('# Measurement report: %s' % os.path.basename(os.path.abspath(d)))
    w(); w('Generated by kit/xikit/measure_read.py. Sources are named in each section; samples are estimates, wrapped-call times are measured.')
    # ---------------------------------------------------------------- the kit's log and environment
    logp = os.path.join(d, 'xikit.log'); log = open(logp, errors='replace').read() if os.path.exists(logp) else ''
    for pat in (r'kit build: .*', r'machine: .*', r'cpu: .*', r'graphics module: .*', r'measure: a wrapped call costs .*', r'measure: \d+ import slots .*',
                r'measure: \d+ of \d+ wrapped calls in place', r'sample=3: QueryThreadCycleTime .*', r'speedfix.*', r'measure: call site .* put back'):
        for m in re.findall(pat, log)[:12]: w('- `%s`' % m.strip()[:300])
    envs = glob.glob(os.path.join(d, 'xikit_env_*.txt'))
    if envs:
        env = open(envs[0], errors='replace').read()
        w(); w('## Environment (xikit_env)'); w(); w('```')
        keep = [l for l in env.splitlines() if l.startswith(('[wine]', '[machine]', '[display]', '[timers', '  timer', '  Sleep(1)', '  timeGetTime', '  GetTickCount', '  process ', '  memory ', '  adapter'))
                or re.match(r'  \S.* ns a call', l) or ('= dword' in l and 'FinalFantasyXI' in env)]
        w('\n'.join(keep[:200])); w('```')
        sl = re.findall(r'Sleep\(1\), 200 calls: min ([\d.]+), median ([\d.]+), 95% ([\d.]+), max ([\d.]+)', env)
        if sl: summ['sleep1_ms'] = [list(map(float, x)) for x in sl]
        summ['wine'] = (re.search(r'\[wine\] (.*)', env) or [None, None])[1]
    # ---------------------------------------------------------------- frames
    fr = glob.glob(os.path.join(d, 'xikit_frames_*.csv'))
    frames = []
    if fr:
        head, rows, _ = read_csv(fr[0]); frames = rows
        ft = [float(r['frame_ms']) for r in rows]; pr = [float(r['present_ms']) for r in rows]; kt = [float(r['kit_ms']) for r in rows]
        game = [a - b - c for a, b, c in zip(ft, pr, kt)]
        w(); w('## Frames (xikit_frames: %d frames, %.1f s)' % (len(ft), sum(ft) / 1000)); w()
        w('| | mean | p50 | p90 | p95 | p99 | max |'); w('|---|---|---|---|---|---|---|')
        for lab, xs in (('frame ms (Present to Present)', ft), ('inside the device\'s Present', pr), ('outside Present (the game\'s frame, the kit\'s check excluded)', game)):
            s = stats(xs)
            if s: w('| %s | %.2f | %.2f | %.2f | %.2f | %.2f | %.2f |' % (lab, s['mean'], s['p50'], s['p90'], s['p95'], s['p99'], s['max']))
        if ft:
            n = len(ft); w()
            w('Frames over 16.7 ms: %.1f%%; over 20 ms: %.1f%%; over 33.4 ms: %.1f%%; over 50 ms: %.1f%%. Mean rate %.1f fps.' % (
                100.0 * sum(x > 16.7 for x in ft) / n, 100.0 * sum(x > 20 for x in ft) / n, 100.0 * sum(x > 33.4 for x in ft) / n, 100.0 * sum(x > 50 for x in ft) / n, 1000.0 * n / sum(ft)))
            summ['frame_ms'] = stats(ft); summ['present_ms'] = stats(pr); summ['outside_present_ms'] = stats(game)
        # the game's pacing fields (RENDERER.md 3)
        p28 = [f32(r['p28']) for r in rows if r.get('p28')]; p2c = [f32(r['p2c']) for r in rows if r.get('p2c')]; p30 = [i32(r['p30']) for r in rows if r.get('p30')]
        tf = [f32(r['tfps']) for r in rows if r.get('tfps')]
        if p28:
            w(); w('**The game\'s pacing (Frame_Render\'s this, docs/RENDERER.md 3):** step `this+0x28` %s; this frame\'s ticks `this+0x2C` mean %.2f; minimum ticks `this+0x30` %s; '
              'the timer\'s last measured rate `[t+4]` mean %.1f.' % (
                  dict(collections.Counter(round(x, 3) for x in p28 if x is not None).most_common(6)), sum(p2c) / len(p2c) if p2c else float('nan'),
                  dict(collections.Counter(p30).most_common(4)), sum(tf) / len(tf) if tf else float('nan')))
            # real ticks against the step: what the step under-counts (the truncation, RENDERER.md 3a)
            real = [x / (1000.0 / 60) for x in ft]; step = [max(x or 0, 1.0) for x in p28]
            fs_on = False
            try:
                fs_on = re.search(r'^\s*frame_step\s*=\s*[1-9]', open(os.path.join(d, 'copy_speedfix.ini'), errors='replace').read(), re.M) is not None
            except OSError: pass
            if fs_on and real:   # (16 Nov) under frame_step speedfix's fork answers the step, not 0x10014CF0: its own log line counts it
                w('The getter\'s step against real time: not given here; frame_step is on (copy_speedfix.ini), so the step is speedfix\'s fork\'s, '
                  'whose whole and fraction ticks against real time are in its `speedfix: frame_step=` lines (xikit.log).')
            elif len(real) == len(step) and real:
                w('Real elapsed ticks per frame (frame ms / 16.67) mean %.3f; the getter\'s step (max(this+0x28, 1), 0x10014CF0) mean %.3f: what the step advances is %.1f%% of real time.' % (
                    sum(real) / len(real), sum(step) / len(step), 100.0 * sum(step) / sum(real)))
                summ['step_over_real'] = sum(step) / sum(real)
        # groups of wrapped calls on the render thread
        groups = [h[:-3] for h in head if h.endswith('_ms') and h not in ('frame_ms', 'present_ms', 'kit_ms', 'rt_cpu_ms', 'longest_ms')]
        if groups:
            w(); w('**The render thread\'s wrapped calls per frame, by group (exclusive ms; xikit_frames):**'); w()
            w('| group | mean ms | p95 ms | max ms | calls a frame |'); w('|---|---|---|---|---|')
            for g in groups:
                xs = [float(r[g + '_ms']) for r in rows]; cs = [int(r[g + '_calls']) for r in rows]
                if sum(cs): s = stats(xs); w('| %s | %.3f | %.3f | %.3f | %.1f |' % (g, s['mean'], s['p95'], s['max'], sum(cs) / len(cs)))
        cpu = [float(r['rt_cpu_ms']) for r in rows if float(r['rt_cpu_ms']) >= 0]
        if cpu and ft: w(); w('Render thread CPU time (GetThreadTimes; coarse on Windows: its clock steps by the scheduler tick) %.1f ms of %.1f ms of frames (%.0f%%).' % (sum(cpu), sum(ft), 100.0 * sum(cpu) / sum(ft)))
        marks = collections.defaultdict(list)
        for r in rows: marks[int(r['mark'])].append(float(r['frame_ms']))
        if len(marks) > 1:
            w(); w('**By mark (mark.bat; the name each mark was given is in xikit.log):**'); w(); w('| mark | frames | mean ms | p50 | p95 | p99 | max |'); w('|---|---|---|---|---|---|---|')
            for k in sorted(marks):
                s = stats(marks[k]); w('| %d | %d | %.2f | %.2f | %.2f | %.2f | %.2f |' % (k, s['n'], s['mean'], s['p50'], s['p95'], s['p99'], s['max']))
            summ['marks'] = {k: stats(v) for k, v in marks.items()}
    # ---------------------------------------------------------------- the clock test (clocktest.bat, frames_player=1)
    names = {int(m.group(1)): m.group(2).strip() for m in re.finditer(r'measure: mark (\d+) at frame \d+ \([^)]*\): (.*)', log)}
    if frames and 'me4' in frames[0] and any(v.startswith('clock ') for v in names.values()):
        qpf = None
        m = re.search(r'frequency (\d+)\)', open(fr[0]).readline())
        if m: qpf = float(m.group(1))
        seg = collections.defaultdict(list)
        for r in frames:
            nm = names.get(int(r['mark']), '')
            if nm in ('clock reference', 'clock test'): seg[(int(r['mark']), nm)].append(r)
        w(); w('## The clock test (clocktest.bat; your position each frame, frames_player=1)'); w()
        w('Reference: speedfix holds 60 fps, frame_step off; each frame advances the game by the getter\'s step, max(this+0x28, 1) ticks '
          '(0x10014CF0; docs/RENDERER.md 3a), so the game\'s time there is that sum / 60 s. Test: uncap and frame_step on; the game\'s time '
          'should be real time. Distance is from the entity\'s +0x04/+0x08/+0x0C and, separately, +0x24/+0x28/+0x2C (which of the two '
          'vectors is the drawn position is not established; both are shown). The first and last 3 s of each segment are left out (the settings and the mark change up to a second apart).'); w()
        w('| mark | segment | frames | real s | fps | game s (reference: by the step) | distance A | A per real s | A per game s | distance B | B per real s | B per game s |')
        w('|---|---|---|---|---|---|---|---|---|---|---|---|')
        per = collections.defaultdict(list)
        for (mk, nm), rows in sorted(seg.items()):
            if not qpf or len(rows) < 10: continue
            t0 = int(rows[0]['qpc']); t1 = int(rows[-1]['qpc'])          # both ends left out: the settings and the mark change a little apart
            rows = [r for r in rows if (int(r['qpc']) - t0) / qpf >= 3.0 and (t1 - int(r['qpc'])) / qpf >= 3.0]
            if len(rows) < 10: continue
            real = (int(rows[-1]['qpc']) - int(rows[0]['qpc'])) / qpf
            def dist(cols):
                d = 0.0; prev = None
                for r in rows:
                    if not r.get(cols[0]): prev = None; continue
                    v = [f32(r[c]) for c in cols]
                    if prev is not None and None not in v and None not in prev:
                        step = math.sqrt(sum((a - b) ** 2 for a, b in zip(v, prev)))
                        if step < 50: d += step            # (a jump of 50 or more in one frame is a warp, not running)
                    prev = v
                return d
            dA = dist(['me4', 'me8', 'meC']); dB = dist(['me24', 'me28', 'me2C'])
            game = sum(max(f32(r['p28']) or 0.0, 1.0) for r in rows[1:]) / 60.0 if nm == 'clock reference' else real
            w('| %d | %s | %d | %.2f | %.1f | %.2f | %.2f | %.3f | %.3f | %.2f | %.3f | %.3f |' % (mk, nm[6:], len(rows), real, (len(rows) - 1) / real,
              game, dA, dA / real, dA / game, dB, dB / real, dB / game))
            per[nm[6:]].append((dA / game, dB / game))
        if per.get('reference') and per.get('test'):
            for i, lab in ((0, 'A (+0x04..)'), (1, 'B (+0x24..)')):
                ref = sum(x[i] for x in per['reference']) / len(per['reference']); tst = sum(x[i] for x in per['test']) / len(per['test'])
                if ref > 0:
                    w(); w('**%s: distance per second of the game\'s time, test / reference = %.3f** (1.000: the game\'s time follows real time at the '
                           'test\'s frame rate as it does at 60).' % (lab, tst / ref))
                    summ.setdefault('clock_ratio', {})[lab] = tst / ref
        sfl = re.findall(r'speedfix: frame_step=.*', log)
        for l in sfl[-6:]: w('- `%s`' % l[:300])
    # ---------------------------------------------------------------- abtest.bat's segments (6 Nov session: marks "ab NAME A|B")
    if frames and any(re.match(r'ab \w+ [AB]$', v) for v in names.values()):
        qpf = None
        m = re.search(r'frequency (\d+)\)', open(fr[0]).readline())
        if m: qpf = float(m.group(1))
        seg = collections.defaultdict(list)
        for r in frames:
            nm = names.get(int(r['mark']), '')
            if re.match(r'ab \w+ [AB]$', nm): seg[(int(r['mark']), nm)].append(r)
        w(); w('## A/B segments (abtest.bat; the first and last 3 s of each left out)'); w()
        w('Speed: your character\'s distance per real second from the entity\'s +0x04/+0x08/+0x0C (frames_player; jumps of 50 or '
          'more in a frame left out as warps).'); w()
        w('| mark | segment | frames | real s | fps | mean ms | p50 | p95 | p99 | max | speed per real s |'); w('|---|---|---|---|---|---|---|---|---|---|---|')
        byseg = collections.defaultdict(list)
        for (mk, nm), rows in sorted(seg.items()):
            if not qpf or len(rows) < 10: continue
            t0 = int(rows[0]['qpc']); t1 = int(rows[-1]['qpc'])
            rows = [r for r in rows if (int(r['qpc']) - t0) / qpf >= 3.0 and (t1 - int(r['qpc'])) / qpf >= 3.0]
            if len(rows) < 10: continue
            real = (int(rows[-1]['qpc']) - int(rows[0]['qpc'])) / qpf
            walked = 0.0; prev = None
            for r in rows:
                v = [f32(r.get(c, '')) if r.get(c) else None for c in ('me4', 'me8', 'meC')]
                if prev is not None and None not in v and None not in prev:
                    st = math.sqrt(sum((a - b) ** 2 for a, b in zip(v, prev)))
                    if st < 50: walked += st
                prev = v
            s = stats([float(r['frame_ms']) for r in rows[1:]])
            w('| %d | %s | %d | %.2f | %.1f | %.2f | %.2f | %.2f | %.2f | %.2f | %.3f |' % (mk, nm, len(rows), real, (len(rows) - 1) / real,
              s['mean'], s['p50'], s['p95'], s['p99'], s['max'], walked / real))
            byseg[nm].append({'frame_mean': s['mean'], 'frame_p95': s['p95'], 'speed': walked / real})
        summ['ab'] = byseg
        for l in re.findall(r'speedfix: (?:frame_step=3|occlusion_late|liminality|gpu_skin).*', log)[-12:]: w('- `%s`' % l[:300])
        for l in re.findall(r'speedfix: occlusion_late=2: .*', log)[-4:]: w('- `%s`' % l[:300])
        # the draw calls by Frame_Render's world pass (8 Nov): summed over the start
        tot = None
        for l in re.findall(r'speedfix: draw calls by Frame_Render\'s world pass.*', log):
            v = [int(x) for x in re.findall(r' (\d+)(?:,| \()', l.split('line: ', 1)[-1])]
            if len(v) == 8: tot = v if tot is None else [a + b for a, b in zip(tot, v)]
        # task_draws (9 Nov): draws by task vtable and slot, summed over the start (draws a frame weighted by frames)
        tt = {}; tf = 0
        for l in re.findall(r'speedfix: task_draws, \d+ frames:.*', log):
            n = int(re.match(r'speedfix: task_draws, (\d+)', l).group(1)); tf += n
            for vt, slot, f20, f24, per in re.findall(r' ([0-9A-F]{8}) \+0x(20|24) \(([0-9A-F]{8}) ([0-9A-F]{8})\) ([\d.]+);', l):
                k = (vt, slot, f20, f24); tt[k] = tt.get(k, 0) + float(per) * n
        if tt and tf:
            w('- draw calls by task (task_draws), the whole start, draws a frame: ' + '; '.join(
                '%s +0x%s (+0x20 %s, +0x24 %s) %.1f' % (k[0], k[1], k[2], k[3], v / tf) for k, v in sorted(tt.items(), key=lambda x: -x[1])[:12]))
        # step reads (11 Nov): the getters' calls by return address, in the drawing tasks / other tasks / outside the task
        # pass, summed over the start; f = a fraction site, F = answered with the fraction (Liminality)
        sr = {}; sf = 0; srk = {}
        for l in re.findall(r'speedfix: step reads, \d+ frames:.*', log):
            n = int(re.match(r'speedfix: step reads, (\d+)', l).group(1)); sf += n
            for ra, kind, a, b, c in re.findall(r' ([0-9A-F]{8})([fF]?) ([\d.]+)/([\d.]+)/([\d.]+);', l):
                v = sr.setdefault(ra, [0.0, 0.0, 0.0]); v[0] += float(a) * n; v[1] += float(b) * n; v[2] += float(c) * n
                if kind: srk[ra] = 'F' if 'F' in (kind, srk.get(ra)) else 'f'
        if sr and sf:
            def where(v):
                return 'drawing tasks' if v[0] and not v[1] and not v[2] else 'other tasks' if v[1] and not v[0] and not v[2] else \
                       'outside the task pass' if v[2] and not v[0] and not v[1] else 'mixed'
            w('- step reads (the frame-step getters\' calls, by return address, the whole start; calls a frame in drawing tasks / '
              'other tasks / outside the task pass; where): ' + '; '.join(
                '%s%s %.1f/%.1f/%.1f %s' % (k, srk.get(k, ''), v[0] / sf, v[1] / sf, v[2] / sf, where(v))
                for k, v in sorted(sr.items(), key=lambda x: -sum(x[1]))[:40]))
            summ['step_reads'] = {k: [x / sf for x in v] + [srk.get(k, '')] for k, v in sr.items()}
        if tot:
            w('- draw calls by Frame_Render\'s world pass, the whole start: XiZone_GetInstance %d, sub_1018C2B0 %d, StDancer_Update %d, '
              'XiZone_DrawScene %d, **the task pass (OnMove) %d**, sub_102515F0 %d, sound %d, the rest of the frame %d' % tuple(tot))
    if log:
        # the occlusion readbacks' waits over the whole start (10 Nov): the game's own readback (first probes) and
        # speedfix's late reads, from the "readback time" lines; and how the never-waiting read went (occlusion_nowait)
        rg = rl = 0; tg = tl = 0.0; lmax = 0.0; fon = 0
        for g_, gm, c_, cm, r_, rm, rx, fr_ in re.findall(r"readback time: the game's readback (\d+) calls, mean ([\d.]+) us,.*?copy (\d+), mean ([\d.]+) us; "
                                                         r"its late read (\d+), mean ([\d.]+) us, longest ([\d.]+) us; the two [\d.]+ ms a frame with it on \((\d+) frames\)", log):
            rg += int(g_); tg += int(g_) * float(gm) / 1000.0; rl += int(r_); tl += int(r_) * float(rm) / 1000.0; lmax = max(lmax, float(rx) / 1000.0); fon += int(fr_)
        if fon:
            w('- occlusion waits over the start (%d frames with occlusion_late on): the game\'s own readback %d calls, %.2f ms a frame; '
              'speedfix\'s late reads %d, %.2f ms a frame, the longest %.1f ms' % (fon, rg, tg / fon, rl, tl / fon, lmax))
            summ['occl_wait'] = {'frames': fon, 'game_calls': rg, 'game_ms_frame': tg / fon, 'late_reads': rl, 'late_ms_frame': tl / fon, 'late_longest_ms': lmax}
        nw = [tuple(map(int, x)) for x in re.findall(r'occlusion_nowait: (\d+) locks told not yet \(no wait\), (\d+) probes answered with the object.s last answer again, (\d+) probes whose picture was not copied', log)]
        if nw: w('- occlusion_nowait over the start: %d locks told "not yet" without a wait, %d probes answered with the last answer again, %d probes not copied (all pictures in flight)' % tuple(sum(c) for c in zip(*nw)))
        nf = sum(int(x) for x in re.findall(r'occlusion_first=\d: (\d+) probes', log))
        if nf: w('- occlusion_first: %d first probes answered without the game\'s readback' % nf)
    # ---------------------------------------------------------------- d3d8on11's own lines (d3d8on11.log next to the game, if collected)
    # (remote mode: the executor's own lines are in d3d8on11_exec.log)
    for d8 in (glob.glob(os.path.join(d, '..', 'd3d8on11.log')) + glob.glob(os.path.join(d, 'd3d8on11.log')) +
               glob.glob(os.path.join(d, '..', 'd3d8on11_exec.log')) + glob.glob(os.path.join(d, 'd3d8on11_exec.log'))):
        t = open(d8, errors='replace').read().splitlines()
        keep = [l for l in t if re.search(r'executor|readback snapshots|present interval|CreateDevice|not implemented|error|fail|gpu_time|swap chain|swap_flip|draw callers|dynamic|frame_latency|buffer.arena|game thread in d3d8on11|draw.merge|draw delta|gpu.skin', l, re.I)]
        w(); w('## d3d8on11.log (%s, %d lines; the executor, readback snapshot, interval and error lines)' % (os.path.basename(d8), len(t))); w(); w('```')
        w('\n'.join(keep[:60] + (['...'] + keep[-20:] if len(keep) > 80 else keep[60:80]))); w('```')
        # the executor's time a frame over the start's windows (each 600 Presents): median and the busiest tenth
        ex = [tuple(map(float, x)) for x in re.findall(r'executor stats, \d+ frames: busy ([\d.]+) ms a frame \(([\d.]+)% of the time; draws ([\d.]+), uploads ([\d.]+), Present ([\d.]+), readbacks ([\d.]+)', '\n'.join(t))]
        if ex:
            cols = list(zip(*ex)); nm = ['busy', 'busy %', 'draws', 'uploads', 'Present', 'readbacks']
            w(); w('**The executor\'s time a frame** (%d windows of 600 Presents; ms, median / 90th percentile): ' % len(ex) +
                   ', '.join('%s %.2f / %.2f' % (n_, pct(list(c), 50), pct(list(c), 90)) for n_, c in zip(nm, cols)))
            summ['executor'] = {n_: [pct(list(c), 50), pct(list(c), 90)] for n_, c in zip(nm, cols)}
        ar = [tuple(map(float, x)) for x in re.findall(r'executor buffer arena, \d+ frames: ([\d.]+) buffers placed, ([\d.]+) ranges written in place, ([\d.]+) MB, ([\d.]+) Maps of an arena, ([\d.]+) times an arena started again, ([\d.]+) buffers back', '\n'.join(t))]
        if ar:
            cols = list(zip(*ar))
            w(); w('**The buffer arena a frame** (median of %d windows): %.1f buffers placed, %.1f ranges in place, %.3f MB, %.1f Maps, %.2f restarts, %.1f buffers back to their own' % ((len(ar),) + tuple(pct(list(c), 50) for c in cols)))
        wt = [tuple(map(float, x)) for x in re.findall(r'ring stats, \d+ frames: the game waited ([\d.]+) ms a frame for room, ([\d.]+) for pacing \(.*?\), ([\d.]+) for answers', '\n'.join(t))]
        if wt:
            cols = list(zip(*wt))
            w(); w('**The game\'s thread waiting on d3d8on11 a frame** (median / 90th percentile of %d windows, ms): for room %.2f / %.2f, for pacing %.2f / %.2f, for answers %.2f / %.2f' % (
                (len(wt),) + tuple(v for c in cols for v in (pct(list(c), 50), pct(list(c), 90)))))
            summ['ring_wait'] = {'answers': [pct(list(cols[2]), 50), pct(list(cols[2]), 90)]}
        # the executor's draw time by the game's call site (d3d8on11's "executor draws by game call site" lines: the Draw*
        # method's return address in FFXiMain.dll; offset + 0x10000000, the analysed image base, named from the IDB)
        tot = collections.defaultdict(lambda: [0.0, 0.0]); nwin = 0
        for l in t:
            m = re.search(r'executor draws by game call site, (\d+) frames \(.*?a frame\):(.*)', l)
            if not m: continue
            fr_ = int(m.group(1)); nwin += 1
            for it in m.group(2).split(';'):
                q = re.match(r'\s*(mod|\+0x[0-9A-F]+) ([\d.]+)/([\d.]+)', it)
                if q: k = q.group(1); tot[k][0] += float(q.group(2)) * fr_; tot[k][1] += float(q.group(3)) * fr_
        if tot:
            allms = sum(v[0] for v in tot.values())
            w(); w('**The executor\'s draw time by the game\'s call site** (%d windows of d3d8on11.log; each window lists its 16 '
                   'costliest sites, so a site outside them in a window is missing from that window\'s total):' % nwin); w()
            w('| call site | in function | share of the listed draw time | draws |'); w('|---|---|---|---|')
            for k, v in sorted(tot.items(), key=lambda kv: -kv[1][0])[:25]:
                if k == 'mod': w('| (a mod\'s call) | speedfix\'s merged ...UP draws | %.1f%% | %.0f |' % (100 * v[0] / allms, v[1])); continue
                ea = 0x10000000 + int(k[1:], 16)
                w('| 0x%08X | %s | %.1f%% | %.0f |' % (ea, fname(ea), 100 * v[0] / allms, v[1]))
            summ['draw_sites'] = {k: v for k, v in tot.items()}
    fms = {int(r['frame']): float(r['frame_ms']) for r in frames}
    med = pct(list(fms.values()), 50) if fms else 0
    spikes = {f for f, m in fms.items() if m > max(2 * med, 33.4)}
    # ---------------------------------------------------------------- wrapped calls
    pl = glob.glob(os.path.join(d, 'xikit_probe_list_*.csv')); pf = glob.glob(os.path.join(d, 'xikit_probe_frames_*.csv')); pt = glob.glob(os.path.join(d, 'xikit_probes_*.csv'))
    if pl and pf:
        _, plist, _ = read_csv(pl[0]); P = {int(r['k']): r for r in plist}
        tpm = None; per = collections.defaultdict(lambda: collections.defaultdict(lambda: [0, 0.0, 0.0, 0.0]))
        for line in open(pf[0]):
            if line.startswith('#cal'): tpm = float(line.split(',')[3]); continue
            if line.startswith('#'): continue
            p = line.split(',')
            if len(p) == 6: per[int(p[1])][int(p[0])] = [int(p[2]), int(p[3]), int(p[4]), int(p[5])]
        if tpm:
            w(); w('## Wrapped calls on the render thread, per frame (xikit_probe_frames; TSC %.0f ticks a ms)' % tpm); w()
            nf = max(1, len(fms) or len({f for v in per.values() for f in v}))
            rowsk = []
            for k, byf in per.items():
                ex = [v[2] / tpm for v in byf.values()]; inc = [v[1] / tpm for v in byf.values()]; calls = sum(v[0] for v in byf.values())
                rowsk.append((sum(ex) / nf, k, sum(inc) / nf, max(v[3] for v in byf.values()) / tpm, calls / nf, len(byf),
                              sum(byf[f][2] / tpm for f in spikes if f in byf) / max(1, len(spikes))))
            rowsk.sort(reverse=True)
            w('Exclusive = the call\'s own time less the wrapped calls inside it. Per frame = over all %d frames (a call not made in a frame counts 0).' % nf); w()
            w('| call | kind | group | from | excl ms/frame | incl ms/frame | max one call ms | calls/frame | frames with it | excl ms/frame in spikes |')
            w('|---|---|---|---|---|---|---|---|---|---|')
            for ex, k, inc, mx, cpf, nfw, spk in rowsk[:80]:
                p = P.get(k, {}); kind = p.get('kind', '?'); fromw = ''
                if kind == 'S': fromw = fname(int(p['analysed'], 16))
                w('| %s | %s | %s | %s | %.3f | %.3f | %.3f | %.1f | %d | %.3f |' % (p.get('name', k), kind, p.get('group', ''), fromw, ex, inc, mx, cpf, nfw, spk))
            summ['probes_top'] = [{'name': P.get(k, {}).get('name'), 'kind': P.get(k, {}).get('kind'), 'excl_ms_frame': ex, 'incl_ms_frame': inc} for ex, k, inc, *_ in rowsk[:40]]
    if pt:
        _, prow, com = read_csv(pt[0])
        oth = [r for r in prow if r['thread'] == 'other']
        if oth:
            w(); w('## Wrapped calls on the other threads (xikit_probes: totals since the start)'); w()
            w('| call | group | from | calls | incl ms | excl ms | max ms |'); w('|---|---|---|---|---|---|---|')
            for r in sorted(oth, key=lambda r: -float(r['excl_ms']))[:40]:
                ra = r['return_to']; m = re.match(r'game:0x([0-9A-F]+)', ra); ra = fname(int(m.group(1), 16)) if m else ra
                w('| %s | %s | %s | %s | %s | %s | %s |' % (r['name'], r['group'], ra, r['calls'], r['incl_ms'], r['excl_ms'], r['max_ms']))
        for c in com: w('- `%s`' % c[:300])
    # ---------------------------------------------------------------- sample=3
    raws = glob.glob(os.path.join(d, 'xikit_raw_*.bin'))
    if raws:
        R = Raw(raws[0])
        render = collections.Counter(); per_thread = collections.Counter(); self_by = collections.defaultdict(collections.Counter)
        lvl1 = collections.Counter(); lvl2 = collections.Counter(); lvl1_spike = collections.Counter(); outside = collections.Counter()
        rt = None; mland_seen = 0; nrt = 0; nrt_spike = 0
        def lab(wh):
            if wh[0] == 'game': return 'game ' + fname(wh[1])
            if wh[0] == 'recon': return 'recon +0x%X' % wh[1]
            if wh[0] == 'module': return wh[1]
            return 'memory'
        for tid, frn, q, eip, ras, fl in R.samples:
            per_thread[tid] += 1
            if fl & 1: rt = tid
            if eip is None: self_by[tid]['(not moved since the last look)'] += 1; continue
            if R.mland in ras: mland_seen += 1
            we = R.where(eip); self_by[tid][lab(we)] += 1
            if not fl & 1: continue
            nrt += 1; sp = frn in spikes; nrt_spike += sp
            # the outermost return address inside Frame_Render, and the next one inward
            ga = [R.analysed(r) if R.glo <= r < R.ghi else None for r in ras]
            idx = [i for i, a in enumerate(ga) if a is not None and F.inside(FRAME_RENDER, a)]
            if idx:
                i = idx[-1]; site = ga[i]
                key = 'Frame_Render call at 0x%08X' % site
                lvl1[key] += 1
                if sp: lvl1_spike[key] += 1
                inner = ga[i - 1] if i > 0 else None
                if inner is not None: lvl2[(key, fname(inner))] += 1
                elif i == 0: lvl2[(key, 'self: ' + lab(we))] += 1
            else:
                # outside Frame_Render: the outermost game return address says where
                gi = [a for a in ga if a is not None]
                outside[fname(gi[-1]) if gi else 'no game return address: ' + lab(we)] += 1
        w(); w('## Samples (sample=3: xikit_raw; %d samples, %d threads)' % (len(R.samples), len(per_thread))); w()
        w('| thread | role | started at | samples | top places (EIP) |'); w('|---|---|---|---|---|')
        for tid, n in per_thread.most_common(30):
            st = R.threads.get(tid); stl = lab(R.where(st)) if st else '?'
            tops = '; '.join('%s %d' % (k, v) for k, v in self_by[tid].most_common(5))
            w('| %d | %s | %s | %d | %s |' % (tid, 'render' if tid == rt else '', stl, n, tops))
        nb = sum(1 for x in R.samples if x[5] & 9 == 9); nr_all = sum(1 for x in R.samples if x[5] & 1)
        if nb: w(); w('**%d of the render thread\'s %d samples (%.1f%%) had ESP outside its stack (the TEB\'s bounds): the x86 context the system gave was not the x86 code\'s (Windows on ARM: the thread was in native code). Those samples carry no return addresses; the native sampler (sample=2) covers them.**' % (nb, nr_all, 100.0 * nb / max(1, nr_all)))
        if mland_seen: w(); w('**%d samples still held the kit\'s m_land on the stack (the shadow stack did not give the real return address).**' % mland_seen)
        if nrt:
            # call sites at Frame_Render: the measure table's target names
            tnames = {}
            mt = mtab or os.path.join(d, '..', '..', 'xikit_measure.txt')
            if os.path.exists(mt):
                for line in open(mt):
                    p = line.split()
                    if p and p[0] == 'S': tnames[int(p[1], 16) + 5] = p[5]
            w(); w('### The render thread\'s samples by Frame_Render call (%d samples; %d in spike frames, frames over max(2 x median, 33.4 ms))' % (nrt, nrt_spike)); w()
            w('| call (return address in Frame_Render) | callee | share | share in spikes |'); w('|---|---|---|---|')
            for k, v in lvl1.most_common(40):
                ra = int(k.split()[-1], 16)
                w('| %s | %s | %.1f%% | %.1f%% |' % (k, tnames.get(ra, '(indirect or unlisted)'), 100.0 * v / nrt, 100.0 * lvl1_spike[k] / max(1, nrt_spike)))
            tot_out = sum(outside.values())
            if tot_out:
                w(); w('Outside Frame_Render: %.1f%% of the render thread\'s samples, by the outermost game return address:' % (100.0 * tot_out / nrt)); w()
                for k, v in outside.most_common(15): w('- %s: %.1f%%' % (k, 100.0 * v / nrt))
            w(); w('### One level down (the next return address inward)'); w()
            w('| Frame_Render call | where inside it | share |'); w('|---|---|---|')
            for (k, inner), v in lvl2.most_common(60): w('| %s | %s | %.1f%% |' % (k, inner, 100.0 * v / nrt))
            summ['render_samples'] = nrt; summ['frame_render_calls'] = {k: v / nrt for k, v in lvl1.most_common(40)}
            summ['outside_frame_render'] = {k: v / nrt for k, v in outside.most_common(15)}
        summ['threads'] = {str(t): {'samples': n, 'top': self_by[t].most_common(8)} for t, n in per_thread.most_common(30)}
    # ---------------------------------------------------------------- sys and threads
    sy = glob.glob(os.path.join(d, 'xikit_sys_*.csv'))
    if sy:
        _, rows, _ = read_csv(sy[0])
        if rows:
            w(); w('## The process and the machine, each second (xikit_sys)'); w()
            for col, labx in (('proc_user_ms', 'process user ms a second'), ('proc_kernel_ms', 'process kernel ms a second'), ('machine_busy_pct', 'machine busy %'),
                              ('ws_mb', 'working set MB'), ('pf', 'page faults a second'), ('read_mb', 'file reads MB a second'), ('read_ops', 'file reads a second')):
                s = stats([float(r[col]) for r in rows])
                if s: w('- %s: mean %.1f, p95 %.1f, max %.1f' % (labx, s['mean'], s['p95'], s['max']))
    th = glob.glob(os.path.join(d, 'xikit_threads_*.csv'))
    if th:
        _, rows, _ = read_csv(th[0]); tot = collections.defaultdict(lambda: [0.0, 0.0, 0.0, '', ''])
        for r in rows:
            t = tot[r['tid']]; t[3] = r['start']; t[4] = r['role']
            for i, c in enumerate(('mcycles', 'user_ms', 'kernel_ms')):
                if float(r[c]) >= 0: t[i] += float(r[c])
        if tot:
            w(); w('## Threads by CPU over the session (xikit_threads)'); w(); w('| tid | role | start | user ms | kernel ms | Mcycles |'); w('|---|---|---|---|---|---|')
            for tid, t in sorted(tot.items(), key=lambda kv: -(kv[1][1] + kv[1][2]))[:25]:
                st = t[3]; m = re.match(r'game:0x([0-9A-F]+)', st); st = fname(int(m.group(1), 16)) if m else st
                w('| %s | %s | %s | %.0f | %.0f | %.0f |' % (tid, t[4], st, t[1], t[2], t[0]))
    # ---------------------------------------------------------------- PresentMon
    if pm_path and os.path.exists(pm_path):
        _, rows, _ = read_csv(pm_path)
        rows = [r for r in rows if r.get('Application', '').lower() == 'pol.exe'] or rows
        w(); w('## PresentMon (%s, %d presents)' % (os.path.basename(pm_path), len(rows))); w()
        cols = [c for c in ('MsBetweenPresents', 'FrameTime', 'MsInPresentAPI', 'CPUBusy', 'CPUWait', 'GPULatency', 'GPUTime', 'GPUBusy', 'GPUWait', 'MsUntilDisplayed',
                            'DisplayLatency', 'MsBetweenDisplayChange', 'DisplayedTime', 'MsGPUActive') if rows and c in rows[0]]
        w('| column | mean | p50 | p95 | p99 | max |'); w('|---|---|---|---|---|---|')
        for c in cols:
            xs = []
            for r in rows:
                try: xs.append(float(r[c]))
                except ValueError: pass
            s = stats(xs)
            if s: w('| %s | %.2f | %.2f | %.2f | %.2f | %.2f |' % (c, s['mean'], s['p50'], s['p95'], s['p99'], s['max']))
        for c in ('PresentMode', 'SyncInterval', 'AllowsTearing', 'Runtime'):
            if rows and c in rows[0]: w('- %s: %s' % (c, dict(collections.Counter(r[c] for r in rows).most_common(4))))
    open(os.path.join(d, 'report.md'), 'w').write('\n'.join(out) + '\n')
    json.dump(summ, open(os.path.join(d, 'summary.json'), 'w'), indent=1, default=str)
    print('wrote %s and summary.json' % os.path.join(d, 'report.md'))
    return summ


if __name__ == '__main__':
    main(sys.argv[1:])
