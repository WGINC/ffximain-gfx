@echo off
rem abtest.bat merge ^| skin ^| nowait -- four 30 s segments in one scene, then the start's settings back (README.md).
rem   merge  (start all): d3d8on11 switches draw_merge itself: off, on, on, off (its own A/B, started by the file
rem          d3d8on11_ab_go.txt written below; its log has each segment's frame time and its worker's time)
rem   skin   (start all): A gpu_skin=0 (the game skins characters itself), B gpu_skin=1 (d3d8on11's vertex shader); A B A B
rem   nowait (start plain): A occlusion_nowait=0 (the late read waits), B occlusion_nowait=1; A B A B
rem The ini is written with type, not copied: speedfix re-reads its ini when the file's time changes.
setlocal
call "%~dp0_kit.bat"
set "W=%~1"
if "%W%"=="merge" goto merge
set "D=plain"
if "%W%"=="skin" set "D=all"
if "%W%"=="skin" goto run
if not "%W%"=="nowait" goto usage
:run
for %%s in (A B A B) do call :segment %%s
type "%HERE%%D%\speedfix.ini" >"%KIT%\speedfix.ini"
>"%KIT%\xikit_mark.txt" echo ab %W% done
echo %date% %time% abtest %W% done: the start's settings are back.
exit /b 0
:segment
type "%HERE%%D%\speedfix_%W%_%1.ini" >"%KIT%\speedfix.ini"
timeout /t 2 /nobreak >nul
>"%KIT%\xikit_mark.txt" echo ab %W% %1
if "%W%%1"=="nowaitA" echo %date% %time% segment A: the late read WAITS (as before 10 Nov), 30 s
if "%W%%1"=="nowaitB" echo %date% %time% segment B: the late read does NOT wait, 30 s
if "%W%%1"=="skinA" echo %date% %time% segment A: the GAME skins the characters (as before), 30 s
if "%W%%1"=="skinB" echo %date% %time% segment B: d3d8on11's vertex shader skins them, 30 s
timeout /t 30 /nobreak >nul
exit /b 0
:merge
>"%KIT%\d3d8on11_ab_go.txt" echo go
>"%KIT%\xikit_mark.txt" echo ab merge
echo %date% %time% draw_merge OFF for 30 s (it starts within a second or two)
timeout /t 32 /nobreak >nul
echo %date% %time% draw_merge ON for 60 s
timeout /t 60 /nobreak >nul
echo %date% %time% draw_merge OFF for 30 s
timeout /t 30 /nobreak >nul
>"%KIT%\xikit_mark.txt" echo ab merge done
echo %date% %time% abtest merge done: draw_merge is on again.
exit /b 0
:usage
echo usage: abtest.bat merge ^| skin ^| nowait
exit /b 2
