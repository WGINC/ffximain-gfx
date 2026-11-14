# Longthinx gobbiefix

```
   \'-._     .-''''''''''-.     _.-'/
    \   '-._/  .--.  .--.  \_.-'   /
     '.     | (    )(    ) |     .'
       '-.  |  '--'  '--'  |  .-'
          '-|      ||      |-'
            \  o   ||   o  /
             | o   ||   o |
              \o   ||   o/
               '--------'
```

Hullo, adventurer! Gobbie bring fixings for your game: a new renderer (d3d8on11, on Direct3D 12), a faster game
loop (speedfix), and the kit that hold them together (xikit). Game run faster, smoother, and gobbie no change how it
look except where this page say so.

## Gobbie move in

1. Close the game.
2. Command Prompt **as administrator**, in this folder: `install.bat`.
   Game somewhere else? `install.bat "D:\path\to\PlayOnlineViewer"`.
3. Start the game same as always. Nothing else to start: game load gobbie by itself.

Gobbie keep your old things safe in the sack:
- the `d3d8.dll` that was there (dgVoodoo's) become `d3d8_before_kit.dll`;
- your `gobbiefix.ini` stay as it is when you install again (`install.bat /defaults` put gobbie's values back, and
  keep yours as `gobbiefix.ini.before_gobbiefix`);
- the other `.ini` files is gobbie's own pot; an older one is kept once as `NAME.ini.before_gobbiefix`.

## Gobbie move out

`uninstall.bat` (as administrator, game closed). Gobbie take the shiny bits and put the old `d3d8.dll` back. The
`.ini` files and logs stay.

## Your knobs: gobbiefix.ini

In the PlayOnlineViewer folder. Change a value, save, start the game: gobbie read it at each start. Only these is for
you; the other `.ini` files is gobbie's pot (gobbiefix.ini write over the few keys it own in them at each start).

### frame_cap (0)
How many frames a second the game may make. **0** = no cap: game go as fast as it can. **10 to 1000** = gobbie hold it
to that many. Gobbie measured a 120 cap on your kind of Mac and it no make things smoother, so gobbie leave it off. With
a cap, `smooth_pacing` take a nap: only one of the two work at a time.

### smooth_pacing (1)
**1** = gobbie space the frames even. Each frame wait until the time most frames (9 in 10) of the last 120 took, so
the speed follow the scene but the steps stay even. Measured: frames taking over twice as long as their neighbours went
from 67 to 19 a minute. **0** = each frame go the moment it is ready (more ups and downs).

### swap_buffers (2)
How many pictures the renderer juggle with Windows (only with `renderer=d3d12`).
- **2** (gobbie's pick): fewer frames made, but nearly all reach the screen. Measured blind, same spot, 3 minutes each:
  Windows showed 94% of frames with 2 against 76% with 3, and frames held on screen for an extra refresh 15 a second
  against 23. You felt no stutter with 2 and the usual judder with 3.
- **3**: the frame counter go higher (127 against 111 that day), but the extra frames get thrown away before the screen,
  and the ones left come uneven.
- **4**: allowed, never measured.

Gobbie also tested the cost: at the same frame rate 2 still win, so it is not only "slower is smoother".

### mipmaps (1)
**1** = far-away textures get smaller, smoothed copies, so floors and walls in the distance no shimmer and sparkle.
This change how far things look (that is the point). **0** = the game's textures exactly as they is.

### anisotropy (16)
How sharp textures stay when you see them at a slant, like a road going away from you. **0 to 16**, 16 sharpest.
Needs `mipmaps=1`.

### smooth_names (1)
**1** = names over heads shrink smooth as things walk away. **0** = the game's own 80 little size steps (you see names
pulse bigger and smaller as people move).

### fitted_shadows (1)
**1** = the round shadows under people sized to each body. **0** = the game's own shadow size.

### renderer (d3d12)
**d3d12** = gobbie's fast renderer. If Direct3D 12 no start on a machine, it use Direct3D 11 by itself. **d3d11** =
always the older way (try this only if d3d12 give you trouble).

### update_notice (1)
When the game get patched, gobbie check its build. If gobbie no know the new build, the game still run with the new
renderer, but speedfix and the other fixings stay off (gobbie no know where things is), and **1** = a window tell you
to fetch a new gobbiefix. **0** = no window.

### debug (0)
**0** = short, tidy logs. **1** = gobbie scribble statistics in the logs every few seconds (for helping find
problems). Easiest: `gobbiefix_debug.bat on` or `gobbiefix_debug.bat off` (as administrator; takes effect at next
start). "on" leave a little file, `gobbiefix_debug.on`, in the game folder: while it is there, debug is on whatever
gobbiefix.ini say. "off" take it away.

## Logs

In the PlayOnlineViewer folder: `d3d8on11.log` (last start's is `d3d8on11_prev.log`; a crash is written there with
where it happened) and `xikit.log`. Something wrong? Turn debug on, play until it happen, and send gobbie those two.

## What gobbie know, and no know

- On a Mac with Parallels, gobbie measured with a slow-motion phone that some frames Windows show never reach the Mac's
  screen (lost after Windows hand them over). That is outside the game: gobbie cannot fix it from inside. Setting the
  Mac's screen to a fixed 60 Hz instead of ProMotion no fix it either.
- Gobbie's files: `d3d8.dll`, `d8x.exe`, `d8x_arm64.exe` (the renderer), `xikit.dll` and `gfx_recon_ingame.dll` (the
  kit), `speedfix.dll`, and the `xikit_manifest*.txt` files (where things is in each game build gobbie know).
