"""idb_members.py -- every member of every struct in the IDB's types header (data/gfx_types_game.h), with the
offset and size the compiler gives it.

Hex-Rays prints a class access as a member of the IDB's struct (`this->field_1EC`, `g_pCDx->_pad_10_1F4[1849]`,
`this->base.base._pad_104`); the member it chose is the one the IDB struct places at the accessed offset, so
the offset of that member in the same struct is the offset the code accesses. The offsets are not computed
here by hand: a translation unit asks i686-w64-mingw32-g++ for offsetof/sizeof of each member of the header
as C++ reads it (hx2cpp.types_header), and the values are read back from the object file.

Output {struct: {"size": n, "members": [[name, offset, size, type, dims]]}} where type is the member's
declared type without its name and dims its array dimensions ([] for none); a function pointer member's
type is the full pointer type. Usage: python3 idb_members.py out.json"""
import json, os, re, struct, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import hx2cpp

def parse(text):
    """{struct: [(name, type, dims)]} from the header's struct blocks"""
    out = {}
    for m in re.finditer(r'^struct (?:/\*VFT\*/ )?(\w+)\n\{\n(.*?)^\};', text, re.M | re.S):
        mem = []
        for line in m.group(2).split('\n'):
            line = line.strip()
            if not line or line.startswith(('//', '/*')): continue
            assert line.endswith(';'), (m.group(1), line)
            d = line[:-1].strip()
            # the member's name is in the first `(__cc *name)` / `(*name)`: a parameter list can hold `(__int16 *self)` too
            fp = re.match(r'^(.*?\(\s*__\w+\s*\*\s*)(\w+)(\).*)$', d) or re.match(r'^(.*?\(\s*\*\s*)(\w+)(\).*)$', d)
            if fp:
                mem.append((fp.group(2), fp.group(1) + fp.group(3), [])); continue
            a = re.match(r'^(.*?)\b(\w+)((?:\[\d+\])*)$', d)
            assert a and ':' not in d, (m.group(1), line)
            mem.append((a.group(2), a.group(1).strip(), [int(x) for x in re.findall(r'\[(\d+)\]', a.group(3))]))
        out[m.group(1)] = mem
    return out

def main(out_path):
    text = hx2cpp.types_header()
    structs = parse(text)
    order = [(s, n) for s in structs for n, _, _ in structs[s]]
    with tempfile.TemporaryDirectory() as d:
        open(os.path.join(d, 'gfx_types_game.h'), 'w').write(text)
        src = ['#include "xi_hexrays.hpp"', '#include "xi_d3d8.hpp"', '#include "gfx_types_game.h"', '#include <stddef.h>',
               'extern "C" { extern const unsigned xi_sizes[] = {']
        src += ['  sizeof(%s),' % s for s in structs] + ['};', 'extern const unsigned xi_offs[] = {']
        src += ['  offsetof(%s, %s), sizeof(((%s *)0)->%s),' % (s, n, s, n) for s, n in order] + ['}; }']
        open(os.path.join(d, 'probe.cpp'), 'w').write('\n'.join(src) + '\n')
        P = os.path.dirname(os.path.abspath(__file__))
        subprocess.run(['i686-w64-mingw32-g++', '-std=c++17', '-w', '-fpermissive', '-c', '-I' + d, '-I' + P,
                        '-I' + os.path.join(P, '..', 'data'), os.path.join(d, 'probe.cpp'), '-o', os.path.join(d, 'probe.o')], check=True)
        subprocess.run(['i686-w64-mingw32-objcopy', '-O', 'binary', '-j', '.rdata', os.path.join(d, 'probe.o'), os.path.join(d, 'rd.bin')], check=True)
        sym = subprocess.run(['i686-w64-mingw32-nm', os.path.join(d, 'probe.o')], capture_output=True, text=True, check=True).stdout
        addr = {n: int(a, 16) for a, k, n in re.findall(r'^([0-9a-f]+) (\w) (\w+)$', sym, re.M)}
        rd = open(os.path.join(d, 'rd.bin'), 'rb').read()
    def words(name, n): return struct.unpack_from('<%dI' % n, rd, addr[name])
    sizes = words('_xi_sizes', len(structs)); offs = words('_xi_offs', 2 * len(order))
    res, k = {}, 0
    for (s, mem), sz in zip(structs.items(), sizes):
        rows = []
        for n, t, dims in mem:
            rows.append([n, offs[2 * k], offs[2 * k + 1], t, dims]); k += 1
        res[s] = {'size': sz, 'members': rows}
    json.dump(res, open(out_path, 'w'), indent=0, sort_keys=True)
    print('%d structs, %d members' % (len(res), len(order)))

if __name__ == '__main__':
    main(sys.argv[1])
