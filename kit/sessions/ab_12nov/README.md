# The 12 Nov session: full Liminality (limbs included), against Liminality without limbs and the current build

Three starts on Windows (Parallels), about 18 minutes, all in Port Jeuno. This replaces the 11 Nov session: if you have
not run that one, run this instead. What it settles is listed at the end.

## What is in this build

Liminality runs the game's own logic on whole ticks of 1/60 s, exactly as the game does at 60 fps. Every frame in
between is drawn at a blend of the last two ticks. After drawing, the tick's own values go back, so the game's logic
never sees a blended value.

- **Flashing fixed (since 10 Nov):** the movement code and the drawing code pass four values to each other every
  frame. On frames between ticks, speedfix puts back what the movement code left at the tick.
- **Blended:** each character's position and facing; the camera (except when the game snaps it).
- **New: the limbs.** The game moves each animation forward in the movement code, once per tick: the animation's frame
  goes up by step × playback speed and wraps when it loops. Blends between two animations move the same way. The
  drawing code only builds the pose at that frame. Each animation's frame and blend weight are now drawn at the blend
  of their last two ticks, so arms and legs should move as smoothly as the body.
  - An animation is blended only when what the game did to it at the tick is exactly that one-tick advance (wrapped
    once, or not).
  - An animation that started, jumped or was changed by anything else is drawn as the game has it.
  - Checked against the game's own instructions run in an emulator (12 cases, including wraps and backwards play).
- **The cost:** what you see is up to one tick (about 17 ms) behind the game's own state.
- **Not known yet:** whether effects (spells, emotes, mounts) draw correctly between ticks. In the plaza every draw of
  the game's task pass came from the three drawing tasks that run every frame. With effects that has not been
  measured, and speedfix's log says which tasks drew.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_12nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_12nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is
kept.

### Start 1: full Liminality (about 7 minutes)

    start.bat lim

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Does anyone flash, vanish or freeze? Does your
   idle animation look smooth?**
2. `mark camera`: rotate the camera slowly all the way round, then quickly, 30 s. **Smooth? Any jitter between the
   camera and your character?**
3. `mark run`: run around the plaza, 60 s. **Smooth, sliding, stepping? Running speed right? Arms and legs smooth,
   in step with the body, or twitching?**
4. `mark others`: stand where people walk past, 60 s. **Do other characters walk smoothly? Do their legs match their
   movement?**
5. `mark pop`: where people walk in and out of view, 60 s, turning the camera now and then. **How do characters
   appear?**
6. (Optional) `mark fx`: anything with effects: a spell, an emote, a mount, an airship arriving. **Do effects or
   animations flicker, freeze or snap?** Close the game normally.

### Start 2: Liminality without the limbs (about 4 minutes)

    start.bat limb0

The same as start 1, with the animations as the game has them (they change 60 times a second).

7. `mark run`: as step 3, 60 s.
8. `mark others`: as step 4, 60 s. **Against start 1: are the limbs choppier here?** Close the game normally.

### Start 3: the current build (about 4 minutes)

    start.bat base

9. `mark camera`: as step 2, 30 s.
10. `mark run`: as step 3, 60 s.
11. `mark others`: as step 4, 60 s. **Against start 1: smoother, rougher, the same?** Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_12nov_results.zip` with your notes on steps
1 to 11.

## What it settles

- **Whether full Liminality draws everything right** (nothing flashing, frozen, twitching or at the wrong speed):
  your notes on steps 1 to 6.
- **Whether the limb blending works:** your notes, start 1 against start 2. speedfix's log counts the animations it
  blended and those it left as the game has them, a minute at a time.
- **Whether Liminality is smoother than the current build:** start 1 against start 3, your notes and the frame logs.
- **Whether other tasks draw** with effects about: speedfix's task_draws lines. The step-read lines confirm where the
  game reads its frame step.
