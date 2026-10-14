# t8 on the owner's machine (run_t8.bat)

What the game's Direct3D 8 implementations draw on the owner's own machine, for kit/d3d8on11 to be compared with:
the game's `d3d8.dll` (dgVoodoo 2.87.5, file version 4.8.2.134, data/dgvoodoo_caps8.json), Windows' own `d3d8.dll`,
and ours (in direct and remote mode). Here, ours is compared with Wine's d3d8 and with Microsoft's DirectX 8.1
runtime and its reference device (test/test_d3d8on11.py), but neither is what the owner runs, and where the reference
leaves behaviour undefined the reference device's choice is only one choice. dgVoodoo's own `d3d8.dll` does not run
under Wine here: it is packed, and its unpacked code faults at the same place on Wine's D3D11 and on DXVK (14 Oct).

## Run (owner)

1. Close the game.
2. Put this folder (in the kit: `t8_compare`) inside the PlayOnlineViewer folder, where the game's `d3d8.dll` and
   `dgVoodoo.conf` are. Nothing in the PlayOnlineViewer folder is changed: the script copies those two files into a
   folder of its own.
3. Run `run_t8.bat`. Small windows open and close for a few minutes.
4. Send the `t8run_<date>_<time>` folder it makes.

## What it runs

For each implementation (`dgv`, `win`, `ours_direct`, `ours_remote`), in a folder of its own: `t8 where` (which
`d3d8.dll` was loaded, its version, the adapter), `t8 basic`, `t8 defaults`, `t8 scenes` and `t8 random` on three
case files (random_cases.py seeds 1 to 3, 200 cases each). `owner_compare.py FOLDER` (here) compares each with ours
scene by scene, with the criterion of the reference-device stage in test_d3d8on11.py.

## What it is for

- The cases where the reference leaves behaviour undefined and the game relies on it. First: a stage whose colour op
  is on and whose alpha op is D3DTOP_DISABLE ("causes undefined behavior", D3DTEXTUREOP). The game does that in a
  quarter of the draws of trace 6796 (from 0x1000CBF3, blending SRCALPHA). Microsoft's reference device takes alpha 1
  there; ours and Wine take the previous stage's alpha. The scenes alphaop_disabled_stage0/1 show which the owner's
  dgVoodoo does.
- Table fog on pre-transformed vertices (scene fog_table_rhw): the game draws them (trace 6796, 255 draws); the
  DirectX 8.1 text, Microsoft's reference device and ours fog them by 1/RHW, Wine by z. Which dgVoodoo does shows.
- The classes of the randomized test whose behaviour is not established (A, C, S, L, F, W; ../README.md "random"),
  as dgVoodoo and Windows draw them.
- Ours on the owner's GPU and driver.
