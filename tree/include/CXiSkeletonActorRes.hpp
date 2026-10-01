// CXiSkeletonActorRes -- graphics members reconstructed from Hex-Rays output, gate-verified.
// Field names are offsets (field_XXX) unless noted; sizes come from the class descriptor.
#pragma once
#include "gfx_common.hpp"
#pragma pack(push, 1)
class XI_API CXiSkeletonActorRes {
public:
    _DWORD * ScalarDeletingDtor(char a2);  // 0x100D7000
    _DWORD * Constructor(int a2, int a3);  // 0x100D7020
    char Destructor();  // 0x100D7050
    int CXiSkeletonActorRes_GetLinkedActor();  // 0x100D7090
    char ProcessLoadState();  // 0x100D70A0

    void **vftable;  // +0x0
    uint8_t _pad_4[0x30];
    void * pLinkedActor;  // +0x34
    void * pfnOnLoaded;  // +0x38
    int loadState;  // +0x3C
    uint32_t field_40;  // +0x40
    int resourceId;  // +0x44
};
#pragma pack(pop)
static_assert(sizeof(CXiSkeletonActorRes) == 0x48, "CXiSkeletonActorRes: sizeof");
static_assert(XI_OFFSETOF(CXiSkeletonActorRes, pLinkedActor) == 0x34, "CXiSkeletonActorRes: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActorRes, pfnOnLoaded) == 0x38, "CXiSkeletonActorRes: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActorRes, loadState) == 0x3C, "CXiSkeletonActorRes: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActorRes, field_40) == 0x40, "CXiSkeletonActorRes: layout");
static_assert(XI_OFFSETOF(CXiSkeletonActorRes, resourceId) == 0x44, "CXiSkeletonActorRes: layout");
