"""community_structs.py -- the community's declared layouts of the client's objects: member names at offsets.

  * Ashita v4 SDK (plugins/sdk/ffxi/*.h, LGPL-3.0): C++ structs. Their offsets are not written in the headers; the
    compiler gives them: every member is put through offsetof() in one translation unit compiled for 32-bit x86
    (i686-w64-mingw32-g++, the headers' own static_asserts on each struct's size checked in the same compile), and
    the values are read from the assembly it writes.
  * Windower 5 (libraries/memory/types.lua): explicit `name = {0xOFF, type}` entries, read as written.
A member's name and offset are each source's declaration, not a fact about the binary; data/community_structs.json
keeps them apart, with their source. community_scan.py ties a struct to the global the source locates it through.

    python3 community_structs.py ASHITA_DIR WINDOWER5_DIR OUT.json"""
import json, os, re, subprocess, sys, tempfile

CXX = os.environ.get('CXX', 'i686-w64-mingw32-g++')


def ashita_members(sdk):
    """{struct: [member, ...]} from the headers' struct bodies (data members only)"""
    out = {}
    for f in sorted(os.listdir(os.path.join(sdk, 'ffxi'))):
        t = open(os.path.join(sdk, 'ffxi', f), errors='replace').read()
        t = re.sub(r'/\*.*?\*/', '', t, flags=re.S); t = re.sub(r'//[^\n]*', '', t)
        for m in re.finditer(r'\bstruct\s+(\w+)\s*\{', t):
            depth, i = 1, m.end()
            while depth and i < len(t):
                depth += {'{': 1, '}': -1}.get(t[i], 0); i += 1
            body = t[m.end():i - 1]
            body = re.sub(r'\{[^{}]*\}', ';', body)                     # nested bodies / inline functions
            mem = []
            for d in body.split(';'):
                d = d.strip()
                if not d or '(' in d or d.startswith(('static', 'public', 'private', 'friend', 'using', 'typedef', 'enum', 'struct', 'union')):
                    continue
                mm = re.match(r'^([\w:<>,\s\*&]+?)[\s\*&]+(\w+)\s*(\[[^\]]*\])*$', d)
                if mm: mem.append((mm.group(2), re.sub(r'\s+', ' ', mm.group(1)).strip() + ('*' if re.search(r'\*\s*\w+\s*(\[|$)', d) else '')))
            if mem: out[m.group(1)] = mem
    return out


def ashita_offsets(sdk):
    members = ashita_members(sdk)
    heads = sorted(f for f in os.listdir(os.path.join(sdk, 'ffxi')) if f.endswith('.h'))
    lines = ['#include <cstddef>', '#include <cstdint>'] + ['#include "ffxi/%s"' % h for h in heads] + ['using namespace Ashita::FFXI;']
    probes = []; types = {}
    for s, mem in members.items():
        for k, (n, ty) in enumerate(mem):
            probes.append((s, n, 'p_%s__%d' % (s, k)))
            types[(s, n)] = ty
    def tu(ps):
        return '\n'.join(lines + ['extern "C" const unsigned %s = offsetof(%s, %s);' % (v, s, n) for s, n, v in ps]
                         + ['extern "C" const unsigned %s_sz = sizeof(((%s *)0)->%s);' % (v, s, n) for s, n, v in ps]) + '\n'
    with tempfile.TemporaryDirectory() as d:
        src = os.path.join(d, 'probe.cpp'); asm = os.path.join(d, 'probe.s')
        for _ in range(50):                                         # drop the probes the compiler refuses, retry
            open(src, 'w').write(tu(probes))
            r = subprocess.run([CXX, '-std=c++17', '-S', '-O0', '-I', sdk, '-o', asm, src], capture_output=True, text=True)
            if r.returncode == 0: break
            bad = {int(x) for x in re.findall(r'probe\.cpp:(\d+):\d+: error', r.stderr)}
            if not bad: raise SystemExit(r.stderr[:2000])
            first = len(lines) + 1; n_ = len(probes)
            probes = [p for k, p in enumerate(probes) if first + k not in bad and first + n_ + k not in bad]
        a = open(asm).read()
    vals = {m.group(1): int(m.group(2)) for m in re.finditer(r'_?(p_\w+):\s*\n\s*\.long\s+(-?\d+)', a)}
    out = {}
    for s, n, v in probes:
        if v in vals: out.setdefault(s, {})['0x%X' % vals[v]] = {'name': n, 'size': vals.get(v + '_sz'), 'type': types.get((s, n))}
    return out


def windower_structs(w5):
    t = open(os.path.join(w5, 'libraries', 'memory', 'types.lua')).read()
    out = {}
    for m in re.finditer(r'(?:local\s+(\w+)|types\.(\w+))\s*=\s*(?:struct|array)\(', t):
        name = m.group(1) or m.group(2)
        depth, i = 1, m.end()
        while depth and i < len(t):
            depth += {'(': 1, ')': -1}.get(t[i], 0); i += 1
        body = t[m.end():i - 1]
        fields = {}
        for f in re.finditer(r'^\s*(\w+)\s*=\s*\{\s*(0x[0-9A-Fa-f]+)\s*,\s*([^\n]*?)\}?\s*,?\s*(?:--\s*(.*))?$', body, re.M):
            if f.group(1).startswith('_dupe'): note = 'a duplicate'
            else: note = None
            fields['0x%X' % int(f.group(2), 16)] = {'name': f.group(1), 'type': f.group(3).strip().rstrip('},').strip(),
                                                   **({'comment': f.group(4).strip()} if f.group(4) else {}), **({'note': note} if note else {})}
        if fields: out[name] = fields
    return out


def main(ashita, w5, out):
    res = {'_sources': {'ashita': 'Ashita v4 SDK, plugins/sdk/ffxi/*.h (LGPL-3.0); offsets by offsetof, compiled for i686',
                        'windower5': 'Windower 5 packages, libraries/memory/types.lua; offsets as written'},
           'ashita': ashita_offsets(os.path.join(ashita, 'plugins', 'sdk')), 'windower5': windower_structs(w5)}
    json.dump(res, open(out, 'w'), indent=1, sort_keys=True)
    print('ashita: %d structs, %d members; windower5: %d structs, %d fields' % (
        len(res['ashita']), sum(len(v) for v in res['ashita'].values()), len(res['windower5']),
        sum(len(v) for v in res['windower5'].values())))


if __name__ == '__main__':
    main(*sys.argv[1:4])
