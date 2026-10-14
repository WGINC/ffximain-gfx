/* fcap_fmt.h -- one frame of the game's Direct3D 8 calls, with every piece of data they use (docs/KIT.md, "Frame
   capture"). Written by the kit (fcap.c), read by the replayer (kit/fcap/replay.c). Little-endian 32-bit words.

   File: the header, then records { tag, payload bytes, payload (padded to 4) } until FC_END.

   A record that refers to an object (texture, buffer, surface) names it by the pointer the game used; the object's
   own record (FC_TEX ...) comes before the first record that uses it. A vertex shader is named by its handle; its
   declaration (FC_VSDECL) comes first too. FVF handles (bit 0 clear) need none.

   The device's state at the start of the frame, and the state an ApplyStateBlock leaves (D3D8 cannot read a block's
   contents), are written as ordinary Set calls (FC_CALL with the FC_SYNTH flag), so the replayer needs no state
   blocks. Calls between BeginStateBlock and EndStateBlock are recorded into the block, not applied, and are left out. */
#pragma once
#include <stdint.h>

#define FC_MAGIC   0x50414346u            /* "FCAP" */
#define FC_VERSION 1u

typedef struct {
  uint32_t magic, version;
  uint32_t game_timestamp;                /* FFXiMain.dll's TimeDateStamp (0: not the game) */
  uint32_t frame;                         /* the kit's frame number */
  uint32_t bb_format, bb_width, bb_height;   /* the back buffer */
  uint32_t ds_format, ds_width, ds_height;   /* the automatic depth-stencil surface (0: none) */
  uint32_t reserved[6];
} FcHeader;

enum {
  FC_TEX = 1,       /* id, width, height, levels, usage, format, pool; per level: bytes, data (0 bytes: not read) */
  FC_CUBE = 2,      /* id, edge, levels, usage, format, pool; per face (0..5), per level: bytes, data */
  FC_VB = 3,        /* id, length, usage, fvf, pool, have; data (length bytes) when have */
  FC_IB = 4,        /* id, length, usage, format, pool, have; data when have */
  FC_SURF = 5,      /* id, kind (FS_*), container id, level, face, format, width, height, usage, pool, multisample,
                       have; data (rows of the surface, its own pitch packed) when have */
  FC_VSDECL = 6,    /* handle, declaration tokens, function tokens; the tokens */
  FC_CALL = 10,     /* slot | flags, argument count, arguments; data blocks: count, then { bytes, data } each */
  FC_BUFDATA = 11,  /* id, offset, bytes, data: a buffer locked and written during the frame */
  FC_TEXDATA = 12,  /* id, face, level, bytes, data: a texture level after an UnlockRect during the frame */
  FC_PICTURE = 20,  /* format, width, height, bytes per pixel, pixels (rows packed): the back buffer at Present */
  FC_NOTE = 99,     /* text: something the capture could not do, said where it happened */
  FC_END = 0xFFFFFFFFu
};
#define FC_SYNTH 0x10000u                 /* FC_CALL: made by the capture (start state, a state block's effect) */

enum { FS_TEXLEVEL = 1, FS_CUBEFACE = 2, FS_BACKBUFFER = 3, FS_AUTODEPTH = 4, FS_RENDERTARGET = 5,
       FS_DEPTHSTENCIL = 6, FS_IMAGE = 7 };

/* IDirect3DDevice8 vtable slots (d3d8.h order), as d3dprof numbers them */
enum {
  FS_PRESENT = 15, FS_GETBACKBUFFER = 16, FS_COPYRECTS = 28, FS_UPDATETEXTURE = 29, FS_SETRT = 31,
  FS_BEGINSCENE = 34, FS_ENDSCENE = 35, FS_CLEAR = 36, FS_SETTRANSFORM = 37, FS_MULTIPLYTRANSFORM = 39,
  FS_SETVIEWPORT = 40, FS_SETMATERIAL = 42, FS_SETLIGHT = 44, FS_LIGHTENABLE = 46, FS_SETCLIPPLANE = 48,
  FS_SETRS = 50, FS_BEGINSB = 52, FS_ENDSB = 53, FS_APPLYSB = 54, FS_SETCLIPSTATUS = 58, FS_SETTEXTURE = 61,
  FS_SETTSS = 63, FS_SETPALETTE = 66, FS_SETCURPALETTE = 68, FS_DP = 70, FS_DIP = 71, FS_DPUP = 72, FS_DIPUP = 73,
  FS_PROCESSVERTICES = 74, FS_CREATEVS = 75, FS_SETVS = 76, FS_DELETEVS = 78, FS_SETVSCONST = 79, FS_SSS = 83,
  FS_SETINDICES = 85, FS_CREATEPS = 87, FS_SETPS = 88, FS_SETPSCONST = 91
};

static inline uint32_t fc_prim_verts(uint32_t type, uint32_t n) {   /* D3DPRIMITIVETYPE: POINTLIST 1 .. TRIANGLEFAN 6 */
  switch (type) { case 1: return n; case 2: return 2 * n; case 3: return n + 1; case 4: return 3 * n; case 5: case 6: return n + 2; }
  return 0;
}

/* the data a call reads through its pointer arguments (d3d8.h prototypes): up to 2 blocks; a = the arguments after
   `this`. Returns the number of blocks; p[i], n[i] their address and size. */
static inline int fc_call_data(int slot, const uint32_t *a, const void **p, uint32_t *n) {
  switch (slot) {
  case FS_SETTRANSFORM: case FS_MULTIPLYTRANSFORM: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 64; return a[1] ? 1 : 0;
  case FS_SETVIEWPORT: p[0] = (const void *)(uintptr_t)a[0]; n[0] = 24; return a[0] ? 1 : 0;
  case FS_SETMATERIAL: p[0] = (const void *)(uintptr_t)a[0]; n[0] = 68; return a[0] ? 1 : 0;
  case FS_SETLIGHT: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 104; return a[1] ? 1 : 0;
  case FS_SETCLIPPLANE: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 16; return a[1] ? 1 : 0;
  case FS_SETCLIPSTATUS: p[0] = (const void *)(uintptr_t)a[0]; n[0] = 24; return a[0] ? 1 : 0;
  case FS_CLEAR: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 16 * a[0]; return a[1] && a[0] ? 1 : 0;   /* Count, pRects */
  case FS_COPYRECTS:                                     /* src, pSrcRects, cRects, dst, pDstPoints */
    p[0] = (const void *)(uintptr_t)a[1]; n[0] = a[1] ? 16 * a[2] : 0;
    p[1] = (const void *)(uintptr_t)a[4]; n[1] = a[4] ? 8 * a[2] : 0;
    return 2;
  case FS_SETPALETTE: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 1024; return a[1] ? 1 : 0;
  case FS_SETVSCONST: case FS_SETPSCONST: p[0] = (const void *)(uintptr_t)a[1]; n[0] = 16 * a[2]; return a[1] ? 1 : 0;
  case FS_DPUP: p[0] = (const void *)(uintptr_t)a[2]; n[0] = fc_prim_verts(a[0], a[1]) * a[3]; return 1;   /* type, count, data, stride */
  case FS_DIPUP:                                         /* type, min, num, count, idx, fmt, data, stride */
    p[0] = (const void *)(uintptr_t)a[4]; n[0] = fc_prim_verts(a[0], a[3]) * (a[5] == 102 /* D3DFMT_INDEX32 */ ? 4 : 2);
    p[1] = (const void *)(uintptr_t)a[6]; n[1] = (a[1] + a[2]) * a[7];
    return 2;
  }
  return 0;
}
