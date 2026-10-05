"""cdecl_args.py -- how many stack arguments each caller-cleanup (cdecl) function reads, from its code.

A gate stub logs a callee's arguments and compares them. For a cdecl callee, logging more slots
than the callee reads compares stale stack contents: MSVC often leaves an earlier call's arguments
on the stack (deferred cleanup), and the reconstruction's stale slots differ. This finds the highest
argument slot each function reads ([esp+N] beyond its own pushes, or [ebp+N] above the frame), with the
stack depth followed exactly over the function's control flow.

    python3 cdecl_args.py FFXiMain_unpacked.dll data/cdecl_args.json
Output: {"0xADDR": number of argument slots read, "_at_least": {"0xADDR": slots read on every path that
could be followed}}, for functions that pop nothing (`ret` / `ret 0`)
and whose stack depth could be followed. A path is not followed past an `esp` change it cannot follow, a
tail call, an indirect jump, or a call whose pop no definitive source gives; the function's count is then a
lower bound, listed under "_at_least". Paths meeting at different depths drop the function. A call removes what its callee
pops: a direct callee's `ret N` (data/purges.json); an indirect call's pop from data/sp_solved.json,
data/vcalls.json or data/com_calls.json. Before 5 Oct calls were taken to pop nothing, so a stdcall call
before an argument read hid it (0x1021D4C0 reads 5 argument slots after two COM calls; it was listed as 0).
"""
import json, os, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_MEM, X86_OP_IMM, X86_OP_REG

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


# MSVC's stack probe (_chkstk): `mov eax, N; call 0x10312150` allocates N bytes. From its code: entry ESP = E;
# push ecx; ecx = E+4 (lea [esp+8]) less N in 0x1000 steps; mov eax, esp; mov esp, ecx; eax = [E] (the return
# address); push eax; ret -- so ESP after the call is the caller's ESP before it less N. purges.json says it pops
# nothing (a plain `ret`), which is true of its `ret` but not of its effect on ESP.
STACK_PROBES = {0x10312150}


def nargs(md, img, base, ranges, pops=None, site_pops=None, entry=None):
    """(the highest argument slot the function reads, complete) following its control flow from the entry with
    the stack depth exact at every instruction. A path is not followed past a call whose pop is not known, a
    tail call, a jump table or an ESP change that cannot be followed: the count is then a lower bound (complete
    False), the slots read on every path that could be followed. Paths that meet at different depths: None."""
    ins = {}
    for a, b in ranges:
        for i in md.disasm(img[a - base:b - base], a): ins[i.address] = i
    if not ins: return None
    if entry is None: entry = min(ins)
    if entry not in ins: return None
    depth = {entry: (0, None)}                            # address -> (ESP depth below entry, EBP depth or None)
    work, top, complete = [entry], 0, True
    prev = {i.address + i.size: i for i in ins.values()}  # the instruction falling through to each address
    while work:
        a = work.pop(); i = ins[a]; m = i.mnemonic; ops = i.operands
        d, eb = depth[a]
        for op in ops:
            if op.type != X86_OP_MEM: continue
            b = i.reg_name(op.mem.base) if op.mem.base else None
            if b == 'esp' and op.mem.index == 0:
                slot = (op.mem.disp - d) // 4             # 0 = return address, 1 = first argument
                if slot >= 1: top = max(top, slot)
            elif b == 'ebp' and eb is not None and op.mem.index == 0 and op.mem.disp > 0:
                slot = (op.mem.disp - eb) // 4               # EBP = ESP at depth eb
                if slot >= 1: top = max(top, slot)
        if m == 'jmp' and (not ops or ops[0].type != X86_OP_IMM or ops[0].imm not in ins):
            complete = False; continue                    # tail call or jump table: arguments may be forwarded
        if m.startswith('ret'): continue
        if m == 'call' and ops and ops[0].type == X86_OP_IMM and ops[0].imm in STACK_PROBES:
            p = prev.get(a)
            if p is None or p.mnemonic != 'mov' or len(p.operands) != 2 or p.operands[0].type != X86_OP_REG \
                    or p.reg_name(p.operands[0].reg) != 'eax' or p.operands[1].type != X86_OP_IMM:
                complete = False; continue                # the size it allocates is not known here
            d += p.operands[1].imm
        elif m == 'call':
            if ops and ops[0].type == X86_OP_IMM:
                n = pops.get('0x%08X' % ops[0].imm) if pops is not None else 0
            else:
                n = site_pops.get('0x%08X' % i.address) if site_pops is not None else 0
            if n is None: complete = False; continue      # its pop is not known: the depth after it is not
            d -= n
        elif m == 'push': d += 4
        elif m == 'pop':
            d -= 4
            if ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'ebp': eb = None
        elif m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG \
                and i.reg_name(ops[0].reg) == 'ebp' and i.reg_name(ops[1].reg) == 'esp':
            eb = d
        elif m in ('sub', 'add') and ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'esp':
            if len(ops) < 2 or ops[1].type != X86_OP_IMM: complete = False; continue
            d += ops[1].imm if m == 'sub' else -ops[1].imm
        elif m == 'leave':
            if eb is None: complete = False; continue
            d, eb = eb - 4, None
        elif ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'esp' and m not in ('cmp', 'test'):
            if m != 'mov' or not (len(ops) == 2 and ops[1].type == X86_OP_REG and i.reg_name(ops[1].reg) == 'ebp') or eb is None:
                complete = False; continue
            d = eb
        elif ops and ops[0].type == X86_OP_REG and i.reg_name(ops[0].reg) == 'ebp' and m not in ('cmp', 'test', 'push'):
            eb = None                                     # EBP no longer the frame pointer
        if d < -8: complete = False; continue
        nxt = []
        if (m.startswith('j') or m.startswith('loop')) and ops and ops[0].type == X86_OP_IMM:
            nxt.append(ops[0].imm)
        if m != 'jmp' and a + i.size in ins: nxt.append(a + i.size)
        for t in nxt:
            if t not in ins: continue
            if t in depth:
                if depth[t] != (d, eb): return None       # paths meet at different depths
                continue
            depth[t] = (d, eb); work.append(t)
    return top, complete


def main(dll, out):
    pe = pefile.PE(dll, fast_load=True)
    base = pe.OPTIONAL_HEADER.ImageBase; img = pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = True
    purges = json.load(open(os.path.join(D, 'purges.json')))
    site = {}
    for f in ('sp_solved.json', 'vcalls.json', 'com_calls.json'):
        for k, v in json.load(open(os.path.join(D, f))).items():
            if not k.startswith('_') and isinstance(v, dict) and 'pops' in v: site.setdefault('0x%08X' % int(k, 16), v['pops'])
    res, low = {}, {}
    for k, ranges in json.load(open(os.path.join(D, 'func_extents.json'))).items():
        key = '0x%08X' % int(k, 16)
        if purges.get(key) != 0: continue
        n = nargs(md, img, base, ranges, purges, site, int(k, 16))
        if n is None: continue
        if n[1]: res[key] = n[0]
        else: low[key] = n[0]
    res['_at_least'] = low
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print(len(res) - 1, 'cdecl functions exact,', len(low), 'a lower bound')


if __name__ == '__main__':
    main(*sys.argv[1:3])
