# Batch 007_CXiActor: 18 functions of `CXiActor`

Name these functions: see `batches/README.md`. Write `data/names/007_CXiActor.json`.
10 of them were sampled in a running frame (data/frame_set.json).

## `0x100827F0` fn_100827F0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 214 (+0x358), `CXiSklFurnitureActor` slot 214 (+0x358), `CXiControlActor` slot 214 (+0x358), `XiFurnitureActor` slot 214 (+0x358), `XiSkeletonActor2` slot 214 (+0x358), `CXiSkeletonActor` slot 214 (+0x358), `XiModelActor` slot 214 (+0x358), `CXiMannequinActor` slot 214 (+0x358) and 8 more

```cpp
XI_API undefined4 __thiscall fn_100827F0(void * self_)
{
  if ((0x53 < *(int *)((int)self_ + 0xa4)) && (*(int *)((int)self_ + 0xa4) < 0x55)) {
    return 1;
  }
  return 0;
}
```

## `0x10082860` fn_10082860

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiAtelActor` slot 219 (+0x36C), `CXiSklFurnitureActor` slot 219 (+0x36C), `CXiControlActor` slot 219 (+0x36C), `XiFurnitureActor` slot 219 (+0x36C), `XiSkeletonActor2` slot 219 (+0x36C), `CXiSkeletonActor` slot 219 (+0x36C), `XiModelActor` slot 219 (+0x36C), `CXiMannequinActor` slot 219 (+0x36C) and 8 more

```cpp
XI_API undefined4 __thiscall fn_10082860(void * self_)
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

```cpp
XI_API undefined4 __thiscall fn_100828B0(void * self_)
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

```cpp
XI_API undefined4 __thiscall fn_10082900(void * self_)
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
- **Calls:** `Camera_GetViewMatrixPtr`, `Camera_GetField50` x2, `Camera_GetMatrix154Ptr`, `Vec3_Copy`, `fn_10027990`, `fn_100279A0`, `Mat4_Copy`, `Mat4_PostMultiply`, `fn_10083430`, `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158D90`

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
    orig::FUN_10027990(local_40);
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
        uVar4 = orig::FUN_10083430(self_,local_70,param_2,param_3,
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
- **Calls:** `Camera_GetViewMatrixPtr`, `fn_100152C0` x2, `sub_10027600`, `fn_10027990`, `fn_100279A0`, `Mat4_Copy`, `fn_10028230` x2, `fn_10083430`, `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158C40`, `fn_10158CA0`

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
  orig::FUN_10027990(local_40);
  puVar3 = (uint32_t *)orig::Camera_GetViewMatrixPtr_v1();
  orig::Mat4_Copy_v1(local_40,puVar3);
  pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x1bc))))((void *)(param_1));
  orig::FUN_10028230(local_40,afStack_50,pfVar4);
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
  fVar8 = orig::FUN_100152c0();
  piStack_64 = (int *)0x0;
  fStack_68 = 3.6e+03f;
  fVar1 = (float)(((long double)afStack_50[0] / (long double)fStack_6c) * fVar8);
  iVar5 = orig::FUN_101cc510();
  uVar10 = orig::FUN_10085240();
  self_ = (int *)uVar10;
  while (self_ != (int *)0x0) {
    if ((self_ != param_1) &&
       (uVar6 = orig::FUN_10083430(self_,iVar5,param_2,xi_castbits<uint32_t>(fVar2),1), (char)uVar6 != '\0')) {
      pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*self_ + 0x1bc))))((void *)(self_));
      orig::FUN_10028230(local_40,afStack_60,pfVar4);
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
        fVar9 = orig::FUN_100152c0();
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
- **Calls:** `Camera_GetViewMatrixPtr`, `fn_100152C0` x2, `sub_10027600`, `fn_10027990`, `fn_100279A0`, `Mat4_Copy`, `fn_10028230` x2, `fn_10083430`, `sub_10085240`, `sub_10085270`, `fn_101CC510`
- **Called by:** `fn_10158C40`, `fn_10158CA0`

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
  orig::FUN_10027990(local_40);
  puVar3 = (uint32_t *)orig::Camera_GetViewMatrixPtr_v1();
  orig::Mat4_Copy_v1(local_40,puVar3);
  pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x1bc))))((void *)(param_1));
  orig::FUN_10028230(local_40,afStack_50,pfVar4);
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
  fVar8 = orig::FUN_100152c0();
  piStack_64 = (int *)0x0;
  fStack_68 = 3.6e+03f;
  fVar1 = (float)(((long double)afStack_50[0] / (long double)fStack_6c) * fVar8);
  iVar5 = orig::FUN_101cc510();
  uVar10 = orig::FUN_10085240();
  self_ = (int *)uVar10;
  while (self_ != (int *)0x0) {
    if ((self_ != param_1) &&
       (uVar6 = orig::FUN_10083430(self_,iVar5,param_2,xi_castbits<uint32_t>(fVar2),1), (char)uVar6 != '\0')) {
      pfVar4 = (float *)((int (__thiscall *)(void *))((**(code **)(*self_ + 0x1bc))))((void *)(self_));
      orig::FUN_10028230(local_40,afStack_60,pfVar4);
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
        fVar9 = orig::FUN_100152c0();
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
- **Calls:** `fn_10083F20`
- **Called by:** `sub_1007A600`, `sub_1007A6C0`, `sub_101587F0`, `sub_101AE8B0`

```cpp
XI_API undefined4 __cdecl fn_10083EC0(undefined4 param_1, undefined4 param_2)
{
  int *piVar1;
  
  piVar1 = orig::FUN_10083f20(param_1,param_2,0);
  return piVar1;
}
```

## `0x10083EE0` fn_10083EE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10083F20`
- **Called by:** `sub_1007A640`, `sub_101587F0`, `sub_101AE880`

```cpp
XI_API undefined4 __cdecl fn_10083EE0(uint32_t param_1, uint32_t param_2)
{
  int *piVar1;
  
  piVar1 = orig::FUN_10083f20(param_1,param_2,1);
  return piVar1;
}
```

## `0x10083F00` fn_10083F00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10083F20`
- **Called by:** `sub_1007A680`, `sub_101587F0`

```cpp
XI_API undefined4 __cdecl fn_10083F00(uint32_t param_1, uint32_t param_2)
{
  int *piVar1;
  
  piVar1 = orig::FUN_10083f20(param_1,param_2,2);
  return piVar1;
}
```

## `0x10083F20` fn_10083F20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10027530`, `fn_10027990`, `fn_100279A0`, `sub_100279B0`, `sub_10027BD0`, `Mat4_SetRow3`, `fn_10028230`, `Mat4_InvertOrthonormal`, `ActorHandle_Resolve`, `fn_10082FA0` x2, `fn_10083430`, `sub_10085240`, `sub_10085270`, `fn_1008E8D0`, `__attribute__`, `fn_101CC510`
- **Called by:** `fn_10083EC0`, `fn_10083EE0`, `fn_10083F00`

```cpp
XI_API int * __cdecl fn_10083F20(uint32_t param_1, uint32_t param_2, int param_3)
{
  uint8_t bVar1;
  void *self_;
  int iVar2;
  char cVar3;
  int *piVar4;
  int iVar5;
  uint32_t *puVar6;
  uint32_t uVar7;
  uint32_t uVar8;
  uint32_t uVar9;
  float *pfVar10;
  char cVar11;
  uint8_t bVar12;
  long double fVar13;
  uint64_t uVar14;
  char xi_frame[88] __attribute__((aligned(4)));
  float  &fStack_58 = *reinterpret_cast<float  *>(xi_frame + 0);
  int * &piStack_54 = *reinterpret_cast<int * *>(xi_frame + 4);
  float  (&afStack_4c)[3] = *reinterpret_cast<float  (*)[3]>(xi_frame + 12);
  uint8_t  (&local_40)[64] = *reinterpret_cast<uint8_t  (*)[64]>(xi_frame + 24);
  
  uVar14 = orig::ActorHandle_Resolve_v1(&g_targetActorHandle_as_v0);
  piVar4 = (int *)uVar14;
  if (piVar4 == (int *)0x0) {
    return (int *)0x0;
  }
  orig::FUN_10027990(local_40);
  orig::Mat4_SetIdentity_v1(local_40);
  iVar5 = (**(code **)(*piVar4 + 0x1c0))();
  orig::Mat4_PostRotateY_v1(local_40,*(float *)(iVar5 + 4));
  puVar6 = (uint32_t *)(**(code **)(*piVar4 + 0x1bc))();
  orig::Mat4_SetRow3_v1(local_40,puVar6);
  orig::Mat4_InvertOrthonormal(local_40);
  piStack_54 = (int *)0x0;
  fStack_58 = 2.5e+03f;
  iVar5 = orig::FUN_101cc510();
  self_ = (void *)piVar4[0x1c];
  uVar14 = orig::FUN_10085240();
  piVar4 = (int *)uVar14;
  do {
    if (piVar4 == (int *)0x0) {
      orig::nullsub_1();
      return piStack_54;
    }
    iVar2 = piVar4[0x1c];
    uVar7 = orig::FUN_10082fa0(piVar4);
    cVar11 = (uVar7 & 0xff000000) == 0;
    uVar7 = orig::FUN_10082fa0(piVar4);
    cVar3 = '\0';
    if ((self_ != (void *)0x0) && (iVar2 != 0)) {
      if (((*(uint32_t *)((int)self_ + 0x1d0) & 0x400) == 0) ||
         (((*(uint32_t *)(iVar2 + 0x1d0) & 0x400) == 0 ||
          (*(char *)((int)self_ + 0x1ff) != *(char *)(iVar2 + 0x1ff))))) {
        bVar1 = *(uint8_t *)((int)self_ + 0x1ff);
        uVar9 = ((uint32_t)(((uint32_t)(4) << 8) | (uint8_t)(bVar1)));
        if ((7 < bVar1) && (bVar1 < 10)) {
          bVar1 = *(uint8_t *)(iVar2 + 0x1ff);
          uVar9 = ((uint32_t)(((uint32_t)(4) << 8) | (uint8_t)(bVar1)));
          if ((7 < bVar1) && (bVar1 < 10)) {
            uVar8 = orig::FUN_1008e8d0(self_,iVar2);
            cVar3 = (char)uVar8;
            goto LAB_10084048;
          }
        }
        if (*(short *)(iVar2 + 0x210) == 0) {
          uVar9 = orig::FUN_101cc220(iVar2,uVar9);
          cVar3 = (char)uVar9;
        }
        else {
          cVar3 = '\0';
        }
      }
      else {
        cVar3 = '\0';
      }
    }
LAB_10084048:
    if (((((param_3 != 0) && (cVar11 = (uVar7 & 0xff000000) != 0, param_3 != 1)) &&
         (cVar11 = cVar3, param_3 != 2)) || (cVar11 != '\0')) &&
       (uVar9 = orig::FUN_10083430(piVar4,iVar5,param_1,param_2,1), (char)uVar9 != '\0')) {
      pfVar10 = (float *)((int (__thiscall *)(void *))((**(code **)(*piVar4 + 0x1bc))))((void *)(piVar4));
      orig::FUN_10028230(local_40,afStack_4c,pfVar10);
      bVar12 = NAN((long double)afStack_4c[0]) || NAN((long double)0.0f);
      if (((bVar12 || (long double)afStack_4c[0] < (long double)0.0f) == bVar12) &&
         (fVar13 = orig::FUN_10027530(afStack_4c,afStack_4c),
         bVar12 = NAN(fVar13) || NAN((long double)fStack_58),
         (bVar12 || fVar13 < (long double)fStack_58) != bVar12)) {
        fStack_58 = (float)fVar13;
        piStack_54 = piVar4;
      }
    }
    uVar14 = orig::FUN_10085270(piVar4);
    piVar4 = (int *)uVar14;
  } while( true );
}
```

## `0x10084100` fn_10084100

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100275D0`
- **Called by:** `sub_10152490`, `fn_101598C0`

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
- **Called by:** `sub_10047960`, `sub_10085E40`, `sub_100AC8E0`, `sub_100C4290`, `sub_100D0B50`, `fn_100D0D40`, `__attribute__`

```cpp
XI_API void __thiscall fn_10085010(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xb4) = param_1;
  return;
}
```

## `0x10085500` fn_10085500

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CMoAttachments::SetCaster`, `CMoAttachments::SetTarget`

```cpp
XI_API uint32_t __thiscall fn_10085500(void * self_)
{
  return *(uint32_t *)((int)self_ + 0x88) >> 3 & 1;
}
```

## `0x10085BF0` fn_10085BF0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CXiSkeletonActor_OnMove`

```cpp
XI_API void __thiscall fn_10085BF0(void * self_, undefined4 param_1)
{
  *(uint32_t *)((int)self_ + 0xbc) = param_1;
  return;
}
```
