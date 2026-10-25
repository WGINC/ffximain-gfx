# Batch r3_049_unit_10073750_ResourceIter: 1 functions of `unit_10073750_ResourceIter`

Name these functions: see `batches/README.md`. Write `data/names/r3_049_unit_10073750_ResourceIter.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10073D80` fn_10073D80

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Mem_AllocFromArea`, `Unit10072A50_Start` (low), `CMoTask::Constructor`, `CMoTask::ResetFields`, `sub_10074A00`, `fn_10074F30`
- **Called by:** `AfterimageTask_Kill` (medium), `sub_10062ED0`
- **Writes globals:** `0x1047D13C`
- **Reads globals:** `0x1047D124`, `0x1047D140`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_resourcesDirty`, `dword_1047D140`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `sub_100DAF30` → `sub_1008ABE0` → `AfterimageTask_Kill` → `fn_10073D80`
- **Named callers' callers:** `Acter_StepAfterimageStates`
- **Named callees' callees:** `_exit_0`, `MemBlock_AllocFromFront`, `MemBlock_AllocFromBack`, `ResourceIter_PopFreeNode`, `CMoTaskMng::Forward10075120`
- **How its callers call it** (their verified code):
  - in `AfterimageTask_Kill` (medium): `uVar2 = orig::FUN_10073d80(&param_1,param_1,1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `AfterimageTask_Kill` (medium): Stops the afterimage scheduler entry for a resource.
  - `Unit10072A50_Start` (low): Starts a mesh load task for a file id.

Short callee `CMoTask::Constructor`:
```cpp
void * __thiscall CMoTask::Constructor(void * self)
{
  *(uint8_t ***)self = &PTR_CMoTask_GetRuntimeClass_variant12_1032c978;
  if (byte_1047D13D == '\0') {
    orig::CMoTask_ResetFields_v2(self);
    orig::FUN_100749f0_v1();
  }
  return self;
}
```

```cpp
XI_API void __stdcall fn_10073D80(undefined4 * param_1, int param_2, undefined4 param_3)
{
  int iVar1;
  undefined4 *self_;
  uint32_t uVar2;
  
  if (DAT_1047d124 == (int *)0x0) {
    iVar1 = 0;
  }
  else {
    iVar1 = *DAT_1047d124;
  }
  while( true ) {
    if (iVar1 == 0) {
      if (((char)param_3 != '\0') &&
         (iVar1 = orig::FUN_10074f30_v1(dword_1047D140_as_v1,param_2,0x4d727400), iVar1 == 0)) {
        self_ = orig::Mem_AllocFromArea_v2(0x49,4);
        orig::CMoTask_ResetFields_v2(self_);
        g_resourcesDirty_as_v0 = 1;
        if (self_ == (undefined4 *)0x0) {
          self_ = (undefined4 *)0x0;
        }
        else {
          orig::CMoTask_Constructor_v2(self_);
          *self_ = &PTR_CMoTask_GetRuntimeClass_variant12_1032c908;
          self_[0xe] = 0;
          *(undefined4 *)((int)self_ + 0x45) = 0;
        }
        orig::FUN_10074a00_v1();
        orig::Unit10072A50_Start_v1(self_,&param_3,param_2);
      }
      *param_1 = 0;
      return;
    }
    uVar2 = *(uint32_t *)(iVar1 + 0x24) & 0x7f;
    if (((uVar2 == 2) || (uVar2 == 1)) && (*(int *)(iVar1 + 0x34) == param_2)) break;
    if (*(int **)(iVar1 + 0x2c) == (int *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = **(int **)(iVar1 + 0x2c);
    }
  }
  *param_1 = *(undefined4 *)(iVar1 + 0x14);
  return;
}
```
