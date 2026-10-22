# Batch 222_sqimImage: 2 functions of `sqimImage`

Name these functions: see `batches/README.md`. Write `data/names/222_sqimImage.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102984B0` fn_102984B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_10298570`
- **Called by:** `sub_10298C90`, `sub_10298D30`, `sub_10298DA0`, `fn_10298E10`

```cpp
XI_API undefined4 * __cdecl fn_102984B0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x3c,s_C__dev_dancer_modules_sqImage_sr_103bbc70,0x1b);
  orig::FUN_10298570(puVar1);
  return puVar1;
}
```

## `0x10298570` fn_10298570

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102984B0`

```cpp
XI_API void __cdecl fn_10298570(undefined4 * param_1)
{
  *(uint8_t *)(param_1 + 5) = 0;
  param_1[6] = 0;
  *param_1 = 0x180001;
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[7] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 3;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  return;
}
```
