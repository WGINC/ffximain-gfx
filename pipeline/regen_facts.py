"""regen_facts.py -- re-derive every fact file in data/ from the DLL, in verify_all's order, until none changes.

The facts read each other (reg_saved reads cdecl_args, caller_reads reads reg_saved, x87_effects reads reg_saved,
...), and not all in list order, so one pass after a change to a tool or an input can leave a later fact stale for
an earlier one. Each pass runs verify_all.fact_commands in order, installing each result before the next runs; the
passes repeat until one changes nothing. That is the state verify_all's step 2 checks: every fact what its tool
derives from the committed inputs.

    FFXI_DLL=... python3 pipeline/regen_facts.py [--max N] [--only a.json,b.json]
Prints, per pass, the facts that changed and how many top-level keys of each.
"""
import json, os, shutil, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import verify_all as VA

ROOT, D = VA.ROOT, VA.D


def main():
    mx = int(sys.argv[sys.argv.index('--max') + 1]) if '--max' in sys.argv else 6
    only = set(sys.argv[sys.argv.index('--only') + 1].split(',')) if '--only' in sys.argv else None
    src = VA.load(os.path.join(ROOT, 'tree', 'source.json'))
    tmp = tempfile.mkdtemp(prefix='regen_facts_')
    texp = os.path.join(tmp, 'tree_export.jsonl')
    VA.sh('git show %s:data/exports/gfx_export.jsonl > %s' % (src['export_commit'], texp))
    cmds = [(f, c) for f, c in VA.fact_commands(texp, src['raw_gate']) if only is None or f in only]
    for n in range(1, mx + 1):
        changed = []
        for f, cmd in cmds:
            o = os.path.join(tmp, f)
            rc, out = VA.sh('python3 pipeline/' + cmd.format(o=o))
            if rc or not os.path.exists(o):
                print('pass %d: %s failed: %s' % (n, f, (out.strip().splitlines() or [rc])[-1])); return 1
            # vcall_resolve.py writes ecx_use.json next to its output
            outs = [(f, o)] + ([('ecx_use.json', os.path.join(tmp, 'ecx_use.json'))] if f == 'vcalls.json' else [])
            for g, og in outs:
                d = VA.jdiff(VA.load(og), VA.load(os.path.join(D, g)))
                if d:
                    shutil.copyfile(og, os.path.join(D, g)); changed.append((g, len(d)))
                    print('pass %d: %s changed (%d keys)' % (n, g, len(d)), flush=True)
        if not changed:
            print('pass %d: nothing changed' % n); shutil.rmtree(tmp); return 0
    print('still changing after %d passes' % mx); return 1


if __name__ == '__main__':
    sys.exit(main())
