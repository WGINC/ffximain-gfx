"""points_to.py -- whole-program points-to analysis of FFXiMain.dll: what every pointer can point to.

The base of de-artifacting phases 2-3 (docs/DEARTIFACT.md): a field, a global or a local can be given a type
only when every value that can reach it has that type, and "every value" needs every store in the program
that can hit it -- including stores through pointers whose target is not obvious. This is a may-analysis:
each value is a set of (object, offset) targets, and a store through a pointer the analysis cannot bound
(UNK) is recorded as able to hit any memory. Nothing it reports is narrower than the code allows, given
the premises below; where it cannot bound a value it says UNK.

Abstract objects:
  ('D',)            the image's data, offset = absolute address (initial contents: the image's own bytes)
  ('C',)            code: offset = the address (function pointers, vtable slots)
  ('F', f)          function f's stack frame, offset from f's entry ESP (args at +4, +8, ...)
  ('H', site)       memory returned by an allocator call at `site` (premise P2)
  ('X', iface)      a Direct3D 8 object; ('XV', iface) its vtable; ('XM',) memory D3D hands out
  ('API',)          memory any other Windows API hands out
  ('U',)            unknown (UNK): anything

Model (all whole-program, context-insensitive; registers flow-sensitive within a function, memory not):
  * ESP at each instruction from sp_solve.insn_depths (exact only where solved; elsewhere the frame offset
    is unknown and an access is to the whole frame);
  * a direct call passes every register and 16 stack dwords to the callee; afterwards each register the
    callee may change (computed: written and not saved by it or anything it calls) holds what the callee
    leaves there; an indirect call goes to every function its operand's value can hold, and to an unknown
    callee if that value can be UNK;
  * an unknown callee or an import may write UNK through any pointer it is given (one level) and returns
    UNK; a function whose address is passed to one of them (a callback) receives UNK in every register and
    argument; the DLL's exports and entry point too;
  * Direct3D 8 calls (an object of a known interface, a vtable slot of it, data/com_spec.json from d3d8.h):
    the header's out-parameter receives an object of the header's interface, any other pointer argument's
    memory D3D data ('XM'), the result is not a pointer;
  * pointer arithmetic with a non-constant keeps the object and loses the offset; with a constant, exact.

Premises (not shown from the code; every result depending on them says so in "premises"):
  P1  an access through a pointer into an object stays inside that object (C/C++ object model): a
      pointer to object O plus an unknown value is O at an unknown offset, never another object.
  P2  an allocator returns memory holding no live object (the malloc contract): Allocator_Generic,
      Mem_AllocDefault (data/allocators.json), HeapAlloc/GlobalAlloc/VirtualAlloc/HeapReAlloc/
      CoTaskMemAlloc (Windows SDK). Without it every heap object is one object.
  P5  no code uses uninitialised stack memory as a pointer (undefined behaviour in C/C++): a value with no
      target at the fixpoint is not a pointer to anything the program can reach, and an access through it
      is not modelled. (Registers are not covered by this premise: every register a function reads at entry
      gets whatever every caller leaves in it.)
  P6  a data object in the image starts at an address the program refers to (an operand or a stored
      pointer) and ends before the next such address: an indexed access [table + i*s] stays in the table
      the address `table` falls in. (Without it any indexed read of a global table can be any pointer.)
  (No closed-world premise: code outside the DLL -- the callers of its exports and entry point, of callbacks
  it hands out, and the code behind an unknown call target -- may read and write every object the DLL has
  exposed to it: anything passed to an import or an unknown callee, returned to an outside caller, or stored
  in external memory, and everything reachable from those. An unknown call target reaches every
  address-taken function.)
  P3  a called function preserves a register it saves on entry and restores (MSVC prologue/epilogue)
      and imports preserve EBX/ESI/EDI/EBP (Windows x86 calling convention).

    python3 points_to.py OUT.json [--max-rounds N]        uses $FFXI_DLL
"""
import bisect, collections, json, os, pickle, sys, time
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S
import com_calls as C

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
R = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'runs')
GPR = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp')
SAVED = ('ebx', 'esi', 'edi', 'ebp')
r32 = C.r32
E = frozenset()
UNK = (('U',), None)
U1 = frozenset([UNK])
EXTV = frozenset([(('EXT',), None)])        # memory owned by code outside the DLL
EXPV = frozenset([(('EXT',), None), (('EXP',), None)])   # what outside code can hand back: its own or exposed memory
ALLOC_API = {'HeapAlloc', 'GlobalAlloc', 'VirtualAlloc', 'HeapReAlloc', 'CoTaskMemAlloc', 'CoTaskMemRealloc', 'LocalAlloc'}
NARGS = 16


# ------------------------------------------------------------------ IR
def memop(i, o):
    return (r32(i.reg_name(o.mem.base)) if o.mem.base else None, r32(i.reg_name(o.mem.index)) if o.mem.index else None,
            o.mem.disp & 0xFFFFFFFF if not o.mem.base and not o.mem.index else o.mem.disp, o.size, bool(o.mem.segment))


def src_of(i, o):
    if o.type == X86_OP_REG: return ('R', r32(i.reg_name(o.reg)), o.size)
    if o.type == X86_OP_IMM: return ('I', o.imm & 0xFFFFFFFF)
    return ('M', memop(i, o))


def ir(i):
    """the instruction as one of a few forms the analysis interprets"""
    m = i.mnemonic; ops = i.operands
    if m in ('cmp', 'test', 'nop') or (m.startswith('j') and m != 'jmp') or m.startswith(('loop', 'fcom', 'fucom', 'ftst', 'fxam', 'bt ')): return ('NOP',)
    if m == 'mov' and len(ops) == 2:
        if ops[0].type == X86_OP_REG:
            if ops[0].size == 4: return ('MOV', r32(i.reg_name(ops[0].reg)), src_of(i, ops[1]))
            return ('GEN', [r32(i.reg_name(ops[0].reg))], [src_of(i, ops[1]), ('R', r32(i.reg_name(ops[0].reg)), 4)], [])
        return ('ST', memop(i, ops[0]), src_of(i, ops[1]))
    if m in ('movzx', 'movsx'):
        return ('GEN', [r32(i.reg_name(ops[0].reg))], [], [])         # a byte or word: never a 32-bit pointer
    if m == 'lea': return ('LEA', r32(i.reg_name(ops[0].reg)), memop(i, ops[1]))
    if m == 'xor' and ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG and ops[0].reg == ops[1].reg:
        return ('MOV', r32(i.reg_name(ops[0].reg)), ('I', 0)) if ops[0].size == 4 else ('NOP',)
    if m in ('add', 'sub') and ops[0].type == X86_OP_REG and ops[0].size == 4 and ops[1].type == X86_OP_IMM:
        return ('ADDI', r32(i.reg_name(ops[0].reg)), ops[1].imm if m == 'add' else -ops[1].imm)
    if m in ('inc', 'dec') and ops[0].type == X86_OP_REG and ops[0].size == 4:
        return ('ADDI', r32(i.reg_name(ops[0].reg)), 1 if m == 'inc' else -1)
    if m == 'push': return ('PUSH', src_of(i, ops[0]) if ops else ('I', 0))
    if m == 'pop': return ('POP', r32(i.reg_name(ops[0].reg)) if ops[0].type == X86_OP_REG else memop(i, ops[0]))
    if m == 'leave': return ('LEAVE',)
    if m == 'call': return ('CALL', src_of(i, ops[0]))
    if m.startswith('ret'): return ('RET',)
    if m == 'jmp':
        return ('JMP', src_of(i, ops[0]))
    if m == 'xchg' and len(ops) == 2:
        if ops[0].type == X86_OP_REG and ops[1].type == X86_OP_REG and ops[0].size == 4:
            return ('XCHG', r32(i.reg_name(ops[0].reg)), r32(i.reg_name(ops[1].reg)))
    if m.startswith(('rep', 'movs', 'stos', 'lods', 'scas', 'cmps')):
        return ('STR', m, i.op_str)
    if m in ('pushal', 'pushad', 'popal', 'popad', 'pushfd', 'popfd', 'pushf', 'popf'): return ('GEN', list(GPR) if m.startswith('pop') else [], [], [])
    # anything else: registers it writes get a value derived from what it reads; memory it writes likewise
    wr, rd, mw = [], [], []
    for k, o in enumerate(ops):
        dest = k == 0 and m not in ('push',) and not m.startswith(('fld', 'fild', 'fcom', 'ficom', 'fucom', 'fbld'))
        if o.type == X86_OP_REG:
            n = r32(i.reg_name(o.reg))
            if n in GPR or n == 'esp':
                rd.append(('R', n, o.size))
                if dest and n != 'esp': wr.append(n)
        elif o.type == X86_OP_MEM:
            mo = memop(i, o)
            if dest and (len(ops) > 1 or m not in ('fld', 'fild', 'push')): mw.append(mo)
            if not m.startswith(('fst', 'fist', 'fnst', 'fbstp', 'set')): rd.append(('M', mo))
    for r in C.written(i):
        if r not in wr: wr.append(r)
    if m.startswith(('f', 'set')) or m in ('fnstsw', 'cdq', 'cpuid', 'rdtsc', 'lahf', 'sahf'):
        return ('GEN', wr, [], mw)                                     # x87/flags results are never pointers
    return ('GEN', wr, rd, mw)


def build(im, ext, cache):
    """per function: (entry, {addr: ir}, {addr: [successors] or None}, {addr: depth}) -- cached"""
    if os.path.exists(cache):
        return pickle.load(open(cache, 'rb'))
    purges = json.load(open(os.path.join(D, 'purges.json')))
    starts = set(ext); out = {}
    for f, e in ext.items():
        ins = im.code(e)
        if f not in ins: continue
        labels = S.labels_of(ins)
        dep = S.insn_depths(im, f, e, purges, starts)
        out[f] = ({a: ir(i) for a, i in ins.items()}, {a: C.successors(im, ins, a, i, labels) for a, i in ins.items()},
                  {a: d[0] for a, d in dep.items()}, {a: d[1] for a, d in dep.items()})
    os.makedirs(os.path.dirname(cache), exist_ok=True)
    pickle.dump(out, open(cache, 'wb'))
    return out


# ------------------------------------------------------------------ solver
WIDEN = 4


BASE_OF = [None]          # set by PT: the data object an address belongs to (premise P6)


def lose(o, off):
    """the target (o, off) at an unknown offset: inside the same object (P1); for the image's data, inside the
    data object holding `off` (P6), which is ('DA', its start)"""
    if o == ('U',): return UNK
    if o == ('D',): return (('DA', BASE_OF[0](off)), None) if off is not None else UNK
    return (o, None)


def norm(v):
    """UNK already stands for every target: a set holding it is just UNK"""
    return U1 if UNK in v else v


def widen(v):
    """at a join: an object reached at more than WIDEN offsets is reached at an unknown offset (a pointer stepped in
    a loop would otherwise take a new offset every pass); sound, as (obj, None) covers every offset"""
    if UNK in v: return U1
    if len(v) <= WIDEN: return v
    c = collections.Counter(o if o != ('D',) else ('DA', BASE_OF[0](off)) for o, off in v if off is not None)
    many = {o for o, n in c.items() if n > WIDEN}
    if not many: return v
    return frozenset(lose(o, off) if (o if o != ('D',) else ('DA', BASE_OF[0](off))) in many and off is not None else (o, off) for o, off in v)


class PT:
    def __init__(self, im, path, ext, fns):
        self.im = im; self.fns = fns; self.starts = set(fns)
        pe = __import__('pefile').PE(path, fast_load=True); pe.parse_data_directories()
        base = pe.OPTIONAL_HEADER.ImageBase
        self.code_rng = [(base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize) for s in pe.sections if s.Characteristics & 0x20000000]
        self.img_rng = (base, base + pe.OPTIONAL_HEADER.SizeOfImage)
        self.iat = S.imports(im, path)
        self.spec = json.load(open(os.path.join(D, 'com_spec.json')))
        self.allocs = {int(k, 16) for k in json.load(open(os.path.join(D, 'allocators.json'))) if k.startswith('0x')}
        self.M = collections.defaultdict(dict)             # obj -> {off or None: frozenset}
        self.ANYW = E                                      # stored through UNK
        self.IN = collections.defaultdict(dict)            # f -> {reg: frozenset}
        self.EXIT = collections.defaultdict(dict)          # f -> {reg: frozenset}
        self.readers = collections.defaultdict(set)        # obj -> functions that read it
        self.callers = collections.defaultdict(set)
        self.any_readers = set()
        self.dirty = set(fns)
        self.unk_stores = {}                               # insn -> function (stores through UNK)
        self.NPW = {}                                      # obj -> offsets where a non-pointer value is stored
        self.unk_calls = {}                                # insn -> function (calls to an unknown callee)
        self.unfollowed = set()                            # functions with control flow this cannot follow
        self.clob = self.clobbers()
        self.taken = self.address_taken(); self.taken_external = False
        self.refs = self.data_refs(); self.dkeys = []; self._sr = {}; self._slots = {}
        self.HUB_IN, self.HUB_EXIT = {}, {}
        self.HUB_CLOB = set().union(*(self.clob.get(g, set(GPR)) for g in self.taken)) if self.taken else set()
        self.exposed = set(); self.exposed_new = False; self.EXPV = EXPV; self.ext_funcs = set(); self.unk_callers = set()
        BASE_OF[0] = self.base_of
        entry = [base + pe.OPTIONAL_HEADER.AddressOfEntryPoint] + [base + s.address for s in getattr(getattr(pe, 'DIRECTORY_ENTRY_EXPORT', None), 'symbols', [])]
        for f in entry:
            if f in fns: self.external(f)

    # values
    def const(self, v):
        if any(a <= v < b for a, b in self.code_rng): return frozenset([(('C',), v)])
        if self.img_rng[0] <= v < self.img_rng[1]: return frozenset([(('D',), v)])
        return E

    def static(self, a):
        o = a - self.im.base
        if 0 <= o <= len(self.im.img) - 4 and not any(x <= a < y for x, y in self.code_rng):
            return self.const(int.from_bytes(self.im.img[o:o + 4], 'little'))
        return E

    def read(self, obj, off, f, raw=False):
        if obj == ('U',): return U1
        if obj[0] == 'X': return frozenset([(('XV', obj[1]), 0)]) if off == 0 else frozenset([(('XM',), None)])
        if obj[0] == 'XV':
            return frozenset([(('XC', obj[1]), off // 4)]) if off is not None and off % 4 == 0 else U1
        if obj[0] in ('C', 'XC'): return E
        if f is not None: self.readers[obj].add(f); self.any_readers.add(f)
        cells = self.M.get(obj, {})
        hubv = E
        if obj[0] == 'F' and obj[1] != 'HUB' and obj[1] in self.taken and ('F', 'HUB') in self.M:
            hc = self.M[('F', 'HUB')]                      # what unknown calls pass (the hub)
            hubv = frozenset().union(*hc.values()) if off is None else hc.get(off, E) | hc.get(None, E)
        if obj in self.exposed and not raw:              # outside code's writes through ('EXP',)
            ev = self.M.get(('EXP',), {}).get(None, E)
            if f is not None: self.readers[('EXP',)].add(f)
            return norm(self.read(obj, off, f, raw=True) | ev)
        if obj == ('EXP',):
            return norm((frozenset().union(*cells.values()) if cells else E) | EXPV | self.ANYW)
        if obj == ('EXT',):                                # external memory: external or exposed values
            return norm((frozenset().union(*cells.values()) if cells else E) | self.EXPV | self.ANYW)
        if obj[0] == 'DA':                                 # a data object at an unknown offset (P6)
            self.readers[('D',)].add(f)
            a = obj[1]; b = self.next_ref(a); dc = self.M.get(('D',), {})
            i = bisect.bisect_left(self.dkeys, a); v = frozenset().union(*cells.values()) if cells else E
            while i < len(self.dkeys) and self.dkeys[i] < b: v = v | dc[self.dkeys[i]]; i += 1
            return norm(v | self.static_range(a, b) | self.ANYW)
        if obj == ('D',) and off is not None:
            da = self.M.get(('DA', self.base_of(off)))
            if da: cells = dict(cells); cells[None] = frozenset().union(*da.values())
            self.readers[('DA', self.base_of(off))].add(f)
        if off is None:
            v = frozenset().union(*cells.values()) if cells else E
            if obj == ('D',): v = v | U1                   # any data address: every pointer the image holds
        else:
            v = cells.get(off, E) | cells.get(None, E)
            if obj == ('D',): v = v | self.static(off)
        return norm(v | hubv | self.ANYW)

    def write(self, obj, off, val, f, at):
        val = norm(val)
        if not val and obj != ('U',): return
        if obj == ('U',):
            self.unk_stores[at] = f
            if not val <= self.ANYW:
                self.ANYW = self.ANYW | val; self.dirty |= self.any_readers
            return
        if obj[0] in ('C', 'XC', 'XV', 'X'): return
        if obj[0] in ('DA', 'EXP'): off = None
        if obj in (('EXT',), ('EXP',)) or obj in self.exposed: self.expose(val)
        cell = self.M[obj]; old = cell.get(off, E)
        if not val <= old:
            if obj == ('D',) and off not in cell and off is not None: bisect.insort(self.dkeys, off)
            cell[off] = old | val; self.dirty |= self.readers.get(obj, set())
            if obj[0] == 'DA': self.dirty |= self.readers.get(('D',), set())

    def clobbers(self):
        """registers each function may change: written and not saved in its first block, or changed by a callee"""
        cl = {}
        for f, (irs, succ, dep, ebp) in self.fns.items():
            saved = set(); a = f
            for a in sorted(irs):
                x = irs[a]
                if x[0] == 'PUSH' and x[1][0] == 'R': saved.add(x[1][1])
                elif x[0] not in ('MOV', 'ADDI', 'LEA', 'NOP', 'GEN', 'ST') or a - f > 40: break
            w = set()
            for x in irs.values():
                if x[0] in ('MOV', 'LEA', 'ADDI', 'POP') and isinstance(x[1], str): w.add(x[1])
                elif x[0] == 'GEN': w |= set(x[1])
                elif x[0] in ('XCHG',): w |= {x[1], x[2]}
                elif x[0] in ('LEAVE',): w.add('ebp')
                elif x[0] == 'STR': w |= {'esi', 'edi', 'ecx', 'eax'}
            cl[f] = (w - saved) | {'eax', 'ecx', 'edx'}
        changed = True
        while changed:
            changed = False
            for f, (irs, succ, dep, ebp) in self.fns.items():
                for x in irs.values():
                    if x[0] in ('CALL', 'JMP') and x[1][0] == 'I' and x[1][1] in cl:
                        add = cl[x[1][1]] - cl[f]
                        # a callee's change of a register this function saves is undone by the restore
                        if add and not add <= set(SAVED) - (cl[f] - {'eax', 'ecx', 'edx'}): pass
                        n = cl[f] | (cl[x[1][1]] - self._saved(f))
                        if n != cl[f]: cl[f] = n; changed = True
        return cl

    def data_refs(self):
        """every data address the program refers to: an operand constant or memory displacement in code, or a
        dword in the image, that lies in the image outside code -- the starts of the data objects (P6)"""
        lo, hi = self.img_rng; code = self.code_rng
        isdata = lambda v: lo <= v < hi and not any(x <= v < y for x, y in code)
        refs = {lo}
        def scan(part):
            if isinstance(part, tuple):
                if len(part) == 2 and part[0] == 'I' and isdata(part[1]): refs.add(part[1])
                elif len(part) == 5 and isinstance(part[2], int) and isdata(part[2] & 0xFFFFFFFF): refs.add(part[2] & 0xFFFFFFFF)
                else:
                    for q in part: scan(q)
            elif isinstance(part, list):
                for q in part: scan(q)
        for f, (irs, succ, dep, ebp) in self.fns.items():
            for x in irs.values(): scan(x[1:])
        img = bytes(self.im.img); b = self.im.base
        for k in range(0, len(img) - 3, 4):
            v = int.from_bytes(img[k:k + 4], 'little')
            if isdata(v): refs.add(v)
        return sorted(refs)

    def base_of(self, x):
        i = bisect.bisect_right(self.refs, x) - 1
        return self.refs[i] if i >= 0 else x

    def next_ref(self, a):
        i = bisect.bisect_right(self.refs, a)
        return self.refs[i] if i < len(self.refs) else self.img_rng[1]

    def static_range(self, a, b):
        if a not in self._sr:
            v = set()
            for x in range(a, b, 4): v |= self.static(x)
            self._sr[a] = frozenset(v)
        return self._sr[a]

    def address_taken(self):
        """functions whose address the program holds as a value (an operand that is not a direct call or
        jump target, or a dword in the image outside code): the only ones an indirect call can reach"""
        t = set()
        for f, (irs, succ, dep, ebp) in self.fns.items():
            for x in irs.values():
                if x[0] in ('CALL', 'JMP'): continue
                for part in x[1:]:
                    if isinstance(part, tuple) and len(part) == 2 and part[0] == 'I' and part[1] in self.fns: t.add(part[1])
                    if isinstance(part, list):
                        for q in part:
                            if isinstance(q, tuple) and len(q) == 2 and q[0] == 'I' and q[1] in self.fns: t.add(q[1])
        img = bytes(self.im.img); b = self.im.base
        for k in range(0, len(img) - 3, 4):
            a = b + k
            if any(x <= a < y for x, y in self.code_rng): continue
            v = int.from_bytes(img[k:k + 4], 'little')
            if v in self.fns: t.add(v)
        return t

    def _saved(self, f):
        if not hasattr(self, '_sv'): self._sv = {}
        if f not in self._sv:
            irs = self.fns[f][0]; s = set()
            for a in sorted(irs):
                x = irs[a]
                if x[0] == 'PUSH' and x[1][0] == 'R': s.add(x[1][1])
                elif x[0] not in ('MOV', 'ADDI', 'LEA', 'NOP', 'GEN', 'ST') or a - f > 40: break
            self._sv[f] = s & set(SAVED)
        return self._sv[f]

    def external(self, f):
        """f can be called by code outside the DLL: every register and argument may be external memory or any
        object the DLL has exposed (escape tracking)"""
        self.ext_funcs.add(f)
        ch = False
        for r in GPR:
            if not self.EXPV <= self.IN[f].get(r, E): self.IN[f][r] = self.IN[f].get(r, E) | self.EXPV; ch = True
        for k in range(NARGS): self.write(('F', f), 4 + 4 * k, self.EXPV, f, None)
        if ch: self.dirty.add(f)

    def objkey(self, o, off):
        """the object a target belongs to, for exposure: a datum is its data object (P6)"""
        if o == ('D',): return ('DA', self.base_of(off)) if off is not None else None
        if o[0] in ('C', 'XC', 'XV', 'X', 'XM', 'U', 'EXT', 'API', 'EXP'): return None
        return o

    def expose(self, vals):
        """objects that code outside the DLL can now reach"""
        for o, off in vals:
            k = self.objkey(o, off)
            if k is not None and k not in self.exposed: self.exposed.add(k); self.exposed_new = True

    def settle_exposure(self):
        """close the exposed set over what exposed objects hold, then let outside code act: it may write
        external or exposed values into every exposed object, pass them to every externally callable
        function, and return them from every unknown call (applied between rounds; the fixpoint repeats
        until nothing changes)"""
        for f in self.ext_funcs: self.expose(self.EXIT[f].get('eax', E))
        while self.exposed_new:
            self.exposed_new = False
            for k in list(self.exposed): self.expose(self.read(k, None, None))
        # ('EXP',) stands for every exposed object: it holds what any of them holds, and what is written through
        # it is in each of them (read() adds its cell to every exposed object)
        for k in list(self.exposed): self.write(('EXP',), None, self.read(k, None, None, raw=True), None, None)

    # one function
    def run(self, f):
        irs, succ, dep, _ = self.fns[f]
        fr = ('F', f)
        hub = self.HUB_IN if f in self.taken else {}
        st = {f: ({r: self.IN[f].get(r, E) | hub.get(r, E) for r in GPR}, {})}
        if f in self.taken: self.readers[('F', 'HUB')].add(f)
        work = [f]; escaped = False
        while work:
            a = work.pop(); regs, frame = st[a]; regs = dict(regs); frame = dict(frame); x = irs[a]
            d = dep.get(a)
            espv = frozenset([(fr, -d)]) if d is not None else frozenset([(fr, None)])

            def val(s):
                if s[0] == 'R': return espv if s[1] == 'esp' else (regs.get(s[1], E) if s[2] == 4 else E)
                if s[0] == 'I': return self.const(s[1])
                return load(s[1])

            def addr(mo):
                base, idx, disp, size, seg = mo
                if seg: return U1
                if base is None and idx is None: return self.const(disp) or frozenset([(('D',), disp)])
                bv = espv if base == 'esp' else (regs.get(base, E) if base else E)
                iv = regs.get(idx, E) if idx else E
                if base is None:                       # [index*scale + disp]: an array at a constant address
                    t = self.const(disp)
                    return frozenset(lose(o, off) for o, off in t) | frozenset(lose(o, off) for o, off in iv) | (U1 if not t else E)
                out = set()
                for o, off in bv | (iv if idx else E):
                    if o == ('U',): out.add(UNK)
                    elif idx or off is None: out.add(lose(o, off))
                    else: out.add((o, off + disp))
                return frozenset(out)

            def load(mo):
                if mo[3] != 4: return E
                v = E
                for o, off in addr(mo):
                    if o == fr and off is not None and off in frame and not escaped: v |= frame[off]
                    elif o == fr and off is not None and off in frame: v |= frame[off] | self.read(o, off, f)
                    else: v |= self.read(o, off, f)
                return v

            def store(mo, v, at, null=False):
                nonlocal escaped
                if (not v and not null) or mo[3] != 4:       # a value that is not a pointer (or part of one) stored
                    for o, off in addr(mo):
                        if o[0] in ('H', 'D', 'DA', 'EXT', 'API'): self.NPW.setdefault(o, set()).add(off if mo[3] == 4 or off is None else off - off % 4)
                if any(o[0] == 'F' and o[1] == f for o, _ in v): escaped = True
                tg = addr(mo)
                if len(tg) == 1:
                    o, off = next(iter(tg))
                    if o == fr and off is not None and mo[3] == 4: frame[off] = v
                    elif o == fr and off is not None:
                        for k in range(off - 3, off + mo[3]): frame.pop(k, None)
                for o, off in tg:
                    if o == fr and (off is None): frame.clear()
                    self.write(o, off, v if mo[3] == 4 else frozenset(lose(oo, x) for oo, x in v), f, at)

            def gen(srcs):
                v = set()
                for s in srcs:
                    for o, off in val(s): v.add(lose(o, off))
                return norm(frozenset(v))

            k = x[0]; nxt = succ.get(a)
            if k == 'MOV': regs[x[1]] = norm(val(x[2]))
            elif k == 'LEA':
                if x[2][1] is None: regs[x[1]] = addr(x[2])
                else: regs[x[1]] = frozenset(lose(o, off) for o, off in addr(x[2]))
            elif k == 'ADDI':
                regs[x[1]] = frozenset((o, off + x[2]) if off is not None and o != ('U',) else (UNK if o == ('U',) else (o, None)) for o, off in regs.get(x[1], E))
            elif k == 'ST': store(x[1], val(x[2]), a, null=(x[2] == ('I', 0)))
            elif k == 'PUSH':
                v = val(x[1])
                if d is not None: store((None, None, 0, 4, False), E, a) if False else None
                if any(o[0] == 'F' and o[1] == f for o, _ in v): escaped = True
                if d is not None: frame[-d - 4] = v; self.write(fr, -d - 4, v, f, a)
                else: self.write(fr, None, v, f, a)
            elif k == 'POP':
                v = (frame.get(-d, E) if (d is not None and -d in frame) else self.read(fr, -d if d is not None else None, f))
                if isinstance(x[1], str): regs[x[1]] = v
                else: store(x[1], v, a)
            elif k == 'LEAVE':
                v = E
                for o, off in regs.get('ebp', E):
                    v |= frame[off] if (o == fr and off in frame and not escaped) else self.read(o, off, f)
                regs['ebp'] = v
            elif k == 'XCHG': regs[x[1]], regs[x[2]] = regs.get(x[2], E), regs.get(x[1], E)
            elif k == 'GEN':
                v = gen(x[2])
                for r in x[1]: regs[r] = v
                for mo in x[3]: store(mo, v | frozenset(lose(o, off) for o, off in load(mo)), a)
            elif k == 'STR':
                m = x[1]; src = regs.get('esi', E); dst = regs.get('edi', E)
                if 'movs' in m:
                    v = E
                    for o, off in src: v |= self.read(*lose(o, off), f)
                    for o, off in dst: self.write(*lose(o, off), v, f, a)
                    if any(o == fr for o, _ in dst): frame.clear()
                elif 'stos' in m:
                    v = regs.get('eax', E) if m.endswith('d') or m.endswith('stosd') else E
                    for o, off in dst: self.write(*lose(o, off), v, f, a)
                    if any(o == fr for o, _ in dst): frame.clear()
                for r in ('esi', 'edi'): regs[r] = frozenset(lose(o, off) for o, off in regs.get(r, E))
                regs['ecx'] = E
                if 'lods' in m: regs['eax'] = gen([('M', ('esi', None, 0, 4, False))])
            elif k in ('CALL', 'JMP'):
                tail = k == 'JMP'
                if tail and nxt is not None and (x[1][0] != 'I' or x[1][1] in irs):
                    pass                                                   # a jump inside the function
                else:
                    if tail and x[1][0] == 'M' and x[1][1][1] is not None and nxt is None and x[1][1][0] is None:
                        self.unfollowed.add(f)
                    self.call(f, a, x[1], regs, frame, d, espv, val, load, tail, escaped)
                    if tail: nxt = []
            elif k == 'RET':
                ex = self.EXIT[f]
                for r in GPR:
                    v = regs.get(r, E)
                    if not v <= ex.get(r, E):
                        ex[r] = ex.get(r, E) | v
                        for c in self.callers[f]: self.dirty.add(c)
                        if f in self.taken and r in self.clob.get(f, ()) and not v <= self.HUB_EXIT.get(r, E):
                            self.HUB_EXIT[r] = self.HUB_EXIT.get(r, E) | v; self.dirty |= self.unk_callers
                nxt = []
            if nxt is None:
                self.unfollowed.add(f); nxt = []
            for n in nxt:
                if n not in irs: continue
                if n not in st: st[n] = (regs, frame); work.append(n)
                else:
                    orr, ofr = st[n]
                    nr = {r: widen(orr.get(r, E) | regs.get(r, E)) for r in set(orr) | set(regs)}
                    nf = {o: widen(ofr[o] | frame[o]) for o in ofr if o in frame}
                    if nr != orr or nf != ofr: st[n] = (nr, nf); work.append(n)
        if escaped:
            pass

    def call(self, f, a, tgt, regs, frame, d, espv, val, load, tail, escaped):
        fr = ('F', f)
        base = (-d + (4 if tail else 0)) if d is not None else None
        def arg(k):
            if base is None: return self.read(fr, None, f)
            off = base + 4 * k
            return frame[off] if off in frame and not escaped else self.read(fr, off, f) | frame.get(off, E)
        callees, unknown, d3d, imp = set(), False, [], None
        if tgt[0] == 'I' and tgt[1] in self.fns: callees.add(tgt[1])
        elif tgt[0] == 'M' and tgt[1][0] is None and tgt[1][1] is None and tgt[1][2] in self.iat: imp = self.iat[tgt[1][2]]
        else:
            for o, off in (val(tgt) if tgt[0] != 'I' else self.const(tgt[1])):
                if o == ('C',) and off in self.fns: callees.add(off)
                elif o[0] == 'XC': d3d.append((o[1], off))
                else: unknown = True
        if tgt[0] == 'I' and tgt[1] not in self.fns: unknown = True
        args = [arg(k) for k in range(NARGS)]
        res = {r: E for r in ('eax', 'ecx', 'edx')}; changed = set(('eax', 'ecx', 'edx'))
        for g in callees:
            self.callers[g].add(f)
            for r in GPR:
                v = regs.get(r, E)
                if not v <= self.IN[g].get(r, E): self.IN[g][r] = self.IN[g].get(r, E) | v; self.dirty.add(g)
            for k in range(NARGS): self.write(('F', g), 4 + 4 * k, args[k], f, None)
            for r in self.clob.get(g, set(GPR)):
                res[r] = res.get(r, E) | self.EXIT[g].get(r, E); changed.add(r)
            if g in self.allocs:                                      # P2
                res['eax'] = frozenset([(('H', a), 0)])
        for iface, slot in d3d:
            meth = self.spec[iface][slot] if slot is not None and slot < len(self.spec[iface]) else None
            if meth is None: unknown = True; continue
            for k in range(1, meth['nparams'] + 1):
                out = meth.get('out')
                v = frozenset([(('X', out[1]), 0)]) if out and out[0] + 1 == k else frozenset([(('XM',), None)])
                for o, off in args[k]:
                    if o[0] in ('X', 'XM', 'C'): continue
                    if out and out[0] + 1 == k: self.write(o, off, v, f, a)
                    else: self.write(*lose(o, off), v, f, a)
        if imp is not None:
            if imp in ALLOC_API: res['eax'] = frozenset([(('H', a), 0)])
            else:
                res['eax'] = frozenset([(('API',), None)])
                self.havoc_args(f, a, args, regs, deep=False)
        if unknown:
            # an unknown target: a function at that slot of some table (P8) or address-taken, or code outside
            # the DLL (P7: it returns and writes external memory)
            self.unk_calls[a] = f; self.unk_callers.add(f)
            # one shared hub instead of an edge to each address-taken function: what any unknown call passes,
            # every one of them receives; what any of them returns, every unknown call gets back
            for r in GPR:
                v = regs.get(r, E)
                if not v <= self.HUB_IN.get(r, E):
                    self.HUB_IN[r] = self.HUB_IN.get(r, E) | v; self.dirty |= self.taken
            for k in range(NARGS): self.write(('F', 'HUB'), 4 + 4 * k, args[k], f, None)
            for r in self.HUB_CLOB:
                res[r] = res.get(r, E) | self.HUB_EXIT.get(r, E); changed.add(r)
            res['eax'] = res['eax'] | self.EXPV; res['edx'] = res['edx'] | self.EXPV
            self.havoc_args(f, a, args, regs, deep=True)
        for r in changed: regs[r] = res.get(r, E)
        if base is not None and not tail:
            for k in range(NARGS): frame.pop(base + 4 * k, None)
        if tail:
            ex = self.EXIT[f]
            for r in GPR:
                v = regs.get(r, E)
                if not v <= ex.get(r, E):
                    ex[r] = ex.get(r, E) | v
                    for c in self.callers[f]: self.dirty.add(c)

    def havoc_args(self, f, a, args, regs, deep):
        """an import or unknown callee: UNK (unknown callee) or API data (import) through every pointer it is
        given, one level; a function whose address it is given becomes externally callable"""
        v = self.EXPV if deep else frozenset([(('API',), None)])
        given = set().union(*args) | regs.get('ecx', E) | regs.get('edx', E)
        self.expose(given)
        for o, off in given:
            if o == ('C',) and off in self.fns: self.external(off); continue
            if o[0] in ('C', 'XC', 'XV', 'X', 'U', 'EXT', 'EXP'): continue
            self.write(*lose(o, off), v, f, a)
            for o2, off2 in self.read(*lose(o, off), f) if deep else ():
                if o2 == ('C',) and off2 in self.fns: self.external(off2)

    def solve(self, max_rounds=10 ** 9, log=print):
        n = 0; t0 = time.time()
        while self.dirty and n < max_rounds:
            batch = sorted(self.dirty); self.dirty = set()
            for f in batch: self.run(f)
            self.settle_exposure()
            n += 1
            log('round %d: %d functions run, %d dirty, %d UNK stores, %d unknown calls, ANYW %d, exposed %d, %.0fs' % (
                n, len(batch), len(self.dirty), len(self.unk_stores), len(self.unk_calls), len(self.ANYW), len(self.exposed), time.time() - t0))
        return n


def main(out, max_rounds):
    path = os.environ['FFXI_DLL']; im = S.Image(path)
    ext = {int(k, 16): v for k, v in json.load(open(os.path.join(D, 'func_extents.json'))).items()}
    fns = build(im, ext, os.path.join(R, 'points_to_ir.pkl'))
    print('%d functions, %d instructions' % (len(fns), sum(len(v[0]) for v in fns.values())))
    pt = PT(im, path, ext, fns)
    pt.solve(max_rounds)
    res = {'_doc': __doc__.split('\n\n')[0], 'converged': not pt.dirty,
           'unk_stores': len(pt.unk_stores), 'unk_calls': len(pt.unk_calls), 'unfollowed': len(pt.unfollowed),
           'anywhere': sorted(map(str, pt.ANYW))[:50],
           'unk_store_funcs': collections.Counter('0x%08X' % f for f in pt.unk_stores.values()).most_common(40),
           'unk_call_funcs': collections.Counter('0x%08X' % f for f in pt.unk_calls.values()).most_common(40)}
    json.dump(res, open(out, 'w'), indent=1)
    pickle.dump({'M': dict(pt.M), 'NPW': pt.NPW, 'ANYW': pt.ANYW, 'IN': dict(pt.IN), 'refs': pt.refs},
                open(os.path.join(R, 'points_to_M.pkl'), 'wb'))


if __name__ == '__main__':
    mr = int(sys.argv[sys.argv.index('--max-rounds') + 1]) if '--max-rounds' in sys.argv else 10 ** 9
    main(sys.argv[1], mr)
