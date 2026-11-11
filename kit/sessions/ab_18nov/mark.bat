@echo off
rem mark.bat [NAME] -- the next step starts: the kit numbers the frames from here on with the next mark.
setlocal
call "%~dp0_kit.bat"
set "NAME=%*"
if not defined NAME set "NAME=step"
>"%KIT%\xikit_mark.txt" echo %NAME%
echo %date% %time% mark written: %NAME%
