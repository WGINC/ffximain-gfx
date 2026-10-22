# Batch 079_unit_100ACAB0_XiEvent: 16 functions of `unit_100ACAB0_XiEvent`

Name these functions: see `batches/README.md`. Write `data/names/079_unit_100ACAB0_XiEvent.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x100AD6D0` fn_100AD6D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_100C2E20`, `fn_100C3190` x2, `fn_100C3210`, `fn_100C3300`, `Allocator_Generic`
- **Called by:** `sub_100A1AB0`
- **Reads globals:** `0x10482F40`, `0x104855AA`

```cpp
XI_API void __cdecl fn_100AD6D0(undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8, undefined4 param_9)
{
  uint8_t *puVar1;
  int iVar2;
  void *pvVar3;
  uint64_t uVar4;
  uint32_t uVar5;
  uint32_t uVar6;
  
  puVar1 = orig::FUN_100c3190();
  iVar2 = orig::FUN_100c3210(puVar1);
  if (iVar2 != 0) {
    uVar4 = orig::Allocator_Generic_v1(0x1304);
    if ((void *)uVar4 == (void *)0x0) {
      pvVar3 = (void *)0x0;
    }
    else {
      pvVar3 = orig::FUN_100c2e20((void *)uVar4,param_2,param_3,
                            ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)param_7 >> 0x10)) << 16) | (uint16_t)(DAT_10482f40))),
                            ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)param_8 >> 0x10)) << 16) | (uint16_t)(DAT_104855aa))),param_7,param_8,
                            param_9);
    }
    uVar6 = 0;
    uVar5 = 0;
    puVar1 = orig::FUN_100c3190();
    orig::FUN_100c3300(puVar1,param_1,pvVar3,param_4,uVar5,param_5,uVar6);
  }
  return;
}
```

## `0x100AE350` fn_100AE350

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100AE180`, `sub_100AE360`, `sub_100AE3F0`, `sub_100AEA40`
- **Reads globals:** `0x104896E4`

```cpp
XI_API int __cdecl fn_100AE350(void)
{
  return (int)DAT_104896e4;
}
```

## `0x100AE3A0` fn_100AE3A0

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1015EB60`
- **Called by:** `sub_100AE3F0`, `sub_100B25F0`, `sub_100B2940`, `sub_100B2AA0`
- **Writes globals:** `0x1047D3EC`, `0x104886F4`, `0x104897F4`, `0x104898F8`
- **Reads globals:** `0x1047D3E8`, `0x1047D3EC`, `0x104886F4`, `0x10578158`

```cpp
XI_API void __cdecl fn_100AE3A0(void)
{
  if (DAT_1047d3ec != '\0') {
    orig::FUN_1015eb60(&g_menuMng_as_v0,DAT_1047d3e8);
    DAT_1047d3ec = '\0';
    DAT_104898f8 = 0;
    DAT_104897f4 = 0;
    if (DAT_104886f4 != '\x02') {
      DAT_104886f4 = '\0';
    }
  }
  orig::FUN_10127ea0();
  return;
}
```

## `0x100BA080` fn_100BA080

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100AE3F0`
- **Writes globals:** `0x10488978`
- **Reads globals:** `0x104882B0`, `0x10488978`, `0x104897EC`, `0x1065CB04`

```cpp
XI_API void __cdecl fn_100BA080(void)
{
  if (DAT_10488978 != 0) {
    *(uint32_t *)(g_pXiZone_as_s32 + 0x138) = DAT_104882b0;
    *(uint32_t *)(g_pXiZone_as_s32 + 0x13c) = DAT_104897ec;
  }
  DAT_10488978 = 0;
  return;
}
```

## `0x100BD5F0` fn_100BD5F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf`, `fn_10087D40`, `fn_10087E40`, `__attribute__`, `sub_100D5430`, `operator_delete`
- **Called by:** `sub_1008EDC0`, `fn_1008EE30`

```cpp
XI_API void __thiscall fn_100BD5F0(void * self_)
{
  void *this_00;
  void *this_01;
  uint32_t xi_eax;
  uint64_t uVar1;
  
  *(uint32_t *)((int)self_ + 8) = *(uint32_t *)((int)self_ + 4);
  *(uint16_t *)((int)self_ + 2) = *(uint16_t *)self_;
  if (*(LPVOID *)((int)self_ + 0x25c) != (LPVOID)0x0) {
    orig::operator_delete_10311BB0_v1(*(LPVOID *)((int)self_ + 0x25c));
  }
  *(uint32_t *)((int)self_ + 0x25c) = 0;
  this_00 = (void *)(&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)];
  if ((this_00 != (void *)0x0) &&
     (this_01 = *(void **)((int)this_00 + 0xa0), this_01 != (void *)0x0)) {
    orig::FUN_10087d40(this_00);
    *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 300) =
         *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 300) & 0xfffeffff;
    uVar1 = orig::CYyObject_IsKindOf(this_01,0x10330ebc);
    if ((char)uVar1 != '\0') {
      *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 300) =
           *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 300) & 0xfffffcff;
      *(uint16_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 0x148) = 0xffff;
      xi_eax = orig::FUN_10087e40((void *)(&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)]);
      orig::FUN_100d44b0(this_01,2,xi_eax);
      *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 0x1a4) =
           *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 0x1a8);
      *(uint32_t *)(*(int *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 0xa0) + 0x7d8) =
           0x20202020;
      orig::FUN_100d5430(*(void **)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 2)] + 0xa0),0);
    }
  }
  return;
}
```

## `0x100BD760` fn_100BD760

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100BD820`, `sub_10313486`
- **Called by:** `sub_10096440`, `sub_10096460`, `sub_100C33D0`
- **Writes globals:** `0x1048A2E1`
- **Reads globals:** `0x1048A2E1`

```cpp
XI_API uint8_t * __cdecl fn_100BD760(void)
{
  if ((DAT_1048a2e1 & 1) == 0) {
    DAT_1048a2e1 = DAT_1048a2e1 | 1;
    orig::FUN_100bd820(&DAT_104899b8);
    orig::_atexit_v1(&LAB_100bd7a0);
  }
  return(uint8_t*)(&DAT_104899b8);
}
```

## `0x100C2E20` fn_100C2E20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 28`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `__attribute__` x2, `sub_10312050`
- **Called by:** `sub_100AD5E0`, `sub_100AD660`, `fn_100AD6D0`, `sub_100AD750`
- **Reads globals:** `0x104804CC`, `0x10482F1A`, `0x10485F7A`

```cpp
XI_API void * __thiscall fn_100C2E20(void * self_, undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, char * param_5, undefined4 param_6, undefined4 param_7)
{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  
  *(undefined4 *)self_ = 0;
  *(uint8_t *)((int)self_ + 4) = 0;
  *(uint8_t *)((int)self_ + 0xbfd) = (uint8_t)param_2;
  *(uint16_t *)((int)self_ + 0xc0c) = DAT_104804cc;
  *(uint16_t *)((int)self_ + 0xc0e) = DAT_10482f1a_as_v0;
  puVar3 = (undefined4 *)((int)self_ + 0x621);
  for (iVar5 = 0x177; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar3 = *param_1;
    param_1 = param_1 + 1;
    puVar3 = puVar3 + 1;
  }
  uVar1 = orig::FUN_10086fc0((uint32_t)(uint16_t)param_3,(uint32_t)(uint16_t)param_3);
  if ((char)uVar1 == '\x01') {
    *(uint16_t *)((int)self_ + 0xbfe) = (uint16_t)param_3;
  }
  else {
    *(uint16_t *)((int)self_ + 0xbfe) = DAT_10485f7a_as_v0;
  }
  uVar1 = orig::FUN_10086fc0((uint32_t)(uint16_t)param_4,uVar1);
  if ((char)uVar1 == '\x01') {
    *(uint16_t *)((int)self_ + 0xc00) = (uint16_t)param_4;
  }
  else {
    *(uint16_t *)((int)self_ + 0xc00) = DAT_10485f7a_as_v0;
  }
  if (param_5 == (char *)0x0) {
    *(uint8_t *)((int)self_ + 0xc10) = 0;
  }
  else {
    orig::_strncpy_v1((char *)((int)self_ + 0xc10),param_5,0x40);
  }
  *(undefined4 *)((int)self_ + 0xc08) = param_7;
  *(undefined4 *)((int)self_ + 0xc04) = param_6;
  puVar2 = &DAT_10487020;
  puVar3 = (undefined4 *)((int)self_ + 0xc50);
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  } while ((int)puVar2 < 0x10487034);
  puVar2 = &DAT_10488800;
  puVar3 = (undefined4 *)((int)self_ + 0xc64);
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  } while ((int)puVar2 < 0x10488820);
  puVar2 = &DAT_10488980;
  puVar3 = (undefined4 *)((int)self_ + 0xc84);
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    *puVar3 = uVar1;
    puVar3 = puVar3 + 1;
  } while ((int)puVar2 < 0x10488a00);
  puVar3 = &DAT_10488d08;
  puVar2 = (undefined4 *)((int)self_ + 0xd04);
  do {
    puVar4 = puVar3 + 0x40;
    puVar6 = puVar2;
    for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar6 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar6 = puVar6 + 1;
    }
    puVar3 = puVar4;
    puVar2 = puVar2 + 0x40;
  } while ((int)puVar4 < 0x10489108);
  puVar3 = &DAT_104886f8;
  puVar2 = (undefined4 *)((int)self_ + 0x1104);
  for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar2 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar2 = puVar2 + 1;
  }
  puVar3 = &DAT_10488108;
  puVar2 = (undefined4 *)((int)self_ + 0x1204);
  for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar2 = *puVar3;
    puVar3 = puVar3 + 1;
    puVar2 = puVar2 + 1;
  }
  return self_;
}
```

## `0x100C3050` fn_100C3050

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `operator_delete`
- **Called by:** `fn_100C3220`, `sub_100C3290`

```cpp
XI_API void __thiscall fn_100C3050(void * self_)
{
  if (*(LPVOID *)((int)self_ + 0x40) != (LPVOID)0x0) {
    orig::operator_delete_10311BB0_v1(*(LPVOID *)((int)self_ + 0x40));
  }
  return;
}
```

## `0x100C3060` fn_100C3060

- **Gate:** none; **frame set:** static
- **Facts:** `ret 32`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_100C3300`

```cpp
XI_API void * __thiscall fn_100C3060(void * self_, undefined4 * param_1, undefined4 param_2, uint16_t * param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, undefined4 param_8)
{
  int iVar1;
  undefined4 *puVar2;
  
  *(undefined4 *)((int)self_ + 0x40) = param_2;
  *(uint16_t *)((int)self_ + 0x44) = *param_3;
  *(uint8_t *)((int)self_ + 0x46) = *(uint8_t *)(param_3 + 1);
  *(uint8_t *)((int)self_ + 0x47) = (uint8_t)param_4;
  *(uint16_t *)((int)self_ + 0x48) = (uint16_t)param_6;
  *(uint16_t *)((int)self_ + 0x4a) = (uint16_t)param_7;
  *(uint16_t *)((int)self_ + 0x4c) = (uint16_t)param_8;
  *(undefined4 *)((int)self_ + 0x50) = param_5;
  *(undefined4 *)((int)self_ + 0x54) = 0;
  *(uint16_t *)((int)self_ + 0x58) = 0;
  iVar1 = 0x10;
  puVar2 = self_;
  if (param_1 == (undefined4 *)0x0) {
    for (; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    return self_;
  }
  for (; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = *param_1;
    param_1 = param_1 + 1;
    puVar2 = puVar2 + 1;
  }
  return self_;
}
```

## `0x100C3190` fn_100C3190

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100C31F0`, `sub_10313486`
- **Called by:** `Unit10086E90_ResetGlobals`, `fn_10093B70`, `sub_10093E30`, `sub_100AD5E0`, `sub_100AD660`, `fn_100AD6D0`, `sub_100AD750`
- **Writes globals:** `0x1048A2E0`
- **Reads globals:** `0x1048A2E0`

```cpp
XI_API uint8_t * __cdecl fn_100C3190(void)
{
  if ((DAT_1048a2e0 & 1) == 0) {
    DAT_1048a2e0 = DAT_1048a2e0 | 1;
    orig::FUN_100c31f0(&DAT_1048a2f8);
    orig::_atexit_v1(&LAB_100c31d0);
  }
  return(uint8_t*)(&DAT_1048a2f8);
}
```

## `0x100C3200` fn_100C3200

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit10086E90_ResetGlobals`

```cpp
XI_API void __thiscall fn_100C3200(void * self_)
{
  *(uint32_t *)((int)self_ + 8) = 1;
  return;
}
```

## `0x100C3210` fn_100C3210

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100AD5E0`, `sub_100AD660`, `fn_100AD6D0`, `sub_100AD750`

```cpp
XI_API undefined4 __thiscall fn_100C3210(void * self_)
{
  return (uint32_t)(*(int *)((int)self_ + 8) != 0);
}
```

## `0x100C3220` fn_100C3220

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_100C3050`, `operator_delete`
- **Called by:** `fn_10093B70`

```cpp
XI_API undefined4 __thiscall fn_100C3220(void * self_)
{
  void *pvVar1;
  void *this_00;
  uint32_t xi_entry_eax = 0;
  uint64_t uVar2;
  
  this_00 = *(void **)self_;
  while (this_00 != (void *)0x0) {
    pvVar1 = *(void **)((int)this_00 + 0x5c);
    orig::FUN_100c3050(this_00);
    uVar2 = orig::operator_delete_10311BB0_v1(this_00);
    xi_entry_eax = (uint32_t)uVar2;
    this_00 = pvVar1;
  }
  *(uint32_t *)self_ = 0;
  *(uint32_t *)((int)self_ + 4) = 0;
  return xi_entry_eax;
}
```

## `0x100C3260` fn_100C3260

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_100C3300`

```cpp
XI_API void __thiscall fn_100C3260(void * self_, int param_1)
{
  *(uint32_t *)(param_1 + 0x5c) = 0;
  *(uint32_t *)(param_1 + 0x60) = 0;
  if (*(int *)self_ == 0) {
    *(int *)self_ = param_1;
    *(int *)((int)self_ + 4) = param_1;
    return;
  }
  *(int *)(*(int *)((int)self_ + 4) + 0x5c) = param_1;
  *(uint32_t *)(param_1 + 0x60) = *(uint32_t *)((int)self_ + 4);
  *(int *)((int)self_ + 4) = param_1;
  return;
}
```

## `0x100C3300` fn_100C3300

- **Gate:** none; **frame set:** static
- **Facts:** `ret 24`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `__attribute__` x2, `__attribute__` x2, `__attribute__` x2, `fn_100C3060`, `fn_100C3260` x2, `Allocator_Generic`
- **Called by:** `sub_100AD5E0`, `sub_100AD660`, `fn_100AD6D0`, `sub_100AD750`
- **Reads globals:** `0x10485F7A`

```cpp
XI_API void __thiscall fn_100C3300(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6)
{
  uint32_t xi_entry_eax = 0;
  uint32_t uVar1;
  uint32_t uVar2;
  void *xi_eax;
  void *pvVar3;
  void *pvVar4;
  uint64_t uVar5;
  
  pvVar3 = param_2;
  uVar1 = orig::FUN_100c3020(param_2,xi_entry_eax);
  uVar2 = orig::FUN_10086fc0(uVar1 & 0xffff,uVar1 & 0xffff);
  if ((char)uVar2 == '\x01') {
    xi_eax = (void *)orig::FUN_100c3020(param_2,uVar2);
    pvVar4 = xi_eax;
  }
  else {
    xi_eax = (void *)((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)uVar2 >> 0x10)) << 16) | (uint16_t)(DAT_10485f7a_as_v0)));
    param_2 = (void *)((uint32_t)(((uint32_t)(uint16_t)(XI_PIECE(param_2, 2, 2)) << 16) | (uint16_t)(DAT_10485f7a_as_v0)));
    pvVar4 = param_2;
  }
  uVar1 = orig::FUN_100c3030(pvVar3,xi_eax);
  uVar2 = orig::FUN_10086fc0(uVar1 & 0xffff,uVar1 & 0xffff);
  if ((char)uVar2 == '\x01') {
    param_2 = (void *)orig::FUN_100c3030(pvVar3,uVar2);
  }
  else {
    param_2 = (void *)((uint32_t)(((uint32_t)(uint16_t)(XI_PIECE(param_2, 2, 2)) << 16) | (uint16_t)(DAT_10485f7a_as_v0)));
  }
  uVar5 = orig::Allocator_Generic_v1(100);
  if ((void *)uVar5 != (void *)0x0) {
    pvVar3 = orig::FUN_100c3060((void *)uVar5,param_1,pvVar3,param_3,param_4,param_5,param_2,pvVar4,
                          param_6);
    orig::FUN_100c3260(self_,(int)pvVar3);
    return;
  }
  orig::FUN_100c3260(self_,0);
  return;
}
```

## `0x100C3480` fn_100C3480

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetFrameScale` x2, `sub_10087DF0`, `sub_100C2D00` x2, `sub_100C2FB0`, `sub_100C2FE0` x2, `sub_100C30F0`, `sub_100C3100` x2, `sub_100C3120`, `sub_100C3130` x15, `sub_100C3140` x2, `sub_100C3150`, `sub_100C3160`, `sub_100C3170`, `sub_10127C00`, `sub_10311C2C` x2
- **Called by:** `sub_100C33D0`
- **Writes globals:** `0x104807FC`
- **Reads globals:** `0x10456A28`, `0x104807FC`, `0x10578158`

```cpp
XI_API undefined4 __thiscall fn_100C3480(void * self_)
{
  short sVar1;
  void *pvVar2;
  int *a2;
  int iVar3;
  uint32_t uVar4;
  char *pcVar5;
  char *pcVar6;
  uint32_t uVar7;
  int a5;
  uint32_t uVar8;
  uint8_t *pbVar9;
  int iVar10;
  uint8_t bVar11;
  long double fVar12;
  int64_t lVar13;
  char xi_frame[1588] __attribute__((aligned(4)));
  uint32_t  &uVar14 = *reinterpret_cast<uint32_t  *>(xi_frame + 0);
  char  (&local_61c)[1564] = *reinterpret_cast<char  (*)[1564]>(xi_frame + 24);
  
  DAT_104807fc = DAT_104807fc + 1;
  iVar3 = *(int *)((int)self_ + 0x54);
  if (iVar3 == 0) {
    iVar3 = orig::FUN_100c3130(self_);
    if (((((iVar3 != 1) && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 9)) &&
         (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0xb)) &&
        ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32 && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2cf))
        )) && ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32b &&
               (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32d)))) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
      return 0;
    }
    uVar4 = orig::FUN_100c3140(self_);
    if ((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] == 0) {
      *(uint16_t *)((int)self_ + 0x58) = 0x3c;
      *(uint32_t *)((int)self_ + 0x54) = 2;
      return 0;
    }
    *(uint16_t *)((int)self_ + 0x58) = 600;
    *(uint32_t *)((int)self_ + 0x54) = 1;
    return 0;
  }
  if (iVar3 == 1) {
    sVar1 = *(short *)((int)self_ + 0x58);
    fVar12 = orig::Camera_GetFrameScale_v1((int)sVar1);
    lVar13 = XI_CALL_ST0(0x10311C2Cu)((long double)(float)(long double)(int)sVar1 - fVar12);
    *(short *)((int)self_ + 0x58) = (short)lVar13;
    if ((short)lVar13 < 1) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
    }
    uVar4 = orig::FUN_100c3140(self_);
    if ((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] == 0) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
    }
    else {
      iVar3 = *(int *)((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] + 0x170);
      if ((iVar3 == 2) || (iVar3 == 3)) {
        *(uint16_t *)((int)self_ + 0x58) = 0x3c;
        *(uint32_t *)((int)self_ + 0x54) = 2;
        return 0;
      }
    }
    return 0;
  }
  sVar1 = *(short *)((int)self_ + 0x58);
  fVar12 = orig::Camera_GetFrameScale_v1(iVar3);
  lVar13 = XI_CALL_ST0(0x10311C2Cu)((long double)(float)(long double)(int)sVar1 - fVar12);
  *(short *)((int)self_ + 0x58) = (short)lVar13;
  if (0 < (short)lVar13) {
    return 0;
  }
  pvVar2 = *(void **)((int)self_ + 0x40);
  iVar3 = orig::FUN_100c2fb0(pvVar2);
  if (iVar3 != 2) {
    return 0;
  }
  pcVar5 = orig::FUN_100c30f0(self_);
  pcVar5 = orig::FUN_100c2d00(local_61c,pcVar5);
  pcVar6 = (char *)orig::FUN_100c2fe0(pvVar2);
  orig::FUN_100c2d00(pcVar5,pcVar6);
  uVar4 = 0xffffffff;
  iVar3 = orig::FUN_100c3130(self_);
  if ((((iVar3 != 9) && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0xb)) &&
      (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32)) &&
     (((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2cf && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2d))
      && ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32b &&
          (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32d)))))) goto LAB_100c37de;
  uVar7 = orig::FUN_100c3150(self_);
  iVar3 = (&ActerBuffPtr_as_v0)[uVar7 & 0xffff];
  if (iVar3 == 0) {
    bVar11 = false;
  }
  else {
    bVar11 = *(short *)(iVar3 + 0x210) != 0;
  }
  iVar10 = *(int *)(g_pSceneState + 8);
  if ((*(char *)(g_pSceneState + 0x18) < '\0') && ((iVar10 == 2 || (iVar10 == 3)))) {
    iVar10 = 1;
  }
  uVar7 = orig::FUN_100c3130(self_);
  if (uVar7 < 0x33) {
    if (uVar7 == 0x32) {
      uVar4 = 2;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 8) * 4);
    }
    else if (uVar7 == 9) {
      uVar4 = -(uint32_t)bVar11 & 4;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + uVar4 * 4) * 4);
    }
    else if (uVar7 == 0xb) {
      uVar4 = 1;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 4) * 4);
    }
    else {
      if ((uVar7 != 0x2d) || (!bVar11)) goto LAB_100c3793;
      uVar4 = 5;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x14) * 4);
    }
  }
  else if (uVar7 == 0x2cf) {
    uVar4 = 3;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0xc) * 4);
  }
  else if (uVar7 == 0x32b) {
    uVar4 = 6;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x18) * 4);
  }
  else if (uVar7 == 0x32d) {
    uVar4 = 7;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x1c) * 4);
  }
  else {
LAB_100c3793:
    iVar10 = -1;
  }
  if ((iVar3 != 0) && ((*(uint32_t *)(iVar3 + 0x120) >> 9 & 1) != 0)) {
    if (uVar4 != 0xffffffff) {
      a2 = *(int **)(iVar3 + 0xa0);
      iVar3 = *a2;
      a5 = orig::FUN_100c3100(self_);
      ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar3 + 0x2c4))))(iVar10,a2,0x6e69616d,local_61c,a5);
    }
    if (iVar10 != -1) {
      *(uint32_t *)((int)self_ + 0x54) = 0;
      return 1;
    }
// ... 22 more lines (tree/src/unit_100ACAB0_XiEvent.cpp)
```
