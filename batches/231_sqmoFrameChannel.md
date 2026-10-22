# Batch 231_sqmoFrameChannel: 3 functions of `sqmoFrameChannel`

Name these functions: see `batches/README.md`. Write `data/names/231_sqmoFrameChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10293F80` fn_10293F80

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_10293FA0`
- **Called by:** `sub_1027E1D0`, `fn_102851C0`, `sub_10294520`, `sub_10294700`, `sub_102A46D0`

```cpp
XI_API undefined4 * __cdecl fn_10293F80(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x40,s_C__dev_dancer_modules_sqMotion_s_103bb558,0x1c);
  orig::FUN_10293fa0(puVar1);
  return puVar1;
}
```

## `0x10293FA0` fn_10293FA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10293F80`

```cpp
XI_API void __cdecl fn_10293FA0(undefined4 * param_1)
{
  *param_1 = 0x50001;
  param_1[2] = &LAB_10293ff0;
  param_1[3] = XI_CODE(FUN_10294000);
  param_1[4] = XI_CODE(sqmoFrameChannel_Dump);
  param_1[5] = &LAB_102942b0;
  param_1[6] = &LAB_102942f0;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 1;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  *(uint8_t *)(param_1 + 0xf) = 0;
  return;
}
```

## `0x102944E0` fn_102944E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1027D650`
- **Called by:** `fn_102851C0`, `sub_102945A0`, `sub_102947F0`, `sub_102A47E0`

```cpp
XI_API void __cdecl fn_102944E0(int param_1, int param_2)
{
  orig::FUN_1027d650(*(int *)(param_1 + 0x38),*(int *)(param_1 + 0x1c),param_2,
               (uint8_t *)(param_1 + 0x3c));
  return;
}
```
