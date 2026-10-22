# Batch 228_sqmdSort: 3 functions of `sqmdSort`

Name these functions: see `batches/README.md`. Write `data/names/228_sqmdSort.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029B100` fn_1029B100

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_1029B120`
- **Called by:** `sub_1029B3A0`

```cpp
XI_API undefined4 * __cdecl fn_1029B100(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x3c,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0x1d);
  orig::FUN_1029b120(puVar1);
  return puVar1;
}
```

## `0x1029B120` fn_1029B120

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1029B100`

```cpp
XI_API void __cdecl fn_1029B120(undefined4 * param_1)
{
  *param_1 = 0x1c0003;
  param_1[2] = &LAB_1029b170;
  param_1[3] = XI_CODE(FUN_1029b1a0);
  param_1[4] = &LAB_1029b230;
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &sqshTriStrip_ThrottledZSortUpdate;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  return;
}
```

## `0x1029B310` fn_1029B310

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x2
- **Called by:** `sub_1029B3A0`

```cpp
XI_API void __cdecl fn_1029B310(int * param_1, int * param_2, int param_3)
{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  
  iVar4 = **(int **)(param_3 + 0x48);
  piVar3 = *(int **)(param_3 + 0x48) + 1;
  *param_1 = iVar4;
  puVar2 = orig::FUN_10275d80(1,iVar4 * 4,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0xb3);
  *param_2 = (int)puVar2;
  while (iVar4 != 0) {
    iVar1 = *piVar3;
    iVar4 = iVar4 + -1;
    if (iVar1 < 1) {
      *(undefined4 *)(*param_2 + iVar4 * 4) = 0;
      iVar1 = iVar1 * -4;
    }
    else {
      iVar1 = iVar1 * 4;
      puVar2 = orig::FUN_10275d80(1,iVar1,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0xb7);
      *(undefined4 **)(*param_2 + iVar4 * 4) = puVar2;
    }
    piVar3 = (int *)((int)piVar3 + iVar1 + 4);
  }
  return;
}
```
