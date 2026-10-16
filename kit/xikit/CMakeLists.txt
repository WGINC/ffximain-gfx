cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(xikit C)

# 32-bit only: it is injected into pol.exe, which is 32-bit. The profiler (../d3dprof/d3dprof.c) is built in,
# with XIKIT, so xikit.dll attaches it to the device it hooks (profile=1 in xikit.ini).
#   Visual Studio:  cmake -B build -A Win32
#                   cmake --build build --config Release      -> build\Release\xikit.dll
# The reconstruction DLL (gfx_recon_ingame.dll) and the manifest come from build.sh (MinGW, $FFXI_DLL).
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Build 32-bit: cmake -B build -A Win32")
endif()

add_library(xikit SHARED xikit.c ../d3dprof/d3dprof.c ../d3dprof/fcap.c)
target_compile_definitions(xikit PRIVATE XIKIT)
if(MSVC)
    # static runtime: the DLL needs no Visual C++ redistributable in the game's process
    set_property(TARGET xikit PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_options(xikit PRIVATE /W3)
    target_compile_definitions(xikit PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_link_options(xikit PRIVATE -static-libgcc -Wl,--kill-at)
endif()
