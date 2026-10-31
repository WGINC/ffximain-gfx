# Two more traces, in halves (1 Nov)

The same as the 31 Oct session, with one setting changed: each trace records 4 frames with the swaps off, then 4
with them on (`trace_alternate=0`), instead of flipping them every frame.

Why: the 31 Oct town trace showed fewer occlusion queries in every swapped frame. The cause is the game's own
schedule: each character runs its occlusion query only on frames where a counter of the game, taken modulo 4,
equals its bucket (0x100CC58A: `[[0x104568FC]+0x34] % 4` against the actor's +0x9F0). Flipping the swaps every
frame put the swapped frames always on buckets 1 and 3 and the unswapped ones on 0 and 2. In halves of 4 frames,
each half covers all four buckets. This kit also records the world's render target as the surface the game draws
into (the 31 Oct kit recorded the texture holding it, so the world-pass split found nothing to cut at).

## Steps

Copy this build's files over the old ones (`xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`) and this folder's `xikit.ini` and `collect.bat`.

1. Start the game, log in, go to a town with people (Port Jeuno). Stand still, camera still, from about 1 minute
   to 2 minutes after the start (trace 1).
2. Go somewhere busy (a fight, a crowd) and stand still with the camera still around the 5-minute mark (trace 2).
   Standing still matters more this time: the two halves are compared with each other, so motion between them
   shows up as differences.
3. Note anything that looked wrong. Close the game normally.

## Sending

Run `collect.bat` from the folder of `xikit.dll` and upload `pending_01nov_results.zip`.
