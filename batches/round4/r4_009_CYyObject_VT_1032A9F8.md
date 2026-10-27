# Batch r4_009_CYyObject_VT_1032A9F8: 1 functions of `CYyObject_VT_1032A9F8`

Name these functions: see `batches/README.md`. Write `data/names/r4_009_CYyObject_VT_1032A9F8.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10039590` fn_10039590

- **Gate:** GATE: PASS -- 100 trials, 100 returned normally, 0 D3D calls compared (diffemu_gfx, clean build); **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `LobbyThread_IsSystemState1` (low)
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Unit10014350_StepMenuOkThenRelease` → `Unit10237880_ForwardFlag1` → `sub_1023BE10` → `Unit10237880_WaitForState1` → `LobbyThread_IsSystemState1` → `fn_10039590`
- **Named callers' callers:** `Unit10237880_WaitForState1`
- **How its callers call it** (their verified code):
  - in `LobbyThread_IsSystemState1` (low): `uVar1 = orig::FUN_10039590(DAT_10669098,1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_IsSystemState1` (low): Tests whether the system at 0x10669098 is in state 1.

```cpp
XI_API undefined4 __thiscall fn_10039590(void * self_, undefined4 xi_s1)
{
  uint32_t uVar1;
  
  if (*(int *)((int)self_ + 8) != 0) {
    uVar1 = orig::FUN_10039700(xi_s1);
    return uVar1;
  }
  return 1;
}
```
