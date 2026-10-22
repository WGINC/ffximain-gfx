# Batch r2_072_CMoPathDriveActorTask: 1 functions of `CMoPathDriveActorTask`

Name these functions: see `batches/README.md`. Write `data/names/r2_072_CMoPathDriveActorTask.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x1005EEF0` fn_1005EEF0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Vec4_Copy`, `CYyObject_IsKindOf` x2, `sub_1003B6D0` x9, `CMoSpline_SetValue` (low), `CXiControlActor_SetVec4` (low)
- **Called by:** `CMoPathDriveActorTask_Update` (medium)
- **Named before** (a naming session's reading, low): `CMoPathDriveActorTask_ApplyFraction`; name it again from this dossier, or leave it out to keep that name
- **Named callees' callees:** `CMoSpline_EnsureSamples`, `ActorHandle_Resolve`
- **How its callers call it** (their verified code):
  - in `CMoPathDriveActorTask_Update` (medium): `uVar5 = CMoPathDriveActorTask_ApplyFraction(self,fStack_44);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoPathDriveActorTask_Update` (medium): Advances the path drive by the frame time and applies its position and heading to the actor.
  - `CMoSpline_SetValue` (low): Sets a value on the spline's sample table.
  - `CXiControlActor_SetVec4` (low): Stores a four-component vector in the actor.

Short callee `CMoSpline_SetValue` (low):
```cpp
XI_API void __thiscall CMoSpline_SetValue(void * self_, undefined4 param_1, undefined4 param_2)
{
  void *this_00;
  
  this_00 = (void *)CMoSpline_EnsureSamples(self_);
  orig::FUN_10063a60(this_00,param_2,param_1);
  return;
}
```

Short callee `CXiControlActor_SetVec4` (low):
```cpp
XI_API undefined4 __thiscall CXiControlActor_SetVec4(void * self_, undefined4 * param_1)
{
  uint32_t uVar1;
  
  *(uint32_t *)((int)self_ + 0xd4) = *param_1;
  *(uint32_t *)((int)self_ + 0xd8) = param_1[1];
  *(uint32_t *)((int)self_ + 0xdc) = param_1[2];
  uVar1 = param_1[3];
  *(uint32_t *)((int)self_ + 0xe0) = uVar1;
  return uVar1;
}
```

```cpp
XI_API undefined4 __thiscall CMoPathDriveActorTask_ApplyFraction(void * self_, undefined4 param_1)
{
  void *this_00;
  int iVar1;
  float *pfVar2;
  float *pfVar3;
  uint8_t bVar4;
  uint8_t bVar5;
  long double fVar6;
  long double fVar7;
  uint64_t uVar8;
  char xi_frame[80] __attribute__((aligned(4)));
  uint8_t ** &ppuVar9 = *reinterpret_cast<uint8_t ** *>(xi_frame + 0);
  uint32_t  &local_40 = *reinterpret_cast<uint32_t  *>(xi_frame + 16);
  float  &local_3c = *reinterpret_cast<float  *>(xi_frame + 20);
  float  &local_38 = *reinterpret_cast<float  *>(xi_frame + 24);
  uint32_t  &local_34 = *reinterpret_cast<uint32_t  *>(xi_frame + 28);
  float  &local_30 = *reinterpret_cast<float  *>(xi_frame + 32);
  float  &local_2c = *reinterpret_cast<float  *>(xi_frame + 36);
  float  &local_28 = *reinterpret_cast<float  *>(xi_frame + 40);
  float  &local_24 = *reinterpret_cast<float  *>(xi_frame + 44);
  float  &local_20 = *reinterpret_cast<float  *>(xi_frame + 48);
  float  &local_1c = *reinterpret_cast<float  *>(xi_frame + 52);
  float  &local_18 = *reinterpret_cast<float  *>(xi_frame + 56);
  float  &local_14 = *reinterpret_cast<float  *>(xi_frame + 60);
  float  &local_10 = *reinterpret_cast<float  *>(xi_frame + 64);
  float  &local_c = *reinterpret_cast<float  *>(xi_frame + 68);
  float  &local_8 = *reinterpret_cast<float  *>(xi_frame + 72);
  float  &local_4 = *reinterpret_cast<float  *>(xi_frame + 76);
  
  if (*(int **)((int)self_ + 0x74) == (int *)0x0) {
    iVar1 = 0;
  }
  else {
    iVar1 = **(int **)((int)self_ + 0x74);
  }
  CMoSpline_SetValue((void *)(iVar1 + 0x30),&local_20,param_1);
  bVar5 = true;
  if ((*(uint32_t *)((int)self_ + 0x9c) != 0) && (*(char *)((int)self_ + 0xa0) != '\0')) {
    local_40 = 0;
    local_30 = (float)((long double)local_20 - (long double)*(float *)((int)self_ + 0x80));
    local_2c = (float)((long double)local_1c - (long double)*(float *)((int)self_ + 0x84));
    local_28 = (float)((long double)local_18 - (long double)*(float *)((int)self_ + 0x88));
    fVar6 = SQRT((long double)local_28 * (long double)local_28 + (long double)local_30 * (long double)local_30);
    fVar7 = xi_castbits<long double>(fpatan((long double)local_28,(long double)local_30));
    fVar7 = (long double)*(float *)((int)self_ + 0x94) - fVar7;
    local_3c = (float)fVar7;
    if (fVar7 > (long double)3.141499996185302734375f) {
      local_3c = (float)((long double)local_3c - (long double)6.28299999237060546875f);
    }
    bVar4 = NAN((long double)local_3c) || NAN((long double)(-3.141499996185302734375f));
    if ((bVar4 || (long double)local_3c < (long double)(-3.141499996185302734375f)) != bVar4) {
      local_3c = (float)((long double)local_3c + (long double)6.28299999237060546875f);
    }
    if ((*(uint32_t *)((int)self_ + 0x9c) & 2) == 0) {
      fVar7 = xi_castbits<long double>(fpatan((long double)local_2c,fVar6));
      local_38 = (float)fVar7;
    }
    bVar4 = NAN(fVar6) || NAN((long double)0.100000001490116119384765625f);
    local_34 = 0x3f800000;
    if ((bVar4 || fVar6 < (long double)0.100000001490116119384765625f) != bVar4) {
      bVar5 = false;
    }
  }
  this_00 = (void *)((int)self_ + 0x34);
  uVar8 = orig::FUN_1003b6d0(this_00);
  if ((int)uVar8 == 0) goto LAB_1005f289;
  ppuVar9 = &PTR_s_CXiControlActor_1032df68;
  uVar8 = orig::FUN_1003b6d0(this_00);
  uVar8 = orig::CYyObject_IsKindOf((void *)uVar8,(int)ppuVar9);
  if ((char)uVar8 == '\0') goto LAB_1005f289;
  if (*(char *)((int)self_ + 0xa1) == '\0') {
    if (*(char *)((int)self_ + 0xa0) != '\0') {
      local_30 = (float)((long double)local_20 - (long double)*(float *)((int)self_ + 0x80));
      local_2c = (float)((long double)local_1c - (long double)*(float *)((int)self_ + 0x84));
      local_28 = (float)((long double)local_18 - (long double)*(float *)((int)self_ + 0x88));
      local_24 = (float)((long double)local_14 - (long double)*(float *)((int)self_ + 0x8c));
      local_10 = local_30;
      local_c = local_2c;
      local_8 = local_28;
      local_4 = local_24;
      uVar8 = orig::FUN_1003b6d0(this_00);
      pfVar2 = (float *)((int (__thiscall *)(void *))((**(code **)(*(int *)uVar8 + 0x1bc))))((void *)((int *)uVar8));
      local_30 = (float)((long double)local_30 + (long double)*pfVar2);
      local_2c = (float)((long double)local_2c + (long double)pfVar2[1]);
      local_28 = (float)((long double)local_28 + (long double)pfVar2[2]);
      pfVar3 = &local_30;
      local_24 = (float)((long double)local_24 + (long double)pfVar2[3]);
      goto LAB_1005f0d9;
    }
  }
  else {
    pfVar3 = &local_20;
LAB_1005f0d9:
    uVar8 = orig::FUN_1003b6d0(this_00);
    CXiControlActor_SetVec4((void *)uVar8,pfVar3);
  }
  if ((*(int *)((int)self_ + 0x9c) != 0) && (*(char *)((int)self_ + 0xa0) != '\0')) {
    if (*(char *)((int)self_ + 0xa3) == '\0') {
      ppuVar9 = &XiModelActor_classdesc;
      uVar8 = orig::FUN_1003b6d0(this_00);
      uVar8 = orig::CYyObject_IsKindOf((void *)uVar8,(int)ppuVar9);
      if ((char)uVar8 != '\0') {
        uVar8 = orig::FUN_1003b6d0(this_00);
        iVar1 = (int)uVar8;
        *(uint32_t *)(iVar1 + 0x744) = local_40;
        *(float *)(iVar1 + 0x748) = local_3c;
        *(float *)(iVar1 + 0x74c) = local_38;
        *(uint32_t *)(iVar1 + 0x750) = local_34;
      }
      uVar8 = orig::FUN_1003b6d0(this_00);
      iVar1 = (int)uVar8;
      *(uint32_t *)(iVar1 + 0xe4) = local_40;
      *(float *)(iVar1 + 0xe8) = local_3c;
      *(float *)(iVar1 + 0xec) = local_38;
      *(uint32_t *)(iVar1 + 0xf0) = local_34;
      uVar8 = orig::FUN_1003b6d0(this_00);
      iVar1 = (int)uVar8;
      pfVar3 = (float *)(iVar1 + 0x44);
      orig::Vec4_Copy_v1(pfVar3,&local_40);
      if ((long double)*pfVar3 > (long double)3.141499996185302734375f) {
        *pfVar3 = (float)((long double)*pfVar3 - (long double)6.28299999237060546875f);
      }
      bVar5 = NAN((long double)*pfVar3) || NAN((long double)(-3.141499996185302734375f));
      if ((bVar5 || (long double)*pfVar3 < (long double)(-3.141499996185302734375f)) != bVar5) {
        *pfVar3 = (float)((long double)*pfVar3 + (long double)6.28299999237060546875f);
      }
      if ((long double)*(float *)(iVar1 + 0x48) > (long double)3.141499996185302734375f) {
        *(float *)(iVar1 + 0x48) = (float)((long double)*(float *)(iVar1 + 0x48) - (long double)6.28299999237060546875f);
      }
      bVar5 = NAN((long double)*(float *)(iVar1 + 0x48)) || NAN((long double)(-3.141499996185302734375f));
      if ((bVar5 || (long double)*(float *)(iVar1 + 0x48) < (long double)(-3.141499996185302734375f)) != bVar5) {
        *(float *)(iVar1 + 0x48) = (float)((long double)*(float *)(iVar1 + 0x48) + (long double)6.28299999237060546875f);
      }
      if ((long double)*(float *)(iVar1 + 0x4c) > (long double)3.141499996185302734375f) {
        *(float *)(iVar1 + 0x4c) = (float)((long double)*(float *)(iVar1 + 0x4c) - (long double)6.28299999237060546875f);
      }
      bVar5 = NAN((long double)*(float *)(iVar1 + 0x4c)) || NAN((long double)(-3.141499996185302734375f));
      if ((bVar5 || (long double)*(float *)(iVar1 + 0x4c) < (long double)(-3.141499996185302734375f)) != bVar5) {
        *(float *)(iVar1 + 0x4c) = (float)((long double)*(float *)(iVar1 + 0x4c) + (long double)6.28299999237060546875f);
      }
      *(uint8_t *)((int)self_ + 0xa3) = 1;
    }
    else if (bVar5) {
      uVar8 = orig::FUN_1003b6d0(this_00);
      iVar1 = (int)uVar8;
      *(uint32_t *)(iVar1 + 0xe4) = local_40;
      *(float *)(iVar1 + 0xe8) = local_3c;
      *(float *)(iVar1 + 0xec) = local_38;
      *(uint32_t *)(iVar1 + 0xf0) = local_34;
    }
  }
LAB_1005f289:
  *(float *)((int)self_ + 0x80) = local_20;
  *(float *)((int)self_ + 0x84) = local_1c;
  *(float *)((int)self_ + 0x88) = local_18;
  *(float *)((int)self_ + 0x8c) = local_14;
  return local_18;
}
```
