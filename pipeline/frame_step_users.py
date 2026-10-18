"""frame_step_users.py -- where the game's frame step goes, from each call of its two getters (docs/RENDERER.md 3a).

    FFXI_DLL=... python3 frame_step_users.py [--out data/frame_step_users.json] [--header kit/speedfix/sf_step_sites.h]
                                             [--show SITE]

Camera_GetFrameScale (0x10014CF0) and its copy 0x10014D20 return max([g_pCamera+0x28], 1.0) on the x87 stack: the
frame step in 1/60 s ticks, always a whole number in the game's own code. Above 60 fps the honest step is less than
one tick. This follows the returned value through the caller's code, from the return address, and answers for each
call site whether a fractional step would survive there:

  fraction   every path ends with the value (or what was computed from it) compared, dropped, or stored as a float
             into a location that was itself an input of that value (an accumulator: `x = x + f(step)`, `x -= step`).
  whole      some path converts it to an integer (_ftol 0x10311C2C, fist, fistp), stores it as a float somewhere it
             was not computed from, passes it to a call, returns it, or reaches code this does not model.
             The reason is recorded. A fractional step is not established safe there; whole ticks are what the game
             gives everywhere.

The step a function is given is one value: a function gets the fraction only if every one of its call sites is
fraction (`answer`); a function whose sites differ gets whole ticks at all of them, so that it never sees two
different steps in one frame. `fraction_sites` lists the call sites answered with the fraction; --header writes them
(their return addresses) for speedfix's frame_step=2 (kit/speedfix/sf_main.cpp).

It also records where the value (as float or after _ftol, as an integer) is a divisor: whole ticks bring the value 0
to every site (a frame on which no tick has passed), which the game's code never produced (max(step, 1)).

The x87 stack is modelled exactly for the instructions MSVC emits here; anything else while the value is live ends
that path as `unmodelled` (whole). Locals are [esp+d] (by the esp the path has tracked, callee pops from
data/purges.json) and [ebp+d]. Nothing here is a guess: a path that cannot be followed exactly is not counted as
safe.
"""
import argparse, json, os, re, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone import x86_const as X

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
GETTERS = (0x10014CF0, 0x10014D20)
FTOL = 0x10311C2C
INLINE = 0x10011A24          # Frame_Render's own copy of the getter: fld [esi+0x28] ... at 0x10011A24..0x10011A3F
MAXSTEPS = 4000

pe = img = base = md = PURGE = None


class St:
    """one path's state. x87: list of entries, top last; each None (not from the step) or a frozenset of the memory
    keys that went into it (the step's own value: an entry that is not None). Below the modelled entries the stack
    holds values not from the step. mem: tainted locals -> lineage. ints: registers holding an integer from the step.
    ver: register write counters (memory keys made with a register carry its version)."""
    __slots__ = ('x87', 'mem', 'ints', 'esp', 'ver')

    def __init__(s):
        s.x87 = []; s.mem = {}; s.ints = set(); s.esp = 0; s.ver = {}

    def copy(s):
        n = St(); n.x87 = list(s.x87); n.mem = dict(s.mem); n.ints = set(s.ints); n.esp = s.esp; n.ver = dict(s.ver)
        return n

    def live(s):
        return any(e is not None for e in s.x87) or s.mem or s.ints

    def key(s):
        return (tuple(s.x87), tuple(sorted(s.mem.items(), key=str)), tuple(sorted(s.ints)), s.esp, tuple(sorted(s.ver.items())))


def reg(r):
    return md.reg_name(r) if r else ''


FULL = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
        'dl': 'edx', 'dh': 'edx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}


def full(r):
    return FULL.get(r, r)


def memkey(st, op):
    """a stable name for a memory operand: locals by their entry-relative offset, others by text and register versions"""
    m = op.mem; b, i = full(reg(m.base)), full(reg(m.index))
    if b == 'esp' and not i: return ('esp', st.esp + m.disp)
    if b == 'ebp' and not i: return ('ebp', m.disp, st.ver.get('ebp', 0))
    return ('m', b, st.ver.get(b, 0), i, st.ver.get(i, 0), m.scale, m.disp, op.size)


def is_local(k):
    return k[0] in ('esp', 'ebp')


def sti(op):
    """st(i) index of a register operand"""
    n = reg(op.reg)
    m = re.match(r'st\((\d)\)', n)
    if m: return int(m.group(1))
    if n == 'st0' or n == 'st(0)': return 0
    raise ValueError(n)


def get(st, i):
    return st.x87[-1 - i] if i < len(st.x87) else None


def setv(st, i, v):
    while i >= len(st.x87): st.x87.insert(0, None)
    st.x87[-1 - i] = v


def pop(st):
    return st.x87.pop() if st.x87 else None


def push(st, v):
    st.x87.append(v)


def join(a, b, extra=()):
    if a is None and b is None: return None
    s = set(extra)
    if a is not None: s |= a
    if b is not None: s |= b
    return frozenset(s)


def written_regs(i):
    try:
        _, w = i.regs_access()
    except Exception:
        return None
    return {full(reg(r)) for r in w}


def follow(site, ret):
    out = []          # (kind, address, detail)
    divs = []
    work = [(ret, None)]
    seen = set()
    steps = 0
    while work:
        a, st = work.pop()
        if st is None:
            st = St(); push(st, frozenset())
        while True:
            steps += 1
            if steps > MAXSTEPS:
                out.append(('unmodelled', a, 'step limit')); work = []; break
            if not st.live(): break
            k = (a, st.key())
            if k in seen: break
            seen.add(k)
            off = a - base
            ins = next(md.disasm(img[off:off + 16], a, 1), None)
            if ins is None:
                out.append(('unmodelled', a, 'no instruction')); break
            mn = ins.mnemonic; ops = ins.operands; nxt = a + ins.size
            # --- control flow
            if mn == 'ret':
                if get(st, 0) is not None: out.append(('returned', a, ''))
                elif st.ints & {'eax'}: out.append(('returned-int', a, ''))
                break
            if mn == 'jmp':
                if ops[0].type == X.X86_OP_IMM: a = ops[0].imm; continue
                out.append(('unmodelled', a, 'indirect jmp')); break
            if mn.startswith('j') or mn.startswith('loop'):
                if ops[0].type != X.X86_OP_IMM: out.append(('unmodelled', a, mn)); break
                work.append((ops[0].imm, st.copy())); a = nxt; continue
            if mn == 'call':
                tgt = ops[0].imm if ops[0].type == X.X86_OP_IMM else None
                if tgt == FTOL:
                    v = pop(st)
                    if v is not None: out.append(('int', a, '_ftol')); st.ints.add('eax')
                    else: st.ints.discard('eax')
                    st.ints.discard('edx')                  # _ftol returns a 64-bit value in edx:eax
                    st.ver['eax'] = st.ver.get('eax', 0) + 1; st.ver['edx'] = st.ver.get('edx', 0) + 1
                    a = nxt; continue
                if any(e is not None for e in st.x87):
                    out.append(('call-with-x87', a, '%X' % tgt if tgt else 'indirect')); break
                args = [kk for kk in st.mem if kk[0] == 'esp' and st.esp <= kk[1] < st.esp + 0x40]
                if args: out.append(('call-arg', a, '%X' % tgt if tgt else 'indirect')); break
                p = PURGE.get('0x%08X' % tgt) if tgt else None
                if p is None:
                    if st.mem or st.ints - {'eax', 'ecx', 'edx'}:
                        out.append(('unmodelled', a, 'callee pop unknown (%s)' % ('%X' % tgt if tgt else 'indirect'))); break
                    p = 0
                st.esp += p
                for r in ('eax', 'ecx', 'edx'):
                    st.ints.discard(r); st.ver[r] = st.ver.get(r, 0) + 1
                st.x87 = []
                a = nxt; continue
            if mn in ('int3', 'hlt', 'ud2'): break
            # --- x87
            if mn.startswith('f'):
                r = x87(st, ins, out, divs)
                if r == 'stop': break
                a = nxt; continue
            # --- integer side
            w = written_regs(ins)
            if w is None: out.append(('unmodelled', a, mn)); break
            src_taint = False
            for o in ops:
                if o.type == X.X86_OP_REG and full(reg(o.reg)) in st.ints and not (o.access & 2 and not o.access & 1 and mn in ('mov', 'lea', 'movzx', 'movsx')):
                    if o.access & 1: src_taint = True
                if o.type == X.X86_OP_MEM:
                    if any(full(reg(rr)) in st.ints for rr in (o.mem.base, o.mem.index) if rr): pass  # address arithmetic
            if mn in ('idiv', 'div'):
                o = ops[0]
                if (o.type == X.X86_OP_REG and full(reg(o.reg)) in st.ints) or (o.type == X.X86_OP_MEM and memkey(st, o) in st.mem):
                    divs.append(('int-divisor', a))
            if mn == 'push':
                st.esp -= 4
                o = ops[0]
                if o.type == X.X86_OP_REG and full(reg(o.reg)) in st.ints: st.mem[('esp', st.esp)] = 'int'
                else: st.mem.pop(('esp', st.esp), None)
                a = nxt; continue
            if mn == 'pop':
                k = ('esp', st.esp); t = st.mem.pop(k, None); st.esp += 4
                rr = full(reg(ops[0].reg)) if ops[0].type == X.X86_OP_REG else None
                if rr:
                    st.ver[rr] = st.ver.get(rr, 0) + 1
                    (st.ints.add if t == 'int' else st.ints.discard)(rr)
                a = nxt; continue
            if 'esp' in w:
                if mn in ('sub', 'add') and ops[1].type == X.X86_OP_IMM:
                    st.esp += -ops[1].imm if mn == 'sub' else ops[1].imm
                    a = nxt; continue
                out.append(('unmodelled', a, 'esp: %s %s' % (mn, ins.op_str))); break
            # memory destinations: an integer from the step stored, or a tainted local overwritten
            for o in ops:
                if o.type == X.X86_OP_MEM and o.access & 2:
                    k = memkey(st, o)
                    if src_taint: st.mem[k] = 'int' if is_local(k) else st.mem.get(k)
                    else: st.mem.pop(k, None)
            # register destinations
            for rr in w:
                if rr in ('eflags', 'esp', ''): continue
                st.ver[rr] = st.ver.get(rr, 0) + 1
                if src_taint and mn not in ('cmp', 'test'): st.ints.add(rr)
                elif mn == 'mov' and ops[1].type == X.X86_OP_MEM and st.mem.get(memkey(st, ops[1])) == 'int': st.ints.add(rr)
                elif not (mn in ('add', 'sub', 'imul', 'shl', 'sar', 'shr', 'and', 'or', 'neg', 'inc', 'dec', 'adc', 'sbb') and rr in st.ints):
                    st.ints.discard(rr)
            for k in [k for k in st.mem if k[0] == 'm' and (k[1] in w or k[3] in w)]: st.mem.pop(k)
            a = nxt
    return out, divs


def x87(st, ins, out, divs):
    mn = ins.mnemonic; ops = ins.operands; a = ins.address
    mems = [o for o in ops if o.type == X.X86_OP_MEM]
    m = mems[0] if mems else None
    mk = memkey(st, m) if m else None
    mval = None
    if m is not None and m.access & 1:
        t = st.mem.get(mk)
        mval = (t if t != 'int' else frozenset()) if t is not None else None
    if mn in ('fnstsw', 'fstsw', 'fldcw', 'fnstcw', 'fstcw', 'fwait', 'wait', 'fnclex', 'fclex', 'fnop'):
        if mn in ('fnstsw', 'fstsw', 'fnstcw', 'fstcw') and m is not None: st.mem.pop(mk, None)
        if mn in ('fnstsw', 'fstsw') and not m: st.ints.discard('eax'); st.ver['eax'] = st.ver.get('eax', 0) + 1
        return
    if mn in ('fld', 'fild'):
        if m is not None:              # a plain load keeps its location, for the lineage when it meets the step
            push(st, mval if mval is not None else ('L', mk))
        else:
            push(st, get(st, sti(ops[0])))
        return
    if mn in ('fld1', 'fldz', 'fldpi', 'fldl2e', 'fldln2', 'fldlg2', 'fldl2t'):
        push(st, None); return
    if mn in ('fst', 'fstp', 'fist', 'fistp', 'fisttp'):
        v = get(st, 0)
        v = None if isinstance(v, tuple) else v
        if m is not None:
            if v is not None:
                if mn.startswith('fi'): out.append(('int', a, mn))
                elif is_local(mk): st.mem[mk] = v
                elif mk in v: out.append(('accumulator', a, ins.op_str))
                else: out.append(('float-store', a, ins.op_str))
            else:
                st.mem.pop(mk, None)
        else:
            setv(st, sti(ops[0]), get(st, 0))
        if mn.endswith('p'): pop(st)
        return
    if mn == 'fxch':
        i = sti(ops[0]) if ops else 1
        t0, ti = get(st, 0), get(st, i); setv(st, 0, ti); setv(st, i, t0); return
    if mn in ('fchs', 'fabs', 'fsqrt', 'frndint', 'fsin', 'fcos', 'f2xm1', 'fscale'):
        if mn == 'frndint' and val(get(st, 0)) is not None: out.append(('int', a, 'frndint'))
        return
    if mn in ('fcom', 'fcomp', 'fucom', 'fucomp', 'ficom', 'ficomp', 'ftst', 'fxam'):
        if mn.endswith('p'): pop(st)
        return
    if mn in ('fcompp', 'fucompp'):
        pop(st); pop(st); return
    if mn in ('fcomi', 'fucomi', 'fcomip', 'fucomip'):
        if mn.endswith('p'): pop(st)
        return
    if mn in ('ffree',):
        setv(st, sti(ops[0]), None); return
    if mn in ('fpatan', 'fyl2x', 'fyl2xp1'):
        v = join(val(get(st, 0)), val(get(st, 1))); pop(st); setv(st, 0, v); return
    ar = re.match(r'fi?(add|sub|subr|mul|div|divr)(p?)$', mn)
    if ar:
        op, p = ar.group(1), ar.group(2)
        if m is not None:                     # st0 = st0 op m
            v0 = val(get(st, 0)); lin0 = lin(get(st, 0))
            if op in ('div',) and mval is not None: divs.append(('float-divisor', a))
            if op in ('divr',) and v0 is not None: divs.append(('float-divisor', a))
            if v0 is None and mval is None: setv(st, 0, None); return
            setv(st, 0, join(v0, mval, [mk] + lin0)); return
        regs = [sti(o) for o in ops if o.type == X.X86_OP_REG]
        if not regs: regs = [1, 0]           # fsubp / faddp with no operands: st(1) = st(1) op st0, pop
        if len(regs) == 1: regs = [regs[0], 0] if p else [0, regs[0]]
        dst, src = regs
        vd, vs = val(get(st, dst)), val(get(st, src))
        if op.startswith('div') and (vd is not None or vs is not None): divs.append(('float-divisor?', a))
        nv = join(vd, vs, lin(get(st, dst)) + lin(get(st, src))) if (vd is not None or vs is not None) else None
        setv(st, dst, nv)
        if p: pop(st)
        return
    if get(st, 0) is not None or any(val(e) is not None for e in st.x87):
        out.append(('unmodelled', a, '%s %s' % (mn, ins.op_str))); return 'stop'


def val(e):
    """an entry's step taint: None (not from the step) or its lineage set; ('L', key) is a plain load"""
    if e is None or isinstance(e, tuple): return None
    return e


def lin(e):
    return [e[1]] if isinstance(e, tuple) and e[1] is not None else []


def sites():
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    res = []
    for f, chunks in ext.items():
        for s, e in chunks:
            for i in md.disasm(img[s - base:e - base], s):
                if i.mnemonic == 'call' and i.operands[0].type == X.X86_OP_IMM and i.operands[0].imm in GETTERS:
                    res.append((int(f, 16), i.address, i.operands[0].imm, i.address + i.size))
    return sorted(res, key=lambda r: r[1])


def classify(out):
    kinds = {k for k, _, _ in out}
    return 'fraction' if kinds <= {'accumulator'} else 'whole'


def main():
    global pe, img, base, md, PURGE
    ap = argparse.ArgumentParser(); ap.add_argument('--out'); ap.add_argument('--header'); ap.add_argument('--show')
    a = ap.parse_args()
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    PURGE = {k.upper().replace('0X', '0x'): v for k, v in json.load(open(os.path.join(D, 'purges.json'))).items()}
    res = {}
    for f, site, g, ret in sites():
        out, divs = follow(site, ret)
        c = classify(out)
        res['0x%08X' % site] = {'func': '0x%08X' % f, 'getter': '0x%08X' % g, 'class': c,
                                'ends': sorted({'%s %08X %s' % (k, x, d) for k, x, d in out}),
                                'divisors': sorted({'%s %08X' % (k, x) for k, x in divs})}
        if a.show and int(a.show, 16) == site: print(json.dumps(res['0x%08X' % site], indent=1))
    mixed = {v['func'] for v in res.values() if v['class'] != 'fraction'}
    for v in res.values(): v['answer'] = 'fraction' if v['func'] not in mixed else 'whole'
    fsites = sorted(s for s, v in res.items() if v['answer'] == 'fraction')
    n = len(res); fr = sum(1 for v in res.values() if v['class'] == 'fraction')
    from collections import Counter
    why = Counter(e.split()[0] for v in res.values() if v['class'] == 'whole' for e in v['ends'])
    dv = [(s, v['divisors']) for s, v in res.items() if v['divisors']]
    print('%d call sites of the two getters: %d fraction, %d whole' % (n, fr, n - fr))
    print('whole, by what ends a path (a site can have several):', dict(why))
    print('sites where the step (or its integer) is a divisor:', len(dv))
    for s, d in dv: print('  ', s, res[s]['func'], d)
    print('answered with the fraction (every site of the function fraction): %d sites in %d functions'
          % (len(fsites), len({res[s]['func'] for s in fsites})))
    if a.out:
        json.dump({'source': 'pipeline/frame_step_users.py on the analysed build', 'inline': '0x%08X' % INLINE, 'sites': res,
                   'fraction_sites': fsites}, open(a.out, 'w'), indent=1, sort_keys=True)
    if a.header:
        with open(a.header, 'w') as h:
            h.write('// sf_step_sites.h -- written by pipeline/frame_step_users.py --header (do not edit): the calls of the frame-step\n'
                    '// getters (0x10014CF0, 0x10014D20) answered with the fractional step under frame_step=2, by return address\n'
                    '// (call site + 5), analysed build. Why each: data/frame_step_users.json.\n#pragma once\n#include <stdint.h>\n'
                    'static const uint32_t kStepFractionRet[] = {\n')
            for s in fsites: h.write('  0x%08Xu,   // call at %s in %s\n' % (int(s, 16) + 5, s, res[s]['func']))
            h.write('};\n')


if __name__ == '__main__':
    main()
