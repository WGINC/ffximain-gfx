# FFXiMain.dll (2026-09-03): `cc.py` and `discover.py` fixes (30 Sep 2026)

This is an addendum to `REPORT-ida.md`. It implements the §4 fixes, using the unpacked DLL
(MD5 `f6dbabef…904d`, uploaded as `FFETO_Swamped.dll`). The DLL itself is not in this bundle.

## Inputs, and one limit

The uploaded DLL has only the standard relocation table (18,416 `.rdata` + 5,671 `.data` + 1 POL1).
It does **not** have the packer's 68,475 private `.text` relocations. `xiunpack.py rebuild` restores
those, but only from the **packed** `FFXiMain.dll`. Without them:

- **`cc.py` could be re-run fully.** It needs the table relocations for one thing: switch-table
  targets. `synth_relocs.py` rebuilds those from `functions.json`, keeping each entry only while it
  points into the same function (8,483 entries).
  - **The substitution was tested.** The *unfixed* `cc.py`, run this way, reproduces the shipped
    `prototypes.json` exactly on all 20,146 entries outside the tail-wrapper post-step.
- **`discover.py` could not be re-run.** The fix is in the code (`discover.py` in this bundle), and
  `functions.json` was corrected by hand. A full re-run needs the packed DLL. **Not verified.**

## Fixes in `cc.py`

| # | Fix |
|---|---|
| 1 | A call or tail jump to a function that uses ECX, made while ECX still holds the incoming value, counts as an ECX use (pass-through `this`). Resolved as a fixpoint over the call graph (5 rounds; 606 functions). |
| 2 | Tail-wrapper inheritance is now in `cc.py`. It was missing from the shipped code, which had applied it as a post-step. A function whose paths all end in tail jumps made at entry depth inherits the target's `ret N`. ECX-ness is the wrapper's own, so a wrapper that loads ECX itself isn't thiscall. Pass 2 and inheritance alternate until stable (4 rounds; 1,465 wrappers). Wrappers whose depth can't be proven are `ambiguous`, not `noreturn`. |
| 3 | A first-instruction `push ecx` (stack reservation) isn't a read. `or r,-1` and `and r,0` (inlined `strlen`, etc.) only write `r`. |
| 4 | `__alloca_probe` (`0x10312150`) and `__EH_prolog` (`0x103176F0`) preserve ECX and EDX. Confirmed by disassembly. |
| 5 | ESP is tracked through `__alloca_probe` (ESP −= the preceding `mov eax, imm`) and `__EH_prolog` (EBP = entry−4, ESP = entry−16). Arity above 16 becomes `ambiguous`. |

## Fix in `discover.py` (not verified)

The gap scan skips a candidate whose first two dwords are relocated `.text` pointers (`looks_like_table`).

`functions.json` corrected directly:
- the 13 switch tables removed (`0x1003C670`, `0x1003C6B0`, `0x1005DE50`, `0x100AF4A0`,
  `0x100B1090`, `0x100BC960/70/80/90`, `0x100D6740`, `0x10205A40`, `0x1021ACE0`, `0x102A4F10`);
- the swallowed `0x1003C6C0` added back. It comes out as thiscall with 1 argument, matching its code.
- Result: 21,616 functions.

## Validation

| Check (independent of the fixed code) | Before | After |
|---|---|---|
| Stack cleanup vs IDA's purged bytes, same-extent functions | 17,440 agree / 0 differ | **17,513 agree / 0 differ** |
| 12 functions adjudicated by hand in IDA's disassembly | 1 / 12 | **12 / 12** |
| Entries with more than 16 arguments that get a type | 60 (up to 199,558) | **0** |
| IDA-side ECX checker, families confirmed by hand | — | 60/60, 184/185, 116/116, 184/189 |

**31 disagreements with the IDA-side checker were adjudicated. The checker was wrong in all 31:**
- 22 are partial-register writes (`mov cl, …` / `cx`) that it counted as reads of `this`.
- The rest are pass-through verdicts that it inherited from the old, wrong callee labels.
- Its `or ecx,-1` blind spot also affected the old `cc.py`, which is now fixed (`0x102988D0`
  becomes cdecl with 2 arguments).

**Net change against the shipped `prototypes.json`:**
- 1,387 conventions changed, **1,001 of them in whether ECX carries `this`** (about 4.6%; the earlier
  report estimated about 2.6%);
- 77 arity changes with the same convention;
- **The gate-verified code is unaffected:** the gate compares ECX at every call.

## IDB

In `FFXiMain_recon.i64`, the 60 functions typed in phase 6 whose convention or arity now differs
were retyped (0 failures), and the database was saved. Existing IDA types are still untouched. Log:
`ffxi_ida_exchange\retype_log.json`.

## Reliability ratings, updated

| Method | Rating | Note |
|---|---|---|
| ECX / thiscall decision (`cc.py`) | C → **B** | Passes every independent check above. No ground truth beyond IDA and hand samples. |
| cdecl arity | D above 16 → **B** | |
| `apply_functions_ida.py` (regenerated) | D → **U** | Parses; no typed entry above 16 arguments. Not run in IDA. The IDB already carries the same types via the §3 scripts. |
| `discover.py` switch-table fix | **U** | Needs the packed DLL to re-run. |

## Files

- `pipeline/`: `cc.py` (fixed), `discover.py` (fixed), `synth_relocs.py`, `cc_run.py`, `validate.py`.
- `data/`: `functions.json` (corrected), `prototypes.json` (regenerated), `apply_functions_ida.py`,
  `functions.csv`.

To reproduce:

```
XI_DLL=FFXiMain_unpacked.dll python3 cc_run.py cc.py
```

This runs in about 75 seconds.
