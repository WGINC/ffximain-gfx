# Batch r2_006_CXiActor: 16 functions of `CXiActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_006_CXiActor.json`.
9 of them were sampled in a running frame (data/frame_set.json).

## `0x10082860` fn_10082860

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 219 (+0x36C), `CXiSklFurnitureActor` slot 219 (+0x36C), `CXiControlActor` slot 219 (+0x36C), `XiFurnitureActor` slot 219 (+0x36C), `XiSkeletonActor2` slot 219 (+0x36C), `CXiSkeletonActor` slot 219 (+0x36C), `XiModelActor` slot 219 (+0x36C), `CXiMannequinActor` slot 219 (+0x36C) and 8 more
- **Named before** (a naming session's reading, low): `CXiActor_IsA4InRange3DTo51`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API undefined4 __thiscall CXiActor_IsA4InRange3DTo51(void * self_)
{
  if ((0x3c < *(int *)((int)self_ + 0xa4)) && (*(int *)((int)self_ + 0xa4) < 0x52)) {
    return 1;
  }
  return 0;
}
```

## `0x100828B0` fn_100828B0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 222 (+0x378), `CXiSklFurnitureActor` slot 222 (+0x378), `CXiControlActor` slot 222 (+0x378), `XiFurnitureActor` slot 222 (+0x378), `XiSkeletonActor2` slot 222 (+0x378), `CXiSkeletonActor` slot 222 (+0x378), `XiModelActor` slot 222 (+0x378), `CXiMannequinActor` slot 222 (+0x378) and 8 more
- **Named before** (a naming session's reading, low): `CXiActor_IsA4InRange25To34`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API undefined4 __thiscall CXiActor_IsA4InRange25To34(void * self_)
{
  if ((0x24 < *(int *)((int)self_ + 0xa4)) && (*(int *)((int)self_ + 0xa4) < 0x35)) {
    return 1;
  }
  return 0;
}
```

## `0x10082900` fn_10082900

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 225 (+0x384), `CXiSklFurnitureActor` slot 225 (+0x384), `CXiControlActor` slot 225 (+0x384), `XiFurnitureActor` slot 225 (+0x384), `XiSkeletonActor2` slot 225 (+0x384), `CXiSkeletonActor` slot 225 (+0x384), `XiModelActor` slot 225 (+0x384), `CXiMannequinActor` slot 225 (+0x384) and 8 more
- **Named before** (a naming session's reading, low): `CXiActor_IsA4InRange35To3C`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API undefined4 __thiscall CXiActor_IsA4InRange35To3C(void * self_)
{
  if ((0x34 < *(int *)((int)self_ + 0xa4)) && (*(int *)((int)self_ + 0xa4) < 0x3d)) {
    return 1;
  }
  return 0;
}
```

## `0x10083750` fn_10083750

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetViewMatrixPtr`, `Camera_GetField50` x2, `Camera_GetMatrix154Ptr`, `Vec3_Copy`, `Unit10026C70_ReturnThis` (low), `Unit10026C70_Nop` (low), `Mat4_Copy`, `Mat4_PostMultiply`, `CXiActor_ComputeDrawState` (low), `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158D90`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `fn_10158D90` → `fn_10083750`
- **Named callers' callers:** `Cursor_HasTargetHover`
- **Named callees' callees:** `CYyObject_IsKindOf`, `ActorHandle_Resolve`, `CXiActor::IsModelMissingOrFlag2000`, `CXiActor::GetModelField170`, `CXiActor::IsModelMissingOrSuppressed`, `Settings_GetValue`
- **How its callers call it** (their verified code):
  - in `fn_10158D90`: `piVar1 = orig::FUN_10083750((float *)((int)self_ + 0x8c),*(uint32_t *)((int)self_ + 0x60),`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10026C70_ReturnThis` (low): Returns its this pointer; a shared trivial method.
  - `Unit10026C70_Nop` (low): Does nothing; a shared empty function.
  - `CXiActor_ComputeDrawState` (low): Computes a draw-state byte for the actor from its model flags and settings.

Short callee `Camera_GetViewMatrixPtr`:
```cpp
XI_API int __cdecl Camera_GetViewMatrixPtr(void)
{
  return g_pCamera_as_s32 + 0x94;
}
```

Short callee `Camera_GetField50`:
```cpp
XI_API undefined4 __cdecl Camera_GetField50(void)
{
  return *(uint32_t *)(g_pCamera_as_s32 + 0x50);
}
```

Short callee `Camera_GetMatrix154Ptr`:
```cpp
XI_API int __cdecl Camera_GetMatrix154Ptr(void)
{
  return g_pCamera_as_s32 + 0x154;
}
```

Short callee `Vec3_Copy`:
```cpp
XI_API undefined4 __cdecl Vec3_Copy(undefined4 * param_1, undefined4 * param_2)
{
  uint32_t uVar1;
  
  *param_1 = *param_2;
  param_1[1] = param_2[1];
  uVar1 = param_2[2];
  param_1[2] = uVar1;
  return uVar1;
}
```

```cpp
XI_API int * __cdecl fn_10083750(float * param_1, uint32_t param_2, uint32_t param_3)
{
  long double fVar1;
  int iVar2;
  uint32_t *puVar3;
  int *self_;
  uint32_t uVar4;
  long double fVar5;
  uint64_t uVar6;
  char xi_frame[124] __attribute__((aligned(4)));
  float  &local_7c = *reinterpret_cast<float  *>(xi_frame + 0);
  float  &fStack_78 = *reinterpret_cast<float  *>(xi_frame + 4);
  int * &local_74 = *reinterpret_cast<int * *>(xi_frame + 8);
  int  &local_70 = *reinterpret_cast<int  *>(xi_frame + 12);
  uint32_t  (&local_6c)[3] = *reinterpret_cast<uint32_t  (*)[3]>(xi_frame + 16);
  float  &local_60 = *reinterpret_cast<float  *>(xi_frame + 28);
  float  &fStack_5c = *reinterpret_cast<float  *>(xi_frame + 32);
  float  &fStack_54 = *reinterpret_cast<float  *>(xi_frame + 40);
  float  &local_50 = *reinterpret_cast<float  *>(xi_frame + 44);
  float  &fStack_4c = *reinterpret_cast<float  *>(xi_frame + 48);
  uint8_t  (&local_40)[64] = *reinterpret_cast<uint8_t  (*)[64]>(xi_frame + 60);
  
  iVar2 = orig::Camera_GetField50_v1();
  if (iVar2 != 0) {
    iVar2 = orig::Camera_GetField50_v1();
    orig::Vec3_Copy_v1(local_6c,(uint32_t *)(iVar2 + 0x44));
    Unit10026C70_ReturnThis(local_40);
    puVar3 = (uint32_t *)orig::Camera_GetViewMatrixPtr_v1();
    orig::Mat4_Copy_v1(local_40,puVar3);
    iVar2 = orig::Camera_GetMatrix154Ptr_v1();
    orig::Mat4_PostMultiply_v1(local_40,iVar2);
    local_74 = (int *)0x0;
    local_7c = 1e+09f;
    local_70 = orig::FUN_101cc510();
    uVar6 = orig::FUN_10085240();
    self_ = (int *)uVar6;
    if (self_ != (int *)0x0) {
      do {
        uVar4 = CXiActor_ComputeDrawState(self_,local_70,param_2,param_3,
                             ((uint32_t)(((uint32_t)((uint32_t)(param_2 >> 0x11)) << 8) | (uint8_t)(~(uint8_t)(param_2 >> 9)))) & 0xffffff01);
        if (((char)uVar4 != '\0') &&
           (uVar4 = ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*self_ + 0x3d0))))((void *)(self_),&local_60,&local_50,local_40),
           (char)uVar4 != '\0')) {
          fVar5 = (long double)local_60;
          fVar1 = (long double)*param_1;
          param_1[3] = fStack_54;
          if ((fVar5 <= fVar1) &&
             ((long double)local_50 >= (long double)*param_1)) {
            fVar5 = (long double)fStack_5c;
            fVar1 = (long double)param_1[1];
            if ((((fVar5 <= fVar1) &&
                 ((long double)fStack_4c >= (long double)param_1[1])) &&
                ((long double)local_7c > (long double)fStack_54)) &&
               ((((!NAN((long double)fStack_54) && !NAN((long double)0.0f)) &&
                  (long double)0.0f < (long double)fStack_54 &&
                 (uVar4 = ((int (__thiscall *)(void *, XI_ANY, XI_ANY))((**(code **)(*self_ + 0x3d4))))((void *)(self_),param_1,&fStack_78),
                 (char)uVar4 == '\x01')) &&
                ((long double)local_7c > (long double)fStack_78)))) {
              local_7c = fStack_78;
              local_74 = self_;
            }
          }
        }
        uVar6 = orig::FUN_10085270(self_);
        self_ = (int *)uVar6;
      } while (self_ != (int *)0x0);
    }
    orig::nullsub_1();
    return local_74;
  }
  return (int *)0x0;
}
```

## `0x10083B40` fn_10083B40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetViewMatrixPtr`, `Camera_GetField2F4` (low) x2, `sub_10027600`, `Unit10026C70_ReturnThis` (low), `Unit10026C70_Nop` (low), `Mat4_Copy`, `Mat4_TransformPoint` (medium) x2, `CXiActor_ComputeDrawState` (low), `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158C40`, `fn_10158CA0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `fn_10158C40` → `fn_10083B40`
- **Named callees' callees:** `CYyObject_IsKindOf`, `ActorHandle_Resolve`, `CXiActor::IsModelMissingOrFlag2000`, `CXiActor::GetModelField170`, `CXiActor::IsModelMissingOrSuppressed`, `Settings_GetValue`
- **How its callers call it** (their verified code):
  - in `fn_10158C40`: `piVar2 = orig::FUN_10083b40(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
  - in `fn_10158CA0`: `piVar3 = orig::FUN_10083b40(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
  - in `fn_10158CA0`: `piVar2 = orig::FUN_10083b40(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Camera_GetField2F4` (low): Returns the float stored at offset 0x2F4 of the camera.
  - `Unit10026C70_ReturnThis` (low): Returns its this pointer; a shared trivial method.
  - `Unit10026C70_Nop` (low): Does nothing; a shared empty function.
  - `Mat4_TransformPoint` (medium): Transforms a 3-component point by the 4x4 matrix at this, translation included.
  - `CXiActor_ComputeDrawState` (low): Computes a draw-state byte for the actor from its model flags and settings.

Short callee `Camera_GetViewMatrixPtr`:
```cpp
XI_API int __cdecl Camera_GetViewMatrixPtr(void)
{
  return g_pCamera_as_s32 + 0x94;
}
```

Short callee `Camera_GetField2F4` (low):
```cpp
XI_API long double __cdecl Camera_GetField2F4(void)
{
  return (long double)*(float *)(g_pCamera_as_s32 + 0x2f4);
}
```

Short callee `Unit10026C70_ReturnThis` (low):
```cpp
XI_API undefined4 __thiscall Unit10026C70_ReturnThis(void * self_)
{
  return self_;
}
```

Short callee `Unit10026C70_Nop` (low):
```cpp
XI_API void __cdecl Unit10026C70_Nop(void)
{
  return;
}
```

```cpp
XI_API int * __cdecl fn_10083B40(int * param_1, uint32_t param_2, float param_3)
{
  long double fVar1;
  float fVar2;
  uint32_t *puVar3;
  float *pfVar4;
  int iVar5;
  int *self_;
  uint32_t uVar6;
  uint8_t bVar7;
  long double fVar8;
  long double fVar9;
  uint64_t uVar10;
  char xi_frame[108] __attribute__((aligned(4)));
  float  &fStack_6c = *reinterpret_cast<float  *>(xi_frame + 0);
  float  &fStack_68 = *reinterpret_cast<float  *>(xi_frame + 4);
  int * &piStack_64 = *reinterpret_cast<int * *>(xi_frame + 8);
  float  (&afStack_60)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 12);
  float  &fStack_58 = *reinterpret_cast<float  *>(xi_frame + 20);
  float  (&afStack_50)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 28);
  float  &fStack_48 = *reinterpret_cast<float  *>(xi_frame + 36);
  uint8_t  (&local_40)[64] = *reinterpret_cast<uint8_t  (*)[64]>(xi_frame + 44);
  
  fVar2 = param_3;
  Unit10026C70_ReturnThis(local_40);
  puVar3 = (uint32_t *)orig::Camera_GetViewMatrixPtr_v1();
  orig::Mat4_Copy_v1(local_40,puVar3);
  pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x1bc))))((void *)(param_1));
  Mat4_TransformPoint(local_40,afStack_50,pfVar4);
  bVar7 = NAN((long double)fStack_48) || NAN((long double)0.0f);
  fVar8 = (long double)fStack_48;
  if ((bVar7 || (long double)fStack_48 < (long double)0.0f) != bVar7) {
    fVar8 = -fVar8;
  }
  fStack_6c = (float)fVar8;
  bVar7 = NAN(fVar8) || NAN((long double)1.0f);
  if ((bVar7 || fVar8 < (long double)1.0f) != bVar7) {
    fStack_6c = 1.0f;
  }
  fVar8 = Camera_GetField2F4();
  piStack_64 = (int *)0x0;
  fStack_68 = 3.6e+03f;
  fVar1 = (float)(((long double)afStack_50[0] / (long double)fStack_6c) * fVar8);
  iVar5 = orig::FUN_101cc510();
  uVar10 = orig::FUN_10085240();
  self_ = (int *)uVar10;
  while (self_ != (int *)0x0) {
    if ((self_ != param_1) &&
       (uVar6 = CXiActor_ComputeDrawState(self_,iVar5,param_2,xi_castbits<uint32_t>(fVar2),1), (char)uVar6 != '\0')) {
      pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*self_ + 0x1bc))))((void *)(self_));
      Mat4_TransformPoint(local_40,afStack_60,pfVar4);
      fVar8 = orig::FUN_10027600(afStack_60);
      bVar7 = NAN(fVar8) || NAN((long double)5e+01f);
      if ((bVar7 || fVar8 < (long double)5e+01f) != bVar7) {
        bVar7 = NAN((long double)fStack_58) || NAN((long double)0.0f);
        if ((bVar7 || (long double)fStack_58 < (long double)0.0f) == bVar7) {
          param_3 = fStack_58;
        }
        else {
          param_3 = (float)-(long double)fStack_58;
        }
        bVar7 = NAN((long double)param_3) || NAN((long double)1.0f);
        if ((bVar7 || (long double)param_3 < (long double)1.0f) != bVar7) {
          param_3 = 1.0f;
        }
        fVar9 = Camera_GetField2F4();
        fVar9 = ((long double)afStack_60[0] / (long double)param_3) * fVar9;
        fVar8 = (long double)fVar1;
        bVar7 = NAN(fVar9) || NAN(fVar8);
        if (((bVar7 || fVar9 < fVar8) != bVar7) &&
           (fVar9 = (long double)fVar1 - fVar9, bVar7 = NAN(fVar9) || NAN((long double)fStack_68),
           (bVar7 || fVar9 < (long double)fStack_68) != bVar7)) {
          fStack_68 = (float)fVar9;
          piStack_64 = self_;
        }
      }
    }
    uVar10 = orig::FUN_10085270(self_);
    self_ = (int *)uVar10;
  }
  orig::nullsub_1();
  return piStack_64;
}
```

## `0x10083D00` fn_10083D00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetViewMatrixPtr`, `Camera_GetField2F4` (low) x2, `sub_10027600`, `Unit10026C70_ReturnThis` (low), `Unit10026C70_Nop` (low), `Mat4_Copy`, `Mat4_TransformPoint` (medium) x2, `CXiActor_ComputeDrawState` (low), `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158C40`, `fn_10158CA0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `fn_10158C40` → `fn_10083D00`
- **Named callees' callees:** `CYyObject_IsKindOf`, `ActorHandle_Resolve`, `CXiActor::IsModelMissingOrFlag2000`, `CXiActor::GetModelField170`, `CXiActor::IsModelMissingOrSuppressed`, `Settings_GetValue`
- **How its callers call it** (their verified code):
  - in `fn_10158C40`: `piVar2 = orig::FUN_10083d00(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
  - in `fn_10158CA0`: `piVar3 = orig::FUN_10083d00(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
  - in `fn_10158CA0`: `piVar2 = orig::FUN_10083d00(piVar2,*(uint32_t *)((int)self_ + 0x60),*(float *)((int)self_ + 100));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Camera_GetField2F4` (low): Returns the float stored at offset 0x2F4 of the camera.
  - `Unit10026C70_ReturnThis` (low): Returns its this pointer; a shared trivial method.
  - `Unit10026C70_Nop` (low): Does nothing; a shared empty function.
  - `Mat4_TransformPoint` (medium): Transforms a 3-component point by the 4x4 matrix at this, translation included.
  - `CXiActor_ComputeDrawState` (low): Computes a draw-state byte for the actor from its model flags and settings.

Short callee `Camera_GetViewMatrixPtr`:
```cpp
XI_API int __cdecl Camera_GetViewMatrixPtr(void)
{
  return g_pCamera_as_s32 + 0x94;
}
```

Short callee `Camera_GetField2F4` (low):
```cpp
XI_API long double __cdecl Camera_GetField2F4(void)
{
  return (long double)*(float *)(g_pCamera_as_s32 + 0x2f4);
}
```

Short callee `Unit10026C70_ReturnThis` (low):
```cpp
XI_API undefined4 __thiscall Unit10026C70_ReturnThis(void * self_)
{
  return self_;
}
```

Short callee `Unit10026C70_Nop` (low):
```cpp
XI_API void __cdecl Unit10026C70_Nop(void)
{
  return;
}
```

```cpp
XI_API int * __cdecl fn_10083D00(int * param_1, uint32_t param_2, float param_3)
{
  long double fVar1;
  float fVar2;
  uint32_t *puVar3;
  float *pfVar4;
  int iVar5;
  int *self_;
  uint32_t uVar6;
  uint8_t bVar7;
  long double fVar8;
  long double fVar9;
  uint64_t uVar10;
  char xi_frame[108] __attribute__((aligned(4)));
  float  &fStack_6c = *reinterpret_cast<float  *>(xi_frame + 0);
  float  &fStack_68 = *reinterpret_cast<float  *>(xi_frame + 4);
  int * &piStack_64 = *reinterpret_cast<int * *>(xi_frame + 8);
  float  (&afStack_60)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 12);
  float  &fStack_58 = *reinterpret_cast<float  *>(xi_frame + 20);
  float  (&afStack_50)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 28);
  float  &fStack_48 = *reinterpret_cast<float  *>(xi_frame + 36);
  uint8_t  (&local_40)[64] = *reinterpret_cast<uint8_t  (*)[64]>(xi_frame + 44);
  
  fVar2 = param_3;
  Unit10026C70_ReturnThis(local_40);
  puVar3 = (uint32_t *)orig::Camera_GetViewMatrixPtr_v1();
  orig::Mat4_Copy_v1(local_40,puVar3);
  pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x1bc))))((void *)(param_1));
  Mat4_TransformPoint(local_40,afStack_50,pfVar4);
  bVar7 = NAN((long double)fStack_48) || NAN((long double)0.0f);
  fVar8 = (long double)fStack_48;
  if ((bVar7 || (long double)fStack_48 < (long double)0.0f) != bVar7) {
    fVar8 = -fVar8;
  }
  fStack_6c = (float)fVar8;
  bVar7 = NAN(fVar8) || NAN((long double)1.0f);
  if ((bVar7 || fVar8 < (long double)1.0f) != bVar7) {
    fStack_6c = 1.0f;
  }
  fVar8 = Camera_GetField2F4();
  piStack_64 = (int *)0x0;
  fStack_68 = 3.6e+03f;
  fVar1 = (float)(((long double)afStack_50[0] / (long double)fStack_6c) * fVar8);
  iVar5 = orig::FUN_101cc510();
  uVar10 = orig::FUN_10085240();
  self_ = (int *)uVar10;
  while (self_ != (int *)0x0) {
    if ((self_ != param_1) &&
       (uVar6 = CXiActor_ComputeDrawState(self_,iVar5,param_2,xi_castbits<uint32_t>(fVar2),1), (char)uVar6 != '\0')) {
      pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*self_ + 0x1bc))))((void *)(self_));
      Mat4_TransformPoint(local_40,afStack_60,pfVar4);
      fVar8 = orig::FUN_10027600(afStack_60);
      bVar7 = NAN(fVar8) || NAN((long double)5e+01f);
      if ((bVar7 || fVar8 < (long double)5e+01f) != bVar7) {
        bVar7 = NAN((long double)fStack_58) || NAN((long double)0.0f);
        if ((bVar7 || (long double)fStack_58 < (long double)0.0f) == bVar7) {
          param_3 = fStack_58;
        }
        else {
          param_3 = (float)-(long double)fStack_58;
        }
        bVar7 = NAN((long double)param_3) || NAN((long double)1.0f);
        if ((bVar7 || (long double)param_3 < (long double)1.0f) != bVar7) {
          param_3 = 1.0f;
        }
        fVar9 = Camera_GetField2F4();
        fVar9 = ((long double)afStack_60[0] / (long double)param_3) * fVar9;
        fVar8 = (long double)fVar1;
        if ((fVar9 > fVar8) &&
           (fVar9 = fVar9 - (long double)fVar1, bVar7 = NAN(fVar9) || NAN((long double)fStack_68),
           (bVar7 || fVar9 < (long double)fStack_68) != bVar7)) {
          fStack_68 = (float)fVar9;
          piStack_64 = self_;
        }
      }
    }
    uVar10 = orig::FUN_10085270(self_);
    self_ = (int *)uVar10;
  }
  orig::nullsub_1();
  return piStack_64;
}
```

## `0x10083EC0` fn_10083EC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor_FindNearestToTarget` (medium)
- **Called by:** `sub_1007A600`, `sub_1007A6C0`, `sub_101587F0`, `sub_101AE8B0`
- **Named before** (a naming session's reading, low): `CXiActor_FindNearestToTarget_Mode0`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `CXiActor_FindNearestToTarget_Mode0`
- **Named callers' callers:** `Target_IsUsable`
- **Named callees' callees:** `Unit10026C70_ReturnThis`, `Unit10026C70_Nop`, `Mat4_SetRow3`, `Mat4_TransformPoint`, `Mat4_InvertOrthonormal`, `ActorHandle_Resolve`, `CXiActor_GetField78Of70`, `CXiActor_ComputeDrawState`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiActor_FindNearestToTarget` (medium): Finds the actor nearest to the target actor, filtered by the mode argument.

```cpp
XI_API undefined4 __cdecl CXiActor_FindNearestToTarget_Mode0(undefined4 param_1, undefined4 param_2)
{
  int *piVar1;
  
  piVar1 = CXiActor_FindNearestToTarget(param_1,param_2,0);
  return piVar1;
}
```

## `0x10083EE0` fn_10083EE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor_FindNearestToTarget` (medium)
- **Called by:** `sub_1007A640`, `sub_101587F0`, `sub_101AE880`
- **Named before** (a naming session's reading, low): `CXiActor_FindNearestToTarget_Mode1`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `CXiActor_FindNearestToTarget_Mode1`
- **Named callers' callers:** `Target_IsUsable`
- **Named callees' callees:** `Unit10026C70_ReturnThis`, `Unit10026C70_Nop`, `Mat4_SetRow3`, `Mat4_TransformPoint`, `Mat4_InvertOrthonormal`, `ActorHandle_Resolve`, `CXiActor_GetField78Of70`, `CXiActor_ComputeDrawState`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiActor_FindNearestToTarget` (medium): Finds the actor nearest to the target actor, filtered by the mode argument.

```cpp
XI_API undefined4 __cdecl CXiActor_FindNearestToTarget_Mode1(uint32_t param_1, uint32_t param_2)
{
  int *piVar1;
  
  piVar1 = CXiActor_FindNearestToTarget(param_1,param_2,1);
  return piVar1;
}
```

## `0x10083F00` fn_10083F00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor_FindNearestToTarget` (medium)
- **Called by:** `sub_1007A680`, `sub_101587F0`
- **Named before** (a naming session's reading, low): `CXiActor_FindNearestToTarget_Mode2`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_101585F0` → `sub_101587F0` → `CXiActor_FindNearestToTarget_Mode2`
- **Named callers' callers:** `Target_IsUsable`
- **Named callees' callees:** `Unit10026C70_ReturnThis`, `Unit10026C70_Nop`, `Mat4_SetRow3`, `Mat4_TransformPoint`, `Mat4_InvertOrthonormal`, `ActorHandle_Resolve`, `CXiActor_GetField78Of70`, `CXiActor_ComputeDrawState`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiActor_FindNearestToTarget` (medium): Finds the actor nearest to the target actor, filtered by the mode argument.

```cpp
XI_API undefined4 __cdecl CXiActor_FindNearestToTarget_Mode2(uint32_t param_1, uint32_t param_2)
{
  int *piVar1;
  
  piVar1 = CXiActor_FindNearestToTarget(param_1,param_2,2);
  return piVar1;
}
```

## `0x10084100` fn_10084100

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100275D0`
- **Called by:** `sub_10152490`, `Unit1012AC80_IsWithinRadius` (medium)
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Target_Update` → `fn_10158480` → `fn_10159540` → `Unit1012AC80_IsWithinRadius` → `fn_10084100`
- **Named callees' callees:** `Vec3_Sub`
- **How its callers call it** (their verified code):
  - in `Unit1012AC80_IsWithinRadius` (medium): `uVar2 = orig::FUN_10084100((int)uVar3,iVar1,(float)((long double)param_1 * (long double)param_1));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit1012AC80_IsWithinRadius` (medium): Tests whether the target actor is within a radius of the entry.

```cpp
XI_API undefined4 __cdecl fn_10084100(int param_1, int param_2, float param_3)
{
  long double fVar1;
  uint8_t bVar2;
  long double fVar3;
  
  fVar3 = orig::FUN_100275d0((float *)(param_1 + 0x34),(float *)(param_2 + 0x34));
  fVar3 = fVar3 * fVar3;
  fVar1 = (long double)param_3;
  bVar2 = !(fVar3 >= fVar1);
  if (fVar3 > fVar1) {
    return (uint32_t)bVar2 << 8;
  }
  return (uint32_t)CONCAT11(bVar2,*(char *)(param_2 + 0xc0) != '\x02');
}
```

## `0x100849A0` fn_100849A0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 68 (+0x110), `CXiSklFurnitureActor` slot 68 (+0x110), `CXiControlActor` slot 68 (+0x110), `XiFurnitureActor` slot 68 (+0x110), `XiSkeletonActor2` slot 68 (+0x110), `CXiSkeletonActor` slot 68 (+0x110), `XiModelActor` slot 68 (+0x110), `CXiMannequinActor` slot 68 (+0x110) and 8 more

```cpp
XI_API long double __thiscall fn_100849A0(void * self_)
{
  if (*(int *)((int)self_ + 0x70) != 0) {
    return (long double)*(float *)(*(int *)((int)self_ + 0x70) + 0x204);
  }
  return (long double)(-1.0f);
}
```

## `0x10084BA0` fn_10084BA0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 82 (+0x148), `CXiSklFurnitureActor` slot 82 (+0x148), `CXiControlActor` slot 82 (+0x148), `XiFurnitureActor` slot 82 (+0x148), `XiSkeletonActor2` slot 82 (+0x148), `CXiSkeletonActor` slot 82 (+0x148), `XiModelActor` slot 82 (+0x148), `CXiMannequinActor` slot 82 (+0x148) and 8 more
- **Reads globals:** `0x1047D631`

```cpp
XI_API uint32_t __thiscall fn_10084BA0(void * self_)
{
  if (*(int *)((int)self_ + 0x70) != 0) {
    return *(uint32_t *)(*(int *)((int)self_ + 0x70) + 300) >> 0x11 & 1;
  }
  return (int)DAT_1047d631;
}
```

## `0x10084BC0` fn_10084BC0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 83 (+0x14C), `CXiSklFurnitureActor` slot 83 (+0x14C), `CXiControlActor` slot 83 (+0x14C), `XiFurnitureActor` slot 83 (+0x14C), `XiSkeletonActor2` slot 83 (+0x14C), `CXiSkeletonActor` slot 83 (+0x14C), `XiModelActor` slot 83 (+0x14C), `CXiMannequinActor` slot 83 (+0x14C) and 8 more

```cpp
XI_API undefined4 __thiscall fn_10084BC0(void * self_)
{
  int iVar1;
  
  iVar1 = *(int *)((int)self_ + 0x70);
  if (iVar1 != 0) {
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)iVar1 >> 8)) << 8) | (uint8_t)(*(short *)(iVar1 + 0x210) != 0)));
  }
  return 0;
}
```

## `0x10085010` fn_10085010

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 164 (+0x290), `CXiControlActor` slot 164 (+0x290), `XiLiftActor` slot 164 (+0x290), `CXiCollisionActor` slot 164 (+0x290), `XiDoorActor` slot 164 (+0x290), `CXiActor` slot 164 (+0x290), `CXiDollActor` slot 164 (+0x290)
- **Called by:** `sub_10047960`, `sub_10085E40`, `sub_100AC8E0`, `sub_100C4290`, `sub_100D0B50`, `fn_100D0D40`, `fn_100D0D70`
- **Named before** (a naming session's reading, low): `CXiActor_SetField0B4`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiSkeletonActor_LoadSkeletonResource` → `CXiSkeletonActor_LoadSkeletonSync` → `sub_100D0B50` → `CXiActor_SetField0B4`
- **Named callers' callers:** `CMoSchedularTask_Interpret`, `CXiSkeletonActor_LoadSkeletonSync`

```cpp
XI_API void __thiscall CXiActor_SetField0B4(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xb4) = param_1;
  return;
}
```

## `0x10085500` fn_10085500

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CMoAttachments::SetCaster`, `CMoAttachments::SetTarget`
- **Named before** (a naming session's reading, low): `CXiActor_GetField88Bit3`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `sub_100DAF30` → `sub_1008ABE0` → `AfterimageTask_Execute` → `Scheduler_Execute` → `Scheduler_CreateTask` → `AfterimageTask_SetCaster` → `CMoAttachments::SetCaster` → `CXiActor_GetField88Bit3`
- **Named callers' callers:** `CMoGeneratorClone_FixupClonedPointers`, `CMoGeneratorClone_CreateInstance`, `CMoSchedularTask_Interpret`, `AfterimageTask_SetCaster`, `AfterimageTask_SetTarget`, `Unit10070F90_Create`, `CXiActor_AddTimerTask`
- **How its callers call it** (their verified code):
  - in `CMoAttachments::SetCaster`: `uVar1 = CXiActor_GetField88Bit3((void *)param_1);`
  - in `CMoAttachments::SetTarget`: `uVar2 = CXiActor_GetField88Bit3((void *)param_1);`

```cpp
XI_API uint32_t __thiscall CXiActor_GetField88Bit3(void * self_)
{
  return *(uint32_t *)((int)self_ + 0x88) >> 3 & 1;
}
```

## `0x10085BF0` fn_10085BF0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CXiSkeletonActor_OnMove`
- **Named before** (a naming session's reading, low): `CXiActor_SetField0BC`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API void __thiscall CXiActor_SetField0BC(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xbc) = param_1;
  return;
}
```
