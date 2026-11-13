# The 20 Nov session: mip levels for distant textures and text, and the occlusion change on test

Three starts of the game, about 15 minutes.

## What changed since 19 Nov

- **Mip levels (anti-aliasing of textures and text at range):** your frame captures showed that every texture the
  game draws has a single level (148 of 148) and no draw uses a mip filter, so far-away ground, walls and names skip
  texels and break up. d3d8on11 now makes the missing levels itself (`mipmaps=1` in d3d8on11.ini): each level the 2x2
  average of the one above, the compressed textures decoded, averaged and compressed again; cutout textures (leaves,
  fences) keep their coverage. Only 3D draws use them, sampled trilinear, with 16x anisotropic filtering where the game
  magnifies smoothly (`anisotropy=16`); the interface and other screen-space drawing are untouched.
  Replayed here through your two captures: the far ground's speckle becomes an even surface; the distant names change
  only slightly (their glyphs are already near their drawn size, so a filter cannot add detail to them). What it costs:
  a level build when a texture is loaded (about 1-2 ms for a 256x256 compressed texture here, natively; the 2048x4096
  font texture about 45 ms when made whole). A rewrite of part of the font texture rebuilds only that part. The log
  says how many builds and how long they took (`mipmaps, 600 frames` and `mipmaps totals` in d3d8on11.log).
- **occlusion_first=2 is on, for testing:** your 19 Nov log put the advanced-shadow stutter at the game's occlusion
  readback (FFXiMain.dll 0x1006C91F), waiting over 100 ms in the busiest minutes. With `occlusion_first=2` an object's
  first probe is answered "visible" without waiting. That answer is a choice, not the game's: for one probe something
  hidden behind a wall may count as seen (a brightness taken from it, e.g. a glare, may be full for a frame). Watch for
  a flash where something appears.
- Round shadows: the projected branch (the one that shrank as you came near) is sized now too (19 Nov).

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_20nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_20nov`.

## 1. Mip levels on (`start.bat mip`)

Your usual settings (simple shadows). Go somewhere with long views: a town street with names at medium to long range,
open ground. `mark mip`, then 2 minutes: stand still and look; walk and turn slowly and watch the distant ground and
walls (shimmer, crawling) and the names. If text is still hard to read somewhere, `capture.bat far` there. Close the
game, `restore.bat`.

## 2. Mip levels off (`start.bat plain`)

The same place and the same views, `mark plain`, 2 minutes. This is the picture as before; compare it with start 1.
Close the game, `restore.bat`.

## 3. Advanced shadows, occlusion A/B (`start.bat adv`)

Advanced shadows on everyone, the same busy place as the last adv runs. `mark adv`, 1 minute, standing still. Then
`ab.bat` and stand still until it says done (about 3 minutes): speedfix switches occlusion_first between 0 and 2 by
itself every 20 seconds and measures the frame times of each. Then walk around for a minute: does anything pop or
flash? Close the game, `restore.bat`.

Then close the xiload window, `collect.bat`, and upload `d12_20nov_results.zip` with your notes: better, worse or the
same at range in start 1 against start 2, and whether start 3 stuttered less.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
