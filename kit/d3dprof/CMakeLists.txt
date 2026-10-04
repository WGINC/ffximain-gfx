cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
project(d3dprof C)

# 32-bit only: it is injected into pol.exe, which is 32-bit, and its thunks are x86 inline assembly.
#   Visual Studio:  cmake -B build -A Win32
#                   cmake --build build --config Release      -> build\Release\d3dprof.dll
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Build 32-bit: cmake -B build -A Win32")
endif()

add_library(d3dprof SHARED d3dprof.c)
if(MSVC)
    # static runtime: the DLL needs no Visual C++ redistributable in the game's process
    set_property(TARGET d3dprof PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_options(d3dprof PRIVATE /W3)
    target_compile_definitions(d3dprof PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_link_options(d3dprof PRIVATE -static-libgcc)
endif()
