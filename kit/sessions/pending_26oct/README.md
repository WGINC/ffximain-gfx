# Everything pending in the game, in one sitting (26 Oct)

Three starts, about an hour in all. Each answers something only the game can (docs/PLAN.md, "Who does what").
Run them in this order. Each start writes its own log folder (`log_folder=1`).

| Start | Settings | Where | Time | Answers |
|---|---|---|---|---|
| A. Facts and captures | `A_facts/xikit.ini` | anywhere, then play | about 35 min | Call pops, output slots and switch targets the code cannot settle (`facts=1`, 3,791 sites). Two real calls each of SkinAndDrawV2, TexObj_Upload, Cell_DrawQuadBlended, Frame_Render and 0x10003890 (the one function the gate cannot decide on fuzzed inputs), for the gate to run their code on real state |
| B. T3, no drawing | `B_no_draws/xikit.ini` and `speedfix.ini` | Port Jeuno plaza, standing | about 10 min | Whether the game's own CPU work fits in a 60 fps frame with nothing drawn (docs/REEVALUATION.md T3; T1 and T2 are done) |
| C. Frames to compare | `C_capture/xikit.ini`, then ours with `C_capture/d3d8on11.ini` | Port Jeuno plaza, standing | about 2 × 8 min | Three whole frames from dgVoodoo and from ours, same spot, for the pixel comparison and the replayer (the UI text that differs in 8,689 pixels) |

Nothing of ours replaces the game's code in A or C (`swap=0`). B loads speedfix with its usual levers, `null_draws`
included.

## Before you start

- Copy the kit's files next to `xikit.dll`: `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
  `xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`, and `speedfix.dll` (for B).
- Keep your own `xikit.ini`, `speedfix.ini` and dgVoodoo's `d3d8.dll` aside.

## A. Facts and captures

1. Copy `A_facts/xikit.ini` over yours. Start the game.
2. `xikit.log` should say `facts=1: 3791 call sites to measure` (or close to it) and list the four captures.
3. **About 2 minutes in, the game freezes for a moment, up to ten times.** Each freeze is one capture: the other
   threads are suspended while up to 80 MB are copied. If it freezes for more than a minute, or crashes, close it
   and send the log folder anyway.
4. Then play for about 30 minutes, touching as much of the game as you can:
   - a busy town, walking through the crowd, turning the camera;
   - the menus (main menu, map, equipment, items, config) and the chat log;
   - a zone change;
   - a fight with spells or abilities that have effects;
   - a cutscene, if one is handy.
5. Close the game normally.

## B. T3, no drawing

1. Copy `B_no_draws/xikit.ini` and `B_no_draws/speedfix.ini` over yours. Start the game, go to the Port Jeuno
   plaza, stand still, leave the game alone.
2. **The screen goes black or freezes every other 30 seconds.** That is the test: in its B phases nothing is drawn.
3. When `xikit.log` says `every group of ab_levers has run` (about 6 minutes), close the game.

## C. Frames to compare

1. Copy `C_capture/xikit.ini` over yours.
2. **dgVoodoo:** start the game, go to the plaza, stand still. The kit captures three frames 4:00, 4:30 and 5:00
   after the game first draws (`fcap: ... written` in the log). Close the game after the third.
3. **Ours:** copy the kit's `d3d8on11` folder's `d3d8.dll`, `d8x.exe` and `d8x_arm64.exe` over dgVoodoo's `d3d8.dll`,
   and `C_capture/d3d8on11.ini` next to them. The same spot, the same camera if you can, the same three captures.
4. Put dgVoodoo's `d3d8.dll` back. Send `dgVoodoo.conf` too.

If ours does not start or draws wrongly, close it, put dgVoodoo back, and send `d3d8on11.log` with the log folder.

## Sending

Run `collect.bat` from the folder of `xikit.dll`. It zips every log folder of today, `d3d8on11.log` and
`dgVoodoo.conf` if present, into `pending_26oct_results.zip`. The captures are large (tens of MB each).

## What comes of each

- **A:** `pipeline/fact_sites.py ingest` turns agreeing sites into facts. The gate then re-runs the functions
  stopped UNRESOLVED at those sites. The captures go to the gate's live mode; it reports how much of each function's
  code they reached. SkinAndDrawV2's fuzzed trials ran 62 of its 1,596 instructions, the plaza captures of session 4 937
(docs/D3D11.md section 5); TexObj_Upload and Cell_DrawQuadBlended have no captures yet.
- **B:** the decision in REEVALUATION.md section 7. If the frame without drawing stays above 16.7 ms, 60 fps is out
  of reach on this machine whatever the renderer does.
- **C:** each frame drawn by both devices and compared offline, pixel by pixel. The dgVoodoo frames are also played
  through ours here.

Not known before the run: whether eight captures in one start are too long a freeze for the game's connection. If
the game disconnects, send what there is; the captures can be split over several starts.
