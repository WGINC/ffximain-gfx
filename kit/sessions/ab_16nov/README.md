# The 16 Nov session: frame time (two waits removed, one upload path changed)

Five short starts on Windows (Parallels), about 20 minutes (the last two are optional). No Liminality in this one: every start is the build as
you last liked it (12 Nov's start 3) with one or two changes. What it settles is listed at the end.

## What your last base run showed (12 Nov, start 3)

- **The game's own thread waited 2 to 6 ms a frame for the occlusion test's answer.** speedfix reads each answer a
  probe late so that it need not wait, but d3d8on11's thread runs up to two frames behind the game, so a quarter of
  those late reads still waited (1 to 2 ms each on average, the longest 29 to 173 ms in each minute). Those long ones
  are a likely cause of the stutters you still saw; that is not proven until this run.
- **d3d8on11's thread was busy 95% of the time** (13 to 21 ms a frame in the plaza): Present 6 to 9 ms, draws 4 to 6,
  buffer uploads 2, read-backs 1 to 3. Each frame it wrote 300 to 650 of the game's vertex buffers, each with a
  call of its own.

## What changed

1. **The late occlusion read never waits** (speedfix `occlusion_nowait=1`, with a new do-not-wait lock in d3d8on11).
   If the answer is not ready, speedfix gives the object's last answer again and takes the new one when it is ready.
   What changes in the game: an answer can be two or three frames old instead of one.
2. **Buffer arena** (d3d8on11 `buffer_arena=1`): the game's vertex buffers that change every frame are written into
   one large buffer with one call per batch instead of one call each. The picture is the same: d3d8on11's tests
   compare every test scene pixel for pixel with and without it. Whether it is faster on your machine is what this
   run measures.
3. **First probe answered at once** (speedfix `occlusion_first=2`, start 4 only): when a character or effect is
   tested for the first time, the game waits about 12 ms for the answer. With this setting speedfix answers
   "visible" that one time instead. This one is a chosen answer, not the game's: something behind a wall is still
   hidden by the wall, but a glow whose brightness comes from the test may show at full brightness for one frame.

4. **d3d8on11's worker as a native ARM64 program** (`mode=remote`, start 5 only). Until now d3d8on11's worker has run
   as a thread inside the game, which is x86 code that Windows translates. In this mode the same worker runs as its
   own ARM64 program (`d8x_arm64.exe`) next to the game. This mode ran in your game once (16 Oct, in the house, where the
   game's own 60 fps limit hid any difference); whether that start used the ARM64 program or the x86 one is not in
   my records, and it has never been measured in the plaza. I cannot run ARM64 programs here, so this one is tested
   only in its x86 build. If it
   cannot start, d3d8on11 falls back to the thread and says so in its log.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_16nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_16nov"

Each start is the same three steps in the same place, so that the starts can be compared. If the game freezes or
crashes, close it and go to the next start: its log is kept.

### Start 1: both changes (about 4 minutes)

    start.bat fast

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s.
2. `mark run`: run around the plaza, turning the camera, 60 s. **Stutters? Anything drawn wrong, missing or
   flickering? Characters popping in or out more or less than before?**
3. `mark crowd`: walk into the busiest place nearby so that many characters load at once, 60 s. Close the game
   normally.

### Start 2: only the occlusion change

    start.bat nowait

Steps 1 to 3 again.

### Start 3: neither (the build as it was)

    start.bat base

Steps 1 to 3 again. **Against start 1: smoother, rougher, the same?**

### Start 4 (optional): start 1 plus the first-probe answer

    start.bat first

Steps 1 to 3 again. **In step 3: fewer hitches when many characters load? Any glow or light flashing for a frame?**

### Start 5 (optional): start 1 with the native ARM64 worker

    start.bat arm

Steps 1 to 3 again. A second program, `d8x_arm64.exe`, runs while the game does; it ends with the game.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_16nov_results.zip` with your notes.

## What it settles

- **Whether the waits are gone and what that is worth:** speedfix logs the time in its late reads and how many were
  told "not yet"; the frame log gives the frame time and the share of long frames, start 2 against start 3.
- **Whether the buffer arena helps:** d3d8on11 logs its thread's time in uploads, draws and Present, start 1 against
  start 2.
- **Whether the first-probe wait is what hitches when a crowd loads:** step 3 of start 4 against start 1.
- **Whether the native ARM64 worker is faster:** d3d8on11's worker time and the frame time, start 5 against start 1.
- **Whether anything looks different:** your notes. Nothing should, except as said under change 3.
