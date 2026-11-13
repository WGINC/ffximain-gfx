@echo off
rem install.bat [FOLDER] -- installs the kit into the game's PlayOnlineViewer folder (FOLDER, or the usual place).
rem Once installed, nothing has to be started before the game: the game loads d3d8.dll from that folder, and d3d8.dll
rem loads the kit (and speedfix) itself when it knows the game's build. If the game is a build it does not know, the
rem game runs with the new renderer only and a window says to update the kit.
rem What is there before is kept: the d3d8.dll there (dgVoodoo's) as d3d8_before_kit.dll, and your own .ini files
rem (the kit's defaults go in only where there is none; with /defaults first they replace yours, which are kept as
rem NAME.ini.before_kit). uninstall.bat puts it back. Run it as administrator.
setlocal
set "SRC=%~dp0files"
set "DEF="
if /i "%~1"=="/defaults" (set "DEF=1" & shift)
set "DST=%~1"
if "%DST%"=="" set "DST=C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer"
if not exist "%DST%\pol.exe" if not exist "%DST%\polcore.dll" goto nofolder
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul
if not errorlevel 1 goto running
if exist "%DST%\d3d8_before_kit.dll" goto copyfiles
if not exist "%DST%\d3d8.dll" goto copyfiles
copy /y "%DST%\d3d8.dll" "%DST%\d3d8_before_kit.dll" >nul
echo The d3d8.dll that was there is kept as d3d8_before_kit.dll.
:copyfiles
for %%f in ("%SRC%\*.dll" "%SRC%\*.exe" "%SRC%\*.txt") do copy /y "%%~f" "%DST%\" >nul
if defined DEF for %%f in ("%SRC%\*.ini") do if exist "%DST%\%%~nxf" copy /y "%DST%\%%~nxf" "%DST%\%%~nxf.before_kit" >nul
if defined DEF for %%f in ("%SRC%\*.ini") do copy /y "%%~f" "%DST%\" >nul
for %%f in ("%SRC%\*.ini") do if not exist "%DST%\%%~nxf" copy /y "%%~f" "%DST%\" >nul
>"%DST%\kit_installed.txt" echo installed by kit/install/install.bat
echo Installed into %DST%. Start the game as you always do.
exit /b 0
:nofolder
echo No PlayOnlineViewer folder at "%DST%". Give it: install.bat "C:\path\to\PlayOnlineViewer"
exit /b 2
:running
echo The game (pol.exe) is running: close it first.
exit /b 2
