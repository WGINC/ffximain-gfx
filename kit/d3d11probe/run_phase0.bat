@echo off
rem run_phase0.bat -- phase 0 of the D3D11 renderer (docs/D3D11.md). Put this folder's files in the PlayOnlineViewer
rem folder (where the game's d3d8.dll, dgVoodoo's, is), close the game, and run this. About 5 minutes; windows open
rem and close by themselves. Then send the phase0_... folder it makes.
cd /d "%~dp0"
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set T=%%t
set O=phase0_%T%
mkdir "%O%"
echo caps... & probe11_x86.exe caps "%O%" & probe11_arm64.exe caps "%O%"
echo D3D11 bench, x86 then ARM64... & probe11_x86.exe bench "%O%" & probe11_arm64.exe bench "%O%"
echo D3D8 bench through this folder's d3d8.dll... & probe8_x86.exe bench "%O%"
echo two processes... & probe11_x86.exe xproc "%O%" probe11_arm64.exe & probe11_arm64.exe xproc "%O%" probe11_x86.exe
echo Done. Send the folder %O%.
pause
