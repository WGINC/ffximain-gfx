"""reg_leftover_audit.py -- every read of a never-assigned ECX/EDX variable in the export, classified from the code.

Hex-Rays declares a local `// ecx` or `// edx` and reads it without assigning it where the original hands on
whatever the register holds (usually what the previous call left). C++ cannot reproduce that value, so the
read is exact only where the value is unused. Each read is classified:
  field-write   the variable is a struct whose field is assigned (`v.lpVtbl = ...`): not a leftover read
  harmless      passed as a register argument to a callee that never reads its entry ECX
                (vcall_resolve, data/ecx_use.json) or EDX (edx_inputs.py, data/edx_use.json)
  not proven    anything else, with the reason; the function's gate verdict is listed beside it (a gate PASS
                shows the read did not matter on the inputs tried, not that it never does)

    python3 reg_leftover_audit.py EXPORT.jsonl GATE.jsonl OUT.json
"""
import json, os, re, sys
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(exp, gate, out):
    use = {'ecx': json.load(open(os.path.join(D, 'ecx_use.json'))), 'edx': json.load(open(os.path.join(D, 'edx_use.json')))}
    verdict = {json.loads(l)['original']: json.loads(l)['verdict'] for l in open(gate)}
    res = []
    for l in open(exp):
        r = json.loads(l); body = r.get('text', '').partition('\n{')[2]
        for m in re.finditer(r'^\s+[\w\s\*]+?\s*\b(v\d+)(?:\[\d+\])?;\s*//\s*(ecx|edx)\s*$', body, re.M):
            v, reg = m.groups()
            if re.search(r'(?<![\w.>])%s\s*(=[^=]|\+=|-=|\|=|&=|\^=|<<=|>>=|\+\+|--)|(\+\+|--)%s\b|&%s\b' % (v, v, v), body): continue
            lines = [x.strip() for x in body.splitlines() if re.search(r'\b%s\b' % v, x) and not x.strip().endswith('// ' + reg)]
            if not lines: continue
            row = {'function': '0x%08X' % r['ea'], 'name': r['name'], 'var': v, 'register': reg, 'uses': lines,
                   'gate': verdict.get('0x%08X' % r['ea'])}
            if all(re.search(r'\b%s\.\w+\s*=[^=]' % v, x) or re.search(r'\b%s\.\w+->' % v, x) for x in lines) and \
               any(re.search(r'\b%s\.\w+\s*=[^=]' % v, x) for x in lines):
                row['class'] = 'field-write'
            else:
                why = []
                for x in lines:
                    c = next((c for c in re.finditer(r'\b(\w+)\(((?:[^()]|\([^()]*\))*)\)', x)
                              if v in [a.strip() for a in c.group(2).split(',')]), None)      # the innermost call taking it
                    o = r.get('objs', {}).get(c.group(1)) if c else None
                    args = [a.strip() for a in c.group(2).split(',')] if c else []
                    if not o or not o.get('func') or v not in args: why.append('not a direct call argument: ' + x); continue
                    cc = re.search(r'__(fastcall|thiscall)', o['type']); k = args.index(v)
                    inreg = {'fastcall': ('ecx', 'edx'), 'thiscall': ('ecx',)}.get(cc.group(1) if cc else '', ())
                    if k >= len(inreg) or inreg[k] != reg: why.append('passed on the stack or in another register: ' + x); continue
                    u = use[reg].get('0x%08X' % o['ea'])
                    if u is False: continue
                    why.append('%s (0x%08X) %s its entry %s' % (c.group(1), o['ea'], 'reads' if u else 'cannot be shown not to read', reg.upper()))
                row['class'] = 'harmless' if not why else 'not proven'
                if why: row['why'] = why
            res.append(row)
    json.dump(res, open(out, 'w'), indent=1)
    by = {}
    for x in res: by[x['class']] = by.get(x['class'], 0) + 1
    print('%d leftover-register reads in %d functions: %s' % (len(res), len({x['function'] for x in res}), by))


if __name__ == '__main__':
    main(*sys.argv[1:4])
