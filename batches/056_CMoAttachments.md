# Batch 056_CMoAttachments: 7 functions of `CMoAttachments`

Name these functions: see `batches/README.md`. Write `data/names/056_CMoAttachments.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1003B540` fn_1003B540

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoAttachments::Reset`, `ActorHandle_Constructor` x2
- **Called by:** `sub_1003CC80`, `sub_10042570`, `sub_100439C0`, `sub_10043BB0`, `sub_10047F80`, `sub_100571E0`, `fn_10057270`, `sub_100572E0`, `CMoSchedularTask_Interpret`, `sub_1005E890`, `sub_1005F320`, `sub_1005F450`, `sub_1005F6C0`, `sub_1005FA20`, `sub_100601F0`, `sub_100607E0`, `sub_10060B30`, `sub_10060DA0`, `sub_10060F10`, `sub_10060F80` and 15 more

```cpp
XI_API void * __thiscall fn_1003B540(void * self_)
{
  orig::ActorHandle_Constructor_v1((void *)((int)self_ + 0x14));
  orig::ActorHandle_Constructor_v1((void *)((int)self_ + 0x20));
  *(uint8_t ***)self_ = &PTR_CMoAttachments_GetRuntimeClass_1032ab08;
  orig::CMoAttachments_Reset_v1(self_);
  return self_;
}
```

## `0x1003B570` CMoAttachments::vf004

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032AE94` slot 4 (+0x10), `CMoAttachments_VT_1032C628` slot 4 (+0x10), `CMoAttachments_VT_1032C4E8` slot 4 (+0x10), `CMoAttachments` slot 4 (+0x10)

```cpp
char CMoAttachments::vf004()
{
  return 0;
}
```

## `0x1003B580` CMoAttachments::vf005

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032AE94` slot 5 (+0x14), `CMoAttachments_VT_1032C628` slot 5 (+0x14), `CMoAttachments_VT_1032C4E8` slot 5 (+0x14), `CMoAttachments` slot 5 (+0x14)

```cpp
char CMoAttachments::vf005()
{
  return 0;
}
```

## `0x1003B590` CMoAttachments::vf006

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032AF44` slot 6 (+0x18), `CMoAttachments_VT_1032BE34` slot 6 (+0x18), `CMoAttachments_VT_1032BDA4` slot 6 (+0x18), `CMoAttachments_VT_1032AE94` slot 6 (+0x18), `CMoAttachments_VT_1032BFE4` slot 6 (+0x18), `CMoAttachments_VT_1032BB38` slot 6 (+0x18), `CMoAttachments_VT_1032BDEC` slot 6 (+0x18), `CMoAttachments_VT_1032C628` slot 6 (+0x18) and 20 more

```cpp
int CMoAttachments::vf006()
{
  return 0;
}
```

## `0x1003B8B0` fn_1003B8B0

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0` x2
- **Called by:** `__attribute__`, `sub_100575A0`, `CMoSchedularTask_Interpret`

```cpp
XI_API int __thiscall fn_1003B8B0(void * self_)
{
  char cVar1;
  void *this_00;
  void *this_01;
  uint64_t uVar2;
  
  uVar2 = orig::FUN_1003b6d0(self_);
  this_00 = (void *)uVar2;
  if (this_00 == (void *)0x0) {
    return 0;
  }
  uVar2 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
  cVar1 = (char)uVar2;
  while( true ) {
    if (cVar1 == '\0') {
      return (int)this_00;
    }
    uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
    this_01 = (void *)uVar2;
    if (this_01 == (void *)0x0) break;
    uVar2 = orig::CYyObject_IsKindOf(this_01,0x1032f910);
    cVar1 = (char)uVar2;
    this_00 = this_01;
  }
  return (int)this_00;
}
```

## `0x1003B900` fn_1003B900

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0`, `sub_1003B700`
- **Called by:** `__attribute__`, `sub_100575A0`, `CMoSchedularTask_Interpret`

```cpp
XI_API int __thiscall fn_1003B900(void * self_)
{
  char cVar1;
  void *this_00;
  void *this_01;
  uint64_t uVar2;
  
  uVar2 = orig::FUN_1003b700(self_);
  this_00 = (void *)uVar2;
  if (this_00 == (void *)0x0) {
    return 0;
  }
  uVar2 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
  cVar1 = (char)uVar2;
  while( true ) {
    if (cVar1 == '\0') {
      return (int)this_00;
    }
    uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
    this_01 = (void *)uVar2;
    if (this_01 == (void *)0x0) break;
    uVar2 = orig::CYyObject_IsKindOf(this_01,0x1032f910);
    cVar1 = (char)uVar2;
    this_00 = this_01;
  }
  return (int)this_00;
}
```

## `0x1003BD90` fn_1003BD90

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `YmGenerater_BuildAttachMatrix`

```cpp
XI_API void __cdecl fn_1003BD90(undefined4 param_1, undefined4 param_2, int * param_3)
{
  switch(param_2) {
  case 0x36:
    param_2 = 0x7e;
    break;
  case 0x37:
    param_2 = 0x7f;
    break;
  case 0x38:
    param_2 = 0x66;
    break;
  case 0x39:
    param_2 = 0x67;
    break;
  case 0x3a:
    param_2 = 0x68;
    break;
  case 0x3b:
    param_2 = 0x69;
    break;
  case 0x3c:
    param_2 = 0x6a;
  }
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY))((**(code **)(*param_3 + 0x1d0))))((void *)(param_3),param_2,param_1);
  return;
}
```
