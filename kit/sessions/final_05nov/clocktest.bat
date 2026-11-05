@echo off
rem clocktest.bat -- start 2 only, while your character auto-runs in an open field (README.md): four 40 s segments,
rem reference / test / reference / test. Reference: speedfix holds 60 frames a second, frame_step off (each frame is
rem one tick of the game's time). Test: no cap, uncap and frame_step on (the target). The kit records your position
rem every frame; the game's speed in the two is compared afterwards. About 3 minutes; then the target settings are
rem back as before.
rem The ini is written with type, not copied: copy keeps the source file's time, and speedfix re-reads its ini only
rem when the file's time changes (5 Nov run: both inis came from one zip with one time, so no segment switched).
setlocal
call "%~dp0_kit.bat"
for %%s in (reference test reference test) do call :segment %%s
type "%HERE%2_target\speedfix.ini" >"%KIT%\speedfix.ini"
>"%KIT%\xikit_mark.txt" echo clock done
echo %date% %time% clock test done: the target settings are back.
exit /b 0
:segment
if "%1"=="reference" type "%HERE%2_target\speedfix_reference.ini" >"%KIT%\speedfix.ini"
if "%1"=="test" type "%HERE%2_target\speedfix.ini" >"%KIT%\speedfix.ini"
timeout /t 2 /nobreak >nul
>"%KIT%\xikit_mark.txt" echo clock %1
echo %date% %time% clock segment: %1 (40 s)
timeout /t 40 /nobreak >nul
exit /b 0
