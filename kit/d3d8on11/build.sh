#!/usr/bin/env bash
# build.sh -- d3d8on11: dist/d3d8.dll (MinGW-w64 i686), our own Direct3D 8 device on Direct3D 11 (README.md)
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"; cd "$H"
python3 gen_stubs.py > d8_gen.h
i686-w64-mingw32-g++ -O2 -std=gnu++17 -Wall -Wno-unused-parameter -Wno-misleading-indentation -Wno-delete-non-virtual-dtor -shared -o dist/d3d8.dll \
  d8_main.cpp d8_device.cpp d8_res.cpp d8_ff.cpp d8_guid.cpp d8_common.cpp d8_exec.cpp d8_link.cpp d3d8.def \
  -ld3d11 -ldxgi -luuid -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp -Wl,--enable-stdcall-fixup
echo "built dist/d3d8.dll"
# the executor as a process of its own (D8ON11_MODE=remote / mode=remote): x86 here (the tests, under Wine) ...
X="d8x_main.cpp d8_exec.cpp d8_link.cpp d8_common.cpp"
i686-w64-mingw32-g++ -O2 -std=gnu++17 -Wall -Wno-unused-parameter -Wno-misleading-indentation -o dist/d8x.exe $X \
  -ld3d11 -ldxgi -luuid -static -Wl,--no-insert-timestamp
echo "built dist/d8x.exe"
# ... and natively for ARM64 (Windows on ARM: the game runs emulated, its executor need not), with zig's clang and its
# mingw-w64 libraries as d3d11probe/build.sh does (-w: the same sources build with -Wall above; zig's libc++ headers
# warn about themselves)
[ "$(python3 -m ziglang version 2>/dev/null)" = 0.16.0 ] || python3 -m pip install -q ziglang==0.16.0
python3 -m ziglang c++ -target aarch64-windows-gnu -O2 -std=gnu++17 -w -o dist/d8x_arm64.exe $X -ld3d11 -ldxgi -luuid
rm -f dist/d8x_arm64.pdb
echo "built dist/d8x_arm64.exe"
