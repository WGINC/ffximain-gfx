#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>
int main(int argc, char **argv) {
  HANDLE p = GetCurrentProcess(); SymSetOptions(SYMOPT_UNDNAME | SYMOPT_LOAD_ANYTHING);
  if (!SymInitialize(p, NULL, FALSE)) { printf("SymInitialize failed %lu\n", GetLastError()); return 1; }
  DWORD64 b = SymLoadModuleEx(p, NULL, argv[1], NULL, 0x10000000, 0xC15000, NULL, 0);
  printf("SymLoadModuleEx: base %llx (error %lu)\n", (unsigned long long)b, b ? 0 : GetLastError());
  IMAGEHLP_MODULE64 mi; memset(&mi, 0, sizeof mi); mi.SizeOfStruct = sizeof mi;
  if (SymGetModuleInfo64(p, 0x10000000, &mi)) printf("symbol type %d, %lu symbols, pdb %s\n", mi.SymType, mi.NumSyms, mi.LoadedPdbName);
  for (int i = 2; i < argc; i++) {
    char buf[sizeof(SYMBOL_INFO) + 256]; SYMBOL_INFO *s = (SYMBOL_INFO *)buf; memset(buf, 0, sizeof buf); s->SizeOfStruct = sizeof(SYMBOL_INFO); s->MaxNameLen = 255;
    DWORD64 a = strtoull(argv[i], NULL, 16), d = 0;
    if (SymFromAddr(p, a, &d, s)) printf("%llx = %s+0x%llx (size %lu)\n", (unsigned long long)a, s->Name, (unsigned long long)d, s->Size); else printf("%llx: SymFromAddr failed %lu\n", (unsigned long long)a, GetLastError());
  }
  char buf[sizeof(SYMBOL_INFO) + 256]; SYMBOL_INFO *s = (SYMBOL_INFO *)buf; memset(buf, 0, sizeof buf); s->SizeOfStruct = sizeof(SYMBOL_INFO); s->MaxNameLen = 255;
  if (SymFromName(p, "CApp::RunMainLoop", s)) printf("CApp::RunMainLoop = %llx\n", (unsigned long long)s->Address); else printf("SymFromName failed %lu\n", GetLastError());
  if (SymGetTypeFromName(p, 0x10000000, "CDx", s)) { ULONG64 len = 0; SymGetTypeInfo(p, 0x10000000, s->TypeIndex, TI_GET_LENGTH, &len); printf("type CDx: index %lu, %llu bytes\n", s->TypeIndex, (unsigned long long)len); } else printf("SymGetTypeFromName CDx failed %lu\n", GetLastError());
  return 0;
}
