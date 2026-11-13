# The 19 Nov session: round shadows everywhere, what stutters with advanced shadows, and frames for the anti-aliasing

Three starts of the game, about 10 minutes.

## What your 18 Nov run showed

- **Round shadows:** speedfix's log sized every simple-mode shadow it saw (footprints 0.93 to 2.99 units, none falling
  back). The game has one more branch that draws the same round shadow at the old shared small size: in its other
  shadow mode, for a character whose shadow caster is 20 or more away (0x10033E94). That is very likely the fallback
  you saw. That branch is sized now too, and the log counts every branch, so the next log says where each shadow went.
  `shadow_fit=1` is now on by default and written into speedfix.ini (`shadow_fit=0` gives the game's own sizes).
- **Advanced shadows stutter:** this place was much busier than last time: the shadow texture was drawn into up to 260
  times a frame (17 last time). The "lazy depth" change did nothing here: I had misread the game's call; it binds no depth
  surface for those passes at all (0x10033800 passes 0 as the depth surface), so there was nothing to save. Something new also showed up: about 4.7 to 6.5 ms a frame (up to 50) where the game reads a picture
  back from the GPU and waits for it. Which game code does that is not known; this build logs every such read by the
  game's call site (`readbacks by call site` in d3d8on11.log).
- **The in-between frames:** in the busy minutes 535 to 562 of every 600 frames already carried a game tick, so the
  game was at 60 fps or below there and there was nothing to put in between. Above 60 it helps; below, it cannot.
  The nameplates jitter because they are not blended (they are drawn where the game has them, the characters one tick
  behind). Not in this session; see my reply.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_19nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_19nov`.

## 1. Round shadows (`start.bat d12`)

Simple shadows. A busy place, `mark simple`, 1 minute, looking around. **Any shadow still at the small size?** Close
the game, `restore.bat`.

## 2. Advanced shadows on everyone (`start.bat adv`)

The same busy place as yesterday's adv. `mark adv`, 2 minutes, standing still. Close the game, `restore.bat`.

## 3. Frames for the anti-aliasing (`start.bat aa`)

Your usual settings. Go somewhere with characters and their names at medium to long range, where you find text hard
to read and edges shimmer. Stand still, then in the Command Prompt: `capture.bat far` (the game pauses a moment). Turn
to another such view: `capture.bat far2`. Close the game, `restore.bat`. Each capture is one whole frame (tens of MB),
which I can replay here exactly as your game drew it, to see which draws the text and the distant edges are and how
they are filtered.

Then close the xiload window, `collect.bat`, and upload `d12_19nov_results.zip` (it will be larger than usual) with
your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
