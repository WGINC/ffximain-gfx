# Batch r2_123_sqinSkin: 3 functions of `sqinSkin`

Name these functions: see `batches/README.md`. Write `data/names/r2_123_sqinSkin.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029A160` fn_1029A160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x2, `SqdfShape_Create` (medium)
- **Called by:** `sub_10288FF0`, `sub_102A0950`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkin\src\sqinSkin.c`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqDeform\src\sqdfShape.c`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `fn_1029A160`
- **Named callees' callees:** `sqDebugPrintf`, `SqdfShape_Init`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqdfShape_Create` (medium): Creates a sqdfShape object.

Short callee `SqdfShape_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqdfShape_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x40,s_C__dev_dancer_modules_sqDeform_s_103bf324,0x22);
  SqdfShape_Init(puVar1);
  return puVar1;
}
```

```cpp
XI_API void __cdecl fn_1029A160(int param_1, int param_2, undefined4 * param_3, undefined4 * param_4, undefined4 param_5, uint32_t param_6)
{
  undefined4 *puVar1;
  uint32_t uVar2;
  int iVar3;
  
  if ((param_1 != 0) && (param_3 != (undefined4 *)0x0)) {
    puVar1 = SqdfShape_Create();
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
- **Named before** (a naming session's reading, low): `SqinSkin_PropagateValue38`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `SqinSkin_PropagateValue38`

```cpp
XI_API void __cdecl SqinSkin_PropagateValue38(int param_1, undefined4 param_2)
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

## `0x1029A450` fn_1029A450

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10274650`, `sub_10275090` x12, `sub_10275D80` x2, `sqFree` x2, `SqskSkeleton_Release` (low), `sub_1027EB10`, `sub_1027F0E0` x2, `sub_10280140`, `SqdfShape_Create` (medium)
- **Called by:** `SqinSkin_ReleaseIfHasData` (medium)
- **Writes globals:** `0x109A2830`, `0x109A2834`, `0x109A2838`, `0x109A283C`, `0x109A2844`
- **Reads globals:** `0x109A2830`, `0x109A2834`, `0x109A2838`, `0x109A283C`, `0x109A2844`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkin\src\sqinSkin.c`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqMatrix4.c`, `C:\dev\dancer\modules\sqBase\src\sqTimer.c`, `C:\dev\dancer\modules\sqDeform\src\sqdfShape.c`, `VALID`, `Warning in %s, line %d: `
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_GetPayloadEnd`, `SqMem_FindArena`, `sqMatrix4_1027FCC0`, `sqDebugPrintf`, `Unit102963D0_Set`, `SqMatrix3_Determinant`, `SqdfShape_Init`
- **How its callers call it** (their verified code):
  - in `SqinSkin_ReleaseIfHasData` (medium): `orig::FUN_1029a450(param_1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqskSkeleton_Release` (low): Releases the skeleton's linked object through its vtable.
  - `SqinSkin_ReleaseIfHasData` (medium): Releases the skin when it holds data.
  - `SqdfShape_Create` (medium): Creates a sqdfShape object.

Short callee `SqskSkeleton_Release` (low):
```cpp
XI_API void __cdecl SqskSkeleton_Release(int param_1)
{
  if (*(int *)(param_1 + 0x1c) != 0) {
                    /* WARNING: Could not recover jumptable at 0x1027c9df. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)(param_1 + 0x1c) + 0x18))();
    return;
  }
  return;
}
```

Short callee `SqdfShape_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqdfShape_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x40,s_C__dev_dancer_modules_sqDeform_s_103bf324,0x22);
  SqdfShape_Init(puVar1);
  return puVar1;
}
```

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
        puVar7 = SqdfShape_Create();
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
