# The 6 Nov session: the stopping characters fixed (frame_step=3), and the occlusion readbacks read late

Two starts on Windows (Parallels): start 1 tests the fixes (about 20 minutes), start 2 only measures for the next
rewrites (about 6 minutes), so the measuring cannot change start 1's results. What they settle is listed at the end.

## What is new, and why

- **frame_step=3.** In the 5 Nov run characters flickered between two poses while moving. The game's code says why
  (docs/RENDERER.md 3c): five loops run once per whole tick of 1/60 s -- the camera following you, each character's
  model following its position, and the characters' turning. Above 60 fps, frame_step=1 gives some frames 0 ticks, and
  on those frames the loop does not run: the model stops and does not turn, and its movement that frame is 0, which
  the game's own test reads as standing. Run on the game's instructions at 144 fps, the model stopped on 42 of 72
  frames. frame_step=3 gives those loops the fraction of a tick and the factor that makes it exact; at 60 fps it is the
  game's code bit for bit, and above 60 it ends where the game ends at 60, every frame moving
  (kit/speedfix/test/test_steploops.py).
- **occlusion_late=1.** In the plaza d3d8on11's worker was busy 98% of the time; its readbacks took 15-17 ms a frame
  and its Present followed them (8 ms with them, 1.5 ms without). Each readback is now read at the object's next
  probe, from d3d8on11's copy of it, without waiting.
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

## Start 1: the fixes (about 20 minutes)

    start.bat play

Start the game (Windower as always). Each step: run the command shown in the prompt, then do what it says.

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 30 s.
2. `abtest occl`: stay in the plaza, standing, camera still, until it says `abtest occl done` (about 2 minutes). In
   segments A the readbacks are the game's, in B they are read late. Note if anything pops in or out of view.
3. `mark camera`: rotate the camera slowly all the way round, a few times: 30 s.
4. `mark run`: run around the plaza: 60 s. Watch the characters: do any flicker between two poses?
5. Go to an open field outside town and turn auto-run on (steer in a wide circle). `abtest step`, keep auto-running
   until it says `abtest step done` (about 2 minutes). **Note for each segment (A, B, A, B) whether your character and
   others flicker between two poses, and whether the camera moves smoothly.** A is the 5 Nov setting, B the fix.
6. `mark busy`: somewhere busy (a fight with effects, or a crowd): 60 s.
7. `mark zone`: walk through a zone line, stand 20 s after you arrive.
8. `mark stand`: stand still anywhere, 30 s. Note anything that looked wrong. Close the game normally.

## Start 2: the measuring for the next rewrites (about 6 minutes)

Close the xiload window from start 1, then:

    start.bat sites

The same settings as start 1, plus d3d8on11's draw time by the game's call site (`draw_sites=1`). No A/B tests here.

1. `mark plaza`: Port Jeuno plaza, standing, camera still: 60 s.
2. `mark run`: run around the plaza: 60 s.
3. `mark menus`: main menu and inventory open, scroll, and some chat on screen: 30 s, then close them.
4. `mark busy`: somewhere busy (a fight with effects, or a crowd): 60 s.
5. Close the game normally.

Then `restore.bat` (dgVoodoo back), close the xiload window, and `collect.bat`. Upload `ab_06nov_results.zip` from
the PlayOnlineViewer folder, with your notes on start 1's step 5 (which segments flickered).

## What it settles

- **The flicker:** your notes per segment (A against B). The frame log records your character's position in the game
  (frames_player), which moved every frame on 5 Nov too; the drawn model's position is not in it, so your eyes are the
  measure here, and speedfix's log says how many frames of each segment had a fraction of a tick.
- **The occlusion readbacks:** the plaza's frame times A against B (the frame log), and d3d8on11's own lines: how many
  readbacks it answered from its copies, its readback and Present times.
- **The game's time above 60 with frame_step=3:** your distance per real second from the position each frame, as on
  5 Nov (5.955-5.981 then).
- **Frames per step** with everything on, without the kit's call timing.
- **Which of the game's draws cost d3d8on11 the most** (start 2 only): every 600 frames d3d8on11's log lists its draw time by the
  game's call site (the return address of each draw into FFXiMain.dll), and the reader names each site's function. That
  says, before any of it is built, whether the characters' draws, the zone's, the UI text or something else is the next
  thing worth rewriting.
