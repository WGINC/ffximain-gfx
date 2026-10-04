"""global_sources.py -- what each store into a global writes, traced back through the instructions before it.

For every instruction that writes a global (a relocated operand inside it, as global_dossier.py finds them), the
stored value is followed backwards within its straight-line run:
  * a constant (`mov [g], imm`, or a register loaded with one);
  * another global's value or address;
  * the result of a call (EAX right after `call F`). When F is a constructor -- its own code stores a vtable
    address at [ECX] before returning ECX/the object -- the object's class is that vtable's class
    (data/class_vtables.json): the global then holds an object of that class (or a pointer to one), from the code
    alone. The allocator that precedes it (`call alloc; ...; mov ecx, eax; call F`) is recorded too.
Output {"0xGLOBAL": [{"at", "fn", "insn", "source": {...}}]}.
Usage: python3 global_sources.py DOSSIER.jsonl OUT.json [0xADDR ...]     (uses $FFXI_DLL)"""
import json, os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
FAM = {'eax': 'eax', 'ax': 'eax', 'al': 'eax', 'ecx': 'ecx', 'cx': 'ecx', 'cl': 'ecx', 'edx': 'edx', 'dx': 'edx', 'dl': 'edx',
       'ebx': 'ebx', 'bx': 'ebx', 'bl': 'ebx', 'esi': 'esi', 'si': 'esi', 'edi': 'edi', 'di': 'edi', 'ebp': 'ebp'}


def vtable_classes():
    m = {}
    for c, v in json.load(open(os.path.join(D, 'class_vtables.json'))).items():
        m[int(v['vtable'], 16)] = c
    return m


def ctor_class(im, ext, f, vt):
    """the class whose vtable F stores at [this] (this = ECX on entry, kept in a register), or None"""
    e = ext.get('0x%x' % f)
    if not e: return None
    ins = im.code(e); this = {'ecx'}; found = set()
    for a in sorted(ins)[:80]:
        i = ins[a]; ops = i.operands
        if i.mnemonic == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG \
                and FAM.get(i.reg_name(ops[1].reg)) in this:
            this.add(FAM.get(i.reg_name(ops[0].reg)))
        if i.mnemonic == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_MEM and ops[1].type == X86_OP_IMM \
                and ops[0].mem.base and FAM.get(i.reg_name(ops[0].mem.base)) in this and ops[0].mem.disp == 0 \
                and not ops[0].mem.index and ops[1].imm in vt:
            found.add(vt[ops[1].imm])
        if i.mnemonic.startswith('ret'): break
    # the last vtable stored is the most derived (base constructors run first and are overwritten)
    return sorted(found)[-1] if len(found) == 1 else (list(found) if found else None)


def main(doss, out, *only):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json'))); vt = vtable_classes()
    want = {int(a, 16) for a in only}
    res = {}
    for l in open(doss):
        g = json.loads(l); ga = int(g['ea'], 16)
        if want and ga not in want: continue
        rows = []
        for r in g['refs']:
            if 'at' not in r or not str(r.get('kind', '')).startswith('write'): continue
            f = int(r['fn'], 16); ins = im.code(ext['0x%x' % f]); order = sorted(ins)
            at = int(r['at'], 16); k = order.index(at); i = ins[at]
            src = i.operands[1] if len(i.operands) > 1 else None
            row = {'at': r['at'], 'fn': r['fn_name'] or r['fn'], 'insn': r['insn']}
            if src is None: row['source'] = {'kind': 'unknown'}
            elif src.type == X86_OP_IMM: row['source'] = {'kind': 'const', 'value': '0x%X' % (src.imm & 0xFFFFFFFF)}
            elif src.type == X86_OP_REG:
                reg = FAM.get(i.reg_name(src.reg)); s = {'kind': 'unknown', 'reg': reg}
                j = k - 1
                while j >= 0 and order[j] + ins[order[j]].size == order[j + 1]:
                    p = ins[order[j]]; po = p.operands
                    if p.mnemonic == 'call':
                        if reg == 'eax':
                            t = po[0].imm if po and po[0].type == X86_OP_IMM else None
                            s = {'kind': 'call', 'callee': '0x%08X' % t if t else p.op_str}
                            if t:
                                c = ctor_class(im, ext, t, vt)
                                if c: s['class'] = c
                        break
                    if po and po[0].type == X86_OP_REG and FAM.get(p.reg_name(po[0].reg)) == reg and p.mnemonic in ('mov', 'lea', 'xor', 'or', 'and'):
                        if p.mnemonic == 'xor' and len(po) == 2 and po[1].type == X86_OP_REG and FAM.get(p.reg_name(po[1].reg)) == reg:
                            s = {'kind': 'const', 'value': '0x0'}
                        elif p.mnemonic == 'mov' and po[1].type == X86_OP_IMM: s = {'kind': 'const', 'value': '0x%X' % (po[1].imm & 0xFFFFFFFF)}
                        elif p.mnemonic == 'mov' and po[1].type == X86_OP_REG: reg = FAM.get(p.reg_name(po[1].reg)); j -= 1; continue
                        else: s = {'kind': 'expr', 'insn': '%s %s' % (p.mnemonic, p.op_str)}
                        break
                    j -= 1
                row['source'] = s
            else: row['source'] = {'kind': 'memory'}
            rows.append(row)
        res[g['ea']] = rows
    json.dump(res, open(out, 'w'), indent=1)
    print('%d globals, %d stores; %d from a constructor of a known class' % (len(res), sum(len(v) for v in res.values()),
          sum(1 for v in res.values() for r in v if r['source'].get('class'))))


if __name__ == '__main__':
    main(*sys.argv[1:])
