#!/usr/bin/env bash
# build.sh -- the speedfix mod: dist/speedfix.dll (MinGW-w64 i686, through sdk/build_mod.sh)
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"
cd "$H" && ../../sdk/build_mod.sh dist/speedfix.dll sf_game.cpp sf_main.cpp sf_device.cpp sf_fork.cpp fork/CYyModelDt__SkinAndDrawV2.fork.cpp -- -Wl,--no-insert-timestamp
