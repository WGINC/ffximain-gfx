@echo off
rem uninstall.bat [FOLDER] -- removes what install.bat put in the PlayOnlineViewer folder and puts back the d3d8.dll that
rem was there (d3d8_before_kit.dll). Your .ini files and logs stay. Run it as administrator.
setlocal
set "SRC=%~dp0files"
set "DST=%~1"
if "%DST%"=="" set "DST=C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer"
if not exist "%DST%\kit_installed.txt" goto notinstalled
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul
if not errorlevel 1 goto running
for %%f in ("%SRC%\*.dll" "%SRC%\*.exe" "%SRC%\*.txt") do if exist "%DST%\%%~nxf" del /q "%DST%\%%~nxf"
if exist "%DST%\d3d8_before_kit.dll" move /y "%DST%\d3d8_before_kit.dll" "%DST%\d3d8.dll" >nul
del /q "%DST%\kit_installed.txt"
echo Removed. The d3d8.dll from before is back.
exit /b 0
:notinstalled
echo The kit is not installed in "%DST%" (no kit_installed.txt).
exit /b 2
:running
echo The game (pol.exe) is running: close it first.
exit /b 2
