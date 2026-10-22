# Batch 242_sqsdGeneric: 1 functions of `sqsdGeneric`

Name these functions: see `batches/README.md`. Write `data/names/242_sqsdGeneric.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10290910` fn_10290910

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270B60`, `sqsdGeneric_Create`

```cpp
XI_API void __cdecl fn_10290910(undefined4 * param_1)
{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  param_1[0x20] = 0x3f000000;
  param_1[0x1b] = 0x3f000000;
  param_1[0x28] = 1;
  param_1[0x1d] = 0x3f800000;
  param_1[0x1f] = 0x3f800000;
  param_1[0x22] = 1;
  param_1[0x23] = 1;
  param_1[0x26] = 1;
  *param_1 = 0xf0001;
  param_1[2] = &LAB_102908d0;
  param_1[3] = XI_CODE(FUN_102908e0);
  param_1[4] = &LAB_10290a80;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  param_1[0x19] = 0;
  param_1[0x1c] = 0x3ba3d70a;
  param_1[0x29] = 0;
  param_1[0x1e] = 0;
  param_1[0x21] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x2a] = 0;
  param_1[0x27] = 0;
  param_1[0x2c] = 0;
  param_1[0x2b] = 0;
  param_1[0x2d] = 0;
  puVar2 = param_1 + 0x56;
  puVar1 = param_1 + 0x2f;
  iVar3 = 10;
  do {
    puVar1[-1] = 0;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar2[-10] = 0;
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
    puVar1 = puVar1 + 3;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  param_1[0x18] = 0;
  param_1[0x1a] = 0;
  orig::FUN_102a3390(param_1 + 5);
  return;
}
```
