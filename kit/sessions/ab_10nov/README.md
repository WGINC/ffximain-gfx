# The 10 Nov session: the new defaults, and Liminality's first stage

Two starts on Windows (Parallels), about 12 minutes, all in Port Jeuno. What it settles is listed at the end.

## What is new

- **d3d8on11's defaults:** dynamic_buffers and frame_latency=1 are now on without any setting (9 Nov: dynamic_buffers
  shortened Present, 12.9 to 10.2 ms in the plaza; both together were the smoothest run, 8 Nov).
- **occlusion_hold (speedfix, on):** 9 Nov measured that occlusion_late's one-probe-late answer gets visible/hidden
  wrong on 3-4% of probes, which can show as characters popping out and back in. Now a character that was seen is
  hidden only when two answers in a row say hidden. (A chosen rule, not the game's: a hidden character can be drawn one
  probe longer. Characters coming into view still appear one probe late.)
- **draw_sites fixed:** the kit put its own small piece of code between the game and speedfix, so the draws' call sites
  all came out as that code. The kit now hands back the game's address; d3d8on11 logs the draw time by the game's
  call site again (on in both starts; a measurement).
- **Liminality, stage 1 (start 2):** the game's own logic runs on whole ticks of 1/60 s, exactly as the game at 60
  fps, none of frame_step=3's changes; on the frames between ticks only the game's three drawing tasks run (the
  characters, the zone, the name plates: 9 Nov showed they make every draw of that pass), so the world is drawn again
  as it is. Expect motion to step 60 times a second (like the game at 60, at a higher frame rate), with the camera
  and the characters on the same clock (no jitter between them). Stage 2 (blending positions between ticks) is not
  built yet: what this start shows decides how.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_10nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones; **xikit.dll is new**), keep your own `xikit.ini` and `speedfix.ini` aside. Open a
Command Prompt **as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_10nov"

If the game freezes or crashes, close it and go to the next start (or to `restore.bat` and `collect.bat`): its log is
kept.

### Start 1: the new defaults (about 5 minutes)

    start.bat base

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s.
2. `mark pop`: where people walk in and out of view, 90 s, turning the camera now and then. **How do characters
   appear? Any popping out and back in?**
3. `mark run`: run around the plaza, 60 s. **Smooth or stuttery?** Close the game normally.

### Start 2: Liminality stage 1 (about 6 minutes)

    start.bat lim

4. `mark plaza`: standing, camera still, 60 s.
5. `mark camera`: rotate the camera slowly all the way round, 30 s. **Does the camera feel smooth?**
6. `mark pop`: the same place as step 2, 90 s. **How do characters appear?**
7. `mark run`: run around, 60 s. **Note anything frozen, flickering or wrong** (animations, effects, name plates,
   the UI, the chat) and how the motion feels against start 1. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_10nov_results.zip` with your notes on steps
2, 3, 5, 6 and 7.

## What it settles

- **The new defaults:** start 1's frame times and Present against 8 and 9 Nov, and your notes on pop-in.
- **occlusion_hold:** speedfix's log counts how often it kept a character visible one probe longer; your notes say
  whether the popping out and back in is gone.
- **Liminality stage 1:** whether drawing only on the frames between ticks works (nothing frozen or broken), and how it
  looks; speedfix's log counts the tasks it skipped.
- **The draws' cost by the game's call site:** d3d8on11's draw-site lines, for the next speed work (merging draws).
