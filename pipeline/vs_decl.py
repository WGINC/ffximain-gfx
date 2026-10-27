"""vs_decl.py -- decode the D3D8 vertex shader declarations the game keeps in its image.

    FFXI_DLL=... python3 pipeline/vs_decl.py ADDR [ADDR...]

Reads DWORD tokens at each address of the DLL's image (image base from the PE header) up to D3DVSD_END
(0xFFFFFFFF) and decodes them with kit/d3dprof/vshader.py's decl(), whose token table is the D3D8 SDK's
(d3d8types.h: D3DVSD_TOKEN_* in bits 29-31, D3DVSDT_* in bits 16-19, register in bits 0-4). The addresses are
the declaration arguments of the game's CreateVertexShader calls (docs/FORMATS.md lists them with their sites).
Reads only; nothing from the game image is written to the repo.
"""
import os, struct, sys
import pefile

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'kit', 'd3dprof'))
from vshader import decl  # noqa: E402


def tokens(img, base, ea, limit=64):
    out = []
    for k in range(limit):
        t = struct.unpack_from('<I', img, ea - base + 4 * k)[0]
        out.append(t)
        if t == 0xFFFFFFFF:
            break
    return out


def main(args):
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image()
    for a in args:
        ea = int(a, 16)
        t = tokens(img, base, ea)
        print('%08X  %s' % (ea, ' '.join('%08X' % x for x in t)))
        for line in decl(t):
            print('    ' + line)


if __name__ == '__main__':
    main(sys.argv[1:])
