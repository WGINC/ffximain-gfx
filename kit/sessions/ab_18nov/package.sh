#!/usr/bin/env bash
# package.sh OUT.zip -- the 18 Nov session's zip: kit/ (kit/xikit/dist, speedfix.dll, xiload.exe) and ab_18nov/
# (this folder, with d3d8on11/ from kit/d3d8on11/dist and tools/PresentMon from $PRESENTMON). Build first:
# kit/xikit/build.sh, kit/speedfix/build.sh, kit/xiload/build.sh, kit/d3d8on11/build.sh.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../../.."; OUT="$(realpath -m "$1")"
PM="${PRESENTMON:-/home/user/tools/presentmon/PresentMon-2.3.0-x64.exe}"
T="$(mktemp -d)"; mkdir -p "$T/kit" "$T/ab_18nov/tools" "$T/ab_18nov/d3d8on11"
D="$ROOT/kit/xikit/dist"
for f in xikit.dll gfx_recon_ingame.dll xikit_manifest.txt xikit_measure.txt xikit_facts.txt xisamp_arm64.exe xisamp_x64.exe VERSION.txt; do cp "$D/$f" "$T/kit/"; done
cp "$D"/xikit_manifest_*.txt "$T/kit/"                  # every other build the kit has a manifest for (6AC37846, 6AE818B4, ...)
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$ROOT/kit/xiload/dist/xiload.exe" "$T/kit/"
cp "$ROOT/kit/d3d8on11/dist/d3d8.dll" "$ROOT/kit/d3d8on11/dist/d8x.exe" "$ROOT/kit/d3d8on11/dist/d8x_arm64.exe" "$T/ab_18nov/d3d8on11/"
( cd "$H" && cp -r README.md _kit.bat start.bat restore.bat mark.bat collect.bat abtest.bat all plain hitch "$T/ab_18nov/" )
cp "$PM" "$T/ab_18nov/tools/"
{ echo "PresentMon-2.3.0-x64.exe: Intel's PresentMon 2.3.0 (MIT licence), from"
  echo "https://github.com/GameTechDev/PresentMon/releases/download/v2.3.0/PresentMon-2.3.0-x64.exe"
  echo "sha256 $(sha256sum "$PM" | cut -d' ' -f1)"; } > "$T/ab_18nov/tools/SOURCES.txt"
( cd "$T/kit" && sha256sum * > SHA256SUMS.txt ); ( cd "$T/ab_18nov/d3d8on11" && sha256sum * > SHA256SUMS.txt )
for f in "$T"/ab_18nov/*.bat "$T"/ab_18nov/*/*.ini; do sed -i 's/\r*$/\r/' "$f"; done      # Windows line ends
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" kit ab_18nov ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
