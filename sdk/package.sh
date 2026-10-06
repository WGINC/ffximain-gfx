#!/usr/bin/env bash
# package.sh -- the SDK as one folder / zip for mod authors: sdk/dist/xikit_sdk.zip
#   include/        xikit_mod.h, xikit_sdk.hpp, xikit_sdk_types.hpp, xikit_game.hpp (generated: gen_sdk.py)
#   tree_include/   the reconstruction's headers (classes with checked field offsets, free functions, orig::, globals, D3D8)
#   tree_src/       the reconstruction's gate-verified source: what each function does, to read before hooking it
#   template/ examples/ build_mod.sh build_mod.bat SDK.md
#   fork.py fork_flags.txt tree_placement.json   forks (fork.py new / diff; `gate` needs the repository)
#   gfx_recon_ingame.dll   the kit's reconstruction (when built): what a fork links against
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/.."; D="$H/dist/xikit_sdk"
rm -rf "$H/dist"; mkdir -p "$D"
cp -r "$H/include" "$H/template" "$H/examples" "$D/"
cp "$H/build_mod.sh" "$H/build_mod.bat" "$H/fork.py" "$H/fork_flags.txt" "$D/"; cp "$ROOT/docs/SDK.md" "$D/"
cp "$ROOT/tree/placement.json" "$D/tree_placement.json"
[ -f "$ROOT/kit/xikit/dist/gfx_recon_ingame.dll" ] && cp "$ROOT/kit/xikit/dist/gfx_recon_ingame.dll" "$D/"
cp -r "$ROOT/tree/include" "$D/tree_include"; cp -r "$ROOT/tree/src" "$D/tree_src"
# the packaged build scripts find the headers next to them (tree_include), as the repo's find ../tree/include
(cd "$H/dist" && "$D/build_mod.sh" /tmp/xikit_sdk_selfcheck.dll xikit_sdk/examples/overlay.cpp >/dev/null && rm -f /tmp/xikit_sdk_selfcheck.dll)
{ printf 'xikit SDK, source %s%s, built %s UTC\r\n' "$(git -C "$ROOT" rev-parse --short=12 HEAD)" \
    "$([ -n "$(git -C "$ROOT" status --porcelain --untracked-files=no)" ] && echo +modified)" "$(date -u '+%Y-%m-%d %H:%M')"
  [ -f "$ROOT/kit/xikit/dist/VERSION.txt" ] && printf 'with the kit: %s\r\n' "$(head -1 "$ROOT/kit/xikit/dist/VERSION.txt" | tr -d '\r')"
} > "$D/VERSION.txt"
(cd "$H/dist" && zip -qr xikit_sdk.zip xikit_sdk)
echo "built $H/dist/xikit_sdk.zip"
