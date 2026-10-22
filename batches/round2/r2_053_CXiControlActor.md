# Batch r2_053_CXiControlActor: 2 functions of `CXiControlActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_053_CXiControlActor.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x100A57C0` fn_100A57C0

- **Gate:** none; **frame set:** static, self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10014390`, `CMoSkeletonElem_UpdateBoneTransform`, `sub_10047960`, `CMoSchedularTask_Interpret`, `CMoPathDriveActorTask_Update` (medium), `CMoPathDriveActorTask_ApplyFraction` (low), `sub_10085E40`, `fn_1008CD90`, `sub_1008F740`, `Acter_SnapHeightToZoneMap` (medium), `sub_10092C20`, `sub_10092F90`, `CXiCollisionActor_OnMove`, `sub_100A57F0`, `sub_100C3900`, `sub_100C47F0`, `sub_100C8ED0`, `sub_100C9330`, `sub_100C9810`, `sub_100C9C10` and 5 more
- **Named before** (a naming session's reading, low): `CXiControlActor_SetVec4`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiControlActor_SetVec4`
- **Named callers' callers:** `CMoSchedularTask_Tick`, `Acter_CopyPosToCached`, `Acter_CopyPosAndSnap`, `Acter_UpdateAllLists`, `CXiSkeletonActor_OnMove`
- **How its callers call it** (their verified code):
  - in `CMoPathDriveActorTask_Update` (medium): `CXiControlActor_SetVec4((void *)uVar10,pfVar1);`
  - in `CMoPathDriveActorTask_ApplyFraction` (low): `CXiControlActor_SetVec4((void *)uVar8,pfVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoPathDriveActorTask_Update` (medium): Advances the path drive by the frame time and applies its position and heading to the actor.
  - `CMoPathDriveActorTask_ApplyFraction` (low): Applies a path fraction to the drive's actor, setting its position and heading.
  - `Acter_SnapHeightToZoneMap` (medium): Snaps the actor's height to the zone map and copies the position into its sub-object.

```cpp
XI_API undefined4 __thiscall CXiControlActor_SetVec4(void * self_, undefined4 * param_1)
{
  uint32_t uVar1;
  
  *(uint32_t *)((int)self_ + 0xd4) = *param_1;
  *(uint32_t *)((int)self_ + 0xd8) = param_1[1];
  *(uint32_t *)((int)self_ + 0xdc) = param_1[2];
  uVar1 = param_1[3];
  *(uint32_t *)((int)self_ + 0xe0) = uVar1;
  return uVar1;
}
```

## `0x100AAEF0` fn_100AAEF0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100A6240`, `sub_100A79A0`, `sub_100A7B80`, `sub_100A8770`, `sub_100CD040`
- **Named before** (a naming session's reading, low): `CXiControlActor_GetSpeed`; name it again from this dossier, or leave it out to keep that name
- **Named callers' callers:** `CXiSkeletonActor_OnMove`

```cpp
XI_API long double __stdcall CXiControlActor_GetSpeed(int * param_1)
{
  char xi_frame[16] __attribute__((aligned(4)));
  float  (&local_10)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 0);
  float  &fStack_8 = *reinterpret_cast<float  *>(xi_frame + 8);
  
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY))((**(code **)(*param_1 + 0x1c8))))((void *)(param_1),0x2c,local_10);
  return SQRT((long double)local_10[0] * (long double)local_10[0] + (long double)fStack_8 * (long double)fStack_8) *
         (long double)0.800000011920928955078125f;
}
```
