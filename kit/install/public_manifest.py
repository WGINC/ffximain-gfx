"""public_manifest.py IN OUT -- the manifest that goes in the public kit (package.sh): each function's 20 bytes of the
game's code (EXPECTED) replaced by their fingerprint, H + 16 hex digits: FNV-1a 64 over the 20 bytes, the ones the kit
does not compare (a relocated dword straddling the window's edge) set to 0, as xikit.c's bytes_hash does with the
live bytes (relocations undone). The kit checks the live entry the same way before it writes anything. Dropped: the
!code lines (where the reconstruction's functions are, for crash reports in it; the public kit has no reconstruction)
and the comments. Everything else is as it was."""
import sys

def fnv(b):
    h = 0xCBF29CE484222325
    for x in b: h = ((h ^ x) * 0x100000001B3) & 0xFFFFFFFFFFFFFFFF
    return h

def line_of(l):
    p = l.split(' ')
    rva, flags, exp, hexs, rels = p[0], p[1], p[2], p[3], p[4]
    if len(hexs) != 40: raise SystemExit('public_manifest: not a 40-digit EXPECTED: %s' % l[:80])
    b = bytearray.fromhex(hexs); care = [1] * 20
    if rels != '-':
        for r in rels.split(','):
            r = int(r)
            if not (0 <= r and r + 4 <= 20):
                for k in range(4):
                    if 0 <= r + k < 20: care[r + k] = 0
    p[3] = 'H%016X' % fnv(bytes(x if c else 0 for x, c in zip(b, care)))
    return ' '.join(p)

def main(src, dst):
    out = []
    for l in open(src).read().splitlines():
        if not l or l.startswith('#') or l.startswith('!code '): continue
        out.append(l if l.startswith('!') else line_of(l))
    open(dst, 'w', newline='\n').write('\n'.join(out) + '\n')

if __name__ == '__main__': main(sys.argv[1], sys.argv[2])
