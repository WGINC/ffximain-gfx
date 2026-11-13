# The 17 Nov session: what advanced shadows cost, simple shadows sized to each character, and the new in-between frames

Three starts of the game, about 12 minutes, at a busy place (the same as last time if you can).

## What your 16 Nov run showed

- **Advanced shadows on everyone (`mark shadows`):** 28.4 ms a frame on average (about 35 fps), against about 9 ms
  before the mark. d3d8on11's own work was about 4 ms a frame; the rest was waiting for the GPU (PresentMon: the GPU
  busy about 29.6 ms a frame at the 95th percentile). So the cost is GPU work.
- **No skin:** choppy, as you saw; GPU skinning stays on.
- **Liminality:** broken as you describe; it is off and stays off (see the reply for what replaces it).
- **Simple shadows shrinking:** the same with dgVoodoo, so it is the game's own behaviour.

## What the game's code says about advanced shadows

Every character's advanced shadow is drawn through **one shared texture the size of the 3D view** (made by
0x10033710 at the viewport's width and height, so probably 3024x1898 with your background resolution). For each
character, every frame, the game switches to that texture, clears all of it, draws the character's silhouette from the
light, switches back to the scene and draws the shadow onto the ground from it (0x10033800, 0x10033DA0,
0x10185CA0). With 30 characters that is 30 clears of a full-screen texture and 60 switches of target. Switching
targets is expensive on Apple's GPUs, whose tiles have to be written out and read back each time. That fits the cost;
this start counts it.

## This build

- d3d8on11 now logs, every 600 frames, how many times a frame the render target changes, and for each target its
  size, draws and clears (`D3D12 render targets` in d3d8on11.log).
- speedfix `shadow_fit=1`: each character's **simple** shadow is sized from that character's own model (the width of
  its box on the ground), instead of the one size the game shares between everyone. The size rule (half-width 0.46 of
  the box's diagonal) is a starting value, not measured: the log records the box sizes it saw, and with those and your
  note it will be set so that a typical character gets about the size the shadow had before it shrank
  (`shadow_fit_scale` in speedfix.ini).

## The new in-between frames (replacing Liminality)

Above 60 fps about half your frames carry no game time, so the characters stand where the game's last tick (1/60 s)
left them, and their motion updates 60 times a second however fast the frames come. The old Liminality changed the
game's own positions between ticks, and the game's logic then ran into them: the jitter, slow animations and
rubber-banding you saw. The new one (d3d8on11 `interp=1`) leaves the game alone. It takes what the game hands the
renderer, each character's bone matrices and its placement in the world, keeps the last two ticks of each, and draws
the frames in between at the blend, one tick (1/60 s) behind the game. speedfix only tells it how far between two
ticks each frame is. Where the game changes a character between ticks itself, or a character jumps 4 units or more in
one tick (a warp), it draws what the game has. Tested here: each case draws exactly the expected picture, on Direct3D
11 and 12. Not covered yet: only the skinned characters (bodies, gear) are blended; spell effects, nameplates and other
objects move at 60 a second as now, so an effect or a name attached to a running character can trail it by up to one
tick.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_17nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_17nov`.

## 1. Advanced shadows on everyone

Set advanced shadows for everyone as you did last time. `start.bat d12`. At the busy place: `mark adv`, stand still,
camera still, 2 minutes. Close the game, `restore.bat`.

## 2. Simple shadows

Set simple shadows. `start.bat d12` again. `mark simple`: the same place, 1 minute. **Do the round shadows now match
each character's size (big characters bigger, small ones smaller)? Too big or too small overall?** Close the game,
`restore.bat`.

## 3. The in-between frames

Shadows as you like. `start.bat interp`. `mark crowd`: the busy place, 1 minute; `mark run`: run around turning the
camera, 1 minute. **Smoother motion than starts 1 and 2? Any jitter, wrong speeds, rubber-banding, parts (weapons, a
head) out of place, characters popping?** Close the game, `restore.bat`.

Then close the xiload window, `collect.bat`, and upload `d12_17nov_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
