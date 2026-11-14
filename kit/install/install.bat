@echo off
rem install.bat [/defaults] [FOLDER] -- Longthinx gobbiefix: installs into the game's PlayOnlineViewer folder (FOLDER, or
rem the usual place). Once installed nothing has to be started before the game: the game loads d3d8.dll from that
rem folder, and d3d8.dll applies gobbiefix.ini and loads the kit (and speedfix) itself when it knows the game's build.
rem Kept: the d3d8.dll that was there (dgVoodoo's) as d3d8_before_kit.dll; your gobbiefix.ini (with /defaults it is
rem replaced, yours kept as gobbiefix.ini.before_gobbiefix). The other .ini files are gobbiefix's own and are replaced
rem (an older one is kept once as NAME.ini.before_gobbiefix). uninstall.bat puts it back. Run it as administrator.
setlocal
set "SRC=%~dp0files"
set "DEF="
if /i "%~1"=="/defaults" (set "DEF=1" & shift)
set "DST=%~1"
if "%DST%"=="" set "DST=C:\Program Files (x86)\PlayOnline\SquareEnix\PlayOnlineViewer"
if exist "%~dp0logo.txt" type "%~dp0logo.txt"
echo  Pshhh... Shkohhh... Gobbie sniffsniff for game in "%DST%"...
if not exist "%DST%\pol.exe" if not exist "%DST%\polcore.dll" goto nofolder
tasklist /fi "imagename eq pol.exe" | find /i "pol.exe" >nul
if not errorlevel 1 goto running
echo  Found it! Yesyes. Pshhh...
if exist "%DST%\d3d8_before_kit.dll" goto copyfiles
if not exist "%DST%\d3d8.dll" goto copyfiles
copy /y "%DST%\d3d8.dll" "%DST%\d3d8_before_kit.dll" >nul
echo  Old d3d8.dll go in gobbiesack, safesafe. (d3d8_before_kit.dll)
:copyfiles
rem (wildcards unquoted, from inside the folder: some cmd versions do not expand a quoted pattern)
pushd "%SRC%"
for %%f in (*.dll *.exe *.txt) do copy /y "%%f" "%DST%\" >nul
popd
rem (an older gobbiefix put the reconstruction DLL here; this one runs without it)
if exist "%DST%\gfx_recon_ingame.dll" del /q "%DST%\gfx_recon_ingame.dll"
echo  Shkohhh... gobbie stuff in the shinybits.
for %%f in (d3d8on11.ini speedfix.ini xikit.ini) do if exist "%DST%\%%f" if not exist "%DST%\%%f.before_gobbiefix" copy /y "%DST%\%%f" "%DST%\%%f.before_gobbiefix" >nul
for %%f in (d3d8on11.ini speedfix.ini xikit.ini) do copy /y "%SRC%\%%f" "%DST%\" >nul
if defined DEF if exist "%DST%\gobbiefix.ini" copy /y "%DST%\gobbiefix.ini" "%DST%\gobbiefix.ini.before_gobbiefix" >nul
if defined DEF copy /y "%SRC%\gobbiefix.ini" "%DST%\" >nul
if exist "%DST%\gobbiefix.ini" goto keepknobs
copy /y "%SRC%\gobbiefix.ini" "%DST%\" >nul
echo  Gobbie leave youknobs: gobbiefix.ini. Pokepoke if youuu want.
goto marked
:keepknobs
if not defined DEF echo  Youuu already got gobbiefix.ini. Gobbie no touchy youknobs.
if defined DEF echo  gobbiefix.ini back to gobbieway. Yours wait in gobbiefix.ini.before_gobbiefix.
:marked
>"%DST%\kit_installed.txt" echo installed by Longthinx gobbiefix (install.bat)
echo(
echo  Pshhh... Allfixed! Youuu go play now. Start game sameway as always. Shkohhh...
exit /b 0
:nofolder
echo  Pshhh... Hmmm. Gobbie no smell game there. Youuu tell gobbie where:
echo      install.bat "C:\path\to\PlayOnlineViewer"
exit /b 2
:running
echo  Shkohhh! Game still running! Youuu close it first, then gobbie workwork.
exit /b 2
