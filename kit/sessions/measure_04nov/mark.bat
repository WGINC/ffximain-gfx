@echo off
rem mark.bat [NAME] -- start the next step of the phase: the kit numbers the frames from here on with the next mark
rem (within half a second; xikit.log says "measure: mark N"). Run it from the same folder as start_phase.bat.
setlocal
set "HERE=%~dp0"
set "KIT=%HERE%"
if not exist "%KIT%xikit.dll" set "KIT=%HERE%..\"
for %%k in ("%KIT%.") do set "KIT=%%~fk"
set "NAME=%*"
if not defined NAME set "NAME=step"
>"%KIT%\xikit_mark.txt" echo %NAME%
echo %date% %time% mark written: %NAME%
