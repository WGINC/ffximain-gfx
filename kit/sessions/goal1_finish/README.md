# Finishing goal 1's research: three runs

What is still unanswered for the performance patch, and what each run answers. Each run's settings are in its
folder; copy that folder's `xikit.ini` and `speedfix.ini` over the ones in the PlayOnlineViewer folder (keep your own
aside if you want them back). Every run writes into a new log folder (`log_folder=1`); send that folder.

| Run | Where | Time | Answers |
|---|---|---|---|
| 0. Probes | Game closed | 5 min | Does dgVoodoo's d3d8.dll keep state blocks as Microsoft's reference says (block_cache rests on it)? What shader limits does it offer (GPU skinning, goal 2)? What does the game's occlusion readback pattern cost on it, and what would an answer a frame late cost (goal 2)? |
| 1. Levers | Port Jeuno plaza, standing | about 20 min | Frame time with and without `block_cache`, `skin_merge` and `extended_state_cache`, each measured on its own with every other lever on |
| 2. Check | Plaza, standing 5 min, then walking through the crowd 5 min | about 12 min | Do speedfix's levers change the picture? Where does the frame's time go with the whole patch on? What does the occlusion readback's unresolved call pop (the one fact that keeps it out of the verified code)? |

## Run 0: probes (game closed)

1. Copy the `d3d11_phase0` folder's files into the PlayOnlineViewer folder.
2. Run `run_phase0.bat`. Windows open and close by themselves.
3. Send the `phase0_...` folder it makes.

## Run 1: levers (`1_levers`)

Every lever measured so far that helped is on: `sse_skinning`, `batch_up_draws`, `lazy_state`. So are the ones not
yet measured: `skin_merge`, `block_cache`, `extended_state_cache`, `vs_dedupe`. speedfix then switches each of the
three in `ab_levers` off (A) and on (B) by itself, one after another: six phases of 30 s each, about 3 minutes a lever.

1. Start the game and go to Port Jeuno's plaza (the busy part).
2. Stand still and leave the game alone. The A/B starts 60 s after the start.
3. When `xikit.log` says `every group of ab_levers has run`, close the game and send the log folder.
4. While it runs, watch the characters and effects for anything that flickers between two looks.

## Run 2: check (`2_check`)

The same levers, all on, no A/B. The kit takes two alternating traces with pictures: frames with the kit's and
speedfix's hooks on and off alternate, and the pictures show whether the levers change what is drawn. It also
samples where the time goes, and measures the readback call (`facts=1`), which only happens while characters come
into view.

1. Start the game and go to the plaza.
2. Stand still for the first 5 minutes. The traces run 90 s and 210 s after the start; `xikit.log` says when each
   is written.
3. Then walk through the crowd for 5 minutes, turning the camera now and then.
4. Close the game and send the log folder. It is large: the pictures are about 8 MB each.

## What happens with the results

- Run 0: if `stateblock.apply` says FAIL, `block_cache` stays off whatever run 1 shows.
- Run 1: each lever whose B is faster by more than the spread within a label becomes a default; the others stay off.
- Run 2: `pictures.py` compares the pictures. A lever that changes the picture stays off until that is explained.
  `fact_sites.py ingest` records the readback call's pop count, and the readback function is gated again. The samples
  give the table goal 2 starts from: where the time goes with the patch on.

## Results (13 Oct runs; docs/HISTORY.md "13 Oct runs")

- Run 0: stateblock.record and stateblock.apply PASS. dgVoodoo keeps state blocks as the reference says.
- Run 1: block_cache +1.43 ms, skin_merge +1.36 ms, extended_state_cache −2.53 ms. Each is within its 3.9 to 5.3 ms
  spread between phases of one label, so none becomes a default.
- Run 2 (docs/HISTORY.md "13 Oct run 2"):
  - The pictures show no lever changing the picture in the traced frames.
  - The readback call's pop is 12 (data/game_sp.json).
  - While walking, the occlusion readback's LockRect is 26.8% of the render thread.
  - The owner saw exploding vertices at times not covered by the traces. Their cause is not established.
