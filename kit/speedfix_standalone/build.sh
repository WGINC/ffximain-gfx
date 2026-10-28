#!/usr/bin/env bash
# build.sh -- speedfix on its own (no xikit): dist/ gets speedfix_standalone.dll (speedfix's sources, unchanged, and
# sfs_host.c), gfx_recon_ingame.dll (the forks call into it), speedfix_manifest.txt and speedfix_manifest_<stamp>.txt
# (kit/xikit's manifests, the second for each other build: $FFXI_DLL_<stamp>, as kit/xikit/build.sh), VERSION.txt.
# Needs kit/xikit/build.sh run first (its dist/ holds the reconstruction DLL and the analysed build's manifest).
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; ROOT="$H/../.."; X="$ROOT/kit/xikit"; OUT="$H/dist"; mkdir -p "$OUT"
for f in gfx_recon_ingame.dll xikit_manifest.txt; do [ -f "$X/dist/$f" ] || { echo "build.sh: $X/dist/$f missing: run kit/xikit/build.sh" >&2; exit 1; }; done
CC="${CC:-i686-w64-mingw32-gcc}"; command -v "$CC" >/dev/null || CC=gcc
T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
"$CC" -O2 -march=i686 -Wall -c "$H/sfs_host.c" -o "$T/sfs_host.o"
cp "$X/dist/gfx_recon_ingame.dll" "$OUT/"
S="$ROOT/kit/speedfix"
( cd "$S" && XIKIT_RECON="$OUT/gfx_recon_ingame.dll" "$ROOT/sdk/build_mod.sh" "$OUT/speedfix_standalone.dll" sf_game.cpp sf_main.cpp sf_device.cpp sf_occlusion.cpp sf_fork.cpp \
    fork/CYyModelDt__SkinAndDrawV2.fork.cpp fork/Frame_Render.fork.cpp fork/Camera_GetFrameScale.fork.cpp fork/Camera_GetClampedField28.fork.cpp \
    -- "$T/sfs_host.o" -Wl,--no-insert-timestamp )
cp "$X/dist/xikit_manifest.txt" "$OUT/speedfix_manifest.txt"
for b in "$X"/builds/*.json; do
  st="$(basename "$b" .json)"; var="FFXI_DLL_$st"
  if [ -n "${!var:-}" ]; then python3 "$X/manifest_for_build.py" "$OUT/speedfix_manifest.txt" "$b" "${!var}" "$OUT/speedfix_manifest_$st.txt" | tail -1
  else echo "build $st: \$$var not set, no manifest for it"; rm -f "$OUT/speedfix_manifest_$st.txt"; fi
done
cp "$H/speedfix.ini" "$OUT/speedfix.ini.example"; cp "$H/speedfix_mac_parallels.ini" "$OUT/"; [ -f "$OUT/speedfix.ini" ] || cp "$H/speedfix.ini" "$OUT/speedfix.ini"
python3 - "$OUT" "$(git -C "$ROOT" rev-parse --short HEAD)" <<'P'
import hashlib, os, sys, datetime
out, rev = sys.argv[1:3]
with open(os.path.join(out, 'VERSION.txt'), 'w', newline='\r\n') as f:
    f.write('speedfix standalone, source %s, built %s\n' % (rev, datetime.date.today()))
    for n in sorted(os.listdir(out)):
        if n != 'VERSION.txt': f.write('%s %s\n' % (hashlib.sha256(open(os.path.join(out, n), 'rb').read()).hexdigest()[:16], n))
P
echo "built $OUT"
