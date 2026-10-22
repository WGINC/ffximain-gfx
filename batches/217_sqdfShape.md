# Batch 217_sqdfShape: 3 functions of `sqdfShape`

Name these functions: see `batches/README.md`. Write `data/names/217_sqdfShape.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AEE90` fn_102AEE90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_102AEEB0`
- **Called by:** `fn_1029A160`, `sub_1029A280`, `fn_1029A450`, `sub_102AF790`

```cpp
XI_API undefined4 * __cdecl fn_102AEE90(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x40,s_C__dev_dancer_modules_sqDeform_s_103bf324,0x22);
  orig::FUN_102aeeb0(puVar1);
  return puVar1;
}
```

## `0x102AEEB0` fn_102AEEB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102AEE90`

```cpp
XI_API void __cdecl fn_102AEEB0(undefined4 * param_1)
{
  *param_1 = 0x30009;
  param_1[2] = &LAB_102aefd0;
  param_1[3] = XI_CODE(FUN_102aefe0);
  param_1[4] = XI_CODE(FUN_102af020);
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[10] = 0;
  param_1[7] = 0;
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[0xb] = 0;
  param_1[0xd] = 0xffffffe0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  return;
}
```

## `0x102AF170` fn_102AF170

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_102AF140`, `sqinCluster_Init`, `sub_102AF790`

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
  orig::FUN_10274640(param_1 + 10);
  return;
}
```
