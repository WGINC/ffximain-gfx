/* target.c -- the test's x86 process (test_xisamp.py): a known call chain on one thread.
   main -> chain_outer -> chain_inner -> spin (busy) for the first half, then main -> chain_outer -> chain_inner ->
   nap (Sleep: a wait in Windows) for the second. Writes its pid, thread id and the address of its frame counter to the
   file named on the command line. Frames: chain_inner adds 1 to g_frame every 20 ms, but its 41st busy frame is a
   stall of 400 ms (chain_inner -> stall), the stutter the test looks for (frame= spike=, xisamp.c). */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static volatile unsigned g_sink; volatile unsigned g_frame;
__attribute__((noinline)) void spin(DWORD until) { while (GetTickCount() < until) for (int i = 0; i < 1000; i++) g_sink += i; }
__attribute__((noinline)) void nap(DWORD until) { while (GetTickCount() < until) Sleep(5); }
__attribute__((noinline)) void stall(DWORD until) { while (GetTickCount() < until) for (int i = 0; i < 1000; i++) g_sink -= i; }
__attribute__((noinline)) void chain_inner(int busy, DWORD until) {
  for (int k = 0; GetTickCount() < until; k++, g_frame++) {
    DWORD e = GetTickCount() + 20; if (e > until) e = until;
    if (busy && k == 40) stall(GetTickCount() + 400); else if (busy) spin(e); else nap(e);
  }
  g_sink++;
}
__attribute__((noinline)) void chain_outer(int busy, DWORD until) { chain_inner(busy, until); g_sink++; }
int main(int argc, char **argv) {
  int ms = argc > 2 ? atoi(argv[2]) : 4000;
  FILE *f = fopen(argv[1], "w"); fprintf(f, "%lu %lu %X\n", GetCurrentProcessId(), GetCurrentThreadId(), (unsigned)(uintptr_t)&g_frame); fclose(f);
  DWORD t = GetTickCount();
  chain_outer(1, t + ms / 2);
  chain_outer(0, t + ms);
  return 0;
}
