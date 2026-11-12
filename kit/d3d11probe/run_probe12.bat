@echo off
rem run_probe12.bat -- (12 Nov) is there D3D12 on this machine, and do more threads shorten a frame of 2,000 draws?
rem Close the game first. Put this folder anywhere and run this. About 2 minutes; nothing opens. Then send the
rem probe12_... folder it makes (two .csv files).
cd /d "%~dp0"
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set T=%%t
set O=probe12_%T%
mkdir "%O%"
echo x86 (the game's architecture, emulated)... & probe12_x86.exe "%O%"
echo ARM64 (native)... & probe12_arm64.exe "%O%"
echo Done. Send the folder %O%.
pause
