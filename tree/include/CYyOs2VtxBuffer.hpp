// CYyOs2VtxBuffer -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CYyOs2VtxBuffer {
public:
    char CreateBuffersStep(int a2);  // 0x1002CCB0
    int Constructor(_DWORD **a2);  // 0x1002D680
    char CYyOs2VtxBuffer_Destructor();  // 0x1002D700

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
    uint32_t field_34;  // +0x34
    uint32_t field_38;  // +0x38
    int32_t field_3C;  // +0x3C
    uint32_t field_40;  // +0x40
    uint32_t field_44;  // +0x44
    uint32_t field_48;  // +0x48
    uint32_t field_4C;  // +0x4C
    uint32_t field_50;  // +0x50
    uint32_t field_54;  // +0x54
    int32_t field_58;  // +0x58
    int32_t field_5C;  // +0x5C
    uint32_t field_60;  // +0x60
    uint32_t field_64;  // +0x64
    uint8_t field_68;  // +0x68
    uint8_t field_69;  // +0x69
    uint32_t field_6A;  // +0x6A
    uint32_t field_6E;  // +0x6E
    uint32_t field_72;  // +0x72
    uint32_t field_76;  // +0x76
    uint32_t field_7A;  // +0x7A
    uint8_t _pad_7E[0x20];
};
#pragma pack(pop)
static_assert(sizeof(CYyOs2VtxBuffer) == 0x9E, "CYyOs2VtxBuffer: sizeof");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_34) == 0x34, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_38) == 0x38, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_3C) == 0x3C, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_40) == 0x40, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_44) == 0x44, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_48) == 0x48, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_4C) == 0x4C, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_50) == 0x50, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_54) == 0x54, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_58) == 0x58, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_5C) == 0x5C, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_60) == 0x60, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_64) == 0x64, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_68) == 0x68, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_69) == 0x69, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_6A) == 0x6A, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_6E) == 0x6E, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_72) == 0x72, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_76) == 0x76, "CYyOs2VtxBuffer: layout");
static_assert(XI_OFFSETOF(CYyOs2VtxBuffer, field_7A) == 0x7A, "CYyOs2VtxBuffer: layout");
