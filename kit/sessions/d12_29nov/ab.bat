@echo off
rem ab.bat -- speedfix compares, by itself, starting 5 seconds from now (30 seconds each, A B B A twice per group):
rem   step_exact off (A) and on (B), four rounds.
rem About 4 minutes. Keep walking and turning the camera along your usual route until this says done.
rem Each switch is a mark in the frame log (speedfix writes it); the results are the 'speedfix: ab' lines in xikit.log.
setlocal
call "%~dp0_kit.bat"
call "%~dp0mark.bat" ab start
rem speedfix starts an A/B when ab_levers changes; after one has finished, the same value again is no change. So it is
rem emptied first, and set again once speedfix has read the empty one (it reads the ini every 60 frames: a second is plenty)
powershell -NoProfile -Command "$p='%KIT%\speedfix.ini'; (Get-Content -LiteralPath $p) -replace '^ab_levers=.*$','ab_levers=' | Set-Content -LiteralPath $p"
timeout /t 3 /nobreak >nul
powershell -NoProfile -Command "$p='%KIT%\speedfix.ini'; (Get-Content -LiteralPath $p) -replace '^ab_levers=.*$','ab_levers=step_exact' | Set-Content -LiteralPath $p"
echo %date% %time% A/B started; keep walking and turning the camera for about 4 minutes.
timeout /t 260 /nobreak
echo %date% %time% done.
