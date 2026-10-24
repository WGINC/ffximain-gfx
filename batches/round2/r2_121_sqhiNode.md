# Batch r2_121_sqhiNode: 2 functions of `sqhiNode`

Name these functions: see `batches/README.md`. Write `data/names/r2_121_sqhiNode.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10291C50` fn_10291C50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `SqskSkeleton_SetNodeCount` (medium), `sub_10291C20`
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029ADF0` → `sub_1029AF20` → `SqskSkeleton_SetNodeCount` → `fn_10291C50`
- **How its callers call it** (their verified code):
  - in `SqskSkeleton_SetNodeCount` (medium): `orig::FUN_10291c50();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqskSkeleton_SetNodeCount` (medium): Resizes a skeleton's node arrays to a new count.

```cpp
XI_API void __cdecl fn_10291C50(void)
{
  return;
}
```

## `0x10291F00` fn_10291F00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10288FF0`, `sub_1029FFF0`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `fn_10291F00`

```cpp
XI_API void __cdecl fn_10291F00(int param_1, undefined4 param_2)
{
  if (param_1 != 0) {
    *(uint32_t *)(param_1 + 100) = param_2;
  }
  return;
}
```
