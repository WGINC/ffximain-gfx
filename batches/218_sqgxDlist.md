# Batch 218_sqgxDlist: 2 functions of `sqgxDlist`

Name these functions: see `batches/README.md`. Write `data/names/218_sqgxDlist.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029B5C0` fn_1029B5C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`
- **Called by:** `sqsdGeneric_BakeOntoShape`, `sqsdSampling_BakeOntoShape`, `sqsdAniso_BakeOntoShape`, `sqsdGlow_BakeOntoShape`
- **Reads globals:** `0x1099D200`

```cpp
XI_API void __cdecl fn_1029B5C0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x2c,aCDevDancerModu_30,0x1a);
  *(undefined4 **)(g_pSqgxContext_as_s32 + 0xa04) = puVar1;
  *(undefined4 *)(g_pSqgxContext_as_s32 + 0xa08) = 0;
  *(undefined4 *)(g_pSqgxContext_as_s32 + 0xa0c) = 0;
  *(undefined4 *)(g_pSqgxContext_as_s32 + 0xa10) = 0;
  *(undefined4 *)(g_pSqgxContext_as_s32 + 0xa14) = 0;
  return;
}
```

## `0x1029BB50` fn_1029BB50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102A2130`

```cpp
XI_API void __cdecl fn_1029BB50(void)
{
  return;
}
```
