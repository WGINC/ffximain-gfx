#!/usr/bin/env bash
# package.sh OUT.zip -- the installable kit: install.bat, uninstall.bat, README.md and files/ (d3d8on11's d3d8.dll and
# executors, xikit.dll with the reconstruction and its manifests, speedfix.dll, the default .ini files). Build first:
# kit/xikit/build.sh, kit/speedfix/build.sh, kit/d3d8on11/build.sh. No game file goes in.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."; OUT="$(realpath -m "$1")"
T="$(mktemp -d)"; mkdir -p "$T/ffxi_kit/files"; F="$T/ffxi_kit/files"
D="$ROOT/kit/xikit/dist"
cp "$D/xikit.dll" "$D/gfx_recon_ingame.dll" "$D/xikit_manifest.txt" "$D"/xikit_manifest_*.txt "$D/VERSION.txt" "$F/"
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$F/"
cp "$ROOT/kit/d3d8on11/dist/d3d8.dll" "$ROOT/kit/d3d8on11/dist/d8x.exe" "$ROOT/kit/d3d8on11/dist/d8x_arm64.exe" "$F/"
cp "$H/config/"*.ini "$F/"
cp "$H/install.bat" "$H/uninstall.bat" "$H/README.md" "$T/ffxi_kit/"
( cd "$F" && sha256sum * > ../SHA256SUMS.txt )
for f in "$T"/ffxi_kit/*.bat "$F"/*.ini; do sed -i 's/\r*$/\r/' "$f"; done      # Windows line ends
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" ffxi_kit ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
