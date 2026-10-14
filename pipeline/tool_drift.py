"""tool_drift.py -- are the tools up to date with the sources they are generated from?

Regenerates every generated tool file and rebuilds the kit, then compares:
  * ida/ffxi_ida_fix.py               make_ida_fix.py from data/ (names, type fixes, SP facts, ...)
  * data/shader_catalogue.json      kit/d3dprof/shader_catalogue.py: the game's shaders, assembled by its own D3DX
  * sdk/include/xikit_game.hpp,       gen_sdk.py from the tree and the kit's manifest (run by kit/xikit/build.sh)
    sdk/fork_flags.txt
  * kit/xikit/dist/*, kit/d3dprof/d3dprof.dll, kit/speedfix/dist/speedfix.dll, kit/d3d11probe/dist/*.exe,
                                      kit/d3d8on11/dist/d3d8.dll
                                      the builds (reproducible: --no-insert-timestamp) against kit/BUILD.lock.json,
                                      the hashes of the last build committed as current
  * xikit.ini keys                    the keys xikit.c reads = the keys of its default ini, each one in docs/KIT.md
Without --fix every regenerated file is put back as it was and the differences are reported (exit 1 if any).
With --fix the regenerated files stay and kit/BUILD.lock.json is rewritten: commit them, and ship the rebuilt kit.

    python3 pipeline/tool_drift.py [--fix]          (uses $FFXI_DLL, MinGW)"""
import hashlib, json, os, re, subprocess, sys
R = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
LOCK = os.path.join(R, 'kit', 'BUILD.lock.json')
WATCH = ['ida/ffxi_ida_fix.py', 'data/shader_catalogue.json', 'sdk/include/xikit_game.hpp', 'sdk/fork_flags.txt']


def sha(p): return hashlib.sha256(open(p, 'rb').read()).hexdigest()


def run(cmd, cwd=R):
    r = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if r.returncode: sys.exit('tool_drift.py: %s failed\n%s%s' % (' '.join(cmd), r.stdout[-3000:], r.stderr[-3000:]))


def built():
    """hashes of the build outputs (VERSION.txt left out: it carries the date)"""
    d = os.path.join(R, 'kit', 'xikit', 'dist')
    h = {'kit/xikit/dist/' + f: sha(os.path.join(d, f)) for f in sorted(os.listdir(d)) if f != 'VERSION.txt'}
    h['kit/d3dprof/d3dprof.dll'] = sha(os.path.join(R, 'kit', 'd3dprof', 'd3dprof.dll'))
    h['kit/speedfix/dist/speedfix.dll'] = sha(os.path.join(R, 'kit', 'speedfix', 'dist', 'speedfix.dll'))
    p = os.path.join(R, 'kit', 'd3d11probe', 'dist')
    for f in ('probe11_x86.exe', 'probe11_arm64.exe', 'probe8_x86.exe'): h['kit/d3d11probe/dist/' + f] = sha(os.path.join(p, f))
    h['kit/d3d8on11/dist/d3d8.dll'] = sha(os.path.join(R, 'kit', 'd3d8on11', 'dist', 'd3d8.dll'))
    return h


def ini_keys():
    s = open(os.path.join(R, 'kit', 'xikit', 'xikit.c')).read()
    read = set(re.findall(r'GetPrivateProfile\w*A\("xikit",\s*"(\w+)"', s))
    m = re.search(r'static const char DEFAULT_INI\[\] =\n(.*?);\n', s, re.S)
    dflt = set(re.findall(r'^(\w+)=', ''.join(eval(x) for x in re.findall(r'"(?:[^"\\]|\\.)*"', m.group(1))), re.M))
    doc = open(os.path.join(R, 'docs', 'KIT.md')).read()
    out = ['xikit.c reads %s, which its default ini lacks' % k for k in sorted(read - dflt)]
    out += ['the default ini has %s, which xikit.c never reads' % k for k in sorted(dflt - read)]
    out += ['docs/KIT.md does not mention %s=' % k for k in sorted(read | dflt) if not re.search(r'\b%s\b' % k, doc)]
    return out


def main():
    fix = '--fix' in sys.argv[1:]
    if not os.environ.get('FFXI_DLL'): sys.exit('tool_drift.py: set FFXI_DLL')
    before = {p: open(os.path.join(R, p), 'rb').read() for p in WATCH}
    problems, manual = [], []
    try:
        run([sys.executable, 'pipeline/make_ida_fix.py'])
        run([sys.executable, 'kit/d3dprof/shader_catalogue.py', os.path.join(R, 'runs', 'shaders')])
        run(['bash', 'kit/xikit/build.sh'])                       # also runs sdk/gen_sdk.py
        run(['sh', 'kit/d3dprof/build.sh'])
        run(['bash', 'kit/speedfix/build.sh'])
        run(['bash', 'kit/d3d11probe/build.sh'])
        run(['bash', 'kit/d3d8on11/build.sh'])
        for p in WATCH:
            if open(os.path.join(R, p), 'rb').read() != before[p]:
                problems.append('%s is not what its generator writes from the current sources' % p)
        now = built()
        lock = json.load(open(LOCK)) if os.path.exists(LOCK) else {}
        for k in sorted(set(now) | set(lock)):
            if now.get(k) != lock.get(k):
                problems.append('%s: the build gives %s, kit/BUILD.lock.json has %s' % (k, (now.get(k) or 'nothing')[:12],
                                                                                         (lock.get(k) or 'nothing')[:12]))
        manual = ini_keys()                                       # not regenerated: fixed by hand
        if fix: json.dump(now, open(LOCK, 'w'), indent=1, sort_keys=True); open(LOCK, 'a').write('\n')
    finally:
        if not fix:
            for p, b in before.items(): open(os.path.join(R, p), 'wb').write(b)
    for p in problems: print(('fixed: ' if fix else 'DRIFT: ') + p)
    for p in manual: print('DRIFT (fix by hand): ' + p)
    print('%d difference(s)%s' % (len(problems) + len(manual), ' (regenerated files kept, lock rewritten)' if fix and problems else ''))
    sys.exit(1 if manual or (problems and not fix) else 0)


if __name__ == '__main__':
    main()
