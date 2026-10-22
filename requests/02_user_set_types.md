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
