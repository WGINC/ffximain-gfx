#!/usr/bin/env bash
# Build the clean graphics tree into one verification DLL (32-bit, freestanding, MSVC6 ABI).
set -uo pipefail
cd "$(dirname "$0")"
CXX=${CXX:-i686-w64-mingw32-g++}; CC=${CC:-i686-w64-mingw32-gcc}
FLAGS=(-O2 -mfpmath=387 -march=i686 -ffreestanding -fno-builtin -fno-asynchronous-unwind-tables
       -fexcess-precision=standard -fno-exceptions -fno-rtti -std=c++17 -Iinclude
       -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks -fpermissive -w
       "-DXI_API=__attribute__((dllexport))")
mkdir -p build/obj build/err
$CC -O2 -ffreestanding -fno-builtin -c rt.c -o build/obj/rt.o
ok=(); bad=()
for f in src/*.cpp; do
  o=build/obj/$(basename "$f" .cpp).o
  if $CXX "${FLAGS[@]}" -c "$f" -o "$o" 2> "build/err/$(basename "$f" .cpp).txt"; then ok+=("$o"); else bad+=("$f"); fi
done
$CXX -shared -nostdlib -o build/gfx_recon.dll "${ok[@]}" build/obj/rt.o -lgcc \
     -Wl,--image-base=0x60000000 -Wl,-e,_DllMainCRTStartup@12 -Wl,--disable-auto-image-base -Wl,--kill-at
echo "built build/gfx_recon.dll from ${#ok[@]} files; failed: ${bad[*]:-none}"
