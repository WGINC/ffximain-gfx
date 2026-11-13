# The 16 Nov session: the shadows, and Liminality on Direct3D 12

Three starts of the game and one look without the kit, about 15 minutes, all at the same place and if you can the same
in-game time of day and weather (please note both for each step).

## What your 15 Nov run showed

- The picture: every test scene identical to Direct3D 11 on your Mac, and you saw it fixed in the game.
- Speed, from the log: 116.6 fps on average over the start; in the crowd 8.97 ms a frame on average (about 111 fps),
  95% of frames under 12.6 ms. Sampler heap changes: 5 to 97 a frame (against 900 to 1,700 draws), so the fix costs
  little.
- The game's clock was right (speedfix's ticks 100.0% of real time every minute).

## What the game's code says about the simple shadows (not yet what you see)

Every simple shadow's size is one number shared by all characters (0x1045B810). It starts at 1.0, and the game then
rewrites it from the bounding box of whichever character it updated last (0x100CBDE0), at least 0.01. A shared number
that changes a moment after you arrive fits "correct size, then a second later all the same small size". Whether the
original game does exactly that, or something of ours makes that bounding box small, is what this session decides.
Advanced shadows: the code draws them projected only while a "caster distance" is under 20 (0x1045B80C); otherwise
the same flat shadow under the character. When that number is over 20 is not traced yet.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_16nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_16nov`.

## 1. Without the kit (your usual dgVoodoo)

Start the game the way you normally do (no start.bat). Go to a place with other players around, where you saw the
shadows shrink. **Do the simple shadows shrink after a second there too? Do advanced shadows cast, or sit under the
characters?** (Switch the shadow setting in the game's config if you need to.) Close the game.

## 2. start.bat d12 (as last time)

`mark shadows`: the same place, 1 minute: watch the simple shadows as you arrive and the advanced ones. Close the game.
Then `restore.bat`.

## 3. start.bat noskin (GPU skinning off)

The same, `mark shadows`. **Any change in the shadows?** Close the game, `restore.bat`.

## 4. start.bat lim (Liminality: in-between frames drawn from the game's own motion)

At over 60 fps about half the frames carry no game time, so they show the same positions as the frame before.
Liminality draws those frames with the characters, limbs and camera blended between the game's last two ticks. Its 12
Nov fixes have not been run yet. `mark crowd`: the busy place, 1 minute; `mark run`: run around turning the camera, 1
minute. **Smoother motion? Any flicker, jumping, wrong speeds, stuck camera?** Close the game, `restore.bat`.

Then close the xiload window, `collect.bat`, and upload `d12_16nov_results.zip` with your notes for each step.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
