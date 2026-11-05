@echo off
rem occl2.bat -- 60 s of occlusion_late=2 (measure only: the game's own answers; speedfix logs how often its late answer
rem would have differed), then start 3's settings back. The ini is written with type (speedfix re-reads it by its time).
setlocal
call "%~dp0_kit.bat"
type "%HERE%hitch\speedfix_occl2.ini" >"%KIT%\speedfix.ini"
timeout /t 2 /nobreak >nul
>"%KIT%\xikit_mark.txt" echo occl2
echo %date% %time% occlusion_late=2 for 60 s
timeout /t 60 /nobreak >nul
type "%HERE%hitch\speedfix.ini" >"%KIT%\speedfix.ini"
>"%KIT%\xikit_mark.txt" echo occl2 done
echo %date% %time% occl2 done: start 3's settings are back.
