#!/usr/bin/env bash
# build.sh MS_DIR DDI_DIR OUT_DIR -- the stand-in d3d8thk.dll (kthunk.c) into OUT_DIR, with Microsoft's d3d8.dll and
# d3dref8.dll copied beside it. MS_DIR holds the DirectX 8.1 SDK's retail d3d8.dll, d3dref8.dll and d3d8thk.dll
# (from its dxnt.cab); DDI_DIR holds the WDK 7.1 DDI headers ddrawint.h, d3dnthal.h, ddrawi.h, d3dhal.h and the
# headers they include. Neither enters the repository.
set -euo pipefail
H="$(cd "$(dirname "$0")" && pwd)"; MS="$1"; DDI="$2"; OUT="$3"; mkdir -p "$OUT"
python3 "$H/gen_thunks.py" "$MS/d3d8thk.dll" "$OUT"
i686-w64-mingw32-gcc -shared -O1 -w -I"$OUT" -I"$DDI" -o "$OUT/d3d8thk.dll" "$H/kthunk.c" "$OUT/thk.def" -static-libgcc -lgdi32
cp "$MS/d3d8.dll" "$MS/d3dref8.dll" "$OUT/"
