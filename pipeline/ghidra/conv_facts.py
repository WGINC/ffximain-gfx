"""conv_facts.py -- each function's calling convention, stack purge and float return, from the game's own code,
for GfxExport.java to set before it decompiles (so Ghidra's text reads ECX as `this`, not as an uninitialised
`in_ECX`, and a float return as ST0, not EAX).

Sources (all whole-DLL facts in data/):
  ecx_use.json    an instruction reads the entry ECX, or the entry ECX reaches a callee that reads it; False: neither
  edx_use.json    the same for EDX (True or an instruction: read; False: not; null: not established)
  purges.json     the function's own `ret N`
  tail_purges.json no `ret`: every exit tail-jumps at stack depth 0 to targets popping the same N (tail_purges.py)
  entry_regs.json reads of EAX/EBX/ESI/EDI/EBP on entry (a __usercall): no standard convention says that
  fpret.json      the value left in ST0 on return ('float') or none; 'consumes': pops a value the caller left in ST0 (RET st0in)
  reg_keeps.json  which of EAX/ECX/EDX the function leaves untouched, its callees included (reg_keeps.py)

Rules (nothing else is decided):
  ECX read, EDX not read  -> __thiscall        ECX read, EDX read -> __fastcall
  neither read            -> __stdcall if ret N > 0, else __cdecl
  ECX read, EDX not established -> __thiscall (an EDX read then shows as `in_EDX`, refused by hx2cpp)
  EDX read, ECX not read  -> __stdcall / __cdecl by ret N, with EDX a register input (`0xADDR regin EDX`)
  entry_regs reads EAX only -> the convention by the rules above, with EAX a register input (`0xADDR regin EAX`)
  ECX not established, EDX not established without ECX, any other entry_regs entry, no purge -> not set (the line says why)
Kept registers: `0xADDR keeps eax,edx` for every function reg_keeps.json lists (a caller reading the register
after the call reads its own value from before it; GfxExport gives the callee a prototype model saying so).

Indirect calls (a virtual or COM call, a function pointer), at the call instruction:
  pops            sp_solved.json (the caller's balance solved), vcalls.json (every proven callee's ret N),
                  com_calls.json, real_sp.json (real code ran); a call they disagree on is left out
  ECX = object    ecx_this_sites.json: ECX holds the object whose vtable is called, not also pushed
  ECX = object, pops known -> __thiscall with pops/4 stack arguments
  otherwise, pops > 0      -> __stdcall with pops/4 stack arguments (a pushed COM `this` is the first)
  pops 0 without ECX, pops unknown -> not set (Ghidra's own reading of the pushes stays)
  returns in ST0  vcalls.json "ret": "st0" (every proven callee); otherwise EAX

    python3 conv_facts.py OUT.txt
Output lines `0xADDR CC PURGE RET` (RET float|st0in|-), `0xCALL icall CC NSTACK RET`, or `0xADDR - why`."""
import json, os, sys

D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'data')
load = lambda f: json.load(open(os.path.join(D, f)))


def decide(k, eu, du, pu, er, fp):
    regin = []
    if k in er:
        if set(er[k]) != {'eax'}: return None, 'usercall: reads %s on entry (entry_regs.json)' % ','.join(sorted(er[k]))
        regin.append('EAX')
    if k not in pu: return None, 'no ret N (purges.json)'
    if k not in eu or eu[k] is None: return None, 'ECX use not established (ecx_use.json)'
    e, d = bool(eu[k]), du.get(k)
    if d is None and not e: return None, 'EDX use not established (edx_use.json)'
    d = bool(d)           # null with ECX read: __thiscall; an EDX read Ghidra sees stays `in_EDX`, which hx2cpp refuses
    if e and d: cc = '__fastcall'
    elif e: cc = '__thiscall'
    else:
        cc = '__stdcall' if pu[k] else '__cdecl'
        if d: regin.append('EDX')
    f = fp.get(k.lower())
    return (cc, pu[k], 'float' if f == 'float' else 'st0in' if f == 'consumes' else '-', regin), None


def icalls():
    pops = {}
    for f in ('sp_solved.json', 'vcalls.json', 'com_calls.json'):
        for k, v in load(f).items():
            if not k.startswith('_') and isinstance(v, dict) and v.get('pops') is not None: pops.setdefault(k, set()).add(v['pops'])
    for r in load('real_sp.json').values():
        if isinstance(r, dict):
            for k, p in r.get('calls', {}).items(): pops.setdefault(k, set()).add(p)
    et = {k: v for k, v in load('ecx_this_sites.json').items() if not k.startswith('_')}
    vc = {k: v for k, v in load('vcalls.json').items() if not k.startswith('_')}
    out = {}
    for k in sorted(set(pops) | set(et), key=lambda s: int(s, 16)):
        p = pops.get(k, set())
        if len(p) > 1: out[k] = (None, 'pops disagree: %s' % sorted(p)); continue
        p = next(iter(p)) if p else None
        ecx = k in et and et[k]['ecx'] and not et[k]['pushed']
        ret = 'float' if vc.get(k, {}).get('ret') == 'st0' else '-'
        if p is None: out[k] = (None, 'pops not established'); continue
        if ecx: out[k] = (('__thiscall', p // 4, ret), None)
        elif p: out[k] = (('__stdcall', p // 4, ret), None)
        else: out[k] = (None, 'pops 0 without ECX: argument count not established')
    return out


def main(out):
    eu, du, pu, er, fp = (load(f) for f in ('ecx_use.json', 'edx_use.json', 'purges.json', 'entry_regs.json', 'fpret.json'))
    pu.update({k: v['pops'] for k, v in load('tail_purges.json').items() if k not in pu})
    n = {}
    with open(out, 'w') as f:
        for k in sorted(set(eu) | set(pu), key=lambda s: int(s, 16)):
            r, why = decide(k, eu, du, pu, er, fp)
            f.write('%s %s %d %s\n' % ((k,) + r[:3]) if r else '%s - %s\n' % (k, why))
            if r and r[3]: f.write('%s regin %s\n' % (k, ','.join(r[3]))); n['regin'] = n.get('regin', 0) + 1
            key = r[0] if r else why.split(':')[0].split(' (')[0]
            n[key] = n.get(key, 0) + 1
        for k, v in sorted(load('reg_keeps.json').items(), key=lambda kv: int(kv[0], 16)):
            f.write('%s keeps %s\n' % (k, ','.join(v))); n['keeps'] = n.get('keeps', 0) + 1
        for k, (r, why) in icalls().items():
            f.write('%s icall %s %d %s\n' % ((k,) + r) if r else '%s - icall: %s\n' % (k, why))
            key = 'icall ' + (r[0] if r else why.split(':')[0])
            n[key] = n.get(key, 0) + 1
    print(n)


if __name__ == '__main__':
    main(sys.argv[1])
