"""ida_gfx.py -- IDA side of the graphics reconstruction (run inside IDA; functions are called in
short, resumable steps from the MCP bridge; state lives in builtins._GFX).

  propagate_round()  push D3D interface / struct pointer types from call sites into callee
                     parameters (and `this`) that are still untyped ints/void pointers
  select()           graphics set: users of device-bearing data + graphics class families
  export_step(sec)   decompile the next functions and write one JSON record each
"""
import json, os, re, time, builtins
import idautils, idc, ida_funcs, ida_name, ida_typeinf, ida_nalt, ida_hexrays, ida_bytes

D = r"C:\Users\wadeglover\Downloads\ffxi_ida_exchange"
S = builtins.__dict__.setdefault('_GFX', {})
TIL = ida_typeinf.get_idati()

DEVICE_DATA = ['g_pD3DHolder', 'g_pCDx', 'g_pDancerDevice']
DEVICE_EAS = [0x10669418]
FAMILY = re.compile(r'^(CDx|Gfx|CShader|CMo[A-Z]\w*|CYyModel\w*|CXiSkeleton\w*|CYyTex\w*|CXiTex\w*|'
                    r'CYyCamera\w*|CYyGen\w*|Post_\w*)')
GENERIC = re.compile(r'^(int|unsigned int|_DWORD \*|void \*|int \*|unsigned __int32|__int32)$')

def is_lib(ea):
    f = ida_funcs.get_func(ea)
    return bool(f and f.flags & ida_funcs.FUNC_LIB)

def decomp(ea):
    try:
        return ida_hexrays.decompile(ea, flags=ida_hexrays.DECOMP_NO_CACHE)
    except Exception:
        return None

def interesting_type(t):
    s = str(t)
    return ('IDirect3D' in s or 'D3DHolder' in s or s.startswith('CDx')) and t.is_ptr()

# ------------------------------------------------------------------ type propagation
def propagate_round(funcs=None, budget=40):
    """For every call in `funcs` whose argument has an interesting pointer type, give the
    callee parameter that type if it is still generic. Returns the callees changed."""
    t0 = time.time()
    todo = S.setdefault('prop_todo', list(funcs or []))
    changed = S.setdefault('prop_changed', set())
    while todo and time.time() - t0 < budget:
        ea = todo.pop()
        cf = decomp(ea)
        if not cf: continue
        class V(ida_hexrays.ctree_visitor_t):
            def __init__(s): super().__init__(ida_hexrays.CV_FAST); s.hits = []
            def visit_expr(s, e):
                if e.op == ida_hexrays.cot_call and e.x.op == ida_hexrays.cot_obj:
                    for i, a in enumerate(e.a):
                        if interesting_type(a.type): s.hits.append((e.x.obj_ea, i, a.type))
                return 0
        v = V(); v.apply_to(cf.body, None)
        for callee, i, at in v.hits:
            if is_lib(callee) or not ida_funcs.get_func(callee): continue
            tif = ida_typeinf.tinfo_t()
            if not ida_nalt.get_tinfo(tif, callee): continue
            fd = ida_typeinf.func_type_data_t()
            if not tif.get_func_details(fd) or i >= fd.size(): continue
            if not GENERIC.match(str(fd[i].type)): continue
            fd[i].type = at
            nt = ida_typeinf.tinfo_t()
            if nt.create_func(fd) and ida_typeinf.apply_tinfo(callee, nt, ida_typeinf.TINFO_DEFINITE):
                changed.add(callee)
    return {'left': len(todo), 'changed': len(changed)}

# ------------------------------------------------------------------ selection
def select():
    seeds = set()
    eas = [ida_name.get_name_ea(idc.BADADDR, n) for n in DEVICE_DATA] + DEVICE_EAS
    for g in eas:
        for x in idautils.XrefsTo(g):
            f = ida_funcs.get_func(x.frm)
            if f and not is_lib(f.start_ea): seeds.add(f.start_ea)
    fam = {f for f in idautils.Functions() if FAMILY.match(ida_name.get_name(f)) and not is_lib(f)}
    typed = set()
    for f in idautils.Functions():
        t = idc.get_type(f) or ''
        if ('IDirect3D' in t or 'D3DHolder' in t or 'CDx *' in t) and not is_lib(f): typed.add(f)
    S['select'] = {'seeds': sorted(seeds), 'family': sorted(fam), 'typed': sorted(typed)}
    allf = sorted(seeds | fam | typed)
    S['targets'] = allf
    return {'seeds': len(seeds), 'family': len(fam), 'typed': len(typed), 'total': len(allf)}

# ------------------------------------------------------------------ export
BAD = re.compile(r'__usercall|__userpurge|JUMPOUT|__asm|__spoils|BUG\(\)|__halt|_UNKNOWN|MEMORY\[|'
                 r'__readfsdword|__writefsdword|__outbyte|__inbyte|__CFSHL__|__SEH_|__try|__except')

def type_decl_closure(t, acc):
    """names of user-defined types reachable from t (structs/enums/typedefs)"""
    try:
        if t.is_ptr() or t.is_array():
            type_decl_closure(t.get_pointed_object() if t.is_ptr() else t.get_array_element(), acc); return
        if t.is_func():
            fd = ida_typeinf.func_type_data_t()
            if t.get_func_details(fd):
                type_decl_closure(fd.rettype, acc)
                for a in fd: type_decl_closure(a.type, acc)
            return
        n = t.get_type_name()
        if n and n not in acc and (t.is_struct() or t.is_union() or t.is_enum() or t.is_typedef()):
            acc.add(n)
            if t.is_struct() or t.is_union():
                udt = ida_typeinf.udt_type_data_t()
                if t.get_udt_details(udt):
                    for m in udt: type_decl_closure(m.type, acc)
    except Exception:
        pass

def export_one(ea):
    cf = decomp(ea)
    rec = {'ea': ea, 'name': ida_name.get_name(ea), 'type': idc.get_type(ea) or ''}
    if not cf:
        rec['skip'] = 'decompile failed'; return rec
    text = str(cf)
    rec['text'] = text
    m = BAD.search(text)
    if m: rec['skip'] = 'unsupported construct ' + m.group(0)
    objs = {}; types = set()
    class V(ida_hexrays.ctree_visitor_t):
        def __init__(s): super().__init__(ida_hexrays.CV_FAST)
        def visit_expr(s, e):
            if e.op == ida_hexrays.cot_obj:
                oea = e.obj_ea; nm = ida_name.get_name(oea) or ('unk_%X' % oea)
                is_func = bool(ida_funcs.get_func(oea)) and ida_funcs.get_func(oea).start_ea == oea
                t = ida_typeinf.tinfo_t()
                if not ida_nalt.get_tinfo(t, oea): t = e.type
                objs[nm] = {'ea': oea, 'func': is_func, 'type': str(t),
                            'size': t.get_size() if not is_func else 0,
                            'is_str': bool(ida_bytes.is_strlit(ida_bytes.get_flags(oea)))}
                if is_func:
                    objs[nm]['purged'] = idc.get_func_attr(oea, idc.FUNCATTR_ARGSIZE)
                type_decl_closure(t, types)
            type_decl_closure(e.type, types)
            return 0
    V().apply_to(cf.body, None)
    for lv in cf.get_lvars(): type_decl_closure(lv.type(), types)
    type_decl_closure(cf.type, types)
    rec['objs'] = objs
    rec['types'] = sorted(types)
    rec['purged'] = idc.get_func_attr(ea, idc.FUNCATTR_ARGSIZE)
    fd = ida_typeinf.func_type_data_t(); cf.type.get_func_details(fd)
    rec['cc'] = {ida_typeinf.CM_CC_CDECL: 'cdecl', ida_typeinf.CM_CC_STDCALL: 'stdcall',
                 ida_typeinf.CM_CC_THISCALL: 'thiscall', ida_typeinf.CM_CC_FASTCALL: 'fastcall'}.get(
                     fd.get_cc() & ida_typeinf.CM_CC_MASK, 'other')
    rec['ret'] = str(fd.rettype)
    rec['args'] = [[a.name, str(a.type), a.type.get_size(),
                    (str(a.type.get_pointed_object()) if a.type.is_ptr() else None),
                    (a.type.get_pointed_object().get_size() if a.type.is_ptr() else None)] for a in fd]
    return rec

def export_step(budget=40, out='gfx_export.jsonl'):
    t0 = time.time()
    pos = S.setdefault('exp_pos', 0); T = S['targets']
    with open(os.path.join(D, out), 'a') as f:
        while pos < len(T) and time.time() - t0 < budget:
            f.write(json.dumps(export_one(T[pos])) + '\n'); pos += 1
    S['exp_pos'] = pos
    return {'done': pos, 'of': len(T)}

def export_types(names, out='gfx_types.h'):
    """C declarations for the named types, in dependency order, via IDA's printer."""
    ordl = []
    for n in names:
        o = ida_typeinf.get_type_ordinal(TIL, n)
        if o: ordl.append(o)
    txt = idc.print_decls(','.join(str(o) for o in ordl), ida_typeinf.PDF_INCL_DEPS | ida_typeinf.PDF_DEF_FWD)
    open(os.path.join(D, out), 'w').write(txt or '')
    return len(txt or '')
