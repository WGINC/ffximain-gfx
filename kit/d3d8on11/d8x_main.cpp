// d8x_main.cpp -- d8x.exe / d8x_arm64.exe: d3d8on11's executor in a process of its own ("ARM mode", design B).
//
//   d8x.exe MAPPING_NAME GAME_PID       (started by d3d8.dll: d8_link.cpp, RemoteLink)
//
// It opens the game's ring (the shared mapping and its three events), runs every record with the executor
// (d8_exec.cpp: Direct3D 11, presenting into the game's window, whose handle comes in X_INIT), and ends at X_QUIT or
// when the game's process ends. Built for x86 (the offline tests, under Wine) and natively for ARM64 (the owner's
// machine: Windows on ARM, where the game itself runs emulated). Its log is d3d8on11_exec.log next to it.
#include "d8_link.h"
#include "d8_exec.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  d8_log_name = "d3d8on11_exec.log";
  if (argc < 3) { d8_log("usage: d8x MAPPING_NAME GAME_PID"); return 2; }
  const char *nm = argv[1];
  HANDLE map = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, nm);
  if (!map) { d8_log("OpenFileMapping %s: error %lu", nm, (unsigned long)GetLastError()); return 2; }
  uint8_t *mem = (uint8_t *)MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
  if (!mem) { d8_log("MapViewOfFile: error %lu", (unsigned long)GetLastError()); return 2; }
  RingHdr *h = (RingHdr *)mem;
  if (h->magic != RING_MAGIC || h->version != RING_VERSION) { d8_log("the mapping is not a version %u ring", RING_VERSION); return 2; }
  char e[128]; HANDLE ev[3];
  const char *suf[3] = {"_w", "_s", "_r"};
  for (int i = 0; i < 3; i++) {
    snprintf(e, sizeof e, "%s%s", nm, suf[i]);
    if (!(ev[i] = OpenEventA(EVENT_ALL_ACCESS, FALSE, e))) { d8_log("OpenEvent %s: error %lu", e, (unsigned long)GetLastError()); return 2; }
  }
  HANDLE parent = OpenProcess(SYNCHRONIZE, FALSE, (DWORD)strtoul(argv[2], NULL, 10));
  SYSTEM_INFO si; GetNativeSystemInfo(&si);
  d8_log("d3d8on11 executor: process %lu for the game's %s, built for %s, native processor architecture %u",
         (unsigned long)GetCurrentProcessId(), argv[2],
#if defined(__aarch64__) || defined(_M_ARM64)
         "ARM64",
#else
         "x86",
#endif
         (unsigned)si.wProcessorArchitecture);
  Exec x;
  __atomic_store_n(&h->ready, 1u, __ATOMIC_SEQ_CST);
  ring_consume(h, mem + 4096, mem + 4096 + h->size, &x, ev[0], ev[1], ev[2], parent);
  d8_log("executor: %llu draws, %llu presents", (unsigned long long)x.draws, (unsigned long long)x.presents);
  x.shutdown();
  return 0;
}
