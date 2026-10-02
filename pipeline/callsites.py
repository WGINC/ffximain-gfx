"""callsites.py -- each call's argument count and convention as Hex-Rays decompiled it, from the export.

The IDA pass (ida/ffxi_ida_fix.py) records every call of each exported function: {"0xCALL": [nargs, cc]}.
The gate needs it where input repair points a fuzzed object's method at a trap (diffemu_gfx.site_pops):
the trap pops what the reconstruction pushes (Hex-Rays' count) and compares exactly those arguments.

    python3 callsites.py EXPORT.jsonl data/callsite_args.json
Output: {"0xCALL": {"args": stack argument dwords, "cc": cc}} (a __thiscall's `this` is in ECX, not counted).
"""
import json, sys


def main(exp, out):
    res = {}
    for l in open(exp):
        r = json.loads(l)
        for ea, (n, cc) in (r.get('calls') or {}).items():
            res[ea] = {'args': max(0, n - {'thiscall': 1, 'fastcall': 2}.get(cc, 0)), 'cc': cc}
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d call sites%s' % (len(res), '' if res else ' (the export has none: re-export with ida/ffxi_ida_fix.py)'))


if __name__ == '__main__':
    main(*sys.argv[1:3])
