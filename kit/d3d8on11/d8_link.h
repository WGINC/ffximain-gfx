// d8_link.h -- how d3d8on11's command stream reaches the executor (d8_exec.cpp). Three ways, one interface:
//   direct  the executor runs each record as it is written, on the writing thread (phase 1);
//   thread  the records go into a ring in memory; the executor runs them on a thread of its own (phase 2, design A2);
//   remote  the ring is a shared-memory mapping; the executor runs in a process of its own, d8x.exe, built for x86
//           or natively for ARM64 ("ARM mode", design B), and presents into the game's window.
// The ring: one writer (any thread of the game, under a lock), one reader. Records are 8-byte aligned (d8_proto.h);
// a record that would cross the end is preceded by an X_PAD record to the end.
#pragma once
#include "d8_common.h"
#include "d8_proto.h"
#include <vector>

struct Link {
  virtual ~Link() {}
  // a record of `bytes` payload: write it at the pointer, then end(). Holds the writer's lock in between.
  virtual uint8_t *begin(uint16_t op, uint32_t bytes) = 0;
  virtual void end() = 0;
  // after a sync record (X_INIT, X_RESIZE, X_READBACK, X_FINISH): wait for its answer
  virtual int32_t answer(std::vector<uint8_t> *reply) = 0;
  virtual const char *name() const = 0;
  // writes a record of a struct and data in one call
  void put(uint16_t op, const void *a, uint32_t na, const void *b = nullptr, uint32_t nb = 0) {
    uint8_t *p = begin(op, na + nb); memcpy(p, a, na); if (nb) memcpy(p + na, b, nb); end();
  }
  int32_t call(uint16_t op, const void *a, uint32_t na, std::vector<uint8_t> *reply = nullptr) { put(op, a, na); return answer(reply); }
};
// mode: "direct", "thread" or "remote" (d3d8on11.ini [d3d8on11] mode=, or D8ON11_MODE); NULL on failure
Link *link_make(const char *mode);

// the shared ring, as both sides see it (the remote executor maps the same layout)
struct RingHdr {
  uint32_t magic, version, size, reply_max;      // the ring's data bytes, the reply area's
  volatile uint32_t write, read;                 // byte counters (mod 2^32), the ring position is counter % size
  volatile uint32_t answered;                    // sync records answered
  volatile uint32_t presents_sent, presents_done;
  volatile uint32_t ready, quit;                 // the executor is up / asked to stop
  int32_t reply_hr; uint32_t reply_bytes;
  uint32_t parent_pid;
  uint32_t pad[2];
};
#define RING_MAGIC 0x52384438u                    /* "8D8R" */
#define RING_VERSION 1u
// the executor's side: run records until X_QUIT (remote) or quit is set; `wake` is signalled when data comes,
// `space` / `replied` signalled by the reader
struct Exec;
void ring_consume(RingHdr *h, uint8_t *data, uint8_t *reply, Exec *x, HANDLE wake, HANDLE space, HANDLE replied, HANDLE parent);
