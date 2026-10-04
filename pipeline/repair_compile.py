"""repair_compile.py -- for translation units that fail only on C-to-C++ pointer conversions,
insert the explicit cast C applied implicitly (`cannot convert 'A' to 'B' in assignment /
argument passing / initialization / return`; in a return only between two pointer types, which C converts without a cast), at the exact line/column the compiler reports, and retry (max 4 rounds).
Usage: python3 repair_compile.py DIR            (raw units: DIR/fn_*.cpp)
       python3 repair_compile.py CLEANDIR --clean (clean tree: CLEANDIR/src/*.cpp)"""
import os, re, subprocess, sys
D = sys.argv[1]; CLEAN = '--clean' in sys.argv
RX = re.compile(r"(?:fn_[0-9A-F]{8}|src/\w+)\.cpp:(\d+):(\d+): error: cannot convert '([^']+)'(?: \{aka '[^']+'\})? to '([^']+)'(?: \{aka '([^']+)'\})? in (assignment|argument passing|initialization|return)")
def span_end(line, start, stop_chars):
    depth = 0
    for i in range(start, len(line)):
        c = line[i]
        if c in '([{': depth += 1
        elif c in ')]}':
            if depth == 0: return i
            depth -= 1
        elif depth == 0 and c in stop_chars: return i
    return len(line)
fixed = 0
CFLAGS = ['-O2', '-mfpmath=387', '-march=i686', '-ffreestanding', '-fno-builtin', '-fno-asynchronous-unwind-tables',
          '-fexcess-precision=standard', '-fno-exceptions', '-fno-rtti', '-std=c++17', '-Iinclude', '-fno-strict-aliasing',
          '-fwrapv', '-fno-delete-null-pointer-checks', '-fpermissive', '-w', '-DXI_API=__attribute__((dllexport))']
SRC = os.path.join(D, 'src') if CLEAN else D
ERR = os.path.join(D, 'build', 'err') if CLEAN else os.path.join(D, 'err')
OBJ = os.path.join(D, 'build', 'obj') if CLEAN else os.path.join(D, 'obj')
def recompile(src, b):
    if not CLEAN: return subprocess.run([os.path.join(os.path.dirname(os.path.abspath(__file__)), 'compile_one.sh'), src, D], capture_output=True)
    r = subprocess.run(['i686-w64-mingw32-g++'] + CFLAGS + ['-c', 'src/' + b + '.cpp', '-o', 'build/obj/' + b + '.o'],
                       cwd=D, capture_output=True, text=True)
    open(os.path.join(ERR, b + '.txt'), 'w').write(r.stderr)
    if r.returncode and os.path.exists(os.path.join(OBJ, b + '.o')): os.remove(os.path.join(OBJ, b + '.o'))
for f in sorted(os.listdir(SRC)):
    if (not CLEAN and not f.startswith('fn_')) or not f.endswith('.cpp'): continue
    b = f[:-4]
    if os.path.exists(os.path.join(OBJ, b + '.o')) or not os.path.exists(os.path.join(ERR, b + '.txt')): continue
    src = os.path.join(SRC, f)
    for rnd in range(4):
        # gcc quotes with \u2018\u2019 under a UTF-8 locale (Python sets LC_CTYPE=C.UTF-8 for its children), ' otherwise
        err = open(os.path.join(ERR, b + '.txt'), encoding='utf-8', errors='replace').read().replace('\u2018', "'").replace('\u2019', "'")
        hits = RX.findall(err)
        if not hits or 'error' in RX.sub('', err).replace('In function', ''):
            # only repair units whose errors are all conversion errors
            other = [l for l in err.splitlines() if ': error:' in l and not RX.search(l)]
            if other or not hits: break
        lines = open(src).read().split('\n')
        for ln, col, a, bt, aka, kind in sorted(hits, key=lambda h: (-int(h[0]), -int(h[1]))):
            ln = int(ln) - 1; col = int(col) - 1; L = lines[ln]
            if kind == 'assignment':
                # gcc points at the operator inside the right-hand side, not its start: cast the whole RHS
                m_eq = re.search(r'(?<![=!<>+\-*/%&|^])=(?!=)', L)
                start = m_eq.end() if m_eq and m_eq.start() < col + 1 else col
                end = span_end(L, start, ';')
            elif kind == 'return':
                m_r = re.search(r'\breturn\b', L)
                if not (m_r and '*' in a and '*' in bt): continue
                start = m_r.end(); end = span_end(L, start, ';')
            else:
                start = col; end = span_end(L, start, ',;')
            target = bt.replace(' ', '') if '*' in bt else bt
            lines[ln] = L[:start] + '(%s)(%s)' % (bt, L[start:end].strip()) + L[end:]
        open(src, 'w').write('\n'.join(lines))
        recompile(src, b)
        if os.path.exists(os.path.join(OBJ, b + '.o')):
            fixed += 1; break
print('repaired', fixed)
