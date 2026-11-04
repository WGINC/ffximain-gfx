@echo off
rem start_phase.bat PHASE -- one phase of the measurement session (README.md). Run it from this folder, which sits
rem inside the folder of xikit.dll (the PlayOnlineViewer folder); "Run as administrator" lets PresentMon record.
rem It puts the phase's settings next to xikit.dll, starts xiload (it loads the kit into pol.exe when the game starts)
rem and, once pol.exe is running, PresentMon (every Present, the GPU's busy time, into xikit_logs).
setlocal
set PH=%~1
set HERE=%~dp0
for %%k in ("%HERE%..") do set KIT=%%~fk
if "%PH%"=="" goto usage
if not exist "%HERE%%PH%\xikit.ini" goto usage
if not exist "%KIT%\xikit.dll" ( echo xikit.dll is not in %KIT%: put this folder inside the folder of xikit.dll & exit /b 1 )
copy /y "%HERE%%PH%\xikit.ini" "%KIT%\xikit.ini" >nul
if exist "%HERE%%PH%\speedfix.ini" copy /y "%HERE%%PH%\speedfix.ini" "%KIT%\speedfix.ini" >nul
>"%KIT%\xikit_phase.txt" echo %PH%
if not exist "%KIT%\xikit_logs" mkdir "%KIT%\xikit_logs"
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul && ( echo pol.exe is already running: close the game first & exit /b 1 )
echo Phase %PH%: settings copied. Starting xiload; now start the game as you always do.
start "xiload" /min "%KIT%\xiload.exe" --once
:wait
timeout /t 2 /nobreak >nul
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul || goto wait
for /f %%t in ('powershell -NoProfile -Command "Get-Date -Format yyyyMMdd_HHmmss"') do set TS=%%t
set PMO=%KIT%\xikit_logs\presentmon_%PH%_%TS%.csv
if exist "%HERE%tools\PresentMon-2.3.0-x64.exe" (
  start "PresentMon %PH%" /min "%HERE%tools\PresentMon-2.3.0-x64.exe" --process_name pol.exe --output_file "%PMO%" --qpc_time --terminate_on_proc_exit --no_console_stats
  echo PresentMon is recording into %PMO% until the game closes.
) else echo tools\PresentMon-2.3.0-x64.exe not found: no PresentMon this phase.
echo Play the phase's steps (README.md), pressing Ctrl+Alt+F7 at the start of each. Close the game when done.
exit /b 0
:usage
echo usage: start_phase.bat W1_clean ^| W2_where ^| W2b_native ^| W3_deep ^| W4_d3d ^| W5_levers ^| W6_ab
exit /b 2
