# The 13 Nov session: Liminality with the flicker fixed

Two starts on Windows (Parallels), about 12 minutes, in Port Jeuno and anywhere with effects. What it settles is
listed at the end.

## What was wrong (your 12 Nov note: light-emitting objects and shadows flicker)

Found in the game's code. The game keeps one sorted draw list, its "ordering table". A part of the game's update
draws it and empties it again every frame. Three kinds of things put themselves in it every frame, from their own
update:

- **effects**: glows, lights, lens flares, spell and particle effects;
- **see-through characters and objects**: anything with transparency (fading in or out, glowing or translucent models,
  ghosts). They are drawn only through that list, with their shadows;
- **afterimages** (motion trails).

Liminality skipped those updates on the frames between ticks, so the list was empty on those frames and they were
not drawn. That made them flicker at the rate of the frames between ticks. (I had told you every draw came from the
three drawing tasks; that was wrong. About 13% of the draws in that part of the frame come from parts the count did
not see.)

## What changed

- **Effects** now run their update on every frame. Their clock is the game's frame step, which is 0 on a frame
  between ticks, so they hold still and are drawn.
- **See-through characters and afterimages:** when their tick put them in the list, speedfix puts them in again on
  the frames between ticks, with the game's own insert, so they are drawn as at the tick.
- **A new check in the log:** speedfix counts that part of the frame's draws on tick frames and on frames between ticks
  apart. If the second is lower, something is still missing. If you still see something flicker, that count shows it
  even if you cannot tell what it is.
- **Not covered yet:** the arms and legs of see-through characters are not blended (they change 60 times a second).
  Everything else blended in the 12 Nov build still is.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_13nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_13nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is
kept.

### Start 1: Liminality (about 8 minutes)

    start.bat lim

1. `mark flicker`: go back to where you saw the light-emitting objects and shadows flicker, 60 s. **Still flickering?**
2. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Anyone or anything flashing, vanishing, frozen?**
3. `mark run`: run around, 60 s. **Smooth? Speeds right? Limbs smooth?**
4. `mark others`: where people walk past, 60 s. **People fading in or out: do they flicker?**
5. `mark fx`: anything with effects: a spell, an emote, a mount, an airship arriving, 60 s. **Do effects flicker, freeze
   or snap?** Close the game normally.

### Start 2: the current build, for comparison (about 4 minutes)

    start.bat base

6. `mark flicker`: the same place as step 1, 60 s.
7. `mark run`: as step 3, 60 s. **Against start 1: smoother, rougher, the same?** Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_13nov_results.zip` with your notes on steps
1 to 7.

## What it settles

- **Whether the flicker is gone:** your notes on steps 1, 4 and 5, and speedfix's draw counts: tick frames against
  frames between ticks.
- **Whether anything else is still missing between ticks:** the same counts. Effects, see-through characters and
  afterimages put back between ticks are counted in the log too.
- **Whether Liminality is smoother than the current build:** your notes and the frame logs, start 1 against start 2.
