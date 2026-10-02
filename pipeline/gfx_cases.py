"""gfx_cases.py -- equivalence-gate cases for the translated graphics functions.
Usage: python3 gfx_cases.py gfx_export3.jsonl raw/meta.json protos_fixed.json cases.json"""
import json, os, re, sys
DATADIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')

# what each callee can really return (ret_kinds.py): stubs answer within that range
RET_KINDS = json.load(open(os.path.join(DATADIR, 'ret_kinds.json'))) if os.path.exists(os.path.join(DATADIR, 'ret_kinds.json')) else {}
# calls whose x87 result the original caller consumes (fp_callsites.py): their stubs push a value
FPRET = json.load(open(os.path.join(DATADIR, 'fpret.json')))
FP_SITES = json.load(open(os.path.join(DATADIR, 'fp_callsites.json'))) if os.path.exists(os.path.join(DATADIR, 'fp_callsites.json')) else {}
# globals whose real values are bounded (a table count): fuzzed values are mapped into the range,
# so a loop over the table runs as in the game instead of to the instruction limit
RANGES = json.load(open(os.path.join(DATADIR, 'input_ranges.json')))['ranges'] if os.path.exists(os.path.join(DATADIR, 'input_ranges.json')) else []
DATA = (0x1034F000, 0x109FA000)          # .data: globals the gate may fuzz
HOLDER, CDX, DANCER, D3D_SQGX = 0x103D3CBC, 0x1045666C, 0x1066941C, 0x10669418
DEVICE_DATA = {HOLDER, CDX, DANCER, D3D_SQGX}
COMMON = {
    'struct_globals': {hex(HOLDER): {'size': '0x478', 'com': {'4': 'IDirect3D8', '8': 'IDirect3DDevice8'}},
                       hex(CDX): {'size': '0xA50', 'com': {'8': 'IDirect3D8', '12': 'IDirect3DDevice8'}}},
    'com_globals': {hex(DANCER): 'IDirect3DDevice8', hex(D3D_SQGX): 'IDirect3D8'},
}
IFACE_RE = re.compile(r'(IDirect3D\w+)\s*\*$')

def ret_kind(t):
    t = t.strip()
    if t == 'void': return 'void'
    if t in ('float', 'double', 'long double'): return 'float'
    if t in ('bool', 'char', 'unsigned __int8', '_BOOL1', '__int8', 'unsigned char', '_BYTE'): return 'bool'
    if t in ('__int64', 'unsigned __int64', '_QWORD'): return 'int64'
    if t in ('__int16', 'unsigned __int16', '_WORD', 'short', 'unsigned short'): return 'word'
    return 'int'

def arg_kind(t, psize):
    t = t.strip()
    if t in ('float',): return 'float'
    if t.endswith('*') or t.endswith(']'):
        # a pointee with no known layout (char/_DWORD/void) is sized generously: decompiled code
        # indexes such pointers far beyond their nominal element (mutation test, 1 Oct 2026)
        size = psize if psize and 16 <= psize < 0x10000 else 0x1000
        return 'ptr:0x%X' % max(size, 0x100)
    return 'int'

def arg_masks(ftype, cc):
    """per stack argument: the bits the callee actually receives (8/16-bit params are pushed as
    dwords whose upper bits are whatever was in the register -- not part of the call)"""
    m = re.search(r'\((.*)\)\s*$', ftype, re.S)
    if not m: return []
    parts, depth, cur = [], 0, ''
    for ch in m.group(1):
        if ch == ',' and depth == 0: parts.append(cur); cur = ''; continue
        depth += (ch == '(') - (ch == ')'); cur += ch
    if cur.strip(): parts.append(cur)
    if cc == 'thiscall' and parts: parts = parts[1:]
    out = []
    for p in parts[:4]:
        p = p.strip()
        if '*' in p: out.append(0xFFFFFFFF)
        elif re.search(r'__int16|\bshort\b|_WORD', p): out.append(0xFFFF)
        elif re.search(r'__int8|\bchar\b|_BYTE|\bbool\b|_BOOL1', p): out.append(0xFF)
        else: out.append(0xFFFFFFFF)
    return out

def conv_of(ftype):
    m = re.search(r'__(cdecl|stdcall|thiscall|fastcall)', ftype)
    return m.group(1) if m else 'cdecl'

VT = json.load(open(os.path.join(DATADIR, 'class_vtables.json')))
MAN = {int(k, 16): v for k, v in json.load(open(os.path.join(DATADIR, 'tree_manifest.json'))).items()}
_CLASSES = sorted(VT, key=len, reverse=True)
def class_of(r):
    """same evidence as the clean placement: tree manifest, IDB name prefix, typed this"""
    m = MAN.get(r['ea'])
    if m and m.get('class'): return m['class']
    for c in _CLASSES:
        if r['name'].startswith(c + '_'): return c
    if r['args'] and 'CDx' in (r['args'][0][3] or ''): return 'CDx'
    return None

GSIZE = {'char': 1, 'byte': 1, 'undefined': 1, 'undefined1': 1, 'bool': 1, 'short': 2, 'ushort': 2, 'undefined2': 2,
         'word': 2, 'int': 4, 'uint': 4, 'undefined4': 4, 'dword': 4, 'float': 4, 'double': 8, 'undefined8': 8}

def ghidra_record(r):
    # Ghidra exports (pipeline/ghidra/GfxExport.java): bare calling convention, and args in the IDA
    # shape [name, type, size, pointee type, pointee size] (pointee size from the type name)
    if r.get('source') != 'ghidra': return r
    import hx2cpp
    r = hx2cpp.ghidra_fix_convention(r)
    args = []
    for nm, t in r['args']:
        pt = t.rstrip()[:-1].strip() if t.rstrip().endswith('*') else None
        args.append([nm, t, 8 if t in ('double', 'undefined8') else 4, pt, GSIZE.get(pt) if pt else None])
    return dict(r, cc=r['cc'].lstrip('_'), args=args)

def main(exp, metaf, protof, out):
    meta = json.load(open(metaf)); protos = json.load(open(protof))
    cases = []
    for line in open(exp):
        r = ghidra_record(json.loads(line)); key = '%08X' % r['ea']
        mt = meta.get(key)
        if not mt or mt['status'] != 'translated': continue
        conv = r['cc']
        if conv not in ('cdecl', 'stdcall', 'thiscall'): continue
        args = r['args']
        c = {'name': '%s 0x%08X' % (r['name'], r['ea']), 'original': '0x%08X' % r['ea'],
             'candidate': mt['export'], 'conv': conv, 'returns': ret_kind(r['ret'])}
        if c['returns'] == 'float' and FPRET.get('0x%x' % r['ea']) == 'none': c['returns'] = 'void'   # see hx2cpp
        if c['returns'] == 'float': c['fret'] = 'float' if r['ret'].strip() == 'float' else 'double' if r['ret'].strip() == 'double' else 'ldouble'
        stack_args = args[1:] if conv == 'thiscall' else args
        if conv == 'thiscall':
            ps = args[0][4] if args and args[0][4] and args[0][4] >= 16 else 0x4000
            c['this_size'] = hex(min(max(ps, 0x100), 0x4000))
            cls = class_of(r)
            if cls == 'CDx' or (args and 'CDx' in (args[0][3] or '')):
                c['this_com'] = {'8': 'IDirect3D8', '12': 'IDirect3DDevice8'}
            if r['name'].startswith(('Gfx_', 'D3DHolder_')) or (args and 'D3DHolder' in (args[0][3] or '')):
                c['this_com'] = {'4': 'IDirect3D8', '8': 'IDirect3DDevice8'}      # the D3D holder (DancingMad 18.1)
            if cls in VT:
                c['vptr'] = VT[cls]['vtable']; c['vslots'] = VT[cls]['slots']
        c['args'] = [arg_kind(a[1], a[4]) for a in stack_args]
        stubs = {}; fuzz = []; glob = {}
        for nm, o in r.get('objs', {}).items():
            ea = o['ea']
            if o['func']:
                if ea == r['ea']: continue
                cc = conv_of(o['type']); pops = (o.get('purged') or 0) // 4
                p = protos.get(hex(ea))
                nargs = pops if cc != 'cdecl' else (p[1] if p and p[1] is not None else 0)
                if '...' in o['type']:      # variadic: only the fixed parameters are defined; slots past
                    nargs = len([x for x in o['type'][o['type'].index('(') + 1:].split(',') if x.strip() and '...' not in x])
                    # them read whatever the caller's frame holds (saved registers etc.)
                stubs['0x%08X' % ea] = {'returns': 'fuzz', 'pops': pops, 'args': min(nargs or 0, 4),
                                        'conv': 'thiscall' if cc == 'thiscall' else cc,
                                        'masks': arg_masks(o['type'], cc)}
                if RET_KINDS.get('0x%08X' % ea): stubs['0x%08X' % ea]['ret'] = RET_KINDS['0x%08X' % ea]
                if '0x%08X' % ea in FP_SITES.get('0x%08X' % r['ea'], ()): stubs['0x%08X' % ea]['fpret'] = 1
            elif o.get('is_str'):
                continue
            elif DATA[0] <= ea < DATA[1] and ea not in DEVICE_DATA:
                t = o['type'].strip()
                if t.endswith('*') and not t.endswith('**') and '(' not in t:
                    glob['0x%08X' % ea] = 'ptr:0x400'
                elif o.get('size'):
                    fuzz.append(['0x%08X' % ea, min(o['size'], 0x200)])
        for s in c.pop('vslots', []):
            sa = int(s, 16)
            if sa == r['ea'] or '0x%08X' % sa in stubs: continue
            p = protos.get(hex(sa))
            if not p or p[0] not in ('thiscall', 'stdcall', 'cdecl') or p[1] is None: continue
            pops = 0 if p[0] == 'cdecl' else p[1]
            stubs['0x%08X' % sa] = {'returns': 'fuzz', 'pops': pops, 'args': min(p[1], 4),
                                    'conv': 'thiscall' if p[0] == 'thiscall' else p[0], 'masks': []}
            if RET_KINDS.get('0x%08X' % sa): stubs['0x%08X' % sa]['ret'] = RET_KINDS['0x%08X' % sa]
            if 'indirect' in FP_SITES.get('0x%08X' % r['ea'], ()) and FPRET.get('0x%x' % sa) in ('float', 'double'):
                stubs['0x%08X' % sa]['fpret'] = 1
        c['stubs'] = stubs
        used = {'0x%08X' % o['ea'] for o in r.get('objs', {}).values()}
        rng_ = [e for e in RANGES if (e.get('func') and int(e['func'], 16) == r['ea']) or
                (not e.get('func') and e['where'] in ('global', 'global_ptr') and '0x%08X' % int(e['addr'], 16) in used)]
        if rng_: c['input_ranges'] = [{k: v for k, v in e.items() if k not in ('why', 'func')} for e in rng_]
        if fuzz: c['fuzz_ranges'] = fuzz
        if glob: c['globals'] = glob
        c.update(COMMON)
        cases.append(c)
    json.dump(cases, open(out, 'w'), indent=0)
    print(len(cases), 'cases')

if __name__ == '__main__':
    main(*sys.argv[1:5])
