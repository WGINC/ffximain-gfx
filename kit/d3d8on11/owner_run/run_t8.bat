@echo off
rem run_t8.bat -- t8 (d3d8on11's test program) through three Direct3D 8 implementations on this machine: the game's
rem d3d8.dll (dgVoodoo's, in the folder above this one), Windows' own, and ours. Put this folder inside the
rem PlayOnlineViewer folder (where the game's d3d8.dll and dgVoodoo.conf are), close the game, and run this. Small
rem windows open and close by themselves for a few minutes. Then send the t8run_... folder it makes.
cd /d "%~dp0"
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set T=%%t
set O=%~dp0t8run_%T%
mkdir "%O%"
if not exist "..\d3d8.dll" (
  echo No d3d8.dll in the folder above this one: put this folder inside the PlayOnlineViewer folder.
  pause
  exit /b 1
)
rem each implementation in a folder of its own, with t8 and the cases beside it
for %%d in (dgv win ours_direct ours_remote ours_warp ours_debug) do (
  mkdir "%O%\%%d"
  copy /y t8.exe "%O%\%%d\" >nul
  copy /y t8_cases*.txt "%O%\%%d\" >nul
)
copy /y "..\d3d8.dll" "%O%\dgv\" >nul
if exist "..\dgVoodoo.conf" copy /y "..\dgVoodoo.conf" "%O%\dgv\" >nul
for %%d in (ours_direct ours_remote ours_warp ours_debug) do copy /y ours\*.* "%O%\%%d\" >nul
set D8ON11_MODE=
call :run dgv
call :run win
set D8ON11_MODE=direct
call :run ours_direct
set D8ON11_MODE=remote
call :run ours_remote
rem ours again on Microsoft's software rasterizer (WARP) instead of the GPU, then with the D3D11 debug layer (its
rem messages go to d3d8on11.log; it needs Windows' optional "Graphics Tools" feature, or the device is not made)
set D8ON11_MODE=direct
set D8ON11_DRIVER=warp
call :run ours_warp
set D8ON11_DRIVER=
set D8ON11_DEBUG_LAYER=1
call :run ours_debug
set D8ON11_DEBUG_LAYER=
set D8ON11_MODE=
echo Done. Send the folder %O%.
pause
exit /b 0

:run
echo %1...
pushd "%O%\%1"
t8.exe where > where.txt 2>&1
t8.exe basic > basic.txt 2>&1
t8.exe defaults > defaults.txt 2>&1
t8.exe scenes > scenes.txt 2>&1
for %%s in (1 2 3) do t8.exe random t8_cases%%s.txt > random%%s.txt 2>&1
popd
exit /b 0
