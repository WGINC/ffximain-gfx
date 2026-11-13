@echo off
rem capture.bat [NAME] -- start 1 only: the next frame is captured whole (a file of some tens of MB; the game pauses
rem for a moment), and marked with NAME.
setlocal
call "%~dp0_kit.bat"
set "NAME=%*"
if not defined NAME set "NAME=capture"
>"%KIT%\xikit_mark.txt" echo capture %NAME%
timeout /t 1 /nobreak >nul
>"%KIT%\xikit_fcap_now.txt" echo %NAME%
echo %date% %time% capture requested: %NAME%
