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
| ffxi_hide_ui (github.com/genoxd/ffxi_hide_ui) `engine/signatures.h`, `engine/game.h` | 148 named byte patterns for the UI layer's routines, each with a comment; the menu object's fields | 0BSD, Copyright (c) 2026 Genoxd | 662a869, 28 Oct 2026 |
| ffxi_world_draw (github.com/genoxd/ffxi_world_draw) `ffxi_world_draw.h` | the renderer and camera globals by pattern, the fields it reads from them, and how it finds the end of the world pass | 0BSD, Copyright (c) 2026 Genoxd | 622e7c1, 28 Oct 2026 |

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

## 28 Oct 2026: ffxi_hide_ui and ffxi_world_draw (data/genoxd_scan.json)

`pipeline/genoxd_scan.py HIDE_UI_DIR WORLD_DRAW_DIR data/genoxd_scan.json` (FFXI_DLL set), under the rules above.

- **Located (fact).** Every one of the 148 ffxi_hide_ui patterns matches this build: 142 exactly once (99 at a
  function start, 43 at a site inside a function), 6 more than once (set aside). ffxi_world_draw's two patterns
  match once each and name the globals 0x1045666C (its "renderer": our `g_pCDx`) and 0x104568FC (its camera
  store: our `g_pCamera`); its camera pattern names the renderer global too, and the two agree.
- **Used (fact).** All 23 field offsets the two claim are accessed, as `[reg + offset]`, in the code they tie to
  the object: the menu's 17 (state +0x10, list +0x14, input wait +0x56, closing +0x6E, ...) in the routines
  ffxi_hide_ui's own signatures locate; the renderer's 4 (device +0x0C, render-target stack +0x10, its depth
  +0x158, world target +0x194) in CDx's methods; the camera's 2 (view +0x94, projection +0x154) in the functions
  that load `g_pCamera`. Where we already named a field the two agree: CDx +0x158 is our `renderTargetDepth`,
  +0x194 our `sceneTargetA` (read by GetStereoSecondEyeTarget, 0x1000CA42). What each field means is the source's
  statement, kept in the file with it.
- **Names (the source's knowledge).** The 99 functions carry the source's identifier and comment (`kSigOpen`,
  `kSigMenuDraw`, ...). Where the tree names the same function the two agree (0x1015E350 kSigOpen /
  MenuMng_OpenByName, 0x1015FA00 kSigUpdate / Menu_DrawAll, 0x101177D0 kSigMenuDraw / Menu_DrawWindowItems,
  0x1000CD00 kSigPrimitiveDraw / DrawPrimitiveUP, 0x1015DD00 kSigMenuRouting / Menu_HandleEvent). They enter the
  tree's names at its next round (hx2clean.py reads them as it reads data/dm_names.json), not before: a name in the
  tree changes its regenerated text.
- **What this settles:** the menu layer's draw reaches the renderer by direct calls (docs/UI.md), and where the
  world pass ends (docs/RENDERER.md, section 9).
