"""ida_listing.py -- read an IDA .asm listing (File > Produce file > Create ASM file) without IDA.

The listing carries what the Hex-Rays export does not: initial values of data, strings (decoded
as Shift-JIS), the bodies of functions outside the graphics set, and the IDB's current names.

    python3 ida_listing.py LISTING proc NAME [NAME...]     disassembly, comments and blank lines removed
    python3 ida_listing.py LISTING data NAME [NAME...]     the bytes of a data item, as hex and as text
    python3 ida_listing.py LISTING xrefs NAME              the DATA/CODE XREF lines of a label
    python3 ida_listing.py LISTING labels OUT.json         every label (name -> line number)
    python3 ida_listing.py LISTING missing NAME...         which names no longer exist (renamed in the IDB)
    python3 ida_listing.py LISTING addrnames OUT.json      address -> current IDB name of every function, by
                                                           anchoring on sub_XXXXXXXX labels (data/func_extents.json)

Set $FFXI_LISTING instead of passing LISTING by writing `-` for it.
"""
import json, mmap, os, re, sys


class Listing:
    def __init__(self, path):
        self.f = open(path, 'rb')
        self.m = mmap.mmap(self.f.fileno(), 0, access=mmap.ACCESS_READ)

    def _find(self, label):
        m = re.search(rb'^' + re.escape(label.encode()) + rb'\b', self.m, re.M)
        return m.start() if m else -1

    def lines(self, label, limit=4000):
        p = self._find(label)
        if p < 0: return []
        return self.m[p:p + 400 * limit].decode('latin1').replace('\r', '').split('\n')[:limit]

    def proc(self, label):
        out = []
        for l in self.lines(label):
            s = re.sub(r'\s+;.*$', '', l).rstrip()
            if not s.strip() or s.strip().startswith(';'): continue
            out.append(s)
            if re.match(r'^%s\s+endp' % re.escape(label), s): break
        return out

    def data(self, label):
        out, first = bytearray(), True
        for l in self.lines(label, 400):
            if not l.strip() or l.strip().startswith(';'): continue
            m = re.match(r'^(\w+)?\s+(db|dw|dd)\s+(.*)$', l)
            if not m or (m.group(1) and not first): break
            first = False
            width = {'db': 1, 'dw': 2, 'dd': 4}[m.group(2)]
            for t in re.findall(r"'[^']*'|[0-9A-Fa-f]+h\b|\b\d+\b|\?|;.*", m.group(3)):
                if t.startswith(';'): break
                if t.startswith("'"): out += t[1:-1].encode('latin1')
                elif t == '?': out += b'\0' * width
                else: out += (int(t[:-1], 16) if t.endswith('h') else int(t)).to_bytes(width, 'little', signed=False)
        return bytes(out)

    def xrefs(self, label):
        out = []
        for l in self.lines(label, 60)[0:60]:
            if out and re.match(r'^\w', l): break
            out += re.findall(r'XREF: ([\w:@?$+]+)', l)
        return out

    def procs(self):
        return [m.group(1).decode() for m in re.finditer(rb'^(\S+)\s+proc\s', self.m, re.M)]

    def addrnames(self, starts):
        # The listing has no addresses, but procedures appear in address order and unnamed ones are
        # sub_ADDR. Between two such anchors, named procedures are assigned to the function starts
        # in between only when the counts agree exactly.
        starts = sorted(starts); idx = {a: i for i, a in enumerate(starts)}
        names, seq, last = {}, self.procs(), None
        pending = []
        for n in seq + ['sub_FFFFFFFF']:
            m = re.match(r'^sub_([0-9A-F]{8})$', n)
            if not m:
                pending.append(n); continue
            a = int(m.group(1), 16)
            if a != 0xFFFFFFFF and a not in idx: pending = []; last = None; continue
            hi = idx.get(a, len(starts)); lo = idx[last] + 1 if last is not None else None
            if lo is not None and hi - lo == len(pending):
                for k, nm in enumerate(pending): names[starts[lo + k]] = nm
            if a != 0xFFFFFFFF: names[a] = n
            pending = []; last = a
        return names

    def labels(self):
        return {m.group(1).decode(): self.m.count(b'\n', 0, m.start()) + 1
                for m in re.finditer(rb'^([A-Za-z_?@$][\w?@$]*)\s', self.m, re.M)}


def main():
    path, cmd, *args = sys.argv[1:]
    L = Listing(os.environ['FFXI_LISTING'] if path == '-' else path)
    if cmd == 'proc':
        for a in args: print('\n'.join(L.proc(a))); print()
    elif cmd == 'data':
        for a in args:
            b = L.data(a)
            print('%s (%d bytes): %s' % (a, len(b), b[:64].hex(' ')))
            print('   text: %s' % b.split(b'\0')[0].decode('cp932', 'replace'))
    elif cmd == 'xrefs':
        print('\n'.join(L.xrefs(args[0])))
    elif cmd == 'labels':
        json.dump(L.labels(), open(args[0], 'w'))
    elif cmd == 'addrnames':
        D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
        starts = [int(k, 16) for k in json.load(open(os.path.join(D, 'func_extents.json')))]
        n = L.addrnames(starts)
        json.dump({'0x%08X' % a: v for a, v in sorted(n.items())}, open(args[0], 'w'), indent=0)
        print(len(n), 'functions named')
    elif cmd == 'missing':
        have = L.labels()
        print('\n'.join(a for a in args if a not in have))


if __name__ == '__main__':
    main()
