# Batch 161_CMoOT: 1 functions of `CMoOT`

Name these functions: see `batches/README.md`. Write `data/names/161_CMoOT.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100552A0` fn_100552A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_100552F0`
- **Called by:** `fn_100555D0`

```cpp
XI_API void * __thiscall fn_100552A0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CMoOT_GetRuntimeClass_1032b6f4;
  orig::FUN_100552f0(self_);
  return self_;
}
```
