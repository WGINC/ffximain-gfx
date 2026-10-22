# Request 02: the user-set function types, for the owner to mark which they typed

Background: `docs/PROBLEMS.md` P13. A function type IDA flags as user-set (`ida_nalt.is_userti`) was set either by the
owner or by an earlier session, and IDA does not record which. Only the owner's own (and SDK/PS2-derived) types are
facts for the pipeline, so the owner needs a list to mark.

## What to produce
1. `data/ida_user_types.json`: every function in the IDB whose type is user-set:
   ```json
   {"0x10001234": {
     "name": "...", "type": "the prototype as IDA prints it",
     "comment": "the function comment (regular, then repeatable), or null",
     "session_note": true | false,       // a comment containing "[type " or "[name " (written by session scripts)
     "fix_script": true | false,         // the address is a key of DATA["types"] in ida/ffxi_ida_fix.py
     "gate_sites": 12,                   // sites in data/ida_proto_origin.json with this "callee"
     "in_tree": true | false             // a "// FUNCTION: FFXIMAIN 0x..." line for it in tree/src/*.cpp
   }}
   ```
   plus `"_doc"`: IDA version, IDB name, date, counts.
2. `requests/review/02_user_types.md`: a table for the owner covering only the entries with `gate_sites > 0` or
   `in_tree`, and neither `session_note` nor `fix_script`. It is sorted by `gate_sites`, highest first, with
   columns: address, name, prototype, gate sites, in tree, and an empty **Origin** column for the owner to fill with
   `mine`, `session` or `unsure`. Put one line above the table explaining the column.

## How
`ida/ida_user_types.py` (IDAPython, read-only). Read `ida/ffxi_ida_fix.py`'s `DATA` by parsing the file, not by
importing it, since importing would run its IDB changes.

## Then
Follow `requests/README.md`: append a `## Status` section, commit, push.

## Status
done, 2026-10-21 (IDA session on the owner's PC; IDA 9.4, `FFXiMain_recon.i64`, same IDB as request 01: `.text` sha256
and PE timestamp match, no patched bytes). The IDB was not modified.

Produced: `ida/ida_user_types.py`, `data/ida_user_types.json`, `requests/review/02_user_types.md` (372 rows, Origin empty).

- 3,219 of 21,410 functions have a user-set type (`is_userti`).
- 148 carry a `[type `/`[name ` comment (`session_note`); 18 are keys of `DATA["types"]` in `ida/ffxi_ida_fix.py`.
- 202 are the callee of at least one site in `data/ida_proto_origin.json` (1,118 sites in all, equal to request 01's
  `kind: function, user_set: true` count); 424 have a `// FUNCTION: FFXIMAIN` line in `tree/src`.
- The review table holds the 372 with gate sites or a tree line and neither `session_note` nor `fix_script`.
- `DATA["types"]` has 31 keys; 13 are not user-set in the IDB (listed in `_doc.fix_script_types_not_user_set`). Not
  known why: the fix script's types may not have been applied to this IDB, or were later replaced.
- `comment` is the regular comment, then the repeatable one, joined by a newline when both exist.
