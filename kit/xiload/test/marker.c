/* marker.c -- the DLL test_xiload.py has xiload load: DllMain writes marker.txt with its process id */
#include <windows.h>
#include <stdio.h>
BOOL WINAPI DllMain(HINSTANCE h, DWORD why, LPVOID r) {
  (void)h; (void)r;
  if (why == DLL_PROCESS_ATTACH) { FILE *f = fopen("marker.txt", "w"); if (f) { fprintf(f, "%lu\n", GetCurrentProcessId()); fclose(f); } }
  return TRUE;
}
