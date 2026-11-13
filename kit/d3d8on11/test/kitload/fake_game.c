/* fake_game.c -- a stand-in FFXiMain.dll for test_d3d8on11.py's load_kit check: a DLL of that name whose
   TimeDateStamp the test sets afterwards (d3d8.dll reads the stamp of the module in memory) */
#include <windows.h>
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID p) { (void)h; (void)r; (void)p; return TRUE; }
