# Batch r2_153_unit_10037AC0_SetMusicTrackVolume: 6 functions of `unit_10037AC0_SetMusicTrackVolume`

Name these functions: see `batches/README.md`. Write `data/names/r2_153_unit_10037AC0_SetMusicTrackVolume.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10037D70` fn_10037D70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `Target_Select` (medium), `fn_10158DB0`, `sub_1024C710`
- **Writes globals:** `0x1046279C`
- **Reads globals:** `0x10351700`, `0x1046279C`, `0x1047D128`
- **Named before** (a naming session's reading, low): `MusicTrack_SetVolumeId0009`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `sub_100016C0` → `Input_HandleChar` → `sub_10080DC0` → `preprocessArg` → `fn_10158DB0` → `MusicTrack_SetVolumeId0009`
- **Named callers' callers:** `preprocessArg`, `Target_SelectByIndex`
- **Named callees' callees:** `CMoResource_FindWorker`
- **How its callers call it** (their verified code):
  - in `Target_Select` (medium): `MusicTrack_SetVolumeId0009();`
  - in `fn_10158DB0`: `uVar7 = MusicTrack_SetVolumeId0009();`
  - in `fn_10158DB0`: `uVar7 = MusicTrack_SetVolumeId0009();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Target_Select` (medium): Makes an actor the current target, moving the selection flag from the previous target.

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl MusicTrack_SetVolumeId0009(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_1046279c == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x39303030);
    DAT_1046279c = (undefined4 *)*(int *)uVar1;
    if (DAT_1046279c == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_1046279c;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x10037E30` fn_10037E30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `fn_101585F0`, `fn_10158DB0`
- **Writes globals:** `0x104627CC`
- **Reads globals:** `0x10351700`, `0x104627CC`, `0x1047D128`
- **Named before** (a naming session's reading, low): `MusicTrack_SetVolumeId0011`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `sub_100016C0` → `Input_HandleChar` → `sub_10080DC0` → `preprocessArg` → `fn_10158DB0` → `MusicTrack_SetVolumeId0011`
- **Named callers' callers:** `preprocessArg`, `Target_Update`
- **Named callees' callees:** `CMoResource_FindWorker`
- **How its callers call it** (their verified code):
  - in `fn_101585F0`: `uVar2 = MusicTrack_SetVolumeId0011();`
  - in `fn_101585F0`: `uVar2 = MusicTrack_SetVolumeId0011();`
  - in `fn_10158DB0`: `iVar5 = MusicTrack_SetVolumeId0011();`

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl MusicTrack_SetVolumeId0011(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_104627cc == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x31313030);
    DAT_104627cc = (undefined4 *)*(int *)uVar1;
    if (DAT_104627cc == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_104627cc;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x10037E90` fn_10037E90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `sub_101306F0`, `sub_10130E40`, `sub_101322B0`, `sub_10134250`, `sub_10135270`, `sub_10135BD0`, `sub_10139250`, `sub_1013E880`, `sub_1013F2E0`, `sub_101431B0`, `sub_10143CD0`, `sub_101515E0`, `fn_10157E20`, `sub_101A4950`, `sub_101A5010`, `sub_101A5690`, `sub_101A6180`, `sub_101A6350`, `sub_101DB690`, `sub_101DFA50` and 18 more
- **Writes globals:** `0x10462690`
- **Reads globals:** `0x10351700`, `0x10462690`, `0x1047D128`
- **Named before** (a naming session's reading, low): `MusicTrack_SetVolumeId0013`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_10157E20` → `MusicTrack_SetVolumeId0013`
- **Named callers' callers:** `Target_Update`
- **Named callees' callees:** `CMoResource_FindWorker`

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl MusicTrack_SetVolumeId0013(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_10462690 == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x33313030);
    DAT_10462690 = (undefined4 *)*(int *)uVar1;
    if (DAT_10462690 == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_10462690;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x100380F0` fn_100380F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10028FD0`, `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10074740`, `CMoTask::ResetFields`, `sub_10311C2C`
- **Called by:** `sub_10127C00`
- **Writes globals:** `0x10463938`, `0x1047D13C`
- **Reads globals:** `0x10351710`, `0x10463938`, `0x1047D128`
- **Named before** (a naming session's reading, medium): `Unit10037AC0_Create`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`, `g_resourcesDirty`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `sub_100016C0` → `Input_HandleChar` → `sub_10080DC0` → `ChatLog_AddFormattedMessage` → `sub_10127C00` → `Unit10037AC0_Create`
- **Named callers' callers:** `ChatLog_AddFormattedMessage`, `CXiControlActor_UseMagic`, `CXiSkeletonActor_SetAttack`
- **Named callees' callees:** `_exit_0`, `MemBlock_AllocFromFront`, `MemBlock_AllocFromBack`, `CMoResource_FindWorker`

Short callee `CMoResource::FindByTypeAndId`:
```cpp
uint64_t __thiscall CMoResource::FindByTypeAndId(void * self, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  uVar1 = CMoResource_FindWorker(self,param_1,param_2,param_3,g_resourceFindDepth);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(param_1)));
}
```

```cpp
XI_API void __cdecl Unit10037AC0_Create(uint32_t param_1, undefined4 param_2, undefined4 param_3)
{
  void *self_;
  undefined4 *this_00;
  uint64_t uVar1;
  int64_t lVar2;
  
  if (DAT_10463938 == (undefined4 *)0x0) {
    uVar1 = orig::Mem_AllocFromArea_v1(0x40,4);
    this_00 = (undefined4 *)uVar1;
    orig::CMoTask_ResetFields_v1(this_00);
    g_resourcesDirty_as_v0 = 1;
    if (this_00 != (undefined4 *)0x0) {
      orig::CMoTask_Constructor_v1(this_00);
      *this_00 = &PTR_CMoTask_GetRuntimeClass_variant12_1032a9bc;
      DAT_10463938 = this_00;
      uVar1 = orig::CMoResource_FindByTypeAndId_v1
                        ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),&param_1,0x3d,
                         (param_1 & 0xff00 | param_1 << 0x10 | param_1 >> 0x10 & 0xff) << 8 |
                         param_1 >> 0x18);
      if (((undefined4 *)*(int *)uVar1 != (undefined4 *)0x0) &&
         (self_ = *(void **)*(int *)uVar1, self_ != (void *)0x0)) {
        lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351710 * (long double)param_2 * (long double)127.0f);
        uVar1 = orig::FUN_10036f00(self_,0x40,(int)lVar2);
        this_00[0xe] = (int)uVar1;
        this_00[0xd] = 0x42f00000;
        *(uint8_t *)(this_00 + 0xf) = (uint8_t)param_3;
        return;
      }
      this_00[0xd] = 0;
      *(uint8_t *)(this_00 + 0xf) = (uint8_t)param_3;
    }
  }
  return;
}
```

## `0x100382A0` fn_100382A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102465E0`
- **Called by:** `sub_10037400`, `sub_100378B0`
- **Reads globals:** `0x1046393C`
- **Strings its callees refer to:** `%s\music%3.3u.bgw`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_100378B0` → `fn_100382A0`
- **Named callees' callees:** `operator_delete`, `Allocator_Generic`

```cpp
XI_API void __attribute__((cdecl, regparm(1))) fn_100382A0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2)
{
  if (DAT_1046393c != 0) {
    orig::FUN_102465e0(param_1,param_2,0);
  }
  return;
}
```

## `0x10038470` fn_10038470

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `LobbyThread_SendCmd4` (low)
- **Called by:** `fn_10035FB0`, `fn_100360F0`, `fn_100364E0`, `sub_10036C10`
- **Reads globals:** `0x1046393C`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `fn_10038470`
- **How its callers call it** (their verified code):
  - in `fn_10035FB0`: `uVar6 = orig::FUN_10038470(DAT_10463910_as_s32,(int)lVar5,uVar6,DAT_10463910_as_s32);`
  - in `fn_100360F0`: `uVar2 = orig::FUN_10038470(DAT_10463920,(int)lVar5,uVar2,DAT_10463920);`
  - in `fn_100364E0`: `uVar4 = orig::FUN_10038470(*(int *)((int)self_ + 0x1e0),local_20,0,local_20);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_SendCmd4` (low): Sends command 4 to the lobby thread and returns its result.

Short callee `LobbyThread_SendCmd4` (low):
```cpp
XI_API undefined4 __stdcall LobbyThread_SendCmd4(undefined4 param_1, uint32_t param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  uVar1 = orig::FUN_1024c060(4,param_1,0,param_2 & 0x7f,param_3,0,0,0);
  return (int)uVar1;
}
```

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10038470(undefined4 xi_eax, int param_1, int param_2, undefined4 param_3)
{
  if (-1 < param_1) {
    if (param_2 < 0) {
      xi_eax = 0;
    }
    else {
      xi_eax = param_2;
      if (0x7f < param_2) {
        xi_eax = 0x7f;
      }
    }
    if (DAT_1046393c != 0) {
      xi_eax = LobbyThread_SendCmd4(param_1,xi_eax,param_3);
    }
  }
  return xi_eax;
}
```
