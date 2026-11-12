// CApp -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Unnamed fields are field_XXX (the offset); sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CApp {
public:
    char RunMainLoop();  // 0x10001520
    static void __thiscall CopyTextToClipboard(void * self, LPCSTR param_1);  // 0x10001CC0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x19];
    uint8_t mainLoopActive;  // +0x1D set to 1 once the scene is initialised and 0 when the main loop exits
    uint8_t _pad_1E[0x26];
};
#pragma pack(pop)
static_assert(sizeof(CApp) == 0x44, "CApp: sizeof");
static_assert(XI_OFFSETOF(CApp, mainLoopActive) == 0x1D, "CApp: layout");
