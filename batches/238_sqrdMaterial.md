# Batch 238_sqrdMaterial: 1 functions of `sqrdMaterial`

Name these functions: see `batches/README.md`. Write `data/names/238_sqrdMaterial.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102A3390` fn_102A3390

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10290910`, `sub_10298F10`, `sub_10299320`, `sub_10299700`, `sqsdGlow_Init`, `sub_102A3330`, `sub_102B2A10`, `sub_102B2C30`

```cpp
XI_API void __cdecl fn_102A3390(undefined4 * param_1)
{
  param_1[5] = 0x3f000000;
  param_1[6] = 0x3f000000;
  param_1[7] = 0x3f000000;
  param_1[10] = 0x3f800000;
  param_1[0xb] = 0x3f800000;
  *param_1 = 0x70003;
  param_1[2] = &LAB_102a3350;
  param_1[3] = XI_CODE(FUN_102a3360);
  param_1[4] = XI_CODE(FUN_102a33f0);
  param_1[8] = 0x3e4ccccd;
  param_1[9] = 0x3f4ccccd;
  *(uint8_t *)(param_1 + 0xd) = 0;
  param_1[0xc] = 0;
  return;
}
```
