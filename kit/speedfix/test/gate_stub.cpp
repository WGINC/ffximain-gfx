// gate_stub.cpp -- for `fork.py gate-hook` (test_speedfix.py): sf_game.cpp's hooks (Occlusion_QueryScreenRect,
// Shadow_DrawByMode, CXiActor::NameCalc, and sound_retry's Unit10036B90_SetVolumeVector and CYySoundElem::UpdateTimerAndPosition) gated on their own. The rest of the mod uses Windows and Direct3D, which the gate does not provide; this stands in for it.
#include "../speedfix.h"
SfConfig sf_cfg;                                   // every switch off: the hook must be the game's function
volatile int sf_frame_no, sf_hooks_frame = -1;
extern "C" { volatile long sf_aq_skipped, sf_aq_run; }
extern "C" int sf_init(const XikitHost *h) { int a = sf_hook_query(h); int b = sf_hook_shadow(h); int c = sf_hook_name(h); int e = sf_hook_sound(h); return a ? a : b ? b : c ? c : e; }   // the hooks, switches off
extern "C" void sf_frame(void) {}
