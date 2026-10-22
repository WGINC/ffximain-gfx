"""reprefix_names.py -- apply batches/README.md's fallback to names whose area word the dossier does not show.

check_names.py refuses a name whose prefix is neither the batch's class, Unit<address> for a unit batch, nor a word
in the batch's dossier: that area word was invented by the naming session. The README's rule for that case is to
prefix with the batch's group instead. This does exactly that, nothing else: the rest of the name, the confidence,
evidence and summary are the session's and stay as written; the session's name is kept under "invented_prefix_name"
so the change can be read back.

    python3 reprefix_names.py data/names/NNN_group.json [...]
A batch whose group is a class keeps that class as prefix (CMoAttachments for CMoAttachments_VT_1032AE94).
Where the new name would collide with a name already taken, the entry is removed (reported): two functions cannot
share a name, and which of them the session meant is not known."""
import json, os, re, sys
import check_names

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')
INDEX = os.path.join(ROOT, 'batches', 'index.json')


def own_prefix(bid):
    group = bid.split('_', 1)[1]
    if group.startswith('unit_'): return 'Unit' + group[5:13]
    g = re.sub(r'_VT_[0-9A-F]{8}$', '', group)
    return g[0].upper() + g[1:]


def main(*paths):
    for path in paths:
        bid = os.path.basename(path)[:-5]
        d = json.load(open(path))
        # the checker's verdict per key, from its own output
        import io, contextlib
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            try: check_names.main(INDEX, path)
            except SystemExit: pass
        bad = {l.split(':', 1)[0] for l in out.getvalue().splitlines() if "prefix '" in l}
        changed = dropped = 0
        for k in sorted(bad):
            old = d[k]['name']
            new = own_prefix(bid) + '_' + old.split('_', 1)[1]
            d[k] = dict(d[k], name=new, invented_prefix_name=old); changed += 1
        json.dump(d, open(path, 'w'), indent=2); open(path, 'a').write('\n')
        # names now colliding (inside the file or with names elsewhere): the entry goes
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            try: check_names.main(INDEX, path)
            except SystemExit: pass
        for l in out.getvalue().splitlines():
            k = l.split(':', 1)[0]
            if k in d and ('already names' in l or 'also used for' in l):
                print('  %s %s: dropped (%s)' % (bid, k, l.split(': ', 1)[1])); d.pop(k); dropped += 1
        json.dump(d, open(path, 'w'), indent=2); open(path, 'a').write('\n')
        print('%s: %d reprefixed, %d dropped' % (bid, changed, dropped))


if __name__ == '__main__':
    main(*sys.argv[1:])
