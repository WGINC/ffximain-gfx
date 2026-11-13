@echo off
rem run_check.bat -- d3d8on11's picture test on this machine: every test scene and 600 random cases (render states the
rem game sets, in four runs of 150), drawn through Direct3D 11 and through Direct3D 12, saved for comparison. About 3 minutes. Small windows
rem open and close by themselves; leave them alone. Then send d3d12_check_results.zip (made in this folder).
cd /d "%~dp0"
set D8ON11_MODE=thread
set D8ON11_EXEC_FILTER=0
set D8ON11_EXEC_BATCH=0
for %%b in (d3d11 d3d12) do (
  echo %%b: scenes...
  set D8ON11_BACKEND=%%b
  set D8ON11_TEST_TIES=
  t8.exe scenes > out_%%b_scenes.txt 2>&1
  copy /y d3d8on11.log log_%%b_scenes.txt >nul
  echo %%b: random cases...
  for %%c in (1 2 3 4) do (
    t8.exe random cases%%c.txt > out_%%b_random%%c.txt 2>&1
    copy /y d3d8on11.log log_%%b_random%%c.txt >nul
  )
  echo %%b: the same, marking texel-border samples...
  set D8ON11_TEST_TIES=0.01
  t8.exe scenes > out_%%b_scenes_ties.txt 2>&1
  for %%c in (1 2 3 4) do t8.exe random cases%%c.txt > out_%%b_random%%c_ties.txt 2>&1
  copy /y d3d8on11.log log_%%b_ties.txt >nul
)
t8.exe where > out_where.txt 2>&1
powershell -NoProfile -Command "Compress-Archive -Force -Path out_*.txt,log_*.txt -DestinationPath d3d12_check_results.zip"
echo Done. Send d3d12_check_results.zip from this folder.
pause
