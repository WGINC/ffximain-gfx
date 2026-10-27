# Batch r4_010_unit_10003890_DirectInput: 2 functions of `unit_10003890_DirectInput`

Name these functions: see `batches/README.md`. Write `data/names/r4_010_unit_10003890_DirectInput.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10008EA0` fn_10008EA0

- **Gate:** GATE: PASS -- 100 trials, 6 returned normally, 0 D3D calls compared (diffemu_gfx, clean build); thin evidence: few trials returned; **frame set:** static
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10005A40`
- **Reads globals:** `0x1045654C`, `0x10456550`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10005A40` → `fn_10008EA0`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API undefined4 __stdcall fn_10008EA0(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  if ((DAT_1045654c_as_s32 != 0) && (DAT_10456550_as_v0 != (code *)0x0)) {
    uVar1 = (*DAT_10456550_as_v0)(param_1,param_2);
    return uVar1;
  }
  return 1;
}
```

## `0x10008F00` fn_10008F00

- **Gate:** GATE: PASS -- 100 trials, 6 returned normally, 0 D3D calls compared (diffemu_gfx, clean build); thin evidence: few trials returned; **frame set:** static
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10005A40`
- **Reads globals:** `0x1045654C`, `0x10456558`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10005A40` → `fn_10008F00`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API undefined4 __stdcall fn_10008F00(undefined4 param_1)
{
  uint32_t uVar1;
  
  if ((DAT_1045654c_as_s32 != 0) && (DAT_10456558_as_v0 != (code *)0x0)) {
    uVar1 = (*DAT_10456558_as_v0)(param_1);
    return uVar1;
  }
  return 1;
}
```
