# Batch r2_097_unit_1025C9E0_FontGlyphCache: 22 functions of `unit_1025C9E0_FontGlyphCache`

Name these functions: see `batches/README.md`. Write `data/names/r2_097_unit_1025C9E0_FontGlyphCache.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x1025E020` fn_1025E020

- **Gate:** none; **frame set:** static, self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_101AB820`, `fn_1025E0F0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `fn_101AB820` → `fn_1025E020`
- **Named callers' callers:** `Frame_Render`
- **How its callers call it** (their verified code):
  - in `fn_1025E0F0`: `return orig::FUN_1025e020(xi_eax);`

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1025E020(undefined4 xi_eax)
{
  return xi_eax & 0xffffff00;
}
```

## `0x1025E0F0` fn_1025E0F0

- **Gate:** none; **frame set:** static
- **Facts:** sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Frame_Render`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `fn_1025E0F0`
- **Named callers' callers:** `App_RenderFrameMaskedFPU`
- **How its callers call it** (their verified code):
  - in `Frame_Render`: `orig::sub_1025E0F0();`

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1025E0F0(undefined4 xi_eax)
{
  return orig::FUN_1025e020(xi_eax);
}
```

## `0x1025E260` fn_1025E260

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10261210`, `sub_10265B70` x3
- **Called by:** `fn_1025FD50`
- **Imports:** `SetEvent`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `sub_10018850` → `CXiMovie_Create` → `fn_1021D930` → `sub_1021D9B0` → `fn_1021DE90` → `fn_1025FD50` → `fn_1025E260`
- **How its callers call it** (their verified code):
  - in `fn_1025FD50`: `orig::FUN_1025e260(self_,param_1,param_2,param_3,param_4);`

```cpp
XI_API void * __thiscall fn_1025E260(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 xi_s4)
{
  char xi_frame[12] __attribute__((aligned(4)));
  void * &local_c = *reinterpret_cast<void * *>(xi_frame + 0);
  uint8_t * &puStack_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint32_t  &local_4 = *reinterpret_cast<uint32_t  *>(xi_frame + 8);
  
  local_4 = 0xffffffff;
  puStack_8 =(uint8_t*)(&LAB_1032752e);
  local_c = ExceptionList;
  ExceptionList = &local_c;
  orig::FUN_10261210(self_,param_2,param_3,(LPCRITICAL_SECTION)((int)self_ + 0x7c),param_1);
  local_4 = 0;
  *(uint32_t *)((int)self_ + 0x50) = 0;
  orig::FUN_10265b70((void *)((int)self_ + 0x54),0);
  XI_PIECE(local_4, 0, 1) = 1;
  orig::FUN_10265b70((void *)((int)self_ + 0x58),1);
  local_4 = ((uint32_t)(((uint32_t)(XI_PIECE(local_4, 1, 3)) << 8) | (uint8_t)(2)));
  orig::FUN_10265b70((uint32_t *)((int)self_ + 0x5c),1);
  *(uint32_t *)((int)self_ + 0x60) = 0;
  *(uint32_t *)((int)self_ + 100) = 0;
  *(uint32_t *)((int)self_ + 0x68) = 0;
  *(uint32_t *)((int)self_ + 0x6c) = 0;
  *(uint32_t *)((int)self_ + 0x70) = 0;
  *(uint32_t *)((int)self_ + 0x74) = 0;
  *(uint32_t *)((int)self_ + 0x78) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x7c));
  InitializeCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x94));
  *(uint32_t *)((int)self_ + 0xac) = 0;
  *(uint32_t *)((int)self_ + 0xb0) = 1;
  *(uint32_t *)((int)self_ + 0xb4) = 0;
  *(uint32_t *)((int)self_ + 0xb8) = 0;
  *(uint32_t *)((int)self_ + 0xbc) = 0;
  *(uint32_t *)((int)self_ + 0xc0) = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0xc4));
  *(uint8_t ***)self_ = &PTR_LAB_10340478;
  *(uint8_t ***)((int)self_ + 0xc) = &PTR_FUN_1034043c;
  *(uint8_t ***)((int)self_ + 0x10) = &PTR_LAB_10340428;
  SetEvent(*(HANDLE *)((int)self_ + 0x5c));
  ExceptionList = local_c;
  return self_;
}
```

## `0x1025E380` fn_1025E380

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1025F2F0`, `sub_1025F640`, `fn_102612D0`, `sub_10265B90` x3
- **Called by:** `sub_1025E360`, `fn_1025FD50`, `sub_1025FE10`
- **Imports:** `DeleteCriticalSection`
- **Named callees' callees:** `operator_delete`

```cpp
XI_API void __thiscall fn_1025E380(void * self_)
{
  int *piVar1;
  char xi_frame[12] __attribute__((aligned(4)));
  void * &pvStack_c = *reinterpret_cast<void * *>(xi_frame + 0);
  uint8_t * &puStack_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  int  &local_4 = *reinterpret_cast<int  *>(xi_frame + 8);
  
  puStack_8 =(uint8_t*)(&LAB_10327590);
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(uint8_t ***)self_ = &PTR_LAB_10340478;
  *(uint8_t ***)((int)self_ + 0xc) = &PTR_FUN_1034043c;
  *(uint8_t ***)((int)self_ + 0x10) = &PTR_LAB_10340428;
  local_4 = 6;
  orig::FUN_1025f640(self_);
  orig::FUN_1025f2f0(self_);
  if (*(int *)((int)self_ + 0x50) != 0) {
    piVar1 = (int *)(*(int *)((int)self_ + 0x50) + 8);
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*piVar1 + 0xc))))((void *)(piVar1),1);
    *(uint32_t *)((int)self_ + 0x50) = 0;
  }
  piVar1 = *(int **)((int)self_ + 0x78);
  if (piVar1 != (int *)0x0) {
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*piVar1 + 0xc))))((void *)(piVar1),1);
    *(uint32_t *)((int)self_ + 0x78) = 0;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0xc4));
  DeleteCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x94));
  DeleteCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x7c));
  XI_PIECE(local_4, 0, 1) = 2;
  orig::FUN_10265b90((void *)((int)self_ + 0x5c));
  XI_PIECE(local_4, 0, 1) = 1;
  orig::FUN_10265b90((void *)((int)self_ + 0x58));
  local_4 = (uint32_t)XI_PIECE(local_4, 1, 3) << 8;
  orig::FUN_10265b90((void *)((int)self_ + 0x54));
  local_4 = 0xffffffff;
  orig::FUN_102612d0(self_);
  ExceptionList = pvStack_c;
  return;
}
```

## `0x1025F2F0` fn_1025F2F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1025E380`
- **Imports:** `EnterCriticalSection`, `LeaveCriticalSection`
- **How its callers call it** (their verified code):
  - in `fn_1025E380`: `orig::FUN_1025f2f0(self_);`

```cpp
XI_API undefined4 __thiscall fn_1025F2F0(void * self_)
{
  int *a1;
  char xi_frame[12] __attribute__((aligned(4)));
  void * &pvStack_c = *reinterpret_cast<void * *>(xi_frame + 0);
  uint8_t * &puStack_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint32_t  &local_4 = *reinterpret_cast<uint32_t  *>(xi_frame + 8);
  
  local_4 = 0xffffffff;
  puStack_8 =(uint8_t*)(&LAB_103276b8);
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  EnterCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x94));
  a1 = *(int **)((int)self_ + 0x6c);
  local_4 = 0;
  if (a1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*a1 + 8))))(a1);
    *(uint32_t *)((int)self_ + 0x6c) = 0;
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)((int)self_ + 0x94));
  ExceptionList = pvStack_c;
  return 0;
}
```

## `0x1025FD50` fn_1025FD50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1025E260`, `fn_1025FE40`
- **Called by:** `fn_1021DE90`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `CXiMovie_Create` → `fn_1021D930` → `sub_1021D9B0` → `fn_1021DE90` → `fn_1025FD50`
- **How its callers call it** (their verified code):
  - in `fn_1021DE90`: `orig::FUN_1025fd50(self_,&DAT_10385eb0,0,param_1,param_2);`

```cpp
XI_API void * __thiscall fn_1025FD50(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  char xi_frame[12] __attribute__((aligned(4)));
  void * &local_c = *reinterpret_cast<void * *>(xi_frame + 0);
  uint8_t * &puStack_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint32_t  &local_4 = *reinterpret_cast<uint32_t  *>(xi_frame + 8);
  
  local_4 = 0xffffffff;
  puStack_8 =(uint8_t*)(&LAB_10327818);
  local_c = ExceptionList;
  ExceptionList = &local_c;
  orig::FUN_1025e260(self_,param_1,param_2,param_3,param_4);
  *(uint8_t ***)((int)self_ + 0xe0) = &PTR_LAB_1034073c;
  local_4 = 0;
  *(uint32_t *)((int)self_ + 0xec) = 0;
  *(uint32_t *)((int)self_ + 0x120) = 0;
  *(uint32_t *)((int)self_ + 0x124) = 0;
  *(uint8_t ***)self_ = &PTR_LAB_10340678;
  *(uint8_t ***)((int)self_ + 0xc) = &PTR_LAB_10340638;
  *(uint8_t ***)((int)self_ + 0x10) = &PTR_LAB_10340624;
  *(uint8_t ***)((int)self_ + 0xe0) = &PTR_LAB_10340600;
  *(uint8_t ***)((int)self_ + 0xe4) = &PTR_LAB_103405ec;
  orig::FUN_1025fe40(self_);
  ExceptionList = local_c;
  return self_;
}
```

## `0x1025FE40` fn_1025FE40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1025FD50`
- **Imports:** `timeGetTime`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `sub_10018850` → `CXiMovie_Create` → `fn_1021D930` → `sub_1021D9B0` → `fn_1021DE90` → `fn_1025FD50` → `fn_1025FE40`
- **How its callers call it** (their verified code):
  - in `fn_1025FD50`: `orig::FUN_1025fe40(self_);`

```cpp
XI_API void __thiscall fn_1025FE40(void * self_)
{
  DWORD DVar1;
  
  *(uint32_t *)((int)self_ + 0x138) = 0xfffffc18;
  *(uint32_t *)((int)self_ + 0x13c) = 0xffffffff;
  DVar1 = timeGetTime();
  *(DWORD *)((int)self_ + 0x158) = DVar1;
  *(uint32_t *)((int)self_ + 0xf4) = 0;
  *(uint32_t *)((int)self_ + 0x10c) = 0xffffffff;
  *(uint32_t *)((int)self_ + 0x110) = 0;
  *(uint32_t *)((int)self_ + 0xf8) = 0;
  *(uint32_t *)((int)self_ + 0x108) = 0;
  *(uint32_t *)((int)self_ + 0xfc) = 0;
  *(uint32_t *)((int)self_ + 0x124) = 0;
  *(uint32_t *)((int)self_ + 0x120) = 0;
  *(uint32_t *)((int)self_ + 0x128) = 0;
  *(uint32_t *)((int)self_ + 300) = 0;
  *(uint32_t *)((int)self_ + 0x130) = 0;
  *(uint32_t *)((int)self_ + 0x134) = 0;
  *(uint32_t *)((int)self_ + 0x140) = 0;
  *(uint32_t *)((int)self_ + 0x144) = 0;
  *(uint32_t *)((int)self_ + 0x154) = 0;
  *(uint32_t *)((int)self_ + 0x150) = 0;
  *(uint32_t *)((int)self_ + 0x148) = 0;
  *(uint32_t *)((int)self_ + 0x14c) = 0;
  *(uint32_t *)((int)self_ + 0xe8) = 0;
  *(uint32_t *)((int)self_ + 0x100) = 0;
  *(uint32_t *)((int)self_ + 0x104) = 0xfffb6c20;
  *(uint32_t *)((int)self_ + 0xf0) = 0;
  *(uint32_t *)((int)self_ + 0x118) = 0;
  *(uint32_t *)((int)self_ + 0x11c) = 0;
  return;
}
```

## `0x10261210` fn_10261210

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102659B0`
- **Called by:** `fn_1025E260`
- **From the frame root** (shortest chain of direct calls): `sub_10018850` → `CXiMovie_Create` → `fn_1021D930` → `sub_1021D9B0` → `fn_1021DE90` → `fn_1025FD50` → `fn_1025E260` → `fn_10261210`
- **How its callers call it** (their verified code):
  - in `fn_1025E260`: `orig::FUN_10261210(self_,param_2,param_3,(LPCRITICAL_SECTION)((int)self_ + 0x7c),param_1);`

Short callee `fn_102659B0`:
```cpp
XI_API void * __thiscall fn_102659B0(void * self_, undefined4 param_1, void * param_2)
{
  *(uint8_t ***)self_ = &PTR_LAB_10340924;
  orig::FUN_10265940((uint32_t *)((int)self_ + 4),param_1);
  if (param_2 == (void *)0x0) {
    param_2 = self_;
  }
  *(uint32_t *)((int)self_ + 4) = param_2;
  *(uint32_t *)((int)self_ + 8) = 0;
  return self_;
}
```

```cpp
XI_API void * __thiscall fn_10261210(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 * param_4)
{
  uint32_t uVar1;
  
  orig::FUN_102659b0(self_,param_1,param_2);
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  *(uint32_t *)((int)self_ + 0x20) = 0;
  *(uint32_t *)((int)self_ + 0x24) = 0;
  *(uint32_t *)((int)self_ + 0x28) = *param_4;
  *(uint32_t *)((int)self_ + 0x2c) = param_4[1];
  *(uint32_t *)((int)self_ + 0x30) = param_4[2];
  uVar1 = param_4[3];
  *(uint32_t *)((int)self_ + 0x3c) = 0;
  *(uint32_t *)((int)self_ + 0x40) = 0;
  *(uint32_t *)((int)self_ + 0x34) = uVar1;
  *(uint32_t *)((int)self_ + 0x44) = 0;
  *(uint32_t *)((int)self_ + 0x38) = param_3;
  *(uint32_t *)((int)self_ + 0x48) = 1;
  return self_;
}
```

## `0x102612D0` fn_102612D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10265960`, `operator_delete`
- **Called by:** `fn_1025E260`, `fn_1025E380`
- **How its callers call it** (their verified code):
  - in `fn_1025E380`: `orig::FUN_102612d0(self_);`

```cpp
XI_API void __thiscall fn_102612D0(void * self_)
{
  int *a1;
  char xi_frame[12] __attribute__((aligned(4)));
  void * &pvStack_c = *reinterpret_cast<void * *>(xi_frame + 0);
  uint8_t * &puStack_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint32_t  &local_4 = *reinterpret_cast<uint32_t  *>(xi_frame + 8);
  
  puStack_8 =(uint8_t*)(&LAB_103278d8);
  pvStack_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &pvStack_c;
  orig::operator_delete_10311BB0_v1(*(LPVOID *)((int)self_ + 0x3c));
  a1 = *(int **)((int)self_ + 0x18);
  if (a1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*a1 + 8))))(a1);
    *(uint32_t *)((int)self_ + 0x18) = 0;
  }
  local_4 = 0xffffffff;
  orig::_dynamic_initializer_for__std__num_put_char_std__ostreambuf_iterator_char_std__char_traits_char_______id___47
            ();
  ExceptionList = pvStack_c;
  return;
}
```

## `0x102659B0` fn_102659B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10265940`
- **Called by:** `sub_10260D60`, `fn_10261210`, `sub_10261270`, `sub_10262450`, `sub_10264BB0`, `sub_10266D00`, `sub_10266E50`, `sub_10266FB0`, `sub_10266FE0`, `sub_10267E00`, `sub_102685E0`, `sub_10268730`, `sub_10268880`, `sub_10268C60`
- **From the frame root** (shortest chain of direct calls): `CXiMovie_Create` → `fn_1021D930` → `sub_1021D9B0` → `fn_1021DE90` → `fn_1025FD50` → `fn_1025E260` → `fn_10261210` → `fn_102659B0`
- **How its callers call it** (their verified code):
  - in `fn_10261210`: `orig::FUN_102659b0(self_,param_1,param_2);`

```cpp
XI_API void * __thiscall fn_102659B0(void * self_, undefined4 param_1, void * param_2)
{
  *(uint8_t ***)self_ = &PTR_LAB_10340924;
  orig::FUN_10265940((uint32_t *)((int)self_ + 4),param_1);
  if (param_2 == (void *)0x0) {
    param_2 = self_;
  }
  *(uint32_t *)((int)self_ + 4) = param_2;
  *(uint32_t *)((int)self_ + 8) = 0;
  return self_;
}
```

## `0x1026CCA5` fn_1026CCA5

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10316D6B`, `sub_10316D90`
- **Called by:** `sub_1026A8AD`, `sub_1026D751`, `fn_1026D866`
- **From the frame root** (shortest chain of direct calls): `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115500` → `fn_101154D0` → `fn_101152F0` → `fn_1026D866` → `fn_1026CCA5`
- **How its callers call it** (their verified code):
  - in `fn_1026D866`: `orig::FUN_1026cca5((void *)(param_1 + local_18 * 0x10058),0);`

```cpp
XI_API void __cdecl fn_1026CCA5(void * param_1, undefined4 param_2)
{
  *(uint32_t *)((int)param_1 + 0x10) = 0xffffffff;
  orig::_memset(param_1,0,0x10);
  *(uint32_t *)((int)param_1 + 0x20) = param_2;
  *(uint32_t *)((int)param_1 + 0x402c) = 0;
  *(uint32_t *)((int)param_1 + 0x4030) = 0;
  *(uint32_t *)((int)param_1 + 0x8034) = 0;
  *(uint32_t *)((int)param_1 + 0x8038) = 0;
  *(uint32_t *)((int)param_1 + 0xc03c) = 0;
  *(uint32_t *)((int)param_1 + 0xc040) = 0;
  *(uint32_t *)((int)param_1 + 0x10044) = 0;
  *(uint32_t *)((int)param_1 + 0x10048) = 0;
  *(uint32_t *)((int)param_1 + 0x24) = 0;
  *(uint32_t *)((int)param_1 + 0x28) = 0;
  *(uint32_t *)((int)param_1 + 0x1004c) = 0;
  *(uint32_t *)((int)param_1 + 0x10050) = 0;
  *(uint32_t *)((int)param_1 + 0x10054) = 0;
  orig::FUN_10316d6b(0);
  return;
}
```

## `0x1026CFA0` fn_1026CFA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026D0CA`, `sub_1026D1B4`, `sub_10316D6B`
- **Called by:** `fn_101157E0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_101157E0` → `fn_1026CFA0`
- **How its callers call it** (their verified code):
  - in `fn_101157E0`: `iVar1 = orig::FUN_1026cfa0(param_1 + 0x2f0d4);`

```cpp
XI_API undefined4 __cdecl fn_1026CFA0(int param_1)
{
  int iVar1;
  uint32_t uVar2;
  char xi_frame[20] __attribute__((aligned(4)));
  uint8_t * &local_14 = *reinterpret_cast<uint8_t * *>(xi_frame + 0);
  uint8_t * &local_10 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint8_t * &local_c = *reinterpret_cast<uint8_t * *>(xi_frame + 8);
  uint8_t * &local_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 12);
  
  local_14 =(uint8_t*)(&DAT_cccccccc);
  local_10 =(uint8_t*)(&DAT_cccccccc);
  local_c =(uint8_t*)(&DAT_cccccccc);
  local_8 =(uint8_t*)(&DAT_cccccccc);
  iVar1 = orig::FUN_1026d0ca(param_1 + 0x4034,&local_14);
  if (iVar1 < 0) {
    uVar2 = 0xffffffff;
  }
  else {
    local_c = (uint8_t *)0x5;
    orig::FUN_1026d1b4(param_1 + 0x2c,&local_14);
    uVar2 = 0;
  }
  local_8 = (uint8_t *)0x1026cffc;
  uVar2 = orig::FUN_10316d6b(uVar2);
  return uVar2;
}
```

## `0x1026D015` fn_1026D015

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026D0CA` x2, `sub_1026D1B4` x2, `sub_10316D6B`
- **Called by:** `fn_10115820`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115820` → `fn_1026D015`
- **How its callers call it** (their verified code):
  - in `fn_10115820`: `orig::FUN_1026d015(param_1 + 0x2f0d4);`

```cpp
XI_API void __cdecl fn_1026D015(int param_1)
{
  int iVar1;
  char xi_frame[20] __attribute__((aligned(4)));
  uint8_t * &local_14 = *reinterpret_cast<uint8_t * *>(xi_frame + 0);
  uint8_t * &local_10 = *reinterpret_cast<uint8_t * *>(xi_frame + 4);
  uint8_t * &local_c = *reinterpret_cast<uint8_t * *>(xi_frame + 8);
  uint8_t * &local_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 12);
  
  local_14 =(uint8_t*)(&DAT_cccccccc);
  local_10 =(uint8_t*)(&DAT_cccccccc);
  local_c =(uint8_t*)(&DAT_cccccccc);
  local_8 =(uint8_t*)(&DAT_cccccccc);
  while (iVar1 = orig::FUN_1026d0ca(param_1 + 0xc044,&local_14), -1 < iVar1) {
    orig::FUN_1026d1b4(param_1 + 0x803c,&local_14);
  }
  while (iVar1 = orig::FUN_1026d0ca(param_1 + 0x2c,&local_14), -1 < iVar1) {
    orig::FUN_1026d1b4(param_1 + 0x4034,&local_14);
  }
  *(uint32_t *)(param_1 + 0x24) = 0;
  *(uint32_t *)(param_1 + 0x28) = 0;
  local_8 = (uint8_t *)0x1026d0bb;
  orig::FUN_10316d6b(param_1);
  return;
}
```

## `0x1026D0CA` fn_1026D0CA

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_103169A0`, `sub_10316D6B`
- **Called by:** `sub_1026A8AD`, `sub_1026BE54`, `sub_1026CDF4`, `sub_1026CEED`, `fn_1026CFA0`, `fn_1026D015`, `fn_1026DB6D`, `fn_1026DBCE`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115610` → `fn_1026DBCE` → `fn_1026D0CA`
- **How its callers call it** (their verified code):
  - in `fn_1026CFA0`: `iVar1 = orig::FUN_1026d0ca(param_1 + 0x4034,&local_14);`
  - in `fn_1026D015`: `while (iVar1 = orig::FUN_1026d0ca(param_1 + 0xc044,&local_14), -1 < iVar1) {`
  - in `fn_1026D015`: `while (iVar1 = orig::FUN_1026d0ca(param_1 + 0x2c,&local_14), -1 < iVar1) {`

```cpp
XI_API undefined4 __cdecl fn_1026D0CA(int param_1, undefined4 param_2)
{
  int iVar1;
  uint32_t uVar2;
  
  iVar1 = *(int *)(param_1 + 0x4004);
  if (iVar1 == *(int *)(param_1 + 0x4000)) {
    uVar2 = 0xffffffff;
  }
  else {
    orig::FUN_103169a0(param_2,(uint32_t *)(param_1 + iVar1 * 0x10),0x10);
    *(uint32_t *)(param_1 + 0x4004) = iVar1 + 1U & 0x3ff;
    uVar2 = 0;
  }
  uVar2 = orig::FUN_10316d6b(uVar2);
  return uVar2;
}
```

## `0x1026D70C` fn_1026D70C

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1026D634`, `sub_10316D6B`
- **Called by:** `fn_101152F0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115500` → `fn_101154D0` → `fn_101152F0` → `fn_1026D70C`
- **How its callers call it** (their verified code):
  - in `fn_101152F0`: `iVar5 = orig::FUN_1026d70c((int)(puVar3 + 0xfc4b));`

```cpp
XI_API undefined4 __cdecl fn_1026D70C(int param_1)
{
  uint32_t uVar1;
  
  *(uint32_t *)(param_1 + 0x30) = 0xffffffff;
  uVar1 = orig::FUN_1026d634(param_1 + 0x20,param_1);
  uVar1 = orig::FUN_10316d6b(uVar1);
  return uVar1;
}
```

## `0x1026D866` fn_1026D866

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026CCA5`, `sub_1026CDF4`, `sub_1026D1B4` x2, `sub_10316D6B`
- **Called by:** `fn_101152F0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115500` → `fn_101154D0` → `fn_101152F0` → `fn_1026D866`
- **How its callers call it** (their verified code):
  - in `fn_101152F0`: `iVar5 = orig::FUN_1026d866((int)(puVar3 + 0xbc35),&local_8,(uint8_t *)((int)puVar3 + 0x16d1),1,`

```cpp
XI_API undefined4 __cdecl fn_1026D866(int param_1, undefined4 * param_2, uint8_t * param_3, int param_4, uint8_t * param_5, int param_6, int param_7)
{
  uint32_t uVar1;
  char xi_frame[28] __attribute__((aligned(4)));
  int  &local_1c = *reinterpret_cast<int  *>(xi_frame + 0);
  int  &local_18 = *reinterpret_cast<int  *>(xi_frame + 4);
  uint8_t * &local_14 = *reinterpret_cast<uint8_t * *>(xi_frame + 8);
  uint8_t * &local_10 = *reinterpret_cast<uint8_t * *>(xi_frame + 12);
  uint8_t * &local_c = *reinterpret_cast<uint8_t * *>(xi_frame + 16);
  uint8_t * &local_8 = *reinterpret_cast<uint8_t * *>(xi_frame + 20);
  
  local_14 =(uint8_t*)(&DAT_cccccccc);
  local_10 =(uint8_t*)(&DAT_cccccccc);
  local_c =(uint8_t*)(&DAT_cccccccc);
  local_8 =(uint8_t*)(&DAT_cccccccc);
  for (local_18 = 0; local_18 < param_4; local_18 = local_18 + 1) {
    orig::FUN_1026cca5((void *)(param_1 + local_18 * 0x10058),0);
    local_10 = param_5;
    local_c = (uint8_t *)0x0;
    for (local_1c = 0; local_14 = param_3, local_1c < param_6; local_1c = local_1c + 1) {
      orig::FUN_1026d1b4(param_1 + 0x803c + local_18 * 0x10058,&local_14);
      param_3 = param_3 + (int)param_5;
    }
    for (local_1c = 0; param_3 = local_14, local_1c < param_7; local_1c = local_1c + 1) {
      orig::FUN_1026d1b4(param_1 + 0x4034 + local_18 * 0x10058,&local_14);
      local_14 = param_3 + (int)param_5;
    }
    if (param_2[1] != 0) {
      orig::FUN_1026cdf4(param_1 + local_18 * 0x10058,*param_2,param_2[1]);
    }
    param_2 = param_2 + 2;
  }
  local_8 = (uint8_t *)0x1026d9ad;
  uVar1 = orig::FUN_10316d6b(1);
  return uVar1;
}
```

## `0x1026DB6D` fn_1026DB6D

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026D0CA`, `sub_10316D6B`
- **Called by:** `sub_10115D50`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `sub_10115D50` → `fn_1026DB6D`

```cpp
XI_API undefined4 __cdecl fn_1026DB6D(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_1026d0ca(param_1 + 0xc044,param_2);
  uVar1 = orig::FUN_10316d6b(uVar1);
  return uVar1;
}
```

## `0x1026DB8F` fn_1026DB8F

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `FontGlyphCache_PeekRing` (low), `sub_10316D6B`
- **Called by:** `sub_10115D50`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `sub_10115D50` → `fn_1026DB8F`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `FontGlyphCache_PeekRing` (low): Returns the value at the head of the ring, or -1 when it is empty.

Short callee `FontGlyphCache_PeekRing` (low):
```cpp
XI_API undefined4 __cdecl FontGlyphCache_PeekRing(int param_1)
{
  uint32_t uVar1;
  
  if (*(int *)(param_1 + 0x4004) == *(int *)(param_1 + 0x4000)) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = *(uint32_t *)(param_1 + 8 + *(int *)(param_1 + 0x4004) * 0x10);
  }
  return uVar1;
}
```

```cpp
XI_API void __cdecl fn_1026DB8F(int param_1)
{
  uint32_t xi_eax;
  
  xi_eax = FontGlyphCache_PeekRing(param_1 + 0xc044);
  orig::FUN_10316d6b(xi_eax);
  return;
}
```

## `0x1026DBAC` fn_1026DBAC

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1026D1B4`, `sub_10316D6B`
- **Called by:** `sub_10115D50`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `sub_10115D50` → `fn_1026DBAC`

```cpp
XI_API void __cdecl fn_1026DBAC(int param_1, undefined4 param_2)
{
  uint32_t xi_eax;
  
  xi_eax = orig::FUN_1026d1b4(param_1 + 0x803c,param_2);
  orig::FUN_10316d6b(xi_eax);
  return;
}
```

## `0x1026DBCE` fn_1026DBCE

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026D0CA`, `sub_10316D6B`
- **Called by:** `fn_10115580`, `fn_10115610`, `fn_10115690`, `sub_10115770`, `fn_10115A00`, `fn_10115A90`, `fn_10115B20`, `fn_10115BB0`, `fn_10115C30`, `fn_10115CC0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115610` → `fn_1026DBCE`
- **How its callers call it** (their verified code):
  - in `fn_10115580`: `orig::FUN_1026dbce(iVar1,local_10);`
  - in `fn_10115610`: `orig::FUN_1026dbce(iVar1,local_10);`
  - in `fn_10115690`: `orig::FUN_1026dbce(iVar1,local_10);`
  - in `fn_10115A00`: `orig::FUN_1026dbce(iVar1,local_10);`
  - in `fn_10115A90`: `orig::FUN_1026dbce(iVar1,local_10);`

```cpp
XI_API void __cdecl fn_1026DBCE(int param_1, undefined4 param_2)
{
  uint32_t xi_eax;
  
  xi_eax = orig::FUN_1026d0ca(param_1 + 0x4034,param_2);
  orig::FUN_10316d6b(xi_eax);
  return;
}
```

## `0x1026DBF0` fn_1026DBF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `FontGlyphCache_PeekRing` (low), `sub_10316D6B`
- **Called by:** `fn_10115580`, `fn_10115610`, `fn_10115690`, `sub_10115770`, `fn_10115A00`, `fn_10115A90`, `fn_10115B20`, `fn_10115BB0`, `fn_10115C30`, `fn_10115CC0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115610` → `fn_1026DBF0`
- **How its callers call it** (their verified code):
  - in `fn_10115580`: `iVar2 = orig::FUN_1026dbf0(iVar1);`
  - in `fn_10115610`: `iVar3 = orig::FUN_1026dbf0(iVar1);`
  - in `fn_10115690`: `iVar3 = orig::FUN_1026dbf0(iVar1);`
  - in `fn_10115A00`: `iVar3 = orig::FUN_1026dbf0(iVar1);`
  - in `fn_10115A90`: `iVar3 = orig::FUN_1026dbf0(iVar1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `FontGlyphCache_PeekRing` (low): Returns the value at the head of the ring, or -1 when it is empty.

Short callee `FontGlyphCache_PeekRing` (low):
```cpp
XI_API undefined4 __cdecl FontGlyphCache_PeekRing(int param_1)
{
  uint32_t uVar1;
  
  if (*(int *)(param_1 + 0x4004) == *(int *)(param_1 + 0x4000)) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = *(uint32_t *)(param_1 + 8 + *(int *)(param_1 + 0x4004) * 0x10);
  }
  return uVar1;
}
```

```cpp
XI_API undefined4 __cdecl fn_1026DBF0(int param_1)
{
  uint32_t uVar1;
  
  uVar1 = FontGlyphCache_PeekRing(param_1 + 0x4034);
  uVar1 = orig::FUN_10316d6b(uVar1);
  return uVar1;
}
```

## `0x1026DC0D` fn_1026DC0D

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1026D1B4`, `sub_10316D6B`
- **Called by:** `fn_10115580`, `fn_10115610`, `fn_10115690`, `sub_10115770`, `fn_10115A00`, `fn_10115A90`, `fn_10115B20`, `fn_10115BB0`, `fn_10115C30`, `fn_10115CC0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_100DA750` → `sub_100ED7E0` → `fn_10115610` → `fn_1026DC0D`
- **How its callers call it** (their verified code):
  - in `fn_10115580`: `orig::FUN_1026dc0d(iVar1,local_10);`
  - in `fn_10115610`: `orig::FUN_1026dc0d(iVar1,local_10);`
  - in `fn_10115690`: `orig::FUN_1026dc0d(iVar1,local_10);`
  - in `fn_10115A00`: `orig::FUN_1026dc0d(iVar1,local_10);`
  - in `fn_10115A90`: `orig::FUN_1026dc0d(iVar1,local_10);`

```cpp
XI_API void __cdecl fn_1026DC0D(int param_1, undefined4 param_2)
{
  uint32_t xi_eax;
  
  xi_eax = orig::FUN_1026d1b4(param_1 + 0x2c,param_2);
  orig::FUN_10316d6b(xi_eax);
  return;
}
```
