# The 11 Nov session: Liminality with the blending between ticks

Two starts on Windows (Parallels), about 12 minutes, all in Port Jeuno. What it settles is listed at the end.

## What the 10 Nov run showed, and what changed

- **Why everything flashed under Liminality's first stage (found in the game's code):** a character's movement code
  (OnMove) and its drawing code pass four values to each other every frame (at +0xB0, +0xB2, +0x6A4 and +0x8E8). For
  example, the drawing code marks a character hidden when its occlusion check finds it covered, and OnMove clears that
  mark at its start. Stage 1 skipped OnMove on the frames between ticks, so the drawing code saw its own marks from the
  frame before and hid the character. That is the flashing. The wrong speeds came the same way: one of the four values
  is how the drawing code knows whether OnMove has run.
- **Fixed:** after each tick, speedfix keeps what OnMove left in those four values. On a frame between ticks it puts
  them back before drawing, so the drawing code sees what it saw at the tick.
- **Stage 2, new:** on every frame, each character is drawn at the blend of where it was at its last two ticks: its
  position and its facing (the facing turns the short way). How far along the blend is set by the time since the last
  tick. After drawing, the tick's own values go back, so the game's logic never sees a blended value.
  The camera's eye and target are blended the same way, just for building the view, except when the game asks for a
  camera snap. A move of 50 or more in one tick (a warp, a zone change) is not blended.
- **Below 60 fps** one tick can be followed by two ticks' worth of movement. The blend then spreads over those two
  ticks, so motion does not jump.
- **What it costs:** what you see is up to one tick (1/60 s, about 17 ms) behind the game's own state. Every way of
  blending between ticks has this delay.
- **Not built yet: the animation of limbs.** The characters' poses still change 60 times a second. Their bodies and
  the camera now move smoothly at any frame rate, but arms and legs step at 60 a second. I have not found where the
  game advances a motion's time. (The one step read I traced in the drawing code is a slow turn of a direction, 1/32
  a tick. It holds still between ticks, exactly as the game does at 60 fps.) This build logs every read of the game's
  frame step, by the code that made it and by whether a drawing task or another task ran it. That log says where the
  motion clock is.
- **The zone's one step read in a drawing task** (0x10178BD0, a slow 0-to-1 progress the zone keeps) now gets the
  frame's own fraction of a tick every frame. It therefore adds up real time frame by frame, rather than whole ticks
  only on tick frames. What it controls on screen is not known.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_11nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_11nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is
kept.

### Start 1: Liminality, stage 2 (about 7 minutes)

    start.bat lim

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Does anyone flash, vanish or freeze?**
2. `mark camera`: rotate the camera slowly all the way round, then quickly, 30 s. **Smooth? Any jitter between the
   camera and your character?**
3. `mark run`: run around the plaza, 60 s. **How does your character move: smooth, sliding, stepping? Does the running
   speed look right? Do arms and legs look choppy next to the body's motion?**
4. `mark others`: stand where people walk past, 60 s. **Do other characters glide smoothly, or jump, stutter, slide?**
5. `mark pop`: where people walk in and out of view, 60 s, turning the camera now and then. **How do characters
   appear?**
6. (Optional) anything with effects: a spell, an emote, a mount, an airship arriving. **Do effects flicker or freeze?**
   (In the plaza, every draw of the game's task pass came from the three drawing tasks. With effects that has not
   been measured. If effects flicker, speedfix's log says which task drew them.) Close the game normally.

### Start 2: the build as it is, for comparison (about 4 minutes)

    start.bat base

7. `mark camera`: the same as step 2, 30 s.
8. `mark run`: the same as step 3, 60 s. **Against start 1: smoother, rougher, the same?**
9. `mark others`: the same as step 4, 60 s. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_11nov_results.zip` with your notes on steps
1 to 9.

## What it settles

- **Whether Liminality now draws everything right** (nothing flashing, frozen or at the wrong speed): your notes on
  steps 1 to 6. speedfix's log counts the tasks it skipped.
- **Whether the blending makes motion smooth:** your notes, start 1 against start 2, and the frame log.
- **Where the game advances animation time:** speedfix's step-read lines, which list each read of the frame step by
  where it came from. A read that runs in the movement tasks is a candidate for the motion clock; reading the game's
  code at that address comes next.
- **Whether other tasks draw** with effects about: speedfix's task_draws lines.
