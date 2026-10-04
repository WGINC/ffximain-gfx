# ida_regflow.py -- register data-flow facts Hex-Rays got wrong (run inside IDA; read-only).
# preserves(f, r): r is never written in f or in anything f calls (conservative)
# entry_reads(f): registers f reads before writing (register arguments / LTCG custom conventions)
# after_call_reads(f): (call ea, callee, reg) where f reads a volatile reg right after a call
import idautils, ida_funcs, ida_ua, idc, ida_gdl, ida_idp
ALIAS = {'eax': ('eax','ax','al','ah'), 'ecx': ('ecx','cx','cl','ch'), 'edx': ('edx','dx','dl','dh'),
         'ebx': ('ebx','bx','bl','bh'), 'esi': ('esi','si'), 'edi': ('edi','di'), 'ebp': ('ebp','bp')}
R_OF = {a: r for r, al in ALIAS.items() for a in al}
import re
TOK = re.compile(r'\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?bp)\b')
def uses_defs(ea):
    """(reads, writes) as sets of 32-bit reg names for one instruction"""
    m = idc.print_insn_mnem(ea); insn = ida_ua.insn_t(); ida_ua.decode_insn(insn, ea)
    feat = insn.get_canon_feature(); R = set(); W = set()
    for i in range(3):
        t = idc.get_operand_type(ea, i)
        if t == 0: break
        s = idc.print_operand(ea, i); regs = {R_OF[x] for x in TOK.findall(s) if x in R_OF}
        if t == 1:   # o_reg
            chg = feat & (ida_idp.CF_CHG1 << i) if hasattr(ida_idp, 'CF_CHG1') else (feat & (0x4 << i))
            use = feat & (ida_idp.CF_USE1 << i) if hasattr(ida_idp, 'CF_USE1') else (feat & (0x100 << i))
            name = TOK.findall(s)
            full = bool(name) and name[0].startswith('e')
            if chg: W |= regs
            if use or (chg and not full): R |= regs
        else: R |= regs
    if m in ('xor', 'sub') and idc.print_operand(ea, 0) == idc.print_operand(ea, 1): R -= W
    if m.startswith('rep') or m in ('movsd','movsb','movsw','stosd','stosb','stosw','lodsd','lodsb','scasb','cmpsb','cmpsd','scasd'):
        R |= {'ecx','esi','edi','eax'}; W |= {'ecx','esi','edi'}
    if m in ('loop','loope','loopne','jecxz'): R.add('ecx'); W.add('ecx')
    if m in ('cdq',): R.add('eax'); W.add('edx')
    if m in ('mul','imul','div','idiv') and idc.get_operand_type(ea, 1) == 0:
        R |= {'eax','edx'} if m in ('div','idiv') else {'eax'}; W |= {'eax','edx'}
    if m in ('shl','shr','sar','rol','ror','shld','shrd') and 'cl' in [idc.print_operand(ea, i) for i in range(3)]: R.add('ecx')
    if m == 'leave': W.add('ebp'); R.add('ebp')
    if m == 'call': W |= {'eax','ecx','edx'}
    return R, W
_pres = {}
def callee_of(ea):
    t = idc.get_operand_value(ea, 0)
    return t if idc.get_operand_type(ea, 0) in (6, 7) and ida_funcs.get_func(t) else None
def preserves(fea, reg, depth=0):
    k = (fea, reg)
    if k in _pres: return _pres[k]
    _pres[k] = False                                  # recursion guard: assume not
    f = ida_funcs.get_func(fea)
    if not f or depth > 6: return False
    ok = True
    for ea in idautils.FuncItems(f.start_ea):
        m = idc.print_insn_mnem(ea)
        if m == 'call':
            c = callee_of(ea)
            if c is None or not preserves(c, reg, depth + 1): ok = False; break
            continue
        if m in ('jmp',) and idc.get_operand_type(ea, 0) in (6, 7):
            t = idc.get_operand_value(ea, 0)
            if not (f.start_ea <= t < f.end_ea) and ida_funcs.get_func(t) and ida_funcs.get_func(t).start_ea != f.start_ea:
                if not preserves(ida_funcs.get_func(t).start_ea, reg, depth + 1): ok = False; break
            continue
        if m == 'jmp' and idc.get_operand_type(ea, 0) not in (6, 7): ok = False; break   # indirect tail jump
        _, W = uses_defs(ea)
        if reg in W: ok = False; break
    _pres[k] = ok
    return ok
def _blocks(fea):
    fc = ida_gdl.FlowChart(ida_funcs.get_func(fea)); return fc
def entry_reads(fea):
    """registers read on some path from entry before any write (push/pop of callee-saved = save, not a read)"""
    fc = _blocks(fea); start = None
    for b in fc:
        if b.start_ea == fea: start = b
    if start is None: return set()
    out = set(); seen = set(); stack = [(start, frozenset())]
    while stack:
        b, defined = stack.pop()
        if (b.start_ea, defined) in seen: continue
        seen.add((b.start_ea, defined)); d = set(defined)
        for ea in idautils.Heads(b.start_ea, b.end_ea):
            m = idc.print_insn_mnem(ea)
            R, W = uses_defs(ea)
            if m == 'push' and idc.get_operand_type(ea, 0) == 1: R = set()      # saving it
            if m == 'call':
                c = callee_of(ea)
                # a callee's own register arguments are not ours unless we never set them
                W = {'eax','ecx','edx'} - ({r for r in ('ecx','edx') if c and preserves(c, r)})
            out |= (R - d); d |= W
        for s in b.succs(): stack.append((s, frozenset(d)))
    return out
def after_call_reads(fea):
    res = []
    f = ida_funcs.get_func(fea)
    for b in _blocks(fea):
        hs = list(idautils.Heads(b.start_ea, b.end_ea))
        for i, ea in enumerate(hs):
            if idc.print_insn_mnem(ea) != 'call': continue
            c = callee_of(ea)
            if c is None: continue
            live = {'ecx', 'edx'}
            for ea2 in hs[i + 1:]:
                R, W = uses_defs(ea2)
                for r in list(live):
                    if r in R: res.append((ea, c, r, preserves(c, r))); live.discard(r)
                live -= W
                if not live or idc.print_insn_mnem(ea2) == 'call': break
    return res
