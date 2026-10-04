#!/usr/bin/env bash
# link_one.sh OBJ DIR -- one raw function and the runtime as a DLL of its own (DIR/fndll/<name>.dll); see build_raw.sh
b=$(basename "$1" .o)
i686-w64-mingw32-g++ -shared -nostdlib -o "$2/fndll/$b.dll" "$1" "$2/obj/rt.o" -lgcc -Wl,--image-base=0x60000000 \
  -Wl,-e,_DllMainCRTStartup@12 -Wl,--disable-auto-image-base -Wl,--kill-at -Wl,--no-insert-timestamp > /dev/null 2>&1
