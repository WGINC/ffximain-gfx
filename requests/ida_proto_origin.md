# Request: where each IDB prototype the gate relies on came from (P13)

For: a Claude Code session on the owner's PC with IDA open on the FFXiMain IDB and its MCP plugin running.
From: the cloud session working on this branch (`claude/gifted-archimedes-cjtg2d`). Background: `docs/PROBLEMS.md`
P13 and `CLAUDE.md` (do not guess; a fact names its source).

## Why
The gate takes a call's pop count, `this` in ECX, and argument count from `data/callsite_args.json` entries with
`"src": "typed"`: a call type declared in the IDB. The owner says some IDB types were inferred by earlier sessions,
so a "typed" entry is a fact only where its type has a definitive origin. 4,784 sites have no other source.

## What to produce
`data/ida_proto_origin.json`, one entry per key of `data/callsite_args.json` whose `src` is `"typed"`:

```json
{"0xCALLSITE": {
  "callee": "0x10001234" | null,          // direct call target, else null
  "kind": "function" | "member" | "callsite" | "other",
  //  function: the type is the callee function's own prototype
  //  member:   an indirect call typed by a struct member (e.g. lpVtbl->Method): give "struct" and "member"
  //  callsite: a type set on the call instruction itself (IDA's call-type override)
  "struct": "IDirect3DDevice8Vtbl", "member": "SetRenderState",   // for kind "member"
  "type": "the type as IDA prints it",
  "user_set": true | false,               // function: ida_nalt.is_userti(callee); callsite: whether an override exists
  "til": "d3d8" | "mssdk_win7" | null,    // the type library the type (or struct) comes from, if any
  "note": "the function's comment text containing \"[type \", if any"
}}
```

Also record, once, at the top level under `"_doc"`: the IDA version, the IDB file name (not its path), and the date.

## How
- Write the exporter as `ida/ida_proto_origin.py` (IDAPython), so it can be rerun, and run it in IDA through the MCP
  plugin or File > Script file. Read `data/callsite_args.json` from this repo for the site list.
- Use only what IDA stores; do not infer or fill in a missing value: `null` where IDA has nothing.
- Do not modify the IDB.
- Never add the IDB, the DLL, or any binary to the repo (`.gitignore` blocks them; keep it that way).

## When done
Commit `ida/ida_proto_origin.py` and `data/ida_proto_origin.json` to `claude/gifted-archimedes-cjtg2d` and push. The
commit message ends with the two attribution lines this repository's commits use. Then tell the owner it is pushed.
