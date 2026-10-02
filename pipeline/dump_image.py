"""dump_image.py -- a module's mapped image out of a Windows minidump.

    python3 dump_image.py DUMP.dmp MODULE_NAME OUT.bin
Writes the module's address range [base, base + size) as one flat file; pages the dump does not hold
are zero. Prints the base address (the image is relocated there).
"""
import sys
from minidump.minidumpfile import MinidumpFile


def main(dmp, name, out):
    m = MinidumpFile.parse(dmp); r = m.get_reader()
    mod = next(x for x in m.modules.modules if x.name.lower().endswith(name.lower()))
    buf = bytearray(mod.size); held = 0
    for s in m.memory_segments_64.memory_segments:
        a, n = s.start_virtual_address, s.size
        lo, hi = max(a, mod.baseaddress), min(a + n, mod.baseaddress + mod.size)
        if lo >= hi: continue
        buf[lo - mod.baseaddress:hi - mod.baseaddress] = r.read(lo, hi - lo); held += hi - lo
    open(out, 'wb').write(buf)
    print('%s at 0x%08X, size 0x%X, %d of %d bytes held' % (mod.name, mod.baseaddress, mod.size, held, mod.size))


if __name__ == '__main__':
    main(*sys.argv[1:4])
