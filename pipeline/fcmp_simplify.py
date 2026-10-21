"""fcmp_simplify.py -- a floating comparison spelt out over NaN tests, written as the one C comparison it is.

The translation of an x87 compare (`fcom; fnstsw ax; test ah, 0x41; jp`) is exact but long: it tests NAN() of
each operand and combines `<` and `==` with `&&`, `||`, `!` and `!=` (hx2cpp's NaN-exact branches). A C comparison
of two floating values is already exact on NaN: `<`, `<=`, `>`, `>=`, `==` are false when either is NaN and `!=`
is true (C99 7.12.14, IEEE 754). So a boolean expression whose atoms are NAN(a), NAN(b) and comparisons of a with
b is a function of six states: a < b, a == b, a > b, a NaN only, b NaN only, both NaN (a literal operand is never
NaN, which leaves out the states it cannot take). Where its truth table over those states equals that of one
comparison (or the negation of one), it is that comparison.

The proof is the table itself, computed here for every rewrite. An operand must have no side effects (no call,
no assignment, no ++ or --): the original evaluates it several times, the comparison once. Nothing is rewritten
that is not one operand pair throughout.

    simplify(text) -> (text, count): every maximal such expression in text replaced"""
import re

STATES = ['lt', 'eq', 'gt', 'na', 'nb', 'nab']


def truth(op, st):
    """a OP b in state st, as C evaluates it"""
    if st in ('na', 'nb', 'nab'): return op == '!='
    return {'<': st == 'lt', '<=': st in ('lt', 'eq'), '>': st == 'gt', '>=': st in ('gt', 'eq'),
            '==': st == 'eq', '!=': st != 'eq'}[op]


TOK = re.compile(r'\s*(\|\||&&|==|!=|<=|>=|<|>|!|\(|\)|[^()!<>=&|]+?(?=\s*(?:\|\||&&|==|!=|<=|>=|<|>|\)|$)))')


def split_top(s, ops):
    """s split at the top-level occurrences of the operator ops (a tuple), or None"""
    depth, i, parts, last = 0, 0, [], 0
    while i < len(s):
        c = s[i]
        if c == '(': depth += 1
        elif c == ')': depth -= 1
        elif depth == 0:
            for op in ops:
                if s.startswith(op, i) and not (op in ('<', '>') and s[i + 1:i + 2] in ('=', '<', '>')) \
                        and not (op in ('<', '>') and s[i - 1:i] in ('<', '>')) \
                        and not (op == '!' and s[i + 1:i + 2] == '=') and not (op in ('==', '!=') and False):
                    parts.append((s[last:i], op)); last = i + len(op); i += len(op) - 1; break
        i += 1
    if not parts: return None
    parts.append((s[last:], None))
    return parts


def strip_parens(s):
    s = s.strip()
    while s.startswith('(') and s.endswith(')'):
        d = 0
        for i, c in enumerate(s):
            d += c == '('; d -= c == ')'
            if d == 0 and i < len(s) - 1: return s
        s = s[1:-1].strip()
    return s


PURE = re.compile(r'^[\w\s.*&\->()\[\]+\-/,]*$')


TYPEWORDS = {'NAN', 'float', 'double', 'long', 'int', 'unsigned', 'signed', 'char', 'short', 'const'}


def pure(e):
    """no call (an identifier other than a type word or NAN followed by `(`), no assignment, no ++ or --"""
    if PURE.match(e) is None or '++' in e or '--' in e: return False
    return all(m.group(1) in TYPEWORDS for m in re.finditer(r'([A-Za-z_]\w*)\s*\(', e))


def parse(s):
    """an AST: ('or'|'and', l, r) | ('not', x) | ('ne'|'eq', l, r) | ('nan', e) | ('cmp', op, a, b), or None"""
    s = strip_parens(s)
    for ops, tag in ((('||',), 'or'), (('&&',), 'and')):
        p = split_top(s, ops)
        if p:
            node = parse(p[0][0])
            for (seg, _), (nxt, _) in zip(p, p[1:]):
                r = parse(nxt)
                if node is None or r is None: return None
                node = (tag, node, r)
            return node
    p = split_top(s, ('!=', '=='))
    if p and len(p) == 2:
        l, r = parse(p[0][0]), parse(p[1][0])
        if l is not None and r is not None and l[0] != 'term' and r[0] != 'term':
            return ('ne' if p[0][1] == '!=' else 'eq', l, r)
        if l is not None and r is not None and l[0] == 'term' and r[0] == 'term':
            return ('cmp', p[0][1], l[1], r[1])
        return None
    p = split_top(s, ('<=', '>=', '<', '>'))
    if p and len(p) == 2:
        a, b = strip_parens(p[0][0]), strip_parens(p[1][0])
        return ('cmp', p[0][1], a, b)
    if s.startswith('!') and not s.startswith('!='):
        x = parse(s[1:])
        return ('not', x) if x is not None else None
    m = re.match(r'^NAN\((.*)\)$', s)
    if m and strip_parens(m.group(1)) == m.group(1).strip() or (m and True):
        return ('nan', strip_parens(m.group(1))) if m else None
    return ('term', s)


def operands(n, acc):
    if n[0] in ('or', 'and', 'ne', 'eq'): operands(n[1], acc); operands(n[2], acc)
    elif n[0] == 'not': operands(n[1], acc)
    elif n[0] == 'nan': acc.add(n[1])
    elif n[0] == 'cmp': acc.add(n[2]); acc.add(n[3])
    else: acc.add(None)


def literal(e):
    return re.match(r'^\(?\s*(\(\s*(long\s+)?(double|float)\s*\))?\s*\(?-?[0-9.]+(e-?\d+)?f?\)?\s*\)?$', e) is not None


def ev(n, a, b, st):
    if n[0] == 'or': return ev(n[1], a, b, st) or ev(n[2], a, b, st)
    if n[0] == 'and': return ev(n[1], a, b, st) and ev(n[2], a, b, st)
    if n[0] == 'not': return not ev(n[1], a, b, st)
    if n[0] == 'ne': return ev(n[1], a, b, st) != ev(n[2], a, b, st)
    if n[0] == 'eq': return ev(n[1], a, b, st) == ev(n[2], a, b, st)
    if n[0] == 'nan': return (st in ('na', 'nab')) if n[1] == a else (st in ('nb', 'nab'))
    op, x, y = n[1], n[2], n[3]
    if x == a: return truth(op, st)
    flip = {'<': '>', '<=': '>=', '>': '<', '>=': '<=', '==': '==', '!=': '!='}[op]       # b OP a = a FLIP b
    return truth(flip, st)


def equivalent(s):
    """the one comparison s is, or None"""
    n = parse(s)
    if n is None or n[0] in ('term', 'cmp'): return None
    acc = set(); operands(n, acc)
    if None in acc or len(acc) != 2: return None
    a, b = sorted(acc, key=lambda e: (literal(e), s.find(e)))      # as the code writes them, a literal last
    if not (pure(a) and pure(b)): return None
    states = [st for st in STATES if not (literal(a) and st in ('na', 'nab')) and not (literal(b) and st in ('nb', 'nab'))]
    tt = tuple(ev(n, a, b, st) for st in states)
    for op in ('<', '<=', '>', '>=', '==', '!='):
        if tt == tuple(truth(op, st) for st in states): return '%s %s %s' % (a, op, b)
        if tt == tuple(not truth(op, st) for st in states): return '!(%s %s %s)' % (a, op, b)
    return None


def simplify(text):
    """each `if (...)`/`while (...)` condition and each `x = ...;` right side that is one comparison in disguise,
    and each parenthesized part of one, replaced by that comparison"""
    count = 0
    def paren_spans(t):
        stack, out = [], []
        for i, c in enumerate(t):
            if c == '(': stack.append(i)
            elif c == ')' and stack: out.append((stack.pop(), i))
        return out
    changed = True
    while changed:
        changed = False
        for i, j in sorted(paren_spans(text), key=lambda p: p[1] - p[0], reverse=True):
            inner = text[i + 1:j]
            if 'NAN(' not in inner: continue
            r = equivalent(inner)
            if r:
                text = text[:i + 1] + r + text[j:]; count += 1; changed = True; break
        if changed: continue
        for m in re.finditer(r'(=\s*)([^;=][^;]*NAN\([^;]*);', text):
            r = equivalent(m.group(2))
            if r:
                text = text[:m.start(2)] + r + text[m.end(2):]; count += 1; changed = True; break
    return text, count


if __name__ == '__main__':
    tests = ['(!NAN(fVar5) && !NAN(fVar1)) && fVar5 < fVar1 != (fVar5 == fVar1)',
             'NAN(a) || NAN(b) || a < b', '(!NAN(x) && !NAN((long double)0.0f)) && (long double)0.0f < x',
             '!NAN(a) && !NAN(b) && a == b']
    for t in tests: print(t, '->', equivalent(t))
