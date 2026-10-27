# Batch r3_010_CXiSkeletonActor: 4 functions of `CXiSkeletonActor`

Name these functions: see `batches/README.md`. Write `data/names/r3_010_CXiSkeletonActor.json`.
3 of them were sampled in a running frame (data/frame_set.json).

## `0x100CD920` fn_100CD920

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 44`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1001B140`, `sub_1001B360`, `CYyModel::GetField44Addr` x2, `List_GetTail`, `ResourceHandle_IsResident` x5, `CXiActor::GetModelByteEE` x3, `CXiActor::GetModelByteEF`, `CXiSkeletonActor::SetMotion` x4
- **Called by:** `CMoSchedularTask_Interpret`, `sub_10060C60`, `sub_100C83C0`, `sub_100CA8C0`, `sub_100CDD60`, `sub_100CDE70`, `sub_100CDF00`
- **Writes globals:** `0x1048A38C`
- **Reads globals:** `0x1048A38C`
- **Named callers' callers:** `CMoSchedularTask_Tick`, `CXiSkeletonActor_OnMove`
- **Named callees' callees:** `Unit10017C10_NoOpVarargsA`, `CYyMotionQue::SetSlotMotion`, `MotionQueList_QueueMotion`, `CMoResource::GetParent`, `ResourceIter_InPoolRange`, `ResourceIter_FindFirst`, `ResourceIter_FindNext`

Short callee `CYyModel::GetField44Addr`:
```cpp
int __thiscall CYyModel::GetField44Addr(void * self)
{
  return (int)self + 0x44;
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

```cpp
XI_API undefined4 __thiscall fn_100CD920(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9, undefined4 param_10, int param_11)
{
  uint8_t bVar1;
  uint8_t bVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int *piVar6;
  undefined4 *puVar7;
  uint32_t uVar8;
  int iVar9;
  void *pvVar10;
  uint32_t uVar11;
  char xi_frame[24] __attribute__((aligned(4)));
  uint32_t  &local_18 = *reinterpret_cast<uint32_t  *>(xi_frame + 0);
  uint8_t  (&auStack_c)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 12);
  uint8_t  (&auStack_8)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 16);
  undefined4  &uStack_4 = *reinterpret_cast<undefined4  *>(xi_frame + 20);
  
  iVar9 = 0;
  bVar2 = false;
  DAT_1048a38c = 0;
  local_18 = 0;
  piVar3 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if (*piVar3 != 0) {
    pvVar10 = (void *)(*piVar3 + 0x24);
    do {
      uVar4 = orig::FUN_1001b360(pvVar10);
      if ((char)uVar4 != '\0') {
        local_18 = local_18 | 1 << ((uint8_t)iVar9 & 0x1f);
      }
      iVar9 = iVar9 + 1;
      pvVar10 = (void *)((int)pvVar10 + 0x14);
    } while (iVar9 < 3);
  }
  bVar1 = false;
  iVar9 = orig::CXiActor_GetModelByteEE_v1(self_);
  if ((iVar9 == 6) || (iVar9 = orig::CXiActor_GetModelByteEE_v1(self_), iVar9 == 7)) {
    bVar1 = true;
  }
  piVar3 = param_10;
  uVar4 = param_9;
  if ((*(char *)((int)self_ + 0x88) < '\0') || (((param_1 & 0xff000000) == 0x3f000000 && (bVar1)))) {
    piVar6 = (int *)param_11;
    if (param_11 == 0) {
      piVar6 = (int *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_10);
      if (*piVar6 != 0) {
        pvVar10 = (void *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_10);
        uVar5 = orig::ResourceHandle_IsResident_v1(pvVar10);
        if ((char)uVar5 != '\0') {
          puVar7 = (undefined4 *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_10);
          piVar6 = (int *)*puVar7;
          goto LAB_100cda6e;
        }
      }
    }
    else {
LAB_100cda6e:
      uVar5 = orig::CXiSkeletonActor_SetMotion
                        (self_,piVar6,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8
                         ,uVar4,piVar3);
      if ((char)uVar5 != '\0') goto switchD_100cdb07_caseD_3;
    }
  }
  for (pvVar10 = orig::List_GetTail_v1(*(void **)((int)self_ + 0x7e4)); pvVar10 != (void *)0x0;
      pvVar10 = *(void **)((int)pvVar10 + 8)) {
    param_10 = *(int **)((int)pvVar10 + 0x1c);
    uVar5 = orig::ResourceHandle_IsResident_v1(&param_10);
    uVar4 = param_9;
    if ((char)uVar5 == '\0') {
      bVar2 = true;
      goto switchD_100cdb07_caseD_3;
    }
    orig::CXiSkeletonActor_SetMotion
              (self_,param_10,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9
               ,piVar3);
  }
  if (*(int *)((int)self_ + 0x70) == 0) {
    piVar6 = (int *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1dc))))((void *)(self_),&param_9);
    if (*piVar6 == 0) goto switchD_100cdb07_caseD_3;
    pvVar10 = (void *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1dc))))((void *)(self_),&param_9);
    uVar5 = orig::ResourceHandle_IsResident_v1(pvVar10);
    if ((char)uVar5 == '\0') goto switchD_100cdb07_caseD_3;
    puVar7 = &param_9;
  }
  else {
    iVar9 = orig::CXiActor_GetModelByteEE_v1(self_);
    switch(iVar9) {
    case 0:
    case 1:
    case 6:
    case 8:
      iVar9 = orig::CXiActor_GetModelByteEF(self_);
      if (((((((iVar9 < 0x1d) && (*(int *)((int)self_ + 0x7e4) == 0)) &&
             (iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x34c))))((void *)(self_)), iVar9 == 0)) &&
            ((iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x358))))((void *)(self_)), iVar9 == 0 &&
             (iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x360))))((void *)(self_)), iVar9 == 0)))) &&
           ((iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x36c))))((void *)(self_)), iVar9 == 0 &&
            ((iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x378))))((void *)(self_)), iVar9 == 0 &&
             (iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 900))))((void *)(self_)), iVar9 == 0)))))) &&
          (iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x394))))((void *)(self_)), iVar9 == 0)) &&
         (iVar9 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x3a4))))((void *)(self_)), iVar9 == 0))
      goto switchD_100cdb07_caseD_3;
      break;
    case 2:
    case 7:
      break;
    default:
      goto switchD_100cdb07_caseD_3;
    }
    piVar6 = (int *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_9);
    if (*piVar6 != 0) {
      pvVar10 = (void *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_10);
      uVar5 = orig::ResourceHandle_IsResident_v1(pvVar10);
      if ((char)uVar5 != '\0') {
        puVar7 = (undefined4 *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x400))))((void *)(self_),&param_11);
        uVar5 = orig::CXiSkeletonActor_SetMotion
                          (self_,(int *)*puVar7,param_1,param_2,param_3,param_4,param_5,param_6,
                           param_7,param_8,uVar4,piVar3);
        if ((char)uVar5 != '\0') goto switchD_100cdb07_caseD_3;
      }
    }
    piVar6 = (int *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1dc))))((void *)(self_),auStack_c);
    if (*piVar6 == 0) goto switchD_100cdb07_caseD_3;
    pvVar10 = (void *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1dc))))((void *)(self_),auStack_8);
    uVar5 = orig::ResourceHandle_IsResident_v1(pvVar10);
    if ((char)uVar5 == '\0') goto switchD_100cdb07_caseD_3;
    puVar7 = &uStack_4;
  }
  puVar7 = (undefined4 *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1dc))))((void *)(self_),puVar7);
  orig::CXiSkeletonActor_SetMotion
            (self_,(int *)*puVar7,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,
             uVar4,piVar3);
switchD_100cdb07_caseD_3:
  piVar3 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if ((*piVar3 == 0) || ((DAT_1048a38c & 1) == 0)) {
    if (bVar2) {
      return 1;
    }
  }
  else {
    if (bVar2) {
      return 1;
    }
    uVar11 = DAT_1048a38c ^ local_18;
    iVar9 = 0;
    pvVar10 = (void *)(*piVar3 + 0x24);
    do {
      uVar8 = 1 << ((uint8_t)iVar9 & 0x1f);
      if (((uVar11 & uVar8) != 0) && ((local_18 & uVar8) != 0)) {
        orig::FUN_1001b140(pvVar10);
      }
      iVar9 = iVar9 + 1;
      pvVar10 = (void *)((int)pvVar10 + 0x14);
    } while (iVar9 < 3);
  }
  if (DAT_1048a38c == 0) {
    return 1;
  }
  return 0;
}
```

## `0x100CF100` fn_100CF100

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor::GetModelField170`, `sub_100D5FE0`, `sub_100D6180`, `CXiSkeletonActor_FindPartValue` (low)
- **Called by:** `sub_1008C480`, `fn_100973F0`
- **Named callers' callers:** `Acter_UpdateOne`
- **Named callees' callees:** `CYyModelBase::GetPartList`, `CYyModel::GetField44Addr`, `CYyModel_QueryPartsA`, `CYyModel_QueryPartsB`, `Settings_GetValue`
- **How its callers call it** (their verified code):
  - in `fn_100973F0`: `orig::fn_100CF100(*(void **)((int)self_ + 0xa0),0);`
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
      uVar3 = orig::CXiSkeletonActor_FindPartValue_v1(self_,piVar4);
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

## `0x100CFB40` fn_100CFB40

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyModel::GetField44Addr`, `Skel_GetTableAfterBones` (low), `CXiActor_ModelTest95680`, `CXiActor_ModelTest956A0`, `CXiActor::GetModelField170` x3
- **Called by:** `CXiSkeletonActor_GetTableOrDefault` (low)
- **Named callees' callees:** `Skel_GetBoneTableBase`
- **How its callers call it** (their verified code):
  - in `CXiSkeletonActor_GetTableOrDefault` (low): `puVar1 = orig::FUN_100cfb40(self);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Skel_GetTableAfterBones` (low): Returns the address of the table that follows the bone table.
  - `CXiSkeletonActor_GetTableOrDefault` (low): Returns the table that sub_100CFB40 gives for this actor, or a fallback address when that is null.

Short callee `CYyModel::GetField44Addr`:
```cpp
int __thiscall CYyModel::GetField44Addr(void * self)
{
  return (int)self + 0x44;
}
```

Short callee `Skel_GetTableAfterBones` (low):
```cpp
XI_API uint16_t * __thiscall Skel_GetTableAfterBones(void * self_)
{
  uint16_t *puVar1;
  
  puVar1 = (uint16_t *)orig::Skel_GetBoneTableBase(self_);
  if (puVar1 == (uint16_t *)0x0) {
    return (uint16_t *)0x0;
  }
  return puVar1 + (uint32_t)*puVar1 * 0xd + 2;
}
```

Short callee `CXiActor_ModelTest95680`:
```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) CXiActor_ModelTest95680(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2)
{
  uint32_t uVar1;
  
  if (*(void **)((int)self_ + 0x70) != (void *)0x0) {
    uVar1 = orig::Acter_IsState5(*(void **)((int)self_ + 0x70),param_2);
    return uVar1;
  }
  return xi_eax & 0xffffff00;
}
```

Short callee `CXiActor_ModelTest956A0`:
```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) CXiActor_ModelTest956A0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_, int param_2)
{
  uint32_t uVar1;
  
  if (*(void **)((int)self_ + 0x70) != (void *)0x0) {
    uVar1 = orig::Acter_IsState55(*(void **)((int)self_ + 0x70),param_2);
    return uVar1;
  }
  return xi_eax & 0xffffff00;
}
```

```cpp
XI_API int __thiscall fn_100CFB40(void * self_)
{
  uint32_t uVar1;
  int *piVar2;
  uint16_t *puVar3;
  int iVar4;
  
  iVar4 = 0;
  uVar1 = orig::CXiActor_GetModelField170_v1(self_);
  switch(uVar1) {
  case 0:
  case 4:
  case 7:
  case 0x1c:
  case 0x1f:
    iVar4 = 0;
    break;
  case 2:
  case 3:
    iVar4 = 2;
    break;
  default:
    uVar1 = orig::CXiActor_GetModelField170_v1(self_);
    uVar1 = orig::CXiActor_ModelTest95680(self_,uVar1,uVar1);
    if ((char)uVar1 == '\0') {
      uVar1 = orig::CXiActor_GetModelField170_v1(self_);
      uVar1 = orig::CXiActor_ModelTest956A0_v1(self_,uVar1,uVar1);
      if ((char)uVar1 == '\0') break;
    }
  case 1:
  case 6:
  case 0x26:
  case 0x27:
  case 0x28:
  case 0x29:
  case 0x2a:
  case 0x2b:
  case 0x32:
  case 0x33:
  case 0x34:
  case 0x35:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0x3d:
  case 0x3e:
    iVar4 = 1;
  }
  piVar2 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if ((*piVar2 != 0) &&
     (puVar3 = orig::Skel_GetTableAfterBones_v1((void *)(*piVar2 + 8)), puVar3 != (uint16_t *)0x0)) {
    return (int)(puVar3 + iVar4 * 0xc);
  }
  return 0;
}
```

## `0x100D43E0` fn_100D43E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10087830`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `fn_100D43E0`
- **Named callers' callers:** `CXiSkeletonActor_OnSkeletonLoaded`, `CXiSkeletonActor_LoadSkeletonResource`
- **Named callees' callees:** `Unit10086E90_IsGlobalNonzero`

Short callee `fn_10087830`:
```cpp
XI_API undefined4 __cdecl fn_10087830(void)
{
  uint32_t uVar1;
  
  uVar1 = orig::Unit10086E90_IsGlobalNonzero();
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
