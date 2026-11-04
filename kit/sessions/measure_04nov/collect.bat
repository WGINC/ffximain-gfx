@echo off
rem collect.bat -- zip today's xikit log folders (each phase's), PresentMon's files, xiload.log, the WPR trace if any,
rem and dgVoodoo's and d3d8on11's settings and logs into measure_04nov_results.zip next to xikit.dll.
setlocal
set "HERE=%~dp0"
set "KIT=%HERE%"
if not exist "%KIT%xikit.dll" set "KIT=%HERE%..\"
for %%k in ("%KIT%.") do set "KIT=%%~fk"
set "OUT=%KIT%\measure_04nov_results.zip"
if exist "%OUT%" del "%OUT%"
pushd "%KIT%"
powershell -NoProfile -Command ^
  "$t=(Get-Date).Date.AddDays(-1); $items=@(); " ^
  "$items += Get-ChildItem -Directory -Path 'xikit_logs' -ErrorAction SilentlyContinue | Where-Object { $_.LastWriteTime -ge $t } | ForEach-Object { $_.FullName }; " ^
  "$items += Get-ChildItem -File -Path 'xikit_logs' -Filter 'presentmon_*.csv' -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }; " ^
  "$items += Get-ChildItem -File -Path 'xikit_logs' -Filter '*.etl' -ErrorAction SilentlyContinue | ForEach-Object { $_.FullName }; " ^
  "foreach ($f in 'xiload.log','d3d8on11.log','d3d8on11_exec.log','dgVoodoo.conf','d3d8on11.ini','VERSION.txt') { if (Test-Path $f) { $items += (Resolve-Path $f).Path } }; " ^
  "if ($items.Count -eq 0) { Write-Host 'nothing found to collect'; exit 1 }; " ^
  "Compress-Archive -Path $items -DestinationPath '%OUT%' -CompressionLevel Optimal; Write-Host ('collected ' + $items.Count + ' items into %OUT%')"
popd
endlocal
