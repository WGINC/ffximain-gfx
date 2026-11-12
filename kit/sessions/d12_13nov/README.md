# The 13 Nov session: Direct3D 11 against Direct3D 12, in the same crowd

Two starts on Windows (Parallels), about 10 minutes. The same kit, the same settings; only the executor differs:
start 1 draws through Direct3D 11 as before, start 2 through d3d8on11's new Direct3D 12 executor.

## Why

Your probe12 run (12 Nov) showed Direct3D 12 on this Mac, and the same 2,000 test draws taking 2.0 ms there against
6.1 ms through Direct3D 11. Whether the game's real frames gain the same is what these two starts measure: the draws,
the Present and the frame time, in the same crowd.

## What is new in start 2

- d3d8on11 runs everything it draws on Direct3D 12 (its log's first lines say "backend=d3d12" and "D3D12 executor:
  device made"). If Direct3D 12 cannot start, it says so and draws through Direct3D 11 instead.
- If the graphics device is lost (as on 18 Nov and 12 Nov through Direct3D 11), the Direct3D 12 executor logs the
  reason and stops waiting for the GPU, so the game should not hang on it (it will stop drawing).
- The picture should look the same. Under Wine, 28 of the 33 test scenes are pixel for pixel what Direct3D 11 draws;
  the other 5 differ only at single pixels where a texture is read exactly on the border between two texels, where
  either texel is allowed. **Anything that looks different on your Mac is worth a note** (what, where).

## Before you start

Unzip into the PlayOnlineViewer folder so that `d12_13nov` sits inside it, copy the zip's `kit` files next to
`xikit.dll` (replace the old ones). Open a Command Prompt **as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\d12_13nov"

`start.bat` copies this folder's own d3d8on11 into place each time (no files to copy by hand this time).

### Start 1: Direct3D 11 (about 4 minutes)

    start.bat d11

1. Go to the busiest place you know (Port Jeuno plaza or wherever is most crowded).
2. `mark crowd`: stand still, camera still, 2 minutes.
3. `mark run`: run around the same place, turning the camera, 1 minute.
4. Close the game normally.

### Start 2: Direct3D 12 (about 4 minutes)

    start.bat d12

5. The same place. `mark crowd`: stand still, camera still, 2 minutes. **Does it look the same as in start 1?**
6. `mark run`: run around, turning the camera, 1 minute. **Smoother, the same, or worse?**
7. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `d12_13nov_results.zip` with your notes.

If the game crashes or hangs in start 2: wait about 40 seconds, close it, run `collect.bat` at once (before another
start), and say at which step.
