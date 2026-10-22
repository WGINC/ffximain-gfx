"""make_ghidra_facts.py -- the verified facts for a Ghidra project (ApplyFacts.java), from the same data as the IDA
pass (make_ida_fix.py): names the code establishes (data/names_pending_idb.json, evidence_names.json) and, per
function, the facts from its code as a plate comment.

    python3 pipeline/ghidra/make_ghidra_facts.py ghidra/ffxi_facts.json"""
import json, os, sys
P = os.path.dirname(os.path.abspath(__file__)); R = os.path.join(P, '..', '..'); sys.path.insert(0, os.path.join(P, '..'))


def main(out):
    import importlib.util
    spec = importlib.util.spec_from_file_location('mif', os.path.join(R, 'pipeline', 'make_ida_fix.py'))
    # make_ida_fix.py builds its data at import and writes the IDA script; the same dictionary is wanted here
    src = open(os.path.join(R, 'pipeline', 'make_ida_fix.py')).read().split('blob = json.dumps')[0]
    g = {'__file__': os.path.join(R, 'pipeline', 'make_ida_fix.py')}
    exec(compile(src, 'make_ida_fix.py', 'exec'), g)
    data = g['data']
    names = {k: v['name'] for k, v in data['names'].items() if not k.startswith('_')}
    res = {'_doc': 'pipeline/ghidra/make_ghidra_facts.py: names and per-function facts from the code; ApplyFacts.java applies them',
           'names': names, 'facts': data['facts']}
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d names, %d functions with facts' % (len(names), len(data['facts'])))


if __name__ == '__main__':
    main(*sys.argv[1:])
