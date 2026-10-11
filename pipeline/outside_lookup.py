"""outside_lookup.py -- for an address in our build (a profile's call site, a hot function), the outside write-up's
entry for the function that contains it (data/outside_writeup_map.json; how it was mapped is in
docs/DECOMP_COMPARISON.md). Their name and entry are a lead to read, not a fact.

    python3 outside_lookup.py ADDR [ADDR...]          our addresses (hex)
    python3 outside_lookup.py --theirs ADDR [ADDR...] their entry addresses

Needs no DLL: the containing function comes from data/func_extents.json (all chunks).
"""
import json, os, sys

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')


def main(argv):
    m = json.load(open(os.path.join(D, 'outside_writeup_map.json')))['entries']
    if argv and argv[0] == '--theirs':
        for a in argv[1:]:
            e = m.get('0x%08X' % int(a, 16))
            print('%s: %s' % (a, 'no entry' if e is None else '%s (%s) -> ours %s %s' % (
                e['their_name'], e['how'], e['ours'], e['our_name'] or '')))
        return
    by_ours = {}
    for t, e in m.items():
        if e['ours']: by_ours[int(e['ours'], 16)] = (t, e)
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    for a in argv:
        x = int(a, 16)
        f = [int(k, 16) for k, chunks in ext.items() if any(s <= x < e for s, e in chunks)]
        if not f:
            print('0x%08X: in no known function' % x); continue
        for fa in f:
            hit = by_ours.get(fa)
            print('0x%08X in 0x%08X: %s' % (x, fa, 'no entry in the write-up' if hit is None else
                  'their %s %s (%s)' % (hit[0], hit[1]['their_name'], hit[1]['how'])))


if __name__ == '__main__':
    main(sys.argv[1:])
