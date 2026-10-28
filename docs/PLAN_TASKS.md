# The plan's tasks and their state (27 Oct, late)

Every task of the plan as tracked in the sessions, done and not done. The percent tag is the point of the plan
(docs/PLAN.md) the task belongs to; `[parallel N%]` tasks run alongside the main line; `[speed]` tasks make the tools
faster; `[later]` tasks are outside the plan's line. Untagged tasks (#50 to #98) came before the percent tags.
Numbers are the session's task ids; #65, #80 to #83 and #85 were merged into others.

States: **done**; **partly** (worked, not finished; what is left is said); **open** (not started); **owner** (needs the
owner or a run in the game).

## Percent-tagged tasks

| # | Tag | Task | State | Notes |
|---|---|---|---|---|
| 99 | 5% | Commit and push the round-4 tree install | done | |
| 100 | 5% | verify_all --quick on the installed tree | done | |
| 101 | 5% | Update ROADMAP/HISTORY to point at PLAN.md | done | |
| 102 | 10% | Switch-table round: generalized proven bound walk | done | |
| 103 | 10% | arg_reads round: must-store analysis and tail jumps | done | |
| 104 | 10% | Register bound across joins (0x100DA750) | done | its first switch's bound stays unproven (P19) |
| 105 | 10% | Ingest the owner's facts=1 recordings (out, jmp, call) | done | 1,717 call sites, 21 switch jumps, 254 output slots (385687e) |
| 106 | 10% | Resolve indirect calls: 1,929 sites in 484 functions | done | data/indirect_targets.json by source; the frame set follows recorded targets (ef81097) |
| 107 | 10% | Per-function frame-set status file | done | verification/frame_set_status.json |
| 168 | 15% | P17: pushed-argument counts from the caller's stack; the gate compares them | done | |
| 169 | 10% | sp_solve round: drop the "no ret N = never returns" assumption | done | +3,820 solved calls |
| 108 | 15% | Fix the 23 remaining fs14 FAILs | done | 6 fixed; the rest each with its cause or missing fact (PROBLEMS P18) |
| 109 | 15% | Bring the remaining frame-set functions into the tree | done | tree r14: 4,449 frame-set functions in the tree, 4,437 PASS; what is left, by blocker, in docs/FRAMESET_REMAINING.md. Earlier: | fs22 (52 raw PASS) and fs23 in tree r13; fs24's 387 raw PASS wait for tree r14 (HANDOFF 3.1); the rest by blocker in docs/FRAMESET_REMAINING.md |
| 110 | 15% | Clear the PROBLEMS.md open items P1 to P16 | done | re-read 27 Oct: settled, or each states what would settle it (P1, P3, P5, P6, P8, P13, P14 open) |
| 111 | 15% | Settle 0x10003890 INCONCLUSIVE | done | PASS on the live gate of the four dumps (3 returning trials each; HISTORY 27 Oct late) |
| 112 | 15% | Raise gate coverage of hot flag-4 functions from captures | done | the owner's 8 captures replayed: 7 PASS, 1 UNRESOLVED (HISTORY 27 Oct) |
| 113 | 15% | Occlusion readback 0x1006C8B0 into the tree | done | |
| 170 | 15% | P18: read the FAILs and INCONCLUSIVEs of the 26 Oct re-gate | done | 4 translator defects fixed, 9 functions PASS raw (03cf30d) |
| 114 | 20% | Test reccmp against our VC6 output | done | weaker than match.py (docs/RECCMP.md) |
| 115 | 20% | Sweep: classify the 1,529 DIFFERS by cause | done | |
| 116 | 20% | Sweep: fix the 582 NO_COMPILE conversions | done | 603 to 39 |
| 117 | 20% | Sweep: handle the 577 LEFT_OUT forms | done | 419 to 252 |
| 118 | 20% | Match the device-calling functions byte-identically | partly | 39 of 57 at the bytes (verification/render_core_status.json); misses listed in recompile/d3d_round_120.md, _121.md |
| 119 | 20% | Match direct callers of the render core | partly | 51 of 100 at the bytes; the rest listed with their smallest diff |
| 120 | 20% | Identify the build-9782 objects' functions | done | |
| 121 | 20% | Decide parallel matching workers | owner | answered "as many as you need", then "no workers" (27 Oct) |
| 122 | 25% | Type the render objects: CDx, model, skin, skeleton | open | |
| 123 | 25% | Type the actor, camera, font, zone objects | open | |
| 124 | 25% | Type the 810 untyped virtual-call slots | partly | 286 left, each without a definitive source (verification/untyped_vcalls.json); the 242 with one typed in r16 |
| 125 | 25% | Naming round 5: remaining unnamed render-path functions | done | 153 named (42c6c25) |
| 126 | 25% | Re-audit low-confidence names on the render path | open | |
| 127 | 30% | Reach 60% byte-identical across the frame set | open | at 38.2% (2,072 of 5,430) |
| 128 | 35% | Document model and skin formats from their readers | done | docs/FORMATS.md |
| 129 | 35% | Document texture, effect, scene and shader formats | done | docs/FORMATS.md |
| 130 | 40% | Standalone render core build with a listed boundary | open | |
| 131 | 45% | Whole-frame differential | open | |
| 132 | 45% | Swap the render core as one block in the game | open | |
| 133 | 50% | One shared definition set with the UI kit | open | |
| 134 | 50% | Fork, gate and match workflows in the SDK | open | |
| 135 | 55% | Devkit release 1 | open | |
| 136 | parallel 60% | Backend interface for all device calls in source | open | |
| 137 | parallel 65% | Build the trace replayer for d3d8on11 | open | |
| 138 | parallel 65% | Second-opinion references: DXVK d3d8 and d3d8to9 | open | |
| 139 | parallel 65% | Check open D3D8 behaviours against the SDK docs | open | |
| 140 | parallel 65% | Find the 8,689-pixel UI text difference | open | not found yet |
| 141 | parallel 65% | Owner run: d3d8on11 vs dgVoodoo on the same scene, one session | owner | run of 26 Oct taken (A, B under ours); speed and plain-colour defects found; d3d8on11 fixed (fe8bf8d); the next kit asks again (HANDOFF 3.2) |
| 142 | 70% | Every MEASURE scene on D3D11, differences declared | open | |
| 143 | 75% | Merged character draws on D3D11 | open | |
| 144 | 75% | Asynchronous occlusion queries | open | |
| 145 | 75% | Batched text and UI quads | open | |
| 146 | 75% | GPU skinning (evaluate) | open | |
| 147 | 80% | Owner test T3 (null_draws); T1, T2 done | done | run B of 26 Oct, written into docs/REEVALUATION.md section 7: B (no drawing) medians 7.51 to 7.71 ms, A 12.88 to 13.97 ms; the frame without drawing is well below 16.7 ms on that machine |
| 148 | 80% | P0 baseline table on two machines | owner | |
| 149 | 85% | Redundant state removal at the source | open | |
| 150 | 85% | Zone mesh sort and large buffers (fn_1017C9C0) | open | |
| 151 | 85% | Lighting setup cost (fn_100467E0) | open | |
| 152 | 85% | Effects scheduler CPU cost (CMoSchedularTask::Interpret) | open | |
| 153 | 90% | Above 60 fps with the game clock checked | open | RENDERER 3a and 3b: the step is one tick per frame above 60 with the divisor 0 |
| 154 | 90% | Generalize frame_step and add interpolation | open | |
| 155 | 95% | Every speedfix lever as a source-level option | open | |
| 156 | 95% | Regression suite of dumps, captures and pictures | open | |
| 157 | 100% | Final devkit: stable API, docs, packaging | open | |
| 158 | 100% | Game-update path: carry manifests to a new FFXiMain build | open | the kit already maps 6A995428 to 6AC37846 by its manifest's !shift lines |

## Speed and later tasks

| # | Tag | Task | State |
|---|---|---|---|
| 161 | speed | regen_facts: measured read sets, rerun only changed, parallel | done |
| 162 | speed | sweep: parallel compiles and a per-function cache | done |
| 163 | speed | sweep: proven rewrite rules applied in bulk | partly |
| 164 | speed | Test decomp-permuter (or equivalent) on VC6 | done |
| 165 | speed | One owner session kit for every pending in-game run | done |
| 166 | speed | Worktree for tool development; progress logs for long runs | done |
| 167 | speed | Worker instructions for matching batches | done |
| 159 | later | Other architectures: assess static recompilation | open |
| 160 | later | Exploding vertices on new actors | open |

## Before the percent tags (#50 to #98)

| # | Task | State |
|---|---|---|
| 50 | Globals: PS2 DWARF global variables and types; match the easy ones | done |
| 51 | In-game kit: design from research | done |
| 52 | In-game kit: hook DLL swapping reconstructed functions in, with config and safety checks | done |
| 53 | In-game kit: profiler, perf fixes hooks, docs and tests | done |
| 54 | Audit the sent repo against our facts | done |
| 55 | Gate the final tree on fuzz and 4 dumps with coverage | done |
| 56 | Promote the tree and update gate records | done |
| 57 | Rebuild the kit, run kit tests and verify_all | done |
| 58 | Docs, commit, push, deliver the build | done |
| 59 | Gate speed 1: split slow functions' trials across workers | done |
| 60 | Gate speed 2: profile and fix 0x1006E970's live slowness | done |
| 61 | Gate speed 3: cache with coverage, default on | done |
| 62 | Gate speed 4: stable function addresses in the tree build | open |
| 63 | Full gate of the fixed tree (regen11) with the fuzz dictionary; promote, rebuild kit, deliver | done |
| 64 | Fix the register-preserving-callee blind spot in ecx_use/edx_use | done |
| 66 | SDK: trampoline data in the manifest | done |
| 67 | SDK: mod loading and hook API in xikit | done |
| 68 | SDK: headers, template, example, build | done |
| 69 | SDK: Wine tests, docs, commit, deliver | done |
| 70 | Input recorder: xikit capture= (entry snapshot .xcap) | done |
| 71 | Input recorder: gate replay of .xcap | done |
| 72 | Timing: Present wait, per-call time, sampling profiler | done |
| 73 | Picture capture and comparison on alternating trace frames | done |
| 74 | In-game fact recorder: pops and COM methods at call sites | done |
| 75 | Per-draw state snapshot, resource log, shader catalogue | done |
| 76 | SDK: fork workflow, device interception API, mod reload | done |
| 77 | Differential gate with declared differences | done |
| 78 | Fewer character draws: SkinAndDrawV2 strip merging | done |
| 79 | Cheaper D3D8 path on the Mac: environment log, compare tool, path A/B protocol | done |
| 84 | frame_step=1 and uncap as C++ edits to verified code | done |
| 86 | Get the missing frame-set functions into the tree | partly (continued as #109) |
| 87 | Naming round 2 (132 of 132 batches) | done |
| 88 | Frame set: a definitive list (resolve indirect calls) | partly: #106 resolved what has a source; 1,768 calls stay unproven, so the set is a lower bound |
| 89 | Naming round 3 | done |
| 90 | x87 effects per function and per site, from the code | done |
| 91 | Ghidra: x87-keep conventions and ST0/ST1 parameter storage | done |
| 92 | x87: 11 new PASS to promote | partly (folded into later tree rounds) |
| 93 | Fix the 78 that do not compile and the 44 gate FAILs | partly (continued as #108, #170) |
| 94 | sp_solved / cdecl_args regenerated | done |
| 96 | Kit: facts=1 records output slots and switch targets | done |
| 97 | Naming round 4 | done |
| 98 | Byte-identical recompilation: assess and start | done |

Count: of the 63 percent-tagged tasks (5% to 100%, parallel included), 26 are done, 3 partly, 3 with the owner and 31
open.
