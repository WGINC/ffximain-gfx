# The port's first runs in the game: our d3d8.dll against dgVoodoo

kit/d3d8on11 is our own Direct3D 8 device on Direct3D 11 (its README). Under Wine in the build container it draws
what Wine's own d3d8 draws, in all three of its modes. It has never run in the game, so these runs answer:

| Run | What | Time | Answers |
|---|---|---|---|
| 1. Capture | Port Jeuno plaza, standing; once with dgVoodoo, once with ours | about 8 min each | Does the game start and draw with ours? Three frames from each, kept whole (every draw and its data), to compare offline pixel by pixel, and to replay the dgVoodoo frames through ours here |
| 2. Timing | Your Mog House, standing; four starts: dgVoodoo, ours `direct`, ours `thread`, ours `remote` | about 5 min each | Frame time on each, measured by the same kit hook on all four; for ours, how long the game thread waited on the executor and how busy the executor was |

speedfix is off in every run (`mods=` empty): these runs compare devices, not levers. The kit swaps no functions
(`swap=0`); it only measures and captures.

## Before you start

1. Keep the files you will change aside: dgVoodoo's `d3d8.dll` (the one in the same folder as `pol.exe`; if yours is
   somewhere else, use that folder for everything below), and your `xikit.ini`.
2. Send `dgVoodoo.conf` (next to dgVoodoo's `d3d8.dll`) with the results: whether dgVoodoo waits for the screen's
   refresh decides whether its frame times can be compared with ours, which does not wait.
3. Keep the game's own window setting the same in every run. Ours always draws in a window; if the game asks for
   full screen it says so in `d3d8on11.log` and draws into the game's window as it is.

**Ours** means: copy `d3d8.dll`, `d8x.exe` and `d8x_arm64.exe` from this kit's `d3d8on11` folder over dgVoodoo's
`d3d8.dll`, and the `d3d8on11.ini` from the run's `d3d8on11_<mode>` folder next to them. **dgVoodoo** means: put
dgVoodoo's `d3d8.dll` back. Our other files can stay: they are only read by our `d3d8.dll`.

## Run 1: capture (folder `1_capture`)

Copy `1_capture/xikit.ini` over yours. The kit captures three frames, 4:00, 4:30 and 5:00 after the game first draws
(its log says `fcap: ... written`). If you are not in place by then, create an empty file `xikit_fcap_now.txt` next to
`xikit.dll` when you are: the next frame is captured.

1. **dgVoodoo.** Start the game, go to Port Jeuno's plaza, stand still. After the third capture, close the game.
2. **Ours, `thread` mode** (`d3d8on11_thread/d3d8on11.ini`). The same: same spot, same camera if you can.
3. Send both log folders, and from the folder of `d3d8.dll`: `d3d8on11.log` and, if it is there,
   `d3d8on11_exec.log`. The captures are large (tens of MB each).

If the game does not start with ours, stops, or draws wrongly: close it, put dgVoodoo's `d3d8.dll` back, and send
`d3d8on11.log` and the log folder. A short description of what was wrong (or a screenshot) helps; that is the
answer this run is for. Skip run 2 until it is fixed.

## Run 2: timing (folder `2_timing`)

Copy `2_timing/xikit.ini` over yours. Four starts, the same each time:

1. Start the game, go into your Mog House, stand still facing the same way, and leave the game alone for 4 minutes.
2. Close the game.

In this order: dgVoodoo; ours with `d3d8on11_direct`; ours with `d3d8on11_thread`; ours with `d3d8on11_remote`.
Before each start of ours, copy that mode's `d3d8on11.ini` next to `d3d8.dll`. After each start, rename
`d3d8on11.log` (and `d3d8on11_exec.log` in `remote` mode) to carry the mode, as `d3d8on11_direct.log`, or they
are overwritten by the next. Send the four log folders and those logs.

The Mog House is used because no other players come into it: frame times from different starts can be compared only
if each start drew much the same thing. How much it still varies from start to start is not known; the spread
between the 600-frame windows of one start is the yardstick until it is.

## What is measured, and where

- `xikit_timing_<pid>.csv` (the kit, every 600 frames): frame time Present to Present (mean, median, p95, max), and
  the time inside the device's Present. The same hook on dgVoodoo and on ours: it gives the device object its own
  copy of its vtable. Tested on ours under Wine: the kit hooks it and writes its timing (kit/xikit/test, `realdev_ours`),
  and the kit's frame capture works on it (kit/fcap/test).
- `d3d8on11.log` (ours, every 600 frames): `ring stats` (`thread` and `remote`): how long the game thread waited for
  room in the ring, for the executor to be at most two frames behind, and for answers (read-backs); MB sent a frame.
  `executor stats`: how long the executor was busy a frame. In `remote` mode the executor's line is in
  `d3d8on11_exec.log`, with the native processor it ran on (`built for ARM64` is the ARM mode).
- The captures (`xikit_fcap_<pid>_<n>.fcap`): the frame's calls and data, and the picture each device drew.

## What happens with the results

- The dgVoodoo captures are replayed here through ours (`kit/fcap/replay.c`) and compared pixel by pixel with the
  picture dgVoodoo drew (`compare.py`). Each difference is traced to a call and explained against the DirectX 8
  reference, or fixed. The captures from ours show what ours drew in the game.
- `d3d8on11.log` lists every Direct3D 8 call the game made that ours does not implement. Each is implemented, or
  shown not to matter, before anything else.
- Run 2 decides the default mode, and whether the port is faster than dgVoodoo at all (docs/D3D11.md phase 2's
  measure). If no mode is faster, that is the result, and the case for the port rests on what dgVoodoo cannot do
  (an occlusion answer a frame late, skinning on the GPU), not on speed.
