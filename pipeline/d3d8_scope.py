"""d3d8_scope.py -- what the game asks of Direct3D 8: the coverage list for our own device (docs/D3D11.md, phase 1).

    python3 d3d8_scope.py [--trace T ...] [--resources R ...] [--profile P ...] [--out data/d3d8_scope.json]

Sources, each kept apart in the output:
  static     data/render_map.json: every Direct3D call site of the graphics code with its interface and method, where
             render_map established them (static proof or observed through the fake device). Sites whose interface is
             not established are listed by slot, as not established.
  traces     xikit_trace_<pid>_<n>.txt (kit trace=1): methods, render states and stage states with the values set,
             vertex shader handles (FVF codes and shader handles kept apart: bit 0 set is a handle, D3D8), primitive
             types of the draws, and the state lines (#S) of each draw.
  resources  xikit_resources_<pid>.csv (kit profile=1): resource creations with their parameters.
  profile    d3dprof_<pid>.csv (kit profile=1): methods and their calls per frame.
  shaders    data/shader_catalogue.json: the programmable shaders the game assembles.
What the owner's runs did not reach is not in the traces, resources or profile parts; the static part is the bound.
Values are as the D3D8 headers number them; state names from the D3D8 header's enums (data/d3d8_ida.h)."""
import argparse, collections, csv, json, os, re
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
D = os.path.join(R, 'data')
DRAWS = {'DrawPrimitive', 'DrawIndexedPrimitive', 'DrawPrimitiveUP', 'DrawIndexedPrimitiveUP'}
PRIM = {1: 'POINTLIST', 2: 'LINELIST', 3: 'LINESTRIP', 4: 'TRIANGLELIST', 5: 'TRIANGLESTRIP', 6: 'TRIANGLEFAN'}


def header_names():
    """D3DRS_* and D3DTSS_* numbers to names, from the D3D8 header the analysis uses (data/d3d8_ida.h)."""
    h = open(os.path.join(D, 'd3d8_ida.h'), errors='replace').read()
    out = {'rs': {}, 'tss': {}}
    for pre, k in (('D3DRS_', 'rs'), ('D3DTSS_', 'tss')):
        for name, val in re.findall(r'\b(%s[A-Z0-9_]+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)' % pre, h):
            out[k].setdefault(str(int(val, 0)), name)
    return out


def static_part():
    rm = json.load(open(os.path.join(D, 'render_map.json')))['sites']
    known = collections.defaultdict(lambda: collections.defaultdict(list))
    unknown = collections.defaultdict(list)
    for site, v in sorted(rm.items()):
        st = v.get('static') or {}
        got = set()
        if st.get('method'): got.add((st['iface'], st['method'], st.get('slot'), 'static ' + st.get('tier', '')))
        for o in v.get('observed', []):
            if o.get('method'): got.add((o['iface'], o['method'], o.get('slot'), 'observed ' + o.get('kind', '')))
        if got:
            for iface, m, slot, how in got: known[iface][m].append(site)
        else:
            for o in v.get('observed', []): unknown[o.get('slot')].append(site)
    return ({i: {m: {'sites': len(s), 'example': s[0]} for m, s in sorted(ms.items())} for i, ms in sorted(known.items())},
            {str(k): {'sites': len(s), 'example': s[0]} for k, s in sorted(unknown.items(), key=lambda x: str(x[0]))})


def traces_part(paths, names):
    meth = collections.Counter(); rs = collections.defaultdict(collections.Counter); tss = collections.defaultdict(collections.Counter)
    fvf = collections.Counter(); vsh = collections.Counter(); prim = collections.Counter(); frames = set()
    for p in paths:
        for line in open(p, errors='replace'):
            f = line.split()
            if len(f) < 6 or line.startswith('#') or not f[0].isdigit(): continue
            m = f[3]; meth[m] += 1; frames.add((p, f[0], f[1]))
            n = int(f[5]) if f[5].isdigit() else 0
            a = [int(x, 16) for x in f[6:6 + n] if re.fullmatch(r'[0-9A-Fa-f]{1,8}', x)]
            if m == 'SetRenderState' and len(a) >= 2: rs[a[0]][a[1]] += 1
            elif m == 'SetTextureStageState' and len(a) >= 3: tss['%d.%d' % (a[0], a[1])][a[2]] += 1
            elif m == 'SetVertexShader' and a: (vsh if a[0] & 1 else fvf)[a[0]] += 1
            elif m in DRAWS and m != 'DrawIndexedPrimitiveUP' and a or m == 'DrawIndexedPrimitiveUP' and a: prim[PRIM.get(a[0], a[0])] += 1
    rsn = names.get('rs', {}); tssn = names.get('tss', {})
    def vals(c, lim=24):
        return {'%X' % k: n for k, n in c.most_common(lim)} | ({'...': '%d more values' % (len(c) - lim)} if len(c) > lim else {})
    return {
        'files': [os.path.basename(p) for p in paths], 'frames': len(frames),
        'methods': dict(meth.most_common()),
        'render_states': {'%d %s' % (k, rsn.get(str(k), '?')): vals(c) for k, c in sorted(rs.items())},
        'texture_stage_states': {'%s %s' % (k, tssn.get(k.split('.')[1], '?')): vals(c) for k, c in sorted(tss.items(), key=lambda x: tuple(map(int, x[0].split('.'))))},
        'fvf_codes': {'%X' % k: n for k, n in fvf.most_common()},
        'vertex_shader_handles': len(vsh),
        'primitive_types': dict(prim.most_common()),
    }


def resources_part(paths):
    """Two formats: the older log (frame,thread,method,call_site,result,object,params; the method's parameter list not
    quoted, so it spans fields) and the newer one (frame,thread,"method",call_site,module,result,us,object,params)."""
    out = collections.defaultdict(collections.Counter)
    for p in paths:
        for row in csv.reader(l for l in open(p, errors='replace') if not l.startswith('#')):
            if len(row) < 6 or '(' not in row[2]: continue
            k = 2
            while ')' not in row[k]: k += 1
            sig = ','.join(row[2:k + 1]); rest = row[k + 1:]
            # newer: call_site,module,result,us,object,params ; older: call_site,result,object,params
            if len(rest) > 3 and re.fullmatch(r'[0-9A-F]{8}', rest[2]) and not re.fullmatch(r'[0-9A-F]{8}', rest[1]):
                result, params = rest[2], rest[5:]
            else:
                result, params = rest[1], rest[3:]
            names = [x.strip() for x in sig[sig.find('(') + 1:sig.rfind(')')].split(',')]
            keep = {n: v for n, v in zip(names, params) if n in ('usage', 'format', 'pool', 'fvf', 'levels', 'multisample', 'lockable')}
            out[sig.split('(')[0]][json.dumps(keep, sort_keys=True) + ('' if result == '00000000' else ' result ' + result)] += 1
    return {'files': [os.path.basename(p) for p in paths], 'values': 'hex, as logged; format and pool as D3DFORMAT and D3DPOOL number them',
            'creations': {m: {k: n for k, n in c.most_common()} for m, c in sorted(out.items())}}


def profile_part(paths):
    out = {}
    for p in paths:
        per = collections.Counter()
        for row in csv.reader(l for l in open(p, errors='replace') if not l.startswith('#')):
            if len(row) < 4 or row[0] == 'method': continue
            try: per[row[0]] += float(row[3])
            except ValueError: pass
        out[os.path.basename(p)] = {k: round(v, 1) for k, v in per.most_common()}
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--trace', nargs='*', default=[]); ap.add_argument('--resources', nargs='*', default=[])
    ap.add_argument('--profile', nargs='*', default=[]); ap.add_argument('--out', default=os.path.join(D, 'd3d8_scope.json'))
    a = ap.parse_args()
    names = header_names()
    known, unknown = static_part()
    sh = json.load(open(os.path.join(D, 'shader_catalogue.json')))
    doc = {
        '_about': __doc__.split('\n\n')[0] + ' Built by pipeline/d3d8_scope.py; its docstring names the sources.',
        'static': {'methods_by_interface': known, 'sites_with_interface_not_established_by_slot': unknown},
        'traces': traces_part(a.trace, names) if a.trace else None,
        'resources': resources_part(a.resources) if a.resources else None,
        'profile_calls_per_frame': profile_part(a.profile) if a.profile else None,
        'programmable_shaders': [{k: s[k] for k in ('name', 'kind', 'built_at', 'declaration') if k in s} for s in sh['shaders']],
    }
    json.dump(doc, open(a.out, 'w'), indent=1); open(a.out, 'a').write('\n')
    print('static: %d interfaces, %d methods; %d sites with the interface not established'
          % (len(known), sum(len(v) for v in known.values()), sum(v['sites'] for v in unknown.values())))
    if doc['traces']: print('traces: %d frames, %d methods, %d render states, %d stage states, %d FVF codes' % (
        doc['traces']['frames'], len(doc['traces']['methods']), len(doc['traces']['render_states']),
        len(doc['traces']['texture_stage_states']), len(doc['traces']['fvf_codes'])))


if __name__ == '__main__':
    main()
