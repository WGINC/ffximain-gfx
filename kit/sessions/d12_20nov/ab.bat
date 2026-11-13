@echo off
rem ab.bat -- start adv only: speedfix compares occlusion_first=0 (A) with occlusion_first=2 (B) by itself, starting 5
rem seconds from now: 20 seconds each, A B B A, twice (about 3 minutes; speedfix.ini ab_*). Stand still until the
rem Command Prompt says it is done. The results are the 'speedfix: ab' lines in xikit.log.
setlocal
call "%~dp0_kit.bat"
call "%~dp0mark.bat" ab occlusion_first
powershell -NoProfile -Command "$p='%KIT%\speedfix.ini'; (Get-Content -LiteralPath $p) -replace '^ab_levers=.*$','ab_levers=occlusion_first' | Set-Content -LiteralPath $p"
echo %date% %time% A/B of occlusion_first started; stand still for about 3 minutes.
timeout /t 170 /nobreak
echo %date% %time% done.
