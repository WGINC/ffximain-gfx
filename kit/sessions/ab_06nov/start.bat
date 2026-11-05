@echo off
rem start.bat [play ^| sites] -- a start of the 6 Nov session (README.md); play when no name is given. Run it from an administrator
rem Command Prompt in this folder (PresentMon needs it). It puts the start's settings next to xikit.dll.
rem It also puts d3d8on11 in place of dgVoodoo's d3d8.dll (kept as d3d8_dgvoodoo_keep.dll; restore.bat puts it back).
rem Then it starts xiload (it loads the kit into pol.exe; its window keeps watching: close it after the start) and,
rem once pol.exe runs, PresentMon. (No parenthesised block holds a folder path: "Program Files (x86)" would end it.)
setlocal
set "PH=%~1"
if "%PH%"=="" set "PH=play"
call "%~dp0_kit.bat"
if "%PH%"=="" goto usage
if not exist "%HERE%%PH%\xikit.ini" goto usage
if not exist "%KIT%\xikit.dll" goto nokit
call :running
if not errorlevel 1 goto already
copy /y "%HERE%%PH%\xikit.ini" "%KIT%\xikit.ini" >nul
if exist "%HERE%%PH%\speedfix.ini" copy /y "%HERE%%PH%\speedfix.ini" "%KIT%\speedfix.ini" >nul
>"%KIT%\xikit_phase.txt" echo %PH%
if not exist "%KIT%\xikit_logs" mkdir "%KIT%\xikit_logs"
if exist "%KIT%\d3d8_dgvoodoo_keep.dll" goto copyours
powershell -NoProfile -Command "$i=(Get-Item -LiteralPath '%KIT%\d3d8.dll').VersionInfo; if ($i.FileMajorPart -eq 4 -and $i.FileMinorPart -eq 8) { exit 0 } else { Write-Host ('d3d8.dll there is version ' + $i.FileMajorPart + '.' + $i.FileMinorPart + '.' + $i.FileBuildPart + '.' + $i.FilePrivatePart + ', not dgVoodoo 4.8.x: left as it is'); exit 1 }"
if errorlevel 1 goto notdgv
ren "%KIT%\d3d8.dll" d3d8_dgvoodoo_keep.dll
echo dgVoodoo's d3d8.dll kept as d3d8_dgvoodoo_keep.dll
:copyours
copy /y "%HERE%d3d8on11\d3d8.dll" "%KIT%\d3d8.dll" >nul
copy /y "%HERE%d3d8on11\d8x.exe" "%KIT%\d8x.exe" >nul
copy /y "%HERE%d3d8on11\d8x_arm64.exe" "%KIT%\d8x_arm64.exe" >nul
copy /y "%HERE%%PH%\d3d8on11.ini" "%KIT%\d3d8on11.ini" >nul
echo d3d8on11 in place.
:launch
rem d3d8on11 writes d3d8on11.log anew at each start: the last start's is kept in xikit_logs (collect.bat takes it)
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set "TS0=%%t"
if exist "%KIT%\d3d8on11.log" move /y "%KIT%\d3d8on11.log" "%KIT%\xikit_logs\d3d8on11_before_%PH%_%TS0%.log" >nul
echo %PH%: settings copied. Starting xiload; now start the game as you always do.
start "xiload" /min "%KIT%\xiload.exe"
:wait
timeout /t 2 /nobreak >nul
call :running
if errorlevel 1 goto wait
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set "TS=%%t"
set "PMO=%KIT%\xikit_logs\presentmon_%PH%_%TS%.csv"
if not exist "%HERE%tools\PresentMon-2.3.0-x64.exe" goto nopm
start "PresentMon %PH%" /min "%HERE%tools\PresentMon-2.3.0-x64.exe" --process_name pol.exe --output_file "%PMO%" --qpc_time --terminate_on_proc_exit --no_console_stats
echo PresentMon is recording until the game closes.
goto play
:nopm
echo tools\PresentMon-2.3.0-x64.exe not found: no PresentMon this start.
:play
echo The game is running. Follow README.md for %PH%; type mark NAME here at the start of each step.
exit /b 0
:running
tasklist /nh /fi "imagename eq pol.exe" 2>nul | find /i "pol.exe" >nul
exit /b %errorlevel%
:already
echo pol.exe is already running: close the game first, then run this again.
exit /b 1
:nokit
echo xikit.dll was not found next to this folder or in the folder above it.
exit /b 1
:notdgv
echo Nothing was changed. Tell Claude what d3d8.dll is in that folder.
exit /b 1
:usage
echo usage: start.bat [play ^| sites]
exit /b 2
