"""com_calls.py -- Direct3D 8 method calls whose object is proven, and the pops the D3D8 header gives them.

A COM call `push obj; mov r, [obj]; call [r + 4*slot]` pops 4 * (1 + the method's parameter count): the
D3D8 SDK header (data/com_spec.json) fixes that once the object's interface is known. The interface is known
only where the object's value is a proven field: data/com_fields.json lists access paths (a global, then
dereferences at offsets) with the interface they hold and the proof. The proof's checkable parts are
re-checked here against the binary every run -- which instructions write the global or the offset, which
code refers to the function that fills it -- and an entry whose check fails is dropped, not used.

Each call's object is traced by a forward data-flow over the function (each register: an access path or
unknown; where paths join, a register keeps a path only if every incoming path has the same one; a call
leaves EAX/ECX/EDX unknown). A switch through a jump table is followed only where the code in front of it bounds the
index (sp_solve.switch_targets: one- and two-level MSVC tables); a function with any other indirect jump
is not traced.

    python3 com_calls.py data/com_calls.json        uses $FFXI_DLL
Output: {"0xCALL": {"func", "path", "iface", "slot", "method", "pops", "field"}}
"""
import json, os, re, sys, collections
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
R32 = {'al': 'eax', 'ah': 'eax', 'ax': 'eax', 'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
       'dl': 'edx', 'dh': 'edx', 'dx': 'edx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}
r32 = lambda n: R32.get(n, n)
GPR = ('eax', 'ebx', 'ecx', 'edx', 'esi', 'edi', 'ebp')


def written(i):
    """the general registers an instruction writes (conservative: any doubt counts as a write)"""
    m = i.mnemonic; out = set()
    if not (m in ('cmp', 'test', 'push', 'jmp') or m.startswith(('j', 'ret'))):
        if i.operands and i.operands[0].type == X86_OP_REG: out.add(r32(i.reg_name(i.operands[0].reg)))
    if m in ('xchg', 'xadd', 'cmpxchg') and len(i.operands) > 1 and i.operands[1].type == X86_OP_REG:
        out.add(r32(i.reg_name(i.operands[1].reg)))
    if m == 'cmpxchg': out.add('eax')
    if m in ('cdq', 'mul', 'imul', 'div', 'idiv', 'cwde', 'cbw', 'lahf', 'xlatb') or m.startswith(('rep', 'movs', 'stos', 'lods', 'scas', 'cmps')):
        out |= {'eax', 'edx', 'ecx', 'esi', 'edi'}
    if m == 'call': out |= {'eax', 'ecx', 'edx'}
    if m in ('pushal', 'popal', 'leave', 'cpuid', 'rdtsc', 'enter'): out |= set(GPR)
    if m.startswith(('loop',)): out.add('ecx')
    return out & set(GPR)


def successors(im, ins, a, i, labels=frozenset()):
    """[next addresses] in the function; None when control goes somewhere this cannot follow"""
    m = i.mnemonic; ops = i.operands
    if m.startswith('ret') or m in ('int3', 'hlt', 'ud2'): return []
    if m == 'jmp':
        if ops[0].type == X86_OP_IMM: return [ops[0].imm] if ops[0].imm in ins else []      # tail jump: leaves
        if ops[0].type == X86_OP_MEM and ops[0].mem.index and not ops[0].mem.base and ops[0].mem.scale == 4:
            prev = [ins[p] for p in sorted(x for x in ins if x < a)[-5:]]
            while prev and prev[-1].address + prev[-1].size != a: prev = []          # not straight-line: no bound
            t = S.switch_targets(im, prev, i, labels)
            return [x for x in t if x in ins] if t is not None else None
        return None
    if m.startswith(('j', 'loop')): return [a + i.size] + ([ops[0].imm] if ops[0].type == X86_OP_IMM else [])
    return [a + i.size]


def flow(im, ins, ea, entry=None):
    """{address: {reg: path}} before each instruction; None if the function cannot be followed. `entry`: the
    registers' paths at the function's entry (none known by default)"""
    state = {ea: dict(entry or {})}; work = [ea]; labels = S.labels_of(ins)
    while work:
        a = work.pop(); i = ins[a]; st = dict(state[a]); ops = i.operands; m = i.mnemonic
        new = dict(st)
        for r in written(i): new.pop(r, None)
        if m == 'mov' and len(ops) == 2 and ops[0].type == X86_OP_REG and ops[0].size == 4:
            d = r32(i.reg_name(ops[0].reg)); s = ops[1]
            if s.type == X86_OP_MEM and s.size == 4 and not s.mem.index and s.mem.segment == 0:
                if not s.mem.base: new[d] = ('G', s.mem.disp)
                elif r32(i.reg_name(s.mem.base)) in st: new[d] = st[r32(i.reg_name(s.mem.base))] + (s.mem.disp,)
            elif s.type == X86_OP_REG and r32(i.reg_name(s.reg)) in st and s.size == 4: new[d] = st[r32(i.reg_name(s.reg))]
        nxt = successors(im, ins, a, i, labels)
        if nxt is None: return None
        for n in nxt:
            if n not in ins: continue
            if n not in state: state[n] = new; work.append(n)
            else:
                mm = {r: p for r, p in state[n].items() if new.get(r) == p}
                if mm != state[n]: state[n] = mm; work.append(n)
    return state


def writers_global(im, code, g):
    """every instruction that writes the dword at g, or takes its address (the address may be written through)"""
    out = []
    for f, ins in code.items():
        for a, i in ins.items():
            ops = i.operands
            for k, o in enumerate(ops):
                if o.type == X86_OP_MEM and not o.mem.base and not o.mem.index and o.mem.disp == g:
                    if i.mnemonic == 'lea' or (k == 0 and i.mnemonic not in ('cmp', 'test', 'push')): out.append(a)
            if i.mnemonic in ('push', 'mov') and any(o.type == X86_OP_IMM and o.imm == g for o in ops): out.append(a)
    return sorted(set(out))


def writers_disp(code, disp):
    """every instruction that writes [reg + disp] or takes its address, in any function"""
    out = []
    for f, ins in code.items():
        for a, i in ins.items():
            ops = i.operands
            for k, o in enumerate(ops):
                if o.type == X86_OP_MEM and o.mem.base and o.mem.disp == disp and r32(i.reg_name(o.mem.base)) != 'esp':
                    if i.mnemonic == 'lea' or (k == 0 and i.mnemonic not in ('cmp', 'test', 'push')): out.append(a)
    return sorted(set(out))


def refs_to(im, code, target):
    """every reference to a function: direct calls/jumps, and its address as a dword anywhere in the image"""
    out = []
    for f, ins in code.items():
        for a, i in ins.items():
            if any(o.type == X86_OP_IMM and o.imm == target for o in i.operands): out.append(a)
    data = bytes(im.img); pat = target.to_bytes(4, 'little'); k = data.find(pat)
    while k >= 0:
        a = im.base + k
        if not any(x <= a < x + 8 for x in out): out.append(a)
        k = data.find(pat, k + 1)
    return sorted(set(out))


def check(field, im, code):
    """the field's checkable claims, each recomputed; [] if all hold, else what differs"""
    bad = []
    for c in field.get('checks', []):
        if c['kind'] == 'writers_global': got = writers_global(im, code, int(c['global'], 16))
        elif c['kind'] == 'writers_disp': got = writers_disp(code, int(c['disp'], 16))
        elif c['kind'] == 'refs_to': got = refs_to(im, code, int(c['function'], 16))
        else: bad.append('unknown check %s' % c['kind']); continue
        want = sorted(int(x, 16) for x in c['expect'])
        if got != want: bad.append('%s %s: expected %s, found %s' % (c['kind'], c.get('global') or c.get('disp') or c.get('function'),
                                                                        c['expect'], ['0x%08X' % x for x in got]))
    return bad


def main(out):
    im = S.Image(os.environ['FFXI_DLL'])
    ext = json.load(open(os.path.join(D, 'func_extents.json')))
    spec = json.load(open(os.path.join(D, 'com_spec.json')))
    fields = json.load(open(os.path.join(D, 'com_fields.json')))
    code = {int(f, 16): im.code(e) for f, e in ext.items()}
    proven = {}
    for name, fd in fields.items():
        if name.startswith('_'): continue
        bad = check(fd, im, code)
        if bad: print('com_fields %s NOT used: %s' % (name, '; '.join(bad))); continue
        if fd['path'][0] == 'this': continue                # an accepted declaration on a class's `this`: below
        proven[tuple(['G', int(fd['path'][0], 16)] + [int(x, 16) for x in fd['path'][1:]])] = (name, fd['iface'])
    # ACCEPTED (not proven) entries: path ["this", off...] holds `iface` in every function whose `this` the IDB
    # declares as the entry's class (`this_class`), or whose IDB name has the class prefix; such calls carry
    # "accepted": the entry's status text, so every use shows the uncertainty
    exp = os.environ.get('GFX_EXPORT', os.path.join(D, 'exports', 'gfx_export.jsonl'))
    this_fields = {}
    for name, fd in fields.items():
        if name.startswith('_') or fd['path'][0] != 'this': continue
        cls = fd['this_class']
        for l in open(exp):
            r = json.loads(l); ty = r.get('type', '')
            if r['name'].startswith(cls + '_') or re.search(r'__thiscall[^(]*\(%s \*' % cls, ty):
                this_fields.setdefault(r['ea'], {})[tuple(['this'] + [int(x, 16) for x in fd['path'][1:]])] = (name, fd['iface'], fd['status'])
    res = {}; stats = collections.Counter()
    for f, ins in code.items():
        calls = [a for a, i in ins.items() if i.mnemonic == 'call' and i.operands[0].type == X86_OP_MEM and i.operands[0].mem.base]
        if not calls or f not in ins: continue
        tf = this_fields.get(f, {})
        st = flow(im, ins, f, {'ecx': ('this',)} if tf else None)
        if st is None: stats['function not followed (indirect jump)'] += len(calls); continue
        for a in calls:
            i = ins[a]; o = i.operands[0]
            if a not in st: stats['not reached'] += 1; continue
            s = st[a]; vreg = r32(i.reg_name(o.mem.base))
            vt = s.get(vreg)
            # the vtable register holds [obj] (one dereference at 0 of the object's path), and the object's
            # path is what was pushed last: the instruction before the call (after the vtable load) pushes it
            if not vt or vt[-1] != 0: stats['vtable not traced'] += 1; continue
            obj = vt[:-1]
            if obj not in proven and obj not in tf: stats['object not a proven field'] += 1; continue
            prev = [x for x in sorted(ins) if x < a]
            pushes = [ins[x] for x in prev[-4:] if ins[x].mnemonic == 'push']
            if not pushes or pushes[-1].operands[0].type != X86_OP_REG or st[pushes[-1].address].get(r32(pushes[-1].reg_name(pushes[-1].operands[0].reg))) != obj:
                stats['object not pushed as this'] += 1; continue
            name, iface = proven[obj] if obj in proven else tf[obj][:2]; slot = o.mem.disp // 4
            if o.mem.disp % 4 or slot >= len(spec[iface]): stats['slot outside the interface'] += 1; continue
            meth = spec[iface][slot]
            res['0x%08X' % a] = {'func': '0x%08X' % f, 'path': '[' * (len(obj) - 1) + '0x%08X' % obj[1] + ''.join(']+0x%X' % x for x in obj[2:]) + ']',
                                 'iface': iface, 'slot': slot, 'method': meth['name'], 'pops': 4 * (1 + meth['nparams']), 'field': name}
            if obj in tf: res['0x%08X' % a]['accepted'] = tf[obj][2]; stats['accepted declaration'] += 1; continue
            stats['proven'] += 1
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d COM calls proven: %s' % (len(res), dict(stats)))


if __name__ == '__main__':
    main(sys.argv[1])
