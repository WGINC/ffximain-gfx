# Batch 224_sqinSkin: 7 functions of `sqinSkin`

Name these functions: see `batches/README.md`. Write `data/names/224_sqinSkin.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10299FE0` fn_10299FE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_1029A000`
- **Called by:** `sub_1029AA80`, `sub_102B0CA0`

```cpp
XI_API undefined4 * __cdecl fn_10299FE0(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x48,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x69);
  orig::FUN_1029a000(puVar1);
  return puVar1;
}
```

## `0x1029A000` fn_1029A000

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102AEC50`
- **Called by:** `fn_10299FE0`

```cpp
XI_API void __cdecl fn_1029A000(undefined4 * param_1)
{
  int iVar1;
  undefined4 *puVar2;
  
  puVar2 = param_1;
  for (iVar1 = 0x12; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *param_1 = 0x150001;
  param_1[2] = &LAB_1029a050;
  param_1[3] = XI_CODE(FUN_1029a060);
  param_1[4] = XI_CODE(FUN_1029a0e0);
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = &LAB_1029a9b0;
  orig::FUN_102aec50(param_1 + 9);
  param_1[0xd] = 0x3f000000;
  return;
}
```

## `0x1029A160` fn_1029A160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x2, `fn_102AEE90`
- **Called by:** `sub_10288FF0`, `sub_102A0950`

```cpp
XI_API void __cdecl fn_1029A160(int param_1, int param_2, undefined4 * param_3, undefined4 * param_4, undefined4 param_5, uint32_t param_6)
{
  undefined4 *puVar1;
  uint32_t uVar2;
  int iVar3;
  
  if ((param_1 != 0) && (param_3 != (undefined4 *)0x0)) {
    puVar1 = orig::FUN_102aee90();
    *(undefined4 **)(param_1 + 0x24) = puVar1;
    puVar1[5] = param_5;
    *(int *)(*(int *)(param_1 + 0x24) + 0x28) = param_2;
    *(undefined4 **)(*(int *)(param_1 + 0x24) + 0x18) = param_3;
    *(undefined4 **)(*(int *)(param_1 + 0x24) + 0x20) = param_4;
    *(uint32_t *)(*(int *)(param_1 + 0x24) + 0x34) = param_6 & 0xfffffffd;
    puVar1 = orig::FUN_10275d80(1,param_2 * 0xc,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x122);
    *(undefined4 **)(param_1 + 0x28) = puVar1;
    for (uVar2 = param_2 * 3 & 0x3fffffff; uVar2 != 0; uVar2 = uVar2 - 1) {
      *puVar1 = *param_3;
      param_3 = param_3 + 1;
      puVar1 = puVar1 + 1;
    }
    for (iVar3 = 0; iVar3 != 0; iVar3 = iVar3 + -1) {
      *(uint8_t *)puVar1 = *(uint8_t *)param_3;
      param_3 = (undefined4 *)((int)param_3 + 1);
      puVar1 = (undefined4 *)((int)puVar1 + 1);
    }
    if (param_4 != (undefined4 *)0x0) {
      puVar1 = orig::FUN_10275d80(1,param_2 * 0xc,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x125);
      *(undefined4 **)(param_1 + 0x2c) = puVar1;
      for (uVar2 = param_2 * 3 & 0x3fffffff; uVar2 != 0; uVar2 = uVar2 - 1) {
        *puVar1 = *param_4;
        param_4 = param_4 + 1;
        puVar1 = puVar1 + 1;
      }
      for (iVar3 = 0; iVar3 != 0; iVar3 = iVar3 + -1) {
        *(uint8_t *)puVar1 = *(uint8_t *)param_4;
        param_4 = (undefined4 *)((int)param_4 + 1);
        puVar1 = (undefined4 *)((int)puVar1 + 1);
      }
    }
    *(uint32_t *)(param_1 + 0x30) = *(uint32_t *)(param_1 + 0x30) | 1;
  }
  return;
}
```

## `0x1029A230` fn_1029A230

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10288FF0`, `sub_102A0950`

```cpp
XI_API void __cdecl fn_1029A230(int param_1, undefined4 param_2)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  if ((param_1 != 0) && (*(int *)(param_1 + 0x24) != 0)) {
    iVar3 = 0;
    *(uint32_t *)(*(int *)(param_1 + 0x24) + 0x38) = param_2;
    if (0 < *(int *)(param_1 + 0x3c)) {
      do {
        piVar1 = *(int **)(*(int *)(param_1 + 0x38) + iVar3 * 4);
        if ((*piVar1 == 0x150002) && (iVar2 = piVar1[0x15], iVar2 != 0)) {
          *(uint32_t *)(iVar2 + 0x38) = param_2;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < *(int *)(param_1 + 0x3c));
    }
  }
  return;
}
```

## `0x1029A330` fn_1029A330

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90`
- **Called by:** `fn_1029A380`, `sqinSkinBuildBinary`, `sub_102B16F0`, `sub_102B1A00`, `sub_102B2010`

```cpp
XI_API undefined4 __cdecl fn_1029A330(int param_1, int param_2)
{
  uint32_t xi_entry_eax = 0;
  undefined4 *puVar1;
  
  if ((param_1 != 0) && (0 < param_2)) {
    puVar1 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x38),param_2 * 4,
                          s_C__dev_dancer_modules_sqSkin_src_103bc310,0x195);
    xi_entry_eax = 0;
    if (puVar1 != (undefined4 *)0x0) {
      *(int *)(param_1 + 0x40) = param_2;
      *(undefined4 **)(param_1 + 0x38) = puVar1;
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)puVar1 >> 8)) << 8) | (uint8_t)(0xff)));
    }
  }
  return xi_entry_eax & 0xffffff00;
}
```

## `0x1029A450` fn_1029A450

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10274650`, `sub_10275090` x12, `sub_10275D80` x2, `sqFree` x2, `fn_1027C9D0`, `sub_1027EB10`, `sub_1027F0E0` x2, `sub_10280140`, `fn_102AEE90`
- **Called by:** `fn_1029A6F0`
- **Writes globals:** `0x109A2830`, `0x109A2834`, `0x109A2838`, `0x109A283C`, `0x109A2844`
- **Reads globals:** `0x109A2830`, `0x109A2834`, `0x109A2838`, `0x109A283C`, `0x109A2844`

```cpp
XI_API void __cdecl fn_1029A450(int param_1)
{
  uint32_t *puVar1;
  int iVar2;
  int iVar3;
  float *pfVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  char xi_frame[156] __attribute__((aligned(4)));
  int  &local_9c = *reinterpret_cast<int  *>(xi_frame + 0);
  undefined4 * &local_98 = *reinterpret_cast<undefined4 * *>(xi_frame + 4);
  float  (&local_80)[16] = *reinterpret_cast<float  (*)[16]>(xi_frame + 28);
  float  (&local_40)[16] = *reinterpret_cast<float  (*)[16]>(xi_frame + 92);
  
  if (param_1 != 0) {
    iVar3 = orig::FUN_10275090();
    pfVar4 = (float *)orig::FUN_10275d80(1,0x40,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x216);
    if (*(int *)(param_1 + 0x1c) != 0) {
      orig::FUN_1027c9d0_v1(*(int *)(param_1 + 0x1c));
    }
    iVar9 = *(int *)(param_1 + 0x3c);
    iVar5 = orig::FUN_10275090();
    orig::sqMatrix4_1027F0E0_v1(pfVar4,*(undefined4 **)(*(int *)(param_1 + 0x24) + 0x14));
    iVar6 = orig::FUN_10275090();
    DAT_109a2830 = DAT_109a2830 + (iVar6 - iVar5);
    local_9c = 0;
    if (0 < iVar9) {
      do {
        iVar5 = *(int *)(*(int *)(param_1 + 0x38) + local_9c * 4);
        iVar6 = *(int *)(iVar5 + 0x30);
        iVar8 = *(int *)(iVar5 + 0x50);
        puVar7 = orig::FUN_102aee90();
        *(undefined4 **)(iVar5 + 0x54) = puVar7;
        puVar7[5] = pfVar4;
        *(undefined4 *)(*(int *)(iVar5 + 0x54) + 0x28) =
             *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x28);
        *(undefined4 *)(*(int *)(iVar5 + 0x54) + 0x18) =
             *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x18);
        *(undefined4 *)(*(int *)(iVar5 + 0x54) + 0x20) =
             *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x20);
        if (*(int *)(*(int *)(iVar5 + 0x54) + 0x20) != 0) {
          iVar2 = *(int *)(*(int *)(param_1 + 0x24) + 0x20);
          local_98 = orig::FUN_10275d80(1,iVar8 * 0xc,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x22e);
          iVar10 = 0;
          *(undefined4 **)(iVar5 + 0x40) = local_98;
          if (0 < iVar8) {
            do {
              orig::FUN_10274650(local_98,(undefined4 *)(iVar2 + *(int *)(iVar6 + iVar10 * 4) * 0xc));
              iVar10 = iVar10 + 1;
              local_98 = local_98 + 3;
            } while (iVar10 < iVar8);
          }
        }
        puVar1 = (uint32_t *)(*(int *)(iVar5 + 0x54) + 0x34);
        *puVar1 = *puVar1 & 0xfffffffa;
        iVar6 = orig::FUN_10275090();
        orig::sqMatrix4_1027F0E0_v1(local_80,(undefined4 *)(*(int *)(iVar5 + 0x9c) + 0x24));
        iVar8 = orig::FUN_10275090();
        DAT_109a2830 = DAT_109a2830 + (iVar8 - iVar6);
        iVar6 = orig::FUN_10275090();
        orig::sqMatrix4_10280140_v1(local_40,local_80,*(float **)(*(int *)(param_1 + 0x24) + 0x14));
        iVar8 = orig::FUN_10275090();
        DAT_109a2834 = DAT_109a2834 + (iVar8 - iVar6);
        iVar6 = orig::FUN_10275090();
        orig::M4fSet_v1((undefined4 *)(iVar5 + 0x5c),local_40);
        iVar8 = orig::FUN_10275090();
        DAT_109a2838 = DAT_109a2838 + (iVar8 - iVar6);
        iVar6 = orig::FUN_10275090();
        iVar8 = orig::FUN_10275090();
        DAT_109a283c = DAT_109a283c + (iVar8 - iVar6);
        if (*(int *)(iVar5 + 0x3c) != 0) {
          orig::sqFree_v1(*(int *)(iVar5 + 0x3c),s_C__dev_dancer_modules_sqSkin_src_103bc310,0x249);
          *(undefined4 *)(iVar5 + 0x3c) = 0;
        }
        local_9c = local_9c + 1;
      } while (local_9c < iVar9);
    }
    if (((*(uint8_t *)(*(int *)(param_1 + 0x24) + 0x34) & 4) != 0) &&
       (iVar9 = *(int *)(*(int *)(param_1 + 0x24) + 0x14), iVar9 != 0)) {
      orig::sqFree_v1(iVar9,s_C__dev_dancer_modules_sqSkin_src_103bc310,0x250);
      *(undefined4 *)(*(int *)(param_1 + 0x24) + 0x14) = 0;
    }
    *(float **)(*(int *)(param_1 + 0x24) + 0x14) = pfVar4;
    *(uint32_t *)(*(int *)(param_1 + 0x24) + 0x34) = *(uint32_t *)(*(int *)(param_1 + 0x24) + 0x34) | 4;
    *(uint8_t *)(param_1 + 0x44) = 0xff;
    iVar9 = orig::FUN_10275090();
    DAT_109a2844 = DAT_109a2844 + (iVar9 - iVar3);
  }
  return;
}
```

## `0x1029A6F0` fn_1029A6F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10288FF0`, `sub_1029A9B0`, `sub_102A0950`

```cpp
XI_API void __cdecl fn_1029A6F0(int param_1)
{
  if ((param_1 != 0) && (*(int *)(param_1 + 0x3c) != 0)) {
    orig::FUN_1029a450(param_1);
    return;
  }
  return;
}
```
