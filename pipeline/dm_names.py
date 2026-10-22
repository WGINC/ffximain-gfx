"""dm_names.py -- DancingMad's names, read from its own IDA script, as data/dm_names.json.

DancingMad (github.com/WGINC/dancingmad) names addresses in tools/ffxi_symbols_ida.py, one call each:
`apply_name(0xADDR, 'Name', 'comment', is_func)`. The script is parsed (Python's ast), never run, and only calls
whose four arguments are literals are taken. hx2clean.py gives a tree function DancingMad's name where its own is
a placeholder (sub_, FUN_, ...). Rerun this whenever DancingMad's names change; the output records DancingMad's
commit so a name can be traced to it.

    python3 dm_names.py DANCINGMAD_DIR data/dm_names.json
Output: {"0xaddr": [name, comment, is_func], "_doc": {...}} (addresses lower-case hex, as hx2clean looks them up)."""
import ast, json, os, subprocess, sys


def read(dm_dir):
    src = open(os.path.join(dm_dir, 'tools', 'ffxi_symbols_ida.py'), encoding='utf-8', errors='replace').read()
    res, skipped, twice = {}, 0, []
    for node in ast.walk(ast.parse(src)):
        if not (isinstance(node, ast.Call) and getattr(node.func, 'id', None) == 'apply_name'): continue
        try: a, name, cmt, is_func = (ast.literal_eval(x) for x in node.args[:4])
        except (ValueError, TypeError): skipped += 1; continue
        k = hex(a)
        if k in res and res[k][0] != name: twice.append(k)
        res[k] = [name, cmt, bool(is_func)]
    return res, skipped, twice


def main(dm_dir, out):
    res, skipped, twice = read(dm_dir)
    # names in the existing file that DancingMad's script does not hold (the first file, 1 Oct, came from a September
    # update of DancingMad's names that its committed history does not contain): kept, and listed, not dropped
    prev = json.load(open(out)) if os.path.exists(out) else {}
    kept = sorted(k for k in prev if not k.startswith('_') and k not in res)
    for k in kept: res[k] = prev[k]
    kept_before = (prev.get('_doc') or {}).get('kept_from_previous_file', [])
    kept = sorted(set(kept) | set(x for x in kept_before if x in res))
    try: commit = subprocess.run(['git', '-C', dm_dir, 'rev-parse', 'HEAD'], capture_output=True, text=True).stdout.strip()
    except OSError: commit = None
    doc = {'source': 'DancingMad tools/ffxi_symbols_ida.py, apply_name calls with literal arguments', 'commit': commit,
           'names': len(res), 'calls_not_literal': skipped, 'named_twice_differently': twice,
           'kept_from_previous_file': kept}
    json.dump(dict(sorted(res.items()), _doc=doc), open(out, 'w'), indent=0, ensure_ascii=False)
    print(doc)


if __name__ == '__main__':
    main(*sys.argv[1:3])
