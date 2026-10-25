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
  x87_effects.json each function's x87 inputs and outputs: its verdict replaces fpret.json's (x87_returns); a function
                  leaving the x87 stack as it was, called with values on it, keeps `st` (x87_keeps); an indirect call
                  it shows returning a float in ST0 gets RET float
  reg_keeps.json  which of EAX/ECX/EDX the function leaves untouched, its callees included (reg_keeps.py)
  exports/gfx_export.jsonl  the IDB's typed prototypes: a variadic one (`...`) for a function that pops 0

Rules (nothing else is decided):
  ECX read, EDX not read  -> __thiscall        ECX read, EDX read -> __fastcall
  neither read            -> __stdcall if ret N > 0, else __cdecl; a __cdecl one's stack arguments are the slots
                             its code reads (data/cdecl_args.json, exact counts only): `0xADDR cargs N`
  ECX read, EDX not established -> __thiscall (an EDX read then shows as `in_EDX`, refused by hx2cpp)
  EDX read, ECX not read  -> __stdcall / __cdecl by ret N, with EDX a register input (`0xADDR regin EDX`)
  entry_regs reads EAX only -> the convention by the rules above, with EAX a register input (`0xADDR regin EAX`)
  eax_inputs.json lists it (its entry EAX is read, or handed back to a caller that reads it: caller_reads.py) and
  reg_keeps.json does not say it leaves all of EAX untouched (that case is the `keeps` model below)
                          -> EAX a register input too
  ECX not established, EDX not established without ECX, any other entry_regs entry, no purge -> not set (the line says why)
Variadic: `0xADDR varargs N` (N fixed parameters, the IDB's declared prototype; only where ret N pops 0, as a
caller-cleanup variadic call must), so Ghidra reads each call's own pushes past the fixed ones.
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


EI, RK = {}, {}          # eax_inputs.json, reg_keeps.json (main loads them)


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
    if 'EAX' not in regin and k in EI and 'eax' not in RK.get(k, ()): regin.insert(0, 'EAX')
    f = fp.get(k.lower())
    return (cc, pu[k], 'float' if f == 'float' else 'st0in' if f == 'consumes' else '-', regin), None


def variadic():
    """{0xADDR: fixed parameter count} for the non-import functions the IDB declares with `...`"""
    out = {}
    for l in open(os.path.join(D, 'exports', 'gfx_export.jsonl')):
        r = json.loads(l)
        ts = [(r['ea'], r.get('type') or '')] + [(o['ea'], o.get('type') or '') for o in r.get('objs', {}).values()
                                                  if o.get('func') and not o.get('import')]
        for ea, t in ts:
            if '...' not in t: continue
            ps = [x for x in t[t.index('(') + 1:t.rindex(')')].split(',') if x.strip()]
            out['0x%08X' % ea] = len(ps) - 1
    return out


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
    # x87_effects.json: an indirect call returning a float in ST0 (vcalls.json's `st0`, or shown by the caller's
    # own first read of the value -- see its doc)
    xf = {k for k, v in load('x87_effects.json')['sites'].items() if v['callee'] is None and v['r'] == 1}
    out = {}
    for k in sorted(set(pops) | set(et), key=lambda s: int(s, 16)):
        p = pops.get(k, set())
        if len(p) > 1: out[k] = (None, 'pops disagree: %s' % sorted(p)); continue
        p = next(iter(p)) if p else None
        ecx = k in et and et[k]['ecx'] and not et[k]['pushed']
        ret = 'float' if vc.get(k, {}).get('ret') == 'st0' or k in xf else '-'
        if p is None: out[k] = (None, 'pops not established'); continue
        if ecx: out[k] = (('__thiscall', p // 4, ret), None)
        elif p: out[k] = (('__stdcall', p // 4, ret), None)
        else: out[k] = (None, 'pops 0 without ECX: argument count not established')
    return out


def x87_returns(fp):
    """fpret.json's verdicts with x87_effects.json's where it has one (it follows tail jumps to a value-popping
    function and float returns fpret.json has no verdict for; its selftest checks every disagreement): (0 in, 1 out)
    'float', (1, 0) 'consumes', (0, 0) 'none'; a verdict with u (its x87 return not known) only when it is a float
    the function's other returns fix. Other shapes keep fpret.json's word."""
    x = load('x87_effects.json')['funcs']
    out = dict(fp)
    for k, v in x.items():
        if 'why' in v or (v['u'] and v['r'] != 1): continue
        kr = (v['k'], v['r'])
        f = {(0, 1): 'float', (1, 0): 'consumes', (0, 0): 'none'}.get(kr)
        if f: out['0x%x' % int(k, 16)] = f
    return out


def x87_keeps():
    """functions that leave every x87 register as they found it -- x87_effects.json (0 in, 0 out, its return known)
    -- and that some caller calls with values on the x87 stack (a site with depth > 0): `st` in their keeps line"""
    x = load('x87_effects.json')
    ok = {k for k, v in x['funcs'].items() if 'why' not in v and (v['k'], v['r']) == (0, 0) and not v['u']}
    return {v['callee'] for v in x['sites'].values() if v['callee'] in ok and v['d'] > 0}


def main(out):
    eu, du, pu, er, fp = (load(f) for f in ('ecx_use.json', 'edx_use.json', 'purges.json', 'entry_regs.json', 'fpret.json'))
    fp = x87_returns(fp)
    EI.update(load('eax_inputs.json')); RK.update(load('reg_keeps.json'))
    pu.update({k: v['pops'] for k, v in load('tail_purges.json').items() if k not in pu})
    n = {}
    with open(out, 'w') as f:
        for k in sorted(set(eu) | set(pu), key=lambda s: int(s, 16)):
            r, why = decide(k, eu, du, pu, er, fp)
            f.write('%s %s %d %s\n' % ((k,) + r[:3]) if r else '%s - %s\n' % (k, why))
            if r and r[3]: f.write('%s regin %s\n' % (k, ','.join(r[3]))); n['regin'] = n.get('regin', 0) + 1
            elif not r and k in EI and 'eax' not in RK.get(k, ()):
                # no convention decided, but the entry EAX is an input all the same (eax_inputs.json): a register
                # parameter on whatever prototype the function has, so its callers pass it (hx2cpp's asm call needs
                # only the callee's ret N)
                f.write('%s regin EAX\n' % k); n['regin EAX alone'] = n.get('regin EAX alone', 0) + 1
            key = r[0] if r else why.split(':')[0].split(' (')[0]
            n[key] = n.get(key, 0) + 1
        ca = {k: v for k, v in load('cdecl_args.json').items() if not k.startswith('_')}   # exact counts only
        va = variadic()
        for k in sorted(set(ca) - set(va), key=lambda s: int(s, 16)):
            r, why = decide(k, eu, du, pu, er, fp)
            if not r or r[0] != '__cdecl': continue
            f.write('%s cargs %d\n' % (k, ca[k])); n['cargs'] = n.get('cargs', 0) + 1
        # no convention decided (ECX or EDX use not established), the stack bytes known: `ret N`, or for one that
        # pops 0 the slots it reads (exact)
        for k in sorted(set(pu), key=lambda s: int(s, 16)):
            r, why = decide(k, eu, du, pu, er, fp)
            if r or not why.startswith(('ECX use not established', 'EDX use not established')): continue
            if pu[k]: f.write('%s spops %d ret\n' % (k, pu[k]))
            elif k in ca: f.write('%s spops %d reads\n' % (k, 4 * ca[k]))
            else: continue
            n['spops'] = n.get('spops', 0) + 1
        # no convention decided, the value left in ST0 on return known (fpret.json `float`): the return alone
        for k in sorted(fp, key=lambda s: int(s, 16)):
            if fp[k] != 'float': continue
            r, why = decide(k, eu, du, pu, er, fp)
            if r: continue
            f.write('0x%08X fret float\n' % int(k, 16)); n['fret'] = n.get('fret', 0) + 1
        for k, nfix in sorted(va.items(), key=lambda kv: int(kv[0], 16)):
            if pu.get(k) != 0: continue
            f.write('%s varargs %d\n' % (k, nfix)); n['varargs'] = n.get('varargs', 0) + 1
        rk = {k: list(v) for k, v in load('reg_keeps.json').items() if not k.startswith('_')}
        for k in x87_keeps(): rk.setdefault(k, []).append('st')
        for k, v in sorted(rk.items(), key=lambda kv: int(kv[0], 16)):
            f.write('%s keeps %s\n' % (k, ','.join(v))); n['keeps'] = n.get('keeps', 0) + 1
            if 'st' in v: n['keeps st'] = n.get('keeps st', 0) + 1
        for k, (r, why) in icalls().items():
            f.write('%s icall %s %d %s\n' % ((k,) + r) if r else '%s - icall: %s\n' % (k, why))
            key = 'icall ' + (r[0] if r else why.split(':')[0])
            n[key] = n.get(key, 0) + 1
    print(n)


if __name__ == '__main__':
    main(sys.argv[1])
