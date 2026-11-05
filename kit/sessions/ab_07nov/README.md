# The 7 Nov session: the camera following you on the fraction of a tick, and where the smudge comes from

One start on Windows (Parallels), about 12 minutes of play. What it settles is listed at the end.

## What is new, and why

- **The camera while running (frame_step=3, third build).** In the 6 Nov run the camera jittered while you ran. The
  game's code says why (docs/RENDERER.md 3c). The camera follows you in a loop that runs once per whole tick of 1/60 s:
  it closes part of the gap to its distance of 6, turns behind you (when that option is on) and pushes off walls. At
  frame rates above 60 the 6 Nov build moved the characters a fraction of a tick every frame, but ran this loop 0 times
  on some frames and once on others. So the camera stood still and then jumped. On the game's own instructions at
  144 fps, the camera's distance to a running player swung by 0.05 from one frame to the next (0.0001 at 60 fps). This
  build gives that loop, and the loop that brings the camera back along its path, the fraction of a tick too. The swing
  is now 0.0001, as at 60 fps, and the camera ends within 0.05 of where the game at 60 puts it after a second of running
  (kit/speedfix/test/test_steploops.py, section 6).
- **The smudge while running.** You saw everything slightly fuzzy or smudged while running, in the step test's B
  segments too. Its cause is not known. The camera's jumps (above) are one possible cause, because the whole picture
  moves with the camera. `abtest cap` (new) runs the same settings capped at 60 fps (A) and uncapped (B). If the smudge
  is in B only, it comes with frame rates above 60. If it is in both, it comes from something else.
- **No flip test.** The game froze when the flip test switched back (B to A). d3d8on11 no longer switches its swap
  chain while the game runs, and no longer accepts that A/B. The two A/B results (24-25 ms a frame both ways) showed
  no gain, so this session leaves it out.
- **No draw_sites and no gpu_time.** draw_sites put every draw under speedfix (its hooks sit between the game and
  d3d8on11): it stays off until that is traced. gpu_time read 0.000 ms on Parallels: it does not measure there.
- **Kept:** occlusion_late=1 (the plaza from 44-49 ms a frame to 22 ms in the 6 Nov run), the walk/run animation fix
  (no moonwalk), no call timing.

## Before you start

Unzip into the PlayOnlineViewer folder so that this folder (`ab_07nov`) sits inside it. Copy the zip's `kit` files
next to `xikit.dll` (replace the old ones): `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_measure.txt`, `xikit_facts.txt`, `speedfix.dll`, `xiload.exe`,
`xisamp_arm64.exe`, `xisamp_x64.exe`, `VERSION.txt`. Keep your own `xikit.ini` and `speedfix.ini` aside: the start
overwrites them. The start puts d3d8on11 in place of dgVoodoo's `d3d8.dll` (kept as `d3d8_dgvoodoo_keep.dll`);
`restore.bat` puts dgVoodoo back.

Open a Command Prompt **as administrator** and go to this folder:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_07nov"

## The start (about 12 minutes)

    start.bat

Start the game (Windower as always). Each step: run the command shown in the prompt, then do what it says. If the
game freezes or crashes, close it and run `start.bat` again before starting the game again: start.bat keeps the
last start's d3d8on11 log, and starting the game without it overwrites that log.

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 30 s.
2. `mark camera`: rotate the camera slowly all the way round, a few times: 30 s.
3. `mark run`: run around the plaza: 60 s. **Is the camera smooth now? Does anything look smudged?**
4. `abtest cap`: keep running around the plaza until it says `abtest cap done` (about 2 minutes). **Note for each
   segment (A, B, A, B) whether the picture looks smudged and whether the camera is smooth.** A is capped at 60 fps,
   B is not.
5. Go to an open field outside town and turn auto-run on (steer in a wide circle). `abtest step`: keep auto-running
   until it says `abtest step done` (about 2 minutes). **Note for each segment whether the camera is smooth, whether
   anything looks smudged, and whether characters flicker or moonwalk.** A is the 5 Nov setting (whole ticks), B this
   build.
6. `mark stand`: stand still anywhere, 30 s. Note anything that looked wrong. Close the game normally.

Then `restore.bat` (dgVoodoo back), close the xiload window, and `collect.bat`. Upload `ab_07nov_results.zip` from
the PlayOnlineViewer folder, with your notes on steps 3 to 5.

## What it settles

- **The camera jitter:** your notes on steps 3 to 5. speedfix's log says the follow's changes are in place
  ("24 of 24 loop changes") and how many frames had a fraction of a tick.
- **Where the smudge comes from:** your notes on `abtest cap`, A against B. The frame log gives each segment's frame
  times, so a smudge that comes with uneven frames would show there too.
- **Frames per step** (plaza, camera, run, stand) with the camera fix in, against the 6 Nov run (camera 16.3 ms, run
  15.2 ms, stand 6.2 ms a frame).
- **The game's time above 60 with frame_step=3:** your distance per real second from the position each frame, as on
  5 and 6 Nov.

## What it does not settle

- **Why Present takes ~12 ms of d3d8on11's worker in the plaza.** This is what keeps the plaza near 45 fps. Neither
  gpu_time nor the flip test answered it on Parallels. It is the next thing to find, offline first.
