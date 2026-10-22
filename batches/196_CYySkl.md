# Batch 196_CYySkl: 1 functions of `CYySkl`

Name these functions: see `batches/README.md`. Write `data/names/196_CYySkl.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10034230` fn_10034230

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10029D30`

```cpp
XI_API void __thiscall fn_10034230(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYySkl_GetRuntimeClass_1032a888;
  *(uint32_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint8_t *)((int)self_ + 0x10) = 0;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  return;
}
```
