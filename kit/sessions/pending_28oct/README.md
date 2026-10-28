# The game's speed under d3d8on11, and five captures (28 Oct)

Three starts, about 35 minutes in all. Each answers something only the game can (docs/PLAN.md, "Who does what").
Each start writes its own log folder (`log_folder=1`). Nothing of ours replaces the game's code (`swap=0`).

| Start | Settings | Where | Time | Answers |
|---|---|---|---|---|
| A. The game's own interval | `A_game_interval/xikit.ini`, `A_game_interval/d3d8on11.ini` | Port Jeuno plaza, then walk | about 10 min | What presentation interval the game asks for (the log line at CreateDevice and Reset), and the game's speed with d3d8on11 honouring it |
| B. Interval 1 | `B_interval1/xikit.ini`, `B_interval1/d3d8on11.ini` (`present_interval=1`) | the same | about 10 min | Whether the game's speed holds steady when every Present waits for the display (docs/RENDERER.md 3b) |
| C. Captures | `C_capture/xikit.ini`, with dgVoodoo | anywhere, then play | about 15 min | Real calls of the five tree functions no gate has settled |

Run A and B under Windower as you always run it (frame divisor 0), speedfix off.

## Before you start

- Copy the kit's files next to `xikit.dll`: `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
  `xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`, and the `d3d8on11` folder's `d3d8.dll`, `d8x.exe`
  and `d8x_arm64.exe` (for A and B).
- Keep your own `xikit.ini`, `speedfix.ini` and dgVoodoo's `d3d8.dll` aside.

## A. The game's own interval

1. Put d3d8on11's `d3d8.dll`, `d8x.exe` and `d8x_arm64.exe` in place of dgVoodoo's `d3d8.dll`, and
   `A_game_interval/d3d8on11.ini` next to them. Copy `A_game_interval/xikit.ini` over yours.
2. Start the game. `d3d8on11.log` should have a `CreateDevice: the game asked ...` line.
3. Go to the Port Jeuno plaza. Stand for a minute, then walk a lap of the plaza and turn the camera.
4. Note whether walking, the camera and animations ran at their normal speed, too fast, or changing.
5. Close the game normally.

## B. Interval 1

1. Copy `B_interval1/d3d8on11.ini` and `B_interval1/xikit.ini` over the ones from A.
2. The same as A, steps 2 to 5. `d3d8on11.log`'s line should end `(present_interval in d3d8on11.ini)`.

## C. Captures

1. Put dgVoodoo's `d3d8.dll` back. Copy `C_capture/xikit.ini` over yours.
2. Start the game. `xikit.log` should list five captures (`capture 0x... : 2 call(s), from 120 s after the start`).
   A function the game does not call in your play is not captured; the log says which were.
3. **From about 2 minutes in, the game may freeze for a moment, up to ten times.** Each freeze is one capture.
   If it freezes for more than a minute, or crashes, close it and send the log folder anyway.
4. Play for about 15 minutes: a town with people, the menus, a zone change, a fight with effects.
5. Close the game normally.

## Sending

Run `collect.bat` from the folder of `xikit.dll`. It zips every log folder of today, `d3d8on11.log` and
`dgVoodoo.conf` if present, into `pending_28oct_results.zip`. Add a line on what you saw of the speed in A and B.

## What comes of each

- **A and B:** the log line says what the game asks for; xikit's timing log (`xikit_timing_<pid>.csv`) gives the frame
  times. With B's frames held to the display, step-driven motion should advance at the game's own rate (RENDERER 3a);
  if it still changes with the frame rate, the cause is not the presentation interval.
- **C:** each capture is replayed with `pipeline/xcap.py CAPTURE verification/cases_clean_final.json
  tree/build/gfx_recon.dll OUT`, comparing the original and the tree on the real call. The five functions:
  0x100289F0, 0x100CD540, 0x100CFA50 (INCONCLUSIVE: the original faults or runs out of instructions on every fuzzed
  trial and on the four dumps), 0x100E7B20, 0x10222E20 (UNRESOLVED on fuzz and on the dumps).

Not captured, because the kit captures only functions of its manifest (the tree): the frame-set functions outside
the tree that docs/FRAMESET_REMAINING.md lists as needing real inputs. Capturing those needs the kit to hook an
address outside the manifest, which it does not do.
