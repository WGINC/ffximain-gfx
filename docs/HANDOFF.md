# Handoff (27 Oct): finishing the work in one session, without workers

For a session that takes over on the branch `claude/gifted-archimedes-cjtg2d`. Read CLAUDE.md first; its rules hold
for everything below (every fact from a named definitive source, otherwise UNRESOLVED; no model identifiers in commits
or files; commits end with the `Claude-Session:` line only; push only to this branch; no pull request unless asked).

The task list with every task's state: docs/PLAN_TASKS.md.

Work alone: no parallel workers. The steps below are ordered so that each costs little. A full clean gate is the one
expensive step (about 2 to 3 hours on 4 cores); do it once per tree, never for a single change.

## 1. Where things stand (commit 7485ffc and the handoff commit after it)

| Measure | Value | Source |
|---|---|---|
| Tree | r13 installed: 4,513 functions, 4,486 PASS, 19 FAIL, 5 INCONCLUSIVE, 3 UNRESOLVED | verification/gate_clean_final.jsonl |
| Frame set | 5,430 functions; 4,066 in the tree, 4,039 PASS | verification/frame_set_status.json (frame_status.py) |
| At the game's bytes | 1,609 of 5,430 (29.6%); Direct3D-calling 25 of 41 | same; recompile/ counted through verification/recompile_match.json |
| Recompiled by hand | 98 functions in recompile/, all IDENTICAL | `pipeline/vc6/match.py --all` |
| Raw PASS waiting for a tree | fs24: 387 of the 436 functions newly reached through recorded indirect targets | verification/gate_raw_frameset24.jsonl |

The plan's percent-tagged tasks: everything to 10% is done; 15% is done except #109 (this round) and #111; 20% is
done except #118, #119 (byte matching, partway). P1 to P16 were re-read on 27 Oct (docs/PROBLEMS.md).

## 2. What a fresh container lacks (none of it is in the repository, by rule)

| Thing | Where it was | How to get it again |
|---|---|---|
| The game DLL (analysed build 6A995428, unpacked) | owner upload `cca0960a-FFXiMain_unpacked.dll` | ask the owner; `export FFXI_DLL=...` |
| The live build 6AC37846 (for the kit's second manifest) | /home/user/tools/ffximain_oct | the owner's FFXiMain.dll; `FFXI_DLL_6AC37846=...` |
| Ghidra and the analysed project | /home/user/tools/ghidra_deb, /home/user/tools/ghproj | pipeline/ghidra/export.sh header: install Ghidra, `analyzeHeadless $PROJ ffxi -import $FFXI_DLL` (about 20 min) |
| Visual C++ 6.0 SP6 + Processor Pack (the game's compiler) | scratchpad `vc6tc` | `VC6_ZIP=en_vs6_ent.zip VC6=... pipeline/vc6/setup_vc6.sh` (the owner's archive); check `match.py --compid` prints 48/49 build 9044 |
| The four live dumps | /root/ffxi_dumps | the owner (only needed for the live gate, P1) |
| The owner's sampler runs (frame_set.py --samples) | scratchpad: xikit_samples_12764.csv, _11952.csv, _6156.csv | the owner's log folders of 6 Oct (runs gd/x/mutil/1, gd5/x/Gumba/2, run2/xikit_logs) |

Without the game DLL nothing runs. Without Ghidra only new exports are blocked; without VC6 only byte matching is.

## 3. Next steps, in order

### 3.1 Tree r14: fs24's 387 raw PASSes and the regparm fix (one full gate)

The regparm fix (commit 36639d3) is in the generator but not yet in the installed tree: 13 of fs22's functions FAIL
clean on it ("stack cleanup differs: original pops 0, reconstruction pops 4", calls missing). r14 takes it.

```
export FFXI_DLL=... XI_REGATE=verification/gate_raw_regate.jsonl
python3 pipeline/tree_round.py regen  $SCRATCH/tree_r14 --last fs24
python3 pipeline/tree_round.py cases  $SCRATCH/tree_r14 --last fs24      # builds runs/fs24c (translate + cases) if missing
python3 pipeline/tree_round.py gate   $SCRATCH/tree_r14 --jobs 4          # the long step; prints every verdict change
python3 pipeline/tree_round.py install $SCRATCH/tree_r14 --last fs24      # same --first/--last as regen
python3 pipeline/regen_facts.py                                            # about 25 min
python3 pipeline/frame_status.py verification/frame_set_status.json
python3 pipeline/verify_all.py --quick --facts-from-state
```
Before installing, read every verdict change `gate` prints: a PASS that turns non-PASS is read to its cause (the r12 and
r13 rounds found real defects that way: 0x10054D70's missing argument, the regparm rule). Record the round in
docs/HISTORY.md (counts, what changed and why) and commit with the counts in the subject.

Known about fs24 before you start (docs/FRAMESET_REMAINING.md, its last section):
- 0x10223B4E, 0x102262EE, 0x1022635E are switch cases, not functions; they left the frame set (3a37ed3). If they
  appear in r14, drop them (`--drop` on regen and install) and say why.
- 0x100D8B20 (LPLONG) and 0x101CC0D0 (CONCAT14) did not compile; fixed in dd4a499, not re-gated raw.
- 0x100372A0, 0x100372E0: a float parameter typed `undefined4` stored into a float global is converted, not copied
  (the reverse of 5916074's fix). Not fixed.

### 3.2 The owner's kit (after r14)

`kit/xikit/build.sh` (MinGW; needs FFXI_DLL and FFXI_DLL_6AC37846) writes kit/xikit/dist; `kit/speedfix/build.sh`;
`kit/d3d8on11/build.sh`. Then `python3 pipeline/tool_drift.py --fix` and commit the lock. Make a session folder like
kit/sessions/pending_26oct with:
- d3d8on11 with `present_interval=1` (d3d8on11.ini), Windower as the owner runs it (frame divisor 0): does the game's
  speed hold steady (RENDERER 3b)? The run logs the game's present parameters at CreateDevice and Reset.
- d3d8on11 with no present_interval: what the game itself asks for (the log line).
- captures (`capture=`) of the functions docs/FRAMESET_REMAINING.md lists as needing real inputs (the "Capture list"),
  plus 0x100CD540, 0x100E08D0, 0x100CFA50 (P18), and 0x10003890 only if the owner knows a scene that runs it.
Replay captures with `pipeline/xcap.py CAPTURE verification/cases_clean_final.json tree/build/gfx_recon.dll OUT`.

### 3.3 Byte matching (#118, #119), by hand, cheapest first

Rules 1 to 15: recompile/d3d_round_118.md and d3d_round_119.md. Closest misses (lines off): 0x1006A020 (2),
0x10017310 (2), then the list in d3d_round_119.md. Write recompile/d3d_<addr>.cpp or d3dcaller_<addr>.cpp; keep
`VC6=... python3 pipeline/vc6/match.py --all` exit 0; check pooled float constants (`__real@`) against the game's bytes
by hand (match.py does not compare them); then refresh verification/recompile_match.json (one `match.py FILE --json`
per file) and frame_status.

### 3.4 What only the owner or a run can settle

- #111 / P1: 0x10003890 and the other fault-INCONCLUSIVEs need the live gate (`./run.sh cleanlive` on the dumps at
  /root/ffxi_dumps) or captures. P1 also says the kit is built on fuzz-only verdicts; no owner decision is recorded.
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
