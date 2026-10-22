# Batch r2_113_chasm: 2 functions of `chasm`

Name these functions: see `batches/README.md`. Write `data/names/r2_113_chasm.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10273D60` fn_10273D60

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit10274050_GetInterpolated` (low)
- **Called by:** `sub_1024E280`
- **Named before** (a naming session's reading, low): `Chasm_ApplyAtCurrentIndex`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `StAvatar_UpdateInput` → `sub_1024E280` → `Chasm_ApplyAtCurrentIndex`
- **Named callers' callers:** `StAvatar_UpdateInput`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10274050_GetInterpolated` (low): Returns two interpolated values for an active track entry at a time ratio.

```cpp
XI_API undefined4 __cdecl Chasm_ApplyAtCurrentIndex(int param_1, undefined4 param_2, undefined4 param_3)
{
  int iVar1;
  uint32_t uVar2;
  
  iVar1 = *(int *)(param_1 + 0x14);
  if ((-1 < iVar1) && (iVar1 < *(int *)(param_1 + 0x18))) {
    uVar2 = Unit10274050_GetInterpolated(param_1,iVar1,param_2,param_3);
    return uVar2;
  }
  return 0xffffffff;
}
```

## `0x10273E80` fn_10273E80

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026F900` x4, `Chasm_ToggleLayerState` (low) x2
- **Called by:** `Chasm_ToggleLayerState` (low), `Unit10274050_Relink` (low), `sub_102742C0`
- **Named before** (a naming session's reading, low): `Chasm_ToggleLayerState`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `StAvatar_UpdateInput` → `sub_1024E280` → `Unit10274050_Relink` → `Chasm_ToggleLayerState`
- **Named callers' callers:** `StAvatar_UpdateSteer`
- **How its callers call it** (their verified code):
  - in `Chasm_ToggleLayerState` (low): `XI_API undefined4 __cdecl Chasm_ToggleLayerState(int param_1, int param_2, int param_3)`
  - in `Chasm_ToggleLayerState` (low): `Chasm_ToggleLayerState(param_1,param_2,0);`
  - in `Unit10274050_Relink` (low): `Chasm_ToggleLayerState(param_1,iVar1,0);`
  - in `Unit10274050_Relink` (low): `Chasm_ToggleLayerState(param_1,*(int *)(param_1 + 4),0);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10274050_Relink` (low): Moves an object to a new link id by finding the matching link entry.

```cpp
XI_API undefined4 __cdecl Chasm_ToggleLayerState(int param_1, int param_2, int param_3)
{
  uint32_t *puVar1;
  uint32_t uVar2;
  
  puVar1 = (uint32_t *)(*(int *)(param_1 + 0x10) + param_2 * 0xc);
  uVar2 = *(uint32_t *)(*(int *)(param_1 + 0x10) + param_2 * 0xc);
  if ((uVar2 & 2) != 0) {
    if (param_3 == -1) {
      if ((uVar2 & 4) != 0) {
        Chasm_ToggleLayerState(param_1,param_2,0);
        return *(uint32_t *)(param_1 + 4);
      }
      Chasm_ToggleLayerState(param_1,param_2,1);
      return *(uint32_t *)(param_1 + 4);
    }
    if (param_3 == 0) {
      *(uint32_t *)(param_1 + 4) = 0xffffffff;
      if ((*puVar1 & 4) != 0) {
        orig::FUN_1026f900(puVar1[1],0,0);
        orig::FUN_1026f900(puVar1[2],0,0);
        *puVar1 = *puVar1 & 0xfffffffb;
      }
    }
    else {
      *(int *)(param_1 + 4) = param_2;
      if ((*puVar1 & 4) == 0) {
        orig::FUN_1026f900(puVar1[1],4,1);
        orig::FUN_1026f900(puVar1[2],4,1);
        *puVar1 = *puVar1 | 4;
        return *(uint32_t *)(param_1 + 4);
      }
    }
  }
  return *(uint32_t *)(param_1 + 4);
}
```
