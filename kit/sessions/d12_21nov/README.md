# The 21 Nov session: the names' sampling, advanced shadows on Direct3D 11 against 12, and one captured shadow frame

Four starts of the game, about 12 minutes.

## What your 20 Nov run showed

- **Advanced shadows:** about 50 ms a frame. The game switched between the screen and its full-size (3024x1898) shadow
  picture about 1,250 times a frame, one pair of switches per character's shadow; with simple shadows 53 to 72. The
  cost follows those switches. Why each switch costs so much on Parallels is not known. Two things to find out: is
  Direct3D 11 cheaper at it (starts adv12 and adv11), and does each character's shadow touch only a small part of the
  shadow picture (start advcap: one captured frame I replay here, pass by pass). If neither shows a way, advanced
  shadows wait for another day.
- **occlusion_first=2:** no change in the frame rate, but with it off the game froze once for 209 ms on its occlusion
  readback; with it on, never. It stays on.
- **Mip levels:** after the loading, a few milliseconds of building now and then. The names pulse with and without mip
  levels, so that is the game's own sampling: start text tries a smoother sampling of the names.

## Before you start

As before: unzip into the PlayOnlineViewer folder so that `d12_21nov` sits inside it, copy the zip's `kit` files next
to `xikit.dll`. Command Prompt **as administrator**, in `d12_21nov`.

## 1. The names (`start.bat text`)

Your usual settings (simple shadows), the same street as last time. `mark text`, 1 minute walking and turning: do the
names still pulse, and do near names look too soft? Close the game, `restore.bat`.

## 2. Advanced shadows, Direct3D 12 (`start.bat adv12`)

Advanced shadows on everyone, a busy place. `mark adv12`, 1 minute standing still, then 1 minute walking. Close the
game, `restore.bat`.

## 3. Advanced shadows, Direct3D 11 (`start.bat adv11`)

The same place, as close to the same crowd as you can. `mark adv11`, 1 minute standing still, then 1 minute walking.
Close the game, `restore.bat`.

## 4. One captured frame (`start.bat advcap`)

Advanced shadows, the same place, with several characters in view. This start skins characters on the CPU (needed for
the capture), so it may run slower: that is expected. Stand still, then `capture.bat shadows` (the game pauses a
moment; the file may be large). Close the game, `restore.bat`.

Then close the xiload window, `collect.bat`, and upload `d12_21nov_results.zip` with your notes.

If the game crashes or hangs: wait about 40 seconds, close it, run `collect.bat` at once, and say at which step.
