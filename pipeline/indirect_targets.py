"""indirect_targets.py -- where each indirect call and jump in the game's code goes, from every source that says so.

A call or jump through a register or memory names no target in its bytes. Each site is listed with its targets and
the source of each, kept apart:

  static    the code proves the complete set: vcall_resolve.py's virtual calls on `this` (data/vcalls.json, every
            vtable the object can have there, each slot's callee); also its calls whose callees it settles but whose
            pop counts disagree or are unknown (the targets are settled, the pops are not, so vcalls.json leaves them
            out; `pops_settled` false). Re-derived here (vcall_resolve.analyse, resolve) and checked against
            vcalls.json: a difference stops the run (the file is out of date). No indirect jump gets a static set:
            vcall_resolve follows no function with one (com_calls.successors cannot follow it), so its tail jumps
            through a vtable are not decided.
  com       the object is a proven Direct3D 8 interface (data/com_calls.json, com_calls.py): the target is that
            interface's method, in the Direct3D implementation, not game code. A call resting on the accepted
            declaration CDx+0xC (com_fields.json device_via_CDx_this, owner's decision 4 Oct, not proven) is
            `com_accepted`, with the declaration's own text.
  recorded  the running game called these (xikit facts=1, data/game_sp.json `targets`; data/game_switch.json for
            jumps). The kit reports every game address at the analysed build's address (kit/xikit/xikit.c
            analysed_of, from the manifest's !shift ranges), so recordings taken on build 6AC37846 are already in
            the analysed build's terms here. OBSERVED, NOT PROVEN COMPLETE: a target not seen was not reached in
            the sessions, which does not show it unreachable. A `module:` or `memory:` target is where the call
            landed outside FFXiMain.dll; a third-party hook (Hook.dll) in the place of the device or a game
            object shows as that hook, not as what the game's code would call without it.
Not listed: a call or jump through an import slot (`[abs]` in the PE import table: the loader fills it with the
named import; counted in _summary), and a switch jump whose table and bound the code gives (com_calls.successors,
sp_solve.switch_targets: the same rule the flows here use; it stays in its function).

Every other site with no static or com source is `unresolved` (with recorded targets: `recorded`), and says why
(`reason`, texts in _reasons): vcall_resolve's own reason for not settling it, or the operand's kind where no tool
here decides that kind. `not_recorded` (texts in _not_recorded) says whether the site was asked for in a facts=1 session
(data/facts_wanted.json) and never observed, or never asked for.

    FFXI_DLL=... python3 indirect_targets.py data/indirect_targets.json
Output {"_doc", "_sources", "_reasons", "_summary",
        "sites": {"0xSITE": {"func", "insn", "kind": "call" | "jmp", "status", "targets": {target: [sources]},
                             "static"?: {"targets", "exact", "pops_settled"}, "com"?: {"iface", "method", "slot"},
                             "recorded"?: {"targets", "calls"}, "reason"?, "not_recorded"?, "conflict"?, "not_entry"?}}}
`targets` keys: a game function ("0x1000ABCD"), a Direct3D method ("IDirect3DDevice8::SetTexture"), or a recorded
"module:NAME+0xOFF" / "memory:0xADDR". `conflict`: recorded game targets outside a static set (would show the
proof wrong; the run reports them). `not_entry`: a recorded game target that is no function start in
data/func_extents.json."""
import collections, json, os, sys
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG
P = os.path.dirname(os.path.abspath(__file__)); sys.path.insert(0, P)
import sp_solve as S
import vcall_resolve as V
import com_calls as C

D = os.path.join(P, '..', 'data')
ld = lambda f: json.load(open(os.path.join(D, f)))

SOURCES = {
    'static': 'vcall_resolve.py (data/vcalls.json): every vtable the object can have at the call, the callee in that '
              'slot of each; the complete set',
    'com': 'com_calls.py (data/com_calls.json): a method of a proven Direct3D 8 interface (d3d8.h, data/com_spec.json)',
    'com_accepted': 'com_calls.py on the accepted declaration CDx+0xC (data/com_fields.json device_via_CDx_this): '
                    'ACCEPTED, NOT PROVEN',
    'recorded': 'the running game (xikit facts=1, data/game_sp.json / game_switch.json), at analysed addresses '
                '(xikit.c analysed_of); observed, not proven complete',
}
NOT_RECORDED = {'asked': 'listed for measuring (data/facts_wanted.json), no observation in data/game_sp.json / game_switch.json',
                'never_asked': 'never listed for a facts=1 session'}
REASONS = {
    'vc_flow': 'vcall_resolve: ' + V.NOT_THIS['flow'],
    'vc_unreached': 'vcall_resolve: ' + V.NOT_THIS['unreached'],
    'vc_base': 'vcall_resolve: ' + V.NOT_THIS['base'] + '; com_calls.py: not a proven Direct3D 8 object either',
    'vc_top': 'vcall_resolve: this may be any object (the method\'s address is taken, or a caller passes another object)',
    'vc_none': 'vcall_resolve: method reached by no vtable and no caller',
    'vc_past': 'vcall_resolve: slot past a vtable end',
    'reg': 'call through a register: no tool here settles the register\'s value',
    'indexed': 'call through an indexed operand ([reg + reg*n + disp]): no tool here settles it',
    'data_ptr': 'call through a function pointer in writable data (.data): no tool here settles its value at the call',
    'ro_ptr': 'call through a pointer in a read-only section, outside the import table: not settled here (the '
              'section flags are not taken as proof that nothing writes it)',
    'jmp_switch': 'switch jump whose index the code before it does not bound (sp_solve.switch_targets)',
    'jmp_other': 'indirect jump (not a switch through a table): no tool here settles it; it also stops vcall_resolve\'s '
                 'flow for its whole function (vc_flow at that function\'s calls)',
}
VC_KEY = {V.NOT_THIS['flow']: 'vc_flow', V.NOT_THIS['unreached']: 'vc_unreached', V.NOT_THIS['base']: 'vc_base',
          'this may be any object': 'vc_top', 'method reached by no vtable and no caller': 'vc_none',
          'slot past a vtable end': 'vc_past'}


def main(out):
    dll = os.environ['FFXI_DLL']
    A = V.analyse()
    im, code, starts = A['im'], A['code'], A['starts']
    iat = S.imports(im, dll)
    keep = {}
    res, _ = V.resolve(A, keep)
    vc = {k: v for k, v in ld('vcalls.json').items() if not k.startswith('_')}
    if {k: v['targets'] for k, v in res.items()} != {k: v['targets'] for k, v in vc.items()}:
        sys.exit('data/vcalls.json differs from vcall_resolve.py run now: regenerate it first')
    com = ld('com_calls.json'); gsp = ld('game_sp.json'); gsw = ld('game_switch.json'); wanted = ld('facts_wanted.json')
    import pefile
    pe = pefile.PE(dll, fast_load=True)
    writable = [(im.base + s.VirtualAddress, im.base + s.VirtualAddress + s.Misc_VirtualSize, s.Name.rstrip(b'\0').decode())
                for s in pe.sections if s.Characteristics & 0x80000000]
    in_writable = lambda a: any(lo <= a < hi for lo, hi, _ in writable)

    sites = {}; n_import = 0; n_switch_bounded = 0
    for f, ins in sorted(code.items()):
        order = sorted(ins); labels = None
        for n, a in enumerate(order):
            i = ins[a]
            if i.mnemonic not in ('call', 'jmp') or not i.operands or i.operands[0].type == X86_OP_IMM: continue
            o = i.operands[0]; k = '0x%08X' % a
            absolute = o.type == X86_OP_MEM and not o.mem.base and not o.mem.index
            if absolute and (o.mem.disp & 0xFFFFFFFF) in iat: n_import += 1; continue
            e = {'func': '0x%08X' % f, 'insn': '%s %s' % (i.mnemonic, i.op_str), 'kind': i.mnemonic}
            tg = collections.defaultdict(list)
            if i.mnemonic == 'jmp':
                labels = labels or S.labels_of(ins)
                if C.successors(im, ins, a, i, labels) is not None: n_switch_bounded += 1; continue
                switch = o.type == X86_OP_MEM and o.mem.index and o.mem.scale == 4 and not o.mem.base
                e['reason'] = 'jmp_switch' if switch else 'jmp_other'
                r = gsw.get(k)
                if r:
                    e['recorded'] = {'targets': sorted(t[5:] if t.startswith('game:') else t for t in r['targets_seen']),
                                     'calls': sum(r['targets_seen'].values())}
            else:
                why, cand = keep.get(a, (None, None))
                if k in res: e['static'] = {'targets': res[k]['targets'], 'exact': res[k]['exact'], 'pops_settled': True}
                elif cand: e['static'] = {'targets': cand, 'exact': False, 'pops_settled': False}
                elif k in com:
                    c = com[k]; e['com'] = {'iface': c['iface'], 'method': c['method'], 'slot': c['slot']}
                    if c.get('accepted'): e['com']['accepted'] = c.get('field')
                elif absolute: e['reason'] = 'data_ptr' if in_writable(o.mem.disp & 0xFFFFFFFF) else 'ro_ptr'
                elif o.type == X86_OP_REG: e['reason'] = 'reg'
                elif o.type == X86_OP_MEM and o.mem.index: e['reason'] = 'indexed'
                else: e['reason'] = VC_KEY[why]
                r = gsp.get(k)
                if r and r.get('targets'):
                    e['recorded'] = {'targets': sorted(t[5:] if t.startswith('game:') else t for t in r['targets']),
                                     'calls': r['observed_calls']}
            for t in (e.get('static') or {}).get('targets', ()): tg[t].append('static')
            if 'com' in e: tg['%s::%s' % (e['com']['iface'], e['com']['method'])].append('com_accepted' if 'accepted' in e['com'] else 'com')
            for t in (e.get('recorded') or {}).get('targets', ()): tg[t].append('recorded')
            e['targets'] = dict(sorted(tg.items()))
            game_rec = [t for t in (e.get('recorded') or {}).get('targets', ()) if t.startswith('0x')]
            if 'static' in e and set(game_rec) - set(e['static']['targets']):
                e['conflict'] = sorted(set(game_rec) - set(e['static']['targets']))
            if 'com' in e and game_rec: e['conflict'] = game_rec
            bad = [t for t in game_rec if int(t, 16) not in starts and i.mnemonic == 'call']
            if bad: e['not_entry'] = bad
            e['status'] = 'static' if 'static' in e else ('com_accepted' if 'accepted' in e['com'] else 'com') if 'com' in e \
                else 'recorded' if 'recorded' in e else 'unresolved'
            if e['status'] in ('static', 'com', 'com_accepted'): e.pop('reason', None)
            if 'recorded' not in e:
                e['not_recorded'] = 'asked' if k in wanted else 'never_asked'
            sites[k] = e

    st = collections.Counter(e['status'] for e in sites.values())
    summary = {
        'sites': len(sites), 'calls': sum(e['kind'] == 'call' for e in sites.values()), 'jumps': sum(e['kind'] == 'jmp' for e in sites.values()),
        'by_status': dict(sorted(st.items())),
        'static_calls_pops_unsettled': sum(1 for e in sites.values() if (e.get('static') or {}).get('pops_settled', True) is False),
        'recorded_also_static_or_com': sum(1 for e in sites.values() if 'recorded' in e and e['status'] != 'recorded'),
        'recorded_game_targets_only_recorded': sum(1 for e in sites.values() if e['status'] == 'recorded' and any(t.startswith('0x') for t in e['recorded']['targets'])),
        'unresolved_by_reason': dict(collections.Counter(e['reason'] for e in sites.values() if e['status'] in ('unresolved', 'recorded')).most_common()),
        'conflicts': sorted(k for k, e in sites.items() if 'conflict' in e),
        'recorded_not_entry': sorted(k for k, e in sites.items() if 'not_entry' in e),
        'not_listed_import_slot_calls_and_jumps': n_import, 'not_listed_bounded_switch_jumps': n_switch_bounded,
    }
    doc = ('pipeline/indirect_targets.py on the analysed build: every indirect call and jump with its targets and the '
           'source of each (see the script\'s docstring). RECORDED TARGETS ARE OBSERVED, NOT PROVEN COMPLETE: a site '
           'with status "recorded" may also go elsewhere. Only "static" target sets are complete.')
    json.dump({'_doc': doc, '_sources': SOURCES, '_reasons': REASONS, '_not_recorded': NOT_RECORDED, '_summary': summary, 'sites': sites},
              open(out, 'w'), indent=0, sort_keys=False)
    for k, v in summary.items(): print('%-40s %s' % (k, v))
    if summary['conflicts']: print('CONFLICT: recorded targets outside a proven set at', ', '.join(summary['conflicts']))


if __name__ == '__main__':
    main(sys.argv[1])
