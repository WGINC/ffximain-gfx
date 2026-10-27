# Batch r3_040_sqskSkeleton: 1 functions of `sqskSkeleton`

Name these functions: see `batches/README.md`. Write `data/names/r3_040_sqskSkeleton.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1027C900` fn_1027C900

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `Unit1027CCF0_ComputeTransform` (low), `sub_1027CE00`, `sub_1027CF10`, `Unit1027CCF0_TransformPoint` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkeleton\src\sqskSkeleton.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `StAvatar_UpdateInput` → `StAvatar_UpdateCameraFollow` → `Unit1027CCF0_TransformOrigin` → `Unit1027CCF0_TransformPoint` → `fn_1027C900`
- **Named callers' callers:** `SqmdModel_LookAt`, `Unit1027CCF0_TransformOrigin`
- **How its callers call it** (their verified code):
  - in `Unit1027CCF0_ComputeTransform` (low): `iVar1 = orig::FUN_1027c900(param_1,param_2);`
  - in `Unit1027CCF0_ComputeTransform` (low): `iVar1 = orig::FUN_1027c900(param_1,param_2);`
  - in `Unit1027CCF0_TransformPoint` (low): `iVar1 = orig::FUN_1027c900(param_1,param_2);`
  - in `Unit1027CCF0_TransformPoint` (low): `iVar1 = orig::FUN_1027c900(param_1,param_2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit1027CCF0_ComputeTransform` (low): Accumulates a chain's matrices and transforms a point through them.
  - `Unit1027CCF0_TransformPoint` (low): Transforms a point through a chain of matrices.
  - `sqDebugPrintf` (high): 

```cpp
XI_API int __cdecl fn_1027C900(int param_1, int param_2)
{
  int iVar1;
  int iVar2;
  
  if (*(int *)(param_2 + 0x68) != 0) {
    iVar1 = 0;
    if (0 < *(int *)(param_1 + 0x28)) {
      iVar2 = *(int *)(param_1 + 0x20);
      do {
        if (iVar2 == param_2) {
          return iVar1;
        }
        iVar1 = iVar1 + 1;
        iVar2 = iVar2 + 0xf8;
      } while (iVar1 < *(int *)(param_1 + 0x28));
    }
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x381)
    ;
    orig::sqDebugPrintf_v1(s_bone_is_not_found_in_a_skeleton_103b44fc);
  }
  return 0;
}
```
