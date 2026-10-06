#!/usr/bin/env bash
# build.sh -- the in-game kit into kit/xikit/dist/ (MinGW-w64 i686; uses $FFXI_DLL for the manifest):
#   gfx_recon_ingame.dll   the clean tree (../../tree) built with XI_INGAME
#   xikit_manifest.txt     make_manifest.py
#   xikit.dll              xikit.c + ../d3dprof/d3dprof.c (XIKIT)
#   xikit.ini              the default settings
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."; OUT="$H/dist"; W="$H/build"
mkdir -p "$OUT" "$W"
rm -rf "$W/tree"; cp -r "$ROOT/tree" "$W/tree"; rm -rf "$W/tree/build"
(cd "$W/tree" && XI_INGAME=1 ./build.sh | tail -1)
cp "$W/tree/build/gfx_recon_ingame.dll" "$OUT/"
python3 "$H/close_gaps.py" "$OUT/gfx_recon_ingame.dll"        # Windows refuses gaps between sections (error 193)
python3 "$H/make_manifest.py" "$OUT/gfx_recon_ingame.dll" "$OUT/xikit_manifest.txt" "$W/manifest_report.json" | head -1
python3 "$ROOT/sdk/gen_sdk.py"                                # sdk/include/xikit_game.hpp follows the manifest
i686-w64-mingw32-gcc -O2 -Wall -Wno-unused-function -std=gnu99 -DXIKIT -shared -o "$OUT/xikit.dll" \
    "$H/xikit.c" "$ROOT/kit/d3dprof/d3dprof.c" -static-libgcc -Wl,--kill-at -Wl,--no-insert-timestamp
python3 - "$H/xikit.c" "$OUT/xikit.ini" <<'PY'
import re, sys
s = open(sys.argv[1]).read()
m = re.search(r'static const char DEFAULT_INI\[\] =\n(.*?);\n', s, re.S)
txt = ''.join(eval(x) for x in re.findall(r'"(?:[^"\\]|\\.)*"', m.group(1)))
open(sys.argv[2], 'w', newline='').write(txt)
PY
ls -la "$OUT"
