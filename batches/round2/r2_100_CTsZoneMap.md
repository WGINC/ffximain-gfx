# Batch r2_100_CTsZoneMap: 4 functions of `CTsZoneMap`

Name these functions: see `batches/README.md`. Write `data/names/r2_100_CTsZoneMap.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101773F0` fn_101773F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `YmGenerater_BuildAttachMatrix` (medium)
- **Named before** (a naming session's reading, low): `ZoneMap_GetMatrixGlobal`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `YmGenerater_UpdateAttach` → `YmGenerater_BuildAttachMatrix` → `ZoneMap_GetMatrixGlobal`
- **Named callers' callers:** `YmGenerater_UpdateAttach`
- **How its callers call it** (their verified code):
  - in `YmGenerater_BuildAttachMatrix` (medium): `qmemcpy(a2, ZoneMap_GetMatrixGlobal(), 0x40u);`
  - in `YmGenerater_BuildAttachMatrix` (medium): `qmemcpy(a2, ZoneMap_GetMatrixGlobal(), 0x40u);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `YmGenerater_BuildAttachMatrix` (medium): 

```cpp
XI_API undefined4 * __cdecl ZoneMap_GetMatrixGlobal(void)
{
  return &DAT_10631ed0;
}
```

## `0x10177770` fn_10177770

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101819B0`
- **Called by:** `Unit1012AC80_ApplyOffset` (low)
- **Reads globals:** `0x10631F24`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pZoneMap`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Target_Update` → `fn_101585F0` → `fn_10158B40` → `fn_10158B00` → `sub_10159BA0` → `Unit1012AC80_ApplyOffset` → `fn_10177770`
- **Named callers' callers:** `Unit1012AC80_MoveByDirection`
- **Named callees' callees:** `ZoneMap_QueryBoxHeight`
- **How its callers call it** (their verified code):
  - in `Unit1012AC80_ApplyOffset` (low): `uVar4 = orig::FUN_10177770(*(uint32_t *)((int)self_ + 0x10),*(uint32_t *)((int)self_ + 0x14),xi_eax,`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit1012AC80_ApplyOffset` (low): Applies a scaled offset to the tracked position.

Short callee `fn_101819B0`:
```cpp
XI_API undefined4 __thiscall fn_101819B0(void * self_, undefined4 param_1, float param_2, undefined4 param_3, float * param_4)
{
  uint32_t uVar1;
  
  uVar1 = ZoneMap_QueryBoxHeight(self_,param_1,
                       (float)((long double)param_2 - (long double)*(float *)((int)self_ + 0x170)),param_3,
                       19.0f,param_4);
  return CONCAT31((int32_t)((uint32_t)uVar1 >> 8),(char)uVar1 != '\0');
}
```

```cpp
XI_API uint32_t __attribute__((cdecl, regparm(1))) fn_10177770(undefined4 xi_eax, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  uint32_t uVar1;
  
  if (g_pZoneMap != (void *)0x0) {
    uVar1 = orig::FUN_101819b0(g_pZoneMap,param_1,param_2,param_3,param_4);
    return uVar1;
  }
  return xi_eax & 0xffffff00;
}
```

## `0x101779B0` fn_101779B0

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10178900`, `ZoneMap_SetArea` (medium)
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `ZoneMap_SetArea` → `fn_101779B0`
- **How its callers call it** (their verified code):
  - in `ZoneMap_SetArea` (medium): `xi_eax = orig::FUN_101779b0(self_,local_10,param_2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `ZoneMap_SetArea` (medium): Requests a zone-map area by id, or clears the current one when the id is zero.

```cpp
XI_API undefined4 __thiscall fn_101779B0(void * self_, undefined4 * param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10173ae0(param_1,param_2);
  return uVar1;
}
```

## `0x10177FA0` fn_10177FA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CTsZoneMap_Init` (medium), `Zone_RenderEnvironment`
- **Named before** (a naming session's reading, low): `CTsZoneMap_SetLayerOrder`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `CTsZoneMap_Init` → `CTsZoneMap_SetLayerOrder`
- **Named callers' callers:** `App_InitScene`, `CTsZoneMap_Render`
- **How its callers call it** (their verified code):
  - in `CTsZoneMap_Init` (medium): `CTsZoneMap_SetLayerOrder(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CTsZoneMap_Init` (medium): Constructs the zone map task and initialises its sub-objects, state and global tables.

```cpp
XI_API void __thiscall CTsZoneMap_SetLayerOrder(void * self_)
{
  *(uint32_t *)((int)self_ + 0x394ac) = 0;
  *(uint32_t *)((int)self_ + 0x394a8) = 0;
  *(uint32_t *)((int)self_ + 0x394b0) = 1;
  *(uint32_t *)((int)self_ + 0x394b4) = 2;
  *(uint32_t *)((int)self_ + 0x394b8) = 3;
  *(uint32_t *)((int)self_ + 0x394bc) = 4;
  if (*(int *)((int)self_ + 0x394a4) != 0) {
    *(uint32_t *)((int)self_ + 0x394a8) = 0;
    *(uint32_t *)((int)self_ + 0x394ac) = 1;
    *(uint32_t *)((int)self_ + 0x394b0) = 2;
    *(uint32_t *)((int)self_ + 0x394b4) = 3;
    *(uint32_t *)((int)self_ + 0x394b8) = 4;
    *(uint32_t *)((int)self_ + 0x394bc) = 5;
  }
  return;
}
```
