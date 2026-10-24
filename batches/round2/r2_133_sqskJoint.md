# Batch r2_133_sqskJoint: 1 functions of `sqskJoint`

Name these functions: see `batches/README.md`. Write `data/names/r2_133_sqskJoint.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10291460` fn_10291460

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `SqskSkeleton_SetNodeCount` (medium), `sub_10291430`
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029ADF0` → `sub_1029AF20` → `SqskSkeleton_SetNodeCount` → `fn_10291460`
- **How its callers call it** (their verified code):
  - in `SqskSkeleton_SetNodeCount` (medium): `orig::FUN_10291460();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqskSkeleton_SetNodeCount` (medium): Resizes a skeleton's node arrays to a new count.

```cpp
XI_API void __cdecl fn_10291460(void)
{
  return;
}
```
