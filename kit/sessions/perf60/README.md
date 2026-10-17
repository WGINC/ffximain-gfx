# Toward 60 fps: two runs

What these runs answer:
- **Does speedfix's `precise_pacing` move the game from 58.8 to 60 frames a second?** The 58.8 comes from the game's
  own frame limiter (0x10011A00), which times frames with whole milliseconds. Two things in the owner's game are not
  established: the limiter's tick count and the timer's scale. speedfix's log now writes both.
- **Does our executor filter (`exec_filter`) cut the executor's time a frame in the game?** It runs on ours only.
- **Where does the render thread's time go?** (`sample=1`). The answer shows what stands between the game and 60
  where it does not reach 60.

Neither run goes above 60. The game's time step is whole ticks of 1/60 s, at least one a frame. Above 60 the
game's time would run fast (docs/ROADMAP.md P4). Your friend's hundreds of frames a second under Vulkan on Linux is
not explained by anything established here. Which frame rate counter they use, and whether they run a patch or a
private server's client, is not known. A frame log from their machine would say.

Each run's settings are in its folder. Copy them over the ones in the PlayOnlineViewer folder, and keep yours aside.
Every run writes a new log folder (`log_folder=1`); send that folder.

| Run | Device | Where | Time | Answers |
|---|---|---|---|---|
| 1. `1_dgvoodoo` | dgVoodoo (yours) | Port Jeuno plaza, standing | about 8 min | `precise_pacing` off (A) against on (B), 3 rounds of A B B A, 30 s each, from 60 s after the start; samples of the render thread throughout |
| 2. `2_ours` | ours, `thread` mode | the same spot | about 17 min | First, 60 s to 540 s: the executor filter off and on (`exec_ab`). Then, from 600 s: `precise_pacing` off and on, as in run 1 |

In run 2 the two A/Bs do not overlap: the filter's A/B ends at 540 s and `precise_pacing`'s starts at 600 s.

## Run 1: dgVoodoo

1. Copy `1_dgvoodoo/xikit.ini` and `1_dgvoodoo/speedfix.ini` over yours, and the `speedfix.dll` of this kit next to
   `xikit.dll`.
2. Start the game, go to Port Jeuno's plaza (the busy part), stand still, and leave the game alone.
3. When `xikit.log` says `every group of ab_levers has run`, close the game.
4. Send the log folder and `dgVoodoo.conf`. Whether dgVoodoo waits for the screen's refresh decides what 60 can look
   like, and that setting is not known here.

## Run 2: ours

1. Copy `d3d8.dll`, `d8x.exe` and `d8x_arm64.exe` from this kit's `d3d8on11` folder over dgVoodoo's `d3d8.dll`
   (keep it aside), and `2_ours/d3d8on11.ini` next to them. Copy `2_ours/xikit.ini` and `2_ours/speedfix.ini`
   over yours.
2. The same spot, standing still. `d3d8on11.log` says `executor ab:` after each phase of the filter's A/B; at
   600 s `xikit.log` starts `speedfix: ab`.
3. When `xikit.log` says `every group of ab_levers has run`, close the game.
4. Put dgVoodoo's `d3d8.dll` back. Send the log folder, `d3d8on11.log`, and `d3d8on11_exec.log` if it is there.

If ours does not start, or stops, or draws wrongly, close the game and put dgVoodoo's `d3d8.dll` back. Send
`d3d8on11.log` and the log folder. Run 1 still stands on its own.

## What happens with the results

- `precise_pacing`:
  - Its log line `precise_pacing: ... frames timed by the counter` says whether the patch timed the frames at all.
    If not, the limiter ran outside the hooks' frames, and the lever's frame test is wrong.
  - It becomes a default if B's mean frame is shorter than A's by more than the spread between phases of one label.
    The expected B is about 16.67 ms.
  - The first speedfix lines name the limiter's tick count and the timer's scale. If they are not 1 and 1.0, the
    58.8 figure does not apply as stated.
- `exec_filter`: it becomes ours' default on the same rule, using the executor's busy time a frame from the
  `executor ab:` lines.
- The samples: whatever function holds the render thread where frames take longer than 16.7 ms is the next target.
