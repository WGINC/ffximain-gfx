"""ida_sp_check.py -- where does IDA's stack pointer for a function go wrong?

IDA writes each stack operand as `[esp+DEPTH+var_X]`, DEPTH being its own stack-pointer value at that
instruction. Hex-Rays names every stack slot from those values, so where IDA's depth differs from the
real one (stack_depth.py) every name is off by the difference -- one variable's accesses split across
two names, which compiles to different memory (CXiSkeletonActor_OnMove after 0x100C668D).

The listing has no addresses per line; it is in address order, so its instructions are aligned with
the disassembly of the function's chunks one for one (mnemonics checked).

    python3 ida_sp_check.py LISTING.asm ADDR [VTABLE]        uses $FFXI_DLL
Prints each instruction where IDA's depth != the real depth (first of each run), and the correction
for IDA (Alt+K at that address: the change in SP the instruction really makes).
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import stack_depth

INSN = re.compile(r'^\s{8,}([a-z][a-z0-9]*)\b(.*?)(?:\s*;.*)?$')


def listing_insns(path, name):
    out, inside = [], False
    for line in open(path, encoding='latin-1'):
        if re.match(r'^%s\s+proc near' % re.escape(name), line): inside = True; continue
        if inside and re.match(r'^%s\s+endp' % re.escape(name), line): break
        if not inside: continue
        m = INSN.match(line)
        if m and m.group(1) not in ('db', 'dd', 'dw', 'align', 'assume', 'public', 'var', 'arg'):
            d = re.search(r'\[esp\+([0-9A-F]+)h\+', m.group(2))
            out.append((m.group(1), int(d.group(1), 16) if d else None, m.group(2).strip()))
    return out


def main(listing, ea, vt=None):
    ea = int(ea, 16)
    ins, depth, _ = stack_depth.depths(ea, int(vt, 16) if vt else None)
    import json
    name = None
    for l in open(os.path.join(stack_depth.D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l)
        if r['ea'] == ea: name = r['name']; break
    li = listing_insns(listing, name)
    addrs = sorted(ins)
    print('%s: %d instructions in the DLL, %d in the listing' % (name, len(addrs), len(li)))
    prev_bad = False; n = 0
    for a, (mn, idad, ops) in zip(addrs, li):
        cm = ins[a].mnemonic
        if cm.split()[0].rstrip('d') != mn.rstrip('d') and not (cm.startswith('j') and mn.startswith('j')) and cm != mn:
            print('alignment lost at %08X: %s vs listing %s %s' % (a, cm, mn, ops)); return
        if idad is None or a not in depth: continue
        bad = idad != depth[a]
        if bad and not prev_bad:
            print('%08X  IDA depth 0x%X, real 0x%X (%+d)  %s %s' % (a, idad, depth[a], idad - depth[a], cm, ins[a].op_str)); n += 1
        if not bad and prev_bad:
            print('%08X  agrees again' % a)
        prev_bad = bad
    print(n, 'runs where IDA differs')


if __name__ == '__main__':
    main(*sys.argv[1:])
