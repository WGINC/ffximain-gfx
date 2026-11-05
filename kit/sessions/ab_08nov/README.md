# The 8 Nov session: where the plaza's frames, hitches and pop-in come from

One start on Windows (Parallels), about 10 minutes, all in Port Jeuno. What it settles is listed at the end.

## What the 7 Nov run showed, and why this session

- **The camera:** fixed (your notes).
- **Frame drops and hitches are in the plaza, not the field.** Running in the plaza, 14% of frames took over 33 ms and
  2.5% over 50 ms; in the field (`abtest step`) 0.1-0.3% and 0.0-0.4%, at 150-180 fps. Two different things:
  - the plaza's steady rate (35-45 fps): d3d8on11's worker spends 12-14 ms of each frame in Present, with 2.7-3.1 ms
    of uploads (as on 6 Nov; Present's time follows the upload time, r = 0.88-0.89 over both 6 Nov runs);
  - the hitches (single frames of 100-300 ms): the game's own render thread busy on the CPU for that long (up to
    140 ms of CPU in one frame), often with thousands of page faults. The game is loading or building something on
    that thread; what it is, and whether the unmodified game hitches the same on your Mac, is not known.
- **Pop-in instead of fade-in:** not explained yet. Two of this build's changes could plausibly touch it: occlusion_late (a
  character's visibility answered one probe late) and the frame-rate changes (uncap and frame_step=3). This session
  turns each off for a few segments, so your notes say which, if either, it is.
- **`abtest cap` (7 Nov) did not test what it was meant to:** the plaza never reached 60 fps, so the 60 cap never held
  a frame back. (A, capped, still averaged 23-24 ms a frame and B 28-29 ms, both times; why the cap made it faster is
  not known.)

## New in this build (logged, nothing to do)

- d3d8on11 splits its uploads into textures and buffers (time, count, MB a frame, and how much goes to things also
  uploaded the frame before): this says what makes Present slow in the plaza.
- speedfix counts the draw calls under each of the game's world passes (the zone, the characters' movement pass, the
  rest): this decides how the frame interpolation can be built.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_08nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_08nov"
    start.bat

Start the game (Windower as always). If the game freezes or crashes, close it and run `start.bat` again before starting
it again (that keeps the last start's d3d8on11 log).

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s.
2. `abtest pop`: stand where people walk in and out of view (near the entrance or the airship gates) until it says
   `abtest pop done` (about 2 minutes). **For each segment (A, B, A, B), note whether characters pop in or fade in.**
   A reads each occlusion result at once (as the game); B as this build.
3. `abtest base`: run around the plaza until it says `abtest base done` (about 2 minutes). **For each segment note
   pop-in, hitches and whether it feels smooth.** A is the game's own pacing (its 30 fps limit, nothing of
   frame_step); B as this build.
4. `mark run`: keep running around the plaza, 60 s.
5. `mark stand`: stand still, 30 s. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_08nov_results.zip` with your notes on steps
2 and 3.

## What it settles

- **Pop-in:** whether it goes away in A of `abtest pop` (then it is occlusion_late) or in A of `abtest base` (then it
  comes with the frame-rate changes), or in neither (then the game does it on this machine anyway).
- **Hitches:** `abtest base` A against B, from the frame log: whether the long frames happen at the game's own pacing too.
- **Present's 12 ms:** the upload split, in the plaza.
- **Interpolation:** whether the characters' movement pass draws anything (speedfix's stats lines).
