#!/usr/bin/env bash
# build.sh -- kit/fcap: dist/fcap_replay.exe (MinGW-w64 i686), the frame capture's replayer (docs/KIT.md, "Frame capture")
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"
i686-w64-mingw32-gcc -O2 -Wall -Wno-misleading-indentation -std=gnu99 -static-libgcc -Wl,--no-insert-timestamp \
  -o "$H/dist/fcap_replay.exe" "$H/replay.c" -ld3d8
cp "$H/compare.py" "$H/dist/"
echo "built dist/fcap_replay.exe"
