# Batch 226_sqmdModel: 14 functions of `sqmdModel`

Name these functions: see `batches/README.md`. Write `data/names/226_sqmdModel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10277AB0` fn_10277AB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `__attribute__`, `sqFree` x3, `fn_10277B70`, `sqDebugPrintf`
- **Called by:** `StModelItemClear`, `mdl_1026E900`, `sub_1026ECC0`

```cpp
XI_API void __cdecl fn_10277AB0(int * param_1)
{
  int iVar1;
  uint64_t uVar2;
  
  orig::FUN_10277b70(*param_1);
  iVar1 = *param_1;
  if ((*(uint32_t *)(iVar1 + 0xa8) & 0x200) == 0) {
    orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xb5);
  }
  else {
    if (((*(char *)(iVar1 + -0x10) != 'D') || (*(char *)(iVar1 + -0xf) != 'M')) ||
       (*(char *)(iVar1 + -0xe) != 'B')) {
      uVar2 = orig::sqDebugPrintf_v1(s_____Error__sqmdDMBHdr_not_found_c_103b3874);
      orig::FUN_102755c0(iVar1 + -0x20,0x30,(int)uVar2,(int)((uint64_t)uVar2 >> 0x20));
      *param_1 = 0;
      return;
    }
    if (*(int *)(iVar1 + -4) != 0) {
      orig::sqFree_v1(*(int *)(iVar1 + -4),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xa6);
      *(uint32_t *)(iVar1 + -4) = 0;
    }
    if (*(int *)(iVar1 + -8) != 0) {
      orig::sqFree_v1(*(int *)(iVar1 + -8),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xae);
      *(uint32_t *)(iVar1 + -8) = 0;
      *param_1 = 0;
      return;
    }
  }
  *param_1 = 0;
  return;
}
```

## `0x10277B70` fn_10277B70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree` x23, `fn_1027A250`, `fn_102819C0`, `fn_10281A70`
- **Called by:** `fn_10277AB0`

```cpp
XI_API void __cdecl fn_10277B70(int param_1)
{
  int iVar1;
  int iVar2;
  int iVar3;
  
  orig::FUN_1027a250((int *)param_1);
  if ((*(int *)(param_1 + 0x24) != 0) || (*(int *)(param_1 + 0x28) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x1c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x1c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xdd);
        *(uint32_t *)(param_1 + 0x1c) = 0;
      }
      if (*(int *)(param_1 + 0x20) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x20),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xde);
        *(uint32_t *)(param_1 + 0x20) = 0;
      }
    }
    else {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x24)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x1c) + iVar3 * 4);
          iVar2 = *(int *)(iVar1 + 0x14);
          if (-1 < iVar2) {
            orig::FUN_10281a70(iVar2);
          }
          iVar1 = *(int *)(iVar1 + 0x18);
          if (-1 < iVar1) {
            orig::FUN_102819c0(iVar1);
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x24));
        *(uint32_t *)(param_1 + 0x28) = 0;
        *(uint32_t *)(param_1 + 0x24) = 0;
        goto LAB_10277c2c;
      }
    }
    *(uint32_t *)(param_1 + 0x28) = 0;
    *(uint32_t *)(param_1 + 0x24) = 0;
  }
LAB_10277c2c:
  if ((*(int *)(param_1 + 0x34) != 0) || (*(int *)(param_1 + 0x38) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x2c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x2c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xe6);
        *(uint32_t *)(param_1 + 0x2c) = 0;
      }
      if (*(int *)(param_1 + 0x30) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x30),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xe7);
        *(uint32_t *)(param_1 + 0x30) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x38) = 0;
    *(uint32_t *)(param_1 + 0x34) = 0;
  }
  if ((*(int *)(param_1 + 0x58) != 0) || (*(int *)(param_1 + 0x5c) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x58)) {
        do {
          if (((*(uint8_t *)(*(int *)(param_1 + 0x54) + iVar3 * 4) & 0x20) != 0) &&
             (iVar1 = *(int *)(*(int *)(param_1 + 0x50) + iVar3 * 4), iVar1 != 0)) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf1);
            *(uint32_t *)(*(int *)(param_1 + 0x50) + iVar3 * 4) = 0;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x58));
      }
      if (*(int *)(param_1 + 0x4c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x4c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf4);
        *(uint32_t *)(param_1 + 0x4c) = 0;
      }
      if (*(int *)(param_1 + 0x50) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x50),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf5);
        *(uint32_t *)(param_1 + 0x50) = 0;
      }
      if (*(int *)(param_1 + 0x54) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x54),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf6);
        *(uint32_t *)(param_1 + 0x54) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x5c) = 0;
    *(uint32_t *)(param_1 + 0x58) = 0;
  }
  if ((*(int *)(param_1 + 0x44) != 0) || (*(int *)(param_1 + 0x48) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x3c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x3c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xfe);
        *(uint32_t *)(param_1 + 0x3c) = 0;
      }
      if (*(int *)(param_1 + 0x40) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x40),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xff);
        *(uint32_t *)(param_1 + 0x40) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x48) = 0;
    *(uint32_t *)(param_1 + 0x44) = 0;
  }
  if ((*(int *)(param_1 + 0x68) != 0) || (*(int *)(param_1 + 0x6c) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x60) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x60),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x107);
        *(uint32_t *)(param_1 + 0x60) = 0;
      }
      if (*(int *)(param_1 + 100) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 100),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x108);
        *(uint32_t *)(param_1 + 100) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x6c) = 0;
    *(uint32_t *)(param_1 + 0x68) = 0;
  }
  if ((*(int *)(param_1 + 0x7c) != 0) || (*(int *)(param_1 + 0x80) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x7c)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x70) + iVar3 * 4);
          if (iVar1 != 0) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x111);
            *(uint32_t *)(*(int *)(param_1 + 0x70) + iVar3 * 4) = 0;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x7c));
      }
      if (*(int *)(param_1 + 0x70) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x70),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x113);
        *(uint32_t *)(param_1 + 0x70) = 0;
      }
      if (*(int *)(param_1 + 0x74) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x74),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x114);
        *(uint32_t *)(param_1 + 0x74) = 0;
      }
      if (*(int *)(param_1 + 0x78) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x78),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x115);
        *(uint32_t *)(param_1 + 0x78) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x80) = 0;
    *(uint32_t *)(param_1 + 0x7c) = 0;
  }
  if ((*(int *)(param_1 + 0x90) != 0) || (*(int *)(param_1 + 0x94) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x90)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x84) + iVar3 * 4);
          if (iVar1 != 0) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x11e);
// ... 39 more lines (tree/src/sqmdModel.cpp)
```

## `0x10278E90` fn_10278E90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102749C0`, `fn_10279870`, `fn_1027A700` x2, `fn_1027ABA0`, `fn_1027B060` x2, `fn_1027B080` x2, `fn_1027B170`, `fn_1027CD30` x2, `sqDebugPrintf`
- **Called by:** `fn_1026E590`

```cpp
XI_API void __cdecl fn_10278E90(int param_1, int param_2, int param_3, undefined4 param_4, float param_5)
{
  int iVar1;
  int iVar2;
  int iVar3;
  float fVar4;
  int *piVar5;
  float *pfVar6;
  uint8_t bVar7;
  long double fVar8;
  char xi_frame[108] __attribute__((aligned(4)));
  float  &local_6c = *reinterpret_cast<float  *>(xi_frame + 0);
  int  &local_68 = *reinterpret_cast<int  *>(xi_frame + 4);
  float  &local_64 = *reinterpret_cast<float  *>(xi_frame + 8);
  float  &local_60 = *reinterpret_cast<float  *>(xi_frame + 12);
  float  &local_5c = *reinterpret_cast<float  *>(xi_frame + 16);
  float  &local_58 = *reinterpret_cast<float  *>(xi_frame + 20);
  float  &local_54 = *reinterpret_cast<float  *>(xi_frame + 24);
  float  &local_50 = *reinterpret_cast<float  *>(xi_frame + 28);
  float  (&local_4c)[3] = *reinterpret_cast<float  (*)[3]>(xi_frame + 32);
  float  (&local_40)[4] = *reinterpret_cast<float  (*)[4]>(xi_frame + 44);
  float  (&local_30)[4] = *reinterpret_cast<float  (*)[4]>(xi_frame + 60);
  float  (&local_20)[4] = *reinterpret_cast<float  (*)[4]>(xi_frame + 76);
  uint8_t  (&local_10)[16] = *reinterpret_cast<uint8_t  (*)[16]>(xi_frame + 92);
  
  local_68 = 0;
  local_6c = 0.0f;
  if (*(int *)(param_1 + 0x44) < 1) {
    return;
  }
  do {
    fVar4 = local_6c;
    piVar5 = (int *)orig::FUN_10279870(param_1,local_6c);
    if (*piVar5 == 0xb0001) {
      if (local_68 == param_2) {
        if ((param_3 < 0) || (piVar5[10] <= param_3)) {
          orig::sqDebugPrintf_v2(s_Error__sqmdModelLookAt___called_w_103b3c30,param_3,piVar5[10]);
          return;
        }
        iVar2 = *(int *)(piVar5[7] + 0x68);
        *(uint32_t *)(piVar5[7] + 0x68) = 0;
        iVar1 = piVar5[8] + param_3 * 0xf8;
        orig::FUN_1027cd30((int)piVar5,iVar1,local_4c,local_10);
        orig::FUN_102749c0(&local_64,param_4,local_4c);
        if (iVar2 != 0) {
          iVar3 = *(int *)(iVar2 + 100);
          orig::FUN_1027a700(local_40,(uint32_t *)(iVar3 + 0x68));
          orig::FUN_1027b060(local_40);
          local_64 = (float)((long double)local_64 - (long double)*(float *)(iVar3 + 0x5c));
          local_60 = (float)((long double)local_60 - (long double)*(float *)(iVar3 + 0x60));
          local_5c = (float)((long double)local_5c - (long double)*(float *)(iVar3 + 100));
          orig::FUN_1027b080(&local_64,local_40,&local_64);
        }
        if (*(int *)(iVar1 + 0x68) != 0) {
          orig::FUN_1027cd30((int)piVar5,*(int *)(iVar1 + 0x68),&local_58,local_30);
          orig::FUN_1027b060(local_30);
          local_64 = (float)((long double)local_64 - (long double)local_58);
          local_60 = (float)((long double)local_60 - (long double)local_54);
          local_5c = (float)((long double)local_5c - (long double)local_50);
          orig::FUN_1027b080(&local_64,local_30,&local_64);
        }
        fVar8 = xi_castbits<long double>(fpatan((long double)local_64,(long double)local_5c));
        local_6c = (float)fVar8;
        if (fVar8 > (long double)param_5) {
          local_6c = param_5;
        }
        fVar8 = -(long double)param_5;
        bVar7 = NAN((long double)local_6c) || NAN(fVar8);
        if ((bVar7 || (long double)local_6c < fVar8) != bVar7) {
          local_6c = (float)fVar8;
        }
        iVar1 = *(int *)(iVar1 + 100);
        if (iVar1 != 0) {
          orig::FUN_1027aba0(local_20,local_6c);
          pfVar6 = (float *)(iVar1 + 0x68);
          orig::FUN_1027b170(local_40,local_20,pfVar6);
          orig::FUN_1027a700(pfVar6,local_40);
        }
        *(int *)(piVar5[7] + 0x68) = iVar2;
      }
      local_68 = local_68 + 1;
    }
    local_6c = xi_castbits<float>((xi_castbits<int>(fVar4) + 1));
    if (*(int *)(param_1 + 0x44) <= xi_castbits<int>(local_6c)) {
      return;
    }
  } while( true );
}
```

## `0x10279530` fn_10279530

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x2
- **Called by:** `sub_1026F200`, `sub_102700C0`, `sub_1029FFF0`, `sub_102A0950`, `sub_102A0F90`, `sub_102A12F0`, `sub_102A17B0`, `sub_102A1B00`

```cpp
XI_API void __cdecl fn_10279530(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar2 = *(int *)(param_1 + 0x44);
  if (*(int *)(param_1 + 0x48) <= iVar2) {
    uVar1 = iVar2 * 4 + 4;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x40),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x547);
    *(undefined4 **)(param_1 + 0x40) = puVar3;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x3c),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x549);
    *(undefined4 **)(param_1 + 0x3c) = puVar3;
  }
  *(undefined4 *)(*(int *)(param_1 + 0x40) + iVar2 * 4) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x3c) + iVar2 * 4) = param_2;
  *(int *)(param_1 + 0x44) = *(int *)(param_1 + 0x44) + 1;
  return;
}
```

## `0x102796A0` fn_102796A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x2
- **Called by:** `sub_1027A220`

```cpp
XI_API void __cdecl fn_102796A0(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar2 = *(int *)(param_1 + 0x68);
  if (*(int *)(param_1 + 0x6c) <= iVar2) {
    uVar1 = iVar2 * 4 + 4;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 100),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x58f);
    *(undefined4 **)(param_1 + 100) = puVar3;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x60),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x591);
    *(undefined4 **)(param_1 + 0x60) = puVar3;
  }
  *(undefined4 *)(*(int *)(param_1 + 100) + iVar2 * 4) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x60) + iVar2 * 4) = param_2;
  *(int *)(param_1 + 0x68) = *(int *)(param_1 + 0x68) + 1;
  return;
}
```

## `0x10279830` fn_10279830

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10279BF0`, `sub_10279E60`, `sub_1028C410`

```cpp
XI_API undefined4 __cdecl fn_10279830(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,0,param_2);
  return uVar1;
}
```

## `0x10279850` fn_10279850

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_102787F0`, `sub_10279BF0`, `sub_10279ED0`, `sub_1027A4B0`, `sub_1028C4F0`, `sub_1028C540`

```cpp
XI_API undefined4 __cdecl fn_10279850(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,1,param_2);
  return uVar1;
}
```

## `0x10279870` fn_10279870

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `fn_1024DFE0`, `fn_1024FF50`, `fn_10252AC0`, `fn_10252B50`, `sub_10252BC0`, `fn_1026F160`, `sub_102700C0`, `sub_10270320`, `sub_102704C0`, `sqmdModel_Dump`, `sub_10278490`, `fn_10278E90`, `sub_10279BF0`, `sub_10279F40`, `sub_10287990`, `sub_1028CBE0`, `sub_1028CE20`, `sub_1028CEB0`, `sub_1028D030`, `sub_1028D2C0` and 1 more

```cpp
XI_API undefined4 __cdecl fn_10279870(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,2,param_2);
  return uVar1;
}
```

## `0x10279890` fn_10279890

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10278490`, `sub_102787F0`, `sub_10279BF0`, `sub_10279FB0`, `sub_1027A4B0`, `sub_10287990`, `sub_1028C1F0`, `sub_1028CE20`, `sub_1028D2C0`

```cpp
XI_API undefined4 __cdecl fn_10279890(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,3,param_2);
  return uVar1;
}
```

## `0x102798B0` fn_102798B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10279BF0`, `sub_1027A020`

```cpp
XI_API undefined4 __cdecl fn_102798B0(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,4,param_2);
  return uVar1;
}
```

## `0x10279B40` fn_10279B40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqmdModel_GetItemName`
- **Called by:** `sub_10278710`, `sub_102787F0`, `sub_1027A4B0`, `sub_1028CBE0`, `sub_1028D2C0`, `sub_1028D350`, `sub_1028D530`

```cpp
XI_API undefined4 __cdecl fn_10279B40(int param_1, int param_2)
{
  char *pcVar1;
  
  pcVar1 = orig::sqmdModel_GetItemName(param_1,3,param_2);
  return pcVar1;
}
```

## `0x10279BA0` fn_10279BA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sqmdModel_Dump`, `sub_10287990`, `sub_10288FF0`, `sub_1028C410`, `sub_1028CBE0`, `sub_1028D350`, `sub_1028D530`

```cpp
XI_API char * __cdecl fn_10279BA0(int param_1, int param_2, int param_3)
{
  int iVar1;
  
  iVar1 = 0;
  if (0 < *(int *)(param_1 + 0x90)) {
    do {
      if ((*(int *)(*(int *)(param_1 + 0x88) + iVar1 * 4) == param_2) &&
         (*(int *)(*(int *)(param_1 + 0x8c) + iVar1 * 4) == param_3)) {
        return *(char **)(*(int *)(param_1 + 0x84) + iVar1 * 4);
      }
      iVar1 = iVar1 + 1;
    } while (iVar1 < *(int *)(param_1 + 0x90));
  }
  return s_untitled_103b3d70;
}
```

## `0x10279D50` fn_10279D50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279BF0`
- **Called by:** `sub_102787F0`, `sqmdScriptCmd_skeleton_shape`, `sub_102A0950`, `sub_102A12F0`, `sub_102A17B0`

```cpp
XI_API undefined4 __cdecl fn_10279D50(undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279bf0(param_1,param_2,2,param_3);
  return uVar1;
}
```

## `0x1027A250` fn_1027A250

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree`, `sub_10285740` x2
- **Called by:** `fn_10277B70`

```cpp
XI_API void __cdecl fn_1027A250(int * param_1)
{
  int iVar1;
  int *piVar2;
  
  piVar2 = param_1;
  iVar1 = param_1[0x1a];
  while (iVar1 = iVar1 + -1, -1 < iVar1) {
    if (((*(uint32_t *)(piVar2[0x19] + iVar1 * 4) & 0x100) != 0) &&
       (param_1 = *(int **)(piVar2[0x18] + iVar1 * 4), *param_1 != 0x70004)) {
      orig::FUN_10285740((int *)&param_1);
      *(uint32_t *)(piVar2[0x18] + iVar1 * 4) = 0;
    }
  }
  iVar1 = piVar2[0x1a];
  while (iVar1 = iVar1 + -1, -1 < iVar1) {
    param_1 = *(int **)(piVar2[0x18] + iVar1 * 4);
    if (((param_1 != (int *)0x0) && ((*(uint32_t *)(piVar2[0x19] + iVar1 * 4) & 0x100) != 0)) &&
       (*param_1 == 0x70004)) {
      if ((*(uint8_t *)(param_1 + 8) & 8) == 0) {
        orig::FUN_10285740((int *)&param_1);
      }
      else {
        orig::sqFree_v1((int)param_1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x848);
      }
    }
  }
  return;
}
```
