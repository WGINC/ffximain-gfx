@echo off
rem run_formats.bat -- the third round (13 Nov): starts Direct3D 12 once, which now logs what the driver says each texture
rem format the game uses supports, and draws test pixels through BGRA, BGRX, BGRA mip levels and DXT1 mip levels. Under
rem a minute. Then send d3d12_formats_results.zip (made in this folder).
cd /d "%~dp0"
set D8ON11_MODE=thread
set D8ON11_BACKEND=d3d12
t8.exe basic > out_formats_basic.txt 2>&1
copy /y d3d8on11.log log_formats.txt >nul
t8.exe scenes > out_formats_scenes.txt 2>&1
copy /y d3d8on11.log log_formats_scenes.txt >nul
t8.exe where > out_where.txt 2>&1
powershell -NoProfile -Command "Compress-Archive -Force -Path out_*.txt,log_*.txt -DestinationPath d3d12_formats_results.zip"
echo Done. Send d3d12_formats_results.zip from this folder.
pause
