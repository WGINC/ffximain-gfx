"""merge_names.py -- bring the naming sessions' files onto this branch, each checked first.

For each branch given, its data/names/*.json files that this branch does not hold yet (or holds differently) are
taken one at a time, written to data/names/, given reprefix_names.py's fallback for invented area words, and checked with check_names.py against everything already here.
A file that fails is removed again and reported; nothing else from the branch is taken, so a session's edits to
any other file never arrive.

    python3 merge_names.py BRANCH [BRANCH ...]      (e.g. origin/claude/names-w1; fetch them first)
Prints, per branch, the files merged and the files refused with the checker's reasons. Commit what it merged."""
import json, os, subprocess, sys

P = os.path.dirname(os.path.abspath(__file__)); ROOT = os.path.join(P, '..')
git = lambda *a: subprocess.run(['git', '-C', ROOT] + list(a), capture_output=True, text=True)


def main(*branches):
    for br in branches:
        files = [f for f in git('ls-tree', '--name-only', br, 'data/names/').stdout.split() if f.endswith('.json')]
        merged, refused = [], []
        for f in sorted(files):
            text = git('show', '%s:%s' % (br, f)).stdout
            dst = os.path.join(ROOT, f)
            if os.path.exists(dst) and open(dst).read() == text: continue
            try: json.loads(text)
            except ValueError as e: refused.append((f, 'not JSON: %s' % e)); continue
            old = open(dst).read() if os.path.exists(dst) else None
            open(dst, 'w').write(text)
            fix = subprocess.run([sys.executable, os.path.join(P, 'reprefix_names.py'), dst], capture_output=True, text=True).stdout.strip()
            r = subprocess.run([sys.executable, os.path.join(P, 'check_names.py'), os.path.join(ROOT, 'batches', 'index.json'), dst],
                               capture_output=True, text=True)
            if r.returncode:
                if old is None: os.remove(dst)
                else: open(dst, 'w').write(old)
                refused.append((f, r.stdout.strip().replace('\n', '; ')[:400]))
            else: merged.append((f, r.stdout.strip() + '  [' + fix.replace('\n', '; ') + ']'))
        print('%s: %d merged, %d refused' % (br, len(merged), len(refused)))
        for f, m in merged: print('  merged  %s  %s' % (os.path.basename(f), m))
        for f, m in refused: print('  REFUSED %s  %s' % (os.path.basename(f), m))


if __name__ == '__main__':
    main(*sys.argv[1:])
