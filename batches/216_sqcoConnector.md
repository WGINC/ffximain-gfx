# Batch 216_sqcoConnector: 2 functions of `sqcoConnector`

Name these functions: see `batches/README.md`. Write `data/names/216_sqcoConnector.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1028D6D0` fn_1028D6D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_1028D6F0`
- **Called by:** `fn_1028D870`

```cpp
XI_API undefined4 * __cdecl fn_1028D6D0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x40,s_C__dev_dancer_modules_sqConstrai_103ba524,0x1c);
  orig::FUN_1028d6f0(puVar1);
  return puVar1;
}
```

## `0x1028D6F0` fn_1028D6F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1027E060`, `fn_1027E3B0`, `fn_1028D6D0`

```cpp
XI_API void __cdecl fn_1028D6F0(undefined4 * param_1)
{
  *param_1 = 0x130001;
  param_1[2] = &LAB_1028d740;
  param_1[3] = XI_CODE(FUN_1028d750);
  param_1[4] = XI_CODE(sqcoConnectorPrint);
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &LAB_1028d820;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  return;
}
```
