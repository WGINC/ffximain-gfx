"""gate_cache.py -- reuse a gate verdict only where the new run is provably the same run.

A case's gate run (diffemu_gfx.run_case) is deterministic: every random value comes from the case's seed,
and each case gets a fresh emulator. Its result is therefore a function of exactly what the run reads:

  static   the harness code (diffemu_gfx.py, diffemu_fixed.py, live_state.py, this file), the unicorn /
           capstone / pefile versions, the game DLL, the case record, trials and seed, the data files the
           harness reads whole (com_spec.json, import_nargs.json, func_extents.json), and in live mode the
           live config, its dump and build map;
  dynamic  the bytes of the reconstruction DLL the run touched -- every address executed, read or written in
           the reconstruction's window (0x60000000-0x61000000) by either side, with whether it lies in the
           image and in an executable section -- and the call-site facts site_pops looked up (each lookup's
           entries in callsite_args / sp_solved / real_sp / vcalls / com_calls).

A cached verdict is reused when the static key matches and every recorded dynamic dependency has the same
value now (the new DLL holds the same bytes at the touched addresses, the facts give the same entries). The
run would then execute the same instructions on the same values step for step, so its result is the cached
one. Anything else is gated afresh and recorded. The cache never decides a verdict that was not computed.

    entries: runs/gate_cache/<static key>.json = [{"deps": {...}, "result": {...}}, ...]
"""
import hashlib, json, os, sys

P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data')
DIR = os.environ.get('GATE_CACHE', os.path.join(P, '..', 'runs', 'gate_cache'))
WIN = (0x60000000, 0x61000000)
_FILES = {}


def _sha_file(path):
    """sha256 of a file, memoised on (path, size, mtime) in the cache directory (dumps are large)"""
    st = os.stat(path); k = '%s|%d|%d' % (os.path.realpath(path), st.st_size, st.st_mtime_ns)
    if k in _FILES: return _FILES[k]
    memo = os.path.join(DIR, 'filehash.json')
    try: m = json.load(open(memo))
    except (OSError, ValueError): m = {}
    if k not in m:
        h = hashlib.sha256()
        with open(path, 'rb') as f:
            for b in iter(lambda: f.read(1 << 22), b''): h.update(b)
        m[k] = h.hexdigest()
        os.makedirs(DIR, exist_ok=True); tmp = memo + '.%d' % os.getpid()
        json.dump(m, open(tmp, 'w')); os.replace(tmp, memo)
    _FILES[k] = m[k]; return m[k]


def base_key(game_dll, live_cfg):
    """the part of the static key shared by every case of a process"""
    import unicorn, capstone, pefile
    h = hashlib.sha256()
    for f in ('diffemu_gfx.py', 'diffemu_fixed.py', 'live_state.py', 'gate_cache.py'):
        h.update(f.encode()); h.update(_sha_file(os.path.join(P, f)).encode())
    for f in ('com_spec.json', 'import_nargs.json', 'func_extents.json'):
        p = os.path.join(D, f); h.update(f.encode()); h.update(_sha_file(p).encode() if os.path.exists(p) else b'-')
    h.update(('%s %s %s' % (unicorn.__version__, capstone.__version__, pefile.__version__)).encode())
    h.update(_sha_file(game_dll).encode())
    if live_cfg:
        h.update(_sha_file(live_cfg).encode()); cfg = json.load(open(live_cfg))
        for k in ('dump', 'build_map'):
            if cfg.get(k): h.update(_sha_file(cfg[k]).encode())
    return h.hexdigest()


def case_key(base, case, trials, seed):
    return hashlib.sha256(('%s|%d|%d|' % (base, trials, seed) + json.dumps(case, sort_keys=True)).encode()).hexdigest()


class Image:
    """the reconstruction DLL as the gate maps it: bytes per address, and which addresses are executable"""
    def __init__(self, path):
        import pefile
        pe = pefile.PE(path)
        self.base = pe.OPTIONAL_HEADER.ImageBase
        self.img = pe.get_memory_mapped_image(ImageBase=self.base)
        self.size = (max(pe.OPTIONAL_HEADER.SizeOfImage, len(self.img)) + 0xFFF) & ~0xFFF
        self.code = [(self.base + s.VirtualAddress, self.base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData))
                     for s in pe.sections if s.Characteristics & 0x20000000]

    def digest(self, ranges):
        h = hashlib.sha256()
        for a, n in ranges:
            h.update(b'%x:%x;' % (a, n))
            for x in range(a, a + n):
                o = x - self.base
                if 0 <= o < self.size: h.update(bytes([self.img[o] if o < len(self.img) else 0]))
                else: h.update(b'U')                                  # not mapped by the image
                h.update(b'x' if any(s <= x < e for s, e in self.code) else b'-')
        return h.hexdigest()


def ranges(points):
    """[(addr, size)] -> merged [(start, length)]"""
    out = []
    for a, n in sorted(points):
        if out and a <= out[-1][0] + out[-1][1]:
            out[-1][1] = max(out[-1][1], a + n - out[-1][0])
        else: out.append([a, n])
    return [tuple(x) for x in out]


def site_record(callsites, k):
    return [callsites.get(s, {}).get(k) for s in ('typed', 'solved', 'real', 'vtable', 'com')]


class Tracker:
    """records a run's dynamic dependencies on a machine (hooks limited to the reconstruction's window)"""
    def __init__(self, m):
        from unicorn import UC_HOOK_BLOCK, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE, UC_HOOK_MEM_INVALID
        self.touched = set(); self.sites = set(); t = self.touched
        m.uc.hook_add(UC_HOOK_BLOCK, lambda uc, a, n, _: t.add((a, max(n, 1))), begin=WIN[0], end=WIN[1] - 1)
        m.uc.hook_add(UC_HOOK_MEM_READ | UC_HOOK_MEM_WRITE, lambda uc, acc, a, n, v, _: t.add((a, n)), begin=WIN[0], end=WIN[1] - 1)
        def inv(uc, acc, a, n, v, _):
            if WIN[0] <= a < WIN[1]: t.add((a, max(n, 1)))
            return False
        m.uc.hook_add(UC_HOOK_MEM_INVALID, inv)
        m.dep_sites = self.sites

    def deps(self, image, callsites):
        r = ranges(self.touched)
        return {'recon': r, 'recon_sha': image.digest(r), 'sites': {k: site_record(callsites, k) for k in sorted(self.sites)}}


def lookup(key, image, callsites):
    try: entries = json.load(open(os.path.join(DIR, key[:2], key + '.json')))
    except (OSError, ValueError): return None
    for e in entries:
        d = e['deps']
        if image.digest([tuple(x) for x in d['recon']]) == d['recon_sha'] and \
           all(site_record(callsites, k) == v for k, v in d['sites'].items()):
            return e['result']
    return None


def store(key, deps, result):
    p = os.path.join(DIR, key[:2], key + '.json'); os.makedirs(os.path.dirname(p), exist_ok=True)
    try: entries = json.load(open(p))
    except (OSError, ValueError): entries = []
    entries = [e for e in entries if e['deps'] != deps] + [{'deps': deps, 'result': result}]
    tmp = p + '.%d' % os.getpid(); json.dump(entries[-8:], open(tmp, 'w')); os.replace(tmp, p)
