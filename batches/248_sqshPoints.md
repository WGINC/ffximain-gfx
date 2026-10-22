# Batch 248_sqshPoints: 2 functions of `sqshPoints`

Name these functions: see `batches/README.md`. Write `data/names/248_sqshPoints.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102ACB60` fn_102ACB60

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102ACB80`
- **Called by:** `sub_10299E40`, `sub_102ACCB0`, `sub_102ACD40`

```cpp
XI_API undefined4 * __cdecl fn_102ACB60(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x44,s_C__dev_dancer_modules_sqShape_sr_103bec10,0x1d);
  orig::FUN_102acb80(puVar1);
  return puVar1;
}
```

## `0x102ACB80` fn_102ACB80

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1028D9A0`
- **Called by:** `fn_102ACB60`

```cpp
XI_API void __cdecl fn_102ACB80(undefined4 * param_1)
{
  *param_1 = 0xa0005;
  param_1[2] = &LAB_102acbd0;
  param_1[3] = XI_CODE(FUN_102acbe0);
  param_1[4] = XI_CODE(sqshPointsPrint);
  param_1[0xb] = &LAB_10291260;
  param_1[0xc] = &LAB_102b3770;
  orig::FUN_1028d9a0(param_1 + 5);
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  return;
}
```
