@echo off
rem run_variants.bat -- (13 Nov) the picture check again, with Direct3D 12 run six ways: as it is, and with each of five
rem diagnostic switches (D8ON11_D3D12_DIAG; d8_exec12.cpp says what each turns off). About 5 to 8 minutes. Small windows
rem open and close by themselves. Then send d3d12_variants_results.zip (made in this folder).
cd /d "%~dp0"
set D8ON11_MODE=thread
set D8ON11_EXEC_FILTER=0
set D8ON11_EXEC_BATCH=0
set D8ON11_TEST_TIES=
set D8ON11_BACKEND=d3d11
set D8ON11_D3D12_DIAG=
echo d3d11...
t8.exe scenes > v_d3d11_scenes.txt 2>&1
t8.exe random cases1.txt > v_d3d11_random.txt 2>&1
set D8ON11_BACKEND=d3d12
for %%v in (base sm4 smpdirect nocheck fresh sync) do (
  echo d3d12 %%v...
  if "%%v"=="base" (set D8ON11_D3D12_DIAG=) else (set D8ON11_D3D12_DIAG=%%v)
  t8.exe scenes > v_%%v_scenes.txt 2>&1
  copy /y d3d8on11.log vlog_%%v_scenes.txt >nul
  t8.exe random cases1.txt > v_%%v_random.txt 2>&1
  copy /y d3d8on11.log vlog_%%v_random.txt >nul
)
powershell -NoProfile -Command "Compress-Archive -Force -Path v_*.txt,vlog_*.txt -DestinationPath d3d12_variants_results.zip"
echo Done. Send d3d12_variants_results.zip from this folder.
pause
