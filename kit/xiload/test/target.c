/* target.c -- a stand-in pol.exe for test_xiload.py: waits up to 20 s for marker.dll to be loaded into it */
#include <windows.h>
#include <stdio.h>
int main(void) {
  for (int i = 0; i < 200; i++) { if (GetModuleHandleA("marker.dll")) { printf("marker.dll loaded\n"); return 0; } Sleep(100); }
  printf("marker.dll not loaded\n"); return 1;
}
