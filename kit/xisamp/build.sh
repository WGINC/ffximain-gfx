#!/usr/bin/env bash
# build.sh -- the native sampler (README.md): dist/xisamp_arm64.exe (Windows on ARM) and dist/xisamp_x64.exe (x64
# Windows, and the test under Wine), with zig's clang and its mingw-w64 libraries as d3d11probe/build.sh does; and
# test/target.exe (MinGW-w64 i686), the test's x86 process.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"; cd "$H"
[ "$(python3 -m ziglang version 2>/dev/null)" = 0.16.0 ] || python3 -m pip install -q ziglang==0.16.0
for t in x86_64 aarch64; do
  a=x64; [ $t = aarch64 ] && a=arm64
  python3 -m ziglang cc -target $t-windows-gnu -O2 -Wall -o dist/xisamp_$a.exe xisamp.c -lpsapi -lwinmm
done
rm -f dist/*.pdb
i686-w64-mingw32-gcc -O1 -fno-omit-frame-pointer -Wall -o test/target.exe test/target.c -static-libgcc -Wl,--no-insert-timestamp
echo "built dist/: $(ls dist | tr '\n' ' ')"
