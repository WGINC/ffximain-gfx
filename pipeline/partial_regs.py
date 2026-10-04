"""partial_regs.py -- Hex-Rays' partial-variable macros (LOBYTE, HIWORD, SLODWORD, ...) as plain C, exactly.

Each macro (pipeline/xi_hexrays.hpp) names a fixed byte, word or dword of a variable's own storage:
`BYTEn(x, n)` is `*((_BYTE *)&(x) + n)`. For a variable of an integer type of width W bytes, on this
target (little-endian, two's complement, and GCC's modulo conversion to a narrower or signed type), part
n of size u is the value's bits [8un, 8u(n+1)), so:

  read   M(x)        ->  (R)((U)(x) >> 8un)                 R = the macro's own type, U = unsigned W bytes
  store  M(x) = E;   ->  x = (T)(((U)(x) & KEEP) | ((U)(Uu)(E) << 8un));
                         T = x's declared type, KEEP = every bit outside the part, Uu = unsigned u bytes

These are identities of C on integers, not facts about the program; they are applied only where they
hold:
  * the operand is a plain identifier declared in this function (a parameter or local) with an integer
    type from INT below; floats (the macro reads their representation), pointers, struct members and
    globals (the macro reads one byte of memory, the rewrite would read the whole global) are left;
  * the part lies inside the variable (n*u + u <= W);
  * a read is a read: the macro is not followed by an assignment operator or ++/--, nor preceded by
    & or ++/--;
  * a store is rewritten only as a whole statement `M(x) = E;` where x's address is never taken in the
    function and E contains no assignment, ++ or --, so E cannot change x and the order in which the
    rewrite reads x and evaluates E does not matter. Compound stores (`BYTE1(x) &= ~4u;`) are left.
Everything left is counted with its reason.
"""
import re

INT = {'char': (1, True), 'signed char': (1, True), 'unsigned char': (1, False), '__int8': (1, True),
       'unsigned __int8': (1, False), '_BYTE': (1, False), 'int8_t': (1, True), 'uint8_t': (1, False),
       'short': (2, True), 'unsigned short': (2, False), '__int16': (2, True), 'unsigned __int16': (2, False),
       '_WORD': (2, False), 'int16_t': (2, True), 'uint16_t': (2, False),
       'int': (4, True), 'unsigned int': (4, False), '__int32': (4, True), 'unsigned __int32': (4, False),
       'long': (4, True), 'unsigned long': (4, False), '_DWORD': (4, False), 'int32_t': (4, True),
       'uint32_t': (4, False), 'unsigned': (4, False), 'signed int': (4, True),
       '__int64': (8, True), 'unsigned __int64': (8, False), '_QWORD': (8, False), 'int64_t': (8, True),
       'uint64_t': (8, False)}
UNS = {1: 'uint8_t', 2: 'uint16_t', 4: 'uint32_t', 8: 'uint64_t'}
SGN = {1: 'int8_t', 2: 'int16_t', 4: 'int32_t'}
RES = {(1, False): '_BYTE', (2, False): '_WORD', (4, False): '_DWORD', (1, True): 'int8_t', (2, True): 'int16_t',
       (4, True): 'int32_t'}
# macro -> (part size u, part index as a function of the variable's width W, signed result)
MACROS = {
    'LOBYTE': (1, lambda W: 0, False), 'BYTE1': (1, lambda W: 1, False), 'BYTE2': (1, lambda W: 2, False),
    'BYTE3': (1, lambda W: 3, False), 'BYTE4': (1, lambda W: 4, False), 'HIBYTE': (1, lambda W: W - 1, False),
    'LOWORD': (2, lambda W: 0, False), 'WORD1': (2, lambda W: 1, False), 'WORD2': (2, lambda W: 2, False),
    'HIWORD': (2, lambda W: W // 2 - 1, False), 'LODWORD': (4, lambda W: 0, False), 'HIDWORD': (4, lambda W: 1, False),
    'SLOBYTE': (1, lambda W: 0, True), 'SBYTE1': (1, lambda W: 1, True), 'SHIBYTE': (1, lambda W: W - 1, True),
    'SLOWORD': (2, lambda W: 0, True), 'SHIWORD': (2, lambda W: 1, True),
    'SLODWORD': (4, lambda W: 0, True), 'SHIDWORD': (4, lambda W: 1, True)}
CALL = re.compile(r'\b(%s)\(\s*([A-Za-z_]\w*)\s*\)' % '|'.join(sorted(MACROS, key=len, reverse=True)))
ASSIGN_AFTER = re.compile(r'\s*(=(?!=)|[-+*/%&|^]=|<<=|>>=|\+\+|--)')
# `&M(x)` takes the address (an & with no operand on its left); `a & M(x)` is a bitwise and
LVAL_BEFORE = re.compile(r'((?:^|[^\w)\]\s&]|\breturn)\s*&|\+\+|--)\s*$')
KEYWORDS = {'return', 'goto', 'else', 'do', 'break', 'continue', 'case', 'delete', 'throw', 'new', 'sizeof'}
DECL = re.compile(r'^\s*((?:(?:unsigned|signed|const)\s+)*[A-Za-z_]\w*(?:\s+__int\d+)?)\s+([A-Za-z_]\w*)\s*;', re.M)


def norm(t):
    return re.sub(r'\s+', ' ', re.sub(r'\bconst\b', '', t)).strip()


def var_types(params, body):
    """name -> declared type text, for parameters (`type name` strings) and the locals declared at the top of
    the body (before the first statement); arrays and pointers are not included"""
    out = {}
    for p in params:
        m = re.match(r'^(.*?)\s*\b([A-Za-z_]\w*)\s*$', p)
        if m and '*' not in m.group(1) and '[' not in p: out[m.group(2)] = norm(m.group(1))
    head = body.split('\n\n', 1)[0]
    for m in DECL.finditer(head):
        if m.group(1).split()[0] not in KEYWORDS: out[m.group(2)] = norm(m.group(1))
    return out


def _lit(v, W): return '0x%X%s' % (v, 'ull' if W == 8 else 'u')


def rewrite(body, types):
    """the body with every macro use the rules above allow rewritten; returns (body, Counter-like dict)"""
    stats = {}
    def count(k): stats[k] = stats.get(k, 0) + 1
    addr_taken = lambda x: re.search(r'(?<![&\w])&\s*(?:\w+\(\s*)?%s\b' % re.escape(x), body) is not None

    def part(mac, x):
        if x not in types: return None, 'operand not a local or parameter'
        t = types[x]
        if t not in INT: return None, 'operand type %s is not an integer type' % t.split()[0]
        W = INT[t][0]; u, idx, sg = MACROS[mac]; n = idx(W)
        if n < 0 or (n + 1) * u > W: return None, 'part outside the variable'
        return (W, u, n, sg, t), None

    # stores: whole statements `M(x) = E;`
    def store(m):
        ind, mac, x, e = m.group(1), m.group(2), m.group(3), m.group(4)
        p, why = part(mac, x)
        if p is None: count('store left: ' + why); return m.group(0)
        if addr_taken(x): count("store left: the variable's address is taken"); return m.group(0)
        if re.search(r'(?<![=!<>])=(?!=)|\+\+|--', e): count('store left: the stored expression assigns'); return m.group(0)
        W, u, n, sg, t = p; s = 8 * u * n
        keep = ((1 << 8 * W) - 1) ^ (((1 << 8 * u) - 1) << s)
        U = UNS[W]
        count('store rewritten')
        return '%s%s = (%s)(((%s)(%s) & %s) | ((%s)(%s)(%s) << %d));' % (ind, x, t, U, x, _lit(keep, W), U, UNS[u], e, s)
    body = re.sub(r'(?m)^(\s*)(%s)\(\s*([A-Za-z_]\w*)\s*\)\s*=(?!=)\s*(.+?);[ \t]*$'
                  % '|'.join(sorted(MACROS, key=len, reverse=True)), store, body)

    # reads
    out, pos = [], 0
    for m in CALL.finditer(body):
        mac, x = m.group(1), m.group(2)
        if ASSIGN_AFTER.match(body, m.end()) or LVAL_BEFORE.search(body, 0, m.start()):
            count('lvalue use left'); continue
        p, why = part(mac, x)
        if p is None: count('read left: ' + why); continue
        W, u, n, sg, t = p; s = 8 * u * n
        r = RES[(u, sg)]
        new = '(%s)(%s)' % (r, x) if s == 0 else '(%s)((%s)(%s) >> %d)' % (r, UNS[W], x, s)
        out.append(body[pos:m.start()]); out.append(new); pos = m.end(); count('read rewritten')
    out.append(body[pos:])
    return ''.join(out), stats
