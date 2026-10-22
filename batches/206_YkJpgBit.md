# Batch 206_YkJpgBit: 1 functions of `YkJpgBit`

Name these functions: see `batches/README.md`. Write `data/names/206_YkJpgBit.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101D08E0` fn_101D08E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_101D0370`, `sub_101D0640`

```cpp
XI_API undefined4 __thiscall fn_101D08E0(void * self_, undefined4 param_1, undefined4 param_2)
{
  *(uint8_t *)((int)self_ + 5) = 0;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint8_t ***)self_ = &PTR_YkJpgBit_GetRuntimeClass_10337e98;
  *(uint8_t *)((int)self_ + 4) = 0x80;
  *(uint32_t *)((int)self_ + 0x10) = param_2;
  *(uint32_t *)((int)self_ + 8) = param_1;
  return self_;
}
```
