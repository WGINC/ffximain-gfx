// gate_stub.cpp -- for `fork.py gate-hook` (test_speedfix.py): sf_game.cpp's hook of Occlusion_QueryScreenRect gated
// on its own. The rest of the mod uses Windows and Direct3D, which the gate does not provide; this stands in for it.
#include "../speedfix.h"
SfConfig sf_cfg;                                   // every switch off: the hook must be the game's function
volatile int sf_frame_no, sf_hooks_frame = -1;
extern "C" { volatile long sf_aq_skipped, sf_aq_run; }
extern "C" int sf_init(const XikitHost *h) { return sf_hook_query(h); }
extern "C" void sf_frame(void) {}
