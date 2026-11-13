/* fake_kit.c -- a stand-in xikit.dll for the load_kit check: when loaded it writes fake_kit_loaded.txt next to itself */
#include <windows.h>
#include <stdio.h>
#include <string.h>
BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID p) {
  (void)p;
  if (r == DLL_PROCESS_ATTACH) { char path[MAX_PATH]; GetModuleFileNameA(h, path, sizeof path); char *s = strrchr(path, '\\'); strcpy(s ? s + 1 : path, "fake_kit_loaded.txt");
    FILE *f = fopen(path, "w"); if (f) { fputs("loaded\n", f); fclose(f); } }
  return TRUE;
}
