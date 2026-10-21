"""gfx_cases.py -- equivalence-gate cases for the translated graphics functions.
Usage: python3 gfx_cases.py gfx_export3.jsonl raw/meta.json protos_fixed.json cases.json"""
import json, os, re, sys
import hx_patches  # noqa: E402  (prototype corrections, data/type_fixes.json)
import case_abs_reads  # noqa: E402  (uses $FFXI_DLL)
DATADIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')

# what each callee can really return (ret_kinds.py): stubs answer within that range
RET_KINDS = json.load(open(os.path.join(DATADIR, 'ret_kinds.json'))) if os.path.exists(os.path.join(DATADIR, 'ret_kinds.json')) else {}
# calls whose x87 result the original caller consumes (fp_callsites.py): their stubs push a value
FPRET = json.load(open(os.path.join(DATADIR, 'fpret.json')))
FP_SITES = json.load(open(os.path.join(DATADIR, 'fp_callsites.json'))) if os.path.exists(os.path.join(DATADIR, 'fp_callsites.json')) else {}
# globals whose real values are bounded (a table count): fuzzed values are mapped into the range,
# so a loop over the table runs as in the game instead of to the instruction limit
RANGES = json.load(open(os.path.join(DATADIR, 'input_ranges.json')))['ranges'] if os.path.exists(os.path.join(DATADIR, 'input_ranges.json')) else []
CDECL_ARGS = json.load(open(os.path.join(DATADIR, 'cdecl_args.json'))) if os.path.exists(os.path.join(DATADIR, 'cdecl_args.json')) else {}
CDECL_LOW = CDECL_ARGS.pop('_at_least', {})
# callees whose own code reads their entry ECX (vcall_resolve.py, data/ecx_use.json: the value is an instruction that
# reads it). Their stubs log ECX whatever the declared convention, so a reconstruction that calls one without passing
# the object (the IDB types DynVB_Reserve __stdcall(int); it reads ECX) differs from the original at the call
ECX_READ = {k for k, v in (json.load(open(os.path.join(DATADIR, 'ecx_use.json'))) if os.path.exists(os.path.join(DATADIR, 'ecx_use.json')) else {}).items()
            if isinstance(v, str) and v.startswith('0x')}
# small leaf functions (leaf_funcs.py) run for real: a stub cannot do their writes (`*out = this->f`)
PURGES = json.load(open(os.path.join(DATADIR, 'purges.json'))) if os.path.exists(os.path.join(DATADIR, 'purges.json')) else {}
# tail-jump functions pop what their target's `ret N` pops (tail_purges.py: the jump is at stack depth exactly 0)
if os.path.exists(os.path.join(DATADIR, 'tail_purges.json')):
    PURGES.update({k: v['pops'] for k, v in json.load(open(os.path.join(DATADIR, 'tail_purges.json'))).items() if k not in PURGES})
THIS_COM = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'this_com.json')))
# fake Direct3D objects behind globals (and in `this`), per function, each with its source (data/global_com.json)
GLOBAL_COM = {k: v for k, v in json.load(open(os.path.join(DATADIR, 'global_com.json'))).items() if not k.startswith('_')} \
    if os.path.exists(os.path.join(DATADIR, 'global_com.json')) else {}
LEAF = json.load(open(os.path.join(DATADIR, 'leaf_funcs.json'))) if os.path.exists(os.path.join(DATADIR, 'leaf_funcs.json')) else {}
ALLOC = {int(k, 16): v for k, v in json.load(open(os.path.join(DATADIR, 'allocators.json'))).items() if k.startswith('0x')} \
    if os.path.exists(os.path.join(DATADIR, 'allocators.json')) else {}
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
    if t in ('float', 'double', 'long double', 'float10'): return 'float'      # float10: Ghidra's ST0 (conv_facts.py, fpret)
    if t in ('bool', 'char', 'unsigned __int8', '_BOOL1', '__int8', 'unsigned char', '_BYTE',
             'undefined1', 'byte', 'uchar', 'xi_bool', 'undefined'): return 'bool'          # Ghidra's 1-byte names
    if t in ('__int64', 'unsigned __int64', '_QWORD'): return 'int64'
    if t in ('__int16', 'unsigned __int16', '_WORD', 'short', 'unsigned short',
             'undefined2', 'ushort', 'word'): return 'word'                                  # Ghidra's 2-byte names
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
    out = []                                   # one mask per stack slot: a double or __int64 takes two
    for p in parts:
        p = p.strip()
        if '*' in p: out.append(0xFFFFFFFF)
        elif re.search(r'^(double|__int64|_QWORD|unsigned __int64)\b', p): out += [0xFFFFFFFF, 0xFFFFFFFF]
        elif re.search(r'__int16|\bshort\b|_WORD', p): out.append(0xFFFF)
        elif re.search(r'__int8|\bchar\b|_BYTE|\bbool\b|_BOOL1', p): out.append(0xFF)
        else: out.append(0xFFFFFFFF)
    return out[:16]

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

COM_OUT = re.compile(r'^\s*(?:const\s+)?(?:struct\s+)?(ID3DXBuffer|IDirect3D\w*8)\s*\*\s*\*')

def com_outs(t):
    # stack parameters typed `I... **`: interface pointers the callee returns through (ppShader, ppErrors)
    if '(' not in t: return []
    ps = t[t.index('(') + 1:t.rindex(')')].split(',')
    if '__thiscall' in t[:t.index('(')] and ps: ps = ps[1:]        # ECX
    return [[i, m.group(1)] for i, p in enumerate(ps) for m in [COM_OUT.match(p)] if m]

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
        if r.get('source') != 'ghidra': r = hx_patches.fix_types(r)    # IDA prototype corrections (hx2cpp the same)
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
            hint = THIS_COM.get('0x%08X' % r['ea'])
            if hint: c['this_com'] = dict(c.get('this_com', {}), **hint['fields'])   # data/this_com.json
            gh = GLOBAL_COM.get('0x%08X' % r['ea'], {}).get('this_com')
            if gh: c['this_com'] = dict(c.get('this_com', {}), **gh)                 # data/global_com.json
        c['args'] = [arg_kind(a[1], a[4]) for a in stack_args]
        stubs = {}; fuzz = []; glob = {}; ran = set()
        for nm, o in r.get('objs', {}).items():
            ea = o['ea']
            if o['func']:
                if ea == r['ea']: continue
                if '0x%08X' % ea in LEAF: ran.add('0x%08X' % ea); continue      # a trivial leaf runs for real
                cc = conv_of(o['type']); pops = max(o.get('purged') or 0, 0) // 4
                # the callee's own `ret N` (purges.json) over the decompiler's count: D3DX dispatch thunks, where IDA sees
                # only `jmp [ptr]` and says 0, and a function Ghidra could not resolve, whose purge it reports as -1
                if '0x%08X' % ea in PURGES and PURGES['0x%08X' % ea] // 4 != pops:
                    pops = PURGES['0x%08X' % ea] // 4
                    if cc == 'cdecl' and pops: cc = 'stdcall'
                p = protos.get(hex(ea))
                nargs = pops if cc != 'cdecl' else (p[1] if p and p[1] is not None else 0)
                # a cdecl callee's arguments are the slots its code reads (cdecl_args.py): slots past them are stale
                # stack (deferred cleanup), not arguments. Exact where every path could be followed; otherwise the
                # slots read on the paths that could be (`_at_least`), so only slots proven to be arguments are
                # compared. The IDB's count only where the code says nothing.
                if cc == 'cdecl' and '0x%08X' % ea in CDECL_ARGS:
                    nargs = CDECL_ARGS['0x%08X' % ea]
                elif cc == 'cdecl' and '0x%08X' % ea in CDECL_LOW:
                    nargs = CDECL_LOW['0x%08X' % ea]
                if cc == 'cdecl' and ea in hx_patches.type_fixes() and r.get('source') != 'ghidra':  # a corrected IDA prototype (each with its evidence)
                    ps = [x for x in o['type'][o['type'].index('(') + 1:o['type'].rindex(')')].split(',') if x.strip() not in ('', 'void')]
                    nargs = sum(2 if re.match(r'\s*(double|__int64|_QWORD)\b', x) else 1 for x in ps)
                if '...' in o['type']:      # variadic: only the fixed parameters are defined; slots past
                    nargs = len([x for x in o['type'][o['type'].index('(') + 1:].split(',') if x.strip() and '...' not in x])
                    # them read whatever the caller's frame holds (saved registers etc.)
                stubs['0x%08X' % ea] = {'returns': 'fuzz', 'pops': pops, 'args': min(nargs or 0, 16),
                                        'conv': 'thiscall' if cc == 'thiscall' else cc,
                                        'masks': arg_masks(o['type'], cc)}
                if RET_KINDS.get('0x%08X' % ea): stubs['0x%08X' % ea]['ret'] = RET_KINDS['0x%08X' % ea]
                if stubs['0x%08X' % ea]['conv'] != 'thiscall' and '0x%08X' % ea in ECX_READ: stubs['0x%08X' % ea]['ecx'] = 1
                outs = com_outs(o['type'])
                if outs: stubs['0x%08X' % ea]['com_out'] = outs
                if ea in ALLOC:
                    stubs['0x%08X' % ea]['ret'] = 'alloc'
                    if ALLOC[ea].get('size_arg') is not None: stubs['0x%08X' % ea]['size_arg'] = ALLOC[ea]['size_arg']
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
            if '0x%08X' % sa in LEAF: ran.add('0x%08X' % sa)
            if sa == r['ea'] or '0x%08X' % sa in stubs or '0x%08X' % sa in LEAF: continue
            p = protos.get(hex(sa))
            if not p or p[0] not in ('thiscall', 'stdcall', 'cdecl') or p[1] is None: continue
            pops = 0 if p[0] == 'cdecl' else p[1]
            stubs['0x%08X' % sa] = {'returns': 'fuzz', 'pops': pops, 'args': min(p[1], 16),
                                    'conv': 'thiscall' if p[0] == 'thiscall' else p[0], 'masks': []}
            if RET_KINDS.get('0x%08X' % sa): stubs['0x%08X' % sa]['ret'] = RET_KINDS['0x%08X' % sa]
            if 'indirect' in FP_SITES.get('0x%08X' % r['ea'], ()) and FPRET.get('0x%x' % sa) in ('float', 'double'):
                stubs['0x%08X' % sa]['fpret'] = 1
        c['stubs'] = stubs
        for lf in sorted(ran):              # globals a leaf that runs for real reads: fuzzed like the caller's own
            for g in LEAF[lf]:
                if int(g, 16) not in DEVICE_DATA and g not in glob and not any(f[0] == g for f in fuzz):
                    glob[g] = 'ptr:0x400'
        used = {'0x%08X' % o['ea'] for o in r.get('objs', {}).values()}
        rng_ = [e for e in RANGES if (e.get('func') and int(e['func'], 16) == r['ea']) or
                (not e.get('func') and e['where'] in ('global', 'global_ptr') and '0x%08X' % int(e['addr'], 16) in used)]
        if rng_: c['input_ranges'] = [{k: v for k, v in e.items() if k not in ('why', 'func')} for e in rng_]
        if fuzz: c['fuzz_ranges'] = fuzz
        if glob: c['globals'] = glob
        c.update(COMMON)
        c = case_abs_reads.add(c)                # globals its own code reads at a fixed address, fuzzed too
        sg = GLOBAL_COM.get('0x%08X' % r['ea'], {}).get('struct_globals')
        if sg:                                   # data/global_com.json: a struct with fake objects there, not fuzzed bytes
            c['struct_globals'] = dict(c['struct_globals'], **{hex(int(g, 16)): {k: v for k, v in e.items() if k != 'why'}
                                                               for g, e in sg.items()})
            held = {int(g, 16) for g in sg}
            c['fuzz_ranges'] = [f for f in c.get('fuzz_ranges', []) if int(f[0], 16) not in held]
            if not c['fuzz_ranges']: del c['fuzz_ranges']
            for g in list(c.get('globals', {})):
                if int(g, 16) in held: del c['globals'][g]
        cases.append(c)
    json.dump(cases, open(out, 'w'), indent=0)
    print(len(cases), 'cases')

if __name__ == '__main__':
    main(*sys.argv[1:5])
