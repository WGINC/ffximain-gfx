# Batch r3_030_sqObject: 1 functions of `sqObject`

Name these functions: see `batches/README.md`. Write `data/names/r3_030_sqObject.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10285740` fn_10285740

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `StChannelItemClear`, `StAvatar_ReleaseMemory` (medium), `scnFini` (high), `SqmdModel_FreeItems` (medium), `Chn_ReleaseRefs` (low), `Unit1027E120_ReleaseConnection` (low), `sub_1028C070`, `SqsnScene_Free` (medium), `sub_1028EFF0`, `sqsdGeneric_BakeOntoShape`, `sqsdSampling_BakeOntoShape`, `sqsdAniso_BakeOntoShape`, `sqsdGlow_BakeOntoShape`, `sub_1029A090`, `sub_102ACC10`, `sub_102ACFF0`, `sub_102B53E0`, `sub_102B5D40`, `sub_102B6D70`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqBase\src\sqObject.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `Camera_ReleaseAll` → `StAvatar_SetActive` → `StAvatar_ReleaseMemory` → `fn_10285740`
- **Named callers' callers:** `StAvatarInit`, `StAvatar_ReleaseResources`, `StAvatarFini`, `StAvatar_SetActive`, `mdl_1026E900`, `Mdl_ReleaseEntries`, `SqmdModel_FreeBuffers`, `Chn_ReleaseIfSet`, `sqmdDMBBuildShapes`, `sqmdScriptCmd_skeleton_shape`
- **How its callers call it** (their verified code):
  - in `StChannelItemClear`: `orig::FUN_10285740((int *)(param_1 + 6));`
  - in `StAvatar_ReleaseMemory` (medium): `orig::FUN_10285740(&g_pSqgxContext_as_v1);`
  - in `scnFini` (high): `orig::FUN_10285740(&DAT_1099ccf8);`
  - in `SqmdModel_FreeItems` (medium): `orig::FUN_10285740((int *)&param_1);`
  - in `SqmdModel_FreeItems` (medium): `orig::FUN_10285740((int *)&param_1);`
  - in `Chn_ReleaseRefs` (low): `orig::FUN_10285740((int *)(param_1 + 0x2f));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_ReleaseMemory` (medium): Releases the avatar's memory blocks and clears their globals.
  - `scnFini` (high): 
  - `SqmdModel_FreeItems` (medium): Releases the model's item entries that it owns.
  - `Chn_ReleaseRefs` (low): Releases the channel's referenced sub-objects.
  - `Unit1027E120_ReleaseConnection` (low): Frees the object's connected resource and resets its connector.
  - `sqDebugPrintf` (high): 
  - `SqsnScene_Free` (medium): Frees a scene's cameras, lights, layers and shadow entries.

```cpp
XI_API void __cdecl fn_10285740(int * param_1)
{
  uint32_t *puVar1;
  
  puVar1 = (uint32_t *)*param_1;
  if ((code *)puVar1[3] != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x10285753. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)puVar1[3])();
    return;
  }
  orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqBase_src_103b7c6c,0x3b);
  orig::sqDebugPrintf_v1(g_103B7C98,puVar1,*puVar1);
  return;
}
```
