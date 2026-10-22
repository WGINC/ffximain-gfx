# Batch r2_110_YkFurniture: 2 functions of `YkFurniture`

Name these functions: see `batches/README.md`. Write `data/names/r2_110_YkFurniture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101CCE00` fn_101CCE00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit100C51C0_ForwardToCce00` (low), `sub_101CD3B0`, `sub_101CDA60`
- **Named before** (a naming session's reading, low): `Yk_MapCodeOffset`; name it again from this dossier, or leave it out to keep that name
- **Named callers' callers:** `Unit100C51C0_MapCodeIndex`
- **How its callers call it** (their verified code):
  - in `Unit100C51C0_ForwardToCce00` (low): `return Yk_MapCodeOffset(param_1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit100C51C0_ForwardToCce00` (low): Forwards its argument to FUN_101cce00.

```cpp
XI_API int __cdecl Yk_MapCodeOffset(int param_1)
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
- **Calls:** `sub_100AB320`, `Unit100AB450_ReleaseSlot` (medium), `XiFurnitureActor_Release` (low), `Unit101CD990_GetValueAt54` (low) x2, `Unit101CD990_GetValueAt56` (low) x2, `Unit101CD990_GetValueAt5A` (low) x2
- **Called by:** `sub_101D3A70`, `Unit101D2FF0_ReleaseTable` (low), `sub_101D5140`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `Acter_ShutdownGlobals` → `YkMyroom_Destructor` → `Unit101D2FF0_ReleaseTable` → `fn_101CD900`
- **Named callers' callers:** `YkMyroom_Destructor`
- **Named callees' callees:** `thunk_sub_10028AA0`
- **How its callers call it** (their verified code):
  - in `Unit101D2FF0_ReleaseTable` (low): `orig::FUN_101cd900(this_00);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit100AB450_ReleaseSlot` (medium): Frees the actor held at a slot and clears the slot.
  - `XiFurnitureActor_Release` (low): Removes the furniture actor from the list and releases its model object.
  - `Unit101CD990_GetValueAt54` (low): Returns the value at offset 0xA0 of the acter selected by the index at offset 0x54.
  - `Unit101CD990_GetValueAt56` (low): Returns the value at offset 0xA0 of the acter selected by the index at offset 0x56.
  - `Unit101CD990_GetValueAt5A` (low): Returns the value at offset 0xA0 of the acter selected by the index at offset 0x5A.
  - `Unit101D2FF0_ReleaseTable` (low): Releases every actor held in a table.

Short callee `Unit101CD990_GetValueAt54` (low):
```cpp
XI_API undefined4 __thiscall Unit101CD990_GetValueAt54(void * self_)
{
  if ((*(uint16_t *)((int)self_ + 0x54) != 0) &&
     ((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x54)] != 0)) {
    return *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x54)] + 0xa0);
  }
  return 0;
}
```

Short callee `Unit101CD990_GetValueAt56` (low):
```cpp
XI_API undefined4 __thiscall Unit101CD990_GetValueAt56(void * self_)
{
  if ((*(uint16_t *)((int)self_ + 0x56) != 0) &&
     ((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x56)] != 0)) {
    return *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x56)] + 0xa0);
  }
  return 0;
}
```

Short callee `Unit101CD990_GetValueAt5A` (low):
```cpp
XI_API undefined4 __thiscall Unit101CD990_GetValueAt5A(void * self_)
{
  if ((*(uint16_t *)((int)self_ + 0x5a) != 0) &&
     ((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x5a)] != 0)) {
    return *(uint32_t *)((&ActerBuffPtr_as_v0)[*(uint16_t *)((int)self_ + 0x5a)] + 0xa0);
  }
  return 0;
}
```

```cpp
XI_API void __thiscall fn_101CD900(void * self_)
{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  
  *(uint8_t ***)self_ = &PTR_YkFurniture_GetRuntimeClass_10337e08;
  iVar2 = Unit101CD990_GetValueAt5A(self_);
  if (iVar2 != 0) {
    iVar2 = Unit101CD990_GetValueAt5A(self_);
    Unit100AB450_ReleaseSlot(iVar2);
    *(uint16_t *)((int)self_ + 0x5a) = 0;
  }
  iVar2 = Unit101CD990_GetValueAt54(self_);
  if (iVar2 != 0) {
    pvVar3 = (void *)Unit101CD990_GetValueAt54(self_);
    XiFurnitureActor_Release(pvVar3);
    *(uint16_t *)((int)self_ + 0x54) = 0;
  }
  iVar2 = Unit101CD990_GetValueAt56(self_);
  if (iVar2 != 0) {
    iVar2 = Unit101CD990_GetValueAt56(self_);
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
