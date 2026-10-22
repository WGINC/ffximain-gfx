# Batch 064_XiZone: 8 functions of `XiZone`

Name these functions: see `batches/README.md`. Write `data/names/064_XiZone.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x101889E0` fn_101889E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101889E0`, `fn_1018A320`
- **Called by:** `sub_100940F0`, `fn_101889E0`, `fn_1018B5E0`, `sub_1018C4D0`

```cpp
XI_API void __thiscall fn_101889E0(void * self_)
{
  void *this_00;
  
  if (*(void **)((int)self_ + 4) != (void *)0x0) {
    orig::FUN_101889e0_v1(*(void **)((int)self_ + 4));
  }
  for (this_00 = *(void **)((int)self_ + 0x10); this_00 != (void *)0x0;
      this_00 = *(void **)((int)this_00 + 8)) {
    orig::FUN_1018a320(this_00);
  }
  return;
}
```

## `0x10188A10` fn_10188A10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `thunk_sub_10028AA0`, `fn_10188A10`, `fn_1018ABF0`
- **Called by:** `fn_10187030`, `fn_10187470`, `fn_10188A10`, `sub_1018B010`, `fn_1018B620`

```cpp
XI_API void __thiscall fn_10188A10(void * self_)
{
  void *pvVar1;
  void *this_00;
  
  if (*(void **)((int)self_ + 4) != (void *)0x0) {
    orig::FUN_10188a10_v1(*(void **)((int)self_ + 4));
  }
  this_00 = *(void **)((int)self_ + 0x10);
  while (this_00 != (void *)0x0) {
    pvVar1 = *(void **)((int)this_00 + 8);
    orig::FUN_1018abf0(this_00);
    orig::thunk_sub_10028AA0_v1((int)this_00);
    this_00 = pvVar1;
  }
  *(uint32_t *)((int)self_ + 0x10) = 0;
  return;
}
```

## `0x10188B10` fn_10188B10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1004A980`, `sub_1004E8D0`
- **Reads globals:** `0x1065CB04`

```cpp
XI_API uint32_t __thiscall fn_10188B10(void * self_)
{
  uint8_t *pbVar1;
  void *pvVar2;
  
  if ((*(int *)(g_pXiZone_as_s32 + 0x1b4) != 0xb6) && (*(int *)(g_pXiZone_as_s32 + 0x1b4) != 0x84)) {
    if (*(uint8_t **)((int)self_ + 0x18) != (uint8_t *)0x0) {
      return (uint8_t)~**(uint8_t **)((int)self_ + 0x18) & 1;
    }
    return 0;
  }
  pbVar1 = *(uint8_t **)((int)self_ + 0x18);
  pvVar2 = (void *)0x0;
  if (pbVar1 != (uint8_t *)0x0) {
    if ((*pbVar1 & 1) == 0) {
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)pbVar1 >> 8)) << 8) | (uint8_t)(1)));
    }
    pvVar2 = *(void **)(g_pXiZone_as_s32 + 0x1c4);
    if ((pvVar2 != (void *)0x0) && (self_ == pvVar2)) {
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)pvVar2 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  return (uint32_t)pvVar2 & 0xffffff00;
}
```

## `0x1018B5E0` fn_1018B5E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `__attribute__`, `__attribute__`, `fn_101889E0`
- **Called by:** `sub_10014390`, `sub_100940F0`

```cpp
XI_API void __thiscall fn_1018B5E0(void * self_, int param_1)
{
  uint32_t xi_eax;
  int xi_eax_00;
  
  xi_eax_00 = *(int *)((int)self_ + 0x1bc);
  if (xi_eax_00 != param_1) {
    *(int *)((int)self_ + 0x1bc) = param_1;
    do {
      xi_eax = orig::FUN_101889e0(self_,xi_eax_00);
      if (param_1 == 0) {
        xi_eax_00 = orig::FUN_101875e0(self_,xi_eax);
      }
      else {
        xi_eax_00 = orig::FUN_101875c0(self_,xi_eax);
      }
      self_ = *(void **)((int)self_ + 4);
    } while (self_ != (void *)0x0);
  }
  return;
}
```

## `0x1018B620` fn_1018B620

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10081A30`, `fn_10178F70`, `fn_10186DB0`, `fn_10187030`, `fn_10188A10`, `Time_Normalize`
- **Called by:** `sub_100B5A90`, `sub_1018B340`, `sub_1018B500`, `sub_10239700`, `fn_10245BB0`
- **Reads globals:** `0x104568FC`, `0x10631F24`

```cpp
XI_API void __thiscall fn_1018B620(void * self_)
{
  int iVar1;
  uint32_t xi_eax;
  
  orig::FUN_10081a30();
  orig::FUN_10187030(self_);
  orig::FUN_10186db0(g_pZoneMap);
  xi_eax = orig::FUN_10188a10(self_);
  orig::FUN_1018bc40((void *)((int)self_ + 0x1c8),-1,xi_eax);
  *(uint16_t *)((int)self_ + 0x1cc) = 0;
  orig::FUN_10178f70(g_pZoneMap);
  *(uint32_t *)((int)self_ + 0x1ac) = 0;
  iVar1 = g_pCamera_as_s32;
  if (g_pCamera_as_s32 != 0) {
    *(uint32_t *)(g_pCamera_as_s32 + 0x330) = 0x3f800000;
    *(uint8_t *)(iVar1 + 0x32e) = 0x80;
    *(uint8_t *)(iVar1 + 0x32d) = 0x80;
    *(uint8_t *)(iVar1 + 0x32c) = 0x80;
    *(uint8_t *)(iVar1 + 0x32f) = 0;
  }
  return;
}
```

## `0x1018B8B0` fn_1018B8B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036170`
- **Called by:** `XiZone_DrawScene`

```cpp
XI_API void __stdcall fn_1018B8B0(int param_1)
{
  orig::FUN_10036170(param_1);
  return;
}
```

## `0x1018B8F0` fn_1018B8F0

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10187020`, `Env_FogDistanceFactor`
- **Called by:** `sub_100496F0`, `sub_100AC5A0`, `CXiSkeletonActor_OnMove`, `Zone_RenderEnvironment`

```cpp
XI_API long double __thiscall fn_1018B8F0(void * self_, undefined4 param_2)
{
  uint8_t bVar1;
  long double fVar2;
  long double fVar3;
  undefined4 local_4;
  
  local_4 = 1e+02f;
  fVar2 = orig::FUN_101892b0(self_,param_2);
  if (*(int *)((int)self_ + 0x1c) != 0) {
    fVar3 = orig::FUN_10187020(self_);
    local_4 = (float)fVar3;
  }
  fVar2 = (long double)(float)fVar2 * (long double)local_4;
  if (!((long double)*(float *)((int)self_ + 0x1d0) > (long double)0.0f)) {
    fVar2 = (long double)local_4;
  }
  else {
    bVar1 = NAN(fVar2) || NAN((long double)*(float *)((int)self_ + 0x1d0));
    if ((bVar1 || fVar2 < (long double)*(float *)((int)self_ + 0x1d0)) != bVar1) {
      return (long double)*(float *)((int)self_ + 0x1d0);
    }
  }
  return fVar2;
}
```

## `0x1018B960` fn_1018B960

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 12`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `XiZone_FindSubArea`, `XiZone::GetDirLights`
- **Called by:** `YmElem_SetupDevice`

```cpp
XI_API void __thiscall fn_1018B960(void * self_, undefined4 param_1, undefined4 param_2, int param_3)
{
  void *this_00;
  
  this_00 = self_;
  if (param_3 != 0) {
    this_00 = (void *)orig::FUN_101889c0(param_3);
    if (this_00 == (void *)0x0) {
      this_00 = self_;
    }
  }
  orig::XiZone_GetDirLights_v1(this_00,param_1,param_2);
  return;
}
```
