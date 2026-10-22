# Batch r2_128_sqmoPlaybackChannel: 1 functions of `sqmoPlaybackChannel`

Name these functions: see `batches/README.md`. Write `data/names/r2_128_sqmoPlaybackChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10293000` fn_10293000

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqmoUtil_MarkSlot` (medium)
- **Called by:** `PlaybackChannel_Load` (low), `sub_10292C00`, `sub_10293E40`
- **Named before** (a naming session's reading, low): `SqmoPlaybackChannel_Update`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqMotion\src\sqmoUtil.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `StChannelItemResolve` → `sqmoChannelBuildSQO` → `PlaybackChannel_Load` → `SqmoPlaybackChannel_Update`
- **Named callers' callers:** `sqmoChannelBuildSQO`
- **Named callees' callees:** `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `PlaybackChannel_Load` (low): `SqmoPlaybackChannel_Update((int)puVar5,iVar8);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmoUtil_MarkSlot` (medium): Marks a slot of a slot array as used and reports whether any slot is still free.
  - `PlaybackChannel_Load` (low): Loads a playback channel of version 3 from a stream, allocating its buffers.

```cpp
XI_API void __cdecl SqmoPlaybackChannel_Update(int param_1, int param_2)
{
  SqmoUtil_MarkSlot(*(int *)(param_1 + 0x50),*(int *)(param_1 + 0x1c),param_2,
               (uint8_t *)(param_1 + 0x54));
  return;
}
```
