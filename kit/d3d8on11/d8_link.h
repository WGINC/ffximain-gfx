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
  // the snapshot table (d8_proto.h, SnapSlot; SNAP_SLOTS entries), shared with the executor; null: none
  SnapSlot *snaps = nullptr;
  // writes a record of a struct and data in one call
  void put(uint16_t op, const void *a, uint32_t na, const void *b = nullptr, uint32_t nb = 0) {
    uint8_t *p = begin(op, na + nb); memcpy(p, a, na); if (nb) memcpy(p + na, b, nb); end();
  }
  int32_t call(uint16_t op, const void *a, uint32_t na, std::vector<uint8_t> *reply = nullptr) { put(op, a, na); return answer(reply); }
  // the largest payload one record may carry (a ring refuses a record larger than half of it)
  virtual uint32_t max_payload() const { return 0xFFFFFFFFu; }
  // an upload of u.rows rows of u.rowbytes (each row ph pixels high: 4 in a block format, else 1), cut into bands of
  // whole rows when one record would not fit: a 2048x4096 32-bit level is 32 MB, the ring's whole size (owner, 15 Oct:
  // the game makes one at startup). A band has an explicit box; lw x lh: the subresource's size, for u.w 0 (all of it).
  void upload_tex(XUpTex u, const uint8_t *d, uint32_t lw, uint32_t lh, uint32_t ph) {
    uint32_t room = max_payload() - (uint32_t)sizeof u, total = u.rowbytes * u.rows;
    if (total <= room || u.rowbytes > room) { put(X_UPLOAD_TEX, &u, sizeof u, d, total); return; }
    if (!u.w) { u.x = 0; u.y = 0; u.w = lw; u.h = lh; }
    uint32_t k = room / u.rowbytes, y1 = u.y + u.h;
    for (uint32_t r = 0; r < u.rows; r += k) {
      XUpTex b = u; b.rows = u.rows - r < k ? u.rows - r : k;
      b.y = u.y + r * ph; b.h = (y1 - b.y < b.rows * ph) ? y1 - b.y : b.rows * ph;
      put(X_UPLOAD_TEX, &b, sizeof b, d + (size_t)r * u.rowbytes, b.rows * u.rowbytes);
    }
  }
  void upload_buf(uint32_t id, uint32_t off, const uint8_t *d, uint32_t len) {
    uint32_t room = max_payload() - (uint32_t)sizeof(XUpBuf);
    for (uint32_t o = 0; o < len; o += room) {
      XUpBuf u = {id, off + o, len - o < room ? len - o : room};
      put(X_UPLOAD_BUF, &u, sizeof u, d + o, u.length);
    }
  }
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
static_assert(sizeof(RingHdr) == 64, "the ring header's layout (shared by x86 and ARM64 processes)");
#define RING_MAGIC 0x52384438u                    /* "8D8R" */
#define RING_VERSION 4u                           /* 2: VSKey.vfog (the draw packet grew); 3: X_OPTION; 4: X_SNAP and
                                                     the snapshot table after the reply area */
// the executor's side: run records until X_QUIT (remote) or quit is set; `wake` is signalled when data comes,
// `space` / `replied` signalled by the reader
struct Exec;
void ring_consume(RingHdr *h, uint8_t *data, uint8_t *reply, Exec *x, HANDLE wake, HANDLE space, HANDLE replied, HANDLE parent);
