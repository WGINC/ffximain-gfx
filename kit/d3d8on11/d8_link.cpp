// d8_link.cpp -- the command stream's transports (d8_link.h).
//
// The ring's counters are published with sequentially consistent atomics (__atomic, GCC and clang), so the same code
// is right between two threads and between an x86 process and an ARM64 one on shared memory. Neither side makes a
// system call per record: the reader sleeps on an event only when the ring is empty and says so in `reader_sleeping`;
// the writer signals only then. Each side checks again after saying it sleeps, so no wake-up is lost.
#include "d8_link.h"
#include "d8_exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline uint32_t ld(volatile uint32_t *p) { return __atomic_load_n(p, __ATOMIC_SEQ_CST); }
static inline void st(volatile uint32_t *p, uint32_t v) { __atomic_store_n(p, v, __ATOMIC_SEQ_CST); }
static inline uint32_t pad8(uint32_t n) { return (n + 7) & ~7u; }
static bool is_sync(uint16_t op) { return op == X_INIT || op == X_RESIZE || op == X_READBACK || op == X_FINISH; }
#define reader_sleeping pad[0]
#define writer_waiting pad[1]
enum { MAX_AHEAD = 2 };                       // frames the game may be ahead of the executor's Presents
enum { STATS_EVERY = 600 };                   // Presents between the stats lines in the logs

// time, for the stats lines (QueryPerformanceCounter)
static double qpc_ms() {
  static LARGE_INTEGER f; if (!f.QuadPart) QueryPerformanceFrequency(&f);
  LARGE_INTEGER t; QueryPerformanceCounter(&t); return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
// the executor's side of the stats: the time spent running records, per frame, split by kind (draws; texture and
// buffer uploads; Present, which is IDXGISwapChain::Present and so includes any wait the swap chain makes; readbacks
// (X_READBACK, which waits for the GPU to finish the copy it reads); the rest).
// An X_OPTION (the A/B, d8_device.cpp) ends a phase: its figures are logged under the option's previous value and
// added to that value's totals, leaving out the first `settle` frames after each switch.
struct ExecStats {
  enum { K_DRAW, K_UPLOAD, K_PRESENT, K_READBACK, K_OTHER, NK };
  double busy[NK] = {}, since = 0; uint32_t frames = 0, records = 0;
  Exec *x = nullptr; uint64_t x0[3] = {};                  // the executor's batch counters at the last stats line
  static void hook(void *s, uint16_t op, double ms) { ((ExecStats *)s)->ran(op, ms); }
  void attach(Exec *e) { x = e; e->timed = hook; e->timed_ctx = this; }
  // the A/B: the phase now running, its option value, frames still to leave out, totals per value (0 and 1)
  bool ab = false; uint32_t key = OPT_FILTER, value = 0, settle = 0, phase = 0, pframes = 0; double pbusy[NK] = {}, tbusy[2][NK] = {}; uint32_t tframes[2] = {};
  static int kind(uint16_t op) {
    return op == X_DRAW ? K_DRAW : op == X_UPLOAD_TEX || op == X_UPLOAD_BUF ? K_UPLOAD : op == X_PRESENT ? K_PRESENT :
           op == X_READBACK ? K_READBACK : K_OTHER;
  }
  const char *kname() const { return key == OPT_BATCH ? "batch" : "filter"; }
  static double sum(const double *b) { double s = 0; for (int k = 0; k < NK; k++) s += b[k]; return s; }
  void phase_end() {
    if (!ab) return;
    if (pframes) {
      uint32_t v = value ? 1 : 0;
      d8_log("executor ab: phase %u, %s=%u, %u frames: busy %.3f ms a frame (draws %.3f, uploads %.3f, Present %.3f, "
             "readbacks %.3f, other %.3f)", phase, kname(), value, pframes, sum(pbusy) / pframes, pbusy[0] / pframes, pbusy[1] / pframes,
             pbusy[2] / pframes, pbusy[3] / pframes, pbusy[4] / pframes);
      for (int k = 0; k < NK; k++) tbusy[v][k] += pbusy[k];
      tframes[v] += pframes;
      for (int i = 0; i < 2; i++) if (tframes[i])
        d8_log("executor ab: so far %s=%d: %u frames, busy %.3f ms a frame (draws %.3f, uploads %.3f, Present %.3f, "
               "readbacks %.3f, other %.3f)", kname(), i, tframes[i], sum(tbusy[i]) / tframes[i], tbusy[i][0] / tframes[i],
               tbusy[i][1] / tframes[i], tbusy[i][2] / tframes[i], tbusy[i][3] / tframes[i], tbusy[i][4] / tframes[i]);
    }
    for (int k = 0; k < NK; k++) pbusy[k] = 0;
    pframes = 0;
  }
  void option(const uint8_t *p) {
    const XOption &o = *(const XOption *)p;
    if (!o.settle && !ab) return;                         // a setting at the start, not the A/B's
    phase_end();
    key = o.key;
    if (ab || o.settle) { ab = true; phase++; }
    value = o.value; settle = o.settle;
  }
  void ran(uint16_t op, double ms) {
    int k = kind(op);
    busy[k] += ms; records++;
    if (ab && !settle) pbusy[k] += ms;
    if (op != X_PRESENT) return;
    if (ab) { if (settle) settle--; else pframes++; }
    if (++frames < STATS_EVERY) return;
    report();
  }
  // the stats line for the frames since the last one; also at shutdown (finish), so that a run shorter than
  // STATS_EVERY Presents (a replay) still gets one, and the A/B's last phase is logged
  void finish() { phase_end(); if (frames) report(); }
  void report() {
    double now = qpc_ms(), span = since ? now - since : 0;
    d8_log("executor stats, %u frames: busy %.3f ms a frame (%.1f%% of the time; draws %.3f, uploads %.3f, Present %.3f, "
           "readbacks %.3f, other %.3f), %u records a frame", frames, sum(busy) / frames, span > 0 ? 100.0 * sum(busy) / span : 0.0,
           busy[0] / frames, busy[1] / frames, busy[2] / frames, busy[3] / frames, busy[4] / frames, records / frames);
    if (x) {
      // Maps a frame: of the arenas (one each per flush, Exec::plan) and by draws themselves (constants, ...UP data)
      d8_log("executor batches, %u frames: batch=%u, %.1f flushes, %.1f arena Maps, %.1f draw Maps a frame", frames, x->batch,
             (double)(x->flushes - x0[0]) / frames, (double)(x->arena_maps - x0[1]) / frames, (double)(x->draw_maps - x0[2]) / frames);
      x0[0] = x->flushes; x0[1] = x->arena_maps; x0[2] = x->draw_maps;
    }
    for (int i = 0; i < NK; i++) busy[i] = 0;
    frames = records = 0; since = now;
  }
};

// ---- the reader (the executor's side) -----------------------------------------------------------------------------
void ring_consume(RingHdr *h, uint8_t *data, uint8_t *reply, Exec *x, HANDLE wake, HANDLE space, HANDLE replied, HANDLE parent) {
  std::vector<uint8_t> out; ExecStats xs; xs.since = qpc_ms(); xs.attach(x);
  struct Finish { ExecStats &s; ~Finish() { s.finish(); } } fin{xs};   // every return below logs the last figures
  // r: the next record; h->read: the first byte the writer may not reuse. While the executor holds records (Exec::batch)
  // their bytes stay in the ring: h->read stays at the first held one, and moves when they have run. They run when the
  // ring is empty, when the writer waits for room, or at the next record that is not held (Exec::run)
  uint32_t r = h->read;
  auto release = [&]() { st(&h->read, r); if (ld(&h->writer_waiting)) SetEvent(space); };
  for (;;) {
    uint32_t w = ld(&h->write);
    if (r != w && x->holding() && ld(&h->writer_waiting)) { x->flush(); release(); }
    if (r == w) {
      if (x->holding()) { x->flush(); release(); continue; }
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
#ifdef D8_TRACE
    d8_log("R r=%u w=%u op=%u bytes=%u", r, w, hd->op, hd->bytes);
#endif
    if (hd->op == X_QUIT) { x->flush(); st(&h->read, r + need); return; }
    if (hd->op != X_PAD) {
      int32_t hr = 0; out.clear();
      x->run(hd->op, (const uint8_t *)(hd + 1), hd->bytes, &hr, &out);
      if (hd->op == X_OPTION) xs.option((const uint8_t *)(hd + 1));
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
    r += need;
    if (!x->holding()) release();
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
  // the game's side of the stats: time waited for room, for pacing, for answers; bytes and records sent
  double waited[3] = {0, 0, 0}, since = 0; uint64_t sent_bytes = 0; uint32_t records = 0, frames = 0;
  RingLink() { InitializeCriticalSection(&lock); since = qpc_ms(); }
  uint32_t max_payload() const override { return h->size / 2 - 8; }   // begin's limit: 8 + pad8(bytes) <= size / 2
  void stats_frame() {
    if (++frames < STATS_EVERY) return;
    double now = qpc_ms();
    d8_log("ring stats, %u frames: the game waited %.3f ms a frame for room, %.3f for pacing (%d frames ahead), %.3f for "
           "answers; sent %.2f MB and %u records a frame", frames, waited[0] / frames,
           waited[1] / frames, (int)MAX_AHEAD, waited[2] / frames, (double)sent_bytes / frames / 1048576.0, records / frames);
    waited[0] = waited[1] = waited[2] = 0; sent_bytes = 0; records = frames = 0; since = now;
  }
  uint8_t *begin(uint16_t op, uint32_t bytes) override {
    EnterCriticalSection(&lock);
    uint32_t need = 8 + pad8(bytes), size = h->size;             // size: a power of two (ring_bytes)
    if (need > size / 2) { d8_log("a record of %u bytes does not fit the ring (%u): the device stops", bytes, size); ExitProcess(3); }
    uint32_t w = h->write, pos = w % size, padb = pos + need > size ? size - pos : 0;
    if (size - (w - ld(&h->read)) < need + padb) {
      double t0 = qpc_ms();
      while (size - (w - ld(&h->read)) < need + padb) {         // wait for room
        st(&h->writer_waiting, 1);
        if (size - (w - ld(&h->read)) < need + padb) wait_for(space);
        st(&h->writer_waiting, 0);
      }
      waited[0] += qpc_ms() - t0;
    }
    sent_bytes += need + padb; records++;
    if (padb) { XHdr *p = (XHdr *)(data + pos); p->op = X_PAD; p->pad = 0; p->bytes = padb - 8; w += padb; pos = 0; }
    XHdr *hd = (XHdr *)(data + pos); hd->op = op; hd->pad = 0; hd->bytes = bytes;
#ifdef D8_TRACE
    d8_log("W w=%u op=%u bytes=%u", w, op, bytes);
#endif
    cur_w = w; cur_need = need; cur_op = op;
    return (uint8_t *)(hd + 1);
  }
  void end() override {
    st(&h->write, cur_w + cur_need);
    if (ld(&h->reader_sleeping)) SetEvent(wake);
    if (cur_op == X_PRESENT) {                                   // pacing: at most MAX_AHEAD frames queued
      uint32_t sent_p = ld(&h->presents_sent) + 1; st(&h->presents_sent, sent_p);
      if (sent_p - ld(&h->presents_done) > MAX_AHEAD) {
        double t0 = qpc_ms();
        while (sent_p - ld(&h->presents_done) > MAX_AHEAD) wait_for(replied);
        waited[1] += qpc_ms() - t0;
      }
      stats_frame();
    }
    if (is_sync(cur_op)) { sent++; return; }                     // the lock is kept until answer()
    LeaveCriticalSection(&lock);
  }
  int32_t answer(std::vector<uint8_t> *out) override {
    double t0 = qpc_ms();
    while ((int32_t)(ld(&h->answered) - sent) < 0) wait_for(replied);
    waited[2] += qpc_ms() - t0;
    int32_t hr = h->reply_hr;
    if (out) out->assign(reply, reply + h->reply_bytes);
    LeaveCriticalSection(&lock);
    return hr;
  }
};
static void ring_init(RingHdr *h, uint32_t size, uint32_t reply_max) {
  memset(h, 0, sizeof *h);
  h->magic = RING_MAGIC; h->version = RING_VERSION; h->size = size; h->reply_max = reply_max; h->parent_pid = GetCurrentProcessId();
  // D8ON11_RING_START (hex, a multiple of 8): where the byte counters start, so that a test reaches their wrap at 2^32
  char v[16]; DWORD n = GetEnvironmentVariableA("D8ON11_RING_START", v, sizeof v);
  if (n > 0 && n < sizeof v) { uint32_t s0 = (uint32_t)strtoul(v, NULL, 16) & ~7u; h->write = h->read = s0; d8_log("ring: counters start at 0x%08X (D8ON11_RING_START)", s0); }
}
enum : uint32_t { RING_BYTES = 32u << 20, REPLY_BYTES = 16u << 20, SNAP_TABLE = SNAP_SLOTS * (uint32_t)sizeof(SnapSlot) };
// the ring's size: 32 MB, or D8ON11_RING_KB (a power of two from 64 to 32768; the tests make it small, so that it wraps
// often). A power of two because positions are counter % size and the counters wrap at 2^32: only a size dividing 2^32
// keeps the position continuous across that wrap (at 9 MB a frame the counters wrap every few minutes).
static uint32_t ring_bytes() {
  char v[16]; DWORD n = GetEnvironmentVariableA("D8ON11_RING_KB", v, sizeof v);
  unsigned long kb = n > 0 && n < sizeof v ? strtoul(v, NULL, 10) : 0;
  if (kb < 64 || kb > (RING_BYTES >> 10) || (kb & (kb - 1))) return RING_BYTES;
  d8_log("ring: %lu KB (D8ON11_RING_KB)", kb);
  return (uint32_t)kb << 10;
}

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
    uint32_t rb = ring_bytes();
    mem = (uint8_t *)VirtualAlloc(NULL, 4096 + rb + REPLY_BYTES + SNAP_TABLE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem) return false;
    h = (RingHdr *)mem; data = mem + 4096; reply = data + rb;
    snaps = x.snaps = (SnapSlot *)(reply + REPLY_BYTES);
    ring_init(h, rb, REPLY_BYTES);
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
    uint32_t rb = ring_bytes();
    map = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4096 + rb + REPLY_BYTES + SNAP_TABLE, nm);
    if (!map) return false;
    mem = (uint8_t *)MapViewOfFile(map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!mem) return false;
    h = (RingHdr *)mem; data = mem + 4096; reply = data + rb;
    snaps = (SnapSlot *)(reply + REPLY_BYTES);                 // the executor finds it at the same place (d8x_main.cpp)
    ring_init(h, rb, REPLY_BYTES);
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
  Exec x; std::vector<uint8_t> buf; uint32_t n = 0; uint16_t op = 0; int32_t hr = 0; std::vector<uint8_t> out; ExecStats xs;
  CRITICAL_SECTION lock;
  DirectLink() {
    InitializeCriticalSection(&lock); xs.since = qpc_ms(); xs.attach(&x); x.copy_held = true;
    snaps = x.snaps = (SnapSlot *)VirtualAlloc(NULL, SNAP_TABLE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  }
  ~DirectLink() override { x.flush(); xs.finish(); x.shutdown(); if (snaps) VirtualFree(snaps, 0, MEM_RELEASE); }
  uint8_t *begin(uint16_t o, uint32_t bytes) override { EnterCriticalSection(&lock); op = o; n = bytes; buf.resize(bytes ? bytes : 1); return buf.data(); }
  void end() override {
    out.clear(); hr = 0;
    x.run(op, buf.data(), n, &hr, &out);
    if (op == X_OPTION) xs.option(buf.data());
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
