#!/usr/bin/env bash
# package.sh OUT.zip -- the final session's zip: kit/ (kit/xikit/dist, speedfix.dll, xiload.exe) and final_05nov/
# (this folder, with d3d8on11/ from kit/d3d8on11/dist and tools/PresentMon from $PRESENTMON). Build first:
# kit/xikit/build.sh, kit/speedfix/build.sh, kit/xiload/build.sh, kit/d3d8on11/build.sh.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../../.."; OUT="$(realpath -m "$1")"
PM="${PRESENTMON:-/home/user/tools/presentmon/PresentMon-2.3.0-x64.exe}"
T="$(mktemp -d)"; mkdir -p "$T/kit" "$T/final_05nov/tools" "$T/final_05nov/d3d8on11"
D="$ROOT/kit/xikit/dist"
for f in xikit.dll gfx_recon_ingame.dll xikit_manifest.txt xikit_manifest_6AC37846.txt xikit_measure.txt xikit_facts.txt xisamp_arm64.exe xisamp_x64.exe VERSION.txt; do cp "$D/$f" "$T/kit/"; done
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$ROOT/kit/xiload/dist/xiload.exe" "$T/kit/"
cp "$ROOT/kit/d3d8on11/dist/d3d8.dll" "$ROOT/kit/d3d8on11/dist/d8x.exe" "$ROOT/kit/d3d8on11/dist/d8x_arm64.exe" "$T/final_05nov/d3d8on11/"
( cd "$H" && cp -r README.md _kit.bat start.bat restore.bat mark.bat capture.bat clocktest.bat collect.bat 1_capture 2_target "$T/final_05nov/" )
cp "$PM" "$T/final_05nov/tools/"
{ echo "PresentMon-2.3.0-x64.exe: Intel's PresentMon 2.3.0 (MIT licence), from"
  echo "https://github.com/GameTechDev/PresentMon/releases/download/v2.3.0/PresentMon-2.3.0-x64.exe"
  echo "sha256 $(sha256sum "$PM" | cut -d' ' -f1)"; } > "$T/final_05nov/tools/SOURCES.txt"
( cd "$T/kit" && sha256sum * > SHA256SUMS.txt ); ( cd "$T/final_05nov/d3d8on11" && sha256sum * > SHA256SUMS.txt )
for f in "$T"/final_05nov/*.bat "$T"/final_05nov/*/*.ini; do sed -i 's/\r*$/\r/' "$f"; done      # Windows line ends
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" kit final_05nov ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
