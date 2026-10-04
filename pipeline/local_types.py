"""local_types.py -- the type each Hex-Rays stack variable's instructions allow, and a copy of the export with the
ones they decide written in.

Reads local_evidence.py's output. A variable is decided only when its instructions leave exactly one type:

  float / double  the variable is declared a 4- or 8-byte integer (int, unsigned int, _DWORD, __int64, _QWORD) and
                  every instruction in the function that touches any of its bytes is either an x87 float operation
                  of its whole width at its start, or a copy (mov/push/pop) of its whole width at its start, with at
                  least one x87 float operation. Its bytes are only ever handled as a float.
Not decided, with the reason:
  blind           the function has a stack operand at an instruction with no known depth (it could be this one)
  sp_differs      IDA's SP is not the exact depth somewhere in the function, so Hex-Rays' slots are not the code's
  address_taken   its address, or that of a variable overlapping it, is taken (lea): accesses through that pointer
                  are not seen
  no_float / mixed / partial
                  no x87 float access; integer and float accesses both; an access to only part of it
  register        a register variable: its live range is not in the map, so its accesses are not all known
Variables other than 4/8-byte integers are listed with what their accesses show, undecided.

The rewrite (a stack local, not an argument: an argument's type is the function's prototype, which its callers
see): the declaration `int v7;` becomes `float v7;`, each `*(float *)&v7` becomes `v7`, and each other use of v7
becomes `*(int *)&v7` (the same bytes read as the declared integer, so nothing changes in what the code does). A
variable used in any other form (`&v7` alone, `v7[`, LODWORD(v7)) is not rewritten (`text_form`).

Usage: python3 local_types.py EVIDENCE.json EXPORT.jsonl OUT.json [TYPED_EXPORT.jsonl]"""
import collections, json, re, sys

INT = {4: {'int', 'unsigned int', '_DWORD', 'signed int'}, 8: {'__int64', '_QWORD', 'unsigned __int64'}}
FLT = {4: 'float', 8: 'double'}


def verdicts(E):
    out = {}
    for f, v in E.items():
        if f.startswith('_'): continue
        st = {n: r for n, r in v['vars'].items() if 'stk_ida' in r['loc']}
        addr_taken = set()
        for n, r in st.items():
            if any(a[1] == 'addr' for a in r['acc']): addr_taken.add(n)
        res = {}
        for n, r in v['vars'].items():
            w, t = r['width'], r['type']
            row = {'type': t, 'arg': r['arg'], 'loc': r['loc'],
                   'kinds': dict(collections.Counter(a[1] for a in r['acc'] if a[1] != 'reg')),
                   'deref': dict(collections.Counter('%s@%d' % (d[1], d[3]) for d in r['deref']))}
            if 'stk_ida' not in r['loc']:
                row['verdict'] = 'register'; res[n] = row; continue
            if w not in INT or t not in INT[w]:
                row['verdict'] = 'not_integer'; res[n] = row; continue
            lo, hi = r['loc']['stk_ida'], r['loc']['stk_ida'] + w
            ov = [m for m, q in st.items() if m != n and q['loc']['stk_ida'] < hi and lo < q['loc']['stk_ida'] + q['width']]
            ks = {a[1] for a in r['acc']}; want = 'f%d' % (w * 8)
            if v['sp_differs']: why = 'sp_differs'
            elif v['blind']: why = 'blind'
            elif n in addr_taken or any(m in addr_taken for m in ov): why = 'address_taken'
            elif want not in ks: why = 'no_float'
            elif not ks <= {want, 'mov'}: why = 'mixed'
            elif not all(a[3] == 0 and a[2] == w for a in r['acc']): why = 'partial'
            else: why = None
            row['overlaps'] = ov
            if why: row['verdict'] = why
            else: row['verdict'] = 'decided'; row['new_type'] = FLT[w]
            res[n] = row
        out[f] = {'name': v['name'], 'vars': res}
    return out


MACRO = re.compile(r'\b(?:LO|HI|SLO|SHI)(?:BYTE|WORD|DWORD)\s*\(\s*$|\bS?BYTE\d\s*\(\s*$|\bWORD\d\s*\(\s*$')


def rewrite(text, n, old, new):
    """text with variable n retyped old -> new as described above, or None when a use has another form"""
    head, sep, body = text.partition('\n{')
    decls, sep2, stmts = body.partition('\n\n')
    if not sep or not sep2: return None
    decl = re.compile(r'^(\s+)%s(\s+%s;)' % (re.escape(old), re.escape(n)), re.M)
    if len(decl.findall(decls)) != 1: return None
    decls = decl.sub(lambda m: m.group(1) + new + m.group(2), decls, count=1)
    ityp = 'int' if new == 'float' else '__int64'
    parts = stmts.split('*(%s *)&%s' % (new, n))
    for k, p in enumerate(parts):
        for m in re.finditer(r'\b%s\b' % re.escape(n), p):
            before, after = p[:m.start()].rstrip(), p[m.end():].lstrip()
            if re.match(r'(\[|\.|->)', after): return None              # n[, n., n->
            if before.endswith('&') and not before.endswith('&&'): return None      # &n
            if MACRO.search(before): return None                           # LODWORD(n), BYTE1(n), ...
        parts[k] = re.sub(r'\b%s\b' % re.escape(n), '*(%s *)&%s' % (ityp, n), p)
    return head + sep + decls + sep2 + n.join(parts)


def main(ev, exp, out, typed=None):
    E = json.load(open(ev)); V = verdicts(E)
    R = [json.loads(l) for l in open(exp)]
    applied = collections.Counter()
    by = {int(f, 16): x for f, x in V.items()}
    for r in R:
        x = by.get(r['ea'])
        if not x: continue
        for n, row in x['vars'].items():
            if row['verdict'] != 'decided': continue
            if row['arg']:
                row['applied'] = 'no: argument (prototype)'; applied['argument'] += 1; continue
            t = rewrite(r['text'], n, row['type'], row['new_type'])
            if t is None:
                row['applied'] = 'no: text_form'; applied['text_form'] += 1; continue
            r['text'] = t; row['applied'] = 'yes'; applied['yes'] += 1
    c = collections.Counter(row['verdict'] for x in V.values() for row in x['vars'].values())
    ph = collections.Counter(row['verdict'] for x in V.values() for row in x['vars'].values()
                             if re.search(r'\b_(DWORD|BYTE|WORD|QWORD)\b', row['type']) and not row['arg'])
    V['_summary'] = {'verdicts': dict(c), 'placeholder_locals': dict(ph), 'applied': dict(applied)}
    json.dump(V, open(out, 'w'), indent=0)
    if typed:
        with open(typed, 'w') as f:
            for r in R: f.write(json.dumps(r) + '\n')
    print(json.dumps(V['_summary']))


if __name__ == '__main__':
    main(*sys.argv[1:5])
