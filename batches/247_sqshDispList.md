# Batch 247_sqshDispList: 2 functions of `sqshDispList`

Name these functions: see `batches/README.md`. Write `data/names/247_sqshDispList.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102A2000` fn_102A2000

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102A2020`
- **Called by:** `sqsdGeneric_BakeOntoShape`, `sqsdSampling_BakeOntoShape`, `sqsdAniso_BakeOntoShape`, `sqsdGlow_BakeOntoShape`

```cpp
XI_API undefined4 * __cdecl fn_102A2000(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x48,s_C__dev_dancer_modules_sqShape_sr_103bd498,0x20);
  orig::FUN_102a2020(puVar1);
  return puVar1;
}
```

## `0x102A2020` fn_102A2020

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1028D9A0`
- **Called by:** `fn_102A2000`

```cpp
XI_API void __cdecl fn_102A2020(undefined4 * param_1)
{
  *param_1 = 0xa000b;
  param_1[2] = &LAB_102a2070;
  param_1[3] = XI_CODE(FUN_102a2080);
  param_1[4] = XI_CODE(sqshDispListPrint);
  param_1[0xb] = 0;
  param_1[0xc] = &LAB_102b3320;
  orig::FUN_1028d9a0(param_1 + 5);
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  return;
}
```
