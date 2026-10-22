# Batch 179_CMoSpline: 1 functions of `CMoSpline`

Name these functions: see `batches/README.md`. Write `data/names/179_CMoSpline.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10063460` fn_10063460

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10055780`, `fn_10063B80`

```cpp
XI_API undefined4 __thiscall fn_10063460(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CMoSpline_GetRuntimeClass_1032c088;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x10) = 0;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint32_t *)((int)self_ + 0x20) = 0;
  return self_;
}
```
