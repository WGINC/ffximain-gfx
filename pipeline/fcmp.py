"""fcmp.py -- what a float comparison branch really tests, from the code: the outcomes of the compare
(less, equal, greater, unordered) for which the jump is taken.

MSVC branches on an x87 compare with `fcom/fcomp/fcompp/fucom*; fnstsw ax; <test|and|cmp|xor> ah|eax, imm;
jcc`, or `fnstsw ax; sahf; jcc`. FCOM sets C0, C2, C3 (Intel SDM): greater 0,0,0; less C0; equal C3;
unordered C0, C2 and C3. In AH, C0 is 0x01, C2 0x04, C3 0x40. Each outcome is pushed through the
instructions between `fnstsw` and the jump (their flag effects are defined by the SDM), and the jump's
condition read from the resulting flags. Nothing is inferred from what the comparison "probably" means.

Hex-Rays prints such a branch as an ordered C operator (`a >= b`), which is false for NaN; the code may
take the branch on unordered too. `exact(x, op, y, taken, st0_is_x)` gives the C expression true exactly
on the outcomes the code tests, NaN included, or None where the text cannot be matched to the code.

    python3 fcmp.py JCC_ADDR [...]     uses $FFXI_DLL: prints each jump's taken outcomes
    python3 fcmp.py --export EXPORT.jsonl data/fcmp_exact.json     every exported float comparison (the IDA
        script's `fcmps`: [jump, op, x, y]) with the outcomes its jump is taken on, read from the code
"""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

OUTCOMES = {'lt': 0x01, 'eq': 0x40, 'gt': 0x00, 'un': 0x45}       # AH after fnstsw (C0, C2, C3 bits)
FCOM = ('fcom', 'fcomp', 'fcompp', 'fucom', 'fucomp', 'fucompp', 'ficom', 'ficomp', 'ftst')


def parity(b): return bin(b & 0xFF).count('1') % 2 == 0


def flags_after(seq, ah):
    """run the instructions between fnstsw and the jump on AX = ah << 8: (CF, ZF, PF, SF) after the last
    flag-setting one, or None for an instruction whose effect is not modelled here"""
    ax = ah << 8; fl = None
    for i in seq:
        m, ops = i.mnemonic, i.op_str.replace(' ', '').split(',')
        if m == 'sahf':
            v = ax >> 8; fl = (bool(v & 1), bool(v & 0x40), bool(v & 4), bool(v & 0x80)); continue
        if m in ('test', 'and', 'cmp', 'xor') and len(ops) == 2 and ops[0] in ('ah', 'eax', 'ax'):
            imm = int(ops[1], 0)
            if ops[0] == 'ah': a, w = ax >> 8, 8
            else: a, w = ax, 32 if ops[0] == 'eax' else 16
            mask = (1 << w) - 1; imm &= mask
            if m in ('test', 'and'): r = a & imm; cf = False
            elif m == 'xor': r = a ^ imm; cf = False
            else: r = (a - imm) & mask; cf = a < imm
            fl = (cf, r == 0, parity(r), bool(r >> (w - 1) & 1))
            if m in ('and', 'xor'): ax = (r << 8) if ops[0] == 'ah' else r
            continue
        if m in ('nop',): continue
        return None
    return fl


JCC = {'jp': lambda c, z, p, s: p, 'jpe': lambda c, z, p, s: p, 'jnp': lambda c, z, p, s: not p, 'jpo': lambda c, z, p, s: not p,
       'je': lambda c, z, p, s: z, 'jz': lambda c, z, p, s: z, 'jne': lambda c, z, p, s: not z, 'jnz': lambda c, z, p, s: not z,
       'jb': lambda c, z, p, s: c, 'jc': lambda c, z, p, s: c, 'jae': lambda c, z, p, s: not c, 'jnc': lambda c, z, p, s: not c,
       'jbe': lambda c, z, p, s: c or z, 'ja': lambda c, z, p, s: not (c or z),
       'js': lambda c, z, p, s: s, 'jns': lambda c, z, p, s: not s}


def taken(ins_list, k):
    """ins_list: the function's instructions in address order; k: index of the jump. Returns (taken outcomes,
    the compare instruction) or None when the sequence is not the fcom/fnstsw form"""
    j = ins_list[k]
    if j.mnemonic not in JCC: return None
    seq = []; n = k - 1
    while n >= 0 and ins_list[n].mnemonic not in ('fnstsw',) and len(seq) < 4:
        seq.insert(0, ins_list[n]); n -= 1
    if n < 0 or ins_list[n].mnemonic != 'fnstsw' or ins_list[n].op_str != 'ax': return None
    c = n - 1
    while c >= 0 and ins_list[c].mnemonic not in FCOM and not ins_list[c].mnemonic.startswith(('j', 'call', 'ret')) and n - c < 6:
        c -= 1
    if c < 0 or ins_list[c].mnemonic not in FCOM: return None
    out = set()
    for o, ah in OUTCOMES.items():
        fl = flags_after(seq, ah)
        if fl is None: return None
        if JCC[j.mnemonic](*fl): out.add(o)
    return frozenset(out), ins_list[c]


SWAP = {'lt': 'gt', 'gt': 'lt', 'eq': 'eq', 'un': 'un'}
TRUE = {'<': {'lt'}, '<=': {'lt', 'eq'}, '>': {'gt'}, '>=': {'gt', 'eq'}, '==': {'eq'}, '!=': {'lt', 'gt', 'un'}}


def exact(x, op, y, taken_set):
    """the C expression over x and y that is true exactly on the outcomes the branch tests, where Hex-Rays'
    `x op y` matches the code on every ordered outcome (as the taken or the not-taken side, x as ST0 or as
    the other operand); None when no reading, or more than one with different NaN behaviour, matches"""
    t_ord = TRUE[op] - {'un'}
    fits = set()
    for side in (taken_set, frozenset(OUTCOMES) - taken_set):
        for sw in (False, True):
            s = {SWAP[o] for o in side} if sw else set(side)
            if s - {'un'} == t_ord: fits.add(frozenset(s))
    if len(fits) != 1: return None
    want = next(iter(fits))
    if want == TRUE[op] or (want == t_ord and op != '!='): return '%s %s %s' % (x, op, y)  # already exact
    forms = {frozenset({'lt', 'un'}): '!(%s >= %s)', frozenset({'lt', 'eq', 'un'}): '!(%s > %s)',
             frozenset({'gt', 'un'}): '!(%s <= %s)', frozenset({'gt', 'eq', 'un'}): '!(%s < %s)',
             frozenset({'lt', 'gt'}): '(%s < %s || %s > %s)', frozenset({'eq', 'un'}): '!(%s < %s || %s > %s)'}
    f = forms.get(want)
    if f is None: return None
    if f.count('%s') == 4:
        if any(c in x + y for c in '(=+-'): return None          # evaluated twice: only plain operands
        return f % (x, y, x, y)
    return f % (x, y)


def export_main(exp, out):
    import sp_solve as S, json
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json')))
    res = {}; n = unread = 0
    for l in open(exp):
        r = json.loads(l)
        if not r.get('fcmps') or ('0x%x' % r['ea']) not in ext: continue
        ins = im.code(ext['0x%x' % r['ea']]); L = [ins[x] for x in sorted(ins)]; idx = {i.address: k for k, i in enumerate(L)}
        rows = []
        for ea, op, x, y in r['fcmps']:
            t = taken(L, idx[ea]) if ea in idx else None
            n += 1; unread += t is None
            rows.append(['0x%08X' % ea, op, x, y, sorted(t[0]) if t else None])
        res['0x%08X' % r['ea']] = rows
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d float comparisons in %d functions; %d jumps not in the fcom/fnstsw form (left as printed)' % (n, len(res), unread))


if __name__ == '__main__' and sys.argv[1:2] == ['--export']:
    export_main(*sys.argv[2:4])
elif __name__ == '__main__':
    import sp_solve as S, json
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'func_extents.json')))
    for a in sys.argv[1:]:
        a = int(a, 16)
        f = next(e for e in ext.values() if any(x <= a < y for x, y in e))
        ins = im.code(f); L = [ins[x] for x in sorted(ins)]
        k = next(n for n, i in enumerate(L) if i.address == a)
        r = taken(L, k)
        print('0x%08X %s: %s' % (a, L[k].mnemonic, sorted(r[0]) if r else 'not an fcom/fnstsw branch'))
