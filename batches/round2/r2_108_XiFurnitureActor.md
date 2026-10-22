# Batch r2_108_XiFurnitureActor: 1 functions of `XiFurnitureActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_108_XiFurnitureActor.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100C3E70` fn_100C3E70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `thunk_sub_10028AA0`, `sub_10085240`, `sub_10085270`, `fn_1008A7B0`
- **Called by:** `fn_101CD900`
- **Named before** (a naming session's reading, low): `XiFurnitureActor_Release`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `Acter_ShutdownGlobals` → `YkMyroom_Destructor` → `Unit101D2FF0_ReleaseTable` → `fn_101CD900` → `XiFurnitureActor_Release`
- **Named callers' callers:** `Unit101D2FF0_ReleaseTable`
- **Named callees' callees:** `CYyObject_IsKindOf`
- **How its callers call it** (their verified code):
  - in `fn_101CD900`: `XiFurnitureActor_Release(pvVar3);`

Short callee `thunk_sub_10028AA0`:
```cpp
XI_API void __cdecl thunk_sub_10028AA0(int param_1)
{
  orig::FUN_10028aa0(param_1);
  return;
}
```

Short callee `fn_1008A7B0`:
```cpp
XI_API void __thiscall fn_1008A7B0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYyObject_GetRuntimeClass_variant18_1032d69c;
  orig::FUN_1008ee30(self_);
  Acter_ReleaseAllSlots(self_);
  return;
}
```

```cpp
XI_API void __cdecl XiFurnitureActor_Release(void * param_1)
{
  uint16_t uVar1;
  void *pvVar2;
  uint64_t uVar3;
  
  uVar3 = orig::FUN_10085240();
  pvVar2 = (void *)uVar3;
  if (pvVar2 != (void *)0x0) {
    while (pvVar2 != param_1) {
      uVar3 = orig::FUN_10085270(pvVar2);
      pvVar2 = (void *)uVar3;
      if (pvVar2 == (void *)0x0) {
        return;
      }
    }
    pvVar2 = *(void **)((int)param_1 + 0x70);
    if (pvVar2 != (void *)0x0) {
      uVar1 = *(uint16_t *)((int)pvVar2 + 0x74);
      orig::FUN_1008a7b0(pvVar2);
      orig::thunk_sub_10028AA0_v1((int)pvVar2);
      (&ActerBuffPtr_as_v0)[uVar1] = 0;
    }
  }
  return;
}
```
