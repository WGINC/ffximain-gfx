"""ida_spfix.py -- repair IDA's stack-pointer tracking from a push-count model (run inside IDA).

IDA tracks SP mostly in address order and guesses the stack purge of indirect (COM/vtable) calls.
When a guess is wrong it compensates elsewhere, so `ret` still balances while individual calls pop
the wrong amount -- Hex-Rays then hands one call's arguments to the next one, or reads saved
registers as arguments. The model here walks the control-flow graph:
  push -4 (pending argument unless it saves a callee-saved register not yet written), pop +4,
  add/sub esp, imm; direct call: the callee's own argsize; indirect call: pops every pending
  argument pushed since the previous call (stdcall COM methods pop all their arguments).
When the model is self-consistent (every join agrees, every ret at SP 0) and IDA disagrees,
align() writes user stack points until IDA's SP equals the model's at every instruction.
"""
import idc, idautils, ida_funcs, ida_gdl, ida_frame, ida_auto, ida_hexrays as hr

SAVED = ('ebx', 'esi', 'edi', 'ebp')

WHY = []

def model(fea, known=None):
    """{insn: sp before it} or None when the function does something the model doesn't cover.
    known: {call ea: purge} for indirect calls whose callee is known (a typed COM method); MSVC
    pushes a later call's arguments before an earlier call, which the pending-push rule misreads"""
    known = known or {}; del WHY[:]
    f = ida_funcs.get_func(fea)
    written, saves = set(), set()
    for h in idautils.FuncItems(fea):
        m = idc.print_insn_mnem(h); o0 = idc.print_operand(h, 0)
        if m == 'push' and o0 in SAVED and o0 not in written: saves.add(h)
        if idc.get_operand_type(h, 0) == 1 and m not in ('push', 'cmp', 'test'): written.add(o0)
    blocks = {b.start_ea: b for b in ida_gdl.FlowChart(f)}
    state = {f.start_ea: (0, 0)}; work = [f.start_ea]; sps = {}
    while work:
        s = work.pop(); b = blocks[s]; sp, pend = state[s]
        for h in idautils.Heads(b.start_ea, b.end_ea):
            sps[h] = sp
            m = idc.print_insn_mnem(h); o0 = idc.print_operand(h, 0)
            if m == 'push':
                sp -= 4
                if h not in saves: pend += 1
            elif m == 'pop': sp += 4
            elif m in ('sub', 'add') and o0 == 'esp':
                if idc.get_operand_type(h, 1) != 5: WHY.append(('esp-nonimm', h)); return None
                v = idc.get_operand_value(h, 1); v = v if v < 0x80000000 else v - 0x100000000
                sp += -v if m == 'sub' else v
                if m == 'add': pend = max(0, pend - v // 4)
            elif m == 'call':
                if idc.get_operand_type(h, 0) in (6, 7):
                    t = idc.get_operand_value(h, 0); g = ida_funcs.get_func(t)
                    if not g or g.start_ea != t: WHY.append(('call-nonfunc', h)); return None
                    p = idc.get_func_attr(t, idc.FUNCATTR_ARGSIZE)
                elif h in known:
                    p = known[h]
                else:
                    p = 4 * pend
                sp += p; pend = max(0, pend - p // 4)
            elif m in ('retn', 'ret'):
                if sp != 0: WHY.append(('ret-sp', h, sp)); return None
            elif m == 'leave' or (o0 == 'esp' and m not in ('push', 'pop', 'cmp', 'test')):
                WHY.append(('esp-other', h)); return None
        for sb in b.succs():
            if sb.start_ea in state:
                if state[sb.start_ea][0] != sp: WHY.append(('join', sb.start_ea, state[sb.start_ea][0], sp)); return None
            else:
                state[sb.start_ea] = (sp, pend); work.append(sb.start_ea)
    return sps

def disagreements(fea, known=None):
    sps = model(fea, known)
    if sps is None: return None
    return [h for h in sorted(sps) if sps[h] != idc.get_spd(h)]

def align(fea, maxit=60, known=None):
    """write user stack points until IDA agrees with the model; returns (points written, left)"""
    f = ida_funcs.get_func(fea); n = 0
    for _ in range(maxit):
        sps = model(fea, known)
        if sps is None: return n, None
        d = [h for h in sorted(sps) if sps[h] != idc.get_spd(h)]
        if not d: break
        h = d[0]
        ida_frame.add_user_stkpnt(h, idc.get_sp_delta(h) + (sps[h] - idc.get_spd(h)))
        ida_auto.plan_and_wait(f.start_ea, f.end_ea); n += 1
    hr.mark_cfunc_dirty(fea)
    return n, len(disagreements(fea, known) or [])

def recreate(fea):
    """delete and re-create a function (drops every user stack point), keeping name, type, comments"""
    f = ida_funcs.get_func(fea); end = f.end_ea
    t = idc.get_type(fea); nm = idc.get_func_name(fea); c1 = idc.get_func_cmt(fea, 1); c0 = idc.get_func_cmt(fea, 0)
    for h in list(idautils.FuncItems(fea)) + [end]: ida_frame.del_stkpnt(f, h)
    ida_funcs.del_func(fea); ida_funcs.add_func(fea, end); ida_auto.auto_wait()
    idc.set_name(fea, nm)
    if t: idc.SetType(fea, t.replace('(', ' f(', 1) + ';')
    if c1: idc.set_func_cmt(fea, c1, 1)
    if c0: idc.set_func_cmt(fea, c0, 0)
    ida_auto.auto_wait()

def solve(fea, known=None, maxcalls=8):
    """when model() fails: find stack purges for the unknown indirect calls that make the function
    consistent (every join agrees, every ret at SP 0), changing as few calls as possible from the
    pop-every-pending-push default. Returns {call: purge} or None (none found / too many calls)."""
    import itertools
    known = dict(known or {})
    if model(fea, known) is not None: return known
    f = ida_funcs.get_func(fea)
    calls = [h for h in idautils.FuncItems(fea) if idc.print_insn_mnem(h) == 'call'
             and idc.get_operand_type(h, 0) not in (6, 7) and h not in known]
    if not calls or len(calls) > maxcalls: return None
    # the pending pushes before each call under the default rule bound its choices
    opts = {h: [4 * k for k in range(0, 9)] for h in calls}
    for n in range(1, len(calls) + 1):
        for sub in itertools.combinations(calls, n):
            for vals in itertools.product(*[opts[h] for h in sub]):
                k = dict(known); k.update(zip(sub, vals))
                if model(fea, k) is not None: return k
    return None
