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
  int task_draws;
  int lim_limbs;            // Liminality: the characters' animation frames and fade weights blended between ticks too (sf_main.cpp)
  int liminality;           // Liminality stage 1: whole ticks only, and on frames of 0 ticks only the drawing tasks run (sf_fork.cpp)           // CMoTask::Dispatch hooked (sf_fork.cpp): draw calls counted by the task (its vtable) and slot
  int stats_every;          // seconds between the statistics lines in xikit.log (60)         // at the start: frame statistics split by camera still / moving (sf_device.cpp)
  int skin_merge;           // CYyModelDt::SkinAndDrawV2 forked (fork/): runs of a model's triangle-list records drawn as one
  int sse_skinning;         // the game's SSE skinning loops on any CPU that reports SSE (sf_main.cpp, apply_sse_skinning)
  int block_cache;          // applying a state block recorded on the render thread sets what it holds in the shadow
                            // instead of forgetting everything (sf_device.cpp)
  int precise_pacing;       // the game's frame limiter timed with the performance counter and waited on precisely
                            // (sf_main.cpp): 60.0 fps instead of 58.8
  int null_draws;           // a test: the draw calls return D3D_OK without reaching the device (sf_device.cpp)
  int gpu_skin;             // skinning handed to d3d8on11 (D8ON11_Skin) from the fork of SkinAndDrawV2; set at the start, then switches live
  int occlusion_first;      // occlusion_late=1: a probe with no finished picture is answered without a wait: 1 hidden, 2 visible (0: the game's readback)
  int occlusion_nowait;     // occlusion_late's reads never wait when the d3d8.dll in use is d3d8on11 with D8ON11_LockDoNotWait (default 1)
  int occlusion_hold;       // occlusion_late=1: a visible object hidden only when two answers in a row say hidden (sf_occlusion.cpp)
  int occlusion_late;       // the occlusion readback answered from the query's previous probe (sf_occlusion.cpp): 1 on, 2 measure only
  int uncap;                // the game's minimum ticks a frame [g_pCamera+0x30] kept at 0: no 60 fps wait (sf_main.cpp)
  int frame_step;           // the game's time step: 0 its own, 1 real time in whole ticks, 2 and the fraction where proven
  int shadow_fit;           // (16 Nov) simple shadows (shadow mode 2) sized by each character's own model (sf_game.cpp); 0 the game's
  int name_smooth;          // (21 Nov) names sized from their unrounded depth (sf_game.cpp); 0 the game's 80 steps
  int step_exact;           // (27 Nov) frame_step's step from this frame's own time by the performance counter, within 1 ms of
                            // the game's whole-ms time (fork/Frame_Render.fork.cpp, xi_exact_ticks); 0 the game's 4-frame average
  int pace_adaptive;        // (27 Nov) each frame held to the pace_pct percentile of the last 120 frames' own time (sf_device.cpp)
  int pace_pct;             // that percentile, 50-99 (90)
  int aura_size;            // (14 Nov) the aura capture texture's size, 256 the game's (fork/Post_CreateScreenEffectTargets.fork.cpp; from SpectralFix via dagnyfix)
  int flare_fix;            // (14 Nov) lamp glows drawn 0.4 nearer along their local z (sf_device.cpp; from dagnyfix); 0 the game's
  int sphere_shine;         // (14 Nov) sphere-mapped batches at the PS2 mapping (fork/CYyModelDt__SkinAndDrawV2.fork.cpp); 0 the game's
  int sound_retry;          // (14 Nov) a looping sound refused a voice kept and asked again next update (sf_game.cpp); 0 the game deletes it
  int shadow_fit_scale;     // the shadow's half-width as hundredths of the model footprint's diagonal (default 46: sf_game.cpp says why)
};
extern SfConfig sf_cfg;

extern "C" {
// sf_main.cpp
int  sf_init(const XikitHost *host);
void sf_frame(void);
// sf_game.cpp
uintptr_t sf_game_addr(uint32_t analysed);      // an analysed address in the running game (XI_GAME, build translation)
int  sf_hook_query(const XikitHost *host);
int  sf_hook_shadow(const XikitHost *host);
int  sf_hook_sound(const XikitHost *host);        // sound_retry: Unit10036B90_SetVolumeVector and CYySoundElem::UpdateTimerAndPosition hooked
void sf_sound_take(long *req, long *kept, long *gone);   // sound_retry: restarts asking a voice; refusals answered 0 (kept); refusals left (deleted)
int  sf_hook_name(const XikitHost *host);         // name_smooth: CXiActor::NameCalc hooked
void sf_name_take(long *smooth, long *kept);       // shadow_fit: Shadow_DrawByMode hooked
void sf_shadow_take(long *n, long *g, float *dmin, float *dmax, float *dsum);   // its counts since the last call
void sf_shadow_branches(long *b);                // Shadow_DrawByMode's branches since the last call (sf_game.cpp)
int  sf_fork_skinmerge(const XikitHost *host);   // XI_FORK of CYyModelDt::SkinAndDrawV2
int  sf_fork_aura(const XikitHost *host);        // aura_size: XI_FORK of the aura capture texture's creation and capture
unsigned sf_aura_remake(void);                   // the texture remade through the fork (early frames); its width now
extern int sf_aura_installed; extern volatile unsigned xi_aura_size;
int  sf_fork_framestep(const XikitHost *host);   // XI_FORK of Frame_Render and the two step getters (frame_step, uncap)
extern volatile int sf_group_on;              // skin_merge=2: sf_device.cpp groups a model's draws
extern volatile long sf_decal_n; extern void *sf_decal_site[8]; extern long sf_decal_site_n[8]; extern float sf_decal_p11[2], sf_decal_p22[2];   // decal probe (sf_device.cpp)
extern volatile long sf_flare_n, sf_flare_unknown; extern void *sf_flare_site[8]; extern long sf_flare_site_n[8];   // flare_fix (sf_device.cpp)
extern volatile int xi_gpuskin_on; extern volatile int xi_sphere_on; extern volatile long xi_sphere_draws;   // (sphere_shine)
            // the fork's gpu skinning switch (the same file)
extern volatile long sf_gs_jobs, sf_gs_refused; extern int sf_gpuskin_api;   // sf_fork.cpp: models handed to the device, declined; D8ON11_Skin found
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
