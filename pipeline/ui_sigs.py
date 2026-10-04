"""ui_sigs.py -- locate the owner's TkMenu functions (docs/owner_ui/tkmenu_signatures.txt) in FFXiMain.dll and check
each declaration in docs/owner_ui/TkMenuManagerFunctionDefinitions.ixx against the game's own code.

For each signature:
  matches     every place the byte pattern (with its ? mask) occurs in .text; a usable signature matches once
  purge       the function's `ret N` (data/purges.json, keyed by function start)
  declared    stack bytes the declaration passes: every parameter after `this` (__thiscall) or every parameter
              (__stdcall), 4 bytes each (no parameter in these declarations is wider than 4 bytes)
  ecx_input   whether ECX is read before it is written on some path from the entry (all paths followed to `ret`;
              None when a path reaches an indirect jump, or a call with ECX unwritten whose callee does not
              write ECX first (writes_ecx_first): that callee may read it); `push ecx` alone is MSVC
              reserving a stack slot and is not counted as a read
  this_at_calls  for __thiscall: the last `mov`/`lea ecx` before each direct call, in bytes before the call that decode
              back to it (a listing over a linear decode, not a proof of the object's type)
Usage: python3 ui_sigs.py FFXiMain.dll out.json"""
import collections, json, os, re, struct, sys
import capstone, pefile
from capstone import x86

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
DOC = os.path.join(ROOT, 'docs', 'owner_ui')
# signature name -> declaration name (the two files name them differently)
NAMES = {'fnCreateDrawMenu': 'fnCreateDrawMenu', 'fnCloseDrawMenu': 'fnCloseDrawMenu', 'fnConvertMenuId': 'fnConvertMenuId',
         'fnGetFromPrimTable': 'fnGetFromPrimTable', 'fnIsEnableCreateDrawMenu': 'fnIsEnableCreateDrawMenu',
         'GetResourceMenu': 'fnGetResourceMenu', 'GetMenuCtrlData': 'fnGetMenuCtrlData',
         'TkMenuCtrlDataRePosition': 'fnTkMenuCtrlDataRePosition', 'TkMenuCtrlDataResizeWindow': 'fnTkMenuCtrlDataResize',
         'fnDestroyDrawMenu': 'fnDestroyDrawMenu', 'fnOnKeyDown': 'fnOnKeyDown'}
ECX = {'ecx', 'cx', 'cl', 'ch'}

def writes_ecx_first(md, pe, base, ea):
    """True if the straight-line code at ea (up to its first branch, call or ret) writes ECX before reading it:
    then ECX is not an input of that function and holds its value after any call to it."""
    for i in md.disasm(pe.get_data(ea - base, 64), ea):
        r, w = i.regs_access()
        if {i.reg_name(x) for x in r} & ECX and not (i.mnemonic == 'push' and i.op_str == 'ecx'): return False
        if 'ecx' in {i.reg_name(x) for x in w}: return True
        if i.group(x86.X86_GRP_JUMP) or i.group(x86.X86_GRP_CALL) or i.group(x86.X86_GRP_RET): return False
    return False

def ecx_input(md, pe, base, ea):
    """True if some path from ea reads ECX before writing it; None if a path reaches a call or an indirect jump first."""
    seen, work, unknown = set(), [ea], False
    while work:
        a = work.pop()
        if a in seen: continue
        seen.add(a)
        for i in md.disasm(pe.get_data(a - base, 16), a, 1):
            r, w = i.regs_access()
            rn = {i.reg_name(x) for x in r}; wn = {i.reg_name(x) for x in w}
            if rn & ECX and not (i.mnemonic == 'push' and i.op_str == 'ecx'): return True
            if 'ecx' in wn or i.group(x86.X86_GRP_RET): break
            if i.group(x86.X86_GRP_CALL):
                op = i.operands[0]
                # ECX still the entry value: unknown unless the callee writes ECX before reading it (writes_ecx_first)
                if not (op.type == x86.X86_OP_IMM and writes_ecx_first(md, pe, base, op.imm)): unknown = True
                break
            if i.group(x86.X86_GRP_JUMP):
                op = i.operands[0]
                if op.type != x86.X86_OP_IMM: unknown = True; break
                work.append(op.imm)
                if i.mnemonic != 'jmp': work.append(i.address + i.size)
                break
            work.append(i.address + i.size)
    return None if unknown else False

def main(dll, out_path):
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
    text = next(s for s in pe.sections if s.Name.rstrip(b'\0') == b'.text'); data = text.get_data(); va0 = base + text.VirtualAddress
    purges = json.load(open(os.path.join(ROOT, 'data', 'purges.json')))
    src = open(os.path.join(DOC, 'tkmenu_signatures.txt')).read()
    pat = dict(re.findall(r'(\w+?)Pattern\s*=\s*"([^"]*)"', src)); msk = dict(re.findall(r'(\w+?)Mask\s*=\s*"([^"]*)"', src))
    decl = {}
    for name, ret, cc, params in re.findall(r'using (\w+) = (.*?)\((__thiscall|__stdcall)\*\)\((.*?)\);', open(os.path.join(DOC, 'TkMenuManagerFunctionDefinitions.ixx')).read()):
        ps = [p.strip() for p in params.split(',') if p.strip()]
        decl[name] = {'ret': ret.strip(), 'cc': cc, 'params': ps, 'stack_bytes': 4 * (len(ps) - (cc == '__thiscall'))}
    calls = collections.defaultdict(list)
    for i in range(len(data) - 5):
        if data[i] == 0xE8: calls[(va0 + i + 5 + struct.unpack_from('<i', data, i + 1)[0]) & 0xFFFFFFFF].append(va0 + i)
    out = {}
    for k, p in pat.items():
        b = bytes(int(x, 16) for x in re.findall(r'\\x([0-9a-fA-F]{2})', p)); m = msk[k]
        assert len(b) == len(m), k
        rx = re.compile(b''.join(b'.' if c == '?' else re.escape(bytes([v])) for v, c in zip(b, m)), re.S)
        hits = ['0x%08X' % (va0 + x.start()) for x in rx.finditer(data)]
        r = {'matches': hits}; d = decl[NAMES[k]]; r['declared'] = d
        if len(hits) == 1:
            ea = int(hits[0], 16); r['ea'] = hits[0]; r['purge'] = purges.get(hits[0])
            r['purge_matches_declared'] = r['purge'] == d['stack_bytes']
            r['ecx_input'] = ecx_input(md, pe, base, ea)
            r['cc_matches_declared'] = r['ecx_input'] is not None and r['ecx_input'] == (d['cc'] == '__thiscall')
            src_ecx = collections.Counter()
            for c in calls.get(ea, ()):
                last = None
                for back in range(24, 1, -1):
                    ins = list(md.disasm(pe.get_data(c - back - base, back), c - back))
                    if ins and ins[-1].address + ins[-1].size == c:
                        for x in ins:
                            if x.mnemonic in ('mov', 'lea') and x.op_str.startswith('ecx,'): last = '%s %s' % (x.mnemonic, x.op_str)
                        break
                src_ecx[last or 'not found in the preceding bytes'] += 1
            r['direct_calls'] = len(calls.get(ea, ()))
            if d['cc'] == '__thiscall': r['this_at_calls'] = dict(src_ecx.most_common())
        out[k] = r
        print('%-28s %s purge=%s declared=%s ecx_input=%s calls=%s' % (k, hits, r.get('purge'), d['stack_bytes'], r.get('ecx_input'), r.get('direct_calls')))
    json.dump(out, open(out_path, 'w'), indent=1)

if __name__ == '__main__':
    main(*sys.argv[1:3])
