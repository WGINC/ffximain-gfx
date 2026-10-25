# Batch r3_045_unit_10037AC0_SetMusicTrackVolume: 2 functions of `unit_10037AC0_SetMusicTrackVolume`

Name these functions: see `batches/README.md`. Write `data/names/r3_045_unit_10037AC0_SetMusicTrackVolume.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10037DD0` fn_10037DD0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit10036B90_SetVolumeScalar` (medium), `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `Target_Select` (medium), `sub_101587F0`
- **Writes globals:** `0x104637E0`
- **Reads globals:** `0x10351700`, `0x104637E0`, `0x1047D128`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `fn_10037DD0`
- **Named callers' callers:** `Target_SelectByIndex`, `Target_IsUsable`
- **Named callees' callees:** `CMoResource_FindWorker`
- **How its callers call it** (their verified code):
  - in `Target_Select` (medium): `orig::FUN_10037dd0();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10036B90_SetVolumeScalar` (medium): Sets a music track's volume from one value through the vector form 0x10036F50.
  - `Target_Select` (medium): Makes an actor the current target, moving the selection flag from the previous target.

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = orig::CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl fn_10037DD0(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_104637e0 == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,0x30313030);
    DAT_104637e0 = (undefined4 *)*(int *)uVar1;
    if (DAT_104637e0 == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_104637e0;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::Unit10036B90_SetVolumeScalar(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x10037FB0` fn_10037FB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit10036B90_SetVolumeScalar` (medium), `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `Unit101D0910_TriggerOnSceneState60` (low)
- **Writes globals:** `0x104627C8`
- **Reads globals:** `0x10351700`, `0x104627C8`, `0x1047D128`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_101D0C70` → `Unit101D0910_TriggerOnSceneState60` → `fn_10037FB0`
- **Named callees' callees:** `CMoResource_FindWorker`
- **How its callers call it** (their verified code):
  - in `Unit101D0910_TriggerOnSceneState60` (low): `uVar2 = orig::FUN_10037fb0();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10036B90_SetVolumeScalar` (medium): Sets a music track's volume from one value through the vector form 0x10036F50.
  - `Unit101D0910_TriggerOnSceneState60` (low): Starts an action and returns true when the scene state is 0x60 and a menu condition holds.

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = orig::CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl fn_10037FB0(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_104627c8 == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,0x36313030);
    DAT_104627c8 = (undefined4 *)*(int *)uVar1;
    if (DAT_104627c8 == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_104627c8;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::Unit10036B90_SetVolumeScalar(self_,0x40,(int)lVar2);
  }
  return;
}
```
