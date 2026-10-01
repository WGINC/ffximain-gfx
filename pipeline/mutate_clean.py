"""mutate_clean.py -- gate sensitivity on the CLEAN graphics tree: for N random gate-PASS functions,
change one integer literal (+1) or one D3D enum value inside that function's body in its clean
source file, rebuild only that file, relink, and gate the function. The gate should reject it."""
import json, os, random, re, shutil, subprocess, sys
N = int(sys.argv[1]) if len(sys.argv) > 1 else 40
random.seed(101)
G = [json.loads(l) for l in open('gate_clean_final.jsonl')]
passed = [g['original'] for g in G if g['verdict'] == 'PASS']
cases = {c['original']: c for c in json.load(open('cases_cleanF.json'))}
W = 'mutclean'
shutil.rmtree(W, ignore_errors=True); shutil.copytree('clean', W)
FLAGS = ['-O2', '-mfpmath=387', '-march=i686', '-ffreestanding', '-fno-builtin', '-fno-asynchronous-unwind-tables',
         '-fexcess-precision=standard', '-fno-exceptions', '-fno-rtti', '-std=c++17', '-I' + W + '/include',
         '-fno-strict-aliasing', '-fwrapv', '-fno-delete-null-pointer-checks', '-fpermissive', '-w',
         '-DXI_API=__attribute__((dllexport))']
objs = sorted(os.path.join(W, 'build/obj', f) for f in os.listdir(W + '/build/obj') if f.endswith('.o') and f != 'rt.o')
D3D_SWAP = {'D3DCULL_NONE': 'D3DCULL_CW', 'D3DCULL_CCW': 'D3DCULL_CW', 'FALSE': 'TRUE', 'TRUE': 'FALSE',
            'D3DTEXF_LINEAR': 'D3DTEXF_POINT', 'D3DTEXF_POINT': 'D3DTEXF_LINEAR', 'D3DTOP_MODULATE': 'D3DTOP_ADD',
            'D3DTOP_SELECTARG1': 'D3DTOP_SELECTARG2', 'D3DTA_TEXTURE': 'D3DTA_DIFFUSE', 'D3DTA_DIFFUSE': 'D3DTA_TEXTURE',
            'D3DBLEND_SRCALPHA': 'D3DBLEND_ONE', 'D3DBLEND_INVSRCALPHA': 'D3DBLEND_ZERO', 'D3DTADDRESS_CLAMP': 'D3DTADDRESS_WRAP'}
results = []; pool = passed[:]; random.shuffle(pool)
for orig in pool:
    if len(results) >= N: break
    ea = int(orig, 16)
    src_files = [f for f in os.listdir('clean/src') if ('FFXIMAIN 0x%08X' % ea) in open('clean/src/' + f).read()]
    if not src_files: continue
    f = src_files[0]; text = open('clean/src/' + f).read()
    a = text.index('FFXIMAIN 0x%08X' % ea); b = text.find('\n}', a) + 2
    body = text[a:b]
    start = body.find('{')
    sites = [('lit', m) for m in re.finditer(r'(?<![\w.x])(\d+)u?(?![\w.])', body[start:])]
    sites += [('enum', m) for m in re.finditer(r'\b(' + '|'.join(D3D_SWAP) + r')\b', body[start:])]
    if not sites: continue
    kind, m = random.choice(sites)
    s0, s1 = start + m.start(1), start + m.end(1)
    new = str(int(m.group(1)) + 1) if kind == 'lit' else D3D_SWAP[m.group(1)]
    mut = body[:s0] + new + body[s1:]
    open(os.path.join(W, 'src', f), 'w').write(text[:a] + mut + text[b:])
    o = os.path.join(W, 'build/obj', f[:-4] + '.o')
    r = subprocess.run(['i686-w64-mingw32-g++'] + FLAGS + ['-c', os.path.join(W, 'src', f), '-o', o], capture_output=True)
    if r.returncode:
        open(os.path.join(W, 'src', f), 'w').write(text); continue
    dll = os.path.join(W, 'mut.dll')
    subprocess.run(['i686-w64-mingw32-g++', '-shared', '-nostdlib', '-o', dll] + objs + [os.path.join(W, 'build/obj/rt.o'), '-lgcc',
                    '-Wl,--image-base=0x60000000', '-Wl,-e,_DllMainCRTStartup@12', '-Wl,--disable-auto-image-base', '-Wl,--kill-at'],
                   capture_output=True)
    json.dump([cases[orig]], open(W + '/c.json', 'w'))
    if os.path.exists(W + '/g.jsonl'): os.remove(W + '/g.jsonl')
    subprocess.run(['python3', 'diffemu_gfx.py', 'FFXiMain_unpacked.dll', dll, W + '/c.json', W + '/g.jsonl',
                    '--trials', '100', '--seed', '11'], capture_output=True, timeout=900)
    g = json.loads(open(W + '/g.jsonl').read().splitlines()[0])
    results.append({'fn': orig, 'file': f, 'kind': kind, 'from': m.group(1), 'to': new, 'verdict': g['verdict'],
                    'returns': g.get('original_outcomes', {}).get('return', 0),
                    'why': (g.get('first_mismatch') or {}).get('why', '')[:140]})
    print(orig, kind, m.group(1), '->', new, g['verdict'], flush=True)
    # restore the file and its object for the next mutant
    open(os.path.join(W, 'src', f), 'w').write(text)
    subprocess.run(['i686-w64-mingw32-g++'] + FLAGS + ['-c', os.path.join(W, 'src', f), '-o', o], capture_output=True)
json.dump(results, open('mutants_clean.json', 'w'), indent=0)
import collections; print(collections.Counter(r['verdict'] for r in results))
