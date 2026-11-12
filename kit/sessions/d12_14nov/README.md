# The 14 Nov session: Direct3D 12 again, with the picture fix

One start on Windows (Parallels), about 5 minutes.

## What your 13 Nov run showed

- **Speed:** in the crowd d3d8on11's worker took 11-13 ms a frame on Direct3D 12 against 17.5-19.7 ms on Direct3D 11
  (the same places, the same number of draws). Direct3D 12 is the faster path on your Mac.
- **The picture on Direct3D 12 was wrong**, and why is not proven yet. What it looks like: the texture settings
  (smooth filtering, and "clamp at the edge") were ignored, so textures were read pixel by pixel and repeated past
  their edges. Your game draws its 3D scene at twice the window's size (your background resolution, 3024x1898) and
  shrinks it; shrunk without smoothing, that gives exactly "pixelated, jittering edges, colours a bit off". Small
  shadow textures read the same way speckle into dots; the projected shadow repeating instead of stopping at its
  edge moves around inside its box.
- Under Wine here it draws right, so it is something Parallels' Direct3D 12 does differently.

## What this build does about it

At the start, before the game draws anything, d3d8on11 now draws two test pixels the way the game's draws are made:
one smooth-filtered, one clamped at a texture's edge. It tries two ways of handing the texture settings to the
driver and keeps the first that gives the right pixels. If neither does, it uses Direct3D 11 for the whole start
(as smooth as your start 1) and the log says so. Either way the log line "D3D12 sampler check" tells us what your
Mac does.

## Also in this build (the next steps, measured in this start)

- **Where the GPU's time goes:** the log now has the GPU's time for each frame, split by what it draws into (your
  double-size scene, the screen, the rest), every 10 seconds or so. That says whether the double-size scene or the
  number of draws is what costs.
- **Fewer draws:** draw merging now works on Direct3D 12 (on in this start, as before): the many draws of a crowd that
  differ only in which model part they draw are sent as one. In your 13 Nov crowd about 1,040 of 1,650 draws a frame
  were of that kind.
- **Fewer hitches next time:** the shaders and pipeline states this start makes are saved next to d3d8on11 (two files,
  d3d8on11_d3d12_*.bin) and loaded at the next start, so a place you have been before does not compile them again.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_14nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\d12_14nov"
    start.bat d12

1. The same busy place as last time. `mark crowd`: stand still, camera still, 2 minutes. **Does the picture look like
   start 1 of last time (smooth, not pixelated)? Shadows right?**
2. `mark run`: run around, turning the camera, 1 minute. **Shadows (yours and others') right?**
3. Close the game normally.

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `d12_14nov_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
