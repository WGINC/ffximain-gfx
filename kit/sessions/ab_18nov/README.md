# The 18 Nov session: everything built for frame time, in one kit

Three starts on Windows (Parallels), about 18 minutes. Use this instead of ab_17nov (it has everything that had, plus
the two pieces below). What it settles is listed at the end.

## What is in it

Already measured in your 16 Nov run:
- **The occlusion read that never waits**: the game's wait fell from 1.5 ms a frame to 0.01.
- **The buffer arena**: about 1.4 ms a frame off d3d8on11's worker.

New, tested here but never run in your game:
- **Draw merging** (`draw_merge`): neighbouring draws that differ only in which buffers they read are drawn as one.
  Your 16 Nov run showed each draw costs d3d8on11's worker about 7.7 millionths of a second, at 1,500 to 1,700 draws
  a frame in the plaza.
- **Delta draws** (`draw_delta`): when nothing has changed since the last draw but its buffers, the game's thread
  sends 176 bytes instead of building and sending 3,200. Your run had the game's thread spending 3.3 to 4 ms a frame
  in those calls.
- **GPU skinning** (`gpu_skin`): characters' vertices are computed by the graphics card from their bones. The game's
  thread no longer computes or uploads them. In the tests the result is pixel for pixel the game's, and the numbers
  handed over match the game's own skinning code to within rounding. If d3d8on11 cannot take a model, the game skins
  it as before.

How the picture can differ: only GPU skinning does arithmetic in a different place, so a character's edge can move
by a fraction of a pixel. Merging and delta draws send the same triangles with the same settings.

## The shimmering edges

Not explained yet. Steps 2, 3 and 6 switch one change at a time in a still scene so that you can see which one it
comes with.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_18nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_18nov"

If the game crashes or draws characters wrongly in start 1, close it, tell me what you saw, and go on to start 2.

### Start 1: everything on (about 8 minutes)

    start.bat all

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Characters drawn right (bodies, faces, weapons,
   shadows)? Shimmering edges?**
2. `abtest merge`: stay there, camera still. The window says when draw merging goes OFF (30 s), ON (60 s), OFF
   (30 s). **Does anything change on screen when it switches?**
3. `abtest skin`: stay there. The window says when the GAME skins the characters (30 s) and when d3d8on11 does
   (30 s), twice. **Does anything change: the shimmering, a character's shape, shadows, anything missing?**
4. `mark run`: run around the plaza, turning the camera, 60 s. **Stutters? Smooth?**
5. `mark crowd`: the busiest place nearby, 60 s. Close the game normally.

### Start 2: the picture as it was before 10 Nov (about 4 minutes)

    start.bat plain

6. `mark plaza`, then `abtest nowait`: standing, camera still. The window says when the occlusion read WAITS (30 s)
   and when it does NOT wait (30 s), twice. **Does the shimmering come and go with it?**
7. `mark run`: as step 4, 60 s. **Against start 1: the shimmering, the smoothness?** Close the game normally.

### Start 3: the stutters recorded (about 4 minutes)

    start.bat hitch

8. `mark run`: run around the plaza for 2 minutes. The sampler records what the game is doing in every long frame.
   It costs a little speed. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_18nov_results.zip` with your notes.

## What it settles

- **What each new piece is worth:** d3d8on11 logs its worker's time with merging off and on (step 2) and how many
  draws merged and went as deltas; speedfix logs how many models the device skinned; the frame log gives the frame
  time and the game thread's time with GPU skinning off and on (step 3).
- **Whether any of them changes the picture:** steps 1 to 3, by your eye.
- **Where the shimmering comes from:** steps 2, 3 and 6 each switch one change.
- **What the stutters are:** start 3's samples name the code the game's thread is in during each long frame.
