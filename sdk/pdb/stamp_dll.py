#!/usr/bin/env python3
"""stamp_dll.py -- a copy of FFXiMain.dll whose debug record names FFXiMain.pdb, so that Visual Studio loads it.

    python stamp_dll.py FFXiMain.dll FFXiMain.pdb OUT.dll          (Python 3, nothing else; runs on Windows)

Why: a debugger matches a PDB to a module by the module's CodeView record (RSDS: the PDB's GUID and age, then its
file name). The game's DLL has one debug directory entry of type 2 (CodeView) with 80 bytes of data in the file at
PointerToRawData, but those bytes are not an RSDS record (noise in build 6A995428, zeros in 6AC37846 and 6AE818B4),
so Visual Studio has nothing to match (x64dbg/x32dbg load a PDB without matching; WinDbg does with `.reload /i`).

What it changes: only those 80 bytes, in the copy: 'RSDS', the PDB's GUID and age (read from the PDB's own info
stream, stream 1 of its MSF file), and the name "FFXiMain.pdb". The DLL's code, data, headers and the debug directory
entry itself are left as they are. The copy is not the game's file: PlayOnline's file check would see it differ and
fetch the original again. Use it only for a debugging session, and keep the original.
"""
import shutil, struct, sys

def pdb_guid_age(path):
    d = open(path, 'rb').read()
    if not d.startswith(b'Microsoft C/C++ MSF 7.00\r\n\x1aDS\0\0\0'): sys.exit('%s is not a PDB (MSF 7.00)' % path)
    bs, _, nblocks, dirbytes, _, bmap = struct.unpack_from('<6I', d, 32)
    ndir = (dirbytes + bs - 1) // bs
    dblocks = struct.unpack_from('<%dI' % ndir, d, bmap * bs)
    dirdata = b''.join(d[b * bs:(b + 1) * bs] for b in dblocks)[:dirbytes]
    n = struct.unpack_from('<I', dirdata, 0)[0]; sizes = struct.unpack_from('<%dI' % n, dirdata, 4)
    pos = 4 + 4 * n; blocks = []
    for sz in sizes:
        k = 0 if sz == 0xFFFFFFFF else (sz + bs - 1) // bs
        blocks.append(struct.unpack_from('<%dI' % k, dirdata, pos)); pos += 4 * k
    info = b''.join(d[b * bs:(b + 1) * bs] for b in blocks[1])     # stream 1: the PDB info stream
    version, signature, age = struct.unpack_from('<3I', info, 0)
    return info[12:28], age                                          # the GUID as stored (Data1..Data3 little-endian)

def main(dll, pdb, out):
    d = bytearray(open(dll, 'rb').read())
    pe = struct.unpack_from('<I', d, 0x3C)[0]
    assert d[pe:pe + 4] == b'PE\0\0', 'not a PE file'
    opt = pe + 24; nsec = struct.unpack_from('<H', d, pe + 6)[0]; optsize = struct.unpack_from('<H', d, pe + 20)[0]
    assert struct.unpack_from('<H', d, opt)[0] == 0x10B, 'not a 32-bit image'
    drva, dsize = struct.unpack_from('<II', d, opt + 96 + 6 * 8)         # data directory 6: debug
    secs = [struct.unpack_from('<8sIIII', d, opt + optsize + 40 * i) for i in range(nsec)]
    def off(rva):
        for _, vsz, va, rsz, raw in secs:
            if va <= rva < va + max(vsz, rsz): return raw + rva - va
        sys.exit('debug directory at RVA %X is in no section' % rva)
    ents = [struct.unpack_from('<IIHHIIII', d, off(drva) + 28 * i) for i in range(dsize // 28)]
    cv = [e for e in ents if e[4] == 2]
    if len(cv) != 1: sys.exit('expected one CodeView debug entry, found %d' % len(cv))
    size, raw = cv[0][5], cv[0][7]
    guid, age = pdb_guid_age(pdb)
    rec = b'RSDS' + guid + struct.pack('<I', age) + b'FFXiMain.pdb\0'
    if len(rec) > size: sys.exit('the entry holds %d bytes, the record needs %d' % (size, len(rec)))
    d[raw:raw + size] = rec + b'\0' * (size - len(rec))
    open(out, 'wb').write(bytes(d))
    g = struct.unpack('<IHH8s', guid)
    print('%s: RSDS {%08X-%04X-%04X-%s-%s} age %d, "FFXiMain.pdb", in the %d bytes at file offset 0x%X; nothing else changed'
          % (out, g[0], g[1], g[2], g[3][:2].hex().upper(), g[3][2:].hex().upper(), age, size, raw))

if __name__ == '__main__':
    if len(sys.argv) != 4: sys.exit(__doc__)
    main(*sys.argv[1:])
