# The 30 Nov session: what the remaining stutters are

One start of the game, about 8 minutes.

## What it does

Your installed settings (`pace_adaptive`, `precise_pacing`, three swap buffers, no cap), plus the native sampler: a
separate ARM64 program that looks at the game's thread every millisecond, from outside it. Every frame that takes
25 ms or more gets its own samples, so each stutter can be put on what the thread was doing: the game's own code
(and where in it), the renderer, Windows, or the emulator. Then we know which stutters are ours to fix and which are
the game's own work. The sampler costs a little itself, so this start's frame rate is not one to compare.

Your 24 Nov run already had 28 frames over 50 ms captured this way. All were in the game's own code (ground-height
lookups as you walk, an MD5 calculation, the game's window messages). This start also catches the shorter ones,
25 to 50 ms, which are more likely the stutters you feel.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_30nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll` (it has `xisamp_arm64.exe`, the sampler). If the kit is installed: `uninstall.bat` in `ffxi_kit`
first. Command Prompt **as administrator**, in `d12_30nov`.

## The start (`start.bat stut`)

Play as you normally would, where you feel stutters:
- `mark town`, 2 minutes walking around town, through busy spots, turning the camera;
- `mark new`, go somewhere new (another area or a zone change), walk 2 minutes;
- `mark back`, come back and walk 1 minute.

If you feel a particular stutter, type `mark felt` right after it (the frame log keeps when). Close the game,
`restore.bat`, `collect.bat`, and upload `d12_30nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
