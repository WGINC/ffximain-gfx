"""dump_verify.py -- is a memory dump of the running game the build that was analysed?

For each minidump: the FFXiMain.dll module's base and the PE header's timestamp, then every executable
section of the analysed DLL relocated to that base (its own relocation table) and compared byte for byte
with the dump's memory. A dump is the analysed build only if every code byte matches; anything else is
reported with the ranges that differ (and the pages the dump does not hold), never assumed to be close
enough.

    python3 dump_verify.py DUMP.dmp [DUMP.dmp ...]        uses $FFXI_DLL; prints one JSON record per dump
"""
import json, os, struct, sys
import pefile
from minidump.minidumpfile import MinidumpFile

IMAGE_SCN_MEM_EXECUTE = 0x20000000


def reloc_blocks(pe, img):
    """the relocation blocks of the `.reloc` section, read as one chain from the section's start. The unpacked
    DLL's header directory points at only the chain's last 87 blocks (pages 0x329000 on), so it does not
    cover .text; the chain from the section start runs unbroken to the section's end (893 blocks). Whether
    it is the right table is not assumed: the byte comparison below checks every relocated dword."""
    s = next(s for s in pe.sections if s.Name.rstrip(b'\0') == b'.reloc')
    a, end, out = s.VirtualAddress, s.VirtualAddress + s.Misc_VirtualSize, []
    while a + 8 <= end:
        page, size = struct.unpack_from('<II', img, a)
        if size < 8 or size % 2 or a + size > end: break
        out.append((page, [struct.unpack_from('<H', img, a + 8 + 2 * i)[0] for i in range((size - 8) // 2)])); a += size
    return out, a == end


def relocated(pe, base):
    """the analysed image as loaded at `base`: every HIGHLOW (type 3) entry gets the load delta; type 0 is
    padding; any other type is counted and reported, not applied"""
    img = bytearray(pe.get_memory_mapped_image()); delta = (base - pe.OPTIONAL_HEADER.ImageBase) & 0xFFFFFFFF
    blocks, whole = reloc_blocks(pe, img); other = 0
    for page, ents in blocks:
        for e in ents:
            t, off = e >> 12, e & 0xFFF
            if t == 3: struct.pack_into('<I', img, page + off, (struct.unpack_from('<I', img, page + off)[0] + delta) & 0xFFFFFFFF)
            elif t != 0: other += 1
    return bytes(img), {'blocks': len(blocks), 'chain_reaches_section_end': whole, 'unhandled_entries': other}


def verify(dmp, dll):
    m = MinidumpFile.parse(dmp)
    mod = [x for x in m.modules.modules if x.name.lower().endswith('ffximain.dll')]
    if not mod: return {'dump': dmp, 'result': 'no FFXiMain.dll module in the dump'}
    mod = mod[0]; base = mod.baseaddress
    r = m.get_reader().get_buffered_reader()
    def read(a, n):
        try: r.move(a); return r.read(n)
        except Exception: return None
    hdr = read(base, 0x400)
    ts = struct.unpack_from('<I', hdr, struct.unpack_from('<I', hdr, 0x3C)[0] + 8)[0] if hdr else None
    pe = pefile.PE(dll); img, rel = relocated(pe, base)
    out = {'dump': os.path.basename(dmp), 'module': mod.name.split('\\')[-1], 'base': '0x%08X' % base,
           'size': '0x%X' % mod.size, 'timestamp': '0x%08X' % ts if ts is not None else None,
           'analysed_timestamp': '0x%08X' % pe.FILE_HEADER.TimeDateStamp, 'relocations': rel, 'sections': {}}
    ok = rel['unhandled_entries'] == 0
    for s in pe.sections:
        if not s.Characteristics & IMAGE_SCN_MEM_EXECUTE: continue
        nm = s.Name.rstrip(b'\0').decode(errors='replace'); va = s.VirtualAddress; n = s.Misc_VirtualSize
        want = img[va:va + n]; diffs = []; missing = 0; same = 0
        for off in range(0, n, 0x1000):                       # page by page: a page the dump lacks is reported
            k = min(0x1000, n - off); got = read(base + va + off, k)
            if got is None or len(got) != k: missing += 1; continue
            for i in range(k):
                if got[i] != want[off + i]:
                    if diffs and diffs[-1][1] == off + i: diffs[-1][1] += 1
                    else: diffs.append([off + i, off + i + 1])
                else: same += 1
        out['sections'][nm] = {'_ranges': [[va + a, va + b] for a, b in diffs], 'bytes': n, 'same': same, 'differing_ranges': len(diffs),
                               'first_differences': ['0x%08X+%d' % (0x10000000 + va + a, b - a) for a, b in diffs[:10]],
                               'pages_missing': missing}
        ok &= not diffs and not missing
    # each differing range, as the dump has it: a `jmp rel32` out of the image at the range's start is code
    # another module patched in at run time (a hook), reported with the module it jumps to
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32); mods = sorted((x.baseaddress, x.size, x.name.split('\\')[-1]) for x in m.modules.modules)
    patched, other = [], []
    for nm, sec in out['sections'].items():
        for d in sec.pop('_ranges', []):
            a = base + d[0]; ins = next(md.disasm(read(a, 16) or b'', a), None)
            tgt = int(ins.op_str, 16) if ins and ins.mnemonic == 'jmp' and ins.op_str.startswith('0x') else None
            if tgt is not None and not base <= tgt < base + mod.size:
                where = next(('%s+0x%X' % (n, tgt - b) for b, sz, n in mods if b <= tgt < b + sz), 'memory of no module')
                patched.append({'at': '0x%08X' % (0x10000000 + d[0]), 'bytes': d[1] - d[0], 'jmp_to': where})
            else: other.append('0x%08X+%d' % (0x10000000 + d[0], d[1] - d[0]))
    out['patched_entries'] = patched; out['other_differences'] = other
    if ok: out['result'] = 'analysed build: every code byte matches'
    elif rel['unhandled_entries'] == 0 and not other and all(v['pages_missing'] == 0 for v in out['sections'].values()):
        out['result'] = 'analysed build: every code byte matches except %d function entries patched at run time (listed)' % len(patched)
    else: out['result'] = 'NOT shown to be the analysed build'
    return out


if __name__ == '__main__':
    for d in sys.argv[1:]: print(json.dumps(verify(d, os.environ['FFXI_DLL']), indent=1))
