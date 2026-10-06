#!/usr/bin/env bash
# package.sh -- the SDK as one folder / zip for mod authors: sdk/dist/xikit_sdk.zip
#   include/        xikit_mod.h, xikit_sdk.hpp, xikit_sdk_types.hpp, xikit_game.hpp (generated: gen_sdk.py)
#   tree_include/   the reconstruction's headers (classes with checked field offsets, free functions, orig::, globals, D3D8)
#   tree_src/       the reconstruction's gate-verified source: what each function does, to read before hooking it
#   template/ examples/ build_mod.sh build_mod.bat SDK.md
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/.."; D="$H/dist/xikit_sdk"
rm -rf "$H/dist"; mkdir -p "$D"
cp -r "$H/include" "$H/template" "$H/examples" "$D/"
cp "$H/build_mod.sh" "$H/build_mod.bat" "$D/"; cp "$ROOT/docs/SDK.md" "$D/"
cp -r "$ROOT/tree/include" "$D/tree_include"; cp -r "$ROOT/tree/src" "$D/tree_src"
# the packaged build scripts find the headers next to them (tree_include), as the repo's find ../tree/include
(cd "$H/dist" && "$D/build_mod.sh" /tmp/xikit_sdk_selfcheck.dll xikit_sdk/examples/overlay.cpp >/dev/null && rm -f /tmp/xikit_sdk_selfcheck.dll)
(cd "$H/dist" && zip -qr xikit_sdk.zip xikit_sdk)
echo "built $H/dist/xikit_sdk.zip"
