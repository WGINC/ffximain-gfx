# The 9 Nov session: which setting made it smoother, which brought the pop-in, and who draws

Two starts on Windows (Parallels), about 10 minutes, all in Port Jeuno. What it settles is listed at the end.

## What the 8 Nov run showed

- **Start 2 (frame_latency=1 and dynamic_buffers=1 together) was much smoother.** Plaza standing: frames over 33 ms
  from 5.7% to 0.1%; running: from 31.7% to 7.3%, frames over 100 ms from 15 to 0. d3d8on11's Present from 15-16 ms a
  frame to 9-10 ms. The uploads that made Present slow are the game's vertex buffers, ~600 a frame, 3.4-4.4 MB, every
  one changed again each frame (textures ~0); with dynamic_buffers about 630 of them a frame went the new way.
- **But the pop-in got worse** (and fade-in, pop-out, pop-in), most when many characters load at once. Start 2
  changed two things, so which one did it is not known. This session runs each alone.
- **The game's own pacing (abtest base A) stuttered as much or more** than this build (frames over 33 ms 24-34% in A,
  9-14% in B): the plaza's hitches are not caused by frame_step or uncap.
- **The laggier and smoother spells in abtest pop** were its segments: A (occlusion_late off) 38-48 ms a frame, B
  28-35 ms.
- **Interpolation:** almost all of the world's drawing (~1,000 draws a frame, the zone included) happens inside the game's
  task pass, the same pass that runs the characters' movement. This session counts draws by task, to see whether the
  movement code itself draws or separate drawing tasks do.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_09nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_09nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is kept.

### Start 1: dynamic_buffers alone (about 5 minutes)

    start.bat dyn

Start the game (Windower as always).

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s.
2. `mark pop`: stand where people walk in and out of view, 90 s, turning the camera now and then. **Note how
   characters appear: fade in, pop in, or fade in then pop out.**
3. `mark run`: run around the plaza, 60 s. **Smooth or stuttery?** Close the game normally.

### Start 2: frame_latency alone (about 5 minutes)

    start.bat lat

Start the game again, and the same three steps:

4. `mark plaza`: standing, 60 s.
5. `mark pop`: the same place, 90 s. **Note how characters appear.**
6. `mark run`: run around, 60 s. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_09nov_results.zip` with your notes on steps
2, 3, 5 and 6.

## What it settles

- **Which setting made it smoother:** each start's plaza frame times and d3d8on11's Present times against 8 Nov's
  start 1 (neither) and start 2 (both). (The crowd differs between starts; only a clear difference counts.)
- **Which setting brought the pop-in:** your notes, start 1 against start 2.
- **Who draws (for interpolation):** speedfix's task_draws lines: the draws of each kind of task, and whether they
  come from its movement call (+0x20) or its other one (+0x24).
