# The final session (5 Nov): above 60 on d3d8on11, measured; the game's clock checked; frames for the UI text

Two starts on Windows (Parallels), about 30 minutes of play. Everything this answers is listed under "What it settles".

## Before you start

Unzip into the PlayOnlineViewer folder so that this folder (`final_05nov`) sits inside it. Copy the zip's `kit` files
next to `xikit.dll` (replace the old ones): `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`,
`xikit_manifest_6AC37846.txt`, `xikit_measure.txt`, `xikit_facts.txt`, `speedfix.dll`, `xiload.exe`,
`xisamp_arm64.exe`, `xisamp_x64.exe`, `VERSION.txt`. Keep your own `xikit.ini` and `speedfix.ini` aside: the starts
overwrite them. d3d8on11's files stay in this folder's `d3d8on11` folder; start 2 puts them in place and keeps
dgVoodoo's `d3d8.dll` as `d3d8_dgvoodoo_keep.dll` (it does this only if the `d3d8.dll` there is dgVoodoo's, version
4.8.x). `restore.bat` puts dgVoodoo back; start 1 does it too.

Open a Command Prompt **as administrator** and go to this folder:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\final_05nov"

Every script below is run from that prompt. Windower as you always run it.

## Start 1: dgVoodoo, frames captured (about 8 minutes)

    start.bat 1_capture

Start the game. In each place below, wait a few seconds, then run the command shown. The game pauses for a moment
for each capture (the frame is written whole, some tens of MB):

1. Port Jeuno plaza, the chat log open with some lines in it, your usual UI on screen: `capture plaza`
2. The same spot, the main menu and the inventory open: `capture menu`
3. Anywhere with your status and target windows up (a target selected): `capture target`

Close the game normally.

## Start 2: d3d8on11, the target (about 20 minutes)

    start.bat 2_target

Start the game. This is d3d8on11 (ours) in place of dgVoodoo, with speedfix's levers, and above 60 frames a second
with the game's time held to real time (`uncap`, `frame_step`). The occlusion readbacks are answered by d3d8on11
(not skipped). If the game does not start, draws wrongly or crashes, close it, run `restore.bat`, and send what you
have: that is a result too.

At the start of each step type `mark NAME` in the prompt (for example `mark plaza`):

1. `mark plaza`: Port Jeuno plaza, standing, camera still: 60 s
2. `mark camera`: rotate the camera slowly all the way round, a few times: 30 s
3. `mark run`: run around the plaza: 60 s
4. `mark menus`: main menu and inventory open, scroll: 30 s, then close them
5. `mark zone`: walk through a zone line, stand 20 s after you arrive
6. `mark busy`: somewhere busy (a fight with effects, or a crowd): 60 s
7. **The clock test.** Go to an open field outside town with nothing to run into. Turn auto-run on so your
   character runs on its own (steer in a wide circle if you need to stay in the field). Then type `clocktest`.
   It runs four 40 s segments by itself (about 3 minutes): two with the game held at 60 frames a second, two at the
   target settings. Keep auto-running until it says `clock test done`.
8. `mark stand`: stand still anywhere, 30 s. Note anything that looked wrong: the speed of walking, the camera or
   animations; the picture (text, effects, shadows, characters); stutters. Close the game normally.

Then `restore.bat` (dgVoodoo back), close the xiload window, and `collect.bat`. Upload
`final_05nov_results.zip` from the PlayOnlineViewer folder.

## What it settles

- **Frames above 60 on d3d8on11, per step, with the readbacks handled (not skipped):** the frame log's median, p95
  and p99 for each step. 26 Oct measured the plaza at a 12.9 to 14.0 ms median with the readbacks skipped; this run
  says what it is with d3d8on11's snapshots answering them, and what the busy scene and zoning do.
- **The game's time above 60:** your position each frame (frames_player), against the frame log's clock. In the reference
  segments every frame is one tick of 1/60 s (docs/RENDERER.md 3a), so the game's speed there is known from the frame
  count; in the test segments `frame_step` must give the same distance per real second. The answer is a ratio, from
  the game's own position.
- **Where each slow frame's time went:** the timed calls, the Windows calls and the samples per frame
  (`measure_read.py`), and d3d8on11's own executor and snapshot lines.
- **The UI text difference** (8,689 pixels, task #140): start 1's captures hold dgVoodoo's own picture of each frame
  and everything the frame drew; they are replayed through d3d8on11 here and compared pixel by pixel.
- **Whether the sampler's stacks are fixed:** the log says how many samples had ESP outside the stack.
