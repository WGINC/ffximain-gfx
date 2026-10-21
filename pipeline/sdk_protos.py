"""sdk_protos.py -- each import's return type as Microsoft's own headers declare it, with its size.

import_returns.py learns an import's return type from the exports' records, which type only the imports some
exported function calls. This reads the declarations themselves: Microsoft's preprocessor (Visual C++ 6.0's
CL.EXE /E, run under wine, never in this repository) expands the SDK headers' macros (WINAPI, WINBASEAPI, STDAPI,
DECLARE_HANDLE ...), and each import's declaration `RET __stdcall NAME(` is read from that text. The headers, in
the order given (their directories never in this repository either):

  * Visual C++ 6.0's Platform SDK headers (MSVC6_PP's ../include): winsock2.h, windows.h, ole2.h, oleauto.h,
    imm.h, mmsystem.h, with _WIN32_DCOM (objbase.h declares CoSetProxyBlanket only under it);
  * the DirectX 8.1 SDK's d3d8.h (DX8_INCLUDE);
  * DirectX's dinput.h of 2002 (DINPUT8_INCLUDE: a directory holding it), with DIRECTINPUT_VERSION 0x0800.

A return type's size comes from the same text: its `typedef` chain down to a C type (char 1, short 2, int and
long 4, __int64 8, a pointer 4, a function pointer 4, void 0). An interface method of the same name (`virtual`)
is not an import's declaration and is skipped; a name declared with two different return types is left out.

    MSVC6_PP=... DX8_INCLUDE=... DINPUT8_INCLUDE=... FFXI_DLL=... python3 sdk_protos.py data/sdk_import_returns.json
Output {"NAME": {"ret": "BOOL", "size": 4 | 2 | 1 | 0 | 8 | null}} for the DLL's imports the headers declare."""
import json, os, re, subprocess, sys, tempfile
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S

PROBE = '''#define _WIN32_DCOM
#define DIRECTINPUT_VERSION 0x0800
#include <winsock2.h>
#include <windows.h>
#include <ole2.h>
#include <oleauto.h>
#include <imm.h>
#include <mmsystem.h>
#include <d3d8.h>
#include <dinput.h>
'''
BASE = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'short': 2, 'unsigned short': 2, 'short int': 2,
        'unsigned short int': 2, 'wchar_t': 2, 'int': 4, 'unsigned int': 4, 'unsigned': 4, 'long': 4, 'unsigned long': 4,
        'long int': 4, 'unsigned long int': 4, 'signed': 4, 'signed int': 4, 'signed long': 4, '__int64': 8,
        'unsigned __int64': 8, 'void': 0, 'float': 4, 'double': 8}


def preprocess():
    pp = os.environ['MSVC6_PP'].rstrip('/')
    inc = ';'.join([os.environ['DINPUT8_INCLUDE'], os.environ['DX8_INCLUDE'], os.path.join(os.path.dirname(pp), 'include')])
    d = tempfile.mkdtemp(prefix='sdkpre_')
    open(os.path.join(d, 'probe.cpp'), 'w').write(PROBE)
    env = dict(os.environ, WINEDEBUG='-all', INCLUDE=inc)
    env.setdefault('WINEPREFIX', os.path.join(os.path.dirname(pp), '..', 'wineprefix'))
    r = subprocess.run(['wine', os.path.join(pp, 'CL.EXE'), '/nologo', '/E', 'probe.cpp'], cwd=d, capture_output=True,
                       text=True, errors='replace', env=env)
    if r.returncode: raise SystemExit('CL /E failed:\n' + r.stderr[-2000:])
    t = re.sub(r'^#line.*$', '', r.stdout, flags=re.M)
    t = re.sub(r'__declspec\s*\(\s*\w+\s*\)', '', t)
    return re.sub(r'\s+', ' ', t)


def typedefs(t):
    """{name: its type, or '*' for a pointer or function pointer} for the text's simple typedefs"""
    td = {}
    for m in re.finditer(r'\btypedef ([^;{}()]+);', t):           # `typedef CHAR *LPSTR, *PSTR;`: each declarator
        decls = m.group(1).split(',')
        first = re.match(r'^(.*?)\s*(\**)\s*(\w+)\s*$', decls[0])
        if not first or not first.group(1).strip(): continue
        base = first.group(1).strip()
        for k, dcl in enumerate(decls):
            x = re.match(r'^\s*(\**)\s*(\w+)\s*$', dcl) if k else first
            if not x: continue
            stars, name = (x.group(2), x.group(3)) if not k else (x.group(1), x.group(2))
            td.setdefault(name, '*' if stars or base.endswith('*') else base)
    for m in re.finditer(r'\btypedef [^;{}()]*\(\s*(?:__\w+\s*)?\*\s*(\w+)\s*\)\s*\(', t):
        td.setdefault(m.group(1), '*')
    return td


def size_of(ty, td, depth=0):
    ty = re.sub(r'\b(const|volatile|struct|union|enum)\b', ' ', ty).strip(); ty = re.sub(r'\s+', ' ', ty)
    if ty.endswith('*') or ty == '*': return 4
    if ty in BASE: return BASE[ty]
    if ty in td and depth < 32: return size_of(td[ty], td, depth + 1)
    return None


def main(out):
    t = preprocess(); td = typedefs(t)
    im = S.Image(os.environ['FFXI_DLL'])
    names = sorted(set(S.imports(im, os.environ['FFXI_DLL']).values()))
    res, ambiguous = {}, []
    for n in names:
        rets = set()
        for m in re.finditer(r'(?:^|[;{}])\s*([^;{}()]*?)\s*\b(?:__stdcall|__cdecl|__fastcall)\s+' + re.escape(n) + r'\s*\(', t):
            if re.search(r'\bvirtual\b', m.group(1)): continue
            r = re.sub(r'\bextern\b|"C"|\binline\b|\bstatic\b', ' ', m.group(1))
            rets.add(re.sub(r'\s+', ' ', r).strip())
        if len(rets) == 1:
            r = rets.pop(); res[n] = {'ret': r, 'size': size_of(r, td)}
        elif rets: ambiguous.append(n)
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d of %d imports declared (%d with two return types, left out: %s); not declared: %s' % (
        len(res), len(names), len(ambiguous), ambiguous, [n for n in names if n not in res and n not in ambiguous]))


if __name__ == '__main__':
    main(sys.argv[1])
