#!/usr/bin/env bash
# package.sh OUT.zip -- the session's zip: kit/ (the built kit: kit/xikit/dist, speedfix.dll, xiload.exe) and
# measure_04nov/ (this folder, with tools/PresentMon from $PRESENTMON). Build first: kit/xikit/build.sh,
# kit/speedfix/build.sh, kit/xiload/build.sh. PresentMon is Intel's (MIT licence), downloaded from
# https://github.com/GameTechDev/PresentMon/releases/download/v2.3.0/PresentMon-2.3.0-x64.exe
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../../.."; OUT="$(realpath -m "$1")"
PM="${PRESENTMON:-/home/user/tools/presentmon/PresentMon-2.3.0-x64.exe}"
T="$(mktemp -d)"; mkdir -p "$T/kit" "$T/measure_04nov/tools"
D="$ROOT/kit/xikit/dist"
for f in xikit.dll gfx_recon_ingame.dll xikit_manifest.txt xikit_manifest_6AC37846.txt xikit_measure.txt xikit_facts.txt xisamp_arm64.exe xisamp_x64.exe VERSION.txt; do cp "$D/$f" "$T/kit/"; done
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$ROOT/kit/xiload/dist/xiload.exe" "$T/kit/"
( cd "$H" && cp -r README.md start_phase.bat collect.bat wpr_optional.bat mac W1_clean W2_where W2b_native W3_deep W4_d3d W5_levers W6_ab "$T/measure_04nov/" )
cp "$PM" "$T/measure_04nov/tools/"
{ echo "PresentMon-2.3.0-x64.exe: Intel's PresentMon 2.3.0 (MIT licence), from"
  echo "https://github.com/GameTechDev/PresentMon/releases/download/v2.3.0/PresentMon-2.3.0-x64.exe"
  echo "sha256 $(sha256sum "$PM" | cut -d' ' -f1)"; } > "$T/measure_04nov/tools/SOURCES.txt"
( cd "$T/kit" && sha256sum * > SHA256SUMS.txt )
for f in "$T"/measure_04nov/*.bat; do sed -i 's/\r*$/\r/' "$f"; done                # Windows line ends for the scripts
for f in "$T"/measure_04nov/*/xikit.ini "$T"/measure_04nov/*/speedfix.ini; do sed -i 's/\r*$/\r/' "$f"; done
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" kit measure_04nov ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
