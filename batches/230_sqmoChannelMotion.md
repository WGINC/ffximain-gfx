# Batch 230_sqmoChannelMotion: 2 functions of `sqmoChannelMotion`

Name these functions: see `batches/README.md`. Write `data/names/230_sqmoChannelMotion.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10295E70` fn_10295E70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_10295E90`
- **Called by:** `fn_102960A0`

```cpp
XI_API undefined4 * __cdecl fn_10295E70(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x2c,s_C__dev_dancer_modules_sqMotion_s_103bbab4,0x1b);
  orig::FUN_10295e90(puVar1);
  return puVar1;
}
```

## `0x10295E90` fn_10295E90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10295E70`

```cpp
XI_API void __cdecl fn_10295E90(undefined4 * param_1)
{
  *param_1 = 0x50004;
  param_1[2] = &LAB_10295ee0;
  param_1[3] = XI_CODE(FUN_10295ef0);
  param_1[4] = XI_CODE(FUN_10295f30);
  param_1[5] = &LAB_10295ff0;
  param_1[6] = &LAB_10296090;
  param_1[7] = &LAB_10296080;
  param_1[8] = 0;
  param_1[9] = 0x3f800000;
  param_1[10] = 0;
  return;
}
```
