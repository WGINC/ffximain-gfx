# Batch r2_112_YmCombineWeather: 3 functions of `YmCombineWeather`

Name these functions: see `batches/README.md`. Write `data/names/r2_112_YmCombineWeather.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1018AAC0` fn_1018AAC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10072540`
- **Called by:** `fn_101875E0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `XiZone_SetTreeState1BC` → `fn_101875E0` → `fn_1018AAC0`
- **Named callers' callers:** `XiZone_SetTreeState1BC`
- **How its callers call it** (their verified code):
  - in `fn_101875E0`: `xi_eax = orig::FUN_1018aac0(this_00,xi_eax);`

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AAC0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10072540(*(void **)((int)self_ + 0xc),5,0,&LAB_1018aae0,0,xi_eax);
  return uVar1;
}
```

## `0x1018AB40` fn_1018AB40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10072540`
- **Called by:** `fn_101875C0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `XiZone_SetTreeState1BC` → `fn_101875C0` → `fn_1018AB40`
- **Named callers' callers:** `XiZone_SetTreeState1BC`
- **How its callers call it** (their verified code):
  - in `fn_101875C0`: `xi_eax = orig::FUN_1018ab40(this_00,xi_eax);`

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AB40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10072540(*(void **)((int)self_ + 0xc),5,0,&LAB_1018ab60,0,xi_eax);
  return uVar1;
}
```

## `0x1018ABF0` fn_1018ABF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Scheduler_RemoveTasksFor` (medium), `sub_100742A0`, `ResourceIter_FindFirst` x3, `ResourceIter_Start` (medium), `ResourceIter_FindNext` x6, `ZoneMap_SetRecord` (medium), `sub_1018ABB0` x2, `fn_1018AD50`, `fn_1018AD60`, `Blend_FindPrevious` (medium), `Blend_FreeResult` (low)
- **Called by:** `sub_101874F0`, `XiZone_DestroyChildList` (medium), `Blend_Update` (medium)
- **Reads globals:** `0x1047D128`
- **Named before** (a naming session's reading, low): `YmCombineWeather_Release`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `sub_10014390` → `ZoneMap_SetArea` → `ZoneMap_ReleaseSlot` → `ZoneMap_FreeSlot` → `ZoneMap_FreeSubAreas` → `ZoneMap_Destroy` → `XiZone_DestroyChildList` → `YmCombineWeather_Release`
- **Named callers' callers:** `CMoSchedularTask_Interpret`, `ZoneMap_ReleaseChain`, `ZoneMap_Destroy`, `XiZone_ResetState`
- **Named callees' callees:** `Camera_SetField2FC`, `Camera_SetField300`, `Camera_SetField314`, `Camera_SetField318`, `Mem_FreeObject`, `YmGenerater_Stop`, `CMoResource::ReleaseChain`, `ResourceIter_MatchCurrent`, `CMoTask_Release`, `ZoneMap_GetFloatRef1CPlus4`, `ZoneMap_GetFloatRef1CPlus8`, `ZoneMap_ApplyColors`
- **How its callers call it** (their verified code):
  - in `XiZone_DestroyChildList` (medium): `YmCombineWeather_Release(this_00);`
  - in `Blend_Update` (medium): `YmCombineWeather_Release(pvVar1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Scheduler_RemoveTasksFor` (medium): Removes every scheduler task that belongs to the given object.
  - `ResourceIter_Start` (medium): Starts a resource iteration for a type and id and advances it to the first match.
  - `ZoneMap_SetRecord` (medium): Sets the zone map's current record and applies its colours.
  - `XiZone_DestroyChildList` (medium): Destroys the zone's child and every node in its list.
  - `Blend_FindPrevious` (medium): Returns the blend before this one in the owner's list.
  - `Blend_FreeResult` (low): Frees the blend's result object when it owns it.
  - `Blend_Update` (medium): Advances a blend and blends its two sources into a new result.

Short callee `ResourceIter_FindNext`:
```cpp
XI_API undefined4 __thiscall ResourceIter_FindNext(void * self_, undefined4 xi_s1)
{
  uint32_t uVar1;
  
  uVar1 = *(uint32_t *)((int)self_ + 0x14);
  orig::FUN_100743d0(self_);
  return uVar1;
}
```

Short callee `fn_1018AD50`:
```cpp
XI_API undefined4 __thiscall fn_1018AD50(void * self_)
{
  return *(uint32_t *)((int)self_ + 4);
}
```

Short callee `fn_1018AD60`:
```cpp
XI_API void __thiscall fn_1018AD60(void * self_, undefined4 param_1)
{
  *(uint32_t *)(*(int *)((int)self_ + 4) + 0x10) = param_1;
  return;
}
```

Short callee `Blend_FreeResult` (low):
```cpp
XI_API void __thiscall Blend_FreeResult(void * self_)
{
  uint32_t *xi_s1;
  
  xi_s1 = *(uint32_t **)((int)self_ + 0x20);
  if ((xi_s1 != (uint32_t *)0x0) && ((*xi_s1 >> 2 & 1) != 0)) {
    orig::Mem_FreeObject_v1(xi_s1);
  }
  *(uint32_t *)((int)self_ + 0x20) = 0;
  return;
}
```

```cpp
XI_API void __thiscall YmCombineWeather_Release(void * self_)
{
  uint8_t bVar1;
  int iVar2;
  void *pvVar3;
  char xi_frame[28] __attribute__((aligned(4)));
  uint8_t  (&local_1c)[28] = *reinterpret_cast<uint8_t  (*)[28]>(xi_frame + 0);
  
  *(uint8_t ***)self_ = &PTR_YmCombineWeather_GetRuntimeClass_10336814;
  orig::FUN_100742a0(local_1c);
  if (*(int *)(*(int *)((int)self_ + 4) + 0x20) == *(int *)((int)self_ + 0x2c)) {
    iVar2 = orig::FUN_1018ad50(self_);
    *(uint32_t *)(iVar2 + 0x20) = 0;
  }
  bVar1 = false;
  orig::ResourceIter_FindFirst_v1(local_1c,*(int *)(*(int *)((int)self_ + 4) + 0x14),1,0,0xffffffff);
  iVar2 = orig::ResourceIter_FindNext_v1(local_1c,0);
  do {
    if (iVar2 == 0) {
LAB_1018ac60:
      do {
        ResourceIter_Start(local_1c,g_pMeshResourceMng_as_v0,5,0,0xffffffff);
        do {
          pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
          if (pvVar3 == (void *)0x0) {
            do {
              orig::ResourceIter_FindFirst_v1(local_1c,*(int *)((int)self_ + 0xc),5,0,0xffffffff);
              do {
                pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                if (pvVar3 == (void *)0x0) {
                  if (*(int *)((int)*(void **)((int)self_ + 4) + 0x18) == *(int *)((int)self_ + 0x20))
                  {
                    ZoneMap_SetRecord(*(void **)((int)self_ + 4),(uint32_t *)0x0);
                  }
                  Blend_FreeResult(self_);
                  pvVar3 = Blend_FindPrevious(self_);
                  if (pvVar3 == (void *)0x0) {
                    orig::FUN_1018ad60(self_,*(uint32_t *)((int)self_ + 8));
                  }
                  else {
                    *(uint32_t *)((int)pvVar3 + 8) = *(uint32_t *)((int)self_ + 8);
                  }
                  orig::ResourceIter_FindFirst_v1(local_1c,*(int *)((int)self_ + 0xc),7,0,0xffffffff);
                  pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                  while (pvVar3 != (void *)0x0) {
                    Scheduler_RemoveTasksFor(pvVar3);
                    pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                  }
                  return;
                }
              } while ((*(void **)((int)pvVar3 + 100) != self_) ||
                      (*(uint32_t *)((int)pvVar3 + 100) = 0, !bVar1));
              orig::FUN_1018abb0(pvVar3,pvVar3);
            } while( true );
          }
        } while ((*(void **)((int)pvVar3 + 100) != self_) ||
                (*(uint32_t *)((int)pvVar3 + 100) = 0, !bVar1));
        orig::FUN_1018abb0(pvVar3,pvVar3);
      } while( true );
    }
    if (iVar2 == *(int *)((int)self_ + 0xc)) {
      bVar1 = true;
      goto LAB_1018ac60;
    }
    iVar2 = orig::ResourceIter_FindNext_v1(local_1c,0);
  } while( true );
}
```
