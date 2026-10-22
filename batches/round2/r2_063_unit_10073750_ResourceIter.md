# Batch r2_063_unit_10073750_ResourceIter: 4 functions of `unit_10073750_ResourceIter`

Name these functions: see `batches/README.md`. Write `data/names/r2_063_unit_10073750_ResourceIter.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x10073B80` fn_10073B80

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100720B0`
- **Called by:** `CMoResource_Destroy`, `sub_10072C90`
- **Named before** (a naming session's reading, low): `CMoResource_InitAndGetFlag`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `Menu_ReloadResource` → `ResHandle_FreeIfUnreferenced` → `MoRes_FreeIfUnreferenced` → `CMoResource_Destroy` → `CMoResource_InitAndGetFlag`
- **Named callers' callers:** `CMoResource::Destructor`, `MoRes_FreeIfUnreferenced`
- **Named callees' callees:** `MoRes_FindPrevious`
- **How its callers call it** (their verified code):
  - in `CMoResource_Destroy`: `uVar2 = CMoResource_InitAndGetFlag(self_);`

```cpp
XI_API undefined4 __stdcall CMoResource_InitAndGetFlag(int * param_1)
{
  uint32_t uVar1;
  
  orig::FUN_100720b0(param_1);
  uVar1 = ((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x24))))((void *)(param_1));
  return uVar1;
}
```

## `0x100741E0` fn_100741E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10070FC0`, `ResourceHandle_IsResident`, `CMoResource::ReleaseChain`
- **Named before** (a naming session's reading, low): `ResourceIter_InPoolRange`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `Menu_ReloadResource` → `CMoResource::ReleaseChain` → `ResourceIter_InPoolRange`
- **Named callers' callers:** `App_Destructor`, `CYyMotionQue_ComputeFrameAdvance`, `CYyMotionQue_StartMotion`, `CYyModelDtBase_Destructor`, `CYyModelBase::Destructor`, `Skel_GetBoneTableBase`, `CMoElemFamily_SharedCleanup`, `CMoGeneratorClone::UnlinkElemListA`, `YmGenerater_Stop`, `Unit10053860_StopGenerators`, `CMoSchedularTask::Destructor`, `CMoSchedularTask_Tick` and 21 more
- **How its callers call it** (their verified code):
  - in `ResourceHandle_IsResident`: `uVar1 = ResourceIter_InPoolRange(g_pMeshResourceMng_as_v1,*(uint32_t **)self_);`
  - in `CMoResource::ReleaseChain`: `uVar4 = ResourceIter_InPoolRange(g_pMeshResourceMng_as_v1,*(uint32_t **)((int)self + 0x14));`
  - in `CMoResource::ReleaseChain`: `(uVar4 = ResourceIter_InPoolRange(g_pMeshResourceMng_as_v1,*(uint32_t **)((int)self + 0x14)),`

```cpp
XI_API undefined4 __thiscall ResourceIter_InPoolRange(void * self_, undefined4 * param_1)
{
  undefined4 *puVar1;
  
  if (((undefined4 *)((int)self_ + 8U) <= param_1) &&
     (param_1 <= (undefined4 *)((int)self_ + 0x40004U))) {
    if (param_1 == (undefined4 *)0x0) {
      puVar1 = (undefined4 *)0x0;
    }
    else {
      puVar1 = (undefined4 *)*param_1;
    }
    if ((undefined4 *)((int)self_ + 8U) <= puVar1) {
      if (param_1 == (undefined4 *)0x0) {
        param_1 = (undefined4 *)0x0;
      }
      else {
        param_1 = (undefined4 *)*param_1;
      }
      if (param_1 <= (undefined4 *)((int)self_ + 0x40004U)) goto LAB_10074218;
    }
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)param_1 >> 8)) << 8) | (uint8_t)(1)));
  }
LAB_10074218:
  return (uint32_t)param_1 & 0xffffff00;
}
```

## `0x10074280` fn_10074280

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit10072A50_Step` (low), `sub_10072FD0`, `sub_100731B0`, `sub_100732F0`
- **Writes globals:** `0x1047D114`
- **Named before** (a naming session's reading, low): `ResourceIter_SetCurrentGlobal`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `sub_10072FD0` → `ResourceIter_SetCurrentGlobal`
- **Named callers' callers:** `App_InitScene`, `Frame_Render`, `OpeningSubtitle_Constructor`, `CMoSchedularTask_Interpret`, `AfterimageTask_Execute`, `MeshResourceMng_LoadSystemResources`, `CXiSkeletonActor_LoadSkeletonSync`, `GameSystems_InitializeAll`, `ZoneMap_UpdateLoadState`
- **How its callers call it** (their verified code):
  - in `Unit10072A50_Step` (low): `ResourceIter_SetCurrentGlobal(*(uint32_t *)((int)self_ + 0x34));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10072A50_Step` (low): Advances a mesh load task through its file reads.

```cpp
XI_API void __cdecl ResourceIter_SetCurrentGlobal(undefined4 param_1)
{
  DAT_1047d114 = param_1;
  return;
}
```

## `0x100743D0` fn_100743D0

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoResource::GetParent` x2
- **Called by:** `ResourceIter_FindFirst`, `ResourceIter_Start` (medium), `sub_100745B0`, `ResourceIter_FindNext`
- **Reads globals:** `0x1047D130`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_resourceFindDepth`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `Unit10163E60_InitGlobalTables` → `ResourceIter_Start` → `fn_100743D0`
- **Named callers' callers:** `Unit10053860_StopGenerators`, `CXiActor_DrawResourceShadows`, `CXiSkeletonActor::SetMotion`, `CXiSkeletonActor_ReverseFindRes`, `CXiSkeletonActor_ActorFindResource`, `CXiSkeletonActor_ActorFindResourceNth`, `CXiSkeletonActor_ActorGetResourceCount`, `CXiSkeletonActor_SetModel`, `CXiSkeletonActor_AddResourceParts`, `Unit10163E60_InitGlobalTables`, `WeatherRes_FindAtOrBefore`, `WeatherRes_FindNeighbor` and 2 more
- **How its callers call it** (their verified code):
  - in `ResourceIter_FindFirst`: `orig::FUN_100743d0(self_);`
  - in `ResourceIter_Start` (medium): `orig::FUN_100743d0(self_);`
  - in `ResourceIter_FindNext`: `orig::FUN_100743d0(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `ResourceIter_Start` (medium): Starts a resource iteration for a type and id and advances it to the first match.

```cpp
XI_API void __thiscall fn_100743D0(void * self_)
{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  int iVar4;
  
  iVar2 = g_resourceFindDepth;
  iVar4 = *(int *)((int)self_ + 0x14);
  if (*(int *)self_ == 0) {
    if (iVar4 != 0) {
      piVar1 = *(int **)(iVar4 + 0x10);
      if (*(uint32_t *)((int)self_ + 0xc) != 0) {
        if (piVar1 == (int *)0x0) {
          iVar4 = 0;
        }
        else {
          iVar4 = *piVar1;
        }
        *(int *)((int)self_ + 0x14) = iVar4;
        if (iVar4 != 0) {
          do {
            if ((*(uint32_t *)(*(int *)((int)self_ + 0x14) + 0x20) & *(uint32_t *)((int)self_ + 0x10)) ==
                *(uint32_t *)((int)self_ + 0xc)) {
              return;
            }
            piVar1 = *(int **)(*(int *)((int)self_ + 0x14) + 0x10);
            if (piVar1 == (int *)0x0) {
              iVar4 = 0;
            }
            else {
              iVar4 = *piVar1;
            }
            *(int *)((int)self_ + 0x14) = iVar4;
          } while (iVar4 != 0);
        }
        *(uint32_t *)((int)self_ + 0x14) = 0;
        return;
      }
      if (piVar1 != (int *)0x0) {
        *(int *)((int)self_ + 0x14) = *piVar1;
        return;
      }
      *(uint32_t *)((int)self_ + 0x14) = 0;
    }
    return;
  }
  if (iVar4 != 0) {
    piVar1 = *(int **)(iVar4 + 8);
    if (*(int *)((int)self_ + 0xc) == 0) {
      if (piVar1 == (int *)0x0) {
        iVar4 = 0;
      }
      else {
        iVar4 = *piVar1;
      }
      *(int *)((int)self_ + 0x14) = iVar4;
      while (iVar4 != 0) {
        iVar4 = *(int *)((int)self_ + 0x14);
        if ((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == 1) {
          if (*(int *)(iVar4 + 0x38) != iVar2) {
            *(int *)(iVar4 + 0x38) = iVar2;
            *(int *)((int)self_ + 0x18) = *(int *)((int)self_ + 0x18) + 1;
            goto LAB_100744f8;
          }
          if (*(uint32_t **)(iVar4 + 0x3c) == (uint32_t *)0x0) {
            *(uint32_t *)((int)self_ + 0x14) = 0;
          }
          else {
            *(uint32_t *)((int)self_ + 0x14) = **(uint32_t **)(iVar4 + 0x3c);
          }
        }
        else {
LAB_100744f8:
          if ((*(uint8_t *)((int)*(void **)((int)self_ + 0x14) + 0x24) & 0x7f) == 0) {
            pvVar3 = orig::CMoResource_GetParent_v1(*(void **)((int)self_ + 0x14));
            *(int *)((int)pvVar3 + 0x38) = iVar2;
            iVar4 = *(int *)((int)self_ + 0x18) + -1;
            *(int *)((int)self_ + 0x18) = iVar4;
            if (iVar4 < 0) break;
          }
        }
        if ((*(uint32_t *)(*(int *)((int)self_ + 0x14) + 0x24) & 0x7f) == *(uint32_t *)((int)self_ + 8)) {
          return;
        }
        piVar1 = *(int **)(*(int *)((int)self_ + 0x14) + 8);
        if (piVar1 == (int *)0x0) {
          iVar4 = 0;
        }
        else {
          iVar4 = *piVar1;
        }
        *(int *)((int)self_ + 0x14) = iVar4;
      }
    }
    else {
      if (piVar1 == (int *)0x0) {
        iVar4 = 0;
      }
      else {
        iVar4 = *piVar1;
      }
      *(int *)((int)self_ + 0x14) = iVar4;
      if (iVar4 != 0) {
        do {
          iVar4 = *(int *)((int)self_ + 0x14);
          if ((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == 1) {
            if (*(int *)(iVar4 + 0x38) != iVar2) {
              *(int *)(iVar4 + 0x38) = iVar2;
              *(int *)((int)self_ + 0x18) = *(int *)((int)self_ + 0x18) + 1;
              goto LAB_1007444e;
            }
            if (*(uint32_t **)(iVar4 + 0x3c) == (uint32_t *)0x0) {
              *(uint32_t *)((int)self_ + 0x14) = 0;
            }
            else {
              *(uint32_t *)((int)self_ + 0x14) = **(uint32_t **)(iVar4 + 0x3c);
            }
          }
          else {
LAB_1007444e:
            if ((*(uint8_t *)((int)*(void **)((int)self_ + 0x14) + 0x24) & 0x7f) == 0) {
              pvVar3 = orig::CMoResource_GetParent_v1(*(void **)((int)self_ + 0x14));
              *(int *)((int)pvVar3 + 0x38) = iVar2;
              iVar4 = *(int *)((int)self_ + 0x18) + -1;
              *(int *)((int)self_ + 0x18) = iVar4;
              if (iVar4 < 0) break;
            }
          }
          iVar4 = *(int *)((int)self_ + 0x14);
          if (((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == *(uint32_t *)((int)self_ + 8)) &&
             ((*(uint32_t *)(iVar4 + 0x20) & *(uint32_t *)((int)self_ + 0x10)) == *(uint32_t *)((int)self_ + 0xc))
             ) {
            return;
          }
          if (*(int **)(iVar4 + 8) == (int *)0x0) {
            iVar4 = 0;
          }
          else {
            iVar4 = **(int **)(iVar4 + 8);
          }
          *(int *)((int)self_ + 0x14) = iVar4;
          if (iVar4 == 0) {
            *(uint32_t *)((int)self_ + 0x14) = 0;
            return;
          }
        } while( true );
      }
    }
  }
  *(uint32_t *)((int)self_ + 0x14) = 0;
  return;
}
```
