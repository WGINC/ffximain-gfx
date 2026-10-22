# Batch r2_166_unit_1005FFD0_AfterimageTask: 1 functions of `unit_1005FFD0_AfterimageTask`

Name these functions: see `batches/README.md`. Write `data/names/r2_166_unit_1005FFD0_AfterimageTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100627D0` fn_100627D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0`, `sub_1003B700`
- **Called by:** `Scheduler_Kill`, `sub_10056F70`, `Scheduler_IsMoving`, `sub_100572E0`, `sub_100575A0`, `CMoSchedularTask_Interpret`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_1008ABE0` → `AfterimageTask_Kill` → `Scheduler_Kill` → `fn_100627D0`
- **Named callers' callers:** `CMoSchedularTask_Tick`, `AfterimageTask_Kill`, `CXiActor_KillAction`, `CXiActor_IsMovingAction`
- **Named callees' callees:** `ActorHandle_Resolve`

```cpp
XI_API uint64_t __thiscall fn_100627D0(void * self_)
{
  char cVar1;
  void *this_00;
  uint64_t uVar2;
  uint64_t uVar3;
  
  uVar2 = orig::FUN_1003b700((void *)((int)self_ + 0x34));
  this_00 = (void *)uVar2;
  if (this_00 != (void *)0x0) {
    uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
    uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
    cVar1 = (char)uVar3;
    while (cVar1 != '\0') {
      uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
      this_00 = (void *)uVar2;
      if (this_00 == (void *)0x0) {
        uVar2 = orig::FUN_1003b700((void *)((int)self_ + 0x34));
        return uVar2;
      }
      uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
      uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
      cVar1 = (char)uVar3;
    }
  }
  return uVar2;
}
```
