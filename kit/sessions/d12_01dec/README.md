# The 1 Dec session: the stutters you see every few seconds

Two starts of the game, about 4 minutes each, in the same place.

## Why

The 30 Nov run did not catch what you felt. 17 of your 19 `mark felt` had no long game frame before them. What it did
show, about 30 times a second: a frame left on screen for two refreshes or more. And the flicker or smear you describe
happens after the game has handed the frame to Windows, where the game's logs cannot see.

This session does two things:
- **Two starts that differ in one setting**, which this README does not name, so that what you notice is not
  coloured by knowing. Compare them on feel, and PresentMon measures them.
- **Two recordings of the same moments:** one from the Mac, which records the picture as software made it, and one
  from your phone, which films what the screen shows. If the smear or flicker is in the Mac recording, software makes
  it (Windows or Parallels). If only the phone sees it, the display does.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_01dec` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_01dec`.

## Each start (`start.bat one`, then `start.bat two`)

Go to the spot where you see the stutters most and stay in that zone. Then, once you are in:
1. `mark go`, then walk and turn the camera for 3 minutes, as you play.
2. Each time you see a stutter, `mark felt` (as before).
3. Close the game. Before the next start, write down how this one felt (one line is enough).

## The recordings (start `two` only, in its last minute)

- **On the Mac:** Cmd+Shift+5, "Record Entire Screen", Record. Stop it from the menu bar after about a minute.
- **At the same time, on the phone:** slow-motion video of the screen, the phone held still, the whole game window in
  view. Film a few clips of 5 to 10 seconds while turning the camera, best when a stutter comes.
- Type `mark rec` in the Command Prompt when you start both recordings, so the logs line up.

Upload the phone clips and the Mac recording with the results (or a minute of each, if they are large).

## After

`restore.bat`, `collect.bat`, upload `d12_01dec_results.zip`, and your two lines about how each start felt.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
