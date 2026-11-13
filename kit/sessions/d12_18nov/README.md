# The 18 Nov session: simple shadows fixed, the in-between frames fixed, advanced shadows cheaper

Three starts of the game, about 9 minutes, at a busy place.

## What your 17 Nov run showed, and what was wrong

- **Black ground (simple shadows, and the interp start too):** my mistake. speedfix's `shadow_fit` read each
  character's size from the wrong place in memory (0x674 bytes off: the function it hooks is handed the character's
  model, which sits that far inside the character). The sizes it read went up to 151 units, so every shadow covered
  the whole ground, and shadows darken what is under them. The interp start had `shadow_fit` on as well, so its black
  ground was the same fault. Now the place is taken from the game's code (the one caller, and its one caller), and a
  size outside 0.05 to 20 units keeps the game's own size.
- **Jittery, blurry moving characters (interp):** also mine. Between ticks the game itself changes the characters'
  bones too (your log: about as many changes as blends; speedfix's fraction steps are a likely source, not traced). The first build then drew the game's pose on those frames
  and the blend (one tick behind) on the others, so what was drawn went back and forth: jitter and blur. Now every
  frame draws the blend of the last two ticks; a change the game makes between ticks shows at the next tick. Tested
  here: the case "the game changes the pose between ticks" now draws the blend exactly.
- **Advanced shadows (choppy):** measured now. With 17 advanced shadows, the screen-sized shadow texture (3024x1898)
  was switched to 17 times a frame and the scene 22.6 times: 49 target switches a frame. The game draws each
  silhouette with depth off, yet the game binds a depth surface with the shadow texture (0x10033800 passes one; its size is not
  known), which a tile-based GPU like Apple's loads and stores for every one of those passes. This build binds a target's depth surface only when a draw actually uses it
  (no pixel changes: every test scene and 600 random cases come out identical with it on and off). How much it saves
  on your Mac is what step 3 measures. Batching the silhouettes would save more but would change the look (the game
  does not clear that texture between characters, so each character's shadow is drawn from it as it is at that
  moment), so it is not done.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_18nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_18nov`.

## 1. Simple shadows sized to each character

Set simple shadows. `start.bat d12`. `mark simple`: the busy place, 1 minute. **Do the round shadows now match each
character's size? Too big or too small overall?** Close the game, `restore.bat`.

## 2. The in-between frames

Simple shadows still (they are the game's own size in this start). `start.bat interp`. `mark crowd`: 1 minute;
`mark run`: run around turning the camera, 1 minute. **Smoother than start 1? Any jitter, blur, wrong speeds,
weapons or heads out of place?** Close the game, `restore.bat`.

## 3. Advanced shadows on everyone (as on 17 Nov)

Set advanced shadows for everyone. `start.bat d12`. At the same busy place as last time: `mark adv`, stand still,
camera still, 2 minutes. **Smoother than last time?** Close the game, `restore.bat`.

Then close the xiload window, `collect.bat`, and upload `d12_18nov_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
