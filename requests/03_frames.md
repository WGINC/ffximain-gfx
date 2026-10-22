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
