# The 6 Nov session: the stopping characters fixed (frame_step=3), and the occlusion readbacks read late

One start on Windows (Parallels), about 20 minutes of play. What it settles is listed at the end.

## What is new, and why

- **The walk/run animation (frame_step=3, second build).** The first run of this session showed characters
  "moonwalking": a slow walk cycle at full speed. The game's code says why (docs/RENDERER.md 3c): the walk/run choice
  and its playback rate come from the model's movement this frame divided by the movement the game expects in whole
  ticks. On frames of 0 ticks (frame_step=1) the rate drops to 0, idle: the 5 Nov flicker. With the movement a
  fraction of a tick (frame_step=3's first build) it was a fraction of the right rate: the moonwalk. Now that division
  gets the fraction too. Run on the game's instructions at 144 fps, the rate is 1.000 on every frame, as at 60 fps;
  the first build gave 0.174, frame_step=1 0 to 0.25 (kit/speedfix/test/test_steploops.py).
- **The camera (frame_step=3, second build).** Two more of the camera's loops (following you, 1/4 and 1/8 of the way a
  tick) counted whole ticks in the first build: on the game's instructions at 144 fps the camera stood still on 42 of
  72 frames. They now take the fraction exactly, as do the camera's turning and height moves.
- **The tick loops (frame_step=3, first build, kept):** the camera following you, each character's model following its
  position and its turning run once per whole tick of 1/60 s; on frames of 0 ticks the model stopped. They get the
  fraction of a tick, exactly.
- **occlusion_late=1.** Measured in the first run: the plaza from 44-49 ms a frame to 22 ms (20-23 fps to 45-46 fps).
  Kept on; `abtest occl` measures it again.
- **The draw time by the game's call site** (`draw_sites=1`, d3d8on11; fixed after the first run, where every draw came
  from speedfix's hooks): what each of the game's draw calls costs
  d3d8on11, for choosing the next rewrite. It is on through the whole start, so A and B segments carry it alike.
- **No call timing.** Every 2.4 s stall of 4-5 Nov came in a run with the kit's call timing on; this session has none.

## Before you start

Unzip into the PlayOnlineViewer folder so that this folder (`ab_06nov`) sits inside it. Copy the zip's `kit` files
next to `xikit.dll` (replace the old ones): `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_measure.txt`, `xikit_facts.txt`, `speedfix.dll`, `xiload.exe`,
`xisamp_arm64.exe`, `xisamp_x64.exe`, `VERSION.txt`. Keep your own `xikit.ini` and `speedfix.ini` aside: the start
overwrites them. The start puts d3d8on11 in place of dgVoodoo's `d3d8.dll` (kept as `d3d8_dgvoodoo_keep.dll`);
`restore.bat` puts dgVoodoo back.

Open a Command Prompt **as administrator** and go to this folder:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_06nov"

## The start (about 20 minutes)

    start.bat

Start the game (Windower as always). Each step: run the command shown in the prompt, then do what it says.

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 30 s.
2. `abtest occl`: stay in the plaza, standing, camera still, until it says `abtest occl done` (about 2 minutes). In
   segments A the readbacks are the game's, in B they are read late. Note if anything pops in or out of view.
3. `mark camera`: rotate the camera slowly all the way round, a few times: 30 s.
4. `mark run`: run around the plaza: 60 s. Watch the characters: do any flicker between two poses?
5. Go to an open field outside town and turn auto-run on (steer in a wide circle). `abtest step`, keep auto-running
   until it says `abtest step done` (about 2 minutes). **Note for each segment (A, B, A, B) whether your character and
   others flicker between two poses, and whether the camera moves smoothly.** A is the 5 Nov setting (flicker), B the fix (no flicker and no moonwalking, if the fix is right).
6. `mark busy`: somewhere busy (a fight with effects, or a crowd): 60 s.
7. `mark menus`: main menu and inventory open, scroll, with some chat on screen: 30 s, then close them.
8. `mark zone`: walk through a zone line, stand 20 s after you arrive.
9. `mark stand`: stand still anywhere, 30 s. Note anything that looked wrong. Close the game normally.

Then `restore.bat` (dgVoodoo back), close the xiload window, and `collect.bat`. Upload `ab_06nov_results.zip` from
the PlayOnlineViewer folder, with your notes on step 5 (which segments flickered).

## What it settles

- **The flicker:** your notes per segment (A against B). The frame log records your character's position in the game
  (frames_player), which moved every frame on 5 Nov too; the drawn model's position is not in it, so your eyes are the
  measure here, and speedfix's log says how many frames of each segment had a fraction of a tick.
- **The occlusion readbacks:** the plaza's frame times A against B (the frame log), and d3d8on11's own lines: how many
  readbacks it answered from its copies, its readback and Present times.
- **The game's time above 60 with frame_step=3:** your distance per real second from the position each frame, as on
  5 Nov (5.955-5.981 then).
- **Frames per step** with everything on, without the kit's call timing.
- **Which of the game's draws cost d3d8on11 the most** (new): every 600 frames d3d8on11's log lists its draw time by the
  game's call site (the return address of each draw into FFXiMain.dll), and the reader names each site's function. That
  says, before any of it is built, whether the characters' draws, the zone's, the UI text or something else is the next
  thing worth rewriting.
