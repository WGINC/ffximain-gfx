# ida_proto_origin.py -- where the type behind each "typed" call site comes from, as the IDB stores it (run inside
# IDA; read-only: the IDB is not changed). docs/PROBLEMS.md P13, requests/ida_proto_origin.md.
#
# data/callsite_args.json marks a call "typed" when Hex-Rays' callee expression carries a declared type (no cast):
# a named function, a struct member such as lpVtbl->Method, a typed variable, an inlined helper. Some IDB types were
# set by earlier sessions, so each one needs its origin. This script decompiles the functions holding those calls,
# finds each call expression (the same walk as ida/ffxi_ida_fix.py) and records what the IDB says about its type.
#
# Input: data/callsite_args.json (the repo's, found from this file's directory, or SITES below).
# Output: data/ida_proto_origin.json, one entry per "typed" key:
#   {"callee": "0x..." | null,   the called function (a direct call), else null
#    "kind": "function" | "member" | "callsite" | "other",
#            function: the callee function's own prototype; member: a struct member's type ("struct", "member");
#            callsite: a type set on the call instruction itself (IDA's call-type override, which wins);
#            other: anything else, with "what" saying which (import slot, variable, helper, global, ...)
#    "type":  the type as IDA prints it (function: the callee's stored prototype; member: the member's type;
#             callsite: the override; other: the stored type of the object/variable, else Hex-Rays' expression type)
#    "user_set": function/import: ida_nalt.is_userti(ea); callsite: true (an override exists); else null
#    "til":   the base type library the type (struct, or the function's symbol) comes from, when IDA says so
#             (tinfo_t.is_from_subtil), or when the library's symbol of the callee's name prints identically
#    "til_name": a base library that defines a type/symbol of the same name, whatever the local copy says
#    "til_same": whether that library's definition prints identically to the IDB's (null when there is none)
#    "note":  the callee's function comment containing "[type ", if any
#    "op_type": the type IDA stores on the call operand (Edit > Operand type), if any
#    "lvar", "lvar_user_type": kind other, a local variable: its name and whether its type was set by hand
#    "helper": kind other, an inlined helper (memset, strlen, ...): its name
#    "name", "object": the call target's name; kind other, a data target (import slot ...): its address
#    "insn": the instruction at the site, as IDA prints it (without comments)
#    "hexrays": "typed" | "cast" | null, the src this walk finds in today's decompilation; null: no call
#               expression for the site ("why": the function did not decompile, or the call is not there), and
#               the entry holds only what the instruction and the IDB say (a direct target's prototype)}
#   plus "_doc": IDA version, IDB file name, date, the PE header's ImageBase, counts, decompile errors.
#
# Run: File > Script file... > ida_proto_origin.py  (IDA 9.x with the Hex-Rays decompiler)
import datetime, json, os, re, struct
import idaapi, idautils, idc, ida_funcs, ida_hexrays, ida_kernwin, ida_lines, ida_nalt, ida_name, ida_netnode, ida_typeinf

try:
    REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
except NameError:
    REPO = globals().get('REPO') or os.getcwd()
SITES = os.path.join(REPO, 'data', 'callsite_args.json')
OUT = os.path.join(REPO, 'data', 'ida_proto_origin.json')
H = ida_hexrays
IDATI = ida_typeinf.get_idati()
BASES = [IDATI.base(i) for i in range(IDATI.nbases)]
DEF = ida_typeinf.PRTYPE_1LINE | ida_typeinf.PRTYPE_DEF


def hx(ea):
    return None if ea is None or ea == idaapi.BADADDR else '0x%08X' % ea


def pr(tif, name=''):
    s = ida_typeinf.print_tinfo('', 0, 0, ida_typeinf.PRTYPE_1LINE, tif, name, '')
    return s if s is not None else str(tif)


def stored(ea):
    t = ida_typeinf.tinfo_t()
    return t if ida_nalt.get_tinfo(t, ea) else None


def lib_type(name, tif):
    # the base library that defines a type of this name, and whether it prints like the IDB's
    for b in BASES:
        t = ida_typeinf.tinfo_t()
        if t.get_named_type(b, name):
            return b.name, pr(t, name) == pr(tif, name) and \
                (ida_typeinf.print_tinfo('', 0, 0, DEF, t, name, '') == ida_typeinf.print_tinfo('', 0, 0, DEF, tif, name, ''))
    return None, None


def lib_symbol(name, tif):
    # the base library's symbol (function declaration) of this name, and whether it prints like the IDB's type
    for nm in dict.fromkeys([name, re.sub(r'^(__imp_|_+)', '', name or '')]):
        if not nm: continue
        for b in BASES:
            got = ida_typeinf.get_named_type(b, nm, ida_typeinf.NTF_SYMM)
            if got is None: continue
            t = ida_typeinf.tinfo_t()
            if not t.deserialize(b, got[1], got[2]): return b.name, None
            return b.name, tif is not None and pr(t) == pr(tif)
    return None, None


def note_of(ea):
    f = ida_funcs.get_func(ea)
    if not f or f.start_ea != ea: return None
    for rep in (0, 1):
        c = ida_funcs.get_func_cmt(f, rep)
        if c and '[type ' in c: return c
    return None


def op_type(ea):
    t = ida_typeinf.tinfo_t()
    return pr(t) if ida_nalt.get_op_tinfo(t, ea, 0) else None


def override(ea):
    # a call-type override is a type stored on the call instruction; a function start's stored type is the
    # function's own prototype, not an override
    if not idaapi.is_call_insn(ea) and idc.print_insn_mnem(ea) != 'jmp': return None
    f = ida_funcs.get_func(ea)
    if f and f.start_ea == ea: return None
    return stored(ea)


def strip(x):
    while x is not None and x.op in (H.cot_ptr, H.cot_cast, H.cot_ref):
        x = x.x
    return x


def blank():
    return {'callee': None, 'kind': 'other', 'type': None, 'user_set': None, 'til': None, 'til_name': None,
            'til_same': None, 'note': None}


def target(rec, oea, ov):
    # a named call target: a function (its prototype), or a data item such as an import slot (its type)
    f = ida_funcs.get_func(oea); nm = ida_name.get_name(oea); t = stored(oea)
    if f and f.start_ea == oea:
        rec['callee'] = hx(oea)
        if ov is None: rec.update(kind='function', user_set=ida_nalt.is_userti(oea))
        rec['note'] = note_of(oea)
    else:
        rec['what'] = 'import' if idc.get_segm_name(oea) == '.idata' else 'object'
        rec['object'] = hx(oea)
        if ov is None: rec['user_set'] = ida_nalt.is_userti(oea)
    rec['name'] = nm
    if ov is None: rec['type'] = pr(t, '') if t is not None else None
    lib, same = lib_symbol(nm, t)
    rec['til_name'] = lib; rec['til_same'] = same
    if ov is None and same: rec['til'] = lib


def from_insn(ea):
    # no call expression for this site in the decompilation (the function did not decompile, or the call is not
    # there): what the instruction itself and the IDB say
    rec = blank(); rec['hexrays'] = None
    ov = override(ea)
    if ov is not None: rec.update(kind='callsite', type=pr(ov), user_set=True)
    # the target from the xref IDA stores, not insn_t.ops[0].addr: at 0x10002254 (e8 e7010000, a call of
    # 0x10002440) that field reads 0x10002430 in IDA 9.4
    ins = idaapi.insn_t()
    kind = idaapi.decode_insn(ins, ea) and ins.ops[0].type
    code = [t for t in idautils.CodeRefsFrom(ea, 0)]
    data = [t for t in idautils.DataRefsFrom(ea)]
    if kind in (idaapi.o_near, idaapi.o_far) and len(code) == 1:
        target(rec, code[0], ov)
    elif kind == idaapi.o_mem and len(data) == 1:
        target(rec, data[0], ov)
    else:
        rec['what'] = 'not in the decompilation (indirect)'
    rec['why'] = 'not in the decompilation'
    return rec


def classify(cf, e, ea):
    rec = blank()
    x = e.x; src = 'typed'
    while x is not None and x.op in (H.cot_ptr, H.cot_cast):
        if x.op == H.cot_cast: src = 'cast'; break
        x = x.x
    rec['hexrays'] = src
    ov = override(ea)
    if ov is not None:
        rec.update(kind='callsite', type=pr(ov), user_set=True)
    x = strip(e.x)
    if x is None:
        rec['what'] = 'none'
    elif x.op == H.cot_obj:
        target(rec, x.obj_ea, ov)
    elif x.op in (H.cot_memptr, H.cot_memref):
        st = x.x.type
        if x.op == H.cot_memptr and st.is_ptr(): st = st.get_pointed_object()
        sname = st.get_type_name() or str(st)
        u = ida_typeinf.udm_t(); u.offset = x.m * 8
        idx = st.find_udm(u, ida_typeinf.STRMEM_OFFSET)
        rec['struct'] = sname
        rec['member'] = u.name if idx >= 0 else None
        if ov is None:
            rec['kind'] = 'member'
            rec['type'] = pr(u.type) if idx >= 0 else None
        if st.is_from_subtil():
            rec['til'] = st.get_til().name if st.get_til() else None
        named = ida_typeinf.tinfo_t()
        if named.get_named_type(IDATI, sname):
            rec['til_name'], rec['til_same'] = lib_type(sname, named)
    elif x.op == H.cot_var:
        lv = cf.get_lvars()[x.v.idx]
        rec['what'] = 'lvar'; rec['lvar'] = lv.name; rec['lvar_user_type'] = bool(lv.has_user_type)
        if ov is None: rec['type'] = pr(lv.type())
    elif x.op == H.cot_helper:
        rec['what'] = 'helper'; rec['helper'] = x.helper
        if ov is None: rec['type'] = pr(e.x.type)
    else:
        rec['what'] = H.get_ctype_name(x.op)
        if ov is None: rec['type'] = pr(e.x.type)
    return rec


def main():
    sites = json.load(open(SITES))
    want = sorted(int(k, 16) for k, v in sites.items() if v.get('src') == 'typed')
    funcs = {}
    for ea in want:
        f = ida_funcs.get_func(ea)
        funcs.setdefault(f.start_ea if f else None, set()).add(ea)
    out = {}; errs = {}
    for fea, eas in sorted(funcs.items(), key=lambda kv: kv[0] or 0):
        cf = None
        if fea is not None:
            hf = H.hexrays_failure_t()
            try:
                cf = H.decompile(fea, hf, H.DECOMP_NO_WAIT)
            except Exception as ex:
                errs[hx(fea)] = str(ex)
            if not cf and hx(fea) not in errs:
                errs[hx(fea)] = '%s (at %s)' % (hf.desc(), hx(hf.errea))
        found = {}
        if cf:
            class V(H.ctree_visitor_t):
                def __init__(s): super().__init__(H.CV_FAST)
                def visit_expr(s, e):
                    if e.op == H.cot_call and e.ea in eas and e.ea not in found:
                        found[e.ea] = classify(cf, e, e.ea)
                    return 0
            V().apply_to(cf.body, None)
        for ea in eas:
            r = found.get(ea)
            if r is None:
                r = from_insn(ea)
                if fea is not None and hx(fea) in errs: r['why'] = 'the function did not decompile'
            r['op_type'] = op_type(ea)
            r['insn'] = ida_lines.tag_remove(idc.generate_disasm_line(ea, idc.GENDSM_FORCE_CODE) or '').split(';')[0].rstrip()
            out[hx(ea)] = r
    kinds = {}
    for r in out.values(): kinds[r['kind']] = kinds.get(r['kind'], 0) + 1
    hexrays = {}
    for r in out.values(): hexrays[str(r['hexrays'])] = hexrays.get(str(r['hexrays']), 0) + 1
    pe = ida_netnode.netnode('$ PE header').valobj()
    doc = {'pe_imagebase': hx(struct.unpack_from('<I', pe, 0x34)[0]) if pe and len(pe) >= 0x38 else None,
           'hexrays': hexrays,'ida_version': ida_kernwin.get_kernel_version(), 'idb': os.path.basename(idc.get_idb_path()),
           'date': datetime.date.today().isoformat(), 'input_file': ida_nalt.get_root_filename(),
           'base_tils': [b.name for b in BASES], 'local_til': IDATI.name, 'sites': len(out), 'kinds': kinds,
           'functions': len(funcs), 'decompile_errors': errs,
           'source': 'ida/ida_proto_origin.py over data/callsite_args.json (src "typed")'}
    res = {'_doc': doc}
    res.update(sorted(out.items()))
    with open(OUT, 'w') as fh:
        json.dump(res, fh, indent=0, sort_keys=False)
    print('%d sites -> %s %s' % (len(out), OUT, kinds))
    return doc


if __name__ == '__main__':
    main()
