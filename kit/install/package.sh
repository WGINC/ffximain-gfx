#!/usr/bin/env bash
# package.sh OUT.zip -- Longthinx gobbiefix, the installable kit: gobbiefix/ with install.bat, uninstall.bat,
# gobbiefix_debug.bat, README.md, logo.txt and files/ (d3d8on11's d3d8.dll and executors, xikit.dll and its manifests, speedfix.dll, gobbiefix.ini and the internal .ini files). Build first:
# kit/xikit/build.sh, kit/speedfix/build.sh, kit/d3d8on11/build.sh. No game file goes in.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."; OUT="$(realpath -m "$1")"
T="$(mktemp -d)"; P="$T/gobbiefix"; F="$P/files"; mkdir -p "$F"
D="$ROOT/kit/xikit/dist"
# (14 Nov) the public kit: no reconstruction DLL (xikit runs the mods' hooks on the game's own code with swap=0), and the
# manifests with fingerprints instead of the game's bytes (public_manifest.py)
cp "$D/xikit.dll" "$F/"
# (a VERSION.txt of its own: the kit's lists the private build's files and source commit)
echo "Longthinx gobbiefix, built $(date -u +%Y-%m-%d)" > "$F/VERSION.txt"
for m in "$D/xikit_manifest.txt" "$D"/xikit_manifest_*.txt; do python3 "$H/public_manifest.py" "$m" "$F/$(basename "$m")"; done
cp "$ROOT/kit/speedfix/dist/speedfix.dll" "$F/"
cp "$ROOT/kit/d3d8on11/dist/d3d8.dll" "$ROOT/kit/d3d8on11/dist/d8x.exe" "$ROOT/kit/d3d8on11/dist/d8x_arm64.exe" "$F/"
cp "$H/config/"*.ini "$F/"
cp "$H/install.bat" "$H/uninstall.bat" "$H/gobbiefix_debug.bat" "$H/README.md" "$H/logo.txt" "$H/LICENSE.txt" "$H/COPYING.txt" "$P/"
( cd "$F" && sha256sum * > ../SHA256SUMS.txt )
for f in "$P"/*.bat "$P"/logo.txt "$P"/LICENSE.txt "$F"/*.ini; do sed -i 's/\r*$/\r/' "$f"; done      # Windows line ends
rm -f "$OUT"; ( cd "$T" && zip -qr "$OUT" gobbiefix ); rm -rf "$T"
echo "wrote $OUT"; unzip -l "$OUT" | tail -1
