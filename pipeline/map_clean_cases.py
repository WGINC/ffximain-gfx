"""map_clean_cases.py -- point the raw gate cases at the clean DLL's exports (demangled names).

A function's export is found by its placed name (placement.json). Where more than one export has that name
(overloads), the export is the one whose parameter count equals the function's declaration in the tree's
headers (include/*.hpp: each declaration carries `// 0xADDR`). A name still ambiguous after that is not mapped,
and is listed as such: the gate does not pick one.

    python3 map_clean_cases.py DLL TREE/placement.json RAW_CASES.json OUT.json"""
import glob, json, os, re, subprocess, sys


def n_params(params):
    """parameter count of a parameter list's text (without the outer parentheses)"""
    s = params.strip(); depth = 0; n = 1
    if s in ('', 'void'): return 0
    for ch in s:
        if ch in '(<[': depth += 1
        elif ch in ')>]': depth -= 1
        elif ch == ',' and depth == 0: n += 1
    return n


def outer_params(text):
    """the text between the first top-level `(` and its matching `)`"""
    i = text.index('('); depth = 0
    for j in range(i, len(text)):
        depth += text[j] == '('; depth -= text[j] == ')'
        if depth == 0: return text[i + 1:j]
    raise ValueError(text)


def declared_arity(include_dir):
    """{address: parameter count} from the headers' `...(...);  // 0xADDR` declarations"""
    res = {}
    for p in glob.glob(os.path.join(include_dir, '*.hpp')):
        for line in open(p, errors='replace'):
            m = re.search(r'\);\s*//\s*0x([0-9A-Fa-f]{8})\b', line)
            if m: res[int(m.group(1), 16)] = n_params(outer_params(line))
    return res


def main(dll, placement, raw_cases, out):
    P = json.load(open(placement))
    arity = declared_arity(os.path.join(os.path.dirname(os.path.abspath(placement)), 'include'))
    exp = subprocess.run(['i686-w64-mingw32-objdump', '-p', dll], capture_output=True, text=True).stdout
    tbl = exp.split('[Ordinal/Name Pointer] Table', 1)[1]
    names = re.findall(r'^\s*\[\s*\d+\]\s+(\S+)\s*$', tbl, re.M)
    dem = subprocess.run(['i686-w64-mingw32-c++filt', '-n'], input='\n'.join(names), capture_output=True, text=True).stdout.split('\n')
    by = {}
    for n, d in zip(names, dem):
        by.setdefault(d.split('(')[0].strip(), []).append((n, n_params(outer_params(d)) if '(' in d else None))
    cases = []; missing = []; ambiguous = []
    for c in json.load(open(raw_cases)):
        ea = int(c['original'], 16); p = P.get(hex(ea))
        if not p: continue
        key = p.get('symbol') or ('%s::%s' % (p['class'], p['method']) if p['class'] else None)
        if key is None:
            cands = [k for k in by if '::' not in k and k.split()[-1] in (c['name'].split()[0], 'fn_%08X' % ea, 'fn_%08x' % ea)]
            if len(cands) > 1: ambiguous.append(hex(ea)); continue
            key = cands[0] if cands else None
        if not key or key not in by: missing.append(hex(ea)); continue
        ex = by[key]
        if len(ex) > 1: ex = [e for e in ex if arity.get(ea) is not None and e[1] == arity[ea]]
        if len(ex) != 1: ambiguous.append(hex(ea)); continue
        c = dict(c); c['candidate'] = ex[0][0]; cases.append(c)
    json.dump(cases, open(out, 'w'), indent=0)
    print(len(cases), 'mapped;', len(missing), 'missing', missing[:5], ';', len(ambiguous), 'ambiguous (not mapped)', ambiguous)


if __name__ == '__main__':
    main(*sys.argv[1:5])
