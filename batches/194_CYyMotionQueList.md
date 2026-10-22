# Batch 194_CYyMotionQueList: 1 functions of `CYyMotionQueList`

Name these functions: see `batches/README.md`. Write `data/names/194_CYyMotionQueList.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001ABF0` fn_1001ABF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10029D30`

```cpp
XI_API void __thiscall fn_1001ABF0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYyMotionQueList_GetRuntimeClass_1032a210;
  *(uint32_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 8) = 0x3f800000;
  return;
}
```
