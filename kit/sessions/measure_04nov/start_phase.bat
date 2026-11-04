@echo off
rem start_phase.bat PHASE -- one phase of the measurement session (README.md). It works from the folder of xikit.dll
rem (the PlayOnlineViewer folder) or from the measure_04nov folder inside it. "Run as administrator" lets PresentMon
rem record. It puts the phase's settings next to xikit.dll, starts xiload (it loads the kit into pol.exe when the game
rem starts) and, once pol.exe is running, PresentMon (every Present, the GPU's busy time, into xikit_logs).
rem (No parenthesised blocks hold a folder path: "Program Files (x86)" would end them.)
setlocal
set "PH=%~1"
set "HERE=%~dp0"
set "KIT=%HERE%"
if not exist "%KIT%xikit.dll" set "KIT=%HERE%..\"
for %%k in ("%KIT%.") do set "KIT=%%~fk"
if "%PH%"=="" goto usage
if not exist "%HERE%%PH%\xikit.ini" goto usage
if not exist "%KIT%\xikit.dll" goto nokit
copy /y "%HERE%%PH%\xikit.ini" "%KIT%\xikit.ini" >nul
if exist "%HERE%%PH%\speedfix.ini" copy /y "%HERE%%PH%\speedfix.ini" "%KIT%\speedfix.ini" >nul
>"%KIT%\xikit_phase.txt" echo %PH%
if not exist "%KIT%\xikit_logs" mkdir "%KIT%\xikit_logs"
call :running
if not errorlevel 1 goto already
echo Phase %PH%: settings copied into "%KIT%".
echo Starting xiload; now start the game as you always do.
start "xiload" /min "%KIT%\xiload.exe" --once
:wait
timeout /t 2 /nobreak >nul
call :running
if errorlevel 1 goto wait
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set "TS=%%t"
set "PMO=%KIT%\xikit_logs\presentmon_%PH%_%TS%.csv"
if not exist "%HERE%tools\PresentMon-2.3.0-x64.exe" goto nopm
start "PresentMon %PH%" /min "%HERE%tools\PresentMon-2.3.0-x64.exe" --process_name pol.exe --output_file "%PMO%" --qpc_time --terminate_on_proc_exit --no_console_stats
echo PresentMon is recording into "%PMO%" until the game closes.
goto play
:nopm
echo tools\PresentMon-2.3.0-x64.exe not found: no PresentMon this phase.
:play
echo Play the phase's steps from README.md, pressing Ctrl+Alt+F7 at the start of each. Close the game when done.
exit /b 0
:running
rem errorlevel 0 while a process named pol.exe runs, 1 otherwise (with no match, tasklist prints only an INFO line)
tasklist /nh /fi "imagename eq pol.exe" 2>nul | find /i "pol.exe" >nul
exit /b %errorlevel%
:already
echo pol.exe is already running: close the game first, then run this again.
exit /b 1
:nokit
echo xikit.dll was not found next to this script or in the folder above it: put the kit files in the PlayOnlineViewer folder.
exit /b 1
:usage
echo usage: start_phase.bat W1_clean ^| W2_where ^| W2b_native ^| W3_deep ^| W4_d3d ^| W5_levers ^| W6_ab
exit /b 2
