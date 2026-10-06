"""case_abs_reads.py -- fuzz the globals a function's own code reads by fixed address, where its case does not.

A case fuzzes the globals Hex-Rays names in the function (gfx_cases.py: a pointer gets a buffer, an object of
known size its bytes). An object the IDB gives no size (`int g_afterimageTex[]`) was left at the image's initial
value in every fuzz trial, so a branch on it was never taken (Post_Afterimage 0x10012F40: `mov eax,[0x10456910];
test eax,eax; je` -- 259 of its 263 instructions never ran). This adds, as a fuzz range, each location the
function's instructions read at a fixed address in a writable, non-executable section of the image (the access's own size), that
the case does not already set (fuzz_ranges, globals, struct_globals, com_globals, global input_ranges) and that is
not one of the device holders. Both sides of the gate get the same inputs; in live mode the dump's values are
kept, as for any fuzz range. Nothing here is a fact about the code: it only widens the inputs.

    python3 case_abs_reads.py CASES_IN.json CASES_OUT.json        uses $FFXI_DLL, data/func_extents.json
"""
import json, os, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM
import pefile

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
DEVICE_DATA = {0x103D3CBC, 0x1045666C, 0x1066941C, 0x10669418}      # gfx_cases.DEVICE_DATA
_IMG = {}
STORES = {'mov', 'fstp', 'fst', 'fistp', 'fist', 'fnstsw', 'fnstcw', 'fstsw', 'fstcw'}


def _image():
    if not _IMG:
        p = pefile.PE(os.environ['FFXI_DLL']); base = p.OPTIONAL_HEADER.ImageBase
        _IMG['img'] = p.get_memory_mapped_image(); _IMG['base'] = base
        _IMG['writable'] = [(base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize) for s in p.sections
                            if s.Characteristics & 0x80000000 and not s.Characteristics & 0x20000000]   # writable data, not code
        _IMG['ext'] = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
        _IMG['md'] = Cs(CS_ARCH_X86, CS_MODE_32); _IMG['md'].detail = True
    return _IMG


def abs_reads(ea):
    """[(address, size)] the function reads at a fixed address in a writable section, in address order"""
    m = _image(); out = set()
    for lo, hi in m['ext'].get(ea, []):
        for i in m['md'].disasm(m['img'][lo - m['base']:hi - m['base']], lo):
            if i.mnemonic == 'lea' or i.mnemonic.startswith('nop'): continue
            reads = set(i.regs_access()[0])
            for n, o in enumerate(i.operands):
                if o.type != X86_OP_MEM or o.mem.base or o.mem.index: continue
                a = o.mem.disp & 0xFFFFFFFF
                if not any(w0 <= a < w1 for w0, w1 in m['writable']): continue
                # a plain store (`mov [g], x`, `fstp [g]`, `setne [g]`) does not read its destination
                if n == 0 and (i.mnemonic in STORES or i.mnemonic.startswith('set')): continue
                out.add((a, max(1, o.size)))
    return sorted(out)


def covered(case):
    r = [(int(g, 16), n) for g, n in case.get('fuzz_ranges', [])]
    # a pointer global, a COM object's global and a struct global each hold a 4-byte pointer
    r += [(int(g, 16), 4) for g in list(case.get('globals', {})) + list(case.get('com_globals', {})) + list(case.get('struct_globals', {}))]
    r += [(int(e['addr'], 16), 4) for e in case.get('input_ranges', []) if e.get('where') == 'global']
    return r


def add(case):
    """the case with its missing fixed-address reads added to fuzz_ranges (a new list; the case is not changed)"""
    have = covered(case); new = []
    for a, n in abs_reads(int(case['original'], 16)):
        if a in DEVICE_DATA or any(g <= a and a + n <= g + k for g, k in have): continue
        if any(g < a + n and a < g + k for g, k in have): continue        # overlaps something set otherwise
        new.append(['0x%08X' % a, n]); have.append((a, n))
    if not new: return case
    c = dict(case); c['fuzz_ranges'] = list(case.get('fuzz_ranges', [])) + new
    return c


def main(cin, cout):
    cases = json.load(open(cin)); out = [add(c) for c in cases]
    n = sum(1 for a, b in zip(cases, out) if a is not b)
    json.dump(out, open(cout, 'w'), indent=0)
    print('%d cases, %d with fixed-address reads added' % (len(out), n))


if __name__ == '__main__':
    main(*sys.argv[1:3])
