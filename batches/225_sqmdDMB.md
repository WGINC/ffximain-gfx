# Batch 225_sqmdDMB: 6 functions of `sqmdDMB`

Name these functions: see `batches/README.md`. Write `data/names/225_sqmdDMB.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10288660` fn_10288660

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10287FB0`, `fn_10288680`, `sub_102886D0`, `sub_10288950`, `fn_10288BB0`, `fn_10288C20`, `sqmdDMBBuildShapes`, `sub_10288FF0`
- **Writes globals:** `0x109A278C`
- **Reads globals:** `0x109A278C`

```cpp
XI_API int __cdecl fn_10288660(int param_1)
{
  if (DAT_109a278c == 0) {
    DAT_109a278c = param_1;
    return 0;
  }
  return param_1 - DAT_109a278c;
}
```

## `0x10288680` fn_10288680

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10288660`, `sub_10314D55`
- **Called by:** `sub_10287FB0`, `sub_102886D0`, `sub_10288950`, `fn_10288BB0`, `fn_10288C20`, `sqmdDMBBuildShapes`, `sub_10288FF0`
- **Reads globals:** `0x109A2788`

```cpp
XI_API uint32_t __cdecl fn_10288680(uint32_t param_1)
{
  int iVar1;
  int xi_s3;
  int iVar2;
  
  iVar1 = DAT_109a2788;
  if ((param_1 & 0xf) != 0) {
    iVar2 = 0x10 - (param_1 & 0xf);
    if (DAT_109a2788 != 0) {
      xi_s3 = orig::FUN_10288660(param_1);
      orig::FUN_10314d55(iVar1,s__06ld__sqmdDMBAlignMem___adding___103b8ec8,xi_s3);
    }
    if (iVar2 != 0) {
      param_1 = param_1 + iVar2;
    }
  }
  return param_1;
}
```

## `0x10288BB0` fn_10288BB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10288660`, `fn_10288680`, `sub_10314D55`
- **Called by:** `sub_10287FB0`
- **Reads globals:** `0x109A2788`

```cpp
XI_API undefined4 __cdecl fn_10288BB0(int param_1, uint32_t param_2)
{
  int iVar1;
  uint32_t uVar2;
  int iVar3;
  
  iVar3 = DAT_109a2788;
  if (DAT_109a2788 != 0) {
    iVar1 = orig::FUN_10288660(param_2);
    orig::FUN_10314d55(iVar3,s__06ld_____sqmdDMBResolveMatrices_103b9134,iVar1);
  }
  uVar2 = orig::FUN_10288680(param_2);
  iVar3 = *(int *)(param_1 + 0x5c);
  if (*(int *)(param_1 + 0x5c) <= *(int *)(param_1 + 0x58)) {
    iVar3 = *(int *)(param_1 + 0x58);
  }
  iVar1 = 0;
  if (0 < iVar3) {
    do {
      if (*(int *)(*(int *)(param_1 + 0x50) + iVar1 * 4) == -0x10412246) {
        *(uint32_t *)(*(int *)(param_1 + 0x50) + iVar1 * 4) = uVar2;
        uVar2 = uVar2 + 0x40;
      }
      iVar1 = iVar1 + 1;
    } while (iVar1 < iVar3);
  }
  return uVar2;
}
```

## `0x10288C20` fn_10288C20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10288660` x5, `fn_10288680`, `sub_10314D55` x5
- **Called by:** `sub_10287FB0`
- **Reads globals:** `0x109A2788`

```cpp
XI_API char * __cdecl fn_10288C20(int param_1, uint32_t param_2)
{
  char cVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  uint32_t uVar6;
  int iVar7;
  char *pcVar8;
  
  iVar7 = DAT_109a2788;
  if (DAT_109a2788 != 0) {
    iVar2 = orig::FUN_10288660(param_2);
    orig::FUN_10314d55(iVar7,s__06ld_____sqmdDMBResolveStrings__103b9190,iVar2);
  }
  pcVar3 = (char *)orig::FUN_10288680(param_2);
  iVar7 = 0;
  if (0 < *(int *)(param_1 + 0x7c)) {
    do {
      iVar2 = DAT_109a2788;
      if (DAT_109a2788 != 0) {
        iVar4 = orig::FUN_10288660((int)pcVar3);
        orig::FUN_10314d55(iVar2,s__06ld__tag_string__s_103b9174,iVar4);
      }
      uVar6 = 0xffffffff;
      *(char **)(*(int *)(param_1 + 0x70) + iVar7 * 4) = pcVar3;
      iVar2 = DAT_109a2788;
      pcVar8 = pcVar3;
      do {
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar8 + 1;
      } while (cVar1 != '\0');
      pcVar3 = pcVar3 + ~uVar6;
      if (((uint32_t)pcVar3 & 3) != 0) {
        iVar4 = 4 - ((uint32_t)pcVar3 & 3);
        if (DAT_109a2788 != 0) {
          iVar5 = orig::FUN_10288660((int)pcVar3);
          orig::FUN_10314d55(iVar2,s__06ld__adding__d_align_bytes_103b8850,iVar5);
        }
        if (iVar4 != 0) {
          pcVar3 = pcVar3 + iVar4;
        }
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 0x7c));
  }
  iVar7 = *(int *)(param_1 + 0x7c);
  if (iVar7 < *(int *)(param_1 + 0x80)) {
    do {
      iVar7 = iVar7 + 1;
      *(uint32_t *)(*(int *)(param_1 + 0x70) + -4 + iVar7 * 4) = 0;
    } while (iVar7 < *(int *)(param_1 + 0x80));
  }
  iVar7 = 0;
  if (0 < *(int *)(param_1 + 0x90)) {
    do {
      iVar2 = DAT_109a2788;
      if (DAT_109a2788 != 0) {
        iVar4 = orig::FUN_10288660((int)pcVar3);
        orig::FUN_10314d55(iVar2,s__06ld__file_string__s_103b9158,iVar4);
      }
      uVar6 = 0xffffffff;
      *(char **)(*(int *)(param_1 + 0x84) + iVar7 * 4) = pcVar3;
      iVar2 = DAT_109a2788;
      pcVar8 = pcVar3;
      do {
        if (uVar6 == 0) break;
        uVar6 = uVar6 - 1;
        cVar1 = *pcVar8;
        pcVar8 = pcVar8 + 1;
      } while (cVar1 != '\0');
      pcVar3 = pcVar3 + ~uVar6;
      if (((uint32_t)pcVar3 & 3) != 0) {
        iVar4 = 4 - ((uint32_t)pcVar3 & 3);
        if (DAT_109a2788 != 0) {
          iVar5 = orig::FUN_10288660((int)pcVar3);
          orig::FUN_10314d55(iVar2,s__06ld__adding__d_align_bytes_103b8850,iVar5);
        }
        if (iVar4 != 0) {
          pcVar3 = pcVar3 + iVar4;
        }
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < *(int *)(param_1 + 0x90));
  }
  iVar7 = *(int *)(param_1 + 0x90);
  if (iVar7 < *(int *)(param_1 + 0x94)) {
    do {
      iVar7 = iVar7 + 1;
      *(uint32_t *)(*(int *)(param_1 + 0x84) + -4 + iVar7 * 4) = 0;
    } while (iVar7 < *(int *)(param_1 + 0x94));
  }
  return pcVar3;
}
```

## `0x10288DC0` fn_10288DC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10287FB0`

```cpp
XI_API void __cdecl fn_10288DC0(int param_1)
{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x68);
  if (*(int *)(param_1 + 0x68) < *(int *)(param_1 + 0x6c)) {
    iVar2 = *(int *)(param_1 + 0x6c);
  }
  iVar1 = 0;
  if (0 < iVar2) {
    do {
      iVar1 = iVar1 + 1;
      *(uint32_t *)(*(int *)(param_1 + 100) + -4 + iVar1 * 4) = 0;
      *(uint32_t *)(*(int *)(param_1 + 0x60) + -4 + iVar1 * 4) = 0;
    } while (iVar1 < iVar2);
  }
  *(uint32_t *)(param_1 + 0x68) = 0;
  return;
}
```

## `0x10288E00` fn_10288E00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10287FB0`

```cpp
XI_API void __cdecl fn_10288E00(int param_1)
{
  int iVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0xa4);
  if (*(int *)(param_1 + 0xa4) < *(int *)(param_1 + 0x5c)) {
    iVar2 = *(int *)(param_1 + 0x5c);
  }
  iVar1 = 0;
  if (0 < iVar2) {
    do {
      iVar1 = iVar1 + 1;
      *(uint32_t *)(*(int *)(param_1 + 0x9c) + -4 + iVar1 * 4) = 0;
    } while (iVar1 < iVar2);
  }
  iVar2 = *(int *)(param_1 + 0x58);
  if (*(int *)(param_1 + 0x58) < *(int *)(param_1 + 0x5c)) {
    iVar2 = *(int *)(param_1 + 0x5c);
  }
  iVar1 = 0;
  if (0 < iVar2) {
    do {
      iVar1 = iVar1 + 1;
      *(uint32_t *)(*(int *)(param_1 + 0xa0) + -4 + iVar1 * 4) = 0;
    } while (iVar1 < iVar2);
  }
  *(uint32_t *)(param_1 + 0xa4) = 0;
  return;
}
```
