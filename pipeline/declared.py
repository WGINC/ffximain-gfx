"""declared.py -- the gate's comparison with stated, intended differences: a changed function (a mod's fork, a
performance fix) run against the game's original, where every difference must be one the change declares.

A declaration file (JSON):

    {"calls": [
       {"may": "drop",   "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x1C"}},
       {"may": "change", "call": "IDirect3DDevice8::SetRenderState", "where": {"0": "0x1C"}, "args": [1]},
       {"may": "add",    "call": "0x1000CD00"}],
     "memory": [{"base": "this", "offset": "0x908", "size": 4}, {"base": "0x10456670", "size": 4}],
     "return": false}

  calls    a call is named as the gate logs it: a Direct3D method "Interface::Method" (data/com_spec.json, the D3D8
           headers' order) or a game function's analysed address (a stubbed callee). Argument indices count the
           method's parameters after `this` from 0, as the SDK documents them; "this" names the object.
           drop    the original makes the call, the change does not
           add     the change makes a call the original does not
           change  both make it; the listed arguments (default: any) may differ
           where   only calls whose arguments have these values (the original's call for drop and change, the
                   change's call for add)
  memory   bytes the change may leave different: `this` + offset, or an absolute address
  return   true: the return value may differ

What is compared, per trial (the same inputs on both sides, as the gate gives them):
  * the calls, aligned: identical calls are matched first (difflib's longest matching blocks over the whole
    call, target and arguments); in what is left between two matched calls, calls to the same target are paired
    in order (a `change`), and the rest are `drop`s and `add`s. The pairing is a reading of two call lists, not
    a fact about intent: a declaration that only one reading allows is reported as it was read;
  * the bytes left different in memory, the return value, the x87 stack, the stack cleanup and the callee-saved
    registers, exactly as the gate's own comparison does (diffemu_fixed.compare), after the declared differences
    are set aside. Stack cleanup and callee-saved registers can never be declared: they are the calling
    convention.

In declared mode the gate answers each stubbed or Direct3D call from the call itself (its target and arguments,
and how many identical calls came before it in the run), not from the run's single random stream, so a dropped or
added call does not change what every later call returns (diffemu_gfx.py, GfxMachine._keyed). Answers to calls
the input repair fixed (a fuzzed pointer made valid) are still keyed by call order per target."""
import copy, difflib, json, os, re

_D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
SPEC = json.load(open(os.path.join(_D, 'com_spec.json')))
IFACES = sorted(SPEC)


def target_of(name):
    m = re.fullmatch(r'(\w+)::(\w+)', name)
    if m:
        iface, meth = m.groups()
        if iface not in SPEC: raise ValueError('declared call %s: no interface %s in data/com_spec.json' % (name, iface))
        slot = next((i for i, x in enumerate(SPEC[iface]) if x['name'] == meth), None)
        if slot is None: raise ValueError('declared call %s: %s has no method %s' % (name, iface, meth))
        return 0xC0000000 | (IFACES.index(iface) << 8) | slot
    if re.fullmatch(r'0x[0-9A-Fa-f]+', name): return int(name, 16)
    raise ValueError('declared call %s: neither Interface::Method nor an address' % name)


def name_of(t):
    if t >> 28 == 0xC and (t >> 8) & 0xFF < len(IFACES):
        iface = IFACES[(t >> 8) & 0xFF]; slot = t & 0xFF
        if slot < len(SPEC[iface]): return '%s::%s' % (iface, SPEC[iface][slot]['name'])
    return '0x%08X' % t


def load(path_or_dict):
    d = json.load(open(path_or_dict)) if isinstance(path_or_dict, str) else path_or_dict
    out = {'calls': [], 'memory': [], 'return': bool(d.get('return'))}
    for i, c in enumerate(d.get('calls', [])):
        if c.get('may') not in ('drop', 'add', 'change'): raise ValueError('declared call #%d: "may" is drop, add or change' % i)
        out['calls'].append({'may': c['may'], 'target': target_of(c['call']), 'call': c['call'],
                             'where': {int(k) if k != 'this' else 'this': int(str(v), 0) for k, v in (c.get('where') or {}).items()},
                             'args': None if c.get('args') is None else [a if a == 'this' else int(a) for a in c['args']],
                             'text': json.dumps(c, sort_keys=True)})
    for m in d.get('memory', []):
        out['memory'].append({'base': m.get('base', 'this'), 'offset': int(str(m.get('offset', 0)), 0), 'size': int(str(m['size']), 0),
                              'text': json.dumps(m, sort_keys=True)})
    return out


def _val(call, k):
    if k == 'this': return call[1]
    return call[2][k] if k < len(call[2]) else None


def _where(d, call):
    return all(_val(call, k) == v for k, v in d['where'].items())


def align(ca, cb):
    """(kind, call_a, call_b) for every difference between the two call lists: drop, add, change"""
    out = []
    sm = difflib.SequenceMatcher(None, ca, cb, autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal': continue
        ra, rb = ca[i1:i2], cb[j1:j2]
        tm = difflib.SequenceMatcher(None, [c[0] for c in ra], [c[0] for c in rb], autojunk=False)
        for op2, a1, a2, b1, b2 in tm.get_opcodes():
            if op2 == 'equal':
                out += [('change', x, y) for x, y in zip(ra[a1:a2], rb[b1:b2])]
            else:
                out += [('drop', x, None) for x in ra[a1:a2]] + [('add', None, y) for y in rb[b1:b2]]
    return out


def _covers(d, kind, x, y):
    if d['may'] != kind: return False
    c = x if kind != 'add' else y
    if c[0] != d['target'] or not _where(d, c): return False
    if kind == 'change' and d['args'] is not None:
        n = max(len(x[2]), len(y[2]))
        diff = (['this'] if x[1] != y[1] else []) + [k for k in range(n) if _val(x, k) != _val(y, k)]
        if any(k not in d['args'] for k in diff): return False
    return True


def fmt(c): return '%s(this=0x%08X, %s)' % (name_of(c[0]), c[1] if isinstance(c[1], int) else 0, ', '.join(
    '0x%08X' % v if isinstance(v, int) else str(v) for v in c[2]))


def compare(a, b, ret, decl, this, base_compare):
    """(undeclared: a description or None, declared: {declaration text: count}) for one trial"""
    seen = {}
    if a.status.split(':')[0] != b.status.split(':')[0]:
        return base_compare(a, b, ret), seen
    for kind, x, y in align(list(a.calls), list(b.calls)):
        d = next((d for d in decl['calls'] if _covers(d, kind, x, y)), None)
        if d is None:
            what = {'drop': 'the original calls %s, the change does not' % fmt(x) if x else '',
                    'add': 'the change calls %s, the original does not' % fmt(y) if y else '',
                    'change': 'call changed: original %s / change %s' % (fmt(x), fmt(y)) if x and y else ''}[kind]
            return 'undeclared: ' + what, seen
        seen[d['text']] = seen.get(d['text'], 0) + 1
    b2 = copy.copy(b); b2.calls = list(a.calls); b2.changes = dict(b.changes)
    for k in set(a.changes) | set(b.changes):
        if a.changes.get(k) == b.changes.get(k): continue
        for m in decl['memory']:
            lo = ((this or 0) if m['base'] == 'this' else int(m['base'], 0)) + m['offset']
            if lo <= k < lo + m['size']:
                if k in a.changes: b2.changes[k] = a.changes[k]
                else: b2.changes.pop(k, None)
                seen[m['text']] = seen.get(m['text'], 0) + 1; break
    if decl['return'] and (a.eax, getattr(a, 'edx', None), a.st0) != (b.eax, getattr(b, 'edx', None), b.st0):
        b2.eax = a.eax; b2.st0 = a.st0
        if hasattr(a, 'edx'): b2.edx = a.edx
        seen['return'] = seen.get('return', 0) + 1
    why = base_compare(a, b2, ret)
    return ('undeclared: ' + why if why else None), seen
