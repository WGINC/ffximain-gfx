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

*Pshhh... Shkohhh...*

Hullo, adventurer! Gobbie bring fixings for youuugame: a new renderer (d3d8on11, on Direct3D 12), a fasterfaster
gameloop (speedfix), and the kit that hold them together (xikit). Game run faster, smoother, and gobbie no change how
it look except where this page say so. *Pshhh...*

## Gobbie movein

1. Close the game.
2. Command Prompt **as administrator**, in this folder: `install.bat`.
   Game somewhere else? `install.bat "D:\path\to\PlayOnlineViewer"`.
3. Start the game sameway as always. Nothing else to start: game load gobbie by itself. *Shkohhh...*

Gobbie keep youuu oldthings safesafe in the sack:
- the `d3d8.dll` that was there (dgVoodoo's) become `d3d8_before_kit.dll`;
- youuu `gobbiefix.ini` stay as it is when youuu install again (`install.bat /defaults` put gobbievalues back, and keep
  yours as `gobbiefix.ini.before_gobbiefix`);
- the other `.ini` files is gobbiepot; an older one is kept once as `NAME.ini.before_gobbiefix`.

## Gobbie moveout

`uninstall.bat` (as administrator, game closed). *Pshhh...* gobbie packpack the shinybits and put the old `d3d8.dll`
back. The `.ini` files and logs stay.

## Youknobs: gobbiefix.ini

In the PlayOnlineViewer folder. Change a value, save, start the game: gobbie sniff it every gamestart. Only these is for
youuu; the other `.ini` files is gobbiepot (gobbiefix.ini write over the few keys it own in them, every gamestart).

### frame_cap (0)
How many frames a second the game may make. **0** = no cap: game go fastfast as it can. **10 to 1000** = gobbie hold it
to that many. Gobbie measured a 120 cap on youuu kind of Mac and it no make things smoother, so gobbie leave it off.
With a cap, `smooth_pacing` take a nap: only one of the two work at a time.

### smooth_pacing (1)
**1** = gobbie space the frames eveneven. Each frame wait until the time most frames (9 in 10) of the last 120 took, so
the speed follow the scene but the steps stay even. Measured: frames taking over twice as long as their neighbours went
from 67 to 19 a minute. **0** = each frame go the moment it is ready (more upsanddowns). *Shkohhh...*

### swap_buffers (2)
How many pictures the renderer juggle with Windows (only with `renderer=d3d12`).
- **2** (gobbiepick): fewer frames made, but nearly all reach the screen. Measured blind, same spot, 3 minutes each:
  Windows showed 94% of frames with 2 against 76% with 3, and frames held on screen for an extra refresh 15 a second
  against 23. Youuu felt no stutter with 2 and the usual judder with 3.
- **3**: the framecounter go higher (127 against 111 that day), but the extra frames get throwaway before the screen,
  and the ones left come uneven.
- **4**: allowed, never measured.

Gobbie also tested the cost: at the same framerate 2 still win, so it is not only "slower is smoother". *Pshhh...*

### mipmaps (1)
**1** = faraway textures get smaller, smoothed copies, so floors and walls in the distance no shimmersparkle. This
change how far things look (that is the point). **0** = the game's textures exactly as they is.

### anisotropy (16)
How sharp textures stay when youuu see them slantways, like a road going away from youuu. **0 to 16**, 16 sharpest.
Needs `mipmaps=1`.

### smooth_names (1)
**1** = names over heads shrink smoothsmooth as things walk away. **0** = the game's own 80 little sizesteps (youuu see
names pulse bigger and smaller as people move).

### fitted_shadows (1)
**1** = the round shadows under people sized to each body. **0** = the game's own shadowsize.

### sound_retry (1)
*Shkohhh...* Youuu hear the rain stop after a while, and it no come back? The game has only so many voices (12, or what
youuu set in the registry, at most 20). When a looping sound like rain or wind lose its voice and ask for a new one while
all of them is busy, the game no wait: it throw the sound away for good, until the weather or zone start it again. Gobbie
found that in the game's own code (it happen with no gobbiefix too). **1** = gobbie keep such a sound and it ask again at
the next update, the sameway the game already does when too many sounds ask in one update, so it come back as soon as a
voice is free. Only sounds the game itself keep while youuu cannot hear them is kept; the rest go as before. **0** = the
game's way.

### sphere_shine (1)
*Pshhh...* Some metal, like Warlord Rojgnoj's gold armour, is meant to shine with a reflection. On PC the game read the
wrong corner of the reflection picture at half the PS2's strength, so the metal look dull. **1** = gobbie draw it the
PS2's way: right part of the picture, full strength. **0** = the game's dull way. (How the PS2 does it was measured by
voetre for portfix; gobbie made its own version inside gobbie's model drawing.)

### flare_fix (1)
Lamps and lanterns far away have a glow that blinkblink behind its own lamp. *Shkohhh...* **1** = gobbie draw the glow a
tiny bit nearer (0.4, along the glow's own depth), so it stay in front. **0** = the game's blinky way. (From dagnyfix, by
Dellingr; gobbie move it with the glow's position instead of copying its points.)

### aura_size (256)
*Pshhh...* The glowy auras around people (and their ghosty copies) is drawn from a small picture of the scene, only
256 dots across, then blown up big. With dgVoodoo the edges come out jaggy; with gobbie's renderer youuu see them
smooth already (why, gobbie no know yet), so gobbie leave the game's **256**. **512, 1024, 2048, 4096** = a bigger
picture, if youuu still see jaggies (2048 is what SpectralFix used; bigger cost a little more of the graphics card). (SpectralFix by Krauerlabs found the problem; dagnyfix by Dellingr brought it over; gobbie
fixed it where the game make the picture.)

### renderer (d3d12)
**d3d12** = gobbie's fastfast renderer. If Direct3D 12 no start on a machine, it use Direct3D 11 by itself. **d3d11** =
always the olderway (try this only if d3d12 give youuu trouble).

### update_notice (1)
When the game get patched, gobbie sniffsniff its build. If gobbie no know the new build, the game still run with the new
renderer, but speedfix and the other fixings stay in the sack (gobbie no know where things is), and **1** = a window
tell youuu to fetch a new gobbiefix. **0** = no window. *Shkohhh...*

### debug (0)
**0** = shortshort, tidy logs. **1** = gobbie scribblescribble statistics in the logs (for helpfixing problems).
Easiest: `gobbiefix_debug.bat on` or `gobbiefix_debug.bat off` (as administrator; takes effect at next gamestart).
"on" leave a little file, `gobbiefix_debug.on`, in the gamefolder: while it is there, debug is on whatever
gobbiefix.ini say. "off" take it away.

## Logs

In the PlayOnlineViewer folder: `d3d8on11.log` (last start's is `d3d8on11_prev.log`; a crash is scribbled there with
where it happened) and `xikit.log`. Something wrong? Turn debug on, play until it happen, and send gobbie those two.

## What gobbie know, and no know

*Pshhh...*
- On a Mac with Parallels, gobbie measured with a slowmotion phone that some frames Windows show never reach the Mac's
  screen (lost after Windows hand them over). That is outside the game: gobbie cannot fixit from inside. Setting the
  Mac's screen to a fixed 60 Hz instead of ProMotion no fixit either.
- Gobbiefiles: `d3d8.dll`, `d8x.exe`, `d8x_arm64.exe` (the renderer), `xikit.dll` (the kit),
  `speedfix.dll`, and the `xikit_manifest*.txt` files (where things is in each gamebuild gobbie know, and a
  sniffprint of each place so gobbie can check it is the build gobbie know; no game code in them). An older
  gobbiefix also put `gfx_recon_ingame.dll` there: install.bat take it away, this one no need it.

## Legalstuff

Gobbie's own work is under the GNU GPL, version 3 only (`COPYING.txt` has the whole thing); FINAL FANTASY XI and everything of it belong to SQUARE ENIX, and gobbie no
belong to them. The real words is in `LICENSE.txt` (plain English, no gobbiespeak, *Shkohhh...*), and they is what count.
