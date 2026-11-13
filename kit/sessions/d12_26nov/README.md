# The 26 Nov session: three buffers, and each frame shown at a refresh

Two starts of the game, about 12 minutes.

## What your 25 Nov run showed

- **The 120 fps cap changes nothing on the screen:** within each start, with the cap off and on, the same share of
  frames stayed up one refresh (cap2: 70.7% and 70.1%; cap3: 84.8% and 84.0%). It stays off.
- **Microstutter, and what moved it:** frames taking over twice as long as the ones around them (30-40 ms):
  - two buffers: 46, 127 and 187 a minute in your last three starts with them;
  - three buffers (cap3): 21 a minute, and smaller (22-31 ms).

  With three buffers the renderer's wait to hand a frame over fell from about 6 ms to 0.2-2 ms a frame, and the game
  stopped waiting for the renderer (4.4 ms to about 0 a frame). One start of each, minutes apart, so a difference
  between starts, not a measure inside one.
- **What is left:** with three buffers the game made about 144 frames a second, but only about 102 reached the
  screen, and 14% of those stayed up for two refreshes. Frames arrive in bunches: two within one refresh (one is never
  shown), then none in the next (the old one stays up). On a camera pan that reads as uneven motion.
- **When things load:** the renderer makes mip levels for each new texture, 1.6-4 ms each, 50-150 textures a minute
  while walking into new places. With two buffers the renderer was busy 98-99% of the time, so it had no time to spare
  for them (whether each delayed a frame is not measured). With three it was busy 30-90%. At the login, one texture's levels took 155-176 ms (once a start).

Start vsync hands each frame to the display at a refresh (`present_interval=1`): none dropped, none bunched. If the
game makes 120 a second, every frame gets one refresh. When a frame takes longer, it gets two.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_26nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. Command Prompt **as administrator**, in `d12_26nov`. The same street and walk as before.

## 1. Three buffers (`start.bat free`)

`mark walk`, 2 minutes walking and turning the camera; `mark load`, change zone and walk 1 minute in the new area.
Close the game, `restore.bat`.

## 2. Three buffers, each frame at a refresh (`start.bat vsync`)

The same: `mark walk`, 2 minutes; `mark load`, change zone, 1 minute. Close the game, `restore.bat`.

Then `collect.bat`, and upload `d12_26nov_results.zip`. **Which start's camera felt smoother?**

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
