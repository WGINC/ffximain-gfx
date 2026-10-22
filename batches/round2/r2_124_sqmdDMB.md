# Batch r2_124_sqmdDMB: 3 functions of `sqmdDMB`

Name these functions: see `batches/README.md`. Write `data/names/r2_124_sqmdDMB.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10288660` fn_10288660

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10287FB0`, `SqmdDMB_AlignMem` (medium), `sub_102886D0`, `sub_10288950`, `SqmdDMB_ResolveMatrices` (medium), `SqmdDMB_ResolveStrings` (medium), `sqmdDMBBuildShapes`, `sub_10288FF0`
- **Writes globals:** `0x109A278C`
- **Reads globals:** `0x109A278C`
- **Named before** (a naming session's reading, low): `SqmdDMB_TraceDelta`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `SqmdDMB_TraceDelta`
- **How its callers call it** (their verified code):
  - in `SqmdDMB_AlignMem` (medium): `xi_s3 = SqmdDMB_TraceDelta(param_1);`
  - in `SqmdDMB_ResolveMatrices` (medium): `iVar1 = SqmdDMB_TraceDelta(param_2);`
  - in `SqmdDMB_ResolveStrings` (medium): `iVar2 = SqmdDMB_TraceDelta(param_2);`
  - in `SqmdDMB_ResolveStrings` (medium): `iVar4 = SqmdDMB_TraceDelta((int)pcVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmdDMB_AlignMem` (medium): Rounds a memory offset up to 16 bytes, logging the padding when tracing.
  - `SqmdDMB_ResolveMatrices` (medium): Assigns memory slots to the matrices of a deform block.
  - `SqmdDMB_ResolveStrings` (medium): Copies a deform block's tag and file strings into its buffer.

```cpp
XI_API int __cdecl SqmdDMB_TraceDelta(int param_1)
{
  if (DAT_109a278c == 0) {
    DAT_109a278c = param_1;
    return 0;
  }
  return param_1 - DAT_109a278c;
}
```

## `0x10288DC0` fn_10288DC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10287FB0`
- **Named before** (a naming session's reading, low): `SqmdDMB_ClearTables`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `SqmdDMB_ClearTables`

```cpp
XI_API void __cdecl SqmdDMB_ClearTables(int param_1)
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
- **Named before** (a naming session's reading, low): `SqmdDMB_ResetCounts`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `SqmdDMB_ResetCounts`

```cpp
XI_API void __cdecl SqmdDMB_ResetCounts(int param_1)
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
