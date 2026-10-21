"""source_shape.py -- the clean tree's decompiler idioms rewritten as source code, each rewrite proven to change nothing.

Each rule is kept only with a proof that it changes nothing, of the kind that fits it; where the proof fails the
function's (or the file's) text is put back. Nothing here relies on the gate.
  * `types`: a typedef name is its type (C++, [dcl.typedef]). Each pair is checked as the tree compiles it
    (static_assert(std::is_same) with build.sh's compiler and flags), and the file must compile.
  * `macros`: the file preprocessed before and after must give the same tokens: what the compiler sees is unchanged.
  * `floats`: the literal reads back to exactly the constant's bits (checked here when it is written; GCC rounds a
    decimal literal correctly), of the same type (float for xi_k4, double for xi_k8); the file must compile.
  * `undefined4`: a struct (xi_ghidra_shared.hpp) that differs from uint32_t in one thing only: a float converted
    into it keeps its bits. Every other operation goes through its uint32_t: integers and pointers convert as to
    uint32_t, it reads as its uint32_t, its compound assignments are the uint32_t's. So in a function that never
    converts a float (or a long double) into an undefined4, it is uint32_t. The proof: the original file compiles
    with undefined4 in its bodies replaced by a probe, the same struct with those two conversions deleted; a
    function whose lines fail keeps undefined4. (GCC's register choices can change with a type's spelling alone,
    so identical machine code would be a stricter test than equal meaning; it is not used.)
  * `fcmp`: a floating comparison spelt out over NAN() tests written as the one C comparison it is
    (fcmp_simplify.py): its truth table over the six IEEE states, computed for each rewrite, is the proof; the
    operands must have no side effects; the file must compile.
  * `params`: a parameter named as the PS2 debug build names it (data/ps2_dwarf_funcs.json: the matched PS2
    function's DWARF formal parameters, `this` aside), where the PC function has exactly as many parameters. The
    proof is that the rename is a renaming only: the file preprocessed after it, with each new name mapped back to
    the old one, gives exactly the tokens it gave before (a new name that met any identifier already there would
    map that one too, and differ).

Rules (phase 1), each on the text of one function at a time, outside string and character literals:
  * types: Ghidra's type names that are typedefs of a standard type (xi_ghidra.hpp, xi_ghidra_shared.hpp) spelt as
    that type: uint -> uint32_t, undefined1 -> uint8_t, float10 -> long double, ...;
  * `undefined4`, a struct (a float copied into it keeps its bits), spelt uint32_t: in the body only (a
    signature must match its declaration); kept only where the code proves identical;
  * xi_k4(0x...ull) / xi_k8(0x...ull), a floating constant by its encoding, written as the shortest decimal literal
    that reads back to the same bits (finite values only; -0.0 kept with its sign);
  * ZEXTnm / SEXTnm / CONCATnm, Ghidra's macros, written as the casts and shifts they expand to (xi_ghidra.hpp).

    python3 source_shape.py TREE_DIR           (the tree's build.sh flags; objects under TREE_DIR/build/obj)
Writes TREE_DIR/source_shape.json: per rule, the functions it was kept in and the ones put back."""
import json, os, re, struct, subprocess, sys
from concurrent.futures import ThreadPoolExecutor

ALIAS = {'undefined': 'uint8_t', 'undefined1': 'uint8_t', 'uchar': 'uint8_t', 'byte': 'uint8_t', 'xi_bool': 'uint8_t',
         'undefined2': 'uint16_t', 'ushort': 'uint16_t', 'word': 'uint16_t', 'wchar16': 'uint16_t',
         'uint': 'uint32_t', 'dword': 'uint32_t', 'undefined3': 'uint32_t', 'uint3': 'uint32_t', 'wchar32': 'uint32_t',
         'int3': 'int32_t', 'ulonglong': 'uint64_t', 'undefined8': 'uint64_t', 'qword': 'uint64_t', 'undefined5': 'uint64_t',
         'undefined6': 'uint64_t', 'undefined7': 'uint64_t', 'longlong': 'int64_t', 'float10': 'long double'}
MACROS = {   # xi_ghidra.hpp's definitions, with h/l/x the arguments
    'ZEXT14': '((uint32_t)(uint8_t)({0}))', 'ZEXT24': '((uint32_t)(uint16_t)({0}))', 'ZEXT48': '((uint64_t)(uint32_t)({0}))',
    'ZEXT12': '((uint16_t)(uint8_t)({0}))', 'SEXT14': '((int32_t)(int8_t)({0}))', 'SEXT24': '((int32_t)(int16_t)({0}))',
    'SEXT48': '((int64_t)(int32_t)({0}))', 'SEXT12': '((int16_t)(int8_t)({0}))',
    'CONCAT11': '((uint16_t)(((uint16_t)(uint8_t)({0}) << 8) | (uint8_t)({1})))',
    'CONCAT12': '((uint32_t)(((uint32_t)(uint8_t)({0}) << 16) | (uint16_t)({1})))',
    'CONCAT22': '((uint32_t)(((uint32_t)(uint16_t)({0}) << 16) | (uint16_t)({1})))',
    'CONCAT31': '((uint32_t)(((uint32_t)({0}) << 8) | (uint8_t)({1})))',
    'CONCAT44': '((uint64_t)(((uint64_t)(uint32_t)({0}) << 32) | (uint32_t)({1})))'}
LIT = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|//[^\n]*|/\*.*?\*/', re.S)   # literals and comments


def outside_literals(text, fn):
    """fn applied to the parts of text outside string and character literals and comments"""
    out, k = [], 0
    for m in LIT.finditer(text):
        out.append(fn(text[k:m.start()])); out.append(m.group(0)); k = m.end()
    out.append(fn(text[k:]))
    return ''.join(out)


def float_literal(bits, size):
    """the shortest decimal that reads back to these bits, with C's suffix; None for a NaN or an infinity"""
    if size == 4:
        x = struct.unpack('<f', struct.pack('<I', bits & 0xFFFFFFFF))[0]
        back = lambda s: struct.unpack('<I', struct.pack('<f', float(s)))[0] == bits & 0xFFFFFFFF
    else:
        x = struct.unpack('<d', struct.pack('<Q', bits))[0]
        back = lambda s: struct.unpack('<Q', struct.pack('<d', float(s)))[0] == bits
    if x != x or x in (float('inf'), float('-inf')): return None
    for p in range(1, 18):
        s = '%.*g' % (p, x)
        if back(s): break
    if 'e' not in s and '.' not in s: s += '.0'
    if size == 4: s += 'f'
    return '(' + s + ')' if s.startswith('-') else s


def args_at(text, i):
    """the comma-separated arguments of the call whose '(' is at i, and the index after its ')'"""
    depth, cur, out = 0, '', []
    for j in range(i, len(text)):
        c = text[j]
        if c == '(':
            depth += 1
            if depth == 1: continue
        elif c == ')':
            depth -= 1
            if depth == 0: out.append(cur); return out, j + 1
        elif c == ',' and depth == 1: out.append(cur); cur = ''; continue
        cur += c
    return None, i


def expand_macros(t):
    out, k = [], 0
    rx = re.compile(r'\b(%s)\s*\(' % '|'.join(MACROS))
    while True:
        m = rx.search(t, k)
        if not m: out.append(t[k:]); break
        args, end = args_at(t, m.end() - 1)
        n = MACROS[m.group(1)].count('{')
        if args is None or len(args) != n: out.append(t[k:m.end()]); k = m.end(); continue
        out.append(t[k:m.start()]); out.append(MACROS[m.group(1)].format(*[expand_macros(a.strip()) for a in args])); k = end
    return ''.join(out)


def rule_types(head, body):
    f = lambda t: re.sub(r'(?<![\w.>])(%s)\b(?!\s*::)' % '|'.join(map(re.escape, ALIAS)), lambda m: ALIAS[m.group(1)], t)
    return outside_literals(head, f), outside_literals(body, f)


def rule_undefined4(head, body):
    return head, outside_literals(body, lambda t: re.sub(r'(?<![\w.>])undefined4\b', 'uint32_t', t))


def rule_floats(head, body):
    def f(t):
        def k(m):
            s = float_literal(int(m.group(2), 16), 4 if m.group(1) == '4' else 8)
            return s if s else m.group(0)
        return re.sub(r'\bxi_k([48])\(0x([0-9A-Fa-f]+)ull\)', k, t)
    return head, outside_literals(body, f)


def rule_macros(head, body):
    return head, outside_literals(body, expand_macros)


PROOF = {'types': 'compiles', 'floats': 'compiles', 'macros': 'preprocessed', 'undefined4': 'probe', 'params': 'alpha',
         'fcmp': 'compiles'}
_DW = {}


def dwarf_params():
    if not _DW:
        path = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data', 'ps2_dwarf_funcs.json')
        _DW.update({int(k, 16): v for k, v in json.load(open(path)).items()} if os.path.exists(path) else {})
    return _DW


RENAMES = {}     # (file, block index) -> {new: old}, filled by rule_params for the alpha proof


def params_map(head, ea):
    """{old parameter name: PS2 name} for the function at ea, or {} (see rule_params)"""
    v = dwarf_params().get(ea)
    if not v: return {}
    sig = head.splitlines()[-1] if head.splitlines() else ''
    m = re.search(r'\((.*)\)\s*$', sig)
    if not m: return {}
    ps = [x.strip() for x in m.group(1).split(',') if x.strip() and x.strip() != 'void']
    names = [re.findall(r'(\w+)\s*$', x)[0] if re.findall(r'(\w+)\s*$', x) else None for x in ps]
    if names and names[0] in ('self', 'self_', 'this'): names = names[1:]
    want = [n for n, t in v['params']]
    if len(names) != len(want) or any(n is None for n in names): return {}
    return {o: n for o, n in zip(names, want) if o != n and re.match(r'^[A-Za-z_]\w*$', n)}


def rule_params(head, body, ea=None):
    pairs = params_map(head, ea)
    if not pairs: return head, body
    def f(t):
        for o, n in pairs.items(): t = re.sub(r'(?<![\w.>])%s\b' % re.escape(o), n, t)
        return t
    return outside_literals(head, f), outside_literals(body, f)


def rule_fcmp(head, body):
    import fcmp_simplify
    return head, outside_literals(body, lambda t: fcmp_simplify.simplify(t)[0])


RULES = [('types', rule_types), ('floats', rule_floats), ('macros', rule_macros), ('undefined4', rule_undefined4),
         ('fcmp', rule_fcmp), ('params', rule_params)]
PROBE = ('''struct xi_u4_probe {          // source_shape.py: undefined4 with the float conversions deleted
    uint32_t v;
    xi_u4_probe() = default;
#define XI_U4_INT(t) xi_u4_probe(t x) : v((uint32_t)x) {}
    XI_U4_INT(bool) XI_U4_INT(char) XI_U4_INT(signed char) XI_U4_INT(unsigned char) XI_U4_INT(short) XI_U4_INT(unsigned short)
    XI_U4_INT(int) XI_U4_INT(unsigned) XI_U4_INT(long) XI_U4_INT(unsigned long) XI_U4_INT(long long) XI_U4_INT(unsigned long long)
#undef XI_U4_INT
    template <class T> xi_u4_probe(T *p) : v((uint32_t)(uintptr_t)p) {}
    xi_u4_probe(float) = delete; xi_u4_probe(double) = delete; xi_u4_probe(long double) = delete;
    xi_u4_probe(undefined4 u) : v(u.v) {}       // as uint32_t <-> undefined4 in the rewrite: the value, both ways
    operator undefined4() const { undefined4 u; u.v = v; return u; }
    operator uint32_t() const { return v; }
    template <class T> explicit operator T *() const { return (T *)(uintptr_t)v; }
#define XI_U4_OP(op) template <class T> xi_u4_probe &operator op##=(T x) { v op##= x; return *this; }
    XI_U4_OP(+) XI_U4_OP(-) XI_U4_OP(*) XI_U4_OP(/) XI_U4_OP(%) XI_U4_OP(&) XI_U4_OP(|) XI_U4_OP(^) XI_U4_OP(<<) XI_U4_OP(>>)
#undef XI_U4_OP
    xi_u4_probe &operator++() { ++v; return *this; } xi_u4_probe &operator--() { --v; return *this; }
    uint32_t operator++(int) { return v++; } uint32_t operator--(int) { return v--; }
};
''')


def probe_failures(tree, fn, text):
    """the block indexes whose lines fail to compile with undefined4 in the bodies replaced by the probe"""
    pre, bl = blocks(text)
    body_re = lambda t: outside_literals(t, lambda x: re.sub(r'(?<![\w.>])undefined4\b', 'xi_u4_probe', x))
    new = [h + body_re(b) for h, b in map(head_body, bl)]
    # the probe goes after the file's includes (its first block's position), so it sees stdint
    probe_text = pre + PROBE + ''.join(new)
    path = os.path.join(tree, 'build', 'probe_' + fn)
    open(path, 'w').write(probe_text)
    r = subprocess.run(['bash', '-c', 'cd %s && source <(sed -n "/^CXX=/,/^OUTDLL=/p" build.sh) && $CXX "${FLAGS[@]}" -Isrc -fsyntax-only build/probe_%s'
                        % (tree, fn)], capture_output=True, text=True)
    os.remove(path)
    bad_lines = {int(m.group(1)) for m in re.finditer(r'probe_[^:]*:(\d+):\d+: error', r.stderr)}
    starts, ln = [], pre.count('\n') + PROBE.count('\n') + 1
    for b in new: starts.append((ln, ln + b.count('\n'))); ln += b.count('\n')
    if r.returncode and not bad_lines: return set(range(len(bl)))        # an error outside any block: none proven
    return {k for k, (a, z) in enumerate(starts) if any(a <= x <= z for x in bad_lines)}


TOK = re.compile(r'[A-Za-z_]\w*|\d[\w.]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|\S')


def preprocessed(tree, src):
    """the file's tokens after preprocessing (whitespace and line markers out), as the tree's compiler sees them"""
    r = subprocess.run(['bash', '-c', 'cd %s && source <(sed -n "/^CXX=/,/^OUTDLL=/p" build.sh) && $CXX "${FLAGS[@]}" -E -P %s'
                        % (tree, src)], capture_output=True, text=True)
    return TOK.findall(r.stdout) if r.returncode == 0 else None


def check_aliases(tree):
    """compile a static_assert(std::is_same) for each ALIAS pair with the tree's compiler and flags"""
    path = os.path.join(tree, 'build', 'alias_check.cpp')
    open(path, 'w').write('#include <type_traits>\n#include "xi_ghidra.hpp"\n' + ''.join(
        'static_assert(std::is_same<%s, %s>::value, "%s");\n' % (a, b, a) for a, b in ALIAS.items()))
    r = subprocess.run(['bash', '-c', 'cd %s && source <(sed -n "/^CXX=/,/^OUTDLL=/p" build.sh) && $CXX "${FLAGS[@]}" -fsyntax-only build/alias_check.cpp'
                        % tree], capture_output=True, text=True)
    if r.returncode: raise SystemExit('source_shape: an ALIAS pair is not the same type:\n' + r.stderr[-2000:])


def blocks(text):
    """[(prefix, [function blocks])]: a file split at its `// FUNCTION: FFXIMAIN` lines"""
    parts = re.split(r'(?m)^(?=// FUNCTION: FFXIMAIN )', text)
    return parts[0], parts[1:]


def head_body(block):
    i = block.find('\n{')
    return (block[:i], block[i:]) if i >= 0 else (block, '')


def objcode(obj):
    """{function key: normalised disassembly} of one object"""
    r = subprocess.run(['i686-w64-mingw32-objdump', '-d', '-r', '-C', '-w', obj], capture_output=True, text=True)
    out, cur, lines = {}, None, []
    for l in r.stdout.splitlines():
        m = re.match(r'^[0-9a-f]+ <(.*)>:$', l)
        if m:
            if cur: out[cur] = '\n'.join(lines)
            cur, lines = key(m.group(1)), []
            continue
        if cur is None or not l.strip(): continue
        l = re.sub(r'^\s*[0-9a-f]+:\s*', '', l)                       # the address
        l = re.sub(r'\b[0-9a-f]+ <([^>]*)>', lambda m: '<%s>' % key(m.group(1)), l)   # a target: by symbol only
        l = re.sub(r'(R_386_\w+)\s+(.*)$', lambda m: m.group(1) + ' ' + key(m.group(2)), l)
        lines.append(l)
    if cur: out[cur] = '\n'.join(lines)
    return out


def datasecs(obj):
    """the object's data sections, byte for byte (objdump -s of every section but code and debug)"""
    r = subprocess.run(['i686-w64-mingw32-objdump', '-h', obj], capture_output=True, text=True)
    secs = [m.group(1) for m in re.finditer(r'^\s*\d+\s+(\S+)\s', r.stdout, re.M) if not m.group(1).startswith(('.text', '.debug'))]
    out = {}
    for sname in secs:
        out[sname] = subprocess.run(['i686-w64-mingw32-objdump', '-s', '-j', sname, obj], capture_output=True, text=True).stdout.split('\n', 3)[-1]
    return out


def key(sym):
    """a symbol without its parameter list (a renamed parameter type renames the mangled name, not the function)"""
    d, i = 0, None
    for j, c in enumerate(sym):
        if c == '(' and d == 0 and not sym[:j].endswith('operator'): i = j; break
        d += c == '<'; d -= c == '>'
    return sym[:i] if i is not None else sym


def compile_one(tree, src, obj):
    flags = open(os.path.join(tree, 'build.sh')).read()
    r = subprocess.run(['bash', '-c', 'cd %s && source <(sed -n "/^CXX=/,/^OUTDLL=/p" build.sh) && $CXX "${FLAGS[@]}" -c %s -o %s'
                        % (tree, src, obj)], capture_output=True, text=True)
    return r.returncode == 0


def fn_symbols(text):
    """the function each block defines, as objdump -C prints it without parameters: Class::Method or name"""
    out = []
    for b in blocks(text)[1]:
        h = head_body(b)[0].splitlines()
        sig = re.sub(r'__attribute__\s*\(\(.*?\)\)', ' ', ' '.join(l for l in h if not l.startswith('//')))
        m = re.search(r'([~\w:]+)\s*\(', sig)
        out.append(m.group(1) if m else None)
    return out


def main(tree):
    srcd = os.path.join(tree, 'src'); objd = os.path.join(tree, 'build', 'obj')
    files = sorted(f for f in os.listdir(srcd) if f.endswith('.cpp'))
    report = {r: {'kept': 0, 'put_back': [], 'proof': PROOF[r]} for r, _ in RULES}
    check_aliases(tree)

    def one(fn):
        path = os.path.join(srcd, fn); base = fn[:-4]
        obj = os.path.join(objd, base + '.base.o')      # the untouched file, compiled as the rewrites are below
        if not compile_one(tree, os.path.join('src', fn), os.path.join('build', 'obj', base + '.base.o')): return fn, None
        before = objcode(obj); bdata = datasecs(obj); os.remove(obj)
        text = open(path).read()
        notes = {r: [0, []] for r, _ in RULES}
        for rname, rule in RULES:
            pre, bl = blocks(text)
            new = []
            back = {}
            for k, b in enumerate(bl):
                h, body = head_body(b)
                if rname == 'params':
                    ea = int(re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', b).group(1), 16)
                    h2, body2 = rule(h, body, ea); back[k] = {n: o for o, n in params_map(h, ea).items()}
                else: h2, body2 = rule(h, body)
                new.append(h2 + body2)
            cand = pre + ''.join(new)
            if cand == text: continue
            tmp = os.path.join(objd, base + '.shape.o')
            open(path, 'w').write(cand)
            ok = compile_one(tree, os.path.join('src', fn), os.path.join('build', 'obj', base + '.shape.o'))
            if PROOF[rname] == 'alpha':
                # each renamed function on its own: the file with only that function renamed, preprocessed, its new
                # names mapped back, must equal the original file preprocessed
                open(path, 'w').write(text); base_pp = preprocessed(tree, os.path.join('src', fn))
                keep = list(bl)
                for k, (b_old, b_new) in enumerate(zip(bl, new)):
                    if b_old == b_new: continue
                    trial = pre + ''.join(b_new if j == k else bl[j] for j in range(len(bl)))   # this function alone
                    open(path, 'w').write(trial)
                    pp = preprocessed(tree, os.path.join('src', fn))
                    good = pp is not None and base_pp is not None
                    if good:
                        good = [back[k].get(t, t) for t in pp] == base_pp
                    if good: keep[k] = b_new; notes[rname][0] += 1
                    else: notes[rname][1].append(re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', b_old).group(1))
                text = pre + ''.join(keep)
                open(path, 'w').write(text)
                if os.path.exists(tmp): os.remove(tmp)
                continue
            if PROOF[rname] == 'probe':
                # the probe on the original, then the rewrite of the rest compiled; a function the compiler
                # rejects (a uint32_t * where a global is undefined4 *) keeps undefined4, until the file compiles
                fails = probe_failures(tree, fn, text)
                for _ in range(6):
                    cand = pre + ''.join(o if k in fails else n for k, (o, n) in enumerate(zip(bl, new)))
                    open(path, 'w').write(cand)
                    r = subprocess.run(['bash', '-c', 'cd %s && source <(sed -n "/^CXX=/,/^OUTDLL=/p" build.sh) && $CXX "${FLAGS[@]}" -fsyntax-only src/%s'
                                        % (tree, fn)], capture_output=True, text=True)
                    if r.returncode == 0: break
                    lines = {int(m.group(1)) for m in re.finditer(r'src/[^:]*:(\d+):\d+: error', r.stderr)}
                    ln, more = pre.count('\n') + 1, set()
                    for k, (o, n) in enumerate(zip(bl, new)):
                        b = o if k in fails else n
                        if any(ln <= x <= ln + b.count('\n') for x in lines): more.add(k)
                        ln += b.count('\n')
                    if not more: fails = set(range(len(bl))); break
                    fails |= more
                else: fails = set(range(len(bl)))
                keep = []
                for k, (b_old, b_new) in enumerate(zip(bl, new)):
                    keep.append(b_old if k in fails else b_new)
                    if b_old != b_new:
                        if k in fails: notes[rname][1].append(re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', b_old).group(1))
                        else: notes[rname][0] += 1
                text = pre + ''.join(keep)
                open(path, 'w').write(text)
                if os.path.exists(tmp): os.remove(tmp)
                continue
            if PROOF[rname] != 'objcode':
                if PROOF[rname] == 'preprocessed':
                    open(path, 'w').write(text); pa = preprocessed(tree, os.path.join('src', fn))
                    open(path, 'w').write(cand); pb = preprocessed(tree, os.path.join('src', fn))
                    ok = ok and pa is not None and pa == pb
                for b_old, b_new in zip(bl, new):
                    if b_old != b_new:
                        if ok: notes[rname][0] += 1
                        else: notes[rname][1].append(re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', b_old).group(1))
                text = cand if ok else text
                open(path, 'w').write(text)
                if os.path.exists(tmp): os.remove(tmp)
                continue
            after = objcode(tmp) if ok else {}
            syms = fn_symbols(cand)
            # everything that is not one of the tree's functions (GCC's .part/.constprop pieces, inline helpers) and
            # every data section must be unchanged too, or the rule is put back in the whole file
            rest = (set(before) | set(after)) - set(syms)
            whole = ok and all(before.get(k) == after.get(k) for k in rest) and datasecs(tmp) == bdata
            keep = []
            for b_old, b_new, s in zip(bl, new, syms):
                same = whole and s is not None and before.get(s) is not None and before.get(s) == after.get(s)
                keep.append(b_new if same else b_old)
                if b_old != b_new:
                    if same: notes[rname][0] += 1
                    else: notes[rname][1].append(re.match(r'// FUNCTION: FFXIMAIN (0x[0-9A-F]+)', b_old).group(1))
            text = pre + ''.join(keep)
            open(path, 'w').write(text)
            if os.path.exists(tmp): os.remove(tmp)
        return fn, notes

    with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as ex:
        for fn, notes in ex.map(one, files):
            if not notes: continue
            for r, (k, back) in notes.items():
                if r not in report: continue
                report[r]['kept'] += k; report[r]['put_back'] += back
    json.dump(report, open(os.path.join(tree, 'source_shape.json'), 'w'), indent=1)
    print(' '.join('%s: %d kept, %d put back;' % (r, v['kept'], len(v['put_back'])) for r, v in report.items()))


if __name__ == '__main__':
    main(sys.argv[1])
