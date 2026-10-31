# Two traces in blocks (1 Nov, second session)

The same as the last two sessions; only the trace's pattern changes. Each trace now records 20 frames with the swaps
flipped every 4 (off, on, off, on, off: `trace_alternate=4`).

Why: the last traces (4 frames off, then 4 on) left the swapped half always after the unswapped one, so a change in
the scene between the halves could not be told from a change the swaps made, and both traces showed such steps.
With blocks, each swapped frame is compared with the unswapped frames 4 before and 4 after it: both sides of it in
time, and on the same step of the game's 4-frame occlusion schedule. The world-pass split worked in those traces:
every frame cut at the pop's call (0x1000A337), and the UI part after it was identical with the swaps on.

## Steps

Copy this build's files over the old ones (`xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`) and this folder's `xikit.ini` and `collect.bat`.

1. Start the game, log in, go to a town with people (Port Jeuno). Stand still, camera still, from about 1 minute
   to 2 minutes after the start (trace 1).
2. Go somewhere busy (a fight, a crowd) and stand still with the camera still around the 5-minute mark (trace 2).
3. Note anything that looked wrong. Close the game normally.

## Sending

Run `collect.bat` from the folder of `xikit.dll` and upload `pending_01nov_b_results.zip`.
