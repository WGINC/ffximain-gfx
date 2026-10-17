# Where the floor is: T1, T2, T3 in one session

This session runs the three tests of docs/REEVALUATION.md section 7. Together they say what limits the frame on
your machine:
- **T1, resolution:** is the GPU's fill a limit?
- **T2, the readback removed:** is the occlusion readback a limit?
- **T3, no drawing:** with nothing drawn, can the game's own CPU work (x86 code under emulation) fit in a 60 fps
  frame at all?

Everything runs on **dgVoodoo** (yours), in the Port Jeuno plaza (the busy part), standing still. It takes two game
starts and about 20 minutes. The levers are the ones of perf60's run 1 (`precise_pacing` included). In each A/B,
speedfix switches the lever by itself; nothing to type.

## What to copy

- `speedfix.dll` of this package next to `xikit.dll`, over the old one. Only speedfix changed. The rest of the
  kit is the same as in perf60.
- `xikit.ini` of this folder over yours. It is perf60's: a new log folder per start, and render-thread samples.
- Keep your own `xikit.ini` and `speedfix.ini` aside.

## Start 1: your usual resolution (T2, then T3)

1. Copy `1_readback/speedfix.ini` over yours. Start the game, go to the plaza, stand still, and leave the game
   alone.
2. About 5 minutes. `xikit.log` says `every group of ab_levers has run` when T2 is done.
3. Without closing the game, copy `2_no_draws/speedfix.ini` over `speedfix.ini`. speedfix checks the
   file every 60 frames (about 2 seconds here) and starts T3 a minute after reading it.
   - **The screen will go black or freeze every other 30 seconds.** That is the test: in its B phases nothing is
     drawn. Leave the game alone.
   - It stays that way after the test ends too.
4. About 5 minutes. When `xikit.log` says `every group of ab_levers has run` a second time, close the game.
5. Send the log folder.

## Start 2: a much lower resolution (T1)

1. In the game's config tool, set the window size and the background resolution much lower than usual
   (for example 1024 × 768). Note what you set, and what your usual values are.
2. Copy `1_readback/speedfix.ini` over yours again. It must not be T3's, or the screen goes black.
3. The same spot, standing still, about 5 minutes. When `xikit.log` says `every group of ab_levers has run`, close
   the game.
4. Put your usual resolution back. Send the log folder, both resolutions, and `dgVoodoo.conf`.
   dgVoodoo can force its own resolution. If it does, the game's setting does not change how many pixels are drawn.
   Its conf says whether it does.

If the game behaves badly at any point (a crash, a hang, a screen that stays wrong after the test), close it.
Then put your own `speedfix.ini` back and send the log folder anyway.

## What each test reads, and what follows

All figures come from `xikit.log` in the log folders.

- **T2** (start 1, first A/B). Readbacks run in A (`skip_all_occlusion_readbacks` off) and are skipped in B.
  - The A/B summary gives the frame time of each.
  - Each minute, `occlusion readbacks: N run, M skipped` shows that the readbacks really ran in A and stopped in B.
    Whether the game requests them depends on something not known (docs/REEVALUATION.md section 4). Session 5's
    readback A/B fell in minutes with none. If that happens again, T2 says nothing, and the count shows it.
  - If B is shorter than A by more than the spread between phases of one label, the readback is a limit. The
    asynchronous readback (task 33) is then the next build.
  - If not, its wait is paid elsewhere, and T1 and T3 decide.
- **T3** (start 1, second A/B). Draws are sent in A and not sent in B. Readbacks are skipped in both: with nothing
  drawn, what the game would read back is meaningless.
  - Each minute, `null_draws: N draw calls not sent` shows the lever acted.
  - The game's own frame limiter (precise_pacing: 16.67 ms) is a floor, and B cannot go below it.
  - If B's median sits at about 16.67 ms, the game's CPU work and every non-draw call fit in a 60 fps frame. The
    rendering path is then the cost.
  - If B stays well above 16.67 ms, 60 is out of reach on this machine whatever the renderer does. Speed then
    means the game's own CPU work, under x86 emulation.
  - How far below 16.67 ms the game would go without its limiter is not measured here. The limiter's waiting time
    is in the `precise_pacing:` lines and the samples, but those lines mix A and B minutes.
- **T1** (start 2 against start 1). Compare the A phases of T2 at the two resolutions: the same settings, only the
  resolution differs. The B phases are a second comparison.
  - If the frame drops a lot at the low resolution, the GPU's fill is a limit.
  - If it barely moves, fill is not the limit.
  - What "a lot" means is the spread between phases of one label, as for every A/B here.

## Not known before the run

- Whether dgVoodoo's settings let the game's resolution change the pixels drawn (`dgVoodoo.conf`).
- Whether presenting a frame with nothing drawn costs dgVoodoo the same as a drawn one. T3's B includes Present
  and Clear as usual, but not the draws.
- Whether `null_draws` shows any problem in the game. It was tested offline only (kit/speedfix/test): the draws
  do not reach the device, the state calls do, and the lever switches live both ways.
