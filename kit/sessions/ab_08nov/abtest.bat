@echo off
rem abtest.bat pop ^| base -- four 30 s segments (A B A B), then the session's settings back (README.md).
rem   pop:  A occlusion_late=0 (each occlusion result read at once), B occlusion_late=1 (as the session)
rem   base: A the game's own pacing (uncap=0 frame_step=0: 30 fps, whole ticks), B as the session
rem Everything else is the session's play\speedfix.ini. Each segment writes a mark ("ab step A", ...) for the frame log.
rem The ini is written with type, not copied: speedfix re-reads its ini when the file's time changes, and copy keeps
rem the source file's time (the 5 Nov clock test never switched because of that).
setlocal
call "%~dp0_kit.bat"
set "W=%~1"
if not exist "%HERE%play\speedfix_%W%_A.ini" goto usage
for %%s in (A B A B) do call :segment %%s
type "%HERE%play\speedfix.ini" >"%KIT%\speedfix.ini"
>"%KIT%\xikit_mark.txt" echo ab %W% done
echo %date% %time% abtest %W% done: the session's settings are back.
exit /b 0
:segment
type "%HERE%play\speedfix_%W%_%1.ini" >"%KIT%\speedfix.ini"
timeout /t 2 /nobreak >nul
>"%KIT%\xikit_mark.txt" echo ab %W% %1
echo %date% %time% segment %1 (30 s)
timeout /t 30 /nobreak >nul
exit /b 0
:usage
echo usage: abtest.bat pop ^| base
exit /b 2
