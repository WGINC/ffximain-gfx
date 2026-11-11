# The documents, and what each is for

One place per subject. When two documents disagree, the one listed here as the place for that subject is right, and
`HISTORY.md` is right about what happened when.

## Read first

| Document | Subject |
|---|---|
| `../README.md` | What the repository is, its layout, where things stand, how to reproduce |
| `../CLAUDE.md` | The working rules (no guessing: every fact has a named source) |
| `EFFICIENCY.md` | How to work on this repository cheaply |
| `HANDOFF.md` | The state handed from one session to the next, and the next steps |

## Reference (kept current)

| Document | Subject |
|---|---|
| `METHOD.md` | How the reconstruction is made and verified: the pipeline, the gate |
| `RENDERER.md` | What the game's renderer calls, where, and what follows from it |
| `FORMATS.md` | The data formats, read from the code that reads them |
| `KIT.md` | The in-game kit: install, every setting, build translation for new game builds |
| `SDK.md` | Writing a mod: hooks, forks, device hooks |
| `DEVKIT.md` | The devkit's parts, the seam of the graphics system, FFXiMain.pdb, editor projects, what is left |
| `D3D11.md` | The Direct3D 11 renderer's scheme (`../kit/d3d8on11/README.md` is the device itself) |
| `SHAPE.md` | Source-shaping: the passes that make the tree read like source, each proven to compile to the same objects |
| `NAMES.md`, `COMMUNITY.md` | How names are given, and how the community's names enter |
| `UI.md` | The TkMenu UI layer checked against the game |
| `RECOMPILE.md`, `RECCMP.md` | Compiling to the game's bytes; reccmp against match.py |
| `IN_EAX.md`, `LOCALS.md` | Two analysis subjects: EAX at entry; local variables |

## Plan

| Document | Subject |
|---|---|
| `PLAN.md` | The plan in 5% steps, each with its "done when" |
| `PLAN_TASKS.md` | The plan's tasks and their state |
| `ROADMAP.md` | The three goals behind the plan (performance items P0 to P4 are defined here) |
| `FRAMESET_REMAINING.md` | The frame-set functions still outside the tree, each with its reason |
| `GAPS.md` | What is not known that stands in the way |

## Record (dated; not updated after the fact)

| Document | Subject |
|---|---|
| `HISTORY.md` | Everything done, by date, with the numbers and their sources |
| `PROBLEMS.md` | Problems met and how each ended |
| `MEASURE.md` | The measuring sessions' design (the results are in `HISTORY.md`) |
| `REEVALUATION.md` | The re-evaluation of 17 Oct |
| `DECOMP_COMPARISON.md` | An outside decompilation compared with ours |
| `DEARTIFACT.md`, `SKINMERGE.md` | Two finished pieces of work: de-artifacting; merged character draws |
| `owner_ui/` | The owner's UI documentation, as received |

The companion repository `dancingmad` holds the research record (findings, the PS2 symbol cross-check, the graphics
pipeline map); this repository holds what is verified against the game and what is built on it.
