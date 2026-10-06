#!/usr/bin/env bash
# build.sh -- phase 0 of the D3D11 renderer (docs/D3D11.md): dist/probe11_x86.exe and dist/probe11_arm64.exe (zig cc,
# from PyPI's `ziglang`: its mingw-w64 libraries cover ARM64 Windows; this container cannot fetch llvm-mingw), and
# dist/probe8_x86.exe (MinGW-w64 i686). Plus run_phase0.bat and the report script.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"; cd "$H"
# the version is pinned: kit/BUILD.lock.json holds these builds' hashes (reproducible with this compiler)
[ "$(python3 -m ziglang version 2>/dev/null)" = 0.16.0 ] || python3 -m pip install -q ziglang==0.16.0
for t in x86 aarch64; do
  a=$t; [ $t = aarch64 ] && a=arm64
  python3 -m ziglang cc -target $t-windows-gnu -O2 -Wall -o dist/probe11_$a.exe probe11.c -ld3d11 -ldxgi -lgdi32 -luser32
done
i686-w64-mingw32-gcc -O2 -Wall -o dist/probe8_x86.exe probe8.c -ld3d8 -lversion -lgdi32 -luser32 -static-libgcc -Wl,--no-insert-timestamp
cp run_phase0.bat phase0_report.py dist/
rm -f dist/*.pdb
echo "built dist/: $(ls dist | tr '\n' ' ')"
