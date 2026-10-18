// SPDX-License-Identifier: GPL-3.0-only
// speedfix.h -- the two halves of the speedfix mod: sf_device.cpp (the Direct3D 8 device layer, built against the
// DirectX 8 headers) and sf_game.cpp (the mod's entry points, the settings and the game-code changes, built against
// the reconstruction's headers). Plain types only: the two header sets declare the same D3D names differently.
#pragma once
#include <stdint.h>
#include "xikit_mod.h"

// Settings ([speedfix] in speedfix.ini next to the mod; re-read when the file changes). Written on the render
// thread (xikit_mod_frame), read by the hooks, which run there too.
struct SfConfig {
  int state_cache;          // skip SetRenderState / SetTextureStageState / SetTexture that repeat the current value
  int vs_cache;             // skip SetVertexShader that repeats the bound handle
  int ext_cache;            // also SetTransform / SetViewport / SetLight / LightEnable / SetStreamSource / SetIndices
  int scene_reset;          // forget the shadow at each BeginScene / EndScene
  int batch_up;             // merge back-to-back DrawPrimitiveUP calls
  int batch_max_prims;      // 0 = no cap beyond the device's MaxPrimitiveCount
  int batch_max_bytes;      // 0 = no cap beyond the 1 MB buffer
  int frame_cap_fps;        // 0 = off; at most 60 unless uncap (sf_main.cpp: why)
  int occlusion_stagger;    // 4 (the game's), 8, 16
  int skip_actor_occlusion;
  int skip_all_occlusion_readbacks;
  int texture_compression, texture_compression_value;
  int cpu_tier;
  int lazy_state;           // hold RS / TSS / texture changes until something reads the state (a draw); a change undone
                            // before then never reaches the device
  int vs_dedupe;            // CreateVertexShader of a declaration already made (no function): the same handle again
  int merge_scenes;         // one BeginScene/EndScene pair a frame: the game's inner EndScene..BeginScene pairs dropped
  int motion_stats;
  int stats_every;          // seconds between the statistics lines in xikit.log (60)         // at the start: frame statistics split by camera still / moving (sf_device.cpp)
  int skin_merge;           // CYyModelDt::SkinAndDrawV2 forked (fork/): runs of a model's triangle-list records drawn as one
  int sse_skinning;         // the game's SSE skinning loops on any CPU that reports SSE (sf_main.cpp, apply_sse_skinning)
  int block_cache;          // applying a state block recorded on the render thread sets what it holds in the shadow
                            // instead of forgetting everything (sf_device.cpp)
  int precise_pacing;       // the game's frame limiter timed with the performance counter and waited on precisely
                            // (sf_main.cpp): 60.0 fps instead of 58.8
  int null_draws;           // a test: the draw calls return D3D_OK without reaching the device (sf_device.cpp)
  int occlusion_late;       // the occlusion readback answered from the query's previous probe (sf_occlusion.cpp): 1 on, 2 measure only
  int uncap;                // the game's minimum ticks a frame [g_pCamera+0x30] kept at 0: no 60 fps wait (sf_main.cpp)
  int frame_step;           // the game's time step: 0 its own, 1 real time in whole ticks, 2 and the fraction where proven
};
extern SfConfig sf_cfg;

extern "C" {
// sf_main.cpp
int  sf_init(const XikitHost *host);
void sf_frame(void);
// sf_game.cpp
uintptr_t sf_game_addr(uint32_t analysed);      // an analysed address in the running game (XI_GAME, build translation)
int  sf_hook_query(const XikitHost *host);
int  sf_fork_skinmerge(const XikitHost *host);   // XI_FORK of CYyModelDt::SkinAndDrawV2
int  sf_fork_framestep(const XikitHost *host);   // XI_FORK of Frame_Render and the two step getters (frame_step, uncap)
extern volatile int xi_skinmerge_on;          // the fork's switch (fork/CYyModelDt__SkinAndDrawV2.fork.cpp)
extern int sf_skinmerge_installed;
extern int sf_framestep_installed;
// sf_occlusion.cpp
void sf_occl_init(const XikitHost *host);
int  sf_occl_readback(void *q, int mode);         // the readback answered a probe late (mode 1) or measured (2)
int  sf_occl_game(void *q, int flag);              // the game's readback 0x1006C8B0, timed
void sf_occl_reset(void);                         // before Reset: its surfaces released
void sf_occl_frame(int on);                       // once a frame: occlusion_late on (1), off (0), or every readback skipped (-1)
void sf_occl_stats(void);
// sf_device.cpp
int  sf_device_install(const XikitHost *host);    // the device hooks; only from xikit_mod_init
void sf_device_config_changed(const SfConfig *before);   // render thread: apply the cache switches safely
void sf_device_frame(void);                       // render thread, before Present: pacing
void sf_device_stats(void);                       // a minute's figures to the log
// the lever A/B (sf_main.cpp): the switches the device layer's hooks at this start allow (0: needs a restart)
int  sf_device_allows(const char *key);
}
// The kit takes mod hooks off for F9 and for a trace's unswapped frames (its frame callbacks still run). The
// game-code changes in sf_game.cpp are not the kit's hooks, so they ask whether this frame's scene went through the
// device hooks, and behave as the game's own code when it did not: an A/B then compares everything speedfix does.
extern volatile int sf_frame_no, sf_hooks_frame;
static inline bool sf_hooks_live(void) { return sf_hooks_frame == sf_frame_no; }
