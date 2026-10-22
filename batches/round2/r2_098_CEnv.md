# Batch r2_098_CEnv: 1 functions of `CEnv`

Name these functions: see `batches/README.md`. Write `data/names/r2_098_CEnv.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1000D620` fn_1000D620

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CDx_CreateStereoResources`, `sub_1000D3D0`, `App_InitScene`
- **Named before** (a naming session's reading, low): `CEnv_Nop`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `CEnv_Nop`
- **Named callers' callers:** `CApp::RunMainLoop`

```cpp
XI_API void __cdecl CEnv_Nop(void)
{
  return;
}
```
