/* xiload.c -- loads xikit.dll into the game: waits for pol.exe, then has it call LoadLibraryA on the kit (a thread
   the loader starts in pol.exe with CreateRemoteThread). The kit waits for FFXiMain.dll by itself, so pol.exe can be
   loaded into as soon as it starts (the PlayOnline viewer, before the game). Works the same on Windows (x86, x64, ARM64)
   and in Wine or a CrossOver bottle (run it in the same bottle as the game).

     xiload.exe                    waits for pol.exe, loads the xikit.dll next to xiload.exe, keeps watching for new
                                   pol.exe processes until closed
     xiload.exe --once             stops after the first load
     xiload.exe --dll PATH         another DLL (an absolute path, or one next to xiload.exe)
     xiload.exe --process NAME     another process name

   It never loads into a process that already has a module of the DLL's name, and it checks the load: the remote
   thread's exit value (LoadLibraryA's result) and the module list afterwards. kernel32's address in the target is read
   from its module list (not assumed to be this process's). Everything it does goes to xiload.log next to it.

   Build (32-bit, like pol.exe): build.sh */
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static char g_log[MAX_PATH];
static void say(const char *fmt, ...) {
  char b[1024]; va_list a; va_start(a, fmt); vsnprintf(b, sizeof b, fmt, a); va_end(a);
  SYSTEMTIME t; GetLocalTime(&t);
  printf("%02u:%02u:%02u %s\n", t.wHour, t.wMinute, t.wSecond, b); fflush(stdout);
  FILE *f = fopen(g_log, "a"); if (f) { fprintf(f, "%04u-%02u-%02u %02u:%02u:%02u %s\n", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, b); fclose(f); }
}
static const char *base_name(const char *p) { const char *s = strrchr(p, '\\'), *t = strrchr(p, '/'); if (t > s) s = t; return s ? s + 1 : p; }

/* the base of a module in another process, by name (0 if not loaded or the list cannot be read) */
static DWORD module_base(DWORD pid, const char *name) {
  HANDLE snap = INVALID_HANDLE_VALUE;
  for (int i = 0; i < 20 && snap == INVALID_HANDLE_VALUE; i++) {     /* ERROR_BAD_LENGTH while the process is starting: retry */
    snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snap == INVALID_HANDLE_VALUE) Sleep(50);
  }
  if (snap == INVALID_HANDLE_VALUE) return 0;
  MODULEENTRY32 me; me.dwSize = sizeof me; DWORD b = 0;
  for (BOOL ok = Module32First(snap, &me); ok; ok = Module32Next(snap, &me)) if (!lstrcmpiA(me.szModule, name)) { b = (DWORD)(UINT_PTR)me.modBaseAddr; break; }
  CloseHandle(snap);
  return b;
}

static int load_into(DWORD pid, const char *dll) {
  const char *dname = base_name(dll);
  if (module_base(pid, dname)) { say("process %lu already has %s: not loaded again", (unsigned long)pid, dname); return 1; }
  HANDLE p = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
  if (!p) { say("process %lu: OpenProcess failed (error %lu): run xiload as the same user as the game (or as administrator)", (unsigned long)pid, GetLastError()); return 0; }
  BOOL wow_me = FALSE, wow_it = FALSE; IsWow64Process(GetCurrentProcess(), &wow_me); IsWow64Process(p, &wow_it);
  if (wow_me != wow_it) say("note: this loader and process %lu differ in WOW64 state (%d, %d); continuing", (unsigned long)pid, wow_me, wow_it);
  HMODULE k32 = GetModuleHandleA("kernel32.dll");
  DWORD mine = (DWORD)(UINT_PTR)k32, theirs = module_base(pid, "kernel32.dll");
  if (!theirs) { say("process %lu: its kernel32.dll is not in its module list yet: retrying later", (unsigned long)pid); CloseHandle(p); return 0; }
  DWORD lla = (DWORD)(UINT_PTR)GetProcAddress(k32, "LoadLibraryA") - mine + theirs;
  size_t n = strlen(dll) + 1;
  void *mem = VirtualAllocEx(p, NULL, n, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if (!mem || !WriteProcessMemory(p, mem, dll, n, NULL)) { say("process %lu: could not write the path (error %lu)", (unsigned long)pid, GetLastError()); CloseHandle(p); return 0; }
  HANDLE t = CreateRemoteThread(p, NULL, 0, (LPTHREAD_START_ROUTINE)(UINT_PTR)lla, mem, 0, NULL);
  if (!t) { say("process %lu: CreateRemoteThread failed (error %lu)", (unsigned long)pid, GetLastError()); VirtualFreeEx(p, mem, 0, MEM_RELEASE); CloseHandle(p); return 0; }
  DWORD w = WaitForSingleObject(t, 30000), code = 0; GetExitCodeThread(t, &code);
  CloseHandle(t);
  if (w == WAIT_OBJECT_0) VirtualFreeEx(p, mem, 0, MEM_RELEASE);
  CloseHandle(p);
  DWORD b = module_base(pid, dname);
  if (w != WAIT_OBJECT_0) { say("process %lu: LoadLibraryA did not return within 30 s", (unsigned long)pid); return 0; }
  if (!code || !b) { say("process %lu: LoadLibraryA(%s) failed (returned %08lX; the DLL is %sin its module list). Is the path right, and are the kit's files next to it?",
                         (unsigned long)pid, dll, (unsigned long)code, b ? "" : "not "); return 0; }
  say("process %lu: %s loaded at %08lX (kernel32 there at %08lX, here at %08lX)", (unsigned long)pid, dll, (unsigned long)b, (unsigned long)theirs, (unsigned long)mine);
  return 1;
}

int main(int argc, char **argv) {
  char here[MAX_PATH], dll[MAX_PATH] = "", proc[MAX_PATH] = "pol.exe"; int once = 0;
  GetModuleFileNameA(NULL, here, sizeof here); char *s = strrchr(here, '\\'); if (s) *s = 0;
  snprintf(g_log, sizeof g_log, "%s\\xiload.log", here);
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--once")) once = 1;
    else if (!strcmp(argv[i], "--dll") && i + 1 < argc) snprintf(dll, sizeof dll, "%s", argv[++i]);
    else if (!strcmp(argv[i], "--process") && i + 1 < argc) snprintf(proc, sizeof proc, "%s", argv[++i]);
    else { printf("usage: xiload.exe [--once] [--dll PATH] [--process NAME]\n"); return 2; }
  }
  if (!dll[0]) snprintf(dll, sizeof dll, "%s\\xikit.dll", here);
  else if (!strchr(dll, '\\') && !strchr(dll, '/') && !strchr(dll, ':')) { char t[MAX_PATH]; snprintf(t, sizeof t, "%s\\%s", here, dll); snprintf(dll, sizeof dll, "%s", t); }
  char full[MAX_PATH]; if (GetFullPathNameA(dll, sizeof full, full, NULL)) snprintf(dll, sizeof dll, "%s", full);
  if (GetFileAttributesA(dll) == INVALID_FILE_ATTRIBUTES) { say("%s does not exist", dll); return 1; }
  say("xiload: waiting for %s, to load %s%s", proc, dll, once ? " (once)" : "; close this window to stop");
  DWORD done[256]; int ndone = 0;
  for (;;) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap != INVALID_HANDLE_VALUE) {
      PROCESSENTRY32 pe; pe.dwSize = sizeof pe;
      for (BOOL ok = Process32First(snap, &pe); ok; ok = Process32Next(snap, &pe)) {
        if (lstrcmpiA(base_name(pe.szExeFile), proc)) continue;
        int seen = 0; for (int i = 0; i < ndone; i++) if (done[i] == pe.th32ProcessID) seen = 1;
        if (seen) continue;
        say("found %s, process %lu", proc, (unsigned long)pe.th32ProcessID);
        Sleep(500);                                   /* its loader has mapped kernel32 by now */
        if (load_into(pe.th32ProcessID, dll)) {
          if (ndone < 256) done[ndone++] = pe.th32ProcessID;
          if (once) { CloseHandle(snap); return 0; }
        }
      }
      CloseHandle(snap);
    }
    Sleep(250);
  }
}
