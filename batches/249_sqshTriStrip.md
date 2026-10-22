# Batch 249_sqshTriStrip: 3 functions of `sqshTriStrip`

Name these functions: see `batches/README.md`. Write `data/names/249_sqshTriStrip.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AA2D0` fn_102AA2D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102AA2F0`
- **Called by:** `sub_10299C30`, `sub_102ABFC0`, `sub_102AC110`

```cpp
XI_API undefined4 * __cdecl fn_102AA2D0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x60,s_C__dev_dancer_modules_sqShape_sr_103be980,0x28);
  orig::FUN_102aa2f0(puVar1);
  return puVar1;
}
```

## `0x102AA2F0` fn_102AA2F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1028D9A0`
- **Called by:** `fn_102AA2D0`

```cpp
XI_API void __cdecl fn_102AA2F0(undefined4 * param_1)
{
  *param_1 = 0xa0009;
  param_1[2] = &LAB_102aa360;
  param_1[3] = XI_CODE(FUN_102aa370);
  param_1[4] = XI_CODE(FUN_102aa420);
  param_1[0xb] = &LAB_10291260;
  param_1[0xc] = &LAB_102b3090;
  orig::FUN_1028d9a0(param_1 + 5);
  param_1[0xd] = 0;
  param_1[0xe] = XI_CODE(FUN_102aa580);
  param_1[0xf] = &LAB_102aaa60;
  param_1[0x10] = &LAB_102aade0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  return;
}
```

## `0x102ABEB0` fn_102ABEB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10299C30`
- **Reads globals:** `0x109AAAC0`

```cpp
XI_API undefined4 __cdecl fn_102ABEB0(void)
{
  return DAT_109aaac0;
}
```
