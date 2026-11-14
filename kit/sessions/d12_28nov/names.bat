@echo off
rem names.bat 0|1 -- while the game runs: name_smooth off (0, the game's 80 size steps) or on (1). speedfix reads its
rem ini again when it changes. Marked in the frame log.
setlocal
call "%~dp0_kit.bat"
if "%~1"=="" goto usage
call "%~dp0mark.bat" names %~1
powershell -NoProfile -Command "$p='%KIT%\speedfix.ini'; (Get-Content -LiteralPath $p) -replace '^name_smooth=.*$','name_smooth=%~1' | Set-Content -LiteralPath $p"
echo %date% %time% name_smooth=%~1
exit /b 0
:usage
echo usage: names.bat 0   or   names.bat 1
exit /b 2
