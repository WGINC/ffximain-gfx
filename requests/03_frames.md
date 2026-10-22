# Request 03: IDA's stack frame layout of every tree function

Background: byte matching (`pipeline/msvc_tu.py`, `verification/msvc_match_tree.json`) leaves out 446 tree functions
because their locals are address-taken (`xi_frame`: locals at their original stack offsets). Writing them as ordinary
locals that Visual C++ 6.0 lays out the same way needs each function's frame as the code uses it. IDA derives the
frame from the code's own stack accesses, so its offsets and sizes are what is wanted. Its names and types are
reported only with their origin.

## Functions
Every address in a `// FUNCTION: FFXIMAIN 0x........` line of `tree/src/*.cpp` (4,205).

## What to produce
`data/ida_frames.json`:
```json
{"0x10001234": {
  "frsize": 0x58, "frregs": 4, "argsize": 8,          // ida_funcs func_t fields: locals, saved regs, purged args
  "fpd": 0,                                          // frame pointer delta (func_t.fpd)
  "ebp_frame": true | false,                         // FUNC_FRAME flag
  "members": [
    {"soff": 0, "size": 4, "name": "var_58",
     "entry_off": -0x5C,       // byte offset from ESP at function entry (the return address is at 0)
     "type": "int" | null,
     "type_user_set": true | false | null,   // whether the member's type was set by hand, if IDA can tell
     "default_name": true | false}           // IDA's own var_XX/arg_X name
  ]
}}
```
plus `"_doc"`: IDA version, IDB name, date, counts, and how `entry_off` was computed: the formula from IDA's frame
offsets, stated, so it can be checked.

Leave out the saved-register and return-address pseudo-members (` r`, ` s`). A function IDA has no frame for gets
`null`.

## How
`ida/ida_frames.py` (IDAPython, read-only).

## Then
Follow `requests/README.md`: append a `## Status` section, commit, push.

## Status
done, 2026-10-21 (IDA session on the owner's PC; IDA 9.4, `FFXiMain_recon.i64`, same IDB as requests 01 and 02: `.text`
sha256 and PE timestamp match, no patched bytes). The IDB was not modified.

Produced: `ida/ida_frames.py`, `data/ida_frames.json`.

- 4,205 addresses (all distinct `// FUNCTION: FFXIMAIN` lines of `tree/src/*.cpp`); every one is a function start in the
  IDB. 4,196 have a frame (8,233 members after leaving out ` r`/` s`); 9 are `null`: tiny functions (5 or 8 bytes,
  `mov eax, [ecx]`...) with `frsize` 0, `frregs` 0, `argsize` 0, for which IDA builds no frame struct (listed in
  `_doc.no_frame`).
- 15 are `ebp_frame` functions; `fpd` is 0 for every function; no gap members.
- `entry_off = soff - (frsize + frregs)`. The return-address member sits at `frsize + frregs` in all 4,196 frames
  (`_doc.entry_off_check`), which is what ties `soff` to ESP at entry.
- A check against the code itself, on a sample only: the first 300 ESP-based frames with members, 3,033 direct
  `[esp+disp]` operands, address = `get_spd` + displacement. 2,765 land exactly on a member's `entry_off`, 117 inside a
  member, 151 on no member: 51 inside the locals area between members, 54 below `frsize`, 46 at or past the return
  address. Not known why the last two groups exist; one function holding some of the 46, `0x100097F0`, is among the 28
  whose Hex-Rays switch analysis failed (request 01's `_doc.decompile_errors`), so its SP deltas may be unreliable.
  A frame is therefore IDA's layout of the accesses it understood, not proof that no other stack bytes are used.
- `type_user_set` is `null` for every member: IDA's frame struct has no flag for it. `default_name` only says the name
  has IDA's `var_`/`arg_` form, not that nobody renamed a member to such a name.
