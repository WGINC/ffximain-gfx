# The 22 Nov session: the final settings, measured; the native ARM64 executor once more; the installer

Three parts, about 15 minutes. Advanced shadows stay off throughout (left for another day).

## What changed since 21 Nov

- **Round shadows shrinking near:** found in the game's code. Near a character (its shadow caster under 20 units away)
  the game switches from a ground quad to a projected shadow, and with the same size number the projected one is
  exactly half as wide (the quad spreads the shadow texture over twice the size the projection maps it to). That is
  in the game as shipped too. speedfix now gives the projected one twice the number, so the two match.
- **Names:** the game sizes a name in 80 steps from its rounded depth, so it jumps a step at a time as you move.
  speedfix `name_smooth=1` sizes it from the unrounded depth. `names.bat 0` / `names.bat 1` switch it while you play.
- **Hitches:** the 4 readbacks at start-up are D3DX inside the game's loading (not in play). Mip levels are now built
  on a worker thread; a texture shows its levels a frame or two after it loads.
- **Crashes:** d3d8on11 now logs a crash with where it happened, and keeps the previous start's log
  (`d3d8on11_prev.log`).
- **Installer** (`ffxi_kit/`): d3d8.dll loads the kit and speedfix itself; nothing to start before the game. If the
  game is a build the kit does not know, the kit stays off, the renderer still runs, and a window says to update.

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_22nov` and `ffxi_kit` sit inside it; copy the zip's `kit` files
next to `xikit.dll`. Command Prompt **as administrator**, in `d12_22nov`.

## 1. The final settings (`start.bat final`)

Your usual settings, simple shadows. A busy town street.
- `mark final`, 1 minute walking: **walk up to a few characters: do their round shadows keep their size?**
- Then stand where names are at mid to long range and walk slowly toward and away from them: `names.bat 0` (the
  game's steps), 30 seconds, `names.bat 1` (smooth), 30 seconds. **Is the pulse gone with 1?**
- Then go to a new area (a zone change), and look out for any hitch while it fills in.

Close the game, `restore.bat`.

## 2. The ARM64 executor (`start.bat arm`; optional)

The same place. In this start the renderer's back end runs as a native ARM64 process instead of inside the
(x86-emulated) game. On 17 Nov it crashed after a few seconds, in Windower's Hook.dll; this time the crash, if it
comes, is logged with where. `mark arm`, 1 minute standing, 1 minute walking. If it crashes: run `collect.bat` at once
and say so. Close the game, `restore.bat`.

## 3. The installer (no start.bat)

1. Close xiload's window if it is open.
2. In `ffxi_kit`: `install.bat /defaults` (as administrator; the settings parts 1 and 2 left are kept as `.before_kit`).
3. Start the game as you always do (nothing else). Play a minute: shadows, names, the picture as in part 1.
4. Close the game (collect.bat takes this start's `d3d8on11.log` and `xikit.log` from the PlayOnlineViewer folder).
5. To go back to dgVoodoo: `uninstall.bat` in `ffxi_kit`. (Or keep it installed.)

Then `collect.bat` in `d12_22nov`, and upload `d12_22nov_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
