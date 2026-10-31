# Handoff (27 Oct): finishing the work in one session, without workers

For a session that takes over on the branch `claude/gifted-archimedes-cjtg2d`. Read CLAUDE.md first; its rules hold
for everything below (every fact from a named definitive source, otherwise UNRESOLVED; no model identifiers in commits
or files; commits end with the `Claude-Session:` line only; push only to this branch; no pull request unless asked).

Read docs/EFFICIENCY.md first: how to work cheaply (no workers, short sessions, rules before functions).

The task list with every task's state: docs/PLAN_TASKS.md.

Work alone: no parallel workers. The steps below are ordered so that each costs little. A full clean gate is the one
expensive step (about 2 to 3 hours on 4 cores); do it once per tree, never for a single change.

## 1. Where things stand (27 Oct, late)

| Measure | Value | Source |
|---|---|---|
| Tree | r14 installed: 4,897 functions, 4,884 PASS, 8 FAIL, 3 INCONCLUSIVE, 2 UNRESOLVED | verification/gate_clean_final.jsonl (fuzz combined with the live gates) |
| Frame set | 5,430 functions; 4,449 in the tree, 4,437 PASS | verification/frame_set_status.json (frame_status.py) |
| At the game's bytes | 1,612 of 5,430 (29.7%); Direct3D-calling 25 of 41 | same; recompile/ counted through verification/recompile_match.json |
| Recompiled by hand | 110 functions in recompile/, all IDENTICAL | `pipeline/vc6/match.py --all` |
| Byte matching of the render core | #118 30 of 57, #119 26 of 100 | verification/render_core_status.json (pipeline/vc6/render_core_status.py) |

The plan's percent-tagged tasks: everything to 15% is done; 20% is done except #118, #119 (byte matching, partway)
and #121 (the owner's). docs/PLAN_TASKS.md has every task.

## 2. What a fresh container lacks (none of it is in the repository, by rule)

| Thing | Where it was | How to get it again |
|---|---|---|
| The game DLL (analysed build 6A995428, unpacked) | owner upload `cca0960a-FFXiMain_unpacked.dll` | ask the owner; `export FFXI_DLL=...` |
| The live build 6AC37846 (for the kit's second manifest) | /home/user/tools/ffximain_oct | the owner's FFXiMain.dll; `FFXI_DLL_6AC37846=...` |
| Ghidra and the analysed project | /home/user/tools/ghidra_deb, /home/user/tools/ghproj | pipeline/ghidra/export.sh header: install Ghidra, `analyzeHeadless $PROJ ffxi -import $FFXI_DLL` (about 20 min) |
| Visual C++ 6.0 SP6 + Processor Pack (the game's compiler) | scratchpad `vc6tc` | `VC6_ZIP=en_vs6_ent.zip VC6=... pipeline/vc6/setup_vc6.sh` (the owner's archive); check `match.py --compid` prints 48/49 build 9044 |
| The four live dumps | /root/ffxi_dumps | the owner (only needed for the live gate, P1) |
| The owner's sampler runs (frame_set.py --samples) | scratchpad: xikit_samples_12764.csv, _11952.csv, _6156.csv | the owner's log folders of 6 Oct (runs gd/x/mutil/1, gd5/x/Gumba/2, run2/xikit_logs) |

The owner was sent these on 27 Oct as zips with a SETUP.txt (where each goes): ffxi_ghidra_project.zip, ghidra.deb in two
parts, ffxi_vc6tc.zip, ffxi_live_build_6AC37846.zip, ffxi_analysed_6A995428.zip, ffxi_dump_states.zip (the 4 Oct live
states; the 4.4 GB dumps themselves are the owner's own copies). Without the game DLL nothing runs. Without Ghidra only new exports are blocked; without VC6 only byte matching is.

## 3. Next steps, in order

### 3.1 Sweep rules from the hand matches (#163, then #118, #119, #127)

docs/EFFICIENCY.md, "The work": encode the source-form rules of recompile/d3d_round_118.md to _120.md into
`pipeline/vc6/sweep.py`, each kept only where match.py then says IDENTICAL, and run the sweep over the tree in the
background. Then refresh verification/vc6_sweep.json, recompile_match.json and frame_status.

### 3.1a A tree round, when there is a new export or a generator fix (the r14 procedure)

```
export FFXI_DLL=... XI_REGATE=verification/gate_raw_regate.jsonl
python3 pipeline/tree_round.py regen  $SCRATCH/tree_rN [--first fsA] [--last fsB] [--drop ADDR... --why TEXT]
python3 pipeline/tree_round.py cases  $SCRATCH/tree_rN (same flags)
python3 pipeline/tree_round.py gate   $SCRATCH/tree_rN --jobs 4          # the long step; prints every verdict change
python3 pipeline/tree_round.py install $SCRATCH/tree_rN (same flags)
python3 pipeline/regen_facts.py; python3 pipeline/frame_status.py verification/frame_set_status.json
```
Run the four steps as one background script. Before installing, read every verdict change: a PASS that turns
non-PASS is read to its cause. Then the live gate on the dumps for whatever is not PASS (below), and record the round
in docs/HISTORY.md.

The live gate as run on 27 Oct: `live_state.py /root/ffxi_dumps/dumps/ffxi_<scene>.dmp - OUT.json $FFXI_DLL
CASES` per dump (they are of the analysed build), then `FFXI_LIVE=OUT.json gate_some.py DIR all --dll ... --cases
<the non-PASS cases>` (DIR must exist), then `combine_gates.py verification/gate_clean_final.jsonl COMBINED.jsonl
scene=DIR/gate.jsonl ...`.

Open from fs24 (docs/FRAMESET_REMAINING.md, its last section): 0x100372A0, 0x100372E0 (a float parameter typed
`undefined4` stored into a float global is converted, not copied; the reverse of 5916074's fix).

### 3.2 The owner's kit

`kit/xikit/build.sh` (MinGW; needs FFXI_DLL and FFXI_DLL_6AC37846) writes kit/xikit/dist; `kit/speedfix/build.sh`;
`kit/d3d8on11/build.sh`. Then `python3 pipeline/tool_drift.py --fix` and commit the lock. Make a session folder like
kit/sessions/pending_26oct with:
- d3d8on11 with `present_interval=1` (d3d8on11.ini), Windower as the owner runs it (frame divisor 0): does the game's
  speed hold steady (RENDERER 3b)? The run logs the game's present parameters at CreateDevice and Reset.
- d3d8on11 with no present_interval: what the game itself asks for (the log line).
- captures (`capture=`) of the functions docs/FRAMESET_REMAINING.md lists as needing real inputs (the "Capture list"),
  plus 0x100CD540 and 0x100CFA50 (P18; INCONCLUSIVE on fuzz and on the dumps).
Replay captures with `pipeline/xcap.py CAPTURE verification/cases_clean_final.json tree/build/gfx_recon.dll OUT`.

### 3.3 Byte matching by hand: only to find a new rule (docs/EFFICIENCY.md)

Rules 1 to 15+: recompile/d3d_round_118.md, _119.md, _120.md; the closest misses are listed there. Keep
`VC6=... python3 pipeline/vc6/match.py --all` exit 0; check pooled float constants (`__real@`) against the game's bytes
(match.py does not compare them); refresh verification/recompile_match.json and frame_status.

### 3.4 What only the owner or a run can settle

- P1: the live gate ran on 27 Oct for the functions not PASS on fuzz (0x10003890, 0x100E08D0, 0x10010D20 PASS
  combined, the first two on few returning trials). The rest of the tree's verdicts are fuzz-only; whether the kit
  may rest on them is the owner's decision, not recorded.
- P13: the owner's marks in requests/review/02_user_types.md.
- #121: treated as answered by the owner's "as many workers as you need"; the owner has since asked for no workers.
- speedfix's precise_pacing waited 0 ms in run B with [this+0x30] = 1: why is not settled (RENDERER 3b).

## 4. Things that cost time in this round (avoid them)

- `rm -rf tree && cp -r OUT tree` deletes tree/source.json (not a generator output). tree_round.py install keeps it.
- Disk is a fixed allowance (about 39 GB here); old tree builds, wine prefixes, installers and gate piece folders fill it. A full disk makes compiles fail silently mid-build (r12 did): check `df` before a regen.
- A gate's "->" column compares with verification/gate_raw_final.jsonl (raw), not the tree's clean verdicts; compare
  with gate_clean_final.jsonl yourself (tree_round.py gate does).
- A raw PASS can be a clean FAIL: the clean generator (hx2clean.py) differs from the raw one (hx2cpp.py). Gate clean.
- A function that stopped at an import under an older case needs the import stub added (tree_round.py gate does it).
