# Batch r2_118_sqdfShape: 1 functions of `sqdfShape`

Name these functions: see `batches/README.md`. Write `data/names/r2_118_sqdfShape.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AF170` fn_102AF170

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_102AF140`, `sqinCluster_Init`, `sub_102AF790`
- **From the frame root** (shortest chain of direct calls): `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029AA80` → `sqinSkinBuildBinary` → `sqinCluster_Create` → `sqinCluster_Init` → `fn_102AF170`
- **Named callers' callers:** `sqinCluster_Create`
- **How its callers call it** (their verified code):
  - in `sqinCluster_Init`: `orig::FUN_102af170(param_1 + 7);`

```cpp
XI_API void __cdecl fn_102AF170(undefined4 * param_1)
{
  *param_1 = 0x30008;
  param_1[2] = &LAB_102af290;
  param_1[3] = XI_CODE(FUN_102af2a0);
  param_1[4] = &LAB_102af2e0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  Unit10274050_ZeroDwords(param_1 + 10);
  return;
}
```
