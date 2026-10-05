#!/usr/bin/env bash
# Build the clean graphics tree into one verification DLL (32-bit, freestanding, MSVC6 ABI).
set -uo pipefail
cd "$(dirname "$0")"
CXX=${CXX:-i686-w64-mingw32-g++}; CC=${CC:-i686-w64-mingw32-gcc}
FLAGS=(-O2 -mfpmath=387 -march=i686 -ffreestanding -fno-builtin -fno-asynchronous-unwind-tables
       -fexcess-precision=standard -fno-exceptions -fno-rtti -std=c++17 -Iinclude
       -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks -fpermissive -w
       "-DXI_API=__attribute__((dllexport))")
# XI_INGAME=1 ./build.sh: the in-game build, every game address rebased through xi_rel (xi_decomp.hpp XI_GAME),
# written to build/gfx_recon_ingame.dll; the default build is the one the gate verifies
OUTDLL=build/gfx_recon.dll; CFL=()
if [ -n "${XI_INGAME:-}" ]; then FLAGS+=(-DXI_INGAME); CFL=(-DXI_INGAME); OUTDLL=build/gfx_recon_ingame.dll; fi
mkdir -p build/obj build/err
$CC -O2 -ffreestanding -fno-builtin "${CFL[@]}" -c rt.c -o build/obj/rt.o
# every file compiled in parallel (JOBS, default: all CPUs); the link order stays the sorted file order
JOBS=${JOBS:-$(nproc 2>/dev/null || echo 4)}
rm -f build/obj/*.ok
export CXX; export FLAGSQ="$(printf '%q ' "${FLAGS[@]}")"
printf '%s\n' src/*.cpp | xargs -P "$JOBS" -I{} bash -c 'f={}; b=$(basename "$f" .cpp)
  eval "$CXX $FLAGSQ -c \"$f\" -o build/obj/$b.o" 2> "build/err/$b.txt" && touch "build/obj/$b.ok"'
ok=(); bad=()
for f in src/*.cpp; do
  b=$(basename "$f" .cpp)
  if [ -e "build/obj/$b.ok" ]; then ok+=("build/obj/$b.o"); else bad+=("$f"); fi
done
# a pinned layout, so a change to one source file moves no other file's code or data and the gate cache
# (pipeline/gate_cache.py) can reuse every other function's verdict: each object's .text starts on its own
# 8 KB boundary and its .rdata/.data on 4 KB ones, the runtime is linked first, and each section kind has
# a fixed base. A file that outgrows its slot moves the files after it (their verdicts are re-run, not reused).
for o in build/obj/rt.o "${ok[@]}"; do
  i686-w64-mingw32-objcopy --set-section-alignment .text=8192 --set-section-alignment .rdata=4096 \
                           --set-section-alignment .data=4096 "$o"
done
$CXX -shared -nostdlib -o $OUTDLL build/obj/rt.o "${ok[@]}" -lgcc \
     -Wl,--image-base=0x60000000 -Wl,-e,_DllMainCRTStartup@12 -Wl,--disable-auto-image-base -Wl,--kill-at \
     -Wl,--section-start=.rdata=0x60800000 -Wl,--section-start=.data=0x60A00000 -Wl,--section-start=.bss=0x60B00000 \
     -Wl,--no-insert-timestamp || { echo "link failed: $OUTDLL not built"; exit 1; }
echo "built $OUTDLL from ${#ok[@]} files; failed: ${bad[*]:-none}"
