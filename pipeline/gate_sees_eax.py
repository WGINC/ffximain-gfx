"""gate_sees_eax.py -- whether the gate compares what a game function hands back from its caller's EAX.

A tree function calls the game's own code through a wrapper (tree/include/gfx_orig.hpp) that passes no EAX. Where
the callee hands back bytes of its entry EAX (eax_inputs.json "hands_back") and this caller reads them after the
call (eax_kept_sites.json), the tree's caller gets back whatever its compiled code left in EAX, the original its
own leftover. The gate sees that difference only where it runs the call as the game does:
  * the caller's gate case does not stub the callee: the callee's own code runs on the reconstruction's EAX, in
    both runs; or
  * the stub hands back all of EAX (its `eax_keeps` is every byte) and the callee's code leaves EAX untouched on
    every path (data/reg_keeps.json): the stub then does exactly what the code does.
Anywhere else the caller is left out of the tree (hx2clean.py) and verify_all.py fails a tree that holds it.

    seen(cases, caller, callee) -> bool       cases: {caller address: case} (a gate cases file)"""
import json, os
D = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'data')
_RK = None


def seen(cases, caller, callee):
    global _RK
    if _RK is None: _RK = json.load(open(os.path.join(D, 'reg_keeps.json')))
    case = cases.get(caller)
    if case is None: return False
    st = (case.get('stubs') or {}).get('0x%08X' % callee)
    if st is None: return True
    return st.get('eax_keeps') == 0xFFFFFFFF and 'eax' in (_RK.get('0x%08X' % callee) or [])


def load_cases(path):
    return {int(c['original'], 16): c for c in json.load(open(path))}
