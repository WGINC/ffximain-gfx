"""livecall.py -- the functions whose game code keeps ECX or EDX across a direct call (recompile/d3d_round_121.md rule 28).

After each `call rel32` in a function's bytes (data/func_extents.json, its first piece), the instructions up to the next
jump, return or call are read: ECX or EDX read there before being written (`xor r, r`, `or r, -1`, `and r, 0` are
writes) is a value the caller kept in a caller-saved register across the call; so is a following call to a function
that reads ECX (data/ecx_use.json) with ECX not written in between. VC6 does this only for a callee it compiled earlier
in the same file, so the function cannot match compiled alone. Reported with the sweep's verdict
(verification/vc6_sweep.json) and frame-set membership (data/frame_set.json).

    FFXI_DLL=... python3 pipeline/vc6/livecall.py verification/live_across_call.json"""
import json, os, sys, struct, capstone, pefile, collections
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..') + os.sep
img = pefile.PE(os.environ['FFXI_DLL'], fast_load=True).get_memory_mapped_image()
ext = {int(k, 16): v for k, v in json.load(open(R + 'data/func_extents.json')).items() if k.startswith('0x')}
sw = {int(k, 16): v for k, v in json.load(open(R + 'verification/vc6_sweep.json')).items() if k.startswith('0x')}
fs = json.load(open(R + 'data/frame_set.json')); fs = {int(k, 16) for k in (fs.get('functions') or fs) if str(k).startswith('0x')}
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
REG = {'ecx': {'ecx', 'cx', 'cl', 'ch'}, 'edx': {'edx', 'dx', 'dl', 'dh'}}
hits = {}
ECXU = {int(k, 16) for k, v in json.load(open(R + 'data/ecx_use.json')).items() if k.startswith('0x') and v}
for ea, pieces in ext.items():
    s, e = pieces[0]
    ins = list(md.disasm(bytes(img[s - 0x10000000:e - 0x10000000]), s))
    for k, i in enumerate(ins):
        if i.mnemonic != 'call' or not i.op_str.startswith('0x'): continue
        tgt = int(i.op_str, 16)
        live = set(REG)
        for j in ins[k + 1:k + 12]:
            if j.mnemonic == 'call' and j.op_str.startswith('0x') and 'ecx' in live and int(j.op_str, 16) in ECXU:
                hits.setdefault(ea, []).append((hex(i.address), hex(tgt), 'ecx', 'call ' + j.op_str)); break
            if j.mnemonic.startswith(('j', 'ret', 'call')) or not live: break
            rd, wr = j.regs_access()
            rd = {j.reg_name(r) for r in rd}; wr = {j.reg_name(r) for r in wr}
            ops = j.op_str.split(', ')
            if j.mnemonic in ('xor', 'sub') and len(ops) == 2 and ops[0] == ops[1]: rd = set()
            if j.mnemonic in ('or', 'and') and len(ops) == 2 and ops[1] in ('0xffffffff', '0'): rd = set()
            found = [n for n in live if rd & REG[n]]
            if found: hits.setdefault(ea, []).append((hex(i.address), hex(tgt), found[0], j.mnemonic + ' ' + j.op_str)); break
            for n in list(live):
                if wr & REG[n]: live.discard(n)
c = collections.Counter()
for ea, h in hits.items():
    c[(sw.get(ea, {}).get('verdict', 'none'), ea in fs)] += 1
print('functions keeping ECX/EDX across a direct call:', len(hits)); print(c)
json.dump({'0x%08X' % k: {'sites': v, 'sweep': sw.get(k, {}).get('verdict'), 'frame_set': k in fs} for k, v in sorted(hits.items())},
          open(sys.argv[1], 'w'), indent=0)
