# The 15 Nov session: Liminality with the 12 Nov breakage fixed, on the game's new build

Two starts on Windows (Parallels), about 12 minutes. Use this instead of ab_13nov and ab_14nov: the game updated
(build 6AE818B4 of 2 Nov), and those two kits do nothing on it. What it settles is listed at the end.

## The game's new build

- This kit has a manifest for build 6AE818B4 (and still for the two before it). It checks the build at start and
  logs which one it found.
- The update moved most of the game's code and, for the first time, its data. The kit now translates both. Of the
  4,826 game functions the kit knows, 4,746 are proven the same code in the new build; 80 changed (mostly the
  game's own logic, networking and interface code) and are left out.
- Everything speedfix and Liminality touch is proven unchanged: all 206 code addresses speedfix names, its data
  addresses, and the 35 game functions Liminality's facts come from (movement, drawing, motions, effects, camera,
  the frame loop).
- The kit's reconstruction (swap=1) is not used on this build: some addresses it needs changed. Your sessions never
  used it (swap=0), so nothing you run is affected; the log says so in one line.

## What your 12 Nov run showed

- The game's clock was right throughout: speedfix's ticks added up to 100.0% of real time every minute. So the average
  speeds were correct; what you saw was the drawing jumping around, not the game running at the wrong speed.
- The plaza ran at about 45 fps under Liminality. Below 60 fps some frames carry one tick (1/60 s) of game time and
  some carry two, and almost no frame falls between ticks.

## What was wrong (found in the game's code and speedfix's)

1. **The blend's delay changed from frame to frame.** What is drawn should always be one tick behind the game. On
   frames carrying two ticks it was drawn two ticks behind, on the others one, so characters, limbs and the camera
   jumped back and forth several times a second. That explains the flicker when moving, the animations looking too
   slow then too fast, and much of the stutter. Fixed: one tick behind on every frame.
2. **The camera was being put back.** speedfix decided whether the camera had moved from the next frame's timing
   instead of this frame's, and on some frames wrote an old camera position back over the game's. That explains the
   camera getting stuck. Fixed: speedfix now goes by whether the game actually moved the camera this frame, and never
   writes back a camera that something else moved.
3. **Blended positions leaked into the game's logic.** Blended positions stayed in the characters for the rest of the
   frame, where other characters' logic could read them. Fixed: blended values exist only while the game is drawing
   (the characters, the name plates, the sorted draw list), and the real ones go back right after.
4. **Effects, see-through characters and afterimages vanished between ticks** (the 13 Nov fix, also in this build).
   This was the lighting and shadow flicker above 60 fps.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_15nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_15nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is
kept.

### Start 1: Liminality (about 8 minutes)

    start.bat lim

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Anything flickering, vanishing or frozen? Lights
   and shadows steady?**
2. `mark camera`: rotate the camera slowly all the way round, then quickly, 30 s; walk into a corner or a wall. **Does
   the camera stick anywhere?**
3. `mark run`: run around the plaza, 60 s. **Smooth? Speeds right? Limbs smooth and steady?**
4. `mark others`: where people walk past, 60 s. **People walking and fading in or out: smooth? flickering?**
5. `mark fx`: spells, emotes, a mount, an airship, 60 s. **Do effects flicker, freeze or snap?**
6. `mark field`: if you can, step outside the city (fewer people, a higher frame rate), 60 s. **How does it feel
   there?** Close the game normally.

### Start 2: the current build, for comparison (about 4 minutes)

    start.bat base

7. `mark camera`: as step 2, 30 s.
8. `mark run`: as step 3, 60 s. **Against start 1: smoother, rougher, the same?** Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_15nov_results.zip` with your notes on steps
1 to 8.

## What it settles

- **Whether the 12 Nov breakage is gone:** your notes on steps 1 to 5.
- **Whether anything is still missing on frames between ticks:** speedfix logs that part of the frame's draws on tick
  frames and on frames between ticks apart. If the second is lower, something is still missing.
- **Whether Liminality is smoother than the current build,** in the city (about 45 fps) and in the field (above 60):
  your notes and the frame logs.
