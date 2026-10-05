"""pin_layout.py -- keep every reconstructed function at the address it had in the previous build.

The gate cache (gate_cache.py) reuses a verdict only when every byte the run touched is the same. Code is
compiled one section per function (-ffunction-sections: `.text$<symbol>`); this writes a linker script that
puts each function's section at the offset recorded in LAYOUT (relative to the start of .text) when it still
fits in its slot (up to the next recorded function), so a change to one function moves no other function.
A function that grew past its slot, and a new one, go after the last recorded function. The layout written
back is the one the script lays down.

    python3 pin_layout.py LAYOUT.json OUT.ld OBJ.o [OBJ.o ...]
LAYOUT: {"obj.o:section": [offset, size]} (missing: the first build lays the functions out in file order)."""
import json, os, re, subprocess, sys

OBJDUMP = 'i686-w64-mingw32-objdump'


def sections(obj):
    """[(section name, size, alignment)] of the object's code sections, in file order"""
    out = subprocess.run([OBJDUMP, '-h', obj], capture_output=True, text=True, check=True).stdout
    res = []
    for m in re.finditer(r'^\s*\d+\s+(\.text\S*)\s+([0-9a-f]+)\s+[0-9a-f]+\s+[0-9a-f]+\s+[0-9a-f]+\s+2\*\*(\d+)', out, re.M):
        name, size, al = m.group(1), int(m.group(2), 16), 1 << int(m.group(3))
        if size: res.append((name, size, al))
    return res


def up(x, a): return (x + a - 1) // a * a


# code that is not ours (libgcc: ___chkstk_ms, 64-bit division ...) goes first, in this fixed region, so a
# function that moves to the end never moves it (calls into it from every other function would change); the
# link fails if it ever outgrows the region (the location counter cannot move backwards)
LIB_RESERVE = 0x2000


def plan(layout, objs):
    cur = {}
    for o in objs:
        for name, size, al in sections(o):
            cur['%s:%s' % (os.path.basename(o), name)] = (o, name, size, max(al, 16))
    rec = sorted((v[0], k, v[1]) for k, v in layout.items())
    slot = {k: (rec[i + 1][0] - off if i + 1 < len(rec) else up(size, 16)) for i, (off, k, size) in enumerate(rec)}
    place, end = {}, LIB_RESERVE
    for off, k, _ in rec:
        if k in cur and cur[k][2] <= slot[k]:
            place[k] = off
        end = max(end, off + slot[k])
    for k, (o, name, size, al) in cur.items():                 # new or grown: after everything recorded
        if k in place: continue
        end = up(end, al); place[k] = end; end += size
    return cur, place


def script(cur, place):
    """the default i386pe script with .text's input sections replaced by the placements"""
    default = subprocess.run(['i686-w64-mingw32-ld', '--verbose'], capture_output=True, text=True).stdout
    default = default.split('==================================================')[1]
    ours = sorted({os.path.dirname(cur[k][0]) or '.' for k in cur})
    excl = ' '.join('"%s/*"' % d for d in ours)
    body = ['    KEEP (*(SORT_NONE(.init)))',
            '    *(EXCLUDE_FILE(%s) .text)' % excl, '    *(EXCLUDE_FILE(%s) .text$*)' % excl,
            '    *(EXCLUDE_FILE(%s) .text.*)' % excl]
    for k in sorted(place, key=lambda k: place[k]):
        o, name, _, _ = cur[k]
        body.append('    . = 0x%X;' % place[k])
        body.append('    "%s"(%s)' % (o, name))
    body += ['    *(.text)', '    *(SORT(.text$*))', '    *(.text.*)']
    pat = re.compile(r'\s*KEEP \(\*\(SORT_NONE\(\.init\)\)\)\s*\*\(\.text\)\s*\*\(SORT\(\.text\$\*\)\)\s*\*\(\.text\.\*\)', re.S)
    if not pat.search(default): sys.exit('pin_layout: the default linker script has an unexpected .text section')
    return pat.sub('\n' + '\n'.join(body), default, count=1)


def main(layout_path, out, *objs):
    layout = json.load(open(layout_path)) if os.path.exists(layout_path) else {}
    cur, place = plan(layout, objs)
    open(out, 'w').write(script(cur, place))
    new = {k: [place[k], cur[k][2]] for k in cur}
    moved = [k for k in new if k in layout and layout[k][0] != new[k][0]]
    json.dump(dict(sorted(new.items())), open(layout_path, 'w'), indent=0)
    print('pin_layout: %d functions, %d kept their address, %d moved, %d new' % (
        len(new), len(new) - len(moved) - len(set(new) - set(layout)), len(moved), len(set(new) - set(layout))))


if __name__ == '__main__':
    main(*sys.argv[1:])
