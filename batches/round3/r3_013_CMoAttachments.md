# Batch r3_013_CMoAttachments: 2 functions of `CMoAttachments`

Name these functions: see `batches/README.md`. Write `data/names/r3_013_CMoAttachments.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1003B6D0` fn_1003B6D0

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `ActorHandle_Resolve`
- **Called by:** `sub_1003B5E0`, `CMoAttachments_ResetAll` (medium), `sub_1003B730`, `CMoAttachments::SetCaster`, `CMoAttachments_FindKindInChain` (low), `CMoAttachments_FindKindFromRoot` (low), `YmGenerater_BuildAttachMatrix` (medium), `CMoAttachments_NotifyCaster` (low), `sub_1003C720`, `sub_1003CBF0`, `sub_100439C0`, `sub_10043AB0`, `sub_10043B00`, `sub_10043BB0`, `sub_10043CB0`, `sub_10043D00`, `CMoGeneratorClone_ExecuteCmdList` (medium), `sub_1004E8D0`, `SharedConditionalStateReset`, `sub_10053E60` and 55 more
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `CYySoundElem_UpdateQueue` → `fn_100364A0` → `CYySoundElem_UpdateTimerAndPosition` → `CMoGeneratorClone_ExecuteCmdList` → `fn_1003B6D0`
- **Named callers' callers:** `CYySoundElem_UpdateTimerAndPosition`, `ReleaseSubObjectIfPresent`, `CMoAttachments::ClearCaster`, `Unit1003C6E0_GetFactorFromAttachments`, `CMoTask_VT_1032AF60::ScalarDeletingDtor`, `CMoTask_VT_1032AFA8::ScalarDeletingDtor`, `CMoElem_AdvanceTimer`, `CMoGeneratorClone_FixupClonedPointers`, `CMoGeneratorClone_CreateInstance`, `YmGenerater_UpdateAttach`, `CMoGeneratorClone::Destructor`, `Scheduler_CreateTask` and 9 more
- **Named callees' callees:** `ActorHandle_Constructor`, `ActorHandle_Destructor`, `ActorHandle_SetFields`
- **How its callers call it** (their verified code):
  - in `CMoAttachments_ResetAll` (medium): `uVar2 = orig::FUN_1003b6d0(self);`
  - in `CMoAttachments::SetCaster`: `uVar2 = orig::FUN_1003b6d0(self);`
  - in `CMoAttachments::SetCaster`: `uVar2 = orig::FUN_1003b6d0(self);`
  - in `CMoAttachments_FindKindInChain` (low): `uVar2 = orig::FUN_1003b6d0(self);`
  - in `CMoAttachments_FindKindInChain` (low): `uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));`
  - in `CMoAttachments_FindKindFromRoot` (low): `uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoAttachments_ResetAll` (medium): Clears the caster and target of an attachment object and resets it.
  - `CMoAttachments_FindKindInChain` (low): Walks the link chain from self and returns the first object of the given class.
  - `CMoAttachments_FindKindFromRoot` (low): Walks the link chain from the root object and returns the first object of the given class.
  - `YmGenerater_BuildAttachMatrix` (medium): 
  - `CMoAttachments_NotifyCaster` (low): Passes this to the caster object's notification slot.
  - `CMoGeneratorClone_ExecuteCmdList` (medium): Runs each command in the generator's command list according to its type.
  - `CMoPathDriveActorTask_Update` (medium): Advances the path drive by the frame time and applies its position and heading to the actor.
  - `CMoPathDriveActorTask_ApplyFraction` (low): Applies a path fraction to the drive's actor, setting its position and heading.
  - `AfterimageTask_FindFirstKind` (low): Returns the first object of the expected kind in the chain from the task's +0x34 link.
  - `Scheduler_ResolveOwner` (low): Follows a chain of handle objects of one class to the owner.

```cpp
XI_API undefined4 __thiscall fn_1003B6D0(void * self_)
{
  int iVar1;
  uint64_t uVar2;
  
  if (*(int *)((int)self_ + 0x1c) == 0x4000000) {
    return *(uint32_t *)((int)self_ + 0xc);
  }
  iVar1 = *(int *)((int)self_ + 0xc);
  uVar2 = orig::ActorHandle_Resolve_v1((void *)((int)self_ + 0x14));
  if ((int)uVar2 == iVar1) {
    uVar2 = orig::ActorHandle_Resolve_v1((void *)((int)self_ + 0x14));
    return (int)uVar2;
  }
  return 0;
}
```

## `0x1003B700` fn_1003B700

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `ActorHandle_Resolve`
- **Called by:** `sub_1003B610`, `CMoAttachments_ResetAll` (medium), `sub_1003B740`, `CMoAttachments::SetTarget`, `CMoAttachments_FindKindFromRoot` (low), `sub_1003B950`, `YmGenerater_BuildAttachMatrix` (medium), `CMoAttachments_NotifyTarget` (low), `sub_1003C730`, `sub_1003CBF0`, `YmElem_DrawFloatingText` (medium), `sub_10047830`, `CMoGeneratorClone_ExecuteCmdList` (medium), `sub_1004E8D0`, `SharedConditionalStateReset`, `Unit10053860_StopGenerators` (medium), `sub_10053E60`, `CMoSchedularTask::Destructor`, `sub_100575A0`, `sub_10057BF0` and 6 more
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `CYySoundElem_UpdateQueue` → `fn_100364A0` → `CYySoundElem_UpdateTimerAndPosition` → `CMoGeneratorClone_ExecuteCmdList` → `fn_1003B700`
- **Named callers' callers:** `CYySoundElem_UpdateTimerAndPosition`, `ReleaseSubObjectIfPresent`, `CMoAttachments::ClearTarget`, `Unit1003C6E0_GetFactorFromAttachments`, `CMoElem_AdvanceTimer`, `CMoGeneratorClone_FixupClonedPointers`, `CMoGeneratorClone_CreateInstance`, `YmGenerater_UpdateAttach`, `CMoGeneratorClone::Destructor`, `Scheduler_CreateTask`, `Scheduler_Kill`, `Scheduler_IsMoving` and 5 more
- **Named callees' callees:** `ActorHandle_Constructor`, `ActorHandle_Destructor`, `ActorHandle_SetFields`
- **How its callers call it** (their verified code):
  - in `CMoAttachments_ResetAll` (medium): `uVar2 = orig::FUN_1003b700(self);`
  - in `CMoAttachments::SetTarget`: `uVar3 = orig::FUN_1003b700(self);`
  - in `CMoAttachments::SetTarget`: `uVar3 = orig::FUN_1003b700(self);`
  - in `CMoAttachments_FindKindFromRoot` (low): `uVar2 = orig::FUN_1003b700(self);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoAttachments_ResetAll` (medium): Clears the caster and target of an attachment object and resets it.
  - `CMoAttachments_FindKindFromRoot` (low): Walks the link chain from the root object and returns the first object of the given class.
  - `YmGenerater_BuildAttachMatrix` (medium): 
  - `CMoAttachments_NotifyTarget` (low): Passes this to the target object's notification slot.
  - `YmElem_DrawFloatingText` (medium): 
  - `CMoGeneratorClone_ExecuteCmdList` (medium): Runs each command in the generator's command list according to its type.
  - `Unit10053860_StopGenerators` (medium): Stops the generators of the mesh resources owned by an object and releases them.
  - `Scheduler_ResolveOwner` (low): Follows a chain of handle objects of one class to the owner.
  - `AfterimageTask_SetTarget` (medium): Sets the target of the afterimage task, notifying the old and new objects.

```cpp
XI_API undefined4 __thiscall fn_1003B700(void * self_)
{
  int iVar1;
  uint64_t uVar2;
  
  if (*(int *)((int)self_ + 0x28) == 0x4000000) {
    return *(uint32_t *)((int)self_ + 0x10);
  }
  iVar1 = *(int *)((int)self_ + 0x10);
  uVar2 = orig::ActorHandle_Resolve_v1((void *)((int)self_ + 0x20));
  if ((int)uVar2 == iVar1) {
    uVar2 = orig::ActorHandle_Resolve_v1((void *)((int)self_ + 0x20));
    return (int)uVar2;
  }
  return 0;
}
```
