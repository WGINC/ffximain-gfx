# ida_lvars.py -- where each Hex-Rays local variable lives, and the instructions that use it (run inside IDA;
# read-only: the IDB is not changed).
#
# The export (gfx_export.jsonl) has the pseudocode but no map from its variables (v12, a3) to the machine code,
# so a local's type cannot be checked against the instructions that use it. This script writes that map.
#
# Input: gfx_export.jsonl next to the IDB (the functions to cover are the ones listed there).
# Output: gfx_lvars.jsonl next to the IDB, one line per function:
#   {"ea", "name", "text_sha1",           sha1 of the pseudocode decompiled now: the pipeline uses the record
#                                         only if it equals the export's text (same IDB state, same v-numbers)
#    "frsize", "frregs", "argsize",       IDA's frame: local bytes, saved-register bytes, argument bytes
#    "lvars": [{"idx", "name", "type", "width", "arg", "result",
#               "loc": {"stk_vd": n, "stk_ida": n} | {"reg": "esi", "reg2": "edi"?} | {"scattered": true} | {},
#               "uses": [[ea, kind, text], ...]}]}
#   kind: "w" written (x = ...), "rw" read and written (x += ..., ++x), "addr" its address taken (&x), "r" read.
#   ea: the instruction Hex-Rays attributes the use to (the use itself, or the nearest enclosing item that
#   has an address); text: that enclosing expression as printed, cut at 120 characters.
#
# Run: File > Script file... > ida_lvars.py  (IDA 9.x with the Hex-Rays decompiler)
import hashlib, json, os
import idaapi, idc, ida_funcs, ida_hexrays, ida_lines, ida_name

OUT_DIR = os.path.dirname(idc.get_idb_path()) or os.getcwd()
H = ida_hexrays
WRITE = {H.cot_asg}
RW = {H.cot_asgbor, H.cot_asgxor, H.cot_asgband, H.cot_asgadd, H.cot_asgsub, H.cot_asgmul, H.cot_asgsshr,
      H.cot_asgushr, H.cot_asgshl, H.cot_asgsdiv, H.cot_asgudiv, H.cot_asgsmod, H.cot_asgumod,
      H.cot_postinc, H.cot_postdec, H.cot_preinc, H.cot_predec}


def same(a, b):
    try:
        return a.obj_id == b.obj_id
    except Exception:
        return a is b


def loc_of(cf, lv):
    L = lv.location
    try:
        if L.is_stkoff():
            vd = L.stkoff()
            return {'stk_vd': vd, 'stk_ida': cf.mba.stkoff_vd2ida(vd)}
        if L.is_reg1():
            d = {'reg': H.get_mreg_name(L.reg1(), lv.width)}
            if L.is_reg2(): d['reg2'] = H.get_mreg_name(L.reg2(), lv.width)
            return d
        if L.is_scattered(): return {'scattered': True}
    except Exception as e:
        return {'error': str(e)}
    return {}


def one(ea):
    rec = {'ea': ea, 'name': ida_name.get_name(ea)}
    try:
        cf = H.decompile(ea, flags=H.DECOMP_NO_CACHE)
    except Exception as e:
        cf = None; rec['error'] = str(e)
    if not cf:
        rec['skip'] = 'decompile failed'; return rec
    rec['text_sha1'] = hashlib.sha1(str(cf).encode('utf-8')).hexdigest()
    rec['frsize'] = idc.get_func_attr(ea, idc.FUNCATTR_FRSIZE)
    rec['frregs'] = idc.get_func_attr(ea, idc.FUNCATTR_FRREGS)
    rec['argsize'] = idc.get_func_attr(ea, idc.FUNCATTR_ARGSIZE)
    lvars = cf.get_lvars()
    uses = {i: [] for i in range(lvars.size())}

    class V(H.ctree_visitor_t):
        def __init__(s): super().__init__(H.CV_PARENTS)

        def visit_expr(s, e):
            if e.op != H.cot_var: return 0
            p = None                       # the enclosing expression (None when the variable is a whole statement)
            if len(s.parents):
                it = s.parents[len(s.parents) - 1]
                if it.is_expr(): p = it.cexpr
            kind = 'r'
            if p is not None:
                if p.op in WRITE and p.x is not None and same(p.x, e): kind = 'w'
                elif p.op in RW and p.x is not None and same(p.x, e): kind = 'rw'
                elif p.op == H.cot_ref: kind = 'addr'
            at, item = e.ea, e
            if at == idaapi.BADADDR:
                for i in range(len(s.parents) - 1, -1, -1):
                    if s.parents[i].ea != idaapi.BADADDR:
                        at, item = s.parents[i].ea, s.parents[i]; break
            src = p if p is not None else e
            try: text = ida_lines.tag_remove(src.print1(None))[:120]
            except Exception: text = ''
            uses.setdefault(e.v.idx, []).append([at if at != idaapi.BADADDR else None, kind, text])
            return 0

    V().apply_to(cf.body, None)
    rec['lvars'] = [{'idx': i, 'name': lv.name, 'type': str(lv.type()), 'width': lv.width,
                     'arg': bool(lv.is_arg_var), 'result': bool(lv.is_result_var),
                     'loc': loc_of(cf, lv), 'uses': uses.get(i, [])} for i, lv in enumerate(lvars)]
    return rec


def main():
    if not H.init_hexrays_plugin():
        print('ida_lvars: the Hex-Rays decompiler is not available'); return
    src = os.path.join(OUT_DIR, 'gfx_export.jsonl')
    eas = [json.loads(l)['ea'] for l in open(src, encoding='utf-8')]
    out = os.path.join(OUT_DIR, 'gfx_lvars.jsonl'); n_var = n_use = n_skip = 0
    with open(out, 'w', encoding='utf-8') as f:
        for k, ea in enumerate(eas):
            r = one(ea)
            n_skip += 'skip' in r
            for lv in r.get('lvars', ()): n_var += 1; n_use += len(lv['uses'])
            f.write(json.dumps(r) + '\n')
            if k % 100 == 0: print('ida_lvars: %d of %d' % (k, len(eas)))
    print('ida_lvars: %d functions, %d variables, %d uses, %d not decompiled -> %s' % (len(eas), n_var, n_use, n_skip, out))


main()
