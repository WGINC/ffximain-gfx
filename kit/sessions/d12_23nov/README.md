# The 23 Nov session: where the game's own thread spends its time; the zone-change fix

One start of the game and the installer, about 10 minutes.

## What your 22 Nov run showed

- Walking in town: 10.0-10.9 ms a frame (92-100 fps). Of that, the game's own thread is busy 9.1-9.9 ms and waits on
  the renderer 0.03-0.12 ms: the limit is the game's own code (x86, emulated on the Mac), not the drawing.
- The zone change: 33-42 ms frames for about a minute. That was mine: speedfix's occlusion reads ran out of the
  renderer's 1,024 snapshot slots (the area had up to 877 occlusion objects, 3 pictures each) and waited for the GPU.
  Now 4,096 slots.
- The arm crash: in Windower's Hook.dll, on the kit's own measuring thread, while it read window titles. It reads
  them without a message now. (Not tried again here.)

To find what in the game's own code takes the time, this start samples the game's thread every millisecond: where it
is, and which game functions are on its stack. The sampling costs a little itself, so this start's frame rate is not
the one to compare; the samples are what it is for.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_23nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed from last time: `uninstall.bat` in your old `ffxi_kit` first. Command
Prompt **as administrator**, in `d12_23nov`.

## 1. The profile (`start.bat prof`)

Your usual settings. The same town street as last time.
- `mark walk`, 2 minutes walking around as you normally would.
- `mark stand`, 1 minute standing still among people.
- `mark zone`, change zone once (the slow minute last time), and walk a minute in the new area.

Close the game, `restore.bat`.

## 2. The installer, updated

In `ffxi_kit`: `install.bat /defaults`, start the game as usual, change zone once, and play a minute. Close the game.
(Keep it installed or `uninstall.bat`, as you like.)

Then `collect.bat` in `d12_23nov`, and upload `d12_23nov_results.zip`.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
