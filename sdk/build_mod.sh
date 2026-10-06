#!/usr/bin/env bash
# build_mod.sh -- build a mod DLL with MinGW-w64 (i686, like the game).
#   sdk/build_mod.sh OUT.dll SOURCE.cpp [MORE.cpp ...] [-- extra g++ flags]
# The tree's headers are included as system headers (-isystem): they need -fpermissive, and their own warnings are
# not the mod's. The mod's own code gets -Wall. Links libgcc/libstdc++ statically, so the DLL needs nothing but
# Windows' own DLLs. To call a reconstructed function directly (not through the game's entry), add
# -- path/to/gfx_recon_ingame.dll (the kit loads that DLL before any mod).
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"
TREE="${XIKIT_TREE_INCLUDE:-$H/../tree/include}"; [ -d "$TREE" ] || TREE="$H/tree_include"
OUT="$1"; shift; SRC=(); EXTRA=()
while [ $# -gt 0 ]; do if [ "$1" = "--" ]; then shift; EXTRA=("$@"); break; fi; SRC+=("$1"); shift; done
CXX="${CXX:-i686-w64-mingw32-g++}"; command -v "$CXX" >/dev/null || CXX=g++     # (MSYS2 mingw32 shell: plain g++)
"$CXX" -std=c++17 -O2 -march=i686 -Wall -fpermissive -fno-strict-aliasing -fwrapv \
  -isystem "$H/include" -isystem "$TREE" -shared -o "$OUT" "${SRC[@]}" "${EXTRA[@]}" \
  -static-libgcc -static-libstdc++ -Wl,--kill-at
echo "built $OUT"
