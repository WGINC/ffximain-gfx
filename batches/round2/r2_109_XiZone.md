# Batch r2_109_XiZone: 1 functions of `XiZone`

Name these functions: see `batches/README.md`. Write `data/names/r2_109_XiZone.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1018B8B0` fn_1018B8B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036170`
- **Called by:** `XiZone_DrawScene`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `XiZone_DrawScene` → `fn_1018B8B0`
- **Named callers' callers:** `Frame_Render`
- **How its callers call it** (their verified code):
  - in `XiZone_DrawScene`: `orig::FUN_1018b8b0(*piVar1);`
  - in `XiZone_DrawScene`: `orig::FUN_1018b8b0(0);`

```cpp
XI_API void __stdcall fn_1018B8B0(int param_1)
{
  orig::FUN_10036170(param_1);
  return;
}
```
