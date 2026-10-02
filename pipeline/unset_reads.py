"""unset_reads.py -- locals Hex-Rays reads but never assigns: each is a decompilation error (a
misframed stack argument, a condition code it did not model, a register it lost track of).

    python3 unset_reads.py EXPORT.jsonl [gate.jsonl]
Prints, per function, each such variable with its declaration comment (register or frame slot) and
its first use. A variable whose address is taken (`&v`), or that is written through a LOBYTE/HIWORD
style partial assignment or an array element, counts as assigned.
"""
import json, re, sys


def unset(text):
    i = text.find('\n\n'); i = i if i > 0 else 0
    head, body = text[:i], text[i:]
    out = []
    for m in re.finditer(r'^\s+[^;\n]*?\b(v\d+)(\[[^\]]*\])*; // (.*)$', head, re.M):
        v, note = m.group(1), m.group(3)
        if 'BYREF' in note: continue
        if re.search(r'&%s\b|\b%s\s*(\[[^\]]*\]\s*)*(=(?!=)|\+\+|--|[-+*/|&^]=|<<=|>>=)|(\+\+|--)%s\b|\b(LO|HI)(BYTE|WORD|DWORD)\d*\(%s\)\s*=|BYTE\d\(%s\)\s*=|\(%s\)\s*=' % ((v,) * 6), body):
            continue
        use = re.search(r'.*\b%s\b.*' % v, body)
        if use: out.append((v, note.strip(), use.group(0).strip()[:100]))
    return out


def main(exp, gate=None):
    verdict = {}
    if gate:
        for l in open(gate):
            g = json.loads(l); verdict[int(g['original'], 16)] = g['verdict']
    n = 0
    for l in open(exp):
        r = json.loads(l)
        u = unset(r['text'])
        if not u: continue
        n += 1
        print('0x%08X %s %s' % (r['ea'], r['name'], verdict.get(r['ea'], '')))
        for v, note, use in u: print('    %-6s // %-28s %s' % (v, note, use))
    print(n, 'functions')


if __name__ == '__main__':
    main(*sys.argv[1:])
