# Batch r2_119_sqgxDlist: 1 functions of `sqgxDlist`

Name these functions: see `batches/README.md`. Write `data/names/r2_119_sqgxDlist.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029BB50` fn_1029BB50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit102A2130_CopyBounds` (low)
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldEnd` → `StAvatar_RenderIfEnabled` → `StAvatarRender` → `StAvatar_DrawScene` → `sqsnSceneDraw` → `sqsnSceneLayerDraw` → `Unit102A2130_CopyBounds` → `fn_1029BB50`
- **Named callers' callers:** `sqsnSceneLayerDraw`, `sqsnSceneLayerDrawShadowed`
- **How its callers call it** (their verified code):
  - in `Unit102A2130_CopyBounds` (low): `orig::FUN_1029bb50();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit102A2130_CopyBounds` (low): Copies a layer's bounding box and runs its follow-up step.

```cpp
XI_API void __cdecl fn_1029BB50(void)
{
  return;
}
```
