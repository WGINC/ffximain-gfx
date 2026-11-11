# The 17 Nov session: fewer draws (draw merging), the look, and the stutters

Three starts on Windows (Parallels), about 15 minutes. What it settles is listed at the end.

## What your 16 Nov run showed

- **The occlusion change worked.** The game's wait in the late occlusion reads went from 1.5 ms a frame to 0.01, and
  the longest single wait from 132 ms to under 6.
- **The buffer arena worked, by less than its log line says.** The upload line fell from 2.4 to 0.05 ms a frame, but
  the copying moved into the draw line: the net saving is about 1.4 ms a frame.
- **The plaza was faster with both:** median frame 14.5 ms standing (start 1) against 16.9 (start 3). But start 1 had
  slightly more long frames, which fits your note that start 3 felt smoother.
- **What limits the frame now is the number of draws.** d3d8on11's worker spends about 7.7 millionths of a second on
  each draw (2.9 in the draw itself, 4.9 that shows up in Present), and the plaza has 1,500 to 1,700 draws a frame.
  The game's own thread spends another 2.3 on each.
- **The stutters are not the occlusion test any more.** Standing, the long frames are the game's own thread working
  24 to 31 ms. Running, the thread is stopped for about 28 ms outside Direct3D, and those frames come in the seconds
  with more file reads (correlation 0.55). What it is stopped in is not known yet; start 3 below records it.
- **Start 5 (the ARM64 worker) crashed inside Windower's Hook.dll** about 450 frames in, both times. d3d8on11's and
  the worker's logs show nothing wrong before it. I cannot tell why from the event log. That start is dropped: the
  one earlier measurement (6 Oct) says an ARM64 worker would gain about 3%.

## What changed

- **Draw merging** (d3d8on11 `draw_merge=1`). The game draws each character part with its own buffers, so
  neighbouring draws that are otherwise identical cannot be combined as they stand. d3d8on11 now copies such
  neighbours into shared buffers and draws them as one: the same triangles, in the same order, with the same
  settings. In d3d8on11's tests every scene is pixel for pixel the same with it on and off. An old trace of yours
  (6 Oct) says the plaza's 1,620 character draws could become 510 to 770 this way; how many really merge is what
  this run measures.
- **One fix in the buffer arena**: a case where the game appends to a buffer that an earlier draw of the same batch
  reads is now kept in the game's order. Whether this is what you saw as aliasing is not known.

## A question for you

**What does the aliasing problem look like, and in which starts did you see it?** For example: jagged or shimmering
edges on characters, on the world, or on text; flickering triangles; wrong textures. Your note names start 4 and, I
think, starts 1 and 2; start 3 (neither change) seemed free of it. Steps 2 and 5 below let you watch each change
switch on and off in one scene.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_17nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_17nov"

### Start 1: everything on (about 6 minutes)

    start.bat merge

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Anything drawn wrong? The aliasing?**
2. `abtest merge`: stay where you are, camera still. The window says when draw merging goes OFF (30 s), ON (60 s),
   OFF (30 s). **Does anything change on screen when it switches?** Nothing should.
3. `mark run`: run around the plaza, turning the camera, 60 s. **Stutters? Smooth?**
4. `mark crowd`: the busiest place nearby, 60 s. Close the game normally.

### Start 2: the picture as it was before 10 Nov (about 4 minutes)

    start.bat plain

5. `mark plaza`, then `abtest nowait`: standing, camera still. The window says when the occlusion read WAITS (30 s)
   and when it does NOT wait (30 s), twice. **Does the aliasing come and go with it?**
6. `mark run`: as step 3, 60 s. **Against start 1: the aliasing, the smoothness?** Close the game normally.

### Start 3: the stutters recorded (about 4 minutes)

    start.bat hitch

7. `mark run`: run around the plaza for 2 minutes, as in step 3. The sampler records what the game is doing in every
   long frame. It costs a little speed. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_17nov_results.zip` with your notes.

## What it settles

- **What draw merging is worth:** d3d8on11 logs how many draws merged and its worker's time with merging off and on
  in the same scene (step 2), and the frame log gives the frame time.
- **Whether merging changes the picture:** step 2, by your eye.
- **Where the aliasing comes from:** steps 1, 5 and 6 separate the arena and merging (start 1 only) from the
  occlusion change (switched in step 5).
- **What the stutters are:** start 3's samples name the code the game's thread is in during each long frame.
