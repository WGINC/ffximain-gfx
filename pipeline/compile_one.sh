#!/usr/bin/env bash
# compile_one.sh SRC.cpp OUTDIR -- one translation unit, errors to OUTDIR/err
f=$1; d=$2; b=$(basename "$f" .cpp)
i686-w64-mingw32-g++ -O2 -mfpmath=387 -march=i686 -ffreestanding -fno-builtin \
  -fno-asynchronous-unwind-tables -fexcess-precision=standard -fno-exceptions -fno-rtti -std=c++17 \
  -I"$(dirname "$0")" -I"$(dirname "$0")/../data" -w -fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks -fpermissive \
  -c "$f" -o "$d/obj/$b.o" 2> "$d/err/$b.txt" || rm -f "$d/obj/$b.o"
