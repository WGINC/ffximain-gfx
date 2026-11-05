"""frame_step_loops.py -- the calls of the frame-step getter whose value the caller uses as a loop count, read from the
game's instructions ($FFXI_DLL, an unpacked analysed build; data/frame_step_users.json for the sites).

    FFXI_DLL=... python3 frame_step_loops.py [--header ../kit/speedfix/sf_step_loops.h]

A site is a loop count when its code is, exactly (instruction by instruction, nothing inferred):
    call getter ; [fst m] ; fcom/fcomp [0.0] ; fnstsw ax ; and eax, 0x4100 ; jne EXIT        (the loop runs while s > 0)
    BODY: ...  fsub [1.0] ... fcomp [0.0] ; fnstsw ax ; and eax, 0x4100 ; je BODY            (s -= 1, again while s > 0)
so the body runs ceil(s) times for s > 0 and not at all for s <= 0 (the x87 compare: C0 and C3 both clear = greater).
The game's getter never answers below 1 (max(step, 1)); speedfix's frame_step 1 answers 0 on some frames above 60 fps,
and then the body does not run that frame.

For each loop the script records where the body's per-pass factor comes from, as the instructions show it:
  * `fmul/fld dword ptr [K]` with K a constant of the image: the factor is that constant (kind "operand");
  * `push k ; push v ; call 0x100272B0` (Vec3 scale, cdecl (float *v, float k); its body: v[i] = k * v[i]): the factor
    is that call's argument (kind "scale_call");
and whether the body is a chase toward a value fixed before the loop: x += (target - x) * k each pass. That last
reading is stated per loop in WHY below (read from its listing), not derived by the script; a loop without it is listed
with kind "other" and is not changed by speedfix."""
import argparse, json, os, struct, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
H = os.path.dirname(os.path.abspath(__file__)); D = os.path.join(H, '..', 'data')
GETTERS = (0x10014CF0, 0x10014D20)
SCALE = 0x100272B0

# What each loop's body does per pass, read from its listing (disasm.py; docs/RENDERER.md 3c). target = a value the
# code computes before the loop and does not change in it. Only loops listed here as a chase are changed by speedfix.
WHY = {
    0x100205B1: ('chase', 'YmCameraManager_Follow: st(1) A, st(0) B kept on the x87 stack; each pass A += 0.25 B, '
                 'B -= 0.25 B (fld [0x10329CE4] = 0.25): B *= 0.75 a pass, A + B fixed'),
    0x100C65BA: ('chase', 'CXiSkeletonActor_OnMove: p = the model position (vtable +0x1BC), t = [esi+0x5C4]; each pass '
                 'p += (t - p) * [0x1032A3BC] (0.125, three fmul)'),
    0x100C6882: ('chase', 'CXiSkeletonActor_OnMove: the angles [esi+0x61C..0x624] toward a target on the stack, the '
                 'difference wrapped to (-pi, pi] each pass, scaled by k (ebp, set before the loop) through 0x100272B0, '
                 'added'),
    0x100C6C64: ('chase', 'CXiSkeletonActor_OnMove: as 0x100C6882 (the other path)'),
    0x100C4878: ('chase', '0x100C47F0: the angles [esi+0x744..0x74C] toward a target on the stack, wrapped each pass, '
                 'scaled by 0.125 (push 0x3E000000) through 0x100272B0, added'),
    0x1002AD55: ('other', '0x1002AC60: 0x100276A0 with 1/32 (push 0x3D000000) and then 0x100274B0 each pass: a step '
                 'followed by a normalisation, not a chase with a closed form; left as the game has it'),
    0x100AAB78: ('other', '0x100AAB70: a velocity [esi+0x130] added each pass after a test (0x100AAB30) on the '
                 'position ahead; left as the game has it'),
    0x100AADF5: ('other', '0x100AADF0: a velocity [edi+0x120] added each pass; left as the game has it'),
}


def main():
    ap = argparse.ArgumentParser(); ap.add_argument('--header'); a = ap.parse_args()
    pe = pefile.PE(os.environ['FFXI_DLL'], fast_load=True); base = pe.OPTIONAL_HEADER.ImageBase
    im = pe.get_memory_mapped_image(); md = Cs(CS_ARCH_X86, CS_MODE_32)
    f32 = lambda ea: struct.unpack('<f', im[ea - base:ea - base + 4])[0]
    def mem(op):
        if 'dword ptr [0x' not in op: return None
        try: return int(op.split('[0x')[1].split(']')[0], 16)
        except ValueError: return None
    users = json.load(open(os.path.join(D, 'frame_step_users.json')))['sites']
    out = {}
    for site in sorted(int(s, 16) for s in users):
        ins = list(md.disasm(im[site - base:site - base + 0x400], site))
        if not ins or ins[0].mnemonic != 'call' or int(ins[0].op_str, 16) not in GETTERS: continue
        k = 1
        if ins[k].mnemonic == 'fst': k += 1
        if not (ins[k].mnemonic in ('fcom', 'fcomp') and mem(ins[k].op_str) and f32(mem(ins[k].op_str)) == 0.0): continue
        if not (ins[k + 1].mnemonic == 'fnstsw' and ins[k + 2].mnemonic == 'and' and ins[k + 2].op_str == 'eax, 0x4100'
                and ins[k + 3].mnemonic == 'jne'): continue
        exit_ = int(ins[k + 3].op_str, 16); body = ins[k + 4].address
        tail = None
        for j in range(k + 4, len(ins) - 4):
            i = ins[j]
            if i.address >= exit_: break
            if (i.mnemonic == 'fsub' and mem(i.op_str) and f32(mem(i.op_str)) == 1.0):
                for m in range(j + 1, min(j + 8, len(ins) - 3)):
                    if (ins[m].mnemonic == 'fcomp' and mem(ins[m].op_str) and f32(mem(ins[m].op_str)) == 0.0
                            and ins[m + 1].mnemonic == 'fnstsw' and ins[m + 2].op_str == 'eax, 0x4100'
                            and ins[m + 3].mnemonic == 'je' and site < int(ins[m + 3].op_str, 16) < i.address):
                        tail = (i.address, ins[m + 3].address, int(ins[m + 3].op_str, 16)); break
            if tail: break
        if not tail: continue
        loop_start = tail[2]
        factors = []
        prev = [None, None]
        for i in ins:
            if i.address < loop_start: continue
            if i.address > tail[1]: break
            ea = mem(i.op_str)
            if i.mnemonic in ('fmul', 'fld') and ea and base <= ea < base + len(im) and 0 < f32(ea) < 1:
                factors.append({'kind': 'operand', 'at': '0x%08X' % i.address, 'bytes': i.bytes.hex(),
                                'constant': '0x%08X' % ea, 'value': f32(ea)})
            if i.mnemonic == 'call' and i.op_str.startswith('0x') and int(i.op_str, 16) == SCALE:
                factors.append({'kind': 'scale_call', 'at': '0x%08X' % i.address, 'bytes': i.bytes.hex(),
                                'argument': prev[0]})
            prev = [prev[1], '%s %s' % (i.mnemonic, i.op_str)] if i.mnemonic == 'push' else prev
        kind, why = WHY.get(site, ('unread', 'not read yet: not changed'))
        out['0x%08X' % site] = {'func': users['0x%08X' % site]['func'], 'return': '0x%08X' % (site + 5),
                                'body': ['0x%08X' % loop_start, '0x%08X' % tail[1]], 'exit': '0x%08X' % exit_,
                                'kind': kind, 'why': why, 'factors': factors}
    json.dump({'source': 'pipeline/frame_step_loops.py on the analysed build ($FFXI_DLL)', 'loops': out},
              open(os.path.join(D, 'frame_step_loops.json'), 'w'), indent=1)
    for s, r in out.items(): print(s, r['func'], r['kind'], [f['kind'] + '@' + f['at'] for f in r['factors']])
    if a.header:
        ch = [(s, r) for s, r in out.items() if r['kind'] == 'chase']
        with open(a.header, 'w') as f:
            f.write('// sf_step_loops.h -- written by pipeline/frame_step_loops.py --header (do not edit): the calls of the step\n'
                    '// getter whose value is a loop count, the loop a chase (x += (target - x) * k a pass; data/frame_step_loops.json),\n'
                    '// by return address (call site + 5), analysed build; and where each loop\'s factor k comes from.\n#pragma once\n#include <stdint.h>\n')
            f.write('static const uint32_t kStepLoopRet[] = {\n')
            for s, r in ch: f.write('  %su,   // call at %s in %s\n' % (r['return'], s, r['func']))
            f.write('};\nstruct StepLoopOperand { uint32_t at; uint8_t op[2]; uint32_t constant; };   // the instruction, its opcode, its constant\n')
            f.write('static const StepLoopOperand kStepLoopOperand[] = {\n')
            for s, r in ch:
                for x in r['factors']:
                    if x['kind'] == 'operand':
                        b = bytes.fromhex(x['bytes'])
                        f.write('  {%su, {0x%02X, 0x%02X}, %su},   // %s, loop of %s\n' % (x['at'], b[0], b[1], x['constant'], x['value'], s))
            f.write('};\nstatic const uint32_t kStepLoopScaleCall[] = {   // `call 0x100272B0` (E8 rel32) in a chase loop\n')
            for s, r in ch:
                for x in r['factors']:
                    if x['kind'] == 'scale_call': f.write('  %su,   // loop of %s, factor %s\n' % (x['at'], s, x['argument']))
            f.write('};\n')
        print('wrote', a.header)


if __name__ == '__main__':
    main()
