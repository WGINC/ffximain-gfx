# The 15 Nov session: Direct3D 12 with the sampler fix

First a one-minute picture check, then one start of the game, about 5 minutes.

## What your 13 Nov checks found

- On your Mac's Direct3D 12 driver, a draw's texture settings (smooth filtering, clamp at the edge) are read from the
  wrong place: the driver reads the sampler table at the start of the sampler heap whatever place the draw names. The
  "table place check" showed it: the smooth setting read right at the heap's start (128) and the first table's
  unfiltered setting at every other place (0). Every game draw after the first therefore got the first draw's settings:
  unfiltered, repeated past the edges. That is the pixelation and jagged edges.
- With every draw's settings put at the start (a slow diagnostic), all 35 test scenes drew exactly as Direct3D 11.

## What this build does

At the start, d3d8on11 checks where the driver reads sampler tables. Where it reads them only at a heap's start (your
Mac), each set of texture settings gets a small heap of its own, the table at its start, and the heap is switched when
the settings change. A second check at the start confirms the driver reads the heap switched to; if it does not, the
whole start runs on Direct3D 11 and the log says so. On drivers that read the place right nothing changes.

Not known yet: whether this also fixes the shadows (the boxes and the moving shadow), and what switching heaps costs in
speed on your Mac. This start measures both.

## 1. The picture check (one minute)

Open the `check` folder in `d12_15nov` and run `run_formats.bat`. Small windows open and close. It makes
`d3d12_formats_results.zip` in that folder.

## 2. The game

As before: unzip into the PlayOnlineViewer folder so that `d12_15nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\d12_15nov"
    start.bat d12

1. The same busy place as last time. `mark crowd`: stand still, camera still, 2 minutes. **Smooth, not pixelated?
   Shadows right?**
2. `mark run`: run around, turning the camera, 1 minute. **Shadows (yours and others') right?**
3. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `d12_15nov_results.zip` and
`check\d3d12_formats_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
