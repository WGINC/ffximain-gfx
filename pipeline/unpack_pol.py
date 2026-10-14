"""unpack_pol.py -- the shipped FFXiMain.dll with its code section filled in, for analysis (never into the repository).

    python3 unpack_pol.py SHIPPED_FFXiMain.dll OUT_unpacked.dll

The shipped DLL carries its code compressed in the POL1 section; .text is empty on disk. Its entry stub decompresses
POL1 into .text, applies relocations for the load address, and jumps to the real entry point. This runs that stub in
an emulator (Unicorn) with the image at its preferred base 0x10000000 (so no relocation is applied), stops at the
first instruction inside .text, and writes a PE file whose sections hold the image as the stub left it. The entry
point is set to where the stub went (as the 3 Sep unpacked copy has it).

Checks: the stub must reach .text without calling anything outside the image (it imports nothing it uses; any
access outside the mapped image stops the run), and .text must be filled to its virtual size."""
import struct, sys
import pefile
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP


def emulate(pe):
    base = pe.OPTIONAL_HEADER.ImageBase; size = pe.OPTIONAL_HEADER.SizeOfImage
    img = bytearray(pe.get_memory_mapped_image()); img += bytes(max(0, size - len(img)))
    text = pe.sections[0]
    if text.Name.rstrip(b'\0') != b'.text': sys.exit('unpack_pol: the first section is not .text')
    lo, hi = base + text.VirtualAddress, base + text.VirtualAddress + text.Misc_VirtualSize
    mu = Uc(UC_ARCH_X86, UC_MODE_32)
    mu.mem_map(base, (size + 0xFFF) & ~0xFFF); mu.mem_write(base, bytes(img[:size]))
    stack, ret = 0x00200000, 0x00300000
    mu.mem_map(stack, 0x100000); mu.mem_map(ret, 0x1000); mu.mem_write(ret, b'\xF4')
    sp = stack + 0xF0000
    for v in (0, 1, base, ret):                      # DllMain(hinst, DLL_PROCESS_ATTACH, 0), returning to ret
        sp -= 4; mu.mem_write(sp, struct.pack('<I', v))
    mu.reg_write(UC_X86_REG_ESP, sp)
    seen = {'oep': None, 'n': 0}

    def hook(uc, addr, sz, _):
        seen['n'] += 1
        if lo <= addr < hi: seen['oep'] = addr; uc.emu_stop()
    mu.hook_add(UC_HOOK_CODE, hook)
    try:
        mu.emu_start(base + pe.OPTIONAL_HEADER.AddressOfEntryPoint, ret, count=2_000_000_000)
    except UcError as e:
        sys.exit('unpack_pol: the stub stopped at 0x%08X: %s' % (mu.reg_read(UC_X86_REG_EIP), e))
    if seen['oep'] is None: sys.exit('unpack_pol: the stub returned without entering .text')
    return bytes(mu.mem_read(base, size)), seen['oep'], seen['n']


def main(src, out):
    pe = pefile.PE(src)
    image, oep, n = emulate(pe)
    base = pe.OPTIONAL_HEADER.ImageBase; text = pe.sections[0]
    filled = image[text.VirtualAddress:text.VirtualAddress + text.Misc_VirtualSize]
    if filled.count(0) == len(filled): sys.exit('unpack_pol: .text is still empty')
    # a PE whose file layout is its memory layout: every section's raw data at its virtual address
    hdr = bytearray(image[:pe.OPTIONAL_HEADER.SizeOfHeaders])
    pe2 = pefile.PE(data=bytes(hdr) + bytes(image[len(hdr):]), fast_load=True)
    pe2.OPTIONAL_HEADER.FileAlignment = pe2.OPTIONAL_HEADER.SectionAlignment
    pe2.OPTIONAL_HEADER.AddressOfEntryPoint = oep - base
    for s in pe2.sections:
        s.PointerToRawData = s.VirtualAddress
        s.SizeOfRawData = (s.Misc_VirtualSize + 0xFFF) & ~0xFFF
    data = bytearray(pe2.write())
    data += bytes(max(0, len(image) - len(data)))
    data[pe.OPTIONAL_HEADER.SizeOfHeaders:] = image[pe.OPTIONAL_HEADER.SizeOfHeaders:len(data)]
    hdr2 = pe2.write()[:pe.OPTIONAL_HEADER.SizeOfHeaders]; data[:len(hdr2)] = hdr2
    open(out, 'wb').write(bytes(data))
    chk = pefile.PE(out)
    assert chk.get_memory_mapped_image()[text.VirtualAddress:text.VirtualAddress + 64] == filled[:64]
    print('%s: TimeDateStamp %08X, stub ran %d instructions, entry point 0x%08X; wrote %s'
          % (src, pe.FILE_HEADER.TimeDateStamp, n, oep, out))


if __name__ == '__main__':
    if len(sys.argv) != 3: sys.exit(__doc__)
    main(*sys.argv[1:3])
