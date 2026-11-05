@echo off
rem abtest.bat step ^| cap -- four 30 s segments (A B A B), then the session's settings back (README.md).
rem   step: A frame_step=1 (whole ticks: frames where the camera and characters stop), B frame_step=3 (the fraction)
rem   cap:  A frame_cap_fps=60 (about one tick a frame), B no cap (as the session)
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
echo usage: abtest.bat step ^| cap
exit /b 2
