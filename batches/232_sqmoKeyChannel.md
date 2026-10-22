# Batch 232_sqmoKeyChannel: 2 functions of `sqmoKeyChannel`

Name these functions: see `batches/README.md`. Write `data/names/232_sqmoKeyChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102957B0` fn_102957B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102957D0`
- **Called by:** `sub_1027D9B0`, `fn_102852E0`, `sub_10295C30`

```cpp
XI_API undefined4 * __cdecl fn_102957B0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x38,s_C__dev_dancer_modules_sqMotion_s_103bb964,0x1b);
  orig::FUN_102957d0(puVar1);
  return puVar1;
}
```

## `0x102957D0` fn_102957D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102957B0`

```cpp
XI_API void __cdecl fn_102957D0(undefined4 * param_1)
{
  *param_1 = 0x50002;
  param_1[2] = &LAB_10295820;
  param_1[3] = XI_CODE(FUN_10295830);
  param_1[4] = XI_CODE(sqmoKeyChannel_Dump);
  param_1[5] = XI_CODE(FUN_10295a20);
  param_1[6] = &LAB_10295b40;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 1;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  return;
}
```
