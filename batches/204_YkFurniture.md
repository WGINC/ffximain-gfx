# Batch 204_YkFurniture: 2 functions of `YkFurniture`

Name these functions: see `batches/README.md`. Write `data/names/204_YkFurniture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101CCE00` fn_101CCE00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_100C5290`, `sub_101CD3B0`, `sub_101CDA60`

```cpp
XI_API int __cdecl fn_101CCE00(int param_1)
{
  int iVar1;
  
  iVar1 = 0x809f;
  if (param_1 < 0x100) {
    return param_1 + 0x809f;
  }
  if (param_1 < 0x200) {
    return param_1 + 0xc7ef;
  }
  if (param_1 < 0xe00) {
    return 0;
  }
  if ((param_1 < 0xf00) || (param_1 < 0x1000)) {
    iVar1 = param_1 + 0xe75b;
  }
  return iVar1;
}
```

## `0x101CD900` fn_101CD900

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100AB320`, `fn_100AB550`, `fn_100C3E70`, `fn_101CD9D0` x2, `fn_101CDA00` x2, `fn_101CDA30` x2
- **Called by:** `sub_101D3A70`, `fn_101D3DC0`, `sub_101D5140`

```cpp
XI_API void __thiscall fn_101CD900(void * self_)
{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  
  *(uint8_t ***)self_ = &PTR_YkFurniture_GetRuntimeClass_10337e08;
  iVar2 = orig::FUN_101cda30(self_);
  if (iVar2 != 0) {
    iVar2 = orig::FUN_101cda30(self_);
    orig::FUN_100ab550(iVar2);
    *(uint16_t *)((int)self_ + 0x5a) = 0;
  }
  iVar2 = orig::FUN_101cd9d0(self_);
  if (iVar2 != 0) {
    pvVar3 = (void *)orig::FUN_101cd9d0(self_);
    orig::FUN_100c3e70(pvVar3);
    *(uint16_t *)((int)self_ + 0x54) = 0;
  }
  iVar2 = orig::FUN_101cda00(self_);
  if (iVar2 != 0) {
    iVar2 = orig::FUN_101cda00(self_);
    orig::FUN_100ab320(iVar2);
    *(uint16_t *)((int)self_ + 0x56) = 0;
  }
  piVar1 = *(int **)((int)self_ + 100);
  *(uint16_t *)((int)self_ + 0x58) = 0;
  if (piVar1 != (int *)0x0) {
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*piVar1 + 0x18))))((void *)(piVar1),1);
    *(uint32_t *)((int)self_ + 100) = 0;
  }
  piVar1 = *(int **)((int)self_ + 0x68);
  if (piVar1 != (int *)0x0) {
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*piVar1 + 0x18))))((void *)(piVar1),1);
    *(uint32_t *)((int)self_ + 0x68) = 0;
  }
  return;
}
```
