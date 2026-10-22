# Batch 253_sqxfXformM: 1 functions of `sqxfXformM`

Name these functions: see `batches/README.md`. Write `data/names/253_sqxfXformM.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10289F50` fn_10289F50

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sqxfXformTD_Init`, `sub_10289F30`, `fn_10291390`

```cpp
XI_API void __cdecl fn_10289F50(undefined4 * param_1)
{
  *param_1 = 0xc0001;
  param_1[2] = &LAB_10289fa0;
  param_1[3] = XI_CODE(FUN_10289fb0);
  param_1[4] = XI_CODE(FUN_1028a040);
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &DAT_1028a0a0;
  orig::FUN_1027ebd0(param_1 + 7);
  return;
}
```
