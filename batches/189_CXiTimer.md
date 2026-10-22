# Batch 189_CXiTimer: 1 functions of `CXiTimer`

Name these functions: see `batches/README.md`. Write `data/names/189_CXiTimer.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10019210` fn_10019210

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10019280`, `fn_10019490`

```cpp
XI_API void __thiscall fn_10019210(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CXiTimer_GetRuntimeClass_variant0_1032a08c;
  *(uint32_t *)((int)self_ + 4) = 0x3f800000;
  *(uint32_t *)((int)self_ + 8) = 0x3f800000;
  return;
}
```
