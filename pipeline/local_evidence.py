"""local_evidence.py -- what the machine code does with each Hex-Rays local variable, from the instructions only.

Input: the export (gfx_export.jsonl) and the owner's IDA map of its variables (gfx_lvars.jsonl, ida/ida_lvars.py).
A record is used only when its `text_sha1` equals the sha1 of the export's text for that function: the same IDB
state, so the same v-numbers; any other record is listed as `stale` and not used.

Where each variable is:
  * a stack variable: IDA's frame offset `stk_ida` (from frame bottom: locals, saved registers, return address,
    arguments), so its address is entry ESP + stk_ida - frsize - frregs. Checked on the export: in every one of
    the 483 functions with stack arguments the lowest one is at entry ESP + 4 (`frame_check`).
    An instruction's memory operand [esp+d] addresses entry ESP - depth + d, [ebp+d] entry ESP - ebp_depth + d,
    with the depths exact from sp_solve.insn_depths (every pop on the way from a definitive source) or, where
    those are not exact, observed by running the original (data/real_sp.json `tdepth`: paths on which every
    call before was real game code; not in a function that aligns ESP, where the observed depth includes the
    run's padding and IDA's frame offsets do not). An address where the two disagree is left out (`depth_conflicts`); an
    instruction with neither gives no stack evidence (`_depth_cover` counts them). EBP-based operands need
    sp_solve's EBP depth.
    Every instruction of the function whose operand overlaps the variable is an access, not only the ones
    Hex-Rays attributes uses to.
  * a register variable: only the instructions Hex-Rays attributes its uses to (the register's live range is not
    in the map). An instruction counts only if the variable's register appears in it.

What each access is (the instruction says it, nothing is inferred from names or the pseudocode):
  f32/f64/f80   an x87 float load, store or operation on that many bits
  i16/i32/i64   an x87 integer load/store (fild/fist)
  sext/zext     a narrow read sign- or zero-extended (movsx/movzx)
  mov           a copy (mov/push/pop): says the width, not the type
  addr          its address taken (lea): later uses through that address are not seen
  op:<mnem>     an integer instruction (cmp, add, sar, imul, ...)
For a pointer-shaped variable, each dereference through it (the variable's register, or a register just loaded
from its stack slot, used as the base of a memory operand in the same straight-line run) is recorded with the
offset and the access kind of the pointee.

A stack operand at an instruction with no known depth could be any variable's: its address is listed in `blind`.
Output {"0xFUNC": {"name", "blind": [...], "sp_differs", "vars": {name: {"type", "loc", "acc": [[ea, kind, size, off, idx]], "deref": [[ea,
kind, size, off]]}}}, "_stale": [...], "_no_depths": [...]}
Usage: python3 local_evidence.py EXPORT.jsonl LVARS.jsonl OUT.json     (uses $FFXI_DLL)"""
import bisect, hashlib, json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from capstone.x86 import X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
FAM = {}
for big, subs in (('eax', 'ax al ah'), ('ebx', 'bx bl bh'), ('ecx', 'cx cl ch'), ('edx', 'dx dl dh'),
                  ('esi', 'si'), ('edi', 'di'), ('ebp', 'bp'), ('esp', 'sp')):
    FAM[big] = big
    for s in subs.split(): FAM[s] = big
X87_INT = ('fild', 'fist', 'fisttp', 'fiadd', 'fisub', 'fimul', 'fidiv', 'ficom')


def kind(i, size):
    m = i.mnemonic
    if m.startswith(X87_INT): return 'i%d' % (size * 8)
    if m in ('fnstcw', 'fldcw', 'fstcw', 'fnstsw', 'fstsw'): return 'x87ctl'
    if m.startswith('f'): return 'f%d' % (size * 8)
    if m == 'lea': return 'addr'
    if m == 'movsx': return 'sext'
    if m == 'movzx': return 'zext'
    if m in ('mov', 'push', 'pop'): return 'mov'
    return 'op:' + m


def writes(i, r):
    """does instruction i write register family r"""
    try:
        _, w = i.regs_access()
    except Exception:
        return True
    return any(FAM.get(i.reg_name(x)) == r for x in w)


def main(exp, lvars, out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json'))); purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = {int(k, 16) for k in ext}; iat = S.imports(im, os.environ['FFXI_DLL']); nr = S.noreturn_funcs(im, ext, iat)
    R = {}
    for l in open(exp):
        r = json.loads(l); R[r['ea']] = r
    RS = json.load(open(os.path.join(D, 'real_sp.json')))
    res = {'_stale': [], '_no_depths': [], '_frame_check': {}, '_depth_cover': [0, 0, 0], '_depth_conflicts': [], '_ida_sp_differs': {}}
    fc = res['_frame_check']
    for l in open(lvars):
        x = json.loads(l); r = R.get(x['ea'])
        if not r or 'lvars' not in x: continue
        if hashlib.sha1(r['text'].encode()).hexdigest() != x.get('text_sha1'):
            res['_stale'].append('0x%08X' % x['ea']); continue
        base = x['frsize'] + x['frregs']
        a = [v['loc']['stk_ida'] for v in x['lvars'] if v['arg'] and 'stk_ida' in v['loc']]
        if a: fc[str(min(a) - base)] = fc.get(str(min(a) - base), 0) + 1
        e = ext.get('0x%x' % x['ea'])
        if not e: continue
        ins = im.code(e); order = sorted(ins)
        dep = S.insn_depths(im, x['ea'], e, purges, starts, nr, iat)
        obs = {int(k, 16): v for k, v in RS.get('0x%08X' % x['ea'], {}).get('tdepth', {}).items()}
        if any(ins[a].mnemonic == 'and' and ins[a].op_str.startswith('esp,') for a in order):
            obs = {}        # `and esp, -N`: an observed depth includes that run's padding, IDA's frame does not
        for ad, v in obs.items():
            if ad in dep and dep[ad][0] != v:
                res['_depth_conflicts'].append('0x%08X' % ad); del dep[ad]
            elif ad not in dep and ad in ins: dep[ad] = (v, None)
        # Hex-Rays names stack slots from IDA's SP: where that differs from the exact depth, its variables are
        # not the slots they claim to be, so the function's stack variables are not used
        pts = sorted(r.get('spd') or []); keys = [a for a, _ in pts]
        ida = lambda a: -pts[bisect.bisect_right(keys, a) - 1][1] if bisect.bisect_right(keys, a) else 0
        off = [a for a in order if a in dep and pts and ida(a) != dep[a][0]]
        if off: res['_ida_sp_differs']['0x%08X' % x['ea']] = ['0x%08X' % off[0], len(off)]
        cov = res['_depth_cover']; cov[0] += len(order); cov[1] += sum(1 for a in order if a in dep); cov[2] += sum(1 for a in obs if a in ins)
        if not dep: res['_no_depths'].append('0x%08X' % x['ea'])
        # every stack memory operand: (entry-relative address, size, indexed)
        mem = {}; blind = []      # blind: stack operands whose address is not known (no exact depth there)
        for ad in order:
            i = ins[ad]
            d, ebpd = dep.get(ad, (None, None))
            for o in i.operands:
                if o.type != X86_OP_MEM: continue
                b = i.reg_name(o.mem.base) if o.mem.base else None
                if b == 'esp' and d is not None: at = -d + o.mem.disp
                elif b == 'ebp' and ebpd is not None: at = -ebpd + o.mem.disp
                elif b in ('esp', 'ebp'): blind.append('0x%08X' % ad); continue
                else: continue
                mem.setdefault(ad, []).append((at, o.size, bool(o.mem.index)))
        vars_ = {}
        for v in x['lvars']:
            if not v['name']: continue
            rec = {'type': v['type'], 'arg': v['arg'], 'width': v['width'], 'loc': v['loc'], 'acc': [], 'deref': []}
            regs = []                                   # (ea, register) a pointer value is in, to follow
            if 'stk_ida' in v['loc'] and not off:
                lo = v['loc']['stk_ida'] - base; hi = lo + max(v['width'], 1)
                for ad, ms in mem.items():
                    for at, sz, idx in ms:
                        if lo <= at < hi:
                            i = ins[ad]; k = kind(i, sz)
                            rec['acc'].append([ad, k, sz, at - lo, idx])
                            if i.mnemonic == 'mov' and i.operands[0].type == X86_OP_REG and sz == 4 and at == lo:
                                regs.append((ad, FAM.get(i.reg_name(i.operands[0].reg))))
            elif 'reg' in v['loc']:
                rr = FAM.get(v['loc']['reg'])
                for ea in sorted({u[0] for u in v['uses'] if u[0] is not None}):
                    if ea not in ins: continue
                    i = ins[ea]
                    names = set()
                    for o in i.operands:
                        if o.type == X86_OP_REG: names.add(FAM.get(i.reg_name(o.reg)))
                        if o.type == X86_OP_MEM:
                            if o.mem.base: names.add(FAM.get(i.reg_name(o.mem.base)))
                            if o.mem.index: names.add(FAM.get(i.reg_name(o.mem.index)))
                    if rr in names:
                        rec['acc'].append([ea, 'reg', 0, 0, False])
                        # a dereference at the use itself
                        for o in i.operands:
                            if o.type == X86_OP_MEM and o.mem.base and FAM.get(i.reg_name(o.mem.base)) == rr and not o.mem.index:
                                rec['deref'].append([ea, kind(i, o.size), o.size, o.mem.disp])
            # follow a pointer loaded from the slot through the straight-line run after the load
            for ad, r0 in regs:
                if not r0: continue
                k = order.index(ad) + 1
                while k < len(order):
                    i = ins[order[k]]
                    for o in i.operands:
                        if o.type == X86_OP_MEM and o.mem.base and FAM.get(i.reg_name(o.mem.base)) == r0 and not o.mem.index:
                            rec['deref'].append([i.address, kind(i, o.size), o.size, o.mem.disp])
                    if writes(i, r0) or i.mnemonic in ('call', 'ret', 'jmp') or i.mnemonic.startswith('j'): break
                    if k + 1 < len(order) and order[k + 1] != i.address + i.size: break
                    k += 1
            vars_[v['name']] = rec
        res['0x%08X' % x['ea']] = {'name': r['name'], 'vars': vars_, 'blind': blind, 'sp_differs': bool(off)}
    json.dump(res, open(out, 'w'))
    n = sum(len(v['vars']) for k, v in res.items() if not k.startswith('_'))
    print('%d functions, %d variables; %d stale records, %d functions without exact depths; frame check %s; '
          'instructions %d, with a depth %d (observed %d), conflicts %d; the IDA SP differs from it in %d functions' % (
        sum(1 for k in res if not k.startswith('_')), n, len(res['_stale']), len(res['_no_depths']), fc,
        *res['_depth_cover'], len(res['_depth_conflicts']), len(res['_ida_sp_differs'])))


if __name__ == '__main__':
    main(*sys.argv[1:4])
