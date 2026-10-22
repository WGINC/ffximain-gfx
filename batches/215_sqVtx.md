# Batch 215_sqVtx: 5 functions of `sqVtx`

Name these functions: see `batches/README.md`. Write `data/names/215_sqVtx.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102A9520` fn_102A9520

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102A9550`
- **Called by:** `fn_10299D60`, `sub_102A95E0`, `sub_102A9A80`, `sub_102A9B90`, `sub_102AC030`, `sub_102ACCE0`, `sub_102ADA30`, `sub_102B5950`, `sub_102B63D0`, `sub_102B7250`

```cpp
XI_API undefined4 * __cdecl fn_102A9520(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x84,s_C__dev_dancer_modules_sqBase_src_103be750,0x37);
  orig::FUN_102a9550(puVar1);
  return puVar1;
}
```

## `0x102A9550` fn_102A9550

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102A9520`

```cpp
XI_API void __cdecl fn_102A9550(undefined4 * param_1)
{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = 0;
  *param_1 = 0x10001;
  param_1[2] = &LAB_102a9590;
  param_1[3] = XI_CODE(FUN_102a95a0);
  param_1[4] = XI_CODE(sqVtxPrint);
  param_1[5] = 0;
  puVar2 = param_1 + 0x15;
  do {
    puVar2[-0xf] = 0;
    *puVar2 = 0;
    *(uint8_t *)((int)param_1 + iVar1 + 0x48) = 0;
    iVar1 = iVar1 + 1;
    puVar2 = puVar2 + 1;
  } while (iVar1 < 0xc);
  return;
}
```

## `0x102A9EF0` fn_102A9EF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_102A9980`
- **Called by:** `sub_102AA650`, `sub_102AAA60`, `fn_102AD990`, `sub_102B5630`, `sub_102B5FE0`

```cpp
XI_API void __cdecl fn_102A9EF0(int param_1)
{
  uint32_t *puVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 0x58) == 0) {
    orig::FUN_102a9980(param_1,1,3);
  }
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x14)) {
    puVar1 = (uint32_t *)(*(int *)(param_1 + 0x58) + 8);
    do {
      puVar1[-2] = 0;
      puVar1[-1] = 0;
      *puVar1 = 0;
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 3;
    } while (iVar2 < *(int *)(param_1 + 0x14));
  }
  return;
}
```

## `0x102A9F30` fn_102A9F30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_102A9980`
- **Called by:** `fn_102AD990`

```cpp
XI_API void __cdecl fn_102A9F30(int param_1)
{
  uint32_t *puVar1;
  int iVar2;
  
  if (*(int *)(param_1 + 100) == 0) {
    orig::FUN_102a9980(param_1,4,3);
  }
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x14)) {
    puVar1 = (uint32_t *)(*(int *)(param_1 + 100) + 8);
    do {
      puVar1[-2] = 0;
      puVar1[-1] = 0;
      *puVar1 = 0;
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 3;
    } while (iVar2 < *(int *)(param_1 + 0x14));
  }
  return;
}
```

## `0x102A9FB0` fn_102A9FB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `sqFree`, `sub_10311C2C`
- **Called by:** `sub_10299C30`

```cpp
XI_API void __cdecl fn_102A9FB0(int param_1, int param_2)
{
  int iVar1;
  uint8_t bVar2;
  undefined4 *puVar3;
  int iVar4;
  float *pfVar5;
  uint8_t bVar6;
  int64_t lVar7;
  uint32_t local_10;
  int local_c;
  
  if ((param_1 != 0) && (iVar1 = *(int *)(param_1 + 0x54 + param_2 * 4), iVar1 != 0)) {
    puVar3 = orig::FUN_10275d80(1,*(int *)(param_1 + 0x14) << 2,
                          s_C__dev_dancer_modules_sqBase_src_103be750,0x29a);
    local_c = 0;
    if (0 < *(int *)(param_1 + 0x14)) {
      do {
        iVar4 = *(int *)(param_1 + 0x18 + param_2 * 4);
        local_10 = 0;
        if (0 < iVar4) {
          bVar6 = 0;
          pfVar5 = (float *)(iVar1 + iVar4 * local_c * 4);
          do {
            lVar7 = XI_CALL_ST0(0x10311C2Cu)((long double)*pfVar5 * (long double)255.0f);
            pfVar5 = pfVar5 + 1;
            bVar2 = bVar6 & 0x1f;
            bVar6 = bVar6 + 8;
            local_10 = local_10 | (int)(char)lVar7 << bVar2;
            iVar4 = iVar4 + -1;
          } while (iVar4 != 0);
        }
        puVar3[local_c] = local_10;
        local_c = local_c + 1;
      } while (local_c < *(int *)(param_1 + 0x14));
    }
    *(undefined4 **)(param_1 + 0x54 + param_2 * 4) = puVar3;
    *(undefined4 *)(param_1 + 0x18 + param_2 * 4) = 1;
    orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqBase_src_103be750,0x2a4);
  }
  return;
}
```
