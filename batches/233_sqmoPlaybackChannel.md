# Batch 233_sqmoPlaybackChannel: 3 functions of `sqmoPlaybackChannel`

Name these functions: see `batches/README.md`. Write `data/names/233_sqmoPlaybackChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102924F0` fn_102924F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_10292520`
- **Called by:** `fn_102853F0`, `sub_10292B80`, `sub_10293D00`

```cpp
XI_API undefined4 * __cdecl fn_102924F0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x5c,s_C__dev_dancer_modules_sqMotion_s_103bb254,0x109);
  orig::FUN_10292520(puVar1);
  return puVar1;
}
```

## `0x10292520` fn_10292520

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102924F0`

```cpp
XI_API void __cdecl fn_10292520(undefined4 * param_1)
{
  *param_1 = 0x50003;
  param_1[2] = &LAB_10292590;
  param_1[3] = XI_CODE(FUN_102925a0);
  param_1[4] = XI_CODE(FUN_10292740);
  param_1[5] = &LAB_10292a40;
  param_1[6] = &LAB_10292b70;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[10] = 1;
  param_1[9] = 0;
  param_1[0xb] = 0;
  param_1[0x12] = 0;
  param_1[0xe] = 0;
  param_1[0x11] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0x13] = 0;
  param_1[0x14] = 0;
  *(uint8_t *)(param_1 + 0x15) = 0;
  *(uint8_t *)((int)param_1 + 0x55) = 0;
  param_1[0x16] = 0;
  return;
}
```

## `0x10293000` fn_10293000

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1027D650`
- **Called by:** `fn_102853F0`, `sub_10292C00`, `sub_10293E40`

```cpp
XI_API void __cdecl fn_10293000(int param_1, int param_2)
{
  orig::FUN_1027d650(*(int *)(param_1 + 0x50),*(int *)(param_1 + 0x1c),param_2,
               (uint8_t *)(param_1 + 0x54));
  return;
}
```
