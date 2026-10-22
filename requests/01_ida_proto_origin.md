# Done: where each IDB prototype the gate relies on came from (P13)

Status: **done**, 2026-10-21, by the IDA session on the owner's PC (see `requests/README.md`). This file replaces the
request; the request itself is in this file's history (7705956).

## Produced
- `ida/ida_proto_origin.py` -- the exporter (IDAPython, read-only). First written and run in d2a2a69; this commit
  adds three fields per entry (`insn_call`, `insn_target`, `target_agrees`) and two lists in `_doc`.
- `data/ida_proto_origin.json` -- one entry per `"src": "typed"` key of `data/callsite_args.json` as of e79080c:
  **8,274** entries (the d2a2a69 run had 8,381; the 107 it no longer has are the sites e79080c moved to `"regs"`).
  No other field of any remaining entry changed between the two runs.

Run in IDA 9.4 (Hex-Rays 9.4.0.260713) on `FFXiMain_recon.i64`. The IDB records no input file name; it is identified
by its contents: `.text` (0x10001000, 0x3275EE bytes) SHA-256
`b55f8b4c730c00229e2febd1ea6a5efba29920e094fa565a3816b763c3d9cdc9`, PE header TimeDateStamp `0x6A995428`, ImageBase
0x10000000, and no patched bytes (`visit_patched_bytes` empty; every `.text` byte equals `get_original_byte`).
The IDB was not modified. Used in `docs/PROBLEMS.md` P13 (the d2a2a69 output).

## What the IDB says (counts over the 8,274)
| kind | sites | origin facts |
|---|---|---|
| function | 5,608 | `user_set` (is_userti) true 1,118, false 4,490; `til` mssdk_win7 for 60 (library symbol prints identically), none otherwise |
| other | 2,466 | helpers, import slots, locals, globals, and sites with no call expression (see `what`) |
| member | 200 | struct member types; no `til` (no struct here reports is_from_subtil) |
| callsite | 0 | no call-type override is stored on any of these instructions |

63 callees carry a function comment containing `[type `. Hex-Rays today: 3,311 sites decompile typed, 1,526 with a
cast, 3,437 have no call expression (28 functions fail switch analysis, listed in `_doc.decompile_errors`; 10 calls
are not in the decompilation).

## Found while doing this (facts, from the IDB; causes not known)
1. **748 of the 8,274 "typed" keys are not call instructions** (`is_call_insn` false; `_doc.not_call_insn`). 632 of
   them have `args` > 0 in `callsite_args.json`. By what the walk finds there: 276 are Hex-Rays helpers with no call
   instruction behind them (qmemcpy 88 on `rep movs`, memset 53, LODWORD 37, SLODWORD 17, atan2 10 on `fpatan`,
   sin 8, ...); 89 are a named function's call expression that Hex-Rays placed at a non-call address (all 89 at a `jmp`);
   269 + 65 + 49 have no call expression or a cast, at `mov`, `push`, `rep`, FPU and other instructions. A stack pop
   count recorded at such an address has no call instruction to pop it.
2. **29 entries record a target that is not the instruction's own** (`target_agrees` false; `_doc.target_disagrees`),
   so their `type` describes a different function than the one at the address:
   - 8 import calls: Hex-Rays reads the IAT slot 0x30 bytes past the one the instruction encodes and IDA's xref names.
     Example 0x10001563: bytes `ff15e4933210`, `call ds:GetWindowRect` (0x103293E4), recorded as `__imp_recv`
     (0x10329414). The others: 0x10001680 TranslateMessage as `__imp___WSAFDIsSet`, 0x1000168B DispatchMessageA as
     `__imp_timeGetDevCaps`, 0x10022C46/0x10022C8F/0x10022E22/0x10022E71 Sleep as FindFirstFileA, 0x101247E5
     GetWindowRect as `__imp_recv`.
   - 21 are `jmp` instructions whose recorded callee is a call elsewhere (e.g. 0x10010F7B `jmp short loc_10010F9B`,
     recorded as nullsub_49).
   `callsite_args.json` takes its count and convention from the same Hex-Rays call expression, so these 29 counts
   belong to the recorded callee, not to the instruction at the key.

## Not answered
- Why Hex-Rays reads those 8 IAT slots at +0x30, and why it puts call expressions at `jmp` and other non-call
  addresses: the IDB does not say.
- For `user_set` false, IDA does not record who set a type, only that it was not marked user-defined; it is not
  evidence that the type came from a library.
