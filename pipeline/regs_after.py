"""regs_after.py -- whether any caller reads the ECX or EDX a function leaves at its return, from the callers' code.

For each function whose callers are all known (every reference is a direct `call`; arg_reads.regs_read_after), the
callers' code is followed from each return address with the register's value (byte_flow.py's may-walk; calls as
caller_reads.py follows them; a caller's own `ret` hands what it still holds on to its callers, a fixpoint). A
function is listed with `false` for a register no caller reads, `true` where some caller may. A function whose
callers are not all known is not listed.

gfx_cases.py uses EDX: a case whose prototype returns 8 bytes (EDX:EAX) compares only EAX where no caller reads
EDX after the call.

    FFXI_DLL=... python3 regs_after.py OUT.json
Output {"0xFUNC": {"ecx": bool, "edx": bool}}."""
import json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import arg_reads as A


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(P, '..', 'data', 'func_extents.json')))
    code = {int(k, 16): im.code(e) for k, e in ext.items()}
    starts = set(code)
    A.load_facts(im, starts)
    eax_read = json.load(open(os.path.join(P, '..', 'data', 'caller_reads.json')))
    after = A.regs_read_after(im, code, starts, eax_read)
    json.dump({'0x%08X' % f: after[f] for f in sorted(after)}, open(out, 'w'), indent=0, sort_keys=True)
    print('%d functions whose callers are all known; no caller reads ECX after the call: %d, EDX: %d'
          % (len(after), sum(not v['ecx'] for v in after.values()), sum(not v['edx'] for v in after.values())))


if __name__ == '__main__':
    main(*sys.argv[1:])
