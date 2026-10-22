# Batch r2_117_sqVtx: 3 functions of `sqVtx`

Name these functions: see `batches/README.md`. Write `data/names/r2_117_sqVtx.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102A9EF0` fn_102A9EF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_102A9980`
- **Called by:** `sub_102AA650`, `sub_102AAA60`, `SqshCurves_RebuildIndex` (medium), `sub_102B5630`, `sub_102B5FE0`
- **Named before** (a naming session's reading, low): `SqVtx_ClearEntries`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqVtx.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sqmdDMBBuildShapes` → `sub_10299B30` → `sub_10299EE0` → `SqshCurves_RebuildIndex` → `SqVtx_ClearEntries`
- **Named callees' callees:** `SqMem_AllocZeroed`, `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `SqshCurves_RebuildIndex` (medium): `SqVtx_ClearEntries(iVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqshCurves_RebuildIndex` (medium): Rebuilds the curve index array from the largest value in its source array.

```cpp
XI_API void __cdecl SqVtx_ClearEntries(int param_1)
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
- **Called by:** `SqshCurves_RebuildIndex` (medium)
- **Named before** (a naming session's reading, low): `SqVtx_ClearEntriesAlt`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqVtx.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sqmdDMBBuildShapes` → `sub_10299B30` → `sub_10299EE0` → `SqshCurves_RebuildIndex` → `SqVtx_ClearEntriesAlt`
- **Named callees' callees:** `SqMem_AllocZeroed`, `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `SqshCurves_RebuildIndex` (medium): `SqVtx_ClearEntriesAlt(iVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqshCurves_RebuildIndex` (medium): Rebuilds the curve index array from the largest value in its source array.

```cpp
XI_API void __cdecl SqVtx_ClearEntriesAlt(int param_1)
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
- **Named before** (a naming session's reading, low): `SqVtx_PackFloatsToBytes`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqBase\src\sqVtx.c`
- **Strings its callees refer to:** `VALID`
- **From the frame root** (shortest chain of direct calls): `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sqmdDMBBuildShapes` → `sub_10299B30` → `sub_10299C30` → `SqVtx_PackFloatsToBytes`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_GetPayloadEnd`, `SqMem_FindArena`, `sqDebugPrintf`

```cpp
XI_API void __cdecl SqVtx_PackFloatsToBytes(int param_1, int param_2)
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
