# Batch r2_011_CXiSkeletonActor: 13 functions of `CXiSkeletonActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_011_CXiSkeletonActor.json`.
7 of them were sampled in a running frame (data/frame_set.json).

## `0x100A47F0` CXiSkeletonActor::vf256

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 256 (+0x400), `XiSkeletonActor2` slot 256 (+0x400), `CXiSkeletonActor` slot 256 (+0x400), `CXiMannequinActor` slot 256 (+0x400), `XiChocoboActor2` slot 256 (+0x400), `CXiGeomActor` slot 256 (+0x400), `CXiDancerActor` slot 256 (+0x400)
- **Named before** (a naming session's reading, low): `CXiSkeletonActor_GetField864`; name it again from this dossier, or leave it out to keep that name

```cpp
_DWORD * CXiSkeletonActor::vf256(_DWORD *a2)
{
  *a2 = this->field_864;
  return a2;
}
```

## `0x100A4820` CXiSkeletonActor::vf257

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 257 (+0x404), `XiSkeletonActor2` slot 257 (+0x404), `CXiSkeletonActor` slot 257 (+0x404), `XiChocoboActor2` slot 257 (+0x404), `CXiGeomActor` slot 257 (+0x404), `CXiDancerActor` slot 257 (+0x404)

```cpp
char CXiSkeletonActor::vf257()
{
  return 0;
}
```

## `0x100A4A00` CXiSkeletonActor::vf030

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 30 (+0x78), `XiSkeletonActor2` slot 30 (+0x78), `CXiSkeletonActor` slot 30 (+0x78), `CXiMannequinActor` slot 30 (+0x78), `XiChocoboActor2` slot 30 (+0x78), `CXiGeomActor` slot 30 (+0x78), `CXiDancerActor` slot 30 (+0x78)
- **Named before** (a naming session's reading, low): `CXiSkeletonActor_IsEffectFlag8`; name it again from this dossier, or leave it out to keep that name

```cpp
BOOL CXiSkeletonActor::vf030()
{
  return (this->effectFlags & 8) != 0;
}
```

## `0x100A4AD0` CXiSkeletonActor::vf045

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 45 (+0xB4), `XiSkeletonActor2` slot 45 (+0xB4), `CXiSkeletonActor` slot 45 (+0xB4), `CXiMannequinActor` slot 45 (+0xB4), `XiChocoboActor2` slot 45 (+0xB4), `CXiGeomActor` slot 45 (+0xB4), `CXiDancerActor` slot 45 (+0xB4)
- **Named before** (a naming session's reading, low): `CXiSkeletonActor_GetField8A7`; name it again from this dossier, or leave it out to keep that name

```cpp
char CXiSkeletonActor::vf045()
{
  return this->field_8A7;
}
```

## `0x100CE160` fn_100CE160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `KzMotionQueList_AdvanceAll` (low), `CYyModel::GetField44Addr`
- **Called by:** `fn_10087E40`, `sub_100B2DD0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `Acter_ShutdownGlobals` → `fn_1008A7B0` → `fn_1008EE30` → `XiEvent_RestoreActer` → `fn_10087E40` → `fn_100CE160`
- **Named callers' callers:** `XiEvent_ExecProg`, `XiEvent_RestoreActer`
- **How its callers call it** (their verified code):
  - in `fn_10087E40`: `orig::FUN_100ce160(*(void **)((int)self_ + 0xa0));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `KzMotionQueList_AdvanceAll` (low): Advances every node of the motion queue list by a time step.

Short callee `KzMotionQueList_AdvanceAll` (low):
```cpp
XI_API void __thiscall KzMotionQueList_AdvanceAll(void * self_, float param_1)
{
  void *this_00;
  
  for (this_00 = *(void **)((int)self_ + 4); this_00 != (void *)0x0;
      this_00 = *(void **)((int)this_00 + 4)) {
    orig::FUN_1001b520(this_00,param_1);
  }
  return;
}
```

Short callee `CYyModel::GetField44Addr`:
```cpp
int __thiscall CYyModel::GetField44Addr(void * self)
{
  return (int)self + 0x44;
}
```

```cpp
XI_API void __thiscall fn_100CE160(void * self_)
{
  int *piVar1;
  
  piVar1 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if (*piVar1 != 0) {
    KzMotionQueList_AdvanceAll((void *)(*piVar1 + 0x74),8.0f);
  }
  return;
}
```

## `0x100CF100` fn_100CF100

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor::GetModelField170`, `sub_100D5FE0`, `sub_100D6180`, `CXiSkeletonActor_FindPartValue` (low)
- **Called by:** `sub_1008C480`, `sub_100973F0`
- **Named callees' callees:** `CYyModelBase::GetPartList`, `CYyModel::GetField44Addr`, `CYyModel_QueryPartsA`, `CYyModel_QueryPartsB`, `Settings_GetValue`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiSkeletonActor_FindPartValue` (low): Finds the part whose tag matches the argument and returns its value.

Short callee `CXiActor::GetModelField170`:
```cpp
undefined4 __thiscall CXiActor::GetModelField170(void * self)
{
  if (*(int *)((int)self + 0x70) != 0) {
    return *(uint32_t *)(*(int *)((int)self + 0x70) + 0x170);
  }
  return DAT_1047d60c;
}
```

```cpp
XI_API uint64_t __thiscall fn_100CF100(void * self_, undefined4 xi_s1)
{
  int iVar1;
  uint32_t uVar2;
  uint32_t uVar3;
  int *piVar4;
  uint64_t uVar5;
  
  piVar4 = (int *)0x0;
  do {
    if (*(short *)((int)self_ + 0xb2) == 0) {
      iVar1 = orig::CXiActor_GetModelField170_v1(self_);
      if (iVar1 != 1) goto LAB_100cf141;
      uVar2 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 800))))((void *)(self_));
      if ((short)uVar2 == 0) goto LAB_100cf141;
      uVar3 = CXiSkeletonActor_FindPartValue(self_,piVar4);
      if ((char)uVar3 != '\0') goto LAB_100cf141;
      uVar5 = orig::FUN_100d5fe0(self_,(int)piVar4);
    }
    else {
LAB_100cf141:
      uVar5 = orig::FUN_100d6180(self_,(int)piVar4);
    }
    piVar4 = (int *)((int)piVar4 + 1);
    if (9 < (int)piVar4) {
      return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar5 >> 0x20)) << 32) | (uint32_t)(((uint32_t)(((uint32_t)((int32_t)((uint64_t)uVar5 >> 8)) << 8) | (uint8_t)(1))))));
    }
  } while( true );
}
```

## `0x100CFAD0` fn_100CFAD0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100CFB40`
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 242 (+0x3C8), `XiSkeletonActor2` slot 242 (+0x3C8), `CXiSkeletonActor` slot 242 (+0x3C8), `CXiMannequinActor` slot 242 (+0x3C8), `XiChocoboActor2` slot 242 (+0x3C8), `CXiGeomActor` slot 242 (+0x3C8), `CXiDancerActor` slot 242 (+0x3C8)
- **Named callees' callees:** `CYyModel::GetField44Addr`, `Skel_GetTableAfterBones`, `CXiActor_ModelTest95680`, `CXiActor_ModelTest956A0`, `CXiActor::GetModelField170`

```cpp
XI_API uint8_t * __thiscall fn_100CFAD0(void * self_, int param_1)
{
  uint16_t *puVar1;
  
  puVar1 = orig::FUN_100cfb40(self_);
  if (puVar1 == (uint16_t *)0x0) {
    if ((param_1 != 0) && (param_1 == 1)) {
      return (uint8_t *)0x0;
    }
    puVar1 = (uint16_t *)&DAT_1048a390;
  }
  return (uint8_t *)puVar1;
}
```

## `0x100D0D40` fn_100D0D40

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor_SetField0B4` (low), `sub_100CF750`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`, `sub_100D4100`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `fn_100D0D40`
- **Named callers' callers:** `CXiSkeletonActor_OnSkeletonLoaded`, `CXiSkeletonActor_LoadSkeletonResource`
- **Named callees' callees:** `Actor_GetPercentScaleByIndex`, `Actor_LookupPercentScaleA`, `Actor_LookupPercentScaleB`, `CYyModelBase::SetResourceAndRebuild`, `CYyModel::GetField44Addr`, `CYyModel::CreateModelBase`, `CMoResource_FindInScope`, `ResourceHandle_Deref`, `CXiActor::GetModelByteEF`, `CXiActor::GetModelFlags120Top2`, `CXiActor::GetModelFlags130Top2`, `CXiActor::GetModelFloat9AEC0`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiActor_SetField0B4` (low): Stores a value in the actor's +0xB4 field.

Short callee `CXiActor_SetField0B4` (low):
```cpp
XI_API void __thiscall CXiActor_SetField0B4(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xb4) = param_1;
  return;
}
```

```cpp
XI_API undefined4 __thiscall fn_100D0D40(void * self_, int * param_1, int param_2)
{
  if ((param_2 == *(int *)((int)self_ + 0x9ec)) && (param_1 != (int *)0x0)) {
    orig::CXiActor_set_field_B4(self_,param_1);
    param_2 = (int)orig::FUN_100cf750(self_,param_1);
  }
  return param_2;
}
```

## `0x100D0D70` fn_100D0D70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `ResourceHandle_IsResident`, `Unit10070F90_Create` (medium), `CMoResource_FindOrCreateTask` (medium), `ActorHandle_Constructor`, `sub_100814F0`, `CXiActor::GetModelByteEE`, `CXiActor::GetModelPartWord`, `CXiActor_SetField0B4` (low), `Unit100C51C0_MapCodeIndex` (low), `sub_100D0B50`, `CXiSkeletonActor_SelectStateMessage` (low), `Allocator_Generic`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`, `sub_100D4220`
- **Reads globals:** `0x1047D128`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMeshResourceMng`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `fn_100D0D70`
- **Named callers' callers:** `CXiSkeletonActor_OnSkeletonLoaded`, `CXiSkeletonActor_LoadSkeletonResource`
- **Named callees' callees:** `CYyModel::GetField44Addr`, `CYyModel::CreateModelBase`, `CMoAttachments_Construct`, `CMoAttachments::SetCaster`, `CMoAttachments::SetTarget`, `CMoTask_InitVariant12`, `CMoTask_InitAndRegister`, `CMoResource::GetParent`, `CMoResource_FindInScope`, `Unit10072A50_Start`, `ResourceIter_InPoolRange`, `CMoTask::ResetFields` and 6 more
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10070F90_Create` (medium): Creates an attachments task with its caster and target.
  - `CMoResource_FindOrCreateTask` (medium): Finds the resource with the given id, or creates and registers a new task for it.
  - `CXiActor_SetField0B4` (low): Stores a value in the actor's +0xB4 field.
  - `Unit100C51C0_MapCodeIndex` (low): Maps a code and index to a table offset.
  - `CXiSkeletonActor_SelectStateMessage` (low): Chooses one of the state messages to send to the skeleton actor from its current flags.

Short callee `ActorHandle_Constructor`:
```cpp
XI_API void __thiscall ActorHandle_Constructor(void * self_)
{
  *(uint8_t *)((int)self_ + 0xb) = 4;
  *(uint8_t ***)self_ = &PTR_FUN_1032cf5c;
  *(uint32_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 8) = *(uint32_t *)((int)self_ + 8) & 0xff000000;
  return;
}
```

Short callee `CXiActor::GetModelByteEE`:
```cpp
int __thiscall CXiActor::GetModelByteEE(void * self)
{
  if (*(int *)((int)self + 0x70) != 0) {
    return (int)*(char *)(*(int *)((int)self + 0x70) + 0xee);
  }
  return DAT_1047d610;
}
```

Short callee `CXiActor_SetField0B4` (low):
```cpp
XI_API void __thiscall CXiActor_SetField0B4(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xb4) = param_1;
  return;
}
```

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100D0D70(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2, int * param_3)
{
  int *xi_s2;
  uint32_t uVar1;
  int iVar2;
  uint32_t uVar3;
  uint32_t *puVar4;
  uint64_t uVar5;
  
  xi_s2 = param_3;
  if (param_3 == *(int **)((int)self_ + 0x9ec)) {
    if (param_2 != 0) {
      orig::CXiActor_set_field_B4(self_,param_2);
      ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x290))))((void *)(self_),param_2);
    }
    CXiSkeletonActor_SelectStateMessage(self_,param_2,xi_s2);
    xi_eax = orig::CXiActor_GetModelByteEE_v1(self_);
    if (xi_eax == 7) {
      uVar1 = orig::CXiActor_GetModelPartWord(self_,9);
      xi_eax = 0;
      if ((uVar1 & 0xfff) != 0) {
        iVar2 = Unit100C51C0_MapCodeIndex(0,uVar1 & 0xfff);
        CMoResource_FindOrCreateTask(&param_3,iVar2);
        uVar3 = orig::ResourceHandle_IsResident_v1(&param_3);
        if ((char)uVar3 != '\0') {
          uVar3 = orig::FUN_100d0b50(self_,param_3,uVar1 | 0x9000,*(int *)((int)self_ + 0x9ec),1);
          return uVar3;
        }
        uVar5 = orig::Allocator_Generic_v1(0x20);
        puVar4 = (uint32_t *)uVar5;
        xi_eax = 0;
        if (puVar4 != (uint32_t *)0x0) {
          orig::ActorHandle_Constructor_v1(puVar4 + 1);
          *puVar4 = *(uint32_t *)((int)self_ + 0x9ec);
          puVar4[5] = 0;
          orig::FUN_100814f0(puVar4 + 1,(int)self_);
          puVar4[4] = 0;
          puVar4[6] = uVar1 | 0x9000;
          puVar4[7] = iVar2;
          xi_eax = Unit10070F90_Create(&param_3,XI_CODE(FUN_100d0e80),puVar4,(int)self_,0);
        }
      }
    }
  }
  return xi_eax;
}
```

## `0x100D0FC0` fn_100D0FC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_100D0D70`
- **Named before** (a naming session's reading, low): `CXiSkeletonActor_SelectStateMessage`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `fn_100D0D70` → `CXiSkeletonActor_SelectStateMessage`
- **Named callers' callers:** `CXiSkeletonActor_LoadSkeletonSync`
- **How its callers call it** (their verified code):
  - in `fn_100D0D70`: `CXiSkeletonActor_SelectStateMessage(self_,param_2,xi_s2);`

```cpp
XI_API void __thiscall CXiSkeletonActor_SelectStateMessage(void * self_, undefined4 xi_s1, undefined4 xi_s2)
{
  uint8_t bVar1;
  int iVar2;
  int iVar3;
  uint32_t uVar4;
  
  iVar2 = (**(code **)(*(int *)self_ + 0x394))();
  iVar3 = (**(code **)(*(int *)self_ + 0x378))();
  if ((iVar3 == 0) && (iVar3 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x36c))))((void *)(self_)), iVar3 == 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  iVar3 = (**(code **)(*(int *)self_ + 0x3a4))();
  if (iVar2 == 0) {
    if (bVar1) {
      ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x74696e69,self_,0);
      return;
    }
    if (iVar3 == 0) {
      return;
    }
  }
  else {
    if (*(int *)((int)self_ + 0x70) == 0) {
      return;
    }
    uVar4 = *(uint32_t *)(*(int *)((int)self_ + 0x70) + 0x134) >> 0x15 & 0xf;
    if (uVar4 != 2) {
      if (uVar4 != 3) {
        return;
      }
      ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x316e6f63,self_,0);
      return;
    }
  }
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x306e6f63,self_,0);
  return;
}
```

## `0x100D43E0` fn_100D43E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10087830`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `fn_100D43E0`
- **Named callers' callers:** `CXiSkeletonActor_OnSkeletonLoaded`, `CXiSkeletonActor_LoadSkeletonResource`

Short callee `fn_10087830`:
```cpp
XI_API undefined4 __cdecl fn_10087830(void)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10087670();
  return CONCAT31((int32_t)((uint32_t)uVar1 >> 8),-((char)uVar1 != '\0') & DAT_10480825);
}
```

```cpp
XI_API int __cdecl fn_100D43E0(void)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10087830();
  uVar1 = uVar1 & 0xff;
  if (1 < uVar1) {
    uVar1 = 0;
  }
  return (uint16_t)(&DAT_10330f34)[uVar1] + 0xca53;
}
```

## `0x100D44B0` fn_100D44B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit1002C7C0_PopHead` (low)
- **Called by:** `sub_1008B460`, `Acter_ReleaseSlotA4` (low), `Acter_ReleaseAllSlots` (medium), `sub_100A8DB0`, `sub_100B5510`, `XiEvent_RestoreActer` (low), `sub_100D16C0`, `sub_100D1970`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `Acter_ShutdownGlobals` → `fn_1008A7B0` → `fn_1008EE30` → `XiEvent_RestoreActer` → `fn_100D44B0`
- **Named callers' callers:** `Acter_UpdateAllLists`, `Acter_ClearPendingState`, `Acter_ClearPending272`, `Acter_UpdateActionState`
- **How its callers call it** (their verified code):
  - in `Acter_ReleaseSlotA4` (low): `orig::FUN_100d44b0(*(void **)((int)self_ + 0xa0),iVar1,xi_eax);`
  - in `Acter_ReleaseAllSlots` (medium): `orig::FUN_100d44b0(*(void **)((int)self_ + 0xa0),3,*(uint32_t *)((int)self_ + 0x120));`
  - in `XiEvent_RestoreActer` (low): `orig::FUN_100d44b0(this_01,2,xi_eax);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit1002C7C0_PopHead` (low): Takes the head entry from one of the object's two lists.
  - `Acter_ReleaseSlotA4` (low): Releases the object at +0xA4 and calls a helper for flag 0x400 before it.
  - `Acter_ReleaseAllSlots` (medium): Runs every slot release in order and then clears the state flags.
  - `XiEvent_RestoreActer` (low): Restores the acter referenced by an event record.

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100D44B0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2)
{
  int *piVar1;
  int *this_00;
  
  piVar1 = *(int **)((int)self_ + 0x7e4);
  while (this_00 = piVar1, this_00 != (int *)0x0) {
    xi_eax = this_00[3];
    piVar1 = (int *)this_00[1];
    if (xi_eax == param_2) {
      Unit1002C7C0_PopHead(this_00,(int *)((int)self_ + 0x7e4));
      xi_eax = ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*this_00 + 0x18))))((void *)(this_00),1);
    }
  }
  return xi_eax;
}
```

## `0x100D6240` fn_100D6240

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyModelBase::GetPartList`, `CYyModel::GetField44Addr`, `CYyModel_QueryPartsA` (low), `CYyModel_QueryPartsB` (low)
- **Called by:** `CXiSkeletonActor_Init`, `fn_100CF100`
- **Named before** (a naming session's reading, low): `CXiSkeletonActor_FindPartValue`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor::Constructor_1Arg` → `CXiSkeletonActor_Init` → `CXiSkeletonActor_FindPartValue`
- **Named callers' callers:** `CXiSkeletonActor::Constructor_1Arg`, `CXiSkeletonActor::Constructor_2Arg`, `CXiSkeletonActor::Constructor_3Arg`, `CXiSkeletonActor::Constructor_4Arg`, `CXiSkeletonActor::Constructor_Factory`
- **Named callees' callees:** `Unit10029CD0_GetField20`
- **How its callers call it** (their verified code):
  - in `CXiSkeletonActor_Init`: `CXiSkeletonActor_FindPartValue((char *)self, 0);`
  - in `fn_100CF100`: `uVar3 = CXiSkeletonActor_FindPartValue(self_,piVar4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYyModel_QueryPartsA` (low): Returns FUN_1002B3F0 of the argument when the model has parts.
  - `CYyModel_QueryPartsB` (low): Returns FUN_1002B470 of the argument when the model has parts.

Short callee `CYyModelBase::GetPartList`:
```cpp
int __thiscall CYyModelBase::GetPartList(void * self)
{
  return (int)self + 0x20;
}
```

Short callee `CYyModel::GetField44Addr`:
```cpp
int __thiscall CYyModel::GetField44Addr(void * self)
{
  return (int)self + 0x44;
}
```

Short callee `CYyModel_QueryPartsA` (low):
```cpp
XI_API undefined4 __thiscall CYyModel_QueryPartsA(void * self_, undefined4 xi_s1)
{
  int *piVar1;
  uint32_t uVar2;
  
  piVar1 = (int *)orig::CYyModel_GetField44Addr_v1(self_);
  if (*piVar1 == 0) {
    return 0;
  }
  uVar2 = orig::FUN_1002b3f0(xi_s1);
  return uVar2;
}
```

Short callee `CYyModel_QueryPartsB` (low):
```cpp
XI_API uint32_t __thiscall CYyModel_QueryPartsB(void * self_, undefined4 xi_s1)
{
  int *piVar1;
  uint32_t uVar2;
  
  piVar1 = (int *)orig::CYyModel_GetField44Addr_v1(self_);
  if (*piVar1 == 0) {
    return (uint32_t)piVar1 & 0xffffff00;
  }
  uVar2 = orig::FUN_1002b470(xi_s1);
  return uVar2;
}
```

```cpp
XI_API uint32_t __thiscall CXiSkeletonActor_FindPartValue(void * self_, int * param_1)
{
  int *piVar1;
  void *this_00;
  int iVar2;
  uint32_t uVar3;
  void *this_01;
  int xi_s1;
  
  if ((-1 < (int)param_1) || ((int)param_1 < 10)) {
    piVar1 = param_1 + 0xc;
    this_01 = (void *)((int)self_ + 0x674);
    param_1 = (int *)orig::CYyModel_GetField44Addr_v1(this_01);
    for (this_00 = (void *)*param_1; this_00 != (void *)0x0; this_00 = *(void **)((int)this_00 + 4))
    {
      xi_s1 = 0;
      param_1 = (int *)orig::CYyModelBase_GetPartList_v1(this_00);
      for (iVar2 = *param_1; iVar2 != 0; iVar2 = *(int *)(iVar2 + 4)) {
        param_1 = (int *)CYyModel_QueryPartsA(this_01,xi_s1);
        if (param_1 == (int *)((int)piVar1 * 0x1000000 | 0x706577U)) {
          uVar3 = CYyModel_QueryPartsB(this_01,xi_s1);
          return uVar3;
        }
        xi_s1 = xi_s1 + 1;
      }
    }
  }
  return (uint32_t)param_1 & 0xffffff00;
}
```
