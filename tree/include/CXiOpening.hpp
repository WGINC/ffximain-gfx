// CXiOpening -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiOpening {
public:
    CDx * fn_100187D0();  // 0x100187D0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x110];
    int32_t field_114;  // +0x114
    int32_t field_118;  // +0x118
    int32_t field_11C;  // +0x11C
    int32_t field_120;  // +0x120
    int32_t field_124;  // +0x124
};
#pragma pack(pop)
static_assert(sizeof(CXiOpening) == 0x128, "CXiOpening: sizeof");
static_assert(XI_OFFSETOF(CXiOpening, field_114) == 0x114, "CXiOpening: layout");
static_assert(XI_OFFSETOF(CXiOpening, field_118) == 0x118, "CXiOpening: layout");
static_assert(XI_OFFSETOF(CXiOpening, field_11C) == 0x11C, "CXiOpening: layout");
static_assert(XI_OFFSETOF(CXiOpening, field_120) == 0x120, "CXiOpening: layout");
static_assert(XI_OFFSETOF(CXiOpening, field_124) == 0x124, "CXiOpening: layout");
