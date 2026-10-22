# Batch r2_127_sqmoFrameChannel: 1 functions of `sqmoFrameChannel`

Name these functions: see `batches/README.md`. Write `data/names/r2_127_sqmoFrameChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102944E0` fn_102944E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqmoUtil_MarkSlot` (medium)
- **Called by:** `SqmoBuild_ReadFrameChannel` (medium), `sub_102945A0`, `sub_102947F0`, `sub_102A47E0`
- **Named before** (a naming session's reading, low): `SqmoFrameChannel_Apply`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqMotion\src\sqmoUtil.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `StChannelItemResolve` → `sqmoChannelBuildSQO` → `SqmoBuild_ReadFrameChannel` → `SqmoFrameChannel_Apply`
- **Named callers' callers:** `sqmoChannelBuildSQO`
- **Named callees' callees:** `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `SqmoBuild_ReadFrameChannel` (medium): `SqmoFrameChannel_Apply((int)puVar5,iVar7);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmoUtil_MarkSlot` (medium): Marks a slot of a slot array as used and reports whether any slot is still free.
  - `SqmoBuild_ReadFrameChannel` (medium): Reads a frame channel from a motion stream.

```cpp
XI_API void __cdecl SqmoFrameChannel_Apply(int param_1, int param_2)
{
  SqmoUtil_MarkSlot(*(int *)(param_1 + 0x38),*(int *)(param_1 + 0x1c),param_2,
               (uint8_t *)(param_1 + 0x3c));
  return;
}
```
