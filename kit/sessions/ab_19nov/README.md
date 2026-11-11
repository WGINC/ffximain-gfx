# The 19 Nov session: the 18 Nov kit with the vanishing characters fixed, and the stutters put to the test

Two starts on Windows (Parallels), about 14 minutes. Use this instead of ab_18nov.

## What your 18 Nov run showed

- **The vanishing characters were a bug in d3d8on11's GPU skinning, now fixed.** While a GPU-skinned character's
  buffers were still selected, d3d8on11 dropped every draw the game makes from its own memory. The game's occlusion
  test draws its test rectangles that way, so every tested character counted as hidden and the game stopped drawing
  it. Your own character stayed, so the game does not hide it on that test (where in its code that is decided I
  have not traced). The test suite now makes such a draw after a
  skinned one (it failed on the 18 Nov build and passes on this one). Whether anything else goes wrong with GPU
  skinning in the game is not known until this run: the 18 Nov run never got far enough to show it.
- **The freeze and crash in `mark run`:** the graphics device was taken away from the game (Direct3D 11 error
  0x887A0005) and nothing could be drawn after it. Why is not known: the log did not record the device's reason.
  This build records it. It happened once, in start 1; start 3 ran the same settings for four minutes without it.
- **The stutters while running (start 3's samples, 191 frames of 33 ms or more in 145 s):**
  - 97 of them (about 40 ms each): the game's thread was waiting inside **Windower's Lua runtime (LuaCore.dll)**,
    called by Windower's Hook.dll from the game's code that handles a packet from the server. That is an addon
    doing its work when a packet arrives. Which addon is not in the samples. The 5 Nov recording has the same
    (43 frames).
  - 84 of them (about 36 ms each): the game waiting for the answer to a character's **first** occlusion test
    (speedfix counted 241 such waits in that minute). The setting `occlusion_first=2` answers a first test
    "visible" at once. On 16 Nov it did not reduce the long frames; that run could not tell the two causes apart,
    this one can.
  - Both counts come from a start in which the bug above was hiding characters, so they need this clean run.
- **The shimmering in the distance and the nameplates jittering while the camera moves** were there in start 2 as
  well, with none of the new pieces on. So they do not come from merging, delta draws or GPU skinning. Their cause
  is not known yet.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `ab_19nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll` (replace the old ones), keep your own `xikit.ini` and `speedfix.ini` aside. Open a Command Prompt
**as administrator**:

    cd /d "C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer\ab_19nov"

If the game crashes or freezes, close it, note which step, and go on to the next start: the log is kept.

### Start 1: everything on (about 7 minutes)

    start.bat all

1. `mark plaza`: Port Jeuno plaza, standing, camera still, 60 s. **Are the other characters there and do they stay?
   Drawn right (bodies, faces, weapons, shadows)?**
2. `abtest skin`: stay there. The window says when the GAME skins the characters (30 s) and when d3d8on11 does
   (30 s), twice. **Does anything change when it switches: a character's shape, shadows, anything missing?**
3. `abtest merge`: stay there. Draw merging goes OFF (30 s), ON (60 s), OFF (30 s). **Does anything change?**
4. `mark run`: run around the plaza, turning the camera, 60 s. **Characters popping in or out? Stutters?**
5. `mark crowd`: the busiest place nearby, 60 s. Close the game normally.

### Start 2: the stutters (about 7 minutes)

    start.bat hitch

6. `mark run`: run around the plaza, turning the camera, 60 s.
7. `abtest first`: keep running and turning. The window says when a first occlusion test WAITS (30 s) and when it is
   answered VISIBLE at once (30 s), twice. **Fewer hitches in the second kind? Anything flashing for a frame?**
8. Unload your Windower addons (in the game's chat line: `//lua unloadall`; if that is not the command on your
   Windower, unload them the way you know, or skip this step and say so). Then `mark noaddons` and run around as
   in step 6 for 60 s. **Fewer hitches?** Close the game normally (the addons load again at the next start).

Then `restore.bat`, close the xiload window, `collect.bat`, and upload `ab_19nov_results.zip` with your notes.

## What it settles

- **Whether GPU skinning now draws every character, and what it, merging and delta draws are worth:** steps 1 to 5;
  d3d8on11's log counts the draws it did not make (0 is right) and its worker's and the game thread's time.
- **Whether the device is lost again, and why:** the log now has the device's own reason.
- **How many long frames each stutter cause makes, and what removing it does:** step 7 against step 6 (first
  tests), step 8 against step 6 (addons). The samples name where the game's thread was in every long frame.
