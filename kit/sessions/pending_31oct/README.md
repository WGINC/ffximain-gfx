# Two A/B traces with the swaps (31 Oct)

One start, about 10 minutes. It answers what only the game can: whether the game draws the same with the
reconstruction swapped in as without it, on your real device, and where the world pass ends (#131). The emulator
already found the world pass identical on three of the four dumps (docs/HISTORY.md, 31 Oct); this is the same
check with real Direct3D answers.

## Before you start

- Keep your own `xikit.ini` aside.
- Copy the kit's files next to `xikit.dll`, over the old ones: `xikit.dll`, `gfx_recon_ingame.dll`,
  `xikit_manifest.txt`, `xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`.
- Copy this folder's `xikit.ini` and `collect.bat` next to them.
- Your usual setup otherwise (dgVoodoo's `d3d8.dll`, Windower). Speedfix off.

The settings: `swap=1` (the reconstructed functions run in place of the game's), `trace=1`, the first trace 90
seconds after the start and the second 240 seconds after that. A trace records 17 frames with the swaps flipped
every frame (off, on, off, ...); the game may stutter for a second while it is written.

## Steps

1. Start the game and log in. Go somewhere in a town with people around (Port Jeuno, say).
2. Stand still, camera still, from about 1 minute after the start until 2 minutes after: trace 1 is taken then.
   `xikit_logs\<today>_...\xikit.log` gets a line `trace: written ...xikit_trace_<pid>_1.txt`.
3. Then go somewhere busier: a fight, or a crowded spot, with effects. About 5 minutes after the start trace 2 is
   taken there; keep the camera still for that minute too.
4. Note anything that looked wrong on screen while playing (with the swaps on all the time between traces).
5. Close the game normally.

If the game crashes or the picture breaks, close it and send the logs anyway; that is a finding too.

## Sending

Run `collect.bat` from the folder of `xikit.dll`. It zips today's log folders into `pending_31oct_results.zip`.
Upload that to the Drive folder, with a line on what you saw.

## What comes of it

`python3 kit/d3dprof/trace_diff.py xikit_trace_<pid>_<n>.txt --part world` and `--part after` compare each swapped
frame with its unswapped neighbours, the world pass and what follows it separately, and print where each frame's
world pass ended. ffxi_world_draw's end-of-pass rule (docs/RENDERER.md section 9) holds if that is the pop's call
(0x1000A2F9) in every frame without swaps.
