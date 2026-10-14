#!/usr/bin/env bash
# build.sh -- d3d8on11: dist/d3d8.dll (MinGW-w64 i686), our own Direct3D 8 device on Direct3D 11 (README.md)
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"; cd "$H"
python3 gen_stubs.py > d8_gen.h
i686-w64-mingw32-g++ -O2 -std=gnu++17 -Wall -Wno-unused-parameter -Wno-misleading-indentation -Wno-delete-non-virtual-dtor -shared -o dist/d3d8.dll \
  d8_main.cpp d8_device.cpp d8_res.cpp d8_ff.cpp d8_guid.cpp d3d8.def \
  -ld3d11 -ldxgi -luuid -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp -Wl,--enable-stdcall-fixup
echo "built dist/d3d8.dll"
