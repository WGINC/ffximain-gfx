#!/usr/bin/env bash
# package.sh OUT.zip -- the 15 Nov session's zip: kit/ (kit/xikit/dist, speedfix.dll, xiload.exe) and d12_15nov/
# (this folder, with d3d8on11/ from kit/d3d8on11/dist and tools/PresentMon from $PRESENTMON). Build first:
# kit/xikit/build.sh, kit/speedfix/build.sh, kit/xiload/build.sh, kit/d3d8on11/build.sh.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../../.."; OUT="$(realpath -m "$1")"
PM="${PRESENTMON:-/home/user/tools/presentmon/PresentMon-2.3.0-x64.exe}"
T="$(mktemp -d)"; mkdir -p "$T/kit" "$T/d12_15nov/tools" "$T/d12_15nov/d3d8on11"
D="$ROOT/kit/xikit/dist"
for f in xikit.dll gfx_recon_ingame.dll xikit_manifest.txt xikit_measure.txt xikit_facts.txt xisamp_arm64.exe xisamp_x64.exe VERSION.txt; do cp "$D/$f" "$T/kit/"; done
cp "$D"/xikit_manifest_*.txt "$T/kit/"                  # every other build the kit has a manifest for (6AC37846, 6AE818B4, ...)
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$ROOT/kit/xiload/dist/xiload.exe" "$T/kit/"
cp "$ROOT/kit/d3d8on11/dist/d3d8.dll" "$ROOT/kit/d3d8on11/dist/d8x.exe" "$ROOT/kit/d3d8on11/dist/d8x_arm64.exe" "$T/d12_15nov/d3d8on11/"
( cd "$H" && cp -r README.md _kit.bat start.bat restore.bat mark.bat collect.bat d12 "$T/d12_15nov/" )
cp "$PM" "$T/d12_15nov/tools/"
"$ROOT/kit/d3d8on11/test/device_check/make_check.sh" "$T/check.zip" >/dev/null   # the picture check, run first (README)
( cd "$T/d12_15nov" && unzip -q "$T/check.zip" && mv d3d12_check check ); rm -f "$T/check.zip"
{ echo "PresentMon-2.3.0-x64.exe: Intel's PresentMon 2.3.0 (MIT licence), from"
  echo "https://github.com/GameTechDev/PresentMon/releases/download/v2.3.0/PresentMon-2.3.0-x64.exe"
  echo "sha256 $(sha256sum "$PM" | cut -d' ' -f1)"; } > "$T/d12_15nov/tools/SOURCES.txt"
( cd "$T/kit" && sha256sum * > SHA256SUMS.txt ); ( cd "$T/d12_15nov/d3d8on11" && sha256sum * > SHA256SUMS.txt )
for f in "$T"/d12_15nov/*.bat "$T"/d12_15nov/d12/*.ini; do sed -i 's/\r*$/\r/' "$f"; done      # Windows line ends
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" kit d12_15nov ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
