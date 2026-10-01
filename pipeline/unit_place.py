"""unit_place.py -- which source unit (.cpp/.c file of the original build) each function came from.

MSVC links each object file's functions contiguously, so a function's neighbours in the image are
evidence of its unit. Anchors (strongest first):
  assert     the function references a source-file path string (__FILE__ in an assert/log)
  vtable     the function stores exactly one known class vtable into an object (ctor/dtor)
  manifest   the class model's tree manifest places it in a class
  name       its IDB/DancingMad name starts with a known class or dancer module name
             (not vtable-slot names like X_VT_..._vf012: a subclass vtable holds inherited methods)
A function with no anchor of its own is placed only when the nearest anchors on BOTH sides belong
to the same unit and both are within 4 KB of it. No chaining: placed functions do not become anchors.

Usage: python3 unit_place.py unit_evidence.json tree_manifest.json dm_names.json class_model_include/ out.json
"""
import json, os, re, sys, bisect, collections

NEAR = 0x1000
WEAK_NAME = re.compile(r'_VT_[0-9A-F]+|_vf\d+|_Slot\d+|_Unk|_variant\d*|^sub_|^nullsub|^j_|^unknown_|^\?')

def main(evf, manf, dmf, incdir, outf):
    E = json.load(open(evf))
    man = {int(k, 16): v for k, v in json.load(open(manf)).items()}
    dm = {int(k, 16): v for k, v in json.load(open(dmf)).items()}
    classes = {fn[:-4] for fn in os.listdir(incdir) if fn.endswith('.hpp')}
    for e in E:
        classes.update(e[4])                                   # every class with a named vtable
    stems = {}
    for e in E:
        for s in e[3]:
            stems[os.path.splitext(s)[0]] = s
    # dancer (C library) modules: file stems from assert paths, plus PS2-name prefixes shared by at
    # least two functions (sqgxTextureAdd, sqgxTextureSet -> module sqgxTexture)
    pre = collections.Counter()
    for e in E:
        m = re.match(r'^(sq[a-z]+[A-Z][a-z0-9]+)', e[2] or '')
        if m and not WEAK_NAME.search(e[2]) and not e[2].endswith('_Candidate'): pre[m.group(1)] += 1
    modset = set(stems) | {m for m, c in pre.items() if c >= 2}
    modules = sorted(modset, key=len, reverse=True)            # dancer modules: sqgxTexture, sqsnScene, ...
    cls_sorted = sorted(classes, key=len, reverse=True)

    def name_unit(nm):
        if not nm or WEAK_NAME.search(nm): return None
        for c in cls_sorted:
            if nm.startswith(c + '_') or nm.startswith(c + '::'): return c
        for m in modules:
            if nm.startswith(m) and (len(nm) == len(m) or nm[len(m)].isupper() or nm[len(m)] == '_'): return m
        return None

    anchors = {}                                               # ea -> (unit, why)
    for ea, end, nm, srcs, vts in E:
        if len(srcs) == 1:
            anchors[ea] = (os.path.splitext(srcs[0])[0], 'asserts %s' % srcs[0]); continue
        if len(vts) == 1:
            anchors[ea] = (vts[0], 'stores %s vtable (ctor/dtor)' % vts[0]); continue
        m = man.get(ea)
        if m and m.get('class'):
            anchors[ea] = (m['class'], 'class model places it in %s' % m['class']); continue
        u = name_unit(nm)
        if not u and ea in dm: u = name_unit(dm[ea][0])
        if u: anchors[ea] = (u, 'name %s' % (nm if name_unit(nm) else dm[ea][0]))

    order = sorted((e[0], e[1]) for e in E)
    starts = [s for s, _ in order]
    akeys = sorted(anchors); aend = {s: e for s, e in order}
    out = {}
    for s, e in order:
        if s in anchors:
            out[hex(s)] = {'unit': anchors[s][0], 'why': anchors[s][1], 'kind': 'anchor'}; continue
        i = bisect.bisect_left(akeys, s)
        if i == 0 or i == len(akeys): continue
        p, n = akeys[i - 1], akeys[i]
        if anchors[p][0] == anchors[n][0] and s - aend[p] <= NEAR and n - e <= NEAR:
            out[hex(s)] = {'unit': anchors[p][0], 'kind': 'between',
                           'why': 'between 0x%08X (%s) and 0x%08X (%s), both within 4 KB' % (p, anchors[p][1], n, anchors[n][1])}
    # functions with no unit: group them by the anchor gap they sit in. Code between two anchors of
    # different units with nothing identifiable in between is one or more unnamed object files;
    # keeping a gap's functions together follows the link order, unlike cutting by address slice
    names = {s: nm for s, _, nm, _, _ in E}
    gap_members = {}
    for s, e in order:
        if hex(s) in out: continue
        i = bisect.bisect_left(akeys, s)
        p = akeys[i - 1] if i else None; n = akeys[i] if i < len(akeys) else None
        gap_members.setdefault((p, n), []).append(s)
    for (p, n), mem in gap_members.items():
        pre = collections.Counter(m.group(1) for s in mem for m in [re.match(r'^([A-Z][A-Za-z0-9]+)_', names[s] or '')]
                                  if m and not WEAK_NAME.search(names[s]))
        tag = ''
        if pre:
            top, c = pre.most_common(1)[0]
            if c >= 2 and c * 2 >= sum(pre.values()): tag = '_' + top
        u = 'unit_%08X%s' % (mem[0], tag)
        why = 'no unit evidence of its own; in the gap between %s and %s -- an unnamed source file' % (
            '0x%08X (%s)' % (p, anchors[p][0]) if p else 'image start', '0x%08X (%s)' % (n, anchors[n][0]) if n else 'image end')
        for s in mem: out[hex(s)] = {'unit': u, 'why': why, 'kind': 'gap'}
    json.dump(out, open(outf, 'w'), indent=0)
    print(len(anchors), 'anchors;', len(out), 'placed;', collections.Counter(v['kind'] for v in out.values()))

if __name__ == '__main__':
    main(*sys.argv[1:6])
