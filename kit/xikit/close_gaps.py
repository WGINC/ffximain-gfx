"""close_gaps.py -- make the in-game reconstruction DLL loadable by the Windows loader, without changing its code.

tree/build.sh pins each section kind at a fixed base (.rdata 0x800000, .data 0xA00000, .bss 0xB00000 past the
image base) so the gate cache can reuse verdicts. That leaves gaps in the virtual layout between sections. Wine
maps such an image; Windows refuses it (LoadLibrary error 193, ERROR_BAD_EXE_FORMAT), since each section must
start where the previous one ends, rounded up to SectionAlignment. This extends each section's VirtualSize to
reach the next section, so the gap becomes zero-filled memory owned by the section before it. No byte of any
section's contents and no address changes: the code is the code the gate ran.

    python3 close_gaps.py DLL        (in place; prints each gap closed)"""
import sys
import pefile


def main(path):
    pe = pefile.PE(path)
    al = pe.OPTIONAL_HEADER.SectionAlignment
    up = lambda n: (n + al - 1) // al * al
    secs = sorted(pe.sections, key=lambda s: s.VirtualAddress)
    n = 0
    for a, b in zip(secs, secs[1:]):
        end = a.VirtualAddress + up(max(a.Misc_VirtualSize, 1))
        if end > b.VirtualAddress:
            sys.exit('%s: section %s overlaps %s' % (path, a.Name, b.Name))
        if end < b.VirtualAddress:
            print('%-8s VirtualSize %#x -> %#x (gap %#x closed)' % (a.Name.rstrip(b'\0').decode(), a.Misc_VirtualSize,
                  b.VirtualAddress - a.VirtualAddress, b.VirtualAddress - end))
            a.Misc_VirtualSize = b.VirtualAddress - a.VirtualAddress; n += 1
    last = secs[-1]
    if pe.OPTIONAL_HEADER.SizeOfImage != last.VirtualAddress + up(last.Misc_VirtualSize):
        sys.exit('%s: SizeOfImage %#x is not the end of the last section' % (path, pe.OPTIONAL_HEADER.SizeOfImage))
    data = pe.write()
    pe.close()
    open(path, 'wb').write(data)
    print('%d gaps closed in %s' % (n, path))


if __name__ == '__main__':
    main(sys.argv[1])
