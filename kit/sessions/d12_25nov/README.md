# The 25 Nov session: a 120 fps cap, and a third swap-chain buffer

Two starts of the game, about 15 minutes.

## Why

- **The screen (your 24 Nov start fast, PresentMon):** your display refreshes 120 times a second. 94% of frames stayed
  up for one refresh (even motion), 3.9% for two (a hitch), and 10% of the frames made were never shown, because the
  game made them faster than 120 a second. A cap at 120 may turn those into frames that each get one refresh. It may
  also not help: the cap's waits are timed by the game's own clock, not the display's.
- **The renderer:** most of its time per frame (3.6-4.5 of 7.2-7.8 ms) is spent handing frames to Windows. With two
  swap-chain buffers, one is on the screen until the next refresh and the other is being drawn, so the renderer waits
  for that refresh before it can start the next frame. A third buffer lets a finished frame wait in a queue instead.
  The frames drawn are the same; on the screen it can mean frames are a little later.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_25nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. If the kit is installed: `uninstall.bat` in `ffxi_kit` first. Command Prompt **as
administrator**, in `d12_25nov`. The same street and the same walk as last time.

## 1. Two buffers (`start.bat cap2`)

The installed settings with the cap on. Once you are on the street and walking: `ab.bat`. For about 4 minutes
speedfix switches the cap off and on by itself every 30 seconds; keep walking your usual route until `ab.bat` says
done. **Can you see when it switches?** Note it if so. Close the game, `restore.bat`.

## 2. Three buffers (`start.bat cap3`)

The same: on the street, walking, `ab.bat`, about 4 minutes, close the game, `restore.bat`.

Then `collect.bat`, and upload `d12_25nov_results.zip`, with which start felt smoother if one did.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
