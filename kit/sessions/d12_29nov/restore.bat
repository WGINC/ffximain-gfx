@echo off
rem restore.bat -- dgVoodoo's d3d8.dll back (from d3d8_dgvoodoo_keep.dll), d3d8on11's files and settings removed.
setlocal
call "%~dp0_kit.bat"
rem xiload (started by start.bat) keeps watching for the game: closed here, so a later start without start.bat
rem (the installer) is not loaded by it too (23 Nov: it was)
taskkill /im xiload.exe /f >nul 2>&1 && echo xiload closed.
if not exist "%KIT%\d3d8_dgvoodoo_keep.dll" goto none
copy /y "%KIT%\d3d8_dgvoodoo_keep.dll" "%KIT%\d3d8.dll" >nul
del "%KIT%\d3d8_dgvoodoo_keep.dll"
if exist "%KIT%\d3d8on11.ini" del "%KIT%\d3d8on11.ini"
echo dgVoodoo's d3d8.dll is back.
exit /b 0
:none
if not "%~1"=="quiet" echo d3d8_dgvoodoo_keep.dll not found: d3d8.dll was not replaced by this session.
exit /b 0
