# FFXI graphics kit: install

Installs d3d8on11 (the new renderer), the kit (xikit) and speedfix into the game's PlayOnlineViewer folder. After that
nothing has to be started before the game: the game loads `d3d8.dll` from its folder, and d3d8.dll loads the kit and
speedfix itself.

## Install

1. Close the game.
2. Command Prompt **as administrator**, in this folder: `install.bat` (or `install.bat "D:\path\to\PlayOnlineViewer"`
   if the game is somewhere else).
3. Start the game as you always do.

What was there is kept: the `d3d8.dll` that was in the folder (dgVoodoo's) becomes `d3d8_before_kit.dll`, and an `.ini`
file you already have is not replaced (the kit's defaults go in only where there is none).

## When the game is updated

At each start d3d8.dll reads the game's build (FFXiMain.dll's build stamp) and looks for a manifest for it (the
`xikit_manifest_*.txt` files: where each function the kit changes is in that build). If there is none, the kit and
speedfix are not loaded, the game runs with the new renderer only, and a window says that the kit needs an update and
where to get it (`update_url` in d3d8on11.ini). A new build's manifest is made offline from an unpacked copy of the game
(pipeline/unpack_pol.py, pipeline/build_translate.py); it is not made on your machine.

## Uninstall

`uninstall.bat` (as administrator, the game closed): removes the kit's files and puts the old d3d8.dll back. Your
`.ini` files and the logs stay.

## Logs

In the PlayOnlineViewer folder: `d3d8on11.log` (the last start's previous log is `d3d8on11_prev.log`; a crash is
logged there with where it happened), `xikit.log`.
