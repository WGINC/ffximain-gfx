"""global_types.py -- the class of object each game global points to, from every instruction that writes it.

A global is written only through its absolute address, so the writers are complete: every instruction with
the global as a memory operand that it writes (or whose address it takes: lea, push/mov of the address), plus
any dword in the image equal to the address (a table holding it). A global is typed `C *` when every writer
stores one of:
  * null: the constant 0, or a register just set to 0 in the same basic block;
  * an object of class C: the return value of a constructor of C (a function that stores C's vtable into
    its entry ECX and returns the entry ECX on every path), moved through registers in the same basic block;
  * an object of class C: a register whose [reg+0] got C's vtable in the same basic block, with no call after.
and at least one writer stores an object. Everything is read from the binary; a writer of any other kind
(a value from memory, an argument, the address taken, a block entered by an indirect jump) leaves the global
untyped, with the reason.

Two tiers, stated per global:
  proven    every constructor involved has C's vtable stored on every path to its returns with no call after
            the store (vcall_resolve.this_flow)
  accepted  some constructor calls functions after storing the vtable (member constructors, Init): that they do
            not rewrite it is not shown; the calls are listed
Classes: data/ida_classes.json, data/class_vtables.json (vtable -> name); a vtable of no named class is named
Vt_<address>.

    python3 global_types.py data/global_types.json        uses $FFXI_DLL
"""
import collections, json, os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import vcall_resolve as V
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
r32 = V.r32


def constructors(im, code, vts):
    """{function: (vtable, [calls after the last vtable store])} for functions that return their entry ECX on
    every path with one vtable stored into it last on every path"""
    out = {}
    for f, ins in code.items():
        if f not in ins: continue
        st = V.this_flow(im, ins, f, vts, V.written_exact)
        if not st: continue
        rets = [a for a, i in ins.items() if i.mnemonic in ('ret', 'retn') and a in st]
        if not rets: continue
        # the vtable last stored into this on each path, ignoring calls (they are listed instead)
        last = {}; work = [f]; seen = {f: None}
        order = sorted(ins)
        ok = True; vt_at = {f: None}; calls_after = set()
        labels = S.labels_of(ins)
        state = {f: (None, frozenset())}
        while work:
            a = work.pop(); vt, cl = state[a]; i = ins[a]; ops = i.operands
            Sx = st.get(a, (frozenset(),))[0]
            if i.mnemonic == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_MEM and ops[0].mem.base and not ops[0].mem.index \
                    and ops[0].mem.disp == 0 and r32(i.reg_name(ops[0].mem.base)) in Sx:
                if ops[1].type == X86_OP_IMM and ops[1].imm in vts: vt, cl = ops[1].imm, frozenset()
                else: vt = 'other'
            elif i.mnemonic == 'call' and vt not in (None, 'other'): cl = cl | {a}
            nxt = V.succs(im, ins, a, i, labels)
            if nxt is None: ok = False; break
            for n in nxt:
                if n not in ins: continue
                new = (vt, cl)
                if n not in state: state[n] = new; work.append(n)
                elif state[n] != new:
                    o = state[n]
                    m = (o[0] if o[0] == vt else 'other', o[1] | cl)
                    if m != o: state[n] = m; work.append(n)
        if not ok: continue
        vt_rets = set(); cls = set()
        for a in rets:
            if 'eax' not in st[a][0]: break
            vt, cl = state.get(a, (None, frozenset()))
            vt_rets.add(vt); cls |= cl
        else:
            if len(vt_rets) == 1 and next(iter(vt_rets)) not in (None, 'other'):
                out[f] = (next(iter(vt_rets)), sorted(cls))
    return out


def main(out):
    im = S.Image(os.environ['FFXI_DLL']); base = im.base; img = bytes(im.img)
    ext = json.load(open(os.path.join(D, 'func_extents.json'))); starts = {int(k, 16) for k in ext}
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    vt_slots = V.vtables(code, img, base, starts); vts = set(vt_slots)
    names = {int(c['vtable'], 16): n for n, c in json.load(open(os.path.join(D, 'ida_classes.json')))['classes'].items()}
    for n, c in json.load(open(os.path.join(D, 'class_vtables.json'))).items(): names.setdefault(int(c['vtable'], 16), n)
    cname = lambda v: names.get(v, 'Vt_%08X' % v)
    ctors = constructors(im, code, vts)
    # the globals the graphics code uses
    gl = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l)
        for n, o in r.get('objs', {}).items():
            if not o['func']: gl.setdefault(o['ea'], n)
    # the global's address held in data: every absolute address stored in this relocatable DLL has a relocation
    # entry (the table dump_verify.py relocates the code with, byte-exact against the running game), so the
    # relocation sites outside the code are exactly the stored addresses; inside the code they are operands
    import bisect, pefile
    pe = pefile.PE(os.environ['FFXI_DLL']); pe.parse_data_directories()
    merged = []
    for a_, b_ in sorted((a_, b_) for e in ext.values() for a_, b_ in e):
        if merged and a_ <= merged[-1][1]: merged[-1][1] = max(merged[-1][1], b_)
        else: merged.append([a_, b_])
    mst = [x for x, _ in merged]
    in_code = lambda x: (lambda j: j >= 0 and merged[j][0] <= x < merged[j][1])(bisect.bisect_right(mst, x) - 1)
    data_refs = collections.Counter(); reloc_n = 0
    for blk in getattr(pe, 'DIRECTORY_ENTRY_BASERELOC', []):
        for e in blk.entries:
            if e.type != 3: continue                                    # IMAGE_REL_BASED_HIGHLOW
            reloc_n += 1; a_ = base + e.rva
            v = struct.unpack_from('<I', img, e.rva)[0]
            if v in gl and not in_code(a_): data_refs[v] += 1
    # every instruction touching each global, with its function
    touch = collections.defaultdict(list)
    for f, ins in code.items():
        for a, i in ins.items():
            for k, o in enumerate(i.operands):
                if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index and o.mem.disp in gl: touch[o.mem.disp].append((f, a, k))
                elif o.type == X86_OP_IMM and o.imm in gl and not (i.mnemonic in ('call', 'jmp') or i.mnemonic.startswith('j')):
                    touch[o.imm].append((f, a, -1))

    def value_at(f, a, reg):
        """what reg holds just before instruction a, from the same basic block: ('null',), ('obj', vt, tier, why)
        or ('?', why)"""
        ins = code[f]; order = sorted(ins); k = order.index(a); labels = S.labels_of(ins)
        while k > 0:
            if order[k] in labels and order[k] != a: return ('?', 'value comes from a join at 0x%08X' % order[k])
            k -= 1; i = ins[order[k]]; ops = i.operands; m = i.mnemonic
            w = V.written_exact(i)
            if reg not in w and not (m == 'call' and reg in ('eax', 'ecx', 'edx')): continue
            if m == 'call':
                if reg != 'eax': return ('?', 'clobbered by the call at 0x%08X' % i.address)
                if ops[0].type == X86_OP_IMM and ops[0].imm in ctors:
                    vt, cl = ctors[ops[0].imm]
                    return ('obj', vt, 'accepted' if cl else 'proven',
                            'the result of constructor 0x%08X (stores vtable 0x%08X%s)' % (ops[0].imm, vt,
                            '; calls %s after the store' % ', '.join('0x%08X' % c for c in cl) if cl else ''))
                # an allocation followed in this block by an inline vtable store is handled by the caller scan
                return ('?', 'the result of 0x%s, not a constructor' % (('%08X' % ops[0].imm) if ops[0].type == X86_OP_IMM else 'indirect call'))
            if m == 'xor' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG and ops[0].reg == ops[1].reg:
                return ('null',)
            if m == 'mov' and len(ops) == 2 and ops[1].type == X86_OP_IMM:
                return ('null',) if ops[1].imm == 0 else ('?', 'a constant 0x%X' % ops[1].imm)
            if m == 'mov' and len(ops) == 2 and ops[1].type == X86_OP_REG:
                reg = r32(i.reg_name(ops[1].reg)); continue
            return ('?', '%s %s at 0x%08X' % (m, i.op_str, i.address))
        return ('?', 'function entry reached')

    def inline_vtable(f, a, reg):
        """an inline constructor: [reg+0] = vtable earlier in the same block, no call between it and a"""
        ins = code[f]; order = sorted(ins); k = order.index(a); labels = S.labels_of(ins)
        while k > 0:
            if order[k] in labels and order[k] != a: return None
            k -= 1; i = ins[order[k]]; ops = i.operands
            if i.mnemonic == 'call': return None
            if i.mnemonic == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_MEM and ops[0].mem.base and not ops[0].mem.index \
                    and ops[0].mem.disp == 0 and r32(i.reg_name(ops[0].mem.base)) == reg and ops[1].type == X86_OP_IMM and ops[1].imm in vts:
                return ops[1].imm
            if reg in V.written_exact(i): return None
        return None

    res, stats = {}, collections.Counter()
    for g, gname in sorted(gl.items()):
        why, objs, bad = [], [], None
        if data_refs[g]: bad = 'its address is stored in the image (%d times): a writer through it cannot be ruled out' % data_refs[g]
        writers = 0
        for f, a, k in touch.get(g, []):
            if bad: break
            i = code[f][a]; m = i.mnemonic; ops = i.operands
            if k == -1 or m == 'lea': bad = 'its address is taken at 0x%08X (%s %s)' % (a, m, i.op_str); break
            if k != 0 or m in ('cmp', 'test', 'push'): continue            # a read
            writers += 1
            if m == 'mov' and ops[1].type == X86_OP_IMM:
                if ops[1].imm == 0: why.append('0x%08X stores 0' % a); continue
                bad = '0x%08X stores the constant 0x%X' % (a, ops[1].imm); break
            if m == 'mov' and ops[1].type == X86_OP_REG:
                reg = r32(i.reg_name(ops[1].reg)); v = value_at(f, a, reg)
                if v[0] == 'null': why.append('0x%08X stores 0' % a); continue
                if v[0] == 'obj': objs.append((v[1], v[2])); why.append('0x%08X stores %s' % (a, v[3])); continue
                iv = inline_vtable(f, a, reg)
                if iv is not None:
                    objs.append((iv, 'proven')); why.append('0x%08X stores an object whose vtable 0x%08X was stored just before (no call between)' % (a, iv)); continue
                bad = '0x%08X stores %s: %s' % (a, reg, v[1]); break
            bad = '0x%08X writes it (%s %s)' % (a, m, i.op_str); break
        if not bad and not writers: bad = 'never written by code (static data or set outside the code)'
        if not bad and not objs: bad = 'only null is ever stored'
        if not bad and len({v for v, _ in objs}) > 1:
            bad = 'objects of several classes are stored: %s' % ', '.join(sorted({cname(v) for v, _ in objs}))
        if bad:
            import re as _re
            stats['untyped: ' + _re.sub(r'0x[0-9A-F]+', 'X', bad).split(':')[0].split(' (')[0][:60]] += 1
            res['0x%08X' % g] = {'name': gname, 'type': None, 'why': bad}
            continue
        vt = objs[0][0]; tier = 'accepted' if any(t == 'accepted' for _, t in objs) else 'proven'
        stats['typed (%s)' % tier] += 1
        res['0x%08X' % g] = {'name': gname, 'type': cname(vt) + ' *', 'vtable': '0x%08X' % vt, 'tier': tier, 'why': '; '.join(why)}
    res['_stats'] = dict(stats, globals=len(gl), constructors=len(ctors), relocations=reloc_n)
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('%d globals; %d constructors; %s' % (len(gl), len(ctors), dict(stats)))


if __name__ == '__main__':
    main(sys.argv[1])
