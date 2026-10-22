# Batch 062_CYyVb: 3 functions of `CYyVb`

Name these functions: see `batches/README.md`. Write `data/names/062_CYyVb.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1003B420` fn_1003B420

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1003B080`

```cpp
XI_API undefined4 __thiscall fn_1003B420(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYyVb_GetRuntimeClass_1032aae0;
  *(uint32_t *)((int)self_ + 8) = 0;
  return self_;
}
```

## `0x1003B430` fn_1003B430

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `thunk_sub_10028AA0`, `fn_1003B4A0`
- **In vtables** (class_vtables.json): `CYyVb` slot 6 (+0x18)

```cpp
XI_API void * __thiscall fn_1003B430(void * self_, undefined4 param_1)
{
  orig::FUN_1003b4a0(self_);
  if ((param_1 & 1) != 0) {
    orig::thunk_sub_10028AA0_v1((int)self_);
  }
  return self_;
}
```

## `0x1003B4A0` fn_1003B4A0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1003B430`
- **Reads globals:** `0x1047B978`

```cpp
XI_API void __thiscall fn_1003B4A0(void * self_)
{
  int *a1;
  
  *(uint8_t ***)self_ = &PTR_CYyVb_GetRuntimeClass_1032aae0;
  if (self_ != g_pVertexBufferListHead_as_v0) {
    a1 = *(int **)((int)self_ + 0x1c);
    if (a1 != (int *)0x0) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*a1 + 8))))(a1);
      *(uint32_t *)((int)self_ + 0x1c) = 0;
    }
    orig::FUN_1003b4e0();
    return;
  }
  return;
}
```
