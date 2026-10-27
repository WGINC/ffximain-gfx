# Batch r3_036_sqmoChannel: 1 functions of `sqmoChannel`

Name these functions: see `batches/README.md`. Write `data/names/r3_036_sqmoChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10284F50` fn_10284F50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `Unit10274050_GetInterpolated` (low), `sub_1027E560`, `sub_10294B60`, `sqmoMixerMotion_Sample`, `sub_10295FF0`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqMotion\src\sqmoChannel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `StAvatar_UpdateInput` → `sub_1024E280` → `Chasm_ApplyAtCurrentIndex` → `Unit10274050_GetInterpolated` → `fn_10284F50`
- **Named callers' callers:** `Chasm_ApplyAtCurrentIndex`, `sqmoMotion_Dispatch`
- **How its callers call it** (their verified code):
  - in `Unit10274050_GetInterpolated` (low): `orig::FUN_10284f50(*(int *)(iVar2 + 0x28),`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10274050_GetInterpolated` (low): Returns two interpolated values for an active track entry at a time ratio.
  - `sqDebugPrintf` (high): 

```cpp
XI_API undefined4 __cdecl fn_10284F50(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  if (*(code **)(param_1 + 0x14) == (code *)0x0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqMotion_s_103b79a8,0x48);
    orig::sqDebugPrintf_v1(g_103B7974,param_1,*(uint32_t *)param_1);
    return 0;
  }
  uVar1 = (**(code **)(param_1 + 0x14))(param_1,param_2);
  return uVar1;
}
```
