"""read.py -- the native sampler's file (xikit_native_<pid>.csv, xisamp.c) as shares of the render thread's time.

    python3 read.py xikit_native_1234.csv [--from 21:50:00] [--to 21:55:00] [--focus 0x1002A034 ...] [--top 25]
    python3 read.py xikit_stutters_1234.csv [--frames 1460 1461 ...] [--min-ms 150] [--top 25]

The stutter file (xisamp.c, frame= spike=) has one block per frame over spike_ms, numbered as xikit_spikes_<pid>.csv
numbers frames; the same views are then over the frames chosen (--frames, --min-ms), and a "stutters" view lists
each frame, longest first, with where its samples were (the game function the x86 code was in, or, outside the game
and the reconstruction, the innermost game/reconstruction call site and the module the x86 code was in).

Each sample holds, from the same instant: where the x86 code was (eip, and the x86 return addresses found on its
stack) and where the machine was natively (ARM64 pc, lr, and the frame-pointer chain). Views, as shares of the
samples in the windows chosen (--from/--to: local times, compared with each window's start):
  x86        the module the x86 eip was in (or the memory, when outside any image);
  native     the module the native pc was in;
  joint      x86 module x native module: which native code ran while the x86 thread stood where;
  no image   samples whose x86 eip is in no image (e.g. the 0x00010002 xikit sample=1 reported): their eip values,
             native pc (module!nearest export) and the native frames' modules;
  elsewhere  samples whose eip is outside the game and the reconstruction, by the innermost game or reconstruction
             return address on the x86 stack (the call site that went there; xikit sample=1's "caller");
  syscalls   the innermost x86 return address into win32u.dll or ntdll.dll (module!export: the system-call stub the
             x86 code called; the native pc is then in its wow64 thunk or the kernel call), with the native module
             paired with it as a check (a stub's samples should sit in one thunk), then stub by call site counting
             only samples whose native pc is in wow64win.dll or ntdll.dll (inside the call, not a stale return
             address left on the stack while other code runs);
  focus      for each --focus call site (analysed address, as the elsewhere view prints it): its samples split by
             x86 module!export, then native module!export, then the system-call stub.
Names: a game address is the analysed build's (the header's rel and shift table) with its function (samples.py's
Names: the manifest and data/func_extents.json); another image's address is module+rva and the nearest export at or
below it -- a position, not the function's name (a module's internal functions have no export). The x86 return
addresses are a stack scan (xisamp.c): a stale one in a dead slot is found too, so the elsewhere view's call site is
the innermost one found, as with sample=1. A share's standard error is about sqrt(p(1-p)/n)."""
import argparse, collections, math, os, sys
H = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(H, '..', 'd3dprof'))
from samples import Names   # noqa: E402

GAME, RECON = 'ffximain.dll', 'gfx_recon_ingame.dll'
STUBS, INCALL = ('win32u.dll', 'ntdll.dll'), ('wow64win.dll', 'ntdll.dll')


class Window:
    def __init__(self, p):
        self.n, self.start, self.end = int(p[1]), p[2], p[3]
        self.samples, self.missed, self.xfail, self.nfail, self.dropped = map(int, p[4:9])
        self.ms = float(p[9]) if len(p) > 9 else None   # a stutter block: the frame's span by the sampler's clock
        self.mods, self.labels, self.rows = [], {}, []

    def mod(self, a):
        for base, size, mach, path in self.mods:
            if base <= a < base + size: return base, mach, path.replace('/', '\\').split('\\')[-1]
        return None


def load(path):
    head, wins = [], []
    for l in open(path, encoding='utf-8', errors='replace'):
        l = l.rstrip('\r\n')
        if l.startswith('#'): head.append(l); continue
        p = l.split(',')
        if p[0] == 'w':
            wins.append(Window(p))
            if len(wins) > 1 and wins[-1].ms is not None:   # stutter blocks: images when they change, labels once
                wins[-1].mods, wins[-1].labels = wins[-2].mods, wins[-2].labels
                wins[-1].fresh = True
        elif p[0] == 'm' and wins and getattr(wins[-1], 'fresh', False):
            wins[-1].mods, wins[-1].fresh = [], False
            wins[-1].mods.append((int(p[1], 16), int(p[2], 16), int(p[3], 16), ','.join(p[4:])))
        elif not wins: continue
        elif p[0] == 'm': wins[-1].mods.append((int(p[1], 16), int(p[2], 16), int(p[3], 16), ','.join(p[4:])))
        elif p[0] == 's':
            ch = lambda s: [int(x, 16) for x in s.split(';') if x]
            wins[-1].rows.append((int(p[1]), int(p[2], 16), ch(p[3]), int(p[4], 16), int(p[5], 16), ch(p[6])))
        elif p[0] == 'a': wins[-1].labels[int(p[1], 16)] = p[2:]   # a dict shared with later blocks
    return head, wins


class Namer:
    def __init__(self, head, names):
        self.rel, self.shift, self.names = 0, [], names
        for h in head:
            if h.startswith('# rel '):
                a, _, b = h[6:].partition('; shift')
                self.rel = int(a.strip(), 0) & 0xFFFFFFFF
                for t in b.split():
                    lo, hi, d = t.split(':'); self.shift.append((int(lo, 16), int(hi, 16), int(d, 0)))

    def analysed(self, a):
        ea = (a - self.rel) & 0xFFFFFFFF
        for lo, hi, d in self.shift:
            if lo + d <= ea < hi + d: return ea - d
        return ea

    def where(self, w, a, depth=2):
        """depth 0: module; 1: module!export (game: function); 2: and the address."""
        m = w.mod(a)
        if not m:
            lab = w.labels.get(a, [])
            kind = lab[5] if len(lab) > 5 and lab[5] else 'unknown'
            alloc = lab[4] if len(lab) > 4 else ''
            if depth == 0: return '(no image: %s)' % kind
            return '(no image: %s, allocation %s)%s' % (kind, alloc or '?', ' 0x%X' % a if depth >= 2 else '')
        base, mach, mn = m
        if mn.lower() == GAME:
            if depth == 0: return 'FFXiMain.dll'
            ea = self.analysed(a); fn = self.names.game(ea)
            return fn if depth == 1 else '%s @0x%08X' % (fn, ea)
        if depth == 0: return mn
        lab = w.labels.get(a, [])
        exp = ('!%s+0x%s' % (lab[2], lab[3][2:] if lab[3].startswith('0x') else lab[3])) if len(lab) > 3 and lab[2] else ''
        if depth == 1: return '%s%s' % (mn, ('!' + lab[2]) if len(lab) > 2 and lab[2] else '')
        return '%s+0x%X%s' % (mn, a - base, exp)

    def ours(self, w, a):
        m = w.mod(a)
        return bool(m) and m[2].lower() in (GAME, RECON)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('file'); ap.add_argument('--from', dest='t0'); ap.add_argument('--to', dest='t1')
    ap.add_argument('--focus', nargs='*', default=[]); ap.add_argument('--top', type=int, default=25)
    ap.add_argument('--frames', nargs='*', type=int, default=[]); ap.add_argument('--min-ms', type=float, default=0)
    ap.add_argument('--manifest', default=os.path.join(H, '..', 'xikit', 'dist', 'xikit_manifest.txt'))
    ap.add_argument('--extents', default=os.path.join(H, '..', '..', 'data', 'func_extents.json'))
    o = ap.parse_args()
    head, wins = load(o.file)
    nm = Namer(head, Names(o.manifest, o.extents))
    sel = [w for w in wins if (not o.t0 or w.start[11:] >= o.t0) and (not o.t1 or w.start[11:] < o.t1)]
    stutter = any(w.ms is not None for w in wins)
    if stutter: sel = [w for w in sel if (not o.frames or w.n in o.frames) and w.ms >= o.min_ms]
    for h in head[:2]: print(h)
    total = sum(c for w in sel for c, *_ in w.rows)
    print('%d windows (%s .. %s), %d samples; missed %d, x86 context failed %d, native context failed %d, dropped (table full) %d' % (
        len(sel), sel[0].start if sel else '-', sel[-1].end if sel else '-', total, sum(w.missed for w in sel),
        sum(w.xfail for w in sel), sum(w.nfail for w in sel), sum(w.dropped for w in sel)))
    if not total: return
    pct = lambda n: '%6.2f%% +-%.2f' % (100 * n / total, 100 * math.sqrt((n / total) * (1 - n / total) / total))

    def table(title, ctr, top=o.top, indent='  '):
        print('\n%s' % title)
        for k, n in ctr.most_common(top): print('%s%s  %s' % (indent, pct(n), k))

    x86, nat, joint, noimg_eip, noimg_pc, noimg_frames, caller = (collections.Counter() for _ in range(7))
    stubs, stub_pc, stub_site = collections.Counter(), collections.Counter(), collections.Counter()
    focus = {int(f, 16): tuple(collections.Counter() for _ in range(4)) for f in o.focus}
    for w in sel:
        for c, eip, xc, pc, lr, nc in w.rows:
            xm, pm = nm.where(w, eip, 0), nm.where(w, pc, 0)
            x86[xm] += c; nat[pm] += c; joint['%s  |  %s' % (xm, pm)] += c
            if not w.mod(eip):
                noimg_eip['eip 0x%08X' % eip] += c
                noimg_pc[nm.where(w, pc, 2)] += c
                noimg_frames[' <- '.join([nm.where(w, pc, 1), nm.where(w, lr, 1)] + [nm.where(w, a, 1) for a in nc[:6]])] += c
            st = next((a for a in xc if w.mod(a) and w.mod(a)[2].lower() in STUBS), None)
            stk = nm.where(w, st, 1) if st is not None else '(no win32u/ntdll return address)'
            stubs[stk] += c; stub_pc['%s  |  %s' % (stk, nm.where(w, pc, 2) if st is not None else nm.where(w, pc, 0))] += c
            if not nm.ours(w, eip):
                site = next((a for a in xc if nm.ours(w, a)), None)
                if site is None: caller['(no game or reconstruction return address found)'] += c; continue
                key = nm.where(w, site, 2) if w.mod(site)[2].lower() != GAME else '0x%08X in %s' % (nm.analysed(site), nm.where(w, site, 1))
                caller[key] += c
                if st is not None and nm.where(w, pc, 0).lower() in INCALL: stub_site['%s  |  %s' % (key, stk)] += c
                if w.mod(site)[2].lower() == GAME and nm.analysed(site) in focus:
                    a, b, d, e = focus[nm.analysed(site)]
                    a[nm.where(w, eip, 1)] += c; b[nm.where(w, pc, 1)] += c; e[stk] += c
                    d['%s  |  %s <- %s' % (nm.where(w, eip, 1), nm.where(w, pc, 1), nm.where(w, lr, 1))] += c
    if stutter:
        print('\nstutters: each frame, longest first (ms by the sampler; frame as in xikit_spikes): where its samples were')
        for w in sorted(sel, key=lambda w: -w.ms)[:o.top]:
            n = sum(c for c, *_ in w.rows); at = collections.Counter()
            for c, eip, xc, pc, lr, nc in w.rows:
                if nm.ours(w, eip): at[nm.where(w, eip, 1)] += c; continue
                site = next((a for a in xc if nm.ours(w, a)), None)
                k = nm.where(w, eip, 1)
                if site is not None:
                    k = '%s <- %s' % (k, '0x%08X in %s' % (nm.analysed(site), nm.where(w, site, 1)) if w.mod(site)[2].lower() == GAME else nm.where(w, site, 2))
                at[k] += c
            print('  frame %d: %.1f ms, %d samples (missed %d, dropped %d)' % (w.n, w.ms, n, w.missed, w.dropped))
            for k, c in at.most_common(6): print('      %5.1f%%  %s' % (100 * c / max(n, 1), k))
    table('x86: the module the x86 code was in', x86)
    table('native: the module the native code was in', nat)
    table('joint: x86 module | native module', joint)
    if noimg_eip:
        table('no image: x86 eip values outside every image', noimg_eip, 10)
        table('no image: their native pc (module+rva!nearest export+offset)', noimg_pc)
        table('no image: their native frames (pc <- lr <- the fp chain), by module!export', noimg_frames)
    table('elsewhere: x86 eip outside the game and reconstruction, by the innermost game/reconstruction call site', caller)
    table('syscalls: the innermost x86 return address into win32u.dll/ntdll.dll (the stub called)', stubs)
    table('syscalls: stub | native pc (a stub\'s samples in one wow64 thunk is the check)', stub_pc)
    table('syscalls: call site | stub, native pc in wow64win.dll/ntdll.dll', stub_site)
    for f, (a, b, d, e) in focus.items():
        n = sum(a.values())
        print('\nfocus 0x%08X: %s' % (f, pct(n)))
        table('  x86 eip (module!export)', a, indent='    ')
        table('  native pc (module!export)', b, indent='    ')
        table('  joint: x86 | native pc <- lr', d, indent='    ')
        table('  system-call stub', e, indent='    ')


if __name__ == '__main__':
    main()
