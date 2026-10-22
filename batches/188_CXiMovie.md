# Batch 188_CXiMovie: 5 functions of `CXiMovie`

Name these functions: see `batches/README.md`. Write `data/names/188_CXiMovie.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1021D3D0` fn_1021D3D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Mem_AllocDefault`, `__attribute__`, `fn_1021D7F0`, `fn_1021D930`, `sub_10311F30`, `sub_10311FDE`
- **Called by:** `sub_10018850`
- **Writes globals:** `0x10666E7C`

```cpp
XI_API undefined4 __cdecl fn_1021D3D0(undefined4 param_1)
{
  int iVar1;
  void *self_;
  uint32_t uVar2;
  
  iVar1 = orig::FUN_10311fde(param_1,&DAT_1034f2d0);
  if (iVar1 != 0) {
    orig::FUN_10311f30(iVar1);
    self_ = (void *)orig::Mem_AllocDefault_v1(0x28);
    if (self_ == (void *)0x0) {
      g_pMovie_as_v1 = (void *)0x0;
    }
    else {
      g_pMovie_as_v1 = orig::FUN_1021d7f0(self_);
      if (g_pMovie_as_v1 != (void *)0x0) {
        uVar2 = orig::FUN_1021d930(g_pMovie_as_v1,param_1);
        if ((char)uVar2 == '\0') {
          uVar2 = orig::FUN_1021d460(uVar2);
          return uVar2 & 0xffffff00;
        }
        return ((uint32_t)(((uint32_t)((int32_t)(uVar2 >> 8)) << 8) | (uint8_t)(1)));
      }
    }
  }
  return 0;
}
```

## `0x1021D440` fn_1021D440

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`

```cpp
XI_API undefined4 __cdecl fn_1021D440(void)
{
  uint32_t xi_entry_eax = 0;
  uint32_t uVar1;
  
  if (g_pMovie != 0) {
    uVar1 = orig::FUN_1021d960();
    return uVar1;
  }
  return xi_entry_eax & 0xffffff00;
}
```

## `0x1021D490` fn_1021D490

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`

```cpp
XI_API undefined4 __cdecl fn_1021D490(void)
{
  if (g_pMovie != 0) {
    return *(uint32_t *)(g_pMovie + 4);
  }
  return 0;
}
```

## `0x1021D4A0` fn_1021D4A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`

```cpp
XI_API long double __cdecl fn_1021D4A0(void)
{
  if (g_pMovie != 0) {
    return (long double)*(double *)(g_pMovie + 8);
  }
  return (long double)0.0f;
}
```

## `0x1021D7F0` fn_1021D7F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1021D3D0`

```cpp
XI_API void * __thiscall fn_1021D7F0(void * self_)
{
  int *piVar1;
  
  *(uint32_t *)((int)self_ + 0x10) = 0;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  *(uint32_t *)((int)self_ + 0x1c) = 0;
  *(uint8_t ***)self_ = &PTR_CXiMovie_GetRuntimeClass_1033bdd0;
  *(uint32_t *)((int)self_ + 4) = 0;
  piVar1 = *(int **)((int)self_ + 0x10);
  if (piVar1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
  }
  *(uint32_t *)((int)self_ + 0x10) = 0;
  piVar1 = *(int **)((int)self_ + 0x14);
  if (piVar1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
  }
  *(uint32_t *)((int)self_ + 0x14) = 0;
  piVar1 = *(int **)((int)self_ + 0x18);
  if (piVar1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
  }
  *(uint32_t *)((int)self_ + 0x18) = 0;
  piVar1 = *(int **)((int)self_ + 0x1c);
  if (piVar1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
  }
  *(uint32_t *)((int)self_ + 0x1c) = 0;
  *(uint32_t *)((int)self_ + 0x20) = 0;
  return self_;
}
```
