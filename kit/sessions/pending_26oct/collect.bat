@echo off
rem collect.bat -- zip today's xikit log folders, d3d8on11.log and dgVoodoo.conf into pending_26oct_results.zip.
rem Run it from the folder of xikit.dll.
setlocal
set OUT=%CD%\pending_26oct_results.zip
if exist "%OUT%" del "%OUT%"
powershell -NoProfile -Command ^
  "$t=(Get-Date).Date; $items=@(); " ^
  "$items += Get-ChildItem -Directory -Path 'xikit_logs' -ErrorAction SilentlyContinue | Where-Object { $_.LastWriteTime -ge $t } | ForEach-Object { $_.FullName }; " ^
  "foreach ($f in 'd3d8on11.log','d3d8on11_exec.log','dgVoodoo.conf','xikit.log') { if (Test-Path $f) { $items += (Resolve-Path $f).Path } }; " ^
  "if ($items.Count -eq 0) { Write-Host 'nothing found to collect'; exit 1 }; " ^
  "Compress-Archive -Path $items -DestinationPath '%OUT%'; Write-Host ('collected ' + $items.Count + ' items into %OUT%')"
endlocal
