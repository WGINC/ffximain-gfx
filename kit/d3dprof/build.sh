#!/bin/sh
# builds d3dprof.dll (32-bit, like the game) with MinGW
cd "$(dirname "$0")" && i686-w64-mingw32-gcc -O2 -Wall -shared -o d3dprof.dll d3dprof.c -static-libgcc -Wl,--kill-at
