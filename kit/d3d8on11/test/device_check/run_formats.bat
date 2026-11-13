@echo off
rem run_formats.bat -- (13 Nov) Direct3D 12's startup checks on this machine: texture formats, every sampler filter, and
rem where in the heap a sampler table is; then the test scenes three ways: Direct3D 11, Direct3D 12, and Direct3D 12 with
rem every draw's samplers at the heap's first table (D8ON11_D3D12_DIAG=smp0). About a minute. Then send
rem d3d12_formats_results.zip (made in this folder).
cd /d "%~dp0"
set D8ON11_MODE=thread
set D8ON11_EXEC_FILTER=0
set D8ON11_EXEC_BATCH=0
set D8ON11_TEST_TIES=
set D8ON11_D3D12_DIAG=
set D8ON11_BACKEND=d3d12
t8.exe basic > out_formats_basic.txt 2>&1
copy /y d3d8on11.log log_formats.txt >nul
t8.exe scenes > out_formats_scenes.txt 2>&1
copy /y d3d8on11.log log_formats_scenes.txt >nul
set D8ON11_D3D12_DIAG=smp0
t8.exe scenes > out_formats_scenes_smp0.txt 2>&1
copy /y d3d8on11.log log_formats_scenes_smp0.txt >nul
set D8ON11_D3D12_DIAG=
set D8ON11_BACKEND=d3d11
t8.exe scenes > out_formats_scenes_d3d11.txt 2>&1
t8.exe where > out_where.txt 2>&1
powershell -NoProfile -Command "Compress-Archive -Force -Path out_*.txt,log_*.txt -DestinationPath d3d12_formats_results.zip"
echo Done. Send d3d12_formats_results.zip from this folder.
pause
