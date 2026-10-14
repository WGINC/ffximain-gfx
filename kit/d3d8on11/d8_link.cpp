// d8_link.cpp -- the command stream's transports (d8_link.h).
//
// The ring's counters are published with sequentially consistent atomics (__atomic, GCC and clang), so the same code
// is right between two threads and between an x86 process and an ARM64 one on shared memory. Neither side makes a
// system call per record: the reader sleeps on an event only when the ring is empty and says so in `reader_sleeping`;
// the writer signals only then. Each side checks again after saying it sleeps, so no wake-up is lost.
#include "d8_link.h"
#include "d8_exec.h"
#include <stdio.h>
#include <string.h>

static inline uint32_t ld(volatile uint32_t *p) { return __atomic_load_n(p, __ATOMIC_SEQ_CST); }
static inline void st(volatile uint32_t *p, uint32_t v) { __atomic_store_n(p, v, __ATOMIC_SEQ_CST); }
static inline uint32_t pad8(uint32_t n) { return (n + 7) & ~7u; }
static bool is_sync(uint16_t op) { return op == X_INIT || op == X_RESIZE || op == X_READBACK || op == X_FINISH; }
#define reader_sleeping pad[0]
#define writer_waiting pad[1]
enum { MAX_AHEAD = 2 };                       // frames the game may be ahead of the executor's Presents

// ---- the reader (the executor's side) -----------------------------------------------------------------------------
void ring_consume(RingHdr *h, uint8_t *data, uint8_t *reply, Exec *x, HANDLE wake, HANDLE space, HANDLE replied, HANDLE parent) {
  std::vector<uint8_t> out;
  for (;;) {
    uint32_t r = h->read, w = ld(&h->write);
    if (r == w) {
      st(&h->reader_sleeping, 1);
      if (ld(&h->write) == r) {
        if (ld(&h->quit)) return;
        HANDLE hs[2] = {wake, parent};
        DWORD got = WaitForMultipleObjects(parent ? 2 : 1, hs, FALSE, 50);
        if (got == WAIT_OBJECT_0 + 1) { d8_log("the game's process ended: the executor stops"); st(&h->reader_sleeping, 0); return; }
      }
      st(&h->reader_sleeping, 0);
      continue;
    }
    const XHdr *hd = (const XHdr *)(data + r % h->size);
    uint32_t need = 8 + pad8(hd->bytes);
    if (hd->op == X_QUIT) { st(&h->read, r + need); return; }
    if (hd->op != X_PAD) {
      int32_t hr = 0; out.clear();
      x->run(hd->op, (const uint8_t *)(hd + 1), hd->bytes, &hr, &out);
      if (is_sync(hd->op)) {
        uint32_t n = (uint32_t)out.size();
        if (n > h->reply_max) { d8_log("a reply of %u bytes is larger than the reply area (%u): cut", n, h->reply_max); n = h->reply_max; }
        if (n) memcpy(reply, out.data(), n);
        h->reply_hr = hr; h->reply_bytes = n;
        st(&h->answered, ld(&h->answered) + 1);
        SetEvent(replied);
      }
      if (hd->op == X_PRESENT) { st(&h->presents_done, ld(&h->presents_done) + 1); SetEvent(replied); }
    }
    st(&h->read, r + need);
    if (ld(&h->writer_waiting)) SetEvent(space);
  }
}

// ---- the writer (the D3D8 side): the ring in memory or in a mapping ----------------------------------------------------
struct RingLink : Link {
  RingHdr *h = nullptr; uint8_t *data = nullptr, *reply = nullptr;
  HANDLE wake = NULL, space = NULL, replied = NULL;
  HANDLE peer = NULL;                                            // remote: the executor's process
  CRITICAL_SECTION lock;
  // every wait for the executor: if its process has ended nothing will ever answer, and the game's state is in it
  void wait_for(HANDLE ev) {
    HANDLE hs[2] = {ev, peer};
    if (WaitForMultipleObjects(peer ? 2 : 1, hs, FALSE, 5) == WAIT_OBJECT_0 + 1) {
      DWORD code = 0; GetExitCodeProcess(peer, &code);
      d8_log("the executor process ended (exit code %lu; d3d8on11_exec.log): the game cannot go on", (unsigned long)code);
      ExitProcess(3);
    }
  }
  uint32_t cur_w = 0, cur_need = 0, sent = 0; uint16_t cur_op = 0;
  RingLink() { InitializeCriticalSection(&lock); }
  uint8_t *begin(uint16_t op, uint32_t bytes) override {
    EnterCriticalSection(&lock);
    uint32_t need = 8 + pad8(bytes), size = h->size;
    if (need > size / 2) { d8_log("a record of %u bytes does not fit the ring (%u): the device stops", bytes, size); ExitProcess(3); }
    uint32_t w = h->write, pos = w % size, padb = pos + need > size ? size - pos : 0;
    while (size - (w - ld(&h->read)) < need + padb) {           // wait for room
      st(&h->writer_waiting, 1);
      if (size - (w - ld(&h->read)) < need + padb) wait_for(space);
      st(&h->writer_waiting, 0);
    }
    if (padb) { XHdr *p = (XHdr *)(data + pos); p->op = X_PAD; p->pad = 0; p->bytes = padb - 8; w += padb; pos = 0; }
    XHdr *hd = (XHdr *)(data + pos); hd->op = op; hd->pad = 0; hd->bytes = bytes;
    cur_w = w; cur_need = need; cur_op = op;
    return (uint8_t *)(hd + 1);
  }
  void end() override {
    st(&h->write, cur_w + cur_need);
    if (ld(&h->reader_sleeping)) SetEvent(wake);
    if (cur_op == X_PRESENT) {                                   // pacing: at most MAX_AHEAD frames queued
      uint32_t sent_p = ld(&h->presents_sent) + 1; st(&h->presents_sent, sent_p);
      while (sent_p - ld(&h->presents_done) > MAX_AHEAD) wait_for(replied);
    }
    if (is_sync(cur_op)) { sent++; return; }                     // the lock is kept until answer()
    LeaveCriticalSection(&lock);
  }
  int32_t answer(std::vector<uint8_t> *out) override {
    while ((int32_t)(ld(&h->answered) - sent) < 0) wait_for(replied);
    int32_t hr = h->reply_hr;
    if (out) out->assign(reply, reply + h->reply_bytes);
    LeaveCriticalSection(&lock);
    return hr;
  }
};
static void ring_init(RingHdr *h, uint32_t size, uint32_t reply_max) {
  memset(h, 0, sizeof *h);
  h->magic = RING_MAGIC; h->version = RING_VERSION; h->size = size; h->reply_max = reply_max; h->parent_pid = GetCurrentProcessId();
}
enum : uint32_t { RING_BYTES = 32u << 20, REPLY_BYTES = 16u << 20 };

// thread: the ring in this process, the executor on a thread of its own
struct ThreadLink : RingLink {
  Exec x; HANDLE thread = NULL; uint8_t *mem = nullptr;
  static DWORD WINAPI main(LPVOID p) {
    ThreadLink *t = (ThreadLink *)p;
    ring_consume(t->h, t->data, t->reply, &t->x, t->wake, t->space, t->replied, NULL);
    t->x.shutdown();
    return 0;
  }
  bool start() {
    mem = (uint8_t *)VirtualAlloc(NULL, 4096 + RING_BYTES + REPLY_BYTES, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem) return false;
    h = (RingHdr *)mem; data = mem + 4096; reply = data + RING_BYTES;
    ring_init(h, RING_BYTES, REPLY_BYTES);
    wake = CreateEventA(NULL, FALSE, FALSE, NULL); space = CreateEventA(NULL, FALSE, FALSE, NULL); replied = CreateEventA(NULL, FALSE, FALSE, NULL);
    thread = CreateThread(NULL, 0, main, this, 0, NULL);
    return thread != NULL;
  }
  ~ThreadLink() override {
    if (thread) { put(X_QUIT, nullptr, 0); WaitForSingleObject(thread, 5000); CloseHandle(thread); }
    if (mem) VirtualFree(mem, 0, MEM_RELEASE);
  }
  const char *name() const override { return "thread"; }
};

// remote: the ring in a shared mapping, the executor in d8x.exe (d8x_arm64.exe on an ARM64 machine, when it is there)
struct RemoteLink : RingLink {
  HANDLE map = NULL, proc = NULL; uint8_t *mem = nullptr; char exe[MAX_PATH] = "";
  bool start() {
    char nm[96]; static LONG n;
    snprintf(nm, sizeof nm, "Local\\d3d8on11_%lu_%ld", (unsigned long)GetCurrentProcessId(), (long)InterlockedIncrement(&n));
    map = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096 + RING_BYTES + REPLY_BYTES, nm);
    if (!map) return false;
    mem = (uint8_t *)MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!mem) return false;
    h = (RingHdr *)mem; data = mem + 4096; reply = data + RING_BYTES;
    ring_init(h, RING_BYTES, REPLY_BYTES);
    char e[128];
    snprintf(e, sizeof e, "%s_w", nm); wake = CreateEventA(NULL, FALSE, FALSE, e);
    snprintf(e, sizeof e, "%s_s", nm); space = CreateEventA(NULL, FALSE, FALSE, e);
    snprintf(e, sizeof e, "%s_r", nm); replied = CreateEventA(NULL, FALSE, FALSE, e);
    // the executor next to this DLL: the ARM64 build on an ARM64 machine, else the x86 one
    char dir[MAX_PATH]; HMODULE m = NULL;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&ring_init, &m);
    GetModuleFileNameA(m, dir, sizeof dir); char *s = strrchr(dir, '\\'); if (s) s[1] = 0; else dir[0] = 0;
    USHORT pm = 0, nmach = 0;
    typedef BOOL (WINAPI *PW2)(HANDLE, USHORT *, USHORT *);
    PW2 w2 = (PW2)(void *)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process2");
    if (w2) w2(GetCurrentProcess(), &pm, &nmach);
    snprintf(exe, sizeof exe, "%sd8x_arm64.exe", dir);
    if (nmach != 0xAA64 || GetFileAttributesA(exe) == INVALID_FILE_ATTRIBUTES) snprintf(exe, sizeof exe, "%sd8x.exe", dir);
    char cmd[MAX_PATH + 160]; snprintf(cmd, sizeof cmd, "\"%s\" %s %lu", exe, nm, (unsigned long)GetCurrentProcessId());
    STARTUPINFOA si = {}; si.cb = sizeof si; PROCESS_INFORMATION pi = {};
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, dir, &si, &pi)) { d8_log("could not start the executor %s: error %lu", exe, (unsigned long)GetLastError()); return false; }
    proc = pi.hProcess; CloseHandle(pi.hThread);
    for (int i = 0; i < 1000 && !ld(&h->ready); i++) {           // up to 10 s
      if (WaitForSingleObject(proc, 10) == WAIT_OBJECT_0) { d8_log("the executor %s ended at its start", exe); return false; }
    }
    if (!ld(&h->ready)) { d8_log("the executor %s did not start in 10 s", exe); return false; }
    d8_log("executor process: %s (machine 0x%X)", exe, (unsigned)nmach);
    peer = proc;
    return true;
  }
  ~RemoteLink() override {
    if (proc && WaitForSingleObject(proc, 0) == WAIT_OBJECT_0) { CloseHandle(proc); proc = NULL; }
    peer = NULL;
    if (proc) { if (h && ld(&h->ready)) put(X_QUIT, nullptr, 0); if (WaitForSingleObject(proc, 3000) != WAIT_OBJECT_0) TerminateProcess(proc, 1); CloseHandle(proc); }
    if (mem) UnmapViewOfFile(mem); if (map) CloseHandle(map);
  }
  const char *name() const override { return "remote"; }
};

// direct: the executor runs each record as it is written (phase 1)
struct DirectLink : Link {
  Exec x; std::vector<uint8_t> buf; uint32_t n = 0; uint16_t op = 0; int32_t hr = 0; std::vector<uint8_t> out;
  CRITICAL_SECTION lock;
  DirectLink() { InitializeCriticalSection(&lock); }
  ~DirectLink() override { x.shutdown(); }
  uint8_t *begin(uint16_t o, uint32_t bytes) override { EnterCriticalSection(&lock); op = o; n = bytes; buf.resize(bytes ? bytes : 1); return buf.data(); }
  void end() override {
    out.clear(); hr = 0;
    x.run(op, buf.data(), n, &hr, &out);
    if (!is_sync(op)) LeaveCriticalSection(&lock);
  }
  int32_t answer(std::vector<uint8_t> *o) override { if (o) *o = out; int32_t r = hr; LeaveCriticalSection(&lock); return r; }
  const char *name() const override { return "direct"; }
};

Link *link_make(const char *mode) {
  if (!strcmp(mode, "direct")) return new DirectLink();
  if (!strcmp(mode, "remote")) { RemoteLink *r = new RemoteLink(); if (r->start()) return r; delete r; return nullptr; }
  ThreadLink *t = new ThreadLink(); if (t->start()) return t; delete t; return nullptr;
}
