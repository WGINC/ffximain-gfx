@echo off
rem uninstall.bat [FOLDER] -- Longthinx gobbiefix: removes what install.bat put in the PlayOnlineViewer folder and puts
rem back the d3d8.dll that was there (d3d8_before_kit.dll). The .ini files and logs stay. Run it as administrator.
setlocal
set "SRC=%~dp0files"
set "DST=%~1"
if "%DST%"=="" set "DST=C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer"
if not exist "%DST%\kit_installed.txt" goto notinstalled
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul
if not errorlevel 1 goto running
pushd "%SRC%"
for %%f in (*.dll *.exe *.txt) do if exist "%DST%\%%f" del /q "%DST%\%%f"
popd
if exist "%DST%\gfx_recon_ingame.dll" del /q "%DST%\gfx_recon_ingame.dll"
if exist "%DST%\d3d8_before_kit.dll" move /y "%DST%\d3d8_before_kit.dll" "%DST%\d3d8.dll" >nul
del /q "%DST%\kit_installed.txt"
if exist "%DST%\gobbiefix_debug.on" del /q "%DST%\gobbiefix_debug.on"
echo  Pshhh... gobbie packpack the shinybits. Old d3d8.dll come back outta the sack.
echo  Shkohhh... gobbie leave the .ini files and logs, in case youuu want gobbie back.
echo  Byebye, adventurer! Pshhh...
exit /b 0
:notinstalled
echo  Pshhh... gobbie no smell gobbiefix in "%DST%" (no kit_installed.txt). Nothing to take.
exit /b 2
:running
echo  Shkohhh! Game still running! Youuu close it first, then gobbie workwork.
exit /b 2
