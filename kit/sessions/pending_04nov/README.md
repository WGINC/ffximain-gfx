# The render path swapped as one set, and the facts the gate is waiting on (4 Nov)

Use the kit built on 4 Nov or later (VERSION.txt: tree r18). Older kits do not know `swap_set` and would swap the
default set instead.

Two things in one session:
- **The render path as one set (#132).** `swap_set=render_path` swaps 230 of the 239 render-path functions the manifest marks
  (the functions that call Direct3D, their direct callers, and the profiler's hot set), including the ones whose gate
  coverage is incomplete. Before this kit, those stayed on the game's code. The emulated frame with exactly this set
  swapped was compared with the game's own on the four dumps first (docs/HISTORY.md, 4 Nov): 9 of the 239 changed the
  frame there and are left out (`exclude=` in this folder's xikit.ini); the other 230 gave the game's own frame on all four. The traces are in blocks
  of 4 as last time, so each swapped frame has unswapped neighbours on both sides.
- **Facts (`facts=1`).** Six call sites in render functions not yet in the tree have no pop count from any source the
  pipeline accepts; the kit measures them in the running game. Nothing to do for this beyond playing.

## Steps

Copy this build's files over the old ones (`xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_facts.txt`, `VERSION.txt`) and this folder's `xikit.ini` and `collect.bat`.

1. Start the game, log in, go to a town with people (Port Jeuno). Stand still, camera still, from about 1 minute
   to 2 minutes after the start (trace 1).
2. Go somewhere busy (a fight, a crowd) and stand still with the camera still around the 5-minute mark (trace 2).
3. Play on for a few minutes more, anywhere: walk, open a menu, zone once if you can (the facts collect while you
   play).
4. Note anything that looked wrong (fog colour in particular). Close the game normally.

If something looks clearly broken, press F9 (all swaps off) and note the time.

## Sending

Run `collect.bat` from the folder of `xikit.dll` and upload `pending_04nov_results.zip`.
