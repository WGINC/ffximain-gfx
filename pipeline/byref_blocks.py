"""byref_blocks.py -- locals the original lays out as one block behind an address it passes, which Hex-Rays split into
separate scalars.

The original builds a structure or array on its stack (a vertex list, a matrix, a rect) and passes its address; Hex-Rays
names the first element BYREF and the others as separate variables at the following stack offsets, written and never
read again. Compiled as separate C++ variables, nothing ties them to the first one's address: the compiler keeps the
first and drops the rest as dead stores, and the callee reads whatever is on the stack there. (CDx::DrawTexturedScreenQuad
0x1000A5A0: the 4 vertices of the full-screen quad, 6 Oct; the gate stubbed the callee and compared the pointer only.)

For each BYREF variable with a stack offset, the block runs from it to the end of the local frame ([esp+X] and [ebp-Y] in
the export's declaration comments); see blocks(). Facts used: the export's own declarations (offsets, frame, types) and
its text.

    python3 byref_blocks.py [EXPORT.jsonl] OUT.json
"""
import json, os, re, sys
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
SIZE = {'char': 1, '_BYTE': 1, '__int8': 1, 'unsigned __int8': 1, 'bool': 1, '_WORD': 2, '__int16': 2, 'unsigned __int16': 2,
        'short': 2, 'int': 4, '_DWORD': 4, 'float': 4, 'unsigned int': 4, '__int32': 4, 'double': 8, '__int64': 8, '_QWORD': 8}
# a local's declaration: type, optional pointer star, name (Hex-Rays reuses argument-like names such as a3 for locals),
# optional array size, then the comment with its stack location
DECL = re.compile(r'^\s*([\w\s\*]+?)\s*(\*?)\s*\b([A-Za-z_]\w*)(\[(\d+)\])?;\s*//\s*(.*)$')


def size_of(t, ptr, n):
    if ptr or '*' in t: s = 4
    else: s = SIZE.get(' '.join(t.split()))
    return None if s is None else s * (int(n) if n else 1)


def decls_of(text):
    """{name: (esp offset, size, BYREF, ebp offset, declaration line index)} for the stack variables the export declares"""
    out = {}
    for i, l in enumerate(text.split('\n')):
        m = DECL.match(l)
        if not m: continue
        off = re.search(r'\[esp\+([0-9A-F]+)h\]', m.group(6)); ebp = re.search(r'\[ebp-([0-9A-F]+)h\]', m.group(6))
        if not off: continue
        out[m.group(3)] = (int(off.group(1), 16), size_of(m.group(1), m.group(2), m.group(5)), 'BYREF' in m.group(6),
                           int(ebp.group(1), 16) if ebp else None, i)
    return out


def blocks(text):
    """each BYREF variable's block: from it to the end of the local frame (its [esp+X] plus its [ebp-Y]: IDA's frame from
    the function's own stack analysis), every declared variable in between as a member at its offset, gaps as padding.
    The callee may read or write any of it (CDx::ApplyStereoEyeProjection 0x1000CF20: Hex-Rays declared the 64-byte
    matrix as float[12] and a float, and its last 12 bytes not at all). Listed when a member other than the BYREF variable
    is written and never read (the compiler may drop it) or the declared members do not cover the block (a callee writing
    the undeclared bytes overruns a translation that declares only them). None when declared variables overlap."""
    decls = decls_of(text)
    body = '\n'.join(l for l in text[max(text.find('{'), 0):].split('\n') if not DECL.match(l))   # the code, without the declarations
    out = []
    for name, (off, sz, byref, ebp, _) in decls.items():
        if not byref or sz is None or ebp is None: continue
        top = off + ebp
        mem = sorted((o, n, s_) for n, (o, s_, _, _, _) in decls.items() if off <= o < top)
        if any(s_ is None for _, _, s_ in mem): continue
        if any(a[0] + a[2] > b[0] for a, b in zip(mem, mem[1:])) or mem[-1][0] + mem[-1][2] > top: continue   # overlapping: not expressible
        dead = []
        for o, v, _ in mem[1:]:
            uses = re.findall(r'(?<![\w.])%s(?!\w)' % v, body)
            writes = re.findall(r'(?<![\w.&])%s\s*=(?!=)' % v, body) + re.findall(r'\*\(\w+ \*\)&%s\s*=(?!=)' % v, body)
            if uses and len(uses) == len(writes): dead.append(v)
        covered = sum(s_ for _, _, s_ in mem)
        if dead or covered < top - off:
            out.append({'byref': name, 'offset': '0x%X' % off, 'bytes': top - off, 'block': [n for _, n, _ in mem],
                        'members': [[n, '0x%X' % o, s_] for o, n, s_ in mem], 'write_only': dead, 'undeclared_bytes': top - off - covered})
    return out


def main(exp, out):
    res = {}
    for l in open(exp):
        d = json.loads(l)
        b = blocks(d.get('text') or '')
        if b: res['0x%08X' % (d['ea'] if isinstance(d['ea'], int) else int(d['ea'], 16))] = {'name': d.get('name'), 'blocks': b}
    json.dump(res, open(out, 'w'), indent=1)
    print('%d functions with a BYREF block holding a write-only local or undeclared bytes' % len(res))


if __name__ == '__main__':
    a = sys.argv[1:]
    main(a[0] if len(a) > 1 else os.path.join(D, 'exports', 'gfx_export.jsonl'), a[-1])
