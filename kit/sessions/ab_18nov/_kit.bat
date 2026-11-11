@rem _kit.bat -- sets KIT to the folder of xikit.dll (this folder, or the one above it). Called by the other scripts.
set "HERE=%~dp0"
set "KIT=%HERE%"
if not exist "%KIT%xikit.dll" set "KIT=%HERE%..\"
for %%k in ("%KIT%.") do set "KIT=%%~fk"
