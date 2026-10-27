# Plan: source → devkit → D3D11 → performance → final devkit and speedfix (25 Oct)

This plan replaces the step order of docs/ROADMAP.md (goals of 6 Oct) with the owner's chain of 25 Oct:

1. source-level decompilation of everything that affects frame time or what is drawn;
2. a devkit on that source;
3. the Direct3D 11 port;
4. performance work across the board;
5. a finalized devkit with a speedfix built on the source.

The groundwork is for D3D11, better effects, rendering and customization, and later other architectures and
platforms. The owner's teammate keeps the UI-specific kit; this repo covers the engine's graphics.

It takes from the existing plans:
- the performance work's P0–P5 and their rules: docs/ROADMAP.md;
- the D3D11 scheme, phases 0–3 and phase 0's measurements: docs/D3D11.md;
- the know/change/predict/confirm gaps and the order of the tools that close them: docs/GAPS.md;
- the three tests that bound what this machine can reach (T1–T3): docs/REEVALUATION.md section 7;
- the compiler identification and the matching tools: docs/RECOMPILE.md;
- the open problems: docs/PROBLEMS.md.

Those documents keep their detail. This one orders the work and says what "done" means at each 5%. The working
rules of CLAUDE.md hold throughout: every fact names a definitive source, and an unknown is reported, not filled.

## What changed in the approach (research of 25 Oct)

- **Matching decompilation stays.** It is the established way an MSVC-built Windows game reaches source, and later
  other platforms. The LEGO Island projects did it with reccmp, and their portable port replaced Direct3D with
  SDL3, D3D9, OpenGL and software renderers. What their port changed on the way is not documented on its page.
- **The renderer track runs in parallel, not after phase 2.**
  - kit/d3d8on11 is already our own Direct3D 8 device on D3D11: milestone 1 (everything but drawing), milestone 2
    (fixed-function drawing, 20 scenes within 1 of 255 against Wine's d3d8) and the command ring (D3D11.md phase 2).
  - It already runs the game: in the plaza in `thread` mode, 29.07 ms a frame with its executor filter against
    34.54 ms without, in one run (HISTORY.md, 17 Oct, the owner's perf60 runs). In the house, `thread` and `remote`
    held the game's 17 ms limiter floor, `direct` ran at 33–35 ms (HISTORY.md, 17 Oct, the run ledger).
  - It works under the game's unreconstructed code, so it does not wait for the source.
  - What is open: its pictures differ from dgVoodoo's in 8,689 pixels of one capture, mostly UI text
    (REEVALUATION.md section 8), and no run has compared it with dgVoodoo within one session.
  - DXVK (D3D8 from 2.4) and d3d8to9 are references for D3D8 behaviour, and a second opinion in picture comparison.
    They are not a replacement: the port's gains (draw merging, asynchronous occlusion, GPU skinning) need knowledge
    of the game's draws, which a generic wrapper lacks (D3D11.md section 4, "What this says about the designs").
- **Identical matching is required where it pays.** That means the render path and the functions the speedfix
  changes. Elsewhere, a gate PASS is enough to build on. A PASS is evidence on the inputs tried with callees
  stubbed, not proof, and the status list says which of the two each function has.
- **Progress uses one measure.** reccmp's annotation format and reports are to be tested against our VC6 output.
  Not known yet: whether reccmp handles VC6, since its page cites MSVC 4.20.
- **Matching is spread across workers.** Each worker takes a batch with no shared files; this session
  integrates. Their number is the owner's decision. Published accounts of agent-assisted matching show fast early
  gains, then a stall on the hard tail.

## Where it stands (sources)

| Measure | Value | Source |
|---|---|---|
| Frame-set functions (game code) | 4,984 (4,765) | the frame-set closure data, task #86 |
| In the verified tree | 4,280 functions, 4,279 PASS, 1 INCONCLUSIVE (0x10003890) | tree/README.md, verification/gate_clean_final.jsonl |
| Unresolved indirect call sites | 1,929 in 484 functions | task #88 working data |
| Byte-identical (sweep of the tree as written) | 1,433 of 4,121 (1,529 DIFFERS, 582 NO_COMPILE, 577 LEFT_OUT) | verification/vc6_sweep.json |
| Hand-matched | 14 functions | recompile/small_math.cpp |
| D3D8 surface | 57 functions with proven device calls (34 in the tree), 370 proven call sites | data/ facts, kit facts=1 |
| D3D11 device | d3d8on11 milestones 1–2 and the ring, Wine-tested; runs the game (plaza, house) | kit/d3d8on11/README.md, HISTORY.md 17 Oct |
| In-game tests | T2 done (readbacks cost about 10.5 ms a frame, runs 8980/8452); T1 done (the smaller size was about 3 ms faster); T3 (`null_draws`) not run | HISTORY.md 18 Oct |
| Objects whose compiler is not reproduced | 189 (stamp 10/11 build 9782) | docs/RECOMPILE.md |

## The steps, in 5%

Each step has a "done when". A step is done only when its check passes and the result is committed.

### Phase 1: source (0–40%)

| % | Step | Done when |
|---|---|---|
| 5 | Tree current | The round-4 tree is installed and committed; facts at their fixpoint (regen_facts.py, no change in a pass); verify_all clean apart from the listed problems. **Done 26 Oct:** every fact re-derives; the tree regenerates to the committed one; export-sp's 0x100412B0 is PROBLEMS P5 |
| 10 | Frame set closed | The switch-table and arg_reads rounds are in; indirect calls are resolved statically or from kit recordings; one per-function status file for the frame set (in tree / PASS / identical / unresolved, with the reason). **26 Oct:** rounds in, status file built. 1,756 indirect calls unresolved (proven D3D8 calls no longer counted), 185 of them observed in game, every observed target already in the set. The rest waits on the owner's session A (kit/sessions/pending_26oct) or a static proof of the object's class |
| 15 | Whole frame set in the tree | Every frame-set function PASSes or has a named reason it cannot yet (UNRESOLVED call, FAIL with cause) |
| 20 | Render core identical | The 57 device-calling functions and their direct callers compile to the game's bytes |
| 25 | Render objects typed | CDx, model/skin/skeleton, actor, camera, font and zone types are declared structs in the tree; no `field_XXX` on the render path; each field's type has a named source |
| 30 | Frame set mostly matched | At least 60% byte-identical; the rest typed and gated |
| 35 | Data formats | Model, texture, effect, scene and shader formats documented from the code that reads them, with readers in tools/ |
| 40 | Standalone render core | The render core builds from source and calls the original DLL only through one listed boundary (OS, CRT, network, file IO) |

### Phase 2: devkit (40–55%)

| % | Step | Done when |
|---|---|---|
| 45 | Whole-engine swap | The render core swaps in as one block; the whole-frame differential (task #65) matches on every dump and in game |
| 50 | One definition set | Names, types and addresses come from one source shared with the teammate's UI kit; fork, gate and match workflows are in the SDK |
| 55 | Devkit release 1 | Build, swap, profile, picture compare, capture and replay are documented and tested on Windows, Wine and the owner's Mac path |

### Phase 3: D3D11 (55–75%); its first half runs in parallel with phases 1–2

| % | Step | Done when |
|---|---|---|
| 60 | D3D8 boundary in source | Every device call in the render core goes through one backend interface; the D3D8 backend reproduces the original frames in picture comparison |
| 65 | d3d8on11 in game | The plaza draws as dgVoodoo does within the comparison's tolerance (the 8,689-pixel UI text difference found and fixed), and its frame time is compared with dgVoodoo's on the same scene (D3D11.md phase 1 "done") |
| 70 | Full frame on D3D11 | Every MEASURE scene renders; every difference is declared and reviewed; the ring's game-thread time is measured (phase 2 "done") |
| 75 | Game-aware D3D11 | Merged character draws, asynchronous occlusion queries and batched text run as gated forks, each A/B'd (phase 3) |

### Phase 4: performance (75–90%)

| % | Step | Done when |
|---|---|---|
| 80 | Baseline | T1–T3 answered; P0's table of where the frame goes on the owner's machine and one other machine |
| 85 | Draw and CPU cost | Each of ROADMAP P1–P3 that the baseline ranks is done at the source, gated, with an A/B in game |
| 90 | Pacing | Above 60 fps with the game's clock checked against real time (ROADMAP P4), frame-step generalized |

### Phase 5: final (90–100%)

| % | Step | Done when |
|---|---|---|
| 95 | Speedfix on the source | Every speedfix lever is a source-level option; a regression suite of dumps, captures and pictures |
| 100 | Final devkit | Stable mod API, docs, examples, packaging; a path to carry the manifests to a new FFXiMain build |

## Speeding it up (25 Oct; done 26 Oct)

Most of the time went to work done one piece at a time, or redone when nothing changed, on one 4-core container.
What was done, and what it measured:

1. **Fact regeneration reruns only what changed** (regen_facts.py, 63f58a6). Each tool's reads are recorded by an
   audit hook; tools run concurrently by recorded dependencies, within 80% of the memory available. A full
   recording pass took 34 min; the sp_solve round, which changed most facts, reached its fixpoint in 24 min.
2. **The sweep compiles in parallel and caches** (4c1a60e): 22 s cold, 3 s cached, verdicts unchanged.
3. **Matching rules become bulk rewrites.** Six rules, each kept only where match.py says IDENTICAL: 158
   functions. `permute.py` runs decomp-permuter on the C functions with Visual C++ 6.0 (b410974); in its first
   batch about a third of the functions tried were proven in 90 s each. Its results give the game's bytes, not the
   original source.
4. **Work follows the status file** (verification/frame_set_status.json, frame_status.py).
5. **The owner's runs are batched** (kit/sessions/pending_26oct): facts and captures, T3, and frames from both
   devices in one sitting, with a collect script.
6. **Fewer stalls of my own.** Tool changes are made and tested in a separate worktree while long runs use the main
   one. Long runs log their progress (regen_facts prints each tool as it ends).
7. **More machines** (docs/WORKERS.md) when the owner says how many. Not started.

## Who does what

- **This session** integrates: analysis tools, data/ facts, tree regeneration, gating, promotion, verify_all and
  commits. These feed each other and need one owner.
- **Workers**, if the owner starts them, each take a batch with no shared files:
  - matching (`recompile/*.cpp`, proven by match.py);
  - types and names per class (`data/names/*.json`);
  - d3d8on11 work in kit/d3d8on11.
- **The owner** runs everything in game:
  - recordings with facts=1 (MEASURE.md Block C);
  - T3 (T1 and T2 are done);
  - d3d8on11 runs and A/Bs;
  - coordination with the teammate.

## Risks

- A game update moves every address. The kit's build translation exists, but each update costs work.
- The 189 objects of build 9782 cannot be matched with the compiler at hand. They stay gate-verified only, unless a
  copy of that compiler turns up.
- Only Wine runs here. Anything the game decides on the owner's driver is known only from the owner's runs.
- The matching tail: by others' accounts, a share of functions resists every attempt.

The task list for this plan is kept in the session's task tracker; docs/HISTORY.md records what each step
delivered.
