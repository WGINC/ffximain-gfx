# Batch 214_sqMatrix4: 2 functions of `sqMatrix4`

Name these functions: see `batches/README.md`. Write `data/names/214_sqMatrix4.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10280650` fn_10280650

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1027EBD0`
- **Called by:** `sub_102712E0`, `sub_10291AD0`

```cpp
XI_API void __cdecl fn_10280650(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  orig::FUN_1027ebd0(param_1);
  *(uint32_t *)(param_1 + 0xc) = param_2;
  *(uint32_t *)(param_1 + 0x1c) = param_3;
  *(uint32_t *)(param_1 + 0x2c) = param_4;
  return;
}
```

## `0x102806B0` fn_102806B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270F30`, `fn_10271630`, `fn_10271F20`, `sub_10272520`, `fn_102725A0`, `sub_10291AD0`

```cpp
XI_API undefined4 __cdecl fn_102806B0(int param_1, float param_2, float param_3, float param_4)
{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  
  iVar3 = 3;
  pfVar1 = (float *)(param_1 + 8);
  do {
    pfVar2 = pfVar1 + 4;
    iVar3 = iVar3 + -1;
    pfVar1[-2] = (float)((long double)param_2 * (long double)pfVar1[-2]);
    pfVar1[-1] = (float)((long double)param_3 * (long double)pfVar1[-1]);
    *pfVar1 = (float)((long double)param_4 * (long double)*pfVar1);
    pfVar1 = pfVar2;
  } while (iVar3 != 0);
  return pfVar2;
}
```
