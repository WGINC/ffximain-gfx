# Batch r2_103_CYyCamMng2: 1 functions of `CYyCamMng2`

Name these functions: see `batches/README.md`. Write `data/names/r2_103_CYyCamMng2.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001E2F0` fn_1001E2F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CYyCamMng2_Construct` (medium), `sub_100A6240`
- **Writes globals:** `0x10456DB0`, `0x10456DB1`, `0x10456DB4`
- **Reads globals:** `0x10456DB0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `CYyCamMng2::CreateGlobal` → `CYyCamMng2_Construct` → `fn_1001E2F0`
- **Named callers' callers:** `CYyCamMng2::CreateGlobal`
- **How its callers call it** (their verified code):
  - in `CYyCamMng2_Construct` (medium): `orig::FUN_1001e2f0(0,1,0,xi_eax);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYyCamMng2_Construct` (medium): Constructs the camera manager and clears its global state.

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1001E2F0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  if (((DAT_10456db0 == '\0') && ((char)param_1 != '\0')) || ((char)param_2 != '\0')) {
    DAT_10456db1 = 0;
  }
  DAT_10456db0 = (char)param_1;
  _DAT_10456db4 = param_3;
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)xi_eax >> 8)) << 8) | (uint8_t)((char)param_1)));
}
```
