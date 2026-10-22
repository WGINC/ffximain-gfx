# Batch 186_CTsZoneMap: 5 functions of `CTsZoneMap`

Name these functions: see `batches/README.md`. Write `data/names/186_CTsZoneMap.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101773F0` fn_101773F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `YmGenerater_BuildAttachMatrix`

```cpp
XI_API undefined4 * __cdecl fn_101773F0(void)
{
  return &DAT_10631ed0;
}
```

## `0x101779B0` fn_101779B0

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10178900`, `fn_10178A20`

```cpp
XI_API undefined4 __thiscall fn_101779B0(void * self_, undefined4 * param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10173ae0(param_1,param_2);
  return uVar1;
}
```

## `0x10177D00` fn_10177D00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10027990`, `fn_10054FC0`, `sub_10074740`, `fn_10167440`, `fn_10167800`, `fn_1016FC40`, `fn_101718E0`, `fn_10177FA0`, `fn_101784E0`
- **Called by:** `App_InitScene`
- **Writes globals:** `0x10376EE8`, `0x10621C00`, `0x10621C70`, `0x10621C74`, `0x10621C78`, `0x10621C7C`, `0x10621C80`, `0x10631C88`, `0x10631F18`, `0x10632038`, `0x1065C4CC`, `0x1065C4D0`, `0x1065C4D4`, `0x1065C5D8`

```cpp
XI_API void * __thiscall fn_10177D00(void * self_)
{
  void *this_00;
  int iVar1;
  undefined4 *puVar2;
  
  orig::CMoTask_Constructor_v1(self_);
  iVar1 = 2;
  this_00 = (void *)((int)self_ + 0xa4);
  do {
    orig::FUN_1016fc40(this_00);
    this_00 = (void *)((int)this_00 + 0x78);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  orig::FUN_10167800((void *)((int)self_ + 0x178));
  orig::FUN_101718e0((void *)((int)self_ + 0x34330));
  orig::FUN_10027990((void *)((int)self_ + 0x343e8));
  orig::FUN_10167440((void *)((int)self_ + 0x34428));
  orig::FUN_10054fc0((void *)((int)self_ + 0x3959c));
  *(uint8_t ***)self_ = &CTsZoneMap_vtbl_as_v0;
  *(uint16_t *)((int)self_ + 0x34) = 0;
  *(undefined4 *)((int)self_ + 0x58) = 0;
  *(undefined4 *)((int)self_ + 0x5c) = 0;
  puVar2 = (undefined4 *)((int)self_ + 0x60);
  for (iVar1 = 0x3c; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = (undefined4 *)((int)self_ + 0x37508);
  for (iVar1 = 0xe; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(undefined4 *)((int)self_ + 0x54) = 0;
  *(undefined4 *)((int)self_ + 0x150) = 1;
  *(undefined4 *)((int)self_ + 0x154) = 0;
  *(undefined4 *)((int)self_ + 0x39454) = 0;
  *(undefined4 *)((int)self_ + 0x39458) = 0;
  *(undefined4 *)((int)self_ + 0x39498) = 0;
  *(undefined4 *)((int)self_ + 0x3946c) = 0;
  *(undefined4 *)((int)self_ + 0x39470) = 0;
  *(undefined4 *)((int)self_ + 0x39474) = 0;
  *(undefined4 *)((int)self_ + 0x39478) = 0;
  *(undefined4 *)((int)self_ + 0x3947c) = 0;
  *(undefined4 *)((int)self_ + 0x39494) = 0;
  *(undefined4 *)((int)self_ + 0x158) = 0;
  *(undefined4 *)((int)self_ + 0x3944c) = 0;
  *(undefined4 *)((int)self_ + 0x39450) = 0;
  *(undefined4 *)((int)self_ + 0x38e54) = 0;
  *(undefined4 *)((int)self_ + 0x38e50) = 0;
  *(undefined4 *)((int)self_ + 0x38e4c) = 0;
  *(undefined4 *)((int)self_ + 0x38e48) = 0;
  *(undefined4 *)((int)self_ + 0x38e40) = 0;
  *(undefined4 *)((int)self_ + 0x38e3c) = 0;
  *(undefined4 *)((int)self_ + 0x38e38) = 0;
  *(undefined4 *)((int)self_ + 0x38e34) = 0;
  *(undefined4 *)((int)self_ + 0x38e2c) = 0;
  *(undefined4 *)((int)self_ + 0x38e28) = 0;
  *(undefined4 *)((int)self_ + 0x38e24) = 0;
  *(undefined4 *)((int)self_ + 0x38e20) = 0;
  *(undefined4 *)((int)self_ + 0x38e58) = 0x3f800000;
  *(undefined4 *)((int)self_ + 0x38e44) = 0x3f800000;
  *(undefined4 *)((int)self_ + 0x38e30) = 0x3f800000;
  *(undefined4 *)((int)self_ + 0x38e1c) = 0x3f800000;
  orig::FUN_101784e0();
  *(undefined4 *)((int)self_ + 0x38) = 0;
  *(undefined4 *)((int)self_ + 0x17c) = 0xffffffff;
  *(undefined4 *)((int)self_ + 0x3c) = 0;
  _DAT_1065c5d8 = 0;
  *(undefined4 *)((int)self_ + 0x40) = 0;
  *(undefined4 *)((int)self_ + 0x44) = 0;
  *(undefined4 *)((int)self_ + 0x48) = 0;
  *(undefined4 *)((int)self_ + 0x50) = 0;
  *(undefined4 *)((int)self_ + 0x174) = 0;
  *(undefined4 *)((int)self_ + 0x3942c) = 0;
  *(undefined4 *)((int)self_ + 0x39430) = 0;
  *(undefined4 *)((int)self_ + 0x39438) = 0;
  *(undefined4 *)((int)self_ + 0x3943c) = 0;
  *(undefined4 *)((int)self_ + 0x39440) = 0;
  *(undefined4 *)((int)self_ + 0x39444) = 0;
  *(undefined4 *)((int)self_ + 0x38d90) = 0;
  *(undefined4 *)((int)self_ + 0x39130) = 0;
  *(undefined4 *)((int)self_ + 0x39448) = 0;
  *(undefined4 *)((int)self_ + 0x39420) = 0;
  *(undefined4 *)((int)self_ + 0x343e4) = 0;
  DAT_10376ee8 = 1;
  *(undefined4 *)((int)self_ + 0x15c) = 0;
  *(undefined4 *)((int)self_ + 0x3949c) = 0;
  *(undefined4 *)((int)self_ + 0x16c) = 0x40000000;
  *(undefined4 *)((int)self_ + 0x170) = 0x40000000;
  *(undefined4 *)((int)self_ + 0x38d94) = 0;
  *(uint8_t *)((int)self_ + 0x38d98) = 0;
  g_zbiasZoneMesh_as_v0 = 0;
  g_zbiasPresetAux_as_v0 = 8;
  g_zbiasPreset_C4D0_as_v0 = 0xf;
  g_zbiasOverlay_as_v0 = 0xf;
  g_zbiasRestore_as_v0 = 0;
  *(undefined4 *)((int)self_ + 0x3954c) = 0x3f800000;
  puVar2 = &DAT_10621c88;
  for (iVar1 = 0x4000; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = (undefined4 *)&DAT_10631f38;
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  DAT_10621c80 = 0;
  DAT_10621c7c = 0;
  DAT_1065c4d4 = 0;
  DAT_1065c4cc = 0;
  _DAT_10621c70 = 0;
  _DAT_10621c74 = 0;
  _DAT_10621c78 = 0;
  *(undefined4 *)((int)self_ + 0x394a4) = 1;
  orig::FUN_10177fa0(self_);
  *(uint8_t *)((int)self_ + 0x39598) = 1;
  *(undefined4 *)((int)self_ + 0x39590) = 0x399d4952;
  *(undefined4 *)((int)self_ + 0x39594) = 0x399d4952;
  return self_;
}
```

## `0x10177FA0` fn_10177FA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10177D00`, `Zone_RenderEnvironment`

```cpp
XI_API void __thiscall fn_10177FA0(void * self_)
{
  *(uint32_t *)((int)self_ + 0x394ac) = 0;
  *(uint32_t *)((int)self_ + 0x394a8) = 0;
  *(uint32_t *)((int)self_ + 0x394b0) = 1;
  *(uint32_t *)((int)self_ + 0x394b4) = 2;
  *(uint32_t *)((int)self_ + 0x394b8) = 3;
  *(uint32_t *)((int)self_ + 0x394bc) = 4;
  if (*(int *)((int)self_ + 0x394a4) != 0) {
    *(uint32_t *)((int)self_ + 0x394a8) = 0;
    *(uint32_t *)((int)self_ + 0x394ac) = 1;
    *(uint32_t *)((int)self_ + 0x394b0) = 2;
    *(uint32_t *)((int)self_ + 0x394b4) = 3;
    *(uint32_t *)((int)self_ + 0x394b8) = 4;
    *(uint32_t *)((int)self_ + 0x394bc) = 5;
  }
  return;
}
```

## `0x101784E0` fn_101784E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10177D00`, `ZoneMap_TeardownStep`

```cpp
XI_API void __cdecl fn_101784E0(void)
{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = &DAT_10632040;
  for (iVar2 = 0x1c00; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
  }
  puVar1 = &DAT_1063208c;
  do {
    puVar1[-0x11] = 3;
    puVar1[-1] = 0;
    *puVar1 = 0x3f800000;
    puVar1[1] = 0;
    puVar1 = puVar1 + 0x1c;
  } while ((int)puVar1 < 0x1063908c);
  puVar1 = &DAT_1065c4d8;
  for (iVar2 = 0x40; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
  }
  return;
}
```
