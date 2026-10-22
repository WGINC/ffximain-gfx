# Batch r2_116_sqMatrix4: 1 functions of `sqMatrix4`

Name these functions: see `batches/README.md`. Write `data/names/r2_116_sqMatrix4.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10280650` fn_10280650

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1027EBD0`
- **Called by:** `sub_102712E0`, `sub_10291AD0`
- **Named before** (a naming session's reading, low): `SqMatrix4_SetFields`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `StAvatar_Start` → `StAvatar_SelectEntry` → `sub_102712E0` → `SqMatrix4_SetFields`
- **Named callers' callers:** `StAvatar_ApplyModes`, `StAvatar_SelectEntry`
- **Named callees' callees:** `Unit1027E120_Zero`

```cpp
XI_API void __cdecl SqMatrix4_SetFields(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  orig::FUN_1027ebd0(param_1);
  *(uint32_t *)(param_1 + 0xc) = param_2;
  *(uint32_t *)(param_1 + 0x1c) = param_3;
  *(uint32_t *)(param_1 + 0x2c) = param_4;
  return;
}
```
