"""global_dossier.py -- the evidence for naming the export's unnamed globals (dword_/unk_/byte_/flt_/off_/...).

For each global the export references under a placeholder name, from the binary and the export only:
  * where it is: section, IDA type and size, the initial bytes in the file (for .data/.rdata; as an integer,
    as a float, and what an address there points to: a string or a named object);
  * every reference in the whole image: each relocated dword whose value falls inside the global
    (dump_verify.reloc_blocks), resolved to the instruction that holds it (func_extents.json, decoded with
    sp_solve.Image) or to the data address that holds it (a pointer table);
  * per instruction: the function it is in (with its name: export, names_pending_idb.json, dm_names.json),
    the mnemonic and operands, and what it does with the global (read / write / float read / float write /
    address taken / call through / compare) and at which width;
  * the export's pseudocode lines that mention it.
Usage: python3 global_dossier.py OUT.jsonl OUT.txt    (uses $FFXI_DLL)"""
import bisect, collections, json, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import pefile
import sp_solve as S
from dump_verify import reloc_blocks

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
PH = re.compile(r'^(dword|unk|byte|word|flt|dbl|qword|off|stru|xmmword|tbyte|asc|stru)_([0-9A-F]{8})$')

def kind_of(i, lo, hi):
    """what instruction i does with the global [lo, hi)"""
    m, ops = i.mnemonic, i.op_str
    mem = re.search(r'(byte|word|dword|qword|tbyte|xmmword) ptr \[([^\]]*)\]', ops)
    width = mem.group(1) if mem else None
    if m.startswith('f'):
        if m.startswith(('fst', 'fist')): return 'float write' if m.startswith('fst') else 'int write (x87)', width
        if m.startswith(('fld', 'fadd', 'fsub', 'fmul', 'fdiv', 'fcom', 'fucom')): return 'float read', width
        if m.startswith(('fild', 'fiadd', 'fisub', 'fimul', 'fidiv', 'ficom')): return 'int read (x87)', width
        return 'x87 ' + m, width
    if not mem:
        return ('address taken (%s)' % m), None
    first = ops.split(',')[0]
    if m == 'call': return 'call through', width
    if m == 'jmp': return 'jump through', width
    if m in ('cmp', 'test'): return 'compare', width
    if m == 'lea': return 'address taken (lea)', None
    if '[' in first:
        if m in ('mov', 'movsx', 'movzx'): return 'write', width
        if m in ('inc', 'dec', 'add', 'sub', 'or', 'and', 'xor', 'neg', 'not', 'shl', 'shr', 'sar'): return 'read-modify-write (%s)' % m, width
        if m.startswith('set'): return 'write (%s)' % m, width
        return 'write? (%s)' % m, width
    if m == 'push': return 'read (push)', width
    return 'read (%s)' % m, width

def main(out_jsonl, out_txt):
    dll = os.environ['FFXI_DLL']
    pe = pefile.PE(dll, fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    img = pe.get_memory_mapped_image()
    secs = [(base + s.VirtualAddress, base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData), s.Name.rstrip(b'\0').decode(), s)
            for s in pe.sections]
    def sec_of(a):
        for lo, hi, n, s in secs:
            if lo <= a < hi: return n, s
        return None, None
    def raw(a, n):
        nm, s = sec_of(a)
        if not s: return None
        off = a - base - s.VirtualAddress
        return img[a - base:a - base + n] if off + n <= s.SizeOfRawData else None   # bss: no file bytes
    # names for functions
    names = {}
    for k, v in json.load(open(os.path.join(D, 'dm_names.json'))).items(): names[int(k, 16)] = v[0]
    R = [json.loads(l) for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl'))]
    for r in R: names[r['ea']] = r['name']
    pend = json.load(open(os.path.join(D, 'names_pending_idb.json')))
    for k, v in pend.items(): names[int(k, 16)] = v['name']
    gnames = {}                                     # named globals, for pointer targets
    for r in R:
        for nm, o in (r.get('objs') or {}).items():
            if not PH.match(nm): gnames[o['ea']] = nm
    # the placeholder globals
    glob = {}
    for r in R:
        for nm, o in (r.get('objs') or {}).items():
            if o.get('func') or not PH.match(nm): continue
            if ('0x%08X' % o['ea']) in pend: continue          # already named, not yet in the IDB
            sz = o.get('size') or 0
            if not 0 < sz < 0x100000: sz = 0           # IDA's BADSIZE (2**64-1) for _UNKNOWN: the exact address only
            g = glob.setdefault(o['ea'], {'name': nm, 'type': o.get('type', ''), 'size': sz,
                                          'is_str': o.get('is_str'), 'lines': [], 'export_funcs': set()})
            g['export_funcs'].add(r['name'])
            for line in r['text'].split('\n'):
                if re.search(r'\b%s\b' % re.escape(nm), line) and len(g['lines']) < 8:
                    t = line.strip()
                    if t not in g['lines'] and not t.startswith('//'): g['lines'].append(t[:160])
    starts = sorted(glob); ends = [a + max(glob[a]['size'], 1) for a in starts]
    def owner_glob(v):
        k = bisect.bisect_right(starts, v) - 1
        return starts[k] if k >= 0 and v < ends[k] else None
    # every relocated dword
    blocks, _ = reloc_blocks(pe, img)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    fstarts = sorted((r[0], r[1], int(k, 16)) for k, rs in ext.items() for r in rs)
    fkeys = [x[0] for x in fstarts]
    def owner_fn(a):
        k = bisect.bisect_right(fkeys, a) - 1
        return fstarts[k][2] if k >= 0 and a < fstarts[k][1] else None
    im = S.Image(dll); code_cache = {}
    refs = collections.defaultdict(list)
    for page, entries in blocks:
        for e in entries:
            if e >> 12 != 3: continue
            slot = base + page + (e & 0xFFF)
            v = struct.unpack_from('<I', img, slot - base)[0]
            g = owner_glob(v)
            if g is None: continue
            f = owner_fn(slot)
            if f is None:
                nm, _ = sec_of(slot)
                refs[g].append({'from_data': '0x%08X' % slot, 'section': nm, 'offset': v - g,
                                'holder': gnames.get(slot) or names.get(slot)})
                continue
            if f not in code_cache: code_cache[f] = im.code([tuple(x) for x in ext['0x%x' % f]])
            ins = next((i for a, i in code_cache[f].items() if a <= slot < a + i.size), None)
            if ins is None:
                refs[g].append({'fn': '0x%08X' % f, 'fn_name': names.get(f), 'slot': '0x%08X' % slot, 'insn': None}); continue
            k, w = kind_of(ins, g, g + max(glob[g]['size'], 1))
            refs[g].append({'fn': '0x%08X' % f, 'fn_name': names.get(f), 'at': '0x%08X' % ins.address,
                            'insn': '%s %s' % (ins.mnemonic, ins.op_str), 'kind': k, 'width': w, 'offset': v - g})
    out = []
    for ea in starts:
        g = glob[ea]; sec, _ = sec_of(ea); b = raw(ea, 4)
        init = None
        if b and len(b) == 4:
            u = struct.unpack('<I', b)[0]; f = struct.unpack('<f', b)[0]
            init = {'u32': '0x%08X' % u, 'f32': f if abs(f) < 1e9 and (f == 0 or abs(f) > 1e-9) else None}
            if base <= u < base + pe.OPTIONAL_HEADER.SizeOfImage:
                init['points_to'] = gnames.get(u) or names.get(u) or '0x%08X' % u
                s = raw(u, 64)
                if s and s.split(b'\0')[0] and all(32 <= c < 127 for c in s.split(b'\0')[0]): init['string'] = s.split(b'\0')[0].decode()
        if g['is_str'] or (sec == '.rdata' and raw(ea, 64)):
            s = raw(ea, 96) or b''
            z = s.split(b'\0')[0]
            if z and all(32 <= c < 127 or c in (9, 10, 13) for c in z): init = dict(init or {}, own_string=z.decode())
        kinds = collections.Counter(r.get('kind') for r in refs[ea] if 'kind' in r)
        fns = collections.Counter(r.get('fn_name') or r.get('fn') for r in refs[ea] if 'fn' in r)
        out.append({'ea': '0x%08X' % ea, 'name': g['name'], 'type': g['type'], 'size': g['size'], 'section': sec,
                    'init': init, 'kinds': dict(kinds), 'functions': dict(fns.most_common()),
                    'refs': refs[ea], 'export_funcs': sorted(g['export_funcs']), 'lines': g['lines']})
    with open(out_jsonl, 'w') as f:
        for o in out: f.write(json.dumps(o) + '\n')
    with open(out_txt, 'w') as f:
        for o in out:
            f.write('== %s %s  type=%s size=%s %s init=%s\n' % (o['ea'], o['name'], o['type'], o['size'], o['section'], json.dumps(o['init'])))
            f.write('   uses: %s\n' % ', '.join('%s x%d' % (k, n) for k, n in sorted(o['kinds'].items(), key=lambda x: -x[1])))
            f.write('   in: %s\n' % ', '.join('%s(%d)' % (k, n) for k, n in list(o['functions'].items())[:10]))
            dat = [r for r in o['refs'] if 'from_data' in r]
            if dat: f.write('   from data: %s\n' % ', '.join('%s%s' % (r['from_data'], '(' + r['holder'] + ')' if r.get('holder') else '') for r in dat[:6]))
            for l in o['lines'][:6]: f.write('   | %s\n' % l)
    print('%d unnamed globals; %d references; %d with none found' % (len(out), sum(len(o['refs']) for o in out),
                                                                     sum(1 for o in out if not o['refs'])))

if __name__ == '__main__':
    main(*sys.argv[1:3])
