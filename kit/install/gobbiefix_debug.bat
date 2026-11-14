@echo off
rem gobbiefix_debug.bat on|off [FOLDER] -- Longthinx gobbiefix: debug mode. on: makes the file gobbiefix_debug.on in
rem the PlayOnlineViewer folder (FOLDER, or the usual place); off: deletes it. At the next start d3d8.dll sees it (as
rem debug=1 in gobbiefix.ini) and puts the statistics lines in the logs (d3d8on11.ini log_stats=1, speedfix.ini
rem stats_every=60); without it, and debug=0, they stay out. Takes effect at the game's next start.
setlocal
set "V="
if /i "%~1"=="on" set "V=1"
if /i "%~1"=="off" set "V=0"
if not defined V goto usage
set "DST=%~2"
if "%DST%"=="" set "DST=C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer"
if not exist "%DST%\kit_installed.txt" goto noini
if "%V%"=="1" >"%DST%\gobbiefix_debug.on" echo gobbiefix debug mode: d3d8.dll sees this file and writes the statistics lines
if "%V%"=="0" if exist "%DST%\gobbiefix_debug.on" del /q "%DST%\gobbiefix_debug.on"
if "%V%"=="1" if not exist "%DST%\gobbiefix_debug.on" goto failed
if "%V%"=="0" if exist "%DST%\gobbiefix_debug.on" goto failed
if "%V%"=="1" echo  Debug on! Gobbie scribble lots of stats in d3d8on11.log and xikit.log next game start.
if "%V%"=="0" echo  Debug off. Gobbie keep logs short and tidy next game start.
echo  (Logs is in "%DST%".)
exit /b 0
:usage
echo  Youuu tell gobbie: gobbiefix_debug.bat on   or   gobbiefix_debug.bat off
exit /b 2
:noini
echo  Gobbie no find gobbiefix in "%DST%". Install gobbiefix first (install.bat).
exit /b 2
:failed
echo  Gobbie no could write in the game folder. Run this as administrator.
exit /b 2
