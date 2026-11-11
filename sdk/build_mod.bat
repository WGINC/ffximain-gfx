@echo off
rem build_mod.bat -- build a mod DLL on Windows with MinGW-w64 i686 g++ on PATH (MSYS2 "mingw32", or WinLibs i686).
rem   build_mod.bat OUT.dll SOURCE.cpp [MORE.cpp ...]
rem The same flags as build_mod.sh (docs/SDK.md). The compiler must be the 32-bit one: the game is 32-bit.
rem A fork (NAME.fork.cpp) needs the tree's own flags: build a mod with forks with build_mod.sh (MSYS2's bash).
setlocal
set H=%~dp0
set TREE=%H%tree_include
if exist "%H%..\tree\include" set TREE=%H%..\tree\include
set OUT=%1
shift
set SRC=
:loop
if "%~1"=="" goto build
set SRC=%SRC% "%~1"
shift
goto loop
:build
if "%XIKIT_PDB%"=="1" goto pdb
g++ -m32 -std=c++17 -O2 -march=i686 -Wall -fpermissive -fno-strict-aliasing -fwrapv -isystem "%H%include" -isystem "%TREE%" -shared -o "%OUT%" %SRC% -static-libgcc -static-libstdc++ -Wl,--kill-at
if errorlevel 1 exit /b 1
echo built %OUT%
exit /b 0
:pdb
rem XIKIT_PDB=1: clang (llvm-mingw's, on PATH) with CodeView debug info and lld, which writes OUT's .pdb beside it:
rem a mod Visual Studio and WinDbg can debug at source level.
clang++ --target=i686-w64-mingw32 -std=c++17 -O1 -g -gcodeview -march=i686 -Wall -fpermissive -fms-extensions -fno-strict-aliasing -fwrapv -isystem "%H%include" -isystem "%TREE%" -shared -o "%OUT%" %SRC% -fuse-ld=lld -Wl,--pdb="%OUT:.dll=.pdb%" -static-libgcc -static-libstdc++ -Wl,--kill-at
if errorlevel 1 exit /b 1
echo built %OUT% and its .pdb
