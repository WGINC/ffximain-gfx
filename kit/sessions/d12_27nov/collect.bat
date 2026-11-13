@echo off
rem collect.bat -- zip this session's log folders (the last two days), PresentMon's files, xiload.log, d3d8on11's logs
rem and dgVoodoo.conf into d12_27nov_results.zip next to xikit.dll.
setlocal
call "%~dp0_kit.bat"
set "OUT=%KIT%\d12_27nov_results.zip"
if exist "%OUT%" del "%OUT%"
pushd "%KIT%"
powershell -NoProfile -Command ^
  "$t=(Get-Date).Date.AddDays(-1); $items=@(); " ^
  "$items += Get-ChildItem -Directory -Path 'xikit_logs' -ErrorAction SilentlyContinue | Where-Object { $_.LastWriteTime -ge $t } | ForEach-Object { $_.FullName }; " ^
  "$items += Get-ChildItem -File -Path 'xikit_logs' -Filter 'presentmon_*.csv' -ErrorAction SilentlyContinue | Where-Object { $_.LastWriteTime -ge $t } | ForEach-Object { $_.FullName }; " ^
  "$items += Get-ChildItem -File -Path 'xikit_logs' -Filter 'd3d8on11_before_*.log' -ErrorAction SilentlyContinue | Where-Object { $_.LastWriteTime -ge $t } | ForEach-Object { $_.FullName }; " ^
  "foreach ($f in 'xiload.log','d3d8on11.log','d3d8on11_prev.log','d3d8on11_exec.log','d3d8on11_exec_prev.log','xikit.log','dgVoodoo.conf','d3d8on11.ini','VERSION.txt') { if (Test-Path $f) { $items += (Resolve-Path $f).Path } }; " ^
  "if ($items.Count -eq 0) { Write-Host 'nothing found to collect'; exit 1 }; " ^
  "Compress-Archive -Path $items -DestinationPath '%OUT%' -CompressionLevel Optimal; Write-Host ('collected ' + $items.Count + ' items into the PlayOnlineViewer folder: d12_27nov_results.zip')"
popd
endlocal
