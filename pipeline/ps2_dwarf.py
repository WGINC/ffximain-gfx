"""ps2_dwarf.py -- the PS2 debug build's own record of each matched function: its source file, its parameters and its
local variables, with their names and types.

The disc executable SCUS_972.66 (PS2_DEBUG_ELF; never in this repository) carries DWARF version 1 from Metrowerks
CodeWarrior: for every function, its compile unit (the source file's path on the developer's machine), and its
formal parameters and local variables with their names and types. DancingMad's parser reads it
(DANCINGMAD/tools/ps2_dwarf/dwarf1.py, the same `parse`); the type naming below follows that project's index.py.

A PC function matched to a PS2 symbol (data/ps2_matches.json) gets the record of the PS2 function of that mangled
name (DWARF attribute 0x200 holds it; the two PS2 builds share their symbol names). Nothing here says which PC
variable a PS2 local is: that needs its own evidence, and the record only lists them.

    PS2_DEBUG_ELF=... DANCINGMAD=... python3 ps2_dwarf.py data/ps2_dwarf_funcs.json
Output {"0xPC": {"ps2": "mangled", "file": "C:\\\\...\\\\x.cpp", "params": [[name, type], ...] (without `this`),
"this": "Class *" | null, "locals": [[name, type], ...]}}"""
import json, os, struct, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import ps2_elf
D = os.path.join(P, '..', 'data')
TAG_FORMAL, TAG_GLOBAL_SUB, TAG_LEXICAL, TAG_LOCAL, TAG_CU, TAG_SUB = 0x05, 0x06, 0x0B, 0x0C, 0x11, 0x14
AT_NAME, AT_MANGLED = 0x03, 0x200


def load_parser():
    sys.path.insert(0, os.path.join(os.environ.get('DANCINGMAD', os.path.join(P, '..', '..', 'dancingmad')), 'tools', 'ps2_dwarf'))
    import dwarf1
    return dwarf1


def main(out):
    dw = load_parser()
    elf = ps2_elf.Elf(os.environ['PS2_DEBUG_ELF'])
    sec = [s for s in elf.secs if s[0] == '.debug'][0]
    dies = dw.parse(elf.b[sec[5]:sec[5] + sec[6]])
    by_off = {d[0]: d for d in dies}
    children = {}
    for d in dies:
        if d[3] is not None: children.setdefault(d[3], []).append(d)

    def ref_name(ref):
        d = by_off.get(ref)
        if d is None: return '<ref 0x%x>' % ref
        if AT_NAME in d[2]: return d[2][AT_NAME]
        return 'array' if d[1] == 0x01 else 'func_type' if d[1] == 0x15 else '<anon 0x%x>' % d[1]

    def type_name(a):
        if dw.AT_FUND_TYPE in a: return dw.FUND.get(a[dw.AT_FUND_TYPE], 'fund0x%x' % a[dw.AT_FUND_TYPE])
        if dw.AT_USER_DEF_TYPE in a: return ref_name(a[dw.AT_USER_DEF_TYPE])
        for k, is_ref in ((dw.AT_MOD_FUND_TYPE, False), (dw.AT_MOD_U_D_TYPE, True)):
            if k in a:
                blk = a[k]; tail = 4 if is_ref else 2
                mods, raw = blk[:-tail], blk[-tail:]
                s = ref_name(struct.unpack('<I', raw)[0]) if is_ref else dw.FUND.get(struct.unpack('<H', raw)[0], '?')
                for m in reversed(mods):
                    s = '%s%s' % (dw.MODS.get(m, '?'), s) if m in (3, 4) else '%s%s' % (s, dw.MODS.get(m, '?'))
                return s
        return '?'

    def unit_of(d):
        while d is not None and d[1] != TAG_CU: d = by_off.get(d[3])
        return d[2].get(AT_NAME) if d else None

    def walk_locals(off, out_l):
        for c in children.get(off, []):
            if c[1] == TAG_LOCAL: out_l.append([c[2].get(AT_NAME, '?'), type_name(c[2])])
            elif c[1] == TAG_LEXICAL: walk_locals(c[0], out_l)

    funcs = {}
    for d in dies:
        if d[1] in (TAG_GLOBAL_SUB, TAG_SUB) and AT_MANGLED in d[2]:
            funcs.setdefault(d[2][AT_MANGLED], d)
        elif d[1] in (TAG_GLOBAL_SUB, TAG_SUB) and AT_NAME in d[2]:
            funcs.setdefault(d[2][AT_NAME], d)
    m = json.load(open(os.path.join(D, 'ps2_matches.json')))
    res, missing = {}, []
    for k, v in sorted(m.items()):
        if k.startswith('_'): continue
        d = funcs.get(v['ps2'])
        if d is None: missing.append(v['ps2']); continue
        params, this = [], None
        for c in children.get(d[0], []):
            if c[1] != TAG_FORMAL: continue
            nm, t = c[2].get(AT_NAME, '?'), type_name(c[2])
            if nm == 'this': this = t
            else: params.append([nm, t])
        locs = []; walk_locals(d[0], locs)
        res[k] = {'ps2': v['ps2'], 'file': unit_of(d), 'this': this, 'params': params, 'locals': locs}
    json.dump(res, open(out, 'w'), indent=1, ensure_ascii=False)
    print('%d of %d matched functions found in the debug data (%d not: %s)' % (len(res), len(res) + len(missing), len(missing), missing[:5]))


if __name__ == '__main__':
    main(sys.argv[1])
