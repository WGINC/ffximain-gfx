# Batch 239_sqrdTexture: 2 functions of `sqrdTexture`

Name these functions: see `batches/README.md`. Write `data/names/239_sqrdTexture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10298B30` fn_10298B30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_10298B90`
- **Called by:** `sub_10298DA0`, `fn_10298E10`, `sub_1029D3C0`

```cpp
XI_API undefined4 * __cdecl fn_10298B30(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x24,s_C__dev_dancer_modules_sqRend_src_103bbde4,0x22);
  orig::FUN_10298b90(puVar1);
  return puVar1;
}
```

## `0x10298B90` fn_10298B90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1028E350`, `fn_10298B30`

```cpp
XI_API void __cdecl fn_10298B90(undefined4 * param_1)
{
  param_1[5] = 0xffffffff;
  param_1[6] = 0xffffffff;
  *param_1 = 0x70004;
  param_1[2] = &LAB_10298b50;
  param_1[3] = XI_CODE(FUN_10298b60);
  param_1[4] = XI_CODE(sqrdTexturePrint);
  param_1[7] = 0;
  param_1[8] = 0;
  return;
}
```
