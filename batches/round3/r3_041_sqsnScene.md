# Batch r3_041_sqsnScene: 2 functions of `sqsnScene`

Name these functions: see `batches/README.md`. Write `data/names/r3_041_sqsnScene.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1028F2E0` fn_1028F2E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `Unit102706F0_InitScene` (low), `fn_10278990`, `sub_10278A20`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqScene\src\sqsnScene.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatarInit` → `Unit102706F0_InitScene` → `fn_1028F2E0`
- **Named callers' callers:** `StAvatarInit`, `Unit10272F20_ApplySceneToModel`
- **How its callers call it** (their verified code):
  - in `Unit102706F0_InitScene` (low): `orig::FUN_1028f2e0(0x1099b13c,uVar3,DAT_1099ccfc);`
  - in `fn_10278990`: `orig::FUN_1028f2e0(param_2,*(uint32_t *)(*(int *)(param_1 + 0x9c) + iVar2 * 4),param_3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit102706F0_InitScene` (low): Initialises the avatar scene's state, cameras, lights and parts.
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_1028F2E0(int param_1, uint32_t param_2, undefined4 param_3)
{
  int iVar1;
  
  iVar1 = (param_2 & 0x7fff) * 0x90 +
          *(int *)(*(int *)(param_1 + 0x48) + 0x28 + ((int)param_2 >> 0x10) * 0x2c);
  if (7 < *(int *)(iVar1 + 0x20)) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x29c)
    ;
    orig::sqDebugPrintf_v1(s_too_many_lights_are_being_added_t_103ba94c);
    return;
  }
  *(uint32_t *)(iVar1 + 0x24 + *(int *)(iVar1 + 0x20) * 4) = param_3;
  *(int *)(iVar1 + 0x20) = *(int *)(iVar1 + 0x20) + 1;
  return;
}
```

## `0x1028F350` fn_1028F350

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `sub_10278AC0`, `fn_10278B50`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqScene\src\sqsnScene.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278B50` → `fn_1028F350`
- **Named callers' callers:** `Unit10272F20_ApplySceneToModel`
- **How its callers call it** (their verified code):
  - in `fn_10278B50`: `orig::FUN_1028f350(param_2,uVar1,param_4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_1028F350(int param_1, uint32_t param_2, undefined4 param_3)
{
  int iVar1;
  
  iVar1 = (param_2 & 0x7fff) * 0x90 +
          *(int *)(*(int *)(param_1 + 0x48) + 0x28 + ((int)param_2 >> 0x10) * 0x2c);
  if (7 < *(int *)(iVar1 + 0x44)) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqScene_sr_103ba694,0x2ae)
    ;
    orig::sqDebugPrintf_v1(s_too_many_shadow_lights_are_being_103ba984);
    return;
  }
  *(uint32_t *)(iVar1 + 0x48 + *(int *)(iVar1 + 0x44) * 4) = param_3;
  *(int *)(iVar1 + 0x44) = *(int *)(iVar1 + 0x44) + 1;
  return;
}
```
