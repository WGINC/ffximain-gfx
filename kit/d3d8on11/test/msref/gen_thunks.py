"""gen_thunks.py D3D8THK_DLL OUT_DIR -- from Microsoft's d3d8thk.dll (the DirectX 8.1 SDK's, beside its d3d8.dll),
writes OUT_DIR/thk.def and OUT_DIR/stubs.inc for kthunk.c: one export per thunk, each stub taking as many 4-byte
arguments as the real thunk pops (its `ret N`, N / 4), logging its call and returning 0. kthunk.c defines
IMPL_<name> for the thunks it implements, and the stub is left out."""
import sys, os, pefile, capstone

dll, out = sys.argv[1], sys.argv[2]
pe = pefile.PE(dll); m = pe.get_memory_mapped_image(); base = pe.OPTIONAL_HEADER.ImageBase
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
defs, stubs = ['LIBRARY d3d8thk', 'EXPORTS'], []
for s in pe.DIRECTORY_ENTRY_EXPORT.symbols:
    n = s.name.decode()
    rets = [i for i in md.disasm(m[s.address:s.address + 32], base + s.address) if i.mnemonic == 'ret']
    if not rets: sys.exit('%s: no ret in its first 32 bytes; not a thunk this script knows' % n)
    nb = int(rets[0].op_str, 16) if rets[0].op_str else 0
    if nb % 4: sys.exit('%s: ret %d is not a whole number of 4-byte arguments' % (n, nb))
    k = nb // 4
    defs.append('  %s=%s@%d' % (n, n, nb))
    args = ', '.join('DWORD a%d' % i for i in range(k)) or 'void'
    fmt = ' '.join(['%08lx'] * k); vals = ''.join(', (unsigned long)a%d' % i for i in range(k))
    stubs += ['#ifndef IMPL_%s' % n,
              'DWORD WINAPI %s(%s){lg("%s%s"%s);return 0;}' % (n, args, n, (' ' + fmt) if k else '', vals), '#endif']
open(os.path.join(out, 'thk.def'), 'w').write('\n'.join(defs) + '\n')
open(os.path.join(out, 'stubs.inc'), 'w').write('\n'.join(stubs) + '\n')
print('%d thunks' % (len(defs) - 2))
