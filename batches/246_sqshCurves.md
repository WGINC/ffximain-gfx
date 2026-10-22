# Batch 246_sqshCurves: 3 functions of `sqshCurves`

Name these functions: see `batches/README.md`. Write `data/names/246_sqshCurves.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102ACF20` fn_102ACF20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `fn_102ACF40`
- **Called by:** `sub_10299EE0`, `sub_102AD950`, `sub_102AE6A0`

```cpp
XI_API undefined4 * __cdecl fn_102ACF20(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d40(1,0x6c,s_C__dev_dancer_modules_sqShape_sr_103bef68,0x24);
  orig::FUN_102acf40(puVar1);
  return puVar1;
}
```

## `0x102ACF40` fn_102ACF40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1028D9A0`
- **Called by:** `fn_102ACF20`

```cpp
XI_API void __cdecl fn_102ACF40(undefined4 * param_1)
{
  *param_1 = 0xa0002;
  param_1[2] = &LAB_102acfb0;
  param_1[3] = XI_CODE(FUN_102acfc0);
  param_1[4] = XI_CODE(sqshCurvesPrint);
  param_1[0xb] = &LAB_10291260;
  param_1[0xc] = &DAT_102b34e0;
  orig::FUN_1028d9a0(param_1 + 5);
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x10] = XI_CODE(FUN_102ad770);
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0x14] = 0;
  param_1[0x15] = 0x3f800000;
  param_1[0x16] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  return;
}
```

## `0x102AD990` fn_102AD990

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275D40`, `sqFree`, `fn_102A9EF0`, `fn_102A9F30`, `sub_102AD1D0`, `sub_102AD770`
- **Called by:** `sub_10299EE0`, `sub_102ADA30`, `sub_102AE6A0`

```cpp
XI_API void __cdecl fn_102AD990(int param_1)
{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  
  if (*(int *)(param_1 + 0x4c) == 0) {
    iVar3 = *(int *)(param_1 + 0x44);
    iVar4 = 0;
    if (0 < iVar3) {
      piVar2 = *(int **)(param_1 + 0x48);
      do {
        if (iVar4 < *piVar2) {
          iVar4 = *piVar2;
        }
        piVar2 = piVar2 + 1;
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
    }
    if (*(int *)(param_1 + 0x50) != 0) {
      orig::sqFree_v1(*(int *)(param_1 + 0x50),s_C__dev_dancer_modules_sqShape_sr_103bef68,0x1dd);
    }
    puVar1 = orig::FUN_10275d40(iVar4,4,s_C__dev_dancer_modules_sqShape_sr_103bef68,0x1df);
    *(undefined4 **)(param_1 + 0x50) = puVar1;
    iVar3 = 0;
    if (0 < iVar4) {
      do {
        *(int *)(*(int *)(param_1 + 0x50) + iVar3 * 4) = iVar3;
        iVar3 = iVar3 + 1;
      } while (iVar3 < iVar4);
    }
    iVar3 = *(int *)(param_1 + 0x34);
    orig::FUN_102a9ef0(iVar3);
    orig::FUN_102ad1d0(param_1,iVar3);
    iVar3 = *(int *)(param_1 + 0x34);
    orig::FUN_102a9f30(iVar3);
    orig::FUN_102ad770_v1(param_1,iVar3);
    *(uint8_t **)(param_1 + 0x30) =(uint8_t*)(&LAB_102b3340);
  }
  return;
}
```
