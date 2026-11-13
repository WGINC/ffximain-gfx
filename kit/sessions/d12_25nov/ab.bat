@echo off
rem ab.bat -- speedfix compares the 120 fps cap off (A) with on (B) by itself, starting 5 seconds from now: 30 seconds
rem each, A B B A, twice (about 4 minutes; speedfix.ini ab_*). Keep walking your usual route until this says done.
rem Each switch is a mark in the frame log (speedfix writes it); the results are the 'speedfix: ab' lines in xikit.log.
setlocal
call "%~dp0_kit.bat"
call "%~dp0mark.bat" ab cap
powershell -NoProfile -Command "$p='%KIT%\speedfix.ini'; (Get-Content -LiteralPath $p) -replace '^ab_levers=.*$','ab_levers=frame_cap_fps' | Set-Content -LiteralPath $p"
echo %date% %time% A/B of the 120 fps cap started; keep walking for about 4 minutes.
timeout /t 250 /nobreak
echo %date% %time% done.
