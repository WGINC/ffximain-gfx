# The FFXI community's names, and how they enter this repository

The owner's rule (22 Oct 2026): where the community has already worked something out, trust it; where it is not
documented, or it conflicts with our own names, measure it (docs/MEASURE.md).

## Sources (checked out locally, never run; read as data)

| Source | What it gives | Licence | Checked out |
|---|---|---|---|
| Ashita v4 (github.com/AshitaXI/Ashita-v4beta) `config/ashita/ashita.pointers.ini` | 35 named byte patterns in FFXiMain.dll | LGPL-3.0 (SDK), see the repository | 4171c74, 11 Sep 2026 |
| Ashita v4 addons (`ashita.memory.find(...) -- name`) | function and global names, many the developers' own symbols (`ntGameMonthGet`, `FsMacroContainer::setPage`) | as above | as above |
| Ashita v4 SDK `plugins/sdk/ffxi/*.h` | 30 structs, 513 members; offsets and sizes by `offsetof`/`sizeof`, compiled for i686 with the headers' own size asserts | LGPL-3.0 | as above |
| Windower 5 packages (github.com/Windower/packages) `libraries/memory/types.lua` | 37 structs with explicit offsets, and the signatures that locate them | BSD-3-Clause, Copyright (c) 2018 Windower Dev Team | 756138e, 30 Dec 2024 |
| Windower 4 (github.com/Windower/Lua) | no memory layouts (its core is closed); packet and resource definitions only | | |

Fenestra was looked for and not found (web search; GitHub's search API is not reachable from this environment).
Windower 5's last change predates this client build by most of two years; Ashita's is from September 2026.

## What is a fact here and what is not

- **Located (fact):** a pattern that matches the analysed DLL exactly once ties its name to one address
  (`pipeline/community_scan.py`, each tool's own rule for the position it returns: Ashita match + offset; Windower
  just after the match, dereferenced once, `&`/`*` markers). Patterns matching more than once are set aside.
- **Mapped (fact, with the evidence in `data/community_objects.json`):** which of our objects a community struct
  describes. The entity: the table both tools locate (0x10480AF0) holds only 0x2AC = 684-byte objects built by
  0x1008A120 (vtable 0x1032D69C), and Ashita asserts `sizeof(entity_t) == 684`. CDx: Windower's gamma red/green/blue
  at +0x7F8.. are what GammaRamp_SetRGB/GetRGB store and load (CDx+0x1F4 + 0x604..).
- **Named (the community's knowledge, kept with its source):** the names themselves.

## Results (data/community_reconcile.json)

- Functions: 25 adopted (ours was a placeholder or a naming session's reading), 1 the same, 11 Ashita pointer labels
  kept as aliases (they are labels, not symbols).
- The entity: of the 161 entity fields our verified code uses, 103 are named by the community at the offset and 51
  lie inside a named member (`Movement+0x4`); 7 are documented by neither. Where the two tools word a field
  differently, 25 pairs are judged the same (data/community_judged.json, a judgement, marked as one) and 8 say
  different things: to measure.
- **A correction:** the IDB (and DancingMad after it) calls the entity `CXiActor` (0x1008A120 is
  "CXiActor_Construct"). The game's own runtime-class descriptor `CXiActor` is a 196-byte class derived from CMoTask;
  the entity is the 684-byte object with vtable 0x1032D69C. The tree's "CXiActor" methods that use offsets past 0xC4
  work on the entity.
- CDx: 6 fields named (gamma); 35 of the 41 our graphics code uses are not documented by either tool: to measure.
- The camera object (g_pCamera = Windower `graphics.render`): frame-rate divisor +0x30, aspect +0x2F0, zoom +0x2F8.

## Pipeline

```
python3 pipeline/community_scan.py ASHITA WINDOWER5 data/community_scan.json      (FFXI_DLL set)
python3 pipeline/community_structs.py ASHITA WINDOWER5 data/community_structs.json
python3 pipeline/community_reconcile.py data/community_scan.json data/community_structs.json data/community_reconcile.json
```
