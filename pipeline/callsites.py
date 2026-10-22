"""callsites.py -- each call's argument count and convention as Hex-Rays decompiled it, from the export.

The IDA pass (ida/ffxi_ida_fix.py) records every call of each exported function: {"0xCALL": [nargs, cc]}.
The gate needs it where input repair points a fuzzed object's method at a trap (diffemu_gfx.site_pops):
the trap pops what the reconstruction pushes (Hex-Rays' count) and compares exactly those arguments.

    python3 callsites.py EXPORT.jsonl data/callsite_args.json
Output: {"0xCALL": {"args": stack argument dwords, "cc": cc, "src": "typed" | "cast" | "unknown" | "regs"}} (a
__thiscall's `this` is in ECX, not counted). Only "typed" (a type declared in the IDB) is used by the gate: "cast" is a
pointer type Hex-Rays made up from the pushes, which is exactly the guess this file exists to avoid.

The record gives the number of arguments, not where each one goes. For a standard convention the stack ones follow
from it (cdecl and stdcall: all; thiscall: all but ECX; fastcall: all but ECX and EDX). A __usercall/__userpurge
type ("other") places its arguments one by one, in registers or on the stack, and the record does not say which, so
its count is not a stack count: such a site is "regs", not "typed" (22 Oct: 66 of them contradicted the callee's own
`ret N`, data/ida_proto_origin.json).
"""
import json, sys

STANDARD = {'cdecl': 0, 'stdcall': 0, 'thiscall': 1, 'fastcall': 2}


def main(exp, out):
    res = {}
    for l in open(exp):
        r = json.loads(l)
        for ea, v in (r.get('calls') or {}).items():
            n, cc = v[0], v[1]; src = v[2] if len(v) > 2 else 'unknown'
            if cc not in STANDARD and src == 'typed': src = 'regs'
            res[ea] = {'args': max(0, n - STANDARD.get(cc, 0)), 'cc': cc, 'src': src}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d call sites%s' % (len(res), '' if res else ' (the export has none: re-export with ida/ffxi_ida_fix.py)'))


if __name__ == '__main__':
    main(*sys.argv[1:3])
