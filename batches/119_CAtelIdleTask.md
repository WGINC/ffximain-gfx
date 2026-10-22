# Batch 119_CAtelIdleTask: 1 functions of `CAtelIdleTask`

Name these functions: see `batches/README.md`. Write `data/names/119_CAtelIdleTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100142B0` fn_100142B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `App_AdvanceSeqStep`, `sub_10028FD0`, `sub_10074740`, `CMoTask::ResetFields`, `sub_100749B0`, `fn_1015E910`, `fn_101AB730`
- **Called by:** `fn_10014170`
- **Writes globals:** `0x1045690C`, `0x1047D13C`
- **Reads globals:** `0x104568FC`, `0x10456A28`

```cpp
XI_API undefined4 __cdecl fn_100142B0(void)
{
  undefined4 *self_;
  uint32_t uVar1;
  undefined4 uVar2;
  uint64_t uVar3;
  
  orig::FUN_1015e910(&g_menuMng_as_v0,aMenuChdummy);
  *g_pSceneState_as_v2 = 0x60;
  orig::FUN_101ab730();
  uVar3 = orig::Mem_AllocFromArea_v1(0x34,4);
  self_ = (undefined4 *)uVar3;
  uVar1 = orig::CMoTask_ResetFields_v1(self_);
  g_resourcesDirty_as_v0 = 1;
  if (self_ != (undefined4 *)0x0) {
    orig::CMoTask_Constructor_v1(self_);
    *self_ = &PTR_CAtelIdleTask_GetRuntimeClass_10329cfc;
    DAT_1045690c = self_;
    uVar2 = orig::CMoTask_RegisterWithMng_v1(0xd);
    orig::App_AdvanceSeqStep_v1(g_pCamera_as_v2);
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar2 >> 8)) << 8) | (uint8_t)(1)));
  }
  DAT_1045690c = (undefined4 *)0x0;
  return uVar1 & 0xffffff00;
}
```
