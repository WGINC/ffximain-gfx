"""callsite_audit.py -- every indirect call in the exported functions: what each definitive source says it pops,
where sources disagree, and which calls no source covers.

Sources (none of them a guess):
  solved   data/sp_solved.json   the caller's own stack balance (sp_solve.py, self-tested)
  measured data/real_sp.json     running real game code (real_sp.py, trusted calls only)
  vtable   data/vcalls.json      every possible callee proven by vtable (vcall_resolve.py)
  typed    the export's calls    a call type declared in the IDB (`"src": "typed"`, e.g. lpVtbl->Method from the
                                 D3D8 header); `"cast"` types Hex-Rays made up are reported, never counted
For a call no source covers, the code's own evidence about its convention is recorded as evidence only:
`object pushed last` (COM style: `this` on the stack) and `ECX = object`.

    python3 callsite_audit.py EXPORT.jsonl data/callsite_audit.json        uses $FFXI_DLL
"""
import json, os, sys, collections
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import sp_solve as S

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
ld = lambda f: json.load(open(os.path.join(D, f))) if os.path.exists(os.path.join(D, f)) else {}


def evidence(ins, addrs, idx, call):
    """the convention evidence in the straight-line code before the call (no branch, call or label crossed)"""
    op = call.operands[0]; R = call.reg_name(op.mem.base) if op.type == X86_OP_MEM and op.mem.base else None
    obj = None; last_push = None; ecx = None
    for b in reversed(addrs[max(0, idx - 24):idx]):
        j = ins[b]
        if j.mnemonic in ('call', 'ret') or j.mnemonic.startswith('j'): break
        o = j.operands
        if last_push is None and j.mnemonic == 'push': last_push = j.op_str
        if ecx is None and o and o[0].type == X86_OP_REG and j.reg_name(o[0].reg) == 'ecx' and j.mnemonic not in ('cmp', 'test', 'push'):
            ecx = j.op_str.split(',', 1)[1].strip() if j.mnemonic == 'mov' and ',' in j.op_str else '?'
        if obj is None and R and j.mnemonic == 'mov' and len(o) == 2 and o[0].type == X86_OP_REG and j.reg_name(o[0].reg) == R \
                and o[1].type == X86_OP_MEM and o[1].mem.disp == 0 and o[1].mem.base and not o[1].mem.index:
            obj = j.reg_name(o[1].mem.base)
    ev = []
    if obj and last_push == obj: ev.append('object pushed last')
    if obj and (obj == 'ecx' or ecx == obj): ev.append('ECX = object')
    return ev or ['none']


def main(exp, out):
    im = S.Image(os.environ['FFXI_DLL']); ext = ld('func_extents.json')
    solved = {k: v['pops'] for k, v in ld('sp_solved.json').items() if not k.startswith('_')}
    measured = {c: p for r in ld('real_sp.json').values() for c, p in r.get('calls', {}).items()}
    vtable = {k: v['pops'] for k, v in ld('vcalls.json').items() if not k.startswith('_')}
    typed, cast, funcs = {}, {}, []
    for l in open(exp):
        r = json.loads(l); funcs.append(r['ea'])
        for k, v in (r.get('calls') or {}).items():
            n, cc = v[0], v[1]; src = v[2] if len(v) > 2 else 'unknown'
            stack = max(0, n - {'thiscall': 1, 'fastcall': 2}.get(cc, 0))
            pops = 0 if cc == 'cdecl' else 4 * stack
            (typed if src == 'typed' else cast)[k] = pops
    have_src = any(len(v) > 2 for l in open(exp) for v in (json.loads(l).get('calls') or {}).values())
    res = {}; stats = collections.Counter(); conflicts = []
    for f in funcs:
        e = ext.get('0x%x' % f)
        if not e: continue
        ins = im.code(e); addrs = sorted(ins)
        for idx, a in enumerate(addrs):
            i = ins[a]
            if i.mnemonic != 'call' or i.operands[0].type == X86_OP_IMM: continue
            k = '0x%08X' % a
            srcs = {n: d[k] for n, d in (('solved', solved), ('measured', measured), ('vtable', vtable), ('typed', typed)) if k in d}
            rec = {'func': '0x%08X' % f, 'sources': srcs}
            if k in cast: rec['hexrays_cast'] = cast[k]
            if len(set(srcs.values())) > 1: rec['conflict'] = True; conflicts.append(k); stats['conflict'] += 1
            elif srcs:
                stats['definitive'] += 1
                if k in cast and cast[k] != next(iter(srcs.values())): rec['hexrays_wrong'] = True; stats['hexrays cast differs'] += 1
            else:
                rec['evidence'] = evidence(ins, addrs, idx, i); stats['no source'] += 1
            res[k] = rec
    res['_stats'] = dict(stats); res['_conflicts'] = conflicts; res['_export_has_call_sources'] = have_src
    json.dump(res, open(out, 'w'), indent=0, sort_keys=True)
    print('%d indirect calls: %s%s' % (sum(1 for k in res if not k.startswith('_')), dict(stats),
          '' if have_src else '  (this export has no call-type sources: re-export with ida/ffxi_ida_fix.py for the typed ones)'))
    for k in conflicts[:20]: print('  CONFLICT %s: %s' % (k, res[k]['sources']))


if __name__ == '__main__':
    main(*sys.argv[1:3])
