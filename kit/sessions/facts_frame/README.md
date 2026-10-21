# Facts and Frame_Render captures: one session

This session measures what the fuzz gate cannot settle in the functions a frame runs. It also records two real
calls of `Frame_Render`. Nothing of ours runs in the game this time:
- `swap=0`: no reconstructed function replaces the game's;
- `mods=` (empty): speedfix is not loaded.

So the game runs as shipped, at its own frame rate. The kit only watches.

It answers two things:

- **Facts (`facts=1`).** For 2,240 call sites in the frame's functions, the kit records:
  - what each call really pops;
  - where it goes. For a call through a pointer, that means the vtable and slot.

  Most are indirect calls whose convention no static source settles; 218 go through the object at
  `[0x1066941C]`. Once ingested (`pipeline/fact_sites.py ingest`), every site seen with one consistent answer
  becomes a fact the gate reads.
  - A site that is never reached in your session stays unmeasured. So the more of the game you touch, the more
    gets settled.
  - 30 sites cannot be patched safely, and are left out by the kit's own rules:
    - something jumps into the bytes before the call;
    - or there are fewer than 5 bytes of plain instructions before the call.
- **Captures (`capture=Frame_Render`).** Two real calls of `Frame_Render`, 120 seconds after the start. Each is
  the registers, the stack and every page the call can reach. The gate replays them to check the
  `uncap`/`frame_step` forks on the game's real state (`kit/speedfix/test/test_framestep.py CAPTURES=dir`), where
  fuzzed inputs never reach the frame's wait.

## What to copy (into the kit folder, next to xikit.dll)

- From the kit: `xikit.dll`, `gfx_recon_ingame.dll`, `xikit_manifest.txt`, `xikit_manifest_6AC37846.txt`,
  `xikit_facts.txt` and `VERSION.txt`.
- `xikit.ini` from this folder. Keep your own aside first. It is the kit's default with four changes:
  - `facts=1`;
  - `capture=Frame_Render`;
  - `capture_count=2`;
  - `capture_after=120`.
- Leave `speedfix.dll` and `speedfix.ini` where they are. With `mods=` empty, the kit does not load speedfix.

## The run (about 30 minutes)

1. Start the game. In `xikit.log`, check two lines:
   - `facts=1: N call sites to measure`;
   - `capture Frame_Render (0x10011A00): 2 call(s), from 120 s after the start ...`.
2. **About 2 minutes after the start, the game freezes for a moment, twice.** Those are the two captures: every
   other thread is suspended while up to 80 MB are copied. `xikit.log` says `capture 1 of Frame_Render ...` and
   `capture 2 ...`. If it freezes for more than a minute or crashes, close it and send the log folder anyway.
3. Then play for about 25 minutes, touching as much of the game as you can. Each of these runs different code:
   - stand in a busy town, then walk through the crowd, turning the camera;
   - open the menus: main menu, map, equipment, items, config, and the chat log;
   - change zone at least once;
   - fight something, with spells or abilities that have effects;
   - a cutscene or event, if one is handy;
   - weather or a day/night change, if you are out in the field.
4. Close the game normally. `xikit_facts_<pid>.csv` is rewritten every 600 frames, so a crash loses at most the
   last ten seconds or so.
5. Zip the log folder and send it. Put your own `xikit.ini` back. The folder holds `xikit.log`,
   `xikit_facts_<pid>.csv` and the two `xikit_capture_<pid>_11A00_<n>.xcap` files, each up to 80 MB.

## What comes of it

- `fact_sites.py ingest xikit_facts_<pid>.csv` adds each agreeing site to `data/game_sp.json`. Then:
  - the frame set is gated again;
  - functions stopped `UNRESOLVED` at a measured site get a verdict;
  - indirect calls with a measured target get that function's own facts.
- A site observed with two different pop counts is not ingested. The report lists it, as a call whose callee
  differs between calls: a virtual call on objects of different classes.
- The captures run through `test_framestep.py` and `pipeline/xcap.py`: the original and the reconstruction from the
  same real call, with how much of the original's code ran.
