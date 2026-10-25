# Batch r3_032_sqVtx: 1 functions of `sqVtx`

Name these functions: see `batches/README.md`. Write `data/names/r3_032_sqVtx.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102A9980` fn_102A9980

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqMem_AllocZeroed` (medium), `sqDebugPrintf` (high) x2
- **Called by:** `SqshBuild_Read` (medium), `sub_102A98F0`, `sub_102A9A80`, `sub_102A9B90`, `SqVtx_ClearEntries` (low), `SqVtx_ClearEntriesAlt` (low), `sub_102A9F70`, `sub_102AADE0`, `sub_102B7250`, `sub_102B7560`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqBase\src\sqVtx.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `sqmdDMBBuildShapes` → `fn_10299B30` → `fn_10299C30` → `SqshBuild_Read` → `fn_102A9980`
- **Named callers' callers:** `SqshCurves_RebuildIndex`
- **How its callers call it** (their verified code):
  - in `SqshBuild_Read` (medium): `orig::FUN_102a9980((int)puVar3,(int)param_1,local_8);`
  - in `SqVtx_ClearEntries` (low): `orig::FUN_102a9980(param_1,1,3);`
  - in `SqVtx_ClearEntriesAlt` (low): `orig::FUN_102a9980(param_1,4,3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqMem_AllocZeroed` (medium): Allocates a zeroed block of count times size bytes.
  - `sqDebugPrintf` (high): 
  - `SqshBuild_Read` (medium): Reads a shape build block from a stream, vertex by vertex.
  - `SqVtx_ClearEntries` (low): Clears the vertex entries of the first table.
  - `SqVtx_ClearEntriesAlt` (low): Clears the vertex entries of the second table.

```cpp
XI_API void __cdecl fn_102A9980(int param_1, int param_2, int param_3)
{
  undefined4 *puVar1;
  
  if (*(int *)(param_1 + 0x18 + param_2 * 4) != 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqBase_src_103be750,0x15a)
    ;
    orig::sqDebugPrintf_v1(s_sqVtxAllocateField__slot__d_is_a_103be8e8,param_2);
    return;
  }
  *(int *)(param_1 + 0x18 + param_2 * 4) = param_3;
  puVar1 = orig::SqMem_AllocZeroed(*(int *)(param_1 + 0x14) * param_3,4,
                             s_C__dev_dancer_modules_sqBase_src_103be750,0x15f);
  *(undefined4 **)(param_1 + 0x54 + param_2 * 4) = puVar1;
  return;
}
```
