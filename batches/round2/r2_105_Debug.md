# Batch r2_105_Debug: 1 functions of `Debug`

Name these functions: see `batches/README.md`. Write `data/names/r2_105_Debug.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10003660` fn_10003660

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Gfx_InitObject` (medium)
- **Named before** (a naming session's reading, low): `Debug_Construct`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `Gfx_InitObject` → `Debug_Construct`
- **Named callers' callers:** `App_InitScene`
- **How its callers call it** (their verified code):
  - in `Gfx_InitObject` (medium): `Debug_Construct((void *)((int)self_ + 4));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Gfx_InitObject` (medium): Initialises the graphics helper object and its sub-object.

```cpp
XI_API void __thiscall Debug_Construct(void * self_)
{
  *(uint8_t ***)self_ = &PTR_FUN_10329660;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint32_t *)((int)self_ + 0x10) = 0;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  return;
}
```
