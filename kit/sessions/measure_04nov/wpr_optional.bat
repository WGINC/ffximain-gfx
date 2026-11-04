@echo off
rem wpr_optional.bat -- OPTIONAL, during phase W1 only, as administrator: 20 s of Windows' own trace (every process's
rem CPU samples, context switches, the GPU's queue) with the built-in Windows Performance Recorder. The file is large
rem (several hundred MB); skip it if uploading that is a problem.
setlocal
set "HERE=%~dp0"
set "KIT=%HERE%"
if not exist "%KIT%xikit.dll" set "KIT=%HERE%..\"
for %%k in ("%KIT%.") do set "KIT=%%~fk"
wpr -cancel >nul 2>&1
wpr -start CPU -start GPU -filemode
if errorlevel 1 goto nowpr
echo Recording 20 s: stand still in the plaza.
timeout /t 20 /nobreak
wpr -stop "%KIT%\xikit_logs\wpr_W1.etl" "W1 clean, standing"
echo Written "%KIT%\xikit_logs\wpr_W1.etl"
exit /b 0
:nowpr
echo wpr could not start: run this as administrator.
exit /b 1
