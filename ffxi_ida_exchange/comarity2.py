import idc, ida_hexrays as hr, ida_typeinf
def com_mismatch(fea):
    """indirect calls whose callee is a typed vtable member (x->lpVtbl->M) where the member's
    parameter count disagrees with IDA's stack purge at that call"""
    c = hr.decompile(fea); out = []
    class V(hr.ctree_visitor_t):
        def __init__(s): hr.ctree_visitor_t.__init__(s, hr.CV_FAST)
        def visit_expr(s, e):
            if e.op != hr.cot_call: return 0
            x = e.x
            while x.op == hr.cot_cast: x = x.x
            if x.op not in (hr.cot_memptr, hr.cot_memref): return 0
            t = x.type
            if t.is_ptr(): t = t.get_pointed_object()
            fd = ida_typeinf.func_type_data_t()
            if not t.get_func_details(fd): return 0
            want = 4 * fd.size()
            ea = e.ea
            if ea == idc.BADADDR or idc.print_insn_mnem(ea) != 'call':
                return 0
            purge = idc.get_spd(idc.next_head(ea)) - idc.get_spd(ea)
            if purge != want:
                out.append((hex(ea), str(x.type), want, purge, len(e.a)))
            return 0
    V().apply_to(c.body, None)
    return out

def all_known(fea):
    """{call ea: purge} for every indirect call to a typed vtable member in the function"""
    c = hr.decompile(fea); out = {}
    class V(hr.ctree_visitor_t):
        def __init__(s): hr.ctree_visitor_t.__init__(s, hr.CV_FAST)
        def visit_expr(s, e):
            if e.op != hr.cot_call: return 0
            x = e.x
            while x.op == hr.cot_cast: x = x.x
            if x.op not in (hr.cot_memptr, hr.cot_memref): return 0
            t = x.type
            if t.is_ptr(): t = t.get_pointed_object()
            fd = ida_typeinf.func_type_data_t()
            if not t.get_func_details(fd) or (t.get_cc() & ida_typeinf.CM_CC_MASK) != ida_typeinf.CM_CC_STDCALL: return 0
            if e.ea != idc.BADADDR and idc.print_insn_mnem(e.ea) == 'call': out[e.ea] = 4 * fd.size()
            return 0
    V().apply_to(c.body, None)
    return out
