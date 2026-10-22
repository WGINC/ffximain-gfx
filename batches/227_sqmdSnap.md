# Batch 227_sqmdSnap: 2 functions of `sqmdSnap`

Name these functions: see `batches/README.md`. Write `data/names/227_sqmdSnap.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1028DC00` fn_1028DC00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_1028DC20`
- **Called by:** `sub_1028DDC0`, `fn_1028DE10`

```cpp
XI_API undefined4 * __cdecl fn_1028DC00(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x48,s_C__dev_dancer_modules_sqModel_sr_103ba65c,0x30);
  orig::FUN_1028dc20(puVar1);
  return puVar1;
}
```

## `0x1028DC20` fn_1028DC20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1028DC00`

```cpp
XI_API void __cdecl fn_1028DC20(undefined4 * param_1)
{
  *param_1 = 0x1c0002;
  param_1[2] = &LAB_1028dc70;
  param_1[3] = XI_CODE(FUN_1028dca0);
  param_1[4] = &LAB_1028dd20;
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &LAB_1028dd50;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  return;
}
```
