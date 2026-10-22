# Batch 250_sqskJoint: 3 functions of `sqskJoint`

Name these functions: see `batches/README.md`. Write `data/names/250_sqskJoint.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10291360` fn_10291360

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_10291390`
- **Called by:** `sub_10288FF0`, `sub_1029FFF0`

```cpp
XI_API undefined4 * __cdecl fn_10291360(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x9c,s_C__dev_dancer_modules_sqSkeleton_103bae5c,0x1e);
  orig::FUN_10291390(puVar1);
  return puVar1;
}
```

## `0x10291390` fn_10291390

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10274640`, `fn_10274670`, `fn_1027A6F0`, `fn_10289F50`
- **Called by:** `fn_1027C4C0`, `fn_10291360`

```cpp
XI_API void __cdecl fn_10291390(undefined4 * param_1)
{
  orig::FUN_10289f50(param_1);
  *param_1 = 0xb0002;
  param_1[2] = &LAB_10291420;
  param_1[3] = XI_CODE(FUN_10291430);
  param_1[4] = XI_CODE(FUN_10291470);
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &LAB_10291ad0;
  orig::FUN_10274640(param_1 + 0x17);
  orig::thunk_FUN_1027ae50(param_1 + 0x1a);
  orig::FUN_10274670((float *)(param_1 + 0x1e),1.0f,1.0f,1.0f);
  param_1[0x21] = 0;
  param_1[0x22] = 0;
  param_1[0x23] = 0;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  return;
}
```

## `0x10291460` fn_10291460

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1027C4C0`, `sub_10291430`

```cpp
XI_API void __cdecl fn_10291460(void)
{
  return;
}
```
