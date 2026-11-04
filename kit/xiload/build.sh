#!/usr/bin/env bash
# build.sh -- xiload.exe (32-bit, like pol.exe) into kit/xiload/dist/; test: python3 test/test_xiload.py (Wine)
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; mkdir -p "$H/dist"
i686-w64-mingw32-gcc -O2 -Wall -o "$H/dist/xiload.exe" "$H/xiload.c" -static-libgcc -Wl,--no-insert-timestamp
echo "built $H/dist/xiload.exe"
