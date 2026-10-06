#!/usr/bin/env bash
# build_mod.sh -- build a mod DLL with MinGW-w64 (i686, like the game).
#   sdk/build_mod.sh OUT.dll SOURCE.cpp [MORE.cpp ...] [-- extra g++ flags]
# The tree's headers are included as system headers (-isystem): they need -fpermissive, and their own warnings are
# not the mod's. The mod's own code gets -Wall. Links libgcc/libstdc++ statically, so the DLL needs nothing but
# Windows' own DLLs. To call a reconstructed function directly (not through the game's entry), add
# -- path/to/gfx_recon_ingame.dll (the kit loads that DLL before any mod).
# A fork (NAME.fork.cpp, from fork.py) is compiled with the tree's own flags (fork_flags.txt, tree/build.sh's) and
# XI_INGAME, as `fork.py gate` compiles it, and the mod is linked against gfx_recon_ingame.dll, which a fork's calls
# to other reconstructed functions go to: XIKIT_RECON, else the kit's dist/ in the repository, else one next to this
# script.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"
TREE="${XIKIT_TREE_INCLUDE:-$H/../tree/include}"; [ -d "$TREE" ] || TREE="$H/tree_include"
OUT="$1"; shift; SRC=(); FORK=(); EXTRA=()
while [ $# -gt 0 ]; do
  if [ "$1" = "--" ]; then shift; EXTRA=("$@"); break; fi
  case "$1" in *.fork.cpp) FORK+=("$1");; *) SRC+=("$1");; esac; shift
done
CXX="${CXX:-i686-w64-mingw32-g++}"; command -v "$CXX" >/dev/null || CXX=g++     # (MSYS2 mingw32 shell: plain g++)
OBJ=()
if [ ${#FORK[@]} -gt 0 ]; then
  mapfile -t FF < "$H/fork_flags.txt"
  T="$(mktemp -d)"; trap 'rm -rf "$T"' EXIT
  for f in "${FORK[@]}"; do
    o="$T/$(basename "$f" .cpp).o"
    "$CXX" "${FF[@]}" -DXI_INGAME -isystem "$H/include" -isystem "$TREE" -c "$f" -o "$o"; OBJ+=("$o")
  done
  RECON="${XIKIT_RECON:-}"
  for c in "$H/../kit/xikit/dist/gfx_recon_ingame.dll" "$H/gfx_recon_ingame.dll"; do [ -z "$RECON" ] && [ -f "$c" ] && RECON="$c"; done
  [ -n "$RECON" ] || { echo "build_mod.sh: a fork needs gfx_recon_ingame.dll to link against: set XIKIT_RECON" >&2; exit 1; }
  OBJ+=("$RECON")
fi
"$CXX" -std=c++17 -O2 -march=i686 -Wall -fpermissive -fno-strict-aliasing -fwrapv \
  -isystem "$H/include" -isystem "$TREE" -shared -o "$OUT" "${SRC[@]}" "${OBJ[@]}" "${EXTRA[@]}" \
  -static-libgcc -static-libstdc++ -Wl,--kill-at
echo "built $OUT"
