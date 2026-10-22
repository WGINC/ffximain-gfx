# Batch 252_sqsnScene: 22 functions of `sqsnScene`

Name these functions: see `batches/README.md`. Write `data/names/252_sqsnScene.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1028E1B0` fn_1028E1B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree` x9, `fn_10281A70`, `sub_10285740`
- **Called by:** `scnFini`, `sub_1028E180`
- **Writes globals:** `0x103BA68C`
- **Reads globals:** `0x103BA68C`

```cpp
XI_API void __cdecl fn_1028E1B0(int param_1)
{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = param_1;
  if (-1 < g_sceneFadeMaskRT) {
    orig::FUN_10281a70(g_sceneFadeMaskRT);
    g_sceneFadeMaskRT = -1;
  }
  if (*(int *)(param_1 + 0x14) != 0) {
    orig::sqFree_v1(*(int *)(param_1 + 0x18),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x5d);
  }
  if (*(int *)(param_1 + 0x20) != 0) {
    orig::sqFree_v1(*(int *)(param_1 + 0x24),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x5f);
    orig::sqFree_v1(*(int *)(param_1 + 0x28),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x60);
  }
  piVar2 = (int *)(param_1 + 0x44);
  if (*piVar2 != 0) {
    param_1 = 0;
    if (0 < *piVar2) {
      iVar5 = 0;
      do {
        iVar4 = 0;
        iVar6 = *(int *)(iVar1 + 0x48);
        iVar3 = *(int *)(iVar5 + 4 + iVar6);
        if (iVar3 != 0) {
          if (0 < iVar3) {
            iVar3 = 0;
            do {
              piVar2 = (int *)(*(int *)(iVar5 + 0x28 + iVar6) + iVar3);
              if (piVar2[3] != 0) {
                orig::FUN_10285740(piVar2);
              }
              iVar6 = *(int *)(*(int *)(*(int *)(iVar1 + 0x48) + 0x28 + iVar5) + 0x14 + iVar3);
              if (iVar6 != 0) {
                orig::sqFree_v1(iVar6,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x6b);
                *(uint32_t *)(*(int *)(*(int *)(iVar1 + 0x48) + 0x28 + iVar5) + 0x14 + iVar3) = 0;
              }
              iVar6 = *(int *)(iVar1 + 0x48);
              iVar4 = iVar4 + 1;
              iVar3 = iVar3 + 0x90;
            } while (iVar4 < *(int *)(iVar5 + 4 + iVar6));
          }
          orig::sqFree_v1(*(int *)(*(int *)(iVar1 + 0x48) + 0x28 + iVar5),
                 s_C__dev_dancer_modules_sqScene_sr_103ba694,0x6f);
          orig::sqFree_v1(*(int *)(*(int *)(iVar1 + 0x48) + 0x24 + iVar5),
                 s_C__dev_dancer_modules_sqScene_sr_103ba694,0x70);
          *(uint32_t *)(*(int *)(iVar1 + 0x48) + 0x28 + iVar5) = 0;
          *(uint32_t *)(*(int *)(iVar1 + 0x48) + 0x24 + iVar5) = 0;
        }
        param_1 = param_1 + 1;
        iVar5 = iVar5 + 0x2c;
      } while (param_1 < *(int *)(iVar1 + 0x44));
    }
    orig::sqFree_v1(*(int *)(iVar1 + 0x48),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x75);
  }
  if (*(int *)(iVar1 + 0x34) != 0) {
    iVar5 = 0;
    if (0 < *(int *)(iVar1 + 0x34)) {
      iVar6 = 0;
      do {
        iVar3 = *(int *)(iVar1 + 0x40) + iVar6;
        if (*(int *)(iVar3 + 4) != 0) {
          orig::sqFree_v1(*(int *)(iVar3 + 8),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x7a);
        }
        iVar5 = iVar5 + 1;
        iVar6 = iVar6 + 0x30;
      } while (iVar5 < *(int *)(iVar1 + 0x34));
    }
    orig::sqFree_v1(*(int *)(iVar1 + 0x40),s_C__dev_dancer_modules_sqScene_sr_103ba694,0x7d);
  }
  return;
}
```

## `0x1028E350` fn_1028E350

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270B60`, `sub_1028E150`

```cpp
XI_API void __cdecl fn_1028E350(undefined4 * param_1)
{
  *param_1 = 0x100001;
  param_1[2] = &LAB_1028e170;
  param_1[3] = XI_CODE(FUN_1028e180);
  param_1[4] = &sqsnScenePrint;
  param_1[0xf] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xd] = 0;
  param_1[0x10] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[0xe] = 0x3f800000;
  param_1[7] = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  param_1[0xc] = 0x100;
  *(uint8_t *)(param_1 + 0xb) = 1;
  orig::FUN_10298b90(param_1 + 0x13);
  return;
}
```

## `0x1028EBF0` fn_1028EBF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1024FDF0`

```cpp
XI_API void __cdecl fn_1028EBF0(int param_1, int param_2, float param_3)
{
  *(float *)(*(int *)(param_1 + 0x48) + 0x14 + param_2 * 0x2c) = (float)(long double)param_3;
  return;
}
```

## `0x1028EC50` fn_1028EC50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270B60`

```cpp
XI_API void __cdecl fn_1028EC50(int param_1, int param_2, undefined4 param_3)
{
  *(uint8_t *)(*(int *)(param_1 + 0x48) + 0x19 + param_2 * 0x2c) = (uint8_t)param_3;
  return;
}
```

## `0x1028ECC0` fn_1028ECC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10270B60`, `fn_10272630`

```cpp
XI_API void __cdecl fn_1028ECC0(int param_1, int param_2, undefined4 param_3)
{
  *(uint8_t *)(param_2 + *(int *)(param_1 + 0x28)) = (uint8_t)param_3;
  return;
}
```

## `0x1028ECE0` fn_1028ECE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_102726E0`

```cpp
XI_API void __cdecl fn_1028ECE0(int param_1, int param_2, undefined4 param_3)
{
  *(uint8_t *)(*(int *)(param_1 + 0x48) + 0x18 + param_2 * 0x2c) = (uint8_t)param_3;
  return;
}
```

## `0x1028ED00` fn_1028ED00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90`
- **Called by:** `fn_10270B60`

```cpp
XI_API int __cdecl fn_1028ED00(int param_1, undefined4 param_2, undefined4 param_3)
{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = *(int *)(param_1 + 0x14);
  *(int *)(param_1 + 0x14) = iVar1 + 1;
  puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x18),(iVar1 + 1) * 0xc,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x186);
  *(undefined4 **)(param_1 + 0x18) = puVar2;
  puVar2[iVar1 * 3] = param_2;
  *(undefined4 *)(*(int *)(param_1 + 0x18) + 4 + iVar1 * 0xc) = param_3;
  return iVar1;
}
```

## `0x1028ED50` fn_1028ED50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x2
- **Called by:** `fn_10270B60`

```cpp
XI_API int __cdecl fn_1028ED50(int param_1, undefined4 param_2, undefined4 param_3)
{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = *(int *)(param_1 + 0x20);
  *(int *)(param_1 + 0x20) = iVar1 + 1;
  puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x24),(iVar1 + 1) * 0xc,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x197);
  *(undefined4 **)(param_1 + 0x24) = puVar2;
  puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x28),*(uint32_t *)(param_1 + 0x20),
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x198);
  *(undefined4 **)(param_1 + 0x28) = puVar2;
  *(undefined4 *)(iVar1 * 0xc + *(int *)(param_1 + 0x24)) = param_2;
  *(uint8_t *)(iVar1 + *(int *)(param_1 + 0x28)) = 1;
  *(undefined4 *)(*(int *)(param_1 + 0x24) + 4 + iVar1 * 0xc) = param_3;
  return iVar1;
}
```

## `0x1028EDE0` fn_1028EDE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x3, `fn_1028EFB0`
- **Called by:** `fn_10270B60`, `sub_102787F0`, `sub_1028EDC0`

```cpp
XI_API int __cdecl fn_1028EDE0(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5)
{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar2 = param_1;
  iVar1 = param_5 + 1;
  if (*(int *)(param_1 + 0x44) < iVar1) {
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x48),iVar1 * 0x2c,
                          s_C__dev_dancer_modules_sqScene_sr_103ba694,0x1d4);
    *(undefined4 **)(param_1 + 0x48) = puVar3;
    iVar4 = *(int *)(param_1 + 0x44);
    if (iVar4 < iVar1) {
      iVar5 = iVar4 * 0x2c;
      param_1 = iVar1 - iVar4;
      do {
        orig::FUN_1028efb0((undefined4 *)(*(int *)(iVar2 + 0x48) + iVar5));
        iVar5 = iVar5 + 0x2c;
        param_1 = param_1 + -1;
      } while (param_1 != 0);
    }
    *(int *)(iVar2 + 0x44) = iVar1;
  }
  iVar6 = param_5 * 0x2c;
  iVar1 = *(int *)(*(int *)(iVar2 + 0x48) + 4 + iVar6);
  *(int *)(*(int *)(iVar2 + 0x48) + 4 + iVar6) = iVar1 + 1;
  puVar3 = orig::FUN_10275e90(*(undefined4 **)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6),
                        *(int *)(*(int *)(iVar2 + 0x48) + 4 + iVar6) * 0x90,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x1dc);
  *(undefined4 **)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) = puVar3;
  puVar3 = orig::FUN_10275e90(*(undefined4 **)(*(int *)(iVar2 + 0x48) + iVar6 + 0x24),
                        *(int *)(*(int *)(iVar2 + 0x48) + 4 + iVar6) << 2,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x1dd);
  *(undefined4 **)(*(int *)(iVar2 + 0x48) + 0x24 + iVar6) = puVar3;
  iVar4 = *(int *)(*(int *)(iVar2 + 0x48) + 4 + iVar6);
  *(int *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x24 + iVar6) + -4 + iVar4 * 4) = iVar4 + -1;
  iVar4 = iVar1 * 0x90;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0xc + iVar4) = 0;
  *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6)) = param_2;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x10 + iVar4) = 0;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 4 + iVar4) = param_3;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 8 + iVar4) = 0;
  *(uint8_t *)(iVar4 + 0x1c + *(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6)) = 0xff;
  *(uint8_t *)(iVar4 + 0x1d + *(int *)(iVar6 + 0x28 + *(int *)(iVar2 + 0x48))) = 0xff;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x18 + iVar4) = param_4;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x20 + iVar4) = 0;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x44 + iVar4) = 0;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x14 + iVar4) = 0;
  iVar4 = iVar4 + 0x68;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x20 + iVar4) = 0;
  *(undefined4 *)(*(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6) + 0x24 + iVar4) = 0;
  iVar5 = 8;
  do {
    iVar5 = iVar5 + -1;
    *(undefined4 *)(iVar4 + *(int *)(*(int *)(iVar2 + 0x48) + 0x28 + iVar6)) = 0xffffffff;
    iVar4 = iVar4 + 4;
  } while (iVar5 != 0);
  return param_5 * 0x10000 + iVar1;
}
```

## `0x1028EFB0` fn_1028EFB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1028EDE0`, `fn_10290610`

```cpp
XI_API void __cdecl fn_1028EFB0(undefined4 * param_1)
{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[4] = 8;
    *(uint8_t *)(param_1 + 6) = 1;
    *(uint8_t *)((int)param_1 + 0x19) = 0;
    param_1[5] = 0x3f800000;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    param_1[10] = 0;
    param_1[3] = 0;
  }
  return;
}
```

## `0x1028F130` fn_1028F130

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90`
- **Called by:** `fn_10272A10`

```cpp
XI_API int __cdecl fn_1028F130(int param_1, undefined4 param_2)
{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar1 = *(int *)(param_1 + 0x34);
  *(int *)(param_1 + 0x34) = iVar1 + 1;
  puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x40),(iVar1 + 1) * 0x30,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x242);
  iVar3 = iVar1 * 0x30;
  *(undefined4 **)(param_1 + 0x40) = puVar2;
  puVar2[iVar1 * 0xc] = param_2;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 4 + iVar3) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 8 + iVar3) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0xc + iVar3) = 0xffffffff;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x10 + iVar3) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x14 + iVar3) = 0;
  *(uint8_t *)(*(int *)(param_1 + 0x40) + 0x24 + iVar3) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x18 + iVar3) = 0x40000000;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x1c + iVar3) = 0x3f4ccccd;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x28 + iVar3) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x20 + iVar3) = 0x100;
  *(undefined4 *)(*(int *)(param_1 + 0x40) + 0x2c + iVar3) = 4;
  return iVar1;
}
```

## `0x1028F220` fn_1028F220

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10272A10`

```cpp
XI_API void __cdecl fn_1028F220(int param_1, int param_2, undefined4 param_3)
{
  *(uint32_t *)(*(int *)(param_1 + 0x40) + 0x28 + param_2 * 0x30) = param_3;
  return;
}
```

## `0x1028F3C0` fn_1028F3C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90`
- **Called by:** `sub_102787F0`

```cpp
XI_API void __cdecl fn_1028F3C0(int param_1, int param_2, undefined4 param_3)
{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  
  iVar4 = param_2 * 0x30 + *(int *)(param_1 + 0x40);
  iVar2 = *(int *)(iVar4 + 4);
  iVar1 = iVar2 + 1;
  *(int *)(iVar4 + 4) = iVar1;
  puVar3 = orig::FUN_10275e90(*(undefined4 **)(iVar4 + 8),iVar1 * 4,
                        s_C__dev_dancer_modules_sqScene_sr_103ba694,0x2c2);
  *(undefined4 **)(iVar4 + 8) = puVar3;
  puVar3[iVar2] = param_3;
  return;
}
```

## `0x1028F430` fn_1028F430

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10273810`, `sub_10278BF0`, `sub_10278C80`, `sub_10278D90`, `sub_1028F280`, `sub_1028F2A0`

```cpp
XI_API int __cdecl fn_1028F430(int param_1, uint32_t param_2)
{
  return (param_2 & 0x7fff) * 0x90 +
         *(int *)(*(int *)(param_1 + 0x48) + 0x28 + ((int)param_2 >> 0x10) * 0x2c);
}
```

## `0x1028F4A0` fn_1028F4A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10273160`, `fn_102733B0`, `fn_10273560`

```cpp
XI_API void __cdecl fn_1028F4A0(int param_1, int param_2)
{
  if (param_2 < *(int *)(param_1 + 0x14)) {
    *(int *)(param_1 + 0x1c) = param_2;
  }
  return;
}
```

## `0x1028F4C0` fn_1028F4C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqgxMatrixShape`, `sub_102A1F20`
- **Called by:** `sqsnSceneLayerDraw`, `sqsnSceneLayerDrawShadowed`

```cpp
XI_API void __cdecl fn_1028F4C0(undefined4 param_1, int * param_2)
{
  orig::sqgxMatrixShape_v1((uint32_t *)param_2[1]);
  orig::FUN_102a1f20((int *)param_2[6],param_2);
  return;
}
```

## `0x1028F6E0` fn_1028F6E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026E3D0` x2, `fn_10283260` x4, `sub_10283300` x4, `fn_10283DA0` x2, `fn_10283DE0` x2
- **Called by:** `sqsnSceneLayerDraw`, `sqsnSceneLayerDrawShadowed`

```cpp
XI_API void __cdecl fn_1028F6E0(int * param_1, int param_2)
{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int local_c;
  int local_8;
  int local_4;
  
  piVar2 = param_1;
  local_8 = 0;
  local_c = 0;
  local_4 = 0;
  if (0 < *(int *)(param_2 + 0x44)) {
    param_1 = (int *)(param_2 + 0x48);
    do {
      iVar3 = *(int *)(*param_1 * 0x30 + piVar2[0x10]);
      piVar1 = (int *)(piVar2[9] + iVar3 * 0xc);
      iVar4 = *piVar1;
      if (*(char *)(iVar3 + piVar2[10]) != '\0') {
        if (*(int *)(iVar4 + 0x30) == 0) {
          iVar3 = local_c;
          local_c = local_c + 1;
        }
        else {
          iVar3 = local_8;
          local_8 = local_8 + 1;
        }
        if ((iVar3 < 4) && (-1 < iVar3)) {
          orig::FUN_10283260(iVar3,(uint32_t)(*(int *)(iVar4 + 0x30) == 0));
          orig::FUN_1026e3d0(iVar4);
          orig::FUN_10283da0((uint32_t *)piVar1[1]);
          orig::FUN_10283de0(*(float *)(iVar4 + 0x24));
          orig::FUN_10283300(1);
        }
      }
      local_4 = local_4 + 1;
      param_1 = param_1 + 1;
    } while (local_4 < *(int *)(param_2 + 0x44));
  }
  local_4 = 0;
  if (0 < *(int *)(param_2 + 0x20)) {
    param_1 = (int *)(param_2 + 0x24);
    do {
      piVar1 = (int *)(piVar2[9] + *param_1 * 0xc);
      iVar3 = *piVar1;
      if (*(char *)(*param_1 + piVar2[10]) != '\0') {
        if (*(int *)(iVar3 + 0x30) == 0) {
          iVar4 = local_c;
          local_c = local_c + 1;
        }
        else {
          iVar4 = local_8;
          local_8 = local_8 + 1;
        }
        if ((iVar4 < 4) && (-1 < iVar4)) {
          orig::FUN_10283260(iVar4,(uint32_t)(*(int *)(iVar3 + 0x30) == 0));
          orig::FUN_1026e3d0(iVar3);
          orig::FUN_10283da0((uint32_t *)piVar1[1]);
          orig::FUN_10283de0(*(float *)(iVar3 + 0x24));
          orig::FUN_10283300(1);
        }
      }
      local_4 = local_4 + 1;
      param_1 = param_1 + 1;
    } while (local_4 < *(int *)(param_2 + 0x20));
  }
  for (; local_8 < 4; local_8 = local_8 + 1) {
    orig::FUN_10283260(local_8,0);
    orig::FUN_10283300(0);
  }
  for (; local_c < 4; local_c = local_c + 1) {
    orig::FUN_10283260(local_c,1);
    orig::FUN_10283300(0);
  }
  return;
}
```

## `0x1028FF80` fn_1028FF80

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10280DE0`, `sub_10281C20`, `sub_10281C50`, `sqgxTextureDraw` x2, `sqgxZTest`
- **Called by:** `sqsnSceneLayerDraw`, `sqsnSceneLayerDrawShadowed`
- **Reads globals:** `0x1099D200`

```cpp
XI_API void __cdecl fn_1028FF80(int param_1)
{
  int iVar1;
  
  orig::sqgxZTest_v1(0);
  iVar1 = *(int *)(g_pSqgxContext_as_s32 + 0x89c);
  orig::sqgxTextureDraw_v1(*(int *)(param_1 + 0x60),0);
  orig::sqgxTextureSet_v1(iVar1);
  orig::sqgxTextureDecal_v1(1);
  orig::sqgxSprite_v1(0,0,0x100,0x100,0,0,-1,-1);
  orig::sqgxTextureDraw_v1(iVar1,1);
  orig::sqgxZTest_v1(1);
  return;
}
```

## `0x10290430` fn_10290430

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sqsnSceneDraw`

```cpp
XI_API void __cdecl fn_10290430(void)
{
  return;
}
```

## `0x10290610` fn_10290610

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x5, `sqDebugPrintf` x5, `fn_1028EFB0`
- **Called by:** `fn_10270B60`

```cpp
XI_API void __cdecl fn_10290610(int param_1, int param_2, int param_3, int param_4, int param_5)
{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  
  if (0 < param_2) {
    if (*(int *)(param_1 + 0x18) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_2 * 0xc,s_C__dev_dancer_modules_sqScene_sr_103ba694,2000);
      *(undefined4 **)(param_1 + 0x18) = puVar1;
    }
    else {
      orig::sqDebugPrintf_v1(s_Error__sqsnScenePreallocate___ca_103baae0);
    }
  }
  if (0 < param_3) {
    if (*(int *)(param_1 + 0x24) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_3 * 0xc,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x7dd);
      *(undefined4 **)(param_1 + 0x24) = puVar1;
    }
    else {
      orig::sqDebugPrintf_v1(s_Error__sqsnScenePreallocate___ca_103baa98);
    }
    if (*(int *)(param_1 + 0x28) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_3,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x7e6);
      *(undefined4 **)(param_1 + 0x28) = puVar1;
    }
    else {
      orig::sqDebugPrintf_v1(s_Error__sqsnScenePreallocate___ca_103baa50);
    }
  }
  if (0 < param_4) {
    if (*(int *)(param_1 + 0x48) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_4 * 0x2c,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x7f3);
      iVar2 = 0;
      *(undefined4 **)(param_1 + 0x48) = puVar1;
      *(int *)(param_1 + 0x44) = param_4;
      if (0 < param_4) {
        iVar3 = 0;
        do {
          orig::FUN_1028efb0((undefined4 *)(*(int *)(param_1 + 0x48) + iVar3));
          iVar2 = iVar2 + 1;
          iVar3 = iVar3 + 0x2c;
        } while (iVar2 < *(int *)(param_1 + 0x44));
      }
    }
    else {
      orig::sqDebugPrintf_v1(s_Error__sqsnScenePreallocate___ca_103baa10);
    }
  }
  if (0 < param_5) {
    if (*(int *)(param_1 + 0x40) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_5 * 0x30,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x805);
      iVar2 = 0;
      *(undefined4 **)(param_1 + 0x40) = puVar1;
      *(int *)(param_1 + 0x34) = param_5;
      if (0 < param_5) {
        iVar3 = 0;
        do {
          iVar2 = iVar2 + 1;
          *(undefined4 *)(iVar3 + *(int *)(param_1 + 0x40)) = 0;
          *(undefined4 *)(iVar3 + 4 + *(int *)(param_1 + 0x40)) = 0;
          *(undefined4 *)(iVar3 + 8 + *(int *)(param_1 + 0x40)) = 0;
          *(undefined4 *)(iVar3 + 0xc + *(int *)(param_1 + 0x40)) = 0xffffffff;
          *(undefined4 *)(iVar3 + 0x10 + *(int *)(param_1 + 0x40)) = 0;
          *(undefined4 *)(iVar3 + 0x14 + *(int *)(param_1 + 0x40)) = 0;
          *(uint8_t *)(iVar3 + 0x24 + *(int *)(param_1 + 0x40)) = 0;
          *(undefined4 *)(iVar3 + 0x18 + *(int *)(param_1 + 0x40)) = 0x40000000;
          *(undefined4 *)(iVar3 + 0x28 + *(int *)(param_1 + 0x40)) = 0;
          iVar3 = iVar3 + 0x30;
        } while (iVar2 < *(int *)(param_1 + 0x34));
        return;
      }
    }
    else {
      orig::sqDebugPrintf_v1(s_Error__sqsnScenePreallocate___ca_103ba9c8);
    }
  }
  return;
}
```

## `0x102907C0` fn_102907C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x2, `sqDebugPrintf`
- **Called by:** `fn_10271C60`

```cpp
XI_API void __cdecl fn_102907C0(int param_1, int param_2, int param_3)
{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = *(int *)(param_1 + 0x48) + param_2 * 0x2c;
  if (0 < param_3) {
    if (*(int *)(iVar1 + 0x28) == 0) {
      puVar2 = orig::FUN_10275d80(1,param_3 * 0x90,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x82f);
      *(undefined4 **)(iVar1 + 0x28) = puVar2;
      puVar2 = orig::FUN_10275d80(1,param_3 * 4,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x831);
      *(undefined4 **)(iVar1 + 0x24) = puVar2;
      return;
    }
    orig::sqDebugPrintf_v1(s_Error__sqsnSceneLayerPreallocate_103bab28);
  }
  return;
}
```

## `0x10290840` fn_10290840

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `sqDebugPrintf`
- **Called by:** `fn_10271C80`

```cpp
XI_API void __cdecl fn_10290840(int param_1, int param_2, int param_3)
{
  undefined4 *puVar1;
  int iVar2;
  
  iVar2 = param_2 * 0x30 + *(int *)(param_1 + 0x40);
  if (0 < param_3) {
    if (*(int *)(iVar2 + 8) == 0) {
      puVar1 = orig::FUN_10275d80(1,param_3 * 4,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x84e);
      *(undefined4 **)(iVar2 + 8) = puVar1;
      return;
    }
    orig::sqDebugPrintf_v1(s_Error__sqsnSceneShadowPeallocate_103bab70);
  }
  return;
}
```
