# Batch 237_sqrdCamera: 1 functions of `sqrdCamera`

Name these functions: see `batches/README.md`. Write `data/names/237_sqrdCamera.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10291110` fn_10291110

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270B60`, `sub_102910B0`

```cpp
XI_API void __cdecl fn_10291110(undefined4 * param_1)
{
  *param_1 = 0x70001;
  param_1[2] = &LAB_102910d0;
  param_1[3] = XI_CODE(FUN_102910e0);
  param_1[4] = XI_CODE(FUN_10291170);
  param_1[5] = 0x42340000;
  param_1[6] = 0x3dcccccd;
  param_1[7] = 0x447a0000;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0x3dcccccd;
  param_1[0xc] = 0x3f800000;
  return;
}
```
