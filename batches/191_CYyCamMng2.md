# Batch 191_CYyCamMng2: 4 functions of `CYyCamMng2`

Name these functions: see `batches/README.md`. Write `data/names/191_CYyCamMng2.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001E4D0` fn_1001E4D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetField50` x2
- **Called by:** `sub_1001EE60`, `fn_10220C20`

```cpp
XI_API long double __cdecl fn_1001E4D0(void)
{
  int iVar1;
  int iVar2;
  long double fVar3;
  
  iVar1 = orig::Camera_GetField50_v1();
  iVar2 = orig::Camera_GetField50_v1();
  fVar3 = xi_castbits<long double>(fpatan(-(long double)*(float *)(iVar2 + 0x2c),(long double)*(float *)(iVar1 + 0x24)));
  return fVar3;
}
```

## `0x1001E4F0` fn_1001E4F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10015290`, `fn_10026E50`, `Vec3_Copy` x4
- **Called by:** `fn_1001E590`, `sub_1018B500`

```cpp
XI_API undefined4 __thiscall fn_1001E4F0(void * self_)
{
  uint32_t *puVar1;
  
  puVar1 = (uint32_t *)((int)self_ + 0x44);
  orig::Vec3_Copy_v1(puVar1,(uint32_t *)&DAT_1035122c);
  orig::FUN_10015290(0x43af0000);
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x50),puVar1);
  *(float *)((int)self_ + 0x58) = (float)((long double)*(float *)((int)self_ + 0x58) + (long double)1.0f);
  *(uint32_t *)((int)self_ + 0x5c) = 0;
  orig::FUN_10026e50((float *)((int)self_ + 0xa8),0.0f,(-1.0f),0.0f);
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x6c),(uint32_t *)((int)self_ + 0x50));
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x60),puVar1);
  *(uint32_t *)((int)self_ + 0x78) = *(uint32_t *)((int)self_ + 0x5c);
  *(uint32_t *)((int)self_ + 0x84) = 0;
  *(uint32_t *)((int)self_ + 0x80) = 0;
  *(uint32_t *)((int)self_ + 0x7c) = 0;
  *(uint8_t *)((int)self_ + 0x88) = 0;
  *(uint8_t *)((int)self_ + 0x89) = 0;
  *(uint8_t *)((int)self_ + 0x8a) = 0;
  *(uint8_t *)((int)self_ + 0xf4) = 0;
  return *(uint32_t *)((int)self_ + 0x5c);
}
```

## `0x1001E590` fn_1001E590

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1001E4F0`
- **Called by:** `CYyCamMng2::CreateGlobal`, `fn_1001E600`

```cpp
XI_API undefined4 __thiscall fn_1001E590(void * self_)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_1001e4f0(self_);
  *(uint32_t *)((int)self_ + 0xb4) = 0x40400000;
  *(uint32_t *)((int)self_ + 0xb8) = 0x40c00000;
  return uVar1;
}
```

## `0x1001E600` fn_1001E600

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `__attribute__`, `fn_1001E590`, `fn_10027990`
- **Called by:** `CYyCamMng2::CreateGlobal`
- **Writes globals:** `0x10456D70`, `0x10456D74`, `0x10456D78`, `0x10456D7C`, `0x10456D80`, `0x10456D84`, `0x10456D88`, `0x10456D8C`, `0x10456D90`, `0x10456D94`, `0x10456D98`, `0x10456D9C`, `0x10456DA0`, `0x10456DA4`, `0x10456DA8`

```cpp
XI_API void * __thiscall fn_1001E600(void * self_)
{
  uint32_t xi_eax;
  
  *(uint8_t ***)self_ = &CYyObject_vtbl_ambiguous0_as_v0;
  orig::FUN_10027990((void *)((int)self_ + 4));
  *(uint8_t *)((int)self_ + 0xf0) = 0;
  *(uint8_t ***)self_ = &PTR_CYyCamMng2_GetRuntimeClass_1032a37c;
  xi_eax = orig::FUN_1001e590(self_);
  DAT_10456d70 = 0;
  DAT_10456d74 = 0;
  _DAT_10456d78 = 0;
  _DAT_10456d7c = 0;
  DAT_10456d80 = 0;
  DAT_10456d84 = 0;
  DAT_10456d88 = 0;
  DAT_10456d8c = 0;
  _DAT_10456d9c = 0;
  _DAT_10456da0 = 0;
  _DAT_10456da4 = 0;
  _DAT_10456d90 = 0;
  _DAT_10456d94 = 0;
  _DAT_10456d98 = 0;
  _DAT_10456da8 = 0;
  orig::FUN_1001e2f0(0,1,0,xi_eax);
  return self_;
}
```
