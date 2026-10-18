/* target.c -- the test's x86 process (test_xisamp.py): a known call chain on one thread.
   main -> chain_outer -> chain_inner -> spin (busy) for the first half, then main -> chain_outer -> chain_inner ->
   nap (Sleep: a wait in Windows) for the second. Writes its pid and thread id to the file named on the command line. */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
static volatile unsigned g_sink;
__attribute__((noinline)) void spin(DWORD until) { while (GetTickCount() < until) for (int i = 0; i < 1000; i++) g_sink += i; }
__attribute__((noinline)) void nap(DWORD until) { while (GetTickCount() < until) Sleep(5); }
__attribute__((noinline)) void chain_inner(int busy, DWORD until) { if (busy) spin(until); else nap(until); g_sink++; }
__attribute__((noinline)) void chain_outer(int busy, DWORD until) { chain_inner(busy, until); g_sink++; }
int main(int argc, char **argv) {
  int ms = argc > 2 ? atoi(argv[2]) : 4000;
  FILE *f = fopen(argv[1], "w"); fprintf(f, "%lu %lu\n", GetCurrentProcessId(), GetCurrentThreadId()); fclose(f);
  DWORD t = GetTickCount();
  chain_outer(1, t + ms / 2);
  chain_outer(0, t + ms);
  return 0;
}
