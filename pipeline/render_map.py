"""render_map.py -- every Direct3D 8 call site of the graphics code, with its method and where that comes from.

Two kinds of source, kept apart in the output:

  static   data/com_calls.json: the object's interface is proven (or an accepted declaration, marked so) and
           the method follows from the slot and the D3D8 header. This holds for every execution.
  observed what the gate's runs saw at the call site (diffemu_gfx.py with FFXI_COM_SITES=1, files RUN/*.com):
           * "fake": the call reached a fake object of interface I. Fake objects are placed by the case's
             declarations (gfx_cases.py: the D3D holder, CDx, this_com.json; those are declarations, not
             proofs) or written by a D3D8 method's out-parameter (the header's type). So "fake I" means: in
             this run the object was the one the declarations or the header call I.
           * "dump": the call reached a real object of a memory dump (live mode). Its vtable is compared with
             the vtables of the dump's device objects, read from the dump through the declared paths
             ([[g_pCDx]+0xC], [[g_pD3DHolder]+8], [g_pDancerDevice]); equal means the same class as that
             device object. Any other vtable is reported by address only: its interface is not known here.
           An observation is a fact about the runs (that input reached the site with that object), not
           about every execution: a site can see other objects in the game.

    python3 render_map.py RUN OUT.json               uses $FFXI_DLL; RUN holds the .com files
"""
import collections, glob, json, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

P = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(P, '..', 'data')
SPEC = json.load(open(os.path.join(D, 'com_spec.json')))
IFACES = sorted(SPEC)
# the device holders the cases declare (gfx_cases.py); each a path from a global
DEVICE_PATHS = {'[[g_pCDx]+0xC]': (0x1045666C, (0, 0xC)), '[[g_pD3DHolder]+0x8]': (0x103D3CBC, (0, 0x8)),
                '[g_pDancerDevice]': (0x1066941C, (0,))}
LIVE = {'town': 'live_town.json', 'town2': 'live_town2.json', 'combat': 'live_combat.json', 'zonetoinn': 'live_zonetoinn.json'}
LIVE_DIR = os.environ.get('FFXI_LIVE_DIR', '/root/ffxi_dumps/out')


def enums():
    """D3D8 enumerations from pipeline/xi_d3d8.hpp (the SDK values): enum name -> {value: constant}"""
    out, cur = {}, None
    for l in open(os.path.join(P, 'xi_d3d8.hpp')):
        m = re.match(r'\s*enum\s+(\w+)\s*\{', l)
        if m: cur = out.setdefault(m.group(1), {}); continue
        if l.strip().startswith('}'): cur = None; continue
        m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+|-?\d+)', l)
        if m and cur is not None: cur.setdefault(int(m.group(2), 0) & 0xFFFFFFFF, m.group(1))
    return out


def call_site(im, md, ret):
    """the call instruction that returns to ret: the one ending exactly there (2-, 3- or 6-byte indirect call)"""
    for L in (3, 2, 6, 7):
        for i in md.disasm(bytes(im.img[ret - L - im.base:ret - im.base]), ret - L):
            if i.mnemonic == 'call' and i.address + i.size == ret: return i
            break
    return None


def device_vtables():
    """{dump: {path: (object, vtable)}} read from each dump through the declared paths"""
    import live_state
    out = {}
    for name, f in LIVE.items():
        p = os.path.join(LIVE_DIR, f)
        if not os.path.exists(p): continue
        cfg = json.load(open(p)); ls = live_state.LiveState(cfg['dump'], cfg['build_map'])
        rd = lambda a: (lambda b: None if b is None else struct.unpack('<I', b)[0])(ls.read(a, 4))
        got = {}
        for label, (g, offs) in DEVICE_PATHS.items():
            v = rd(g + ls.rel)
            for o in offs[1:]:
                v = rd(v + o) if v else None
            vt = rd(v) if v else None
            if v and vt: got[label] = (v, vt)
        out[name] = got
    return out


def main(run, out):
    im = S.Image(os.environ['FFXI_DLL']); md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    names = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l); names['0x%08X' % r['ea']] = r['name']
    static = json.load(open(os.path.join(D, 'com_calls.json')))
    devs = device_vtables()
    sites = {}
    def site(ret, func):
        if ret not in sites:
            i = call_site(im, md, ret)
            a = '0x%08X' % (i.address if i else ret)
            sites[ret] = {'call': a, 'func': func, 'insn': ('%s %s' % (i.mnemonic, i.op_str)) if i else None,
                          'static': None, 'observed': collections.defaultdict(lambda: {'runs': 0, 'calls': 0, 'args': set(), 'objects': set()})}
        return sites[ret]
    for f in sorted(glob.glob(os.path.join(run, '*.jsonl.com'))):
        rn = os.path.basename(f).split('_')[0]
        for l in open(f):
            r = json.loads(l); func = '0x%08X' % int(r['original'], 16)
            for ret, kind, a, slot, n, objs, args in r['sites']:
                s = site(ret, func)
                if kind == 'fake':
                    key = ('fake', a, slot)
                else:                                 # a call into a dump module: kind is the callee address
                    vt = a
                    same = sorted(lbl for lbl, (o, v) in devs.get(rn, {}).items() if vt is not None and v == vt)
                    key = ('dump-device', 'IDirect3DDevice8', slot, tuple(same)) if same and slot is not None else \
                          ('dump-other', '0x%08X' % vt if vt is not None else None, slot)
                ob = s['observed'][key]
                ob['runs'] += 1; ob['calls'] += n; ob.setdefault('in', set()).add(rn)
                ob['args'].update(tuple(x) for x in args[:8]); ob['objects'].update(objs)
    for k, v in static.items():
        ret = None
        i = next(md.disasm(bytes(im.img[int(k, 16) - im.base:int(k, 16) - im.base + 8]), int(k, 16)), None)
        if i is None: continue
        s = site(i.address + i.size, v['func'])
        s['static'] = {'iface': v['iface'], 'method': v['method'], 'slot': v['slot'], 'tier': 'accepted' if v.get('accepted') else 'proven'}
    E = enums()
    res, conflicts = {}, []
    for ret, s in sorted(sites.items()):
        obs = []
        for key, ob in sorted(s['observed'].items(), key=str):
            kind, iface, slot = key[0], key[1], key[2]
            m = SPEC[iface][slot]['name'] if iface in SPEC and slot is not None and slot < len(SPEC[iface]) else None
            e = {'kind': kind, 'iface': iface, 'slot': slot, 'method': m, 'calls': ob['calls'], 'in': sorted(ob['in']),
                 'args': [list(a) for a in sorted(ob['args'])[:8]]}
            if kind == 'dump-device': e['same_vtable_as'] = list(key[3])
            if m: e['args_named'] = [name_args(m, a, E) for a in sorted(ob['args'])[:8]]
            obs.append(e)
        methods = {(o['iface'], o['method']) for o in obs if o['method']}
        if s['static']: methods.add((s['static']['iface'], s['static']['method']))
        if len(methods) > 1: conflicts.append(s['call'])
        res[s['call']] = {'func': s['func'], 'func_name': names.get(s['func']), 'insn': s['insn'], 'static': s['static'],
                          'observed': obs}
    stats = collections.Counter()
    for c, r in res.items():
        tiers = []
        if r['static']: tiers.append('static ' + r['static']['tier'])
        if any(o['kind'] == 'fake' for o in r['observed']): tiers.append('observed fake')
        if any(o['kind'] == 'dump-device' for o in r['observed']): tiers.append('observed dump device')
        if any(o['kind'] == 'dump-other' for o in r['observed']): tiers.append('observed dump other')
        stats[' + '.join(tiers) or 'none'] += 1
    json.dump({'_doc': __doc__.strip().split('\n')[0], 'device_objects': {k: {l: ['0x%08X' % x for x in v] for l, v in d.items()}
                                                                           for k, d in devs.items()},
               'conflicts': conflicts, 'stats': dict(stats), 'sites': res}, open(out, 'w'), indent=1, sort_keys=True)
    print('%d call sites; %d with more than one method; %s' % (len(res), len(conflicts), dict(stats)))


def name_args(method, a, E):
    """the SDK constant names of a call's first arguments where the header gives the parameter an enum type"""
    rs, tss, pt, ts = E.get('D3DRENDERSTATETYPE', {}), E.get('D3DTEXTURESTAGESTATETYPE', {}), E.get('D3DPRIMITIVETYPE', {}), E.get('D3DTRANSFORMSTATETYPE', {})
    if method == 'SetRenderState' and a: return [rs.get(a[0], hex(a[0]))] + ['0x%X' % x for x in a[1:2]]
    if method == 'SetTextureStageState' and len(a) > 1: return [a[0], tss.get(a[1], hex(a[1]))] + ['0x%X' % x for x in a[2:3]]
    if method in ('DrawPrimitive', 'DrawIndexedPrimitive', 'DrawPrimitiveUP', 'DrawIndexedPrimitiveUP') and a:
        return [pt.get(a[0], hex(a[0]))] + ['0x%X' % x for x in a[1:]]
    if method in ('SetTransform', 'GetTransform', 'MultiplyTransform') and a: return [ts.get(a[0], hex(a[0]))]
    return ['0x%X' % x for x in a]


if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2])
