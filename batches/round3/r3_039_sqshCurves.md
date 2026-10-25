# Batch r3_039_sqshCurves: 1 functions of `sqshCurves`

Name these functions: see `batches/README.md`. Write `data/names/r3_039_sqshCurves.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AD1D0` fn_102AD1D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x4
- **Called by:** `SqshCurves_RebuildIndex` (medium)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqShape\src\sqshCurves.c`, `Error in %s, line %d: `, `compute curve normals`
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `sqmdDMBBuildShapes` → `fn_10299B30` → `fn_10299EE0` → `SqshCurves_RebuildIndex` → `fn_102AD1D0`
- **How its callers call it** (their verified code):
  - in `SqshCurves_RebuildIndex` (medium): `orig::FUN_102ad1d0(param_1,iVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 
  - `SqshCurves_RebuildIndex` (medium): Rebuilds the curve index array from the largest value in its source array.

```cpp
XI_API void __cdecl fn_102AD1D0(int param_1, undefined4 xi_s2)
{
  int iVar1;
  uint32_t *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = *(int *)(param_1 + 0x34);
  if ((iVar1 == 0) || (*(int *)(iVar1 + 0x14) == 0)) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqShape_sr_103bef68,0xba);
    orig::sqDebugPrintf_v1(s_empty_or_missing_vertex__can_t___103beaa4,s_compute_curve_normals_103bf024);
  }
  else {
    if (*(int *)(iVar1 + 0x54) == 0) {
      orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqShape_sr_103bef68,0xbd
                  );
      orig::sqDebugPrintf_v1(s_missing_control_vertices_103bf050);
      return;
    }
    iVar1 = *(int *)(iVar1 + 0x58);
    if (iVar1 == 0) {
      orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqShape_sr_103bef68,0xc1
                  );
      orig::sqDebugPrintf_v1(s_missing_normals_103bf03c);
      return;
    }
    iVar6 = 0;
    iVar4 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        iVar3 = *(int *)(*(int *)(param_1 + 0x48) + iVar6 * 4);
        iVar5 = iVar4;
        if (0 < iVar3) {
          iVar5 = iVar4 + iVar3;
          puVar2 = (uint32_t *)(iVar1 + 8 + iVar4 * 0xc);
          do {
            puVar2[-2] = 0;
            puVar2[-1] = 0;
            *puVar2 = 0x3f800000;
            puVar2 = puVar2 + 3;
            iVar3 = iVar3 + -1;
          } while (iVar3 != 0);
        }
        iVar6 = iVar6 + 1;
        iVar4 = iVar5;
      } while (iVar6 < *(int *)(param_1 + 0x44));
      return;
    }
  }
  return;
}
```
