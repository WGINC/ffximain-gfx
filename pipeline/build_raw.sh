#!/usr/bin/env bash
# build_raw.sh DIR -- compile every translated function in DIR separately; link the ones that compile
set -u
P="$(cd "$(dirname "$0")" && pwd)"
CXX=i686-w64-mingw32-g++; CC=i686-w64-mingw32-gcc
DIR=${1:?usage: build_raw.sh DIR}
mkdir -p "$DIR/obj" "$DIR/err"
$CC -O2 -ffreestanding -fno-builtin -c "$P/rt.c" -o "$DIR/obj/rt.o"
ls "$DIR"/fn_*.cpp | xargs -P "$(nproc)" -I{} "$P/compile_one.sh" {} "$DIR"
$CXX -shared -nostdlib -o "$DIR/gfx_raw.dll" "$DIR"/obj/fn_*.o "$DIR/obj/rt.o" -lgcc \
     -Wl,--image-base=0x60000000 -Wl,-e,_DllMainCRTStartup@12 -Wl,--disable-auto-image-base -Wl,--kill-at 2>&1 | tail -5
echo "compiled $(ls "$DIR"/obj/fn_*.o | wc -l) of $(ls "$DIR"/fn_*.cpp | wc -l)"
