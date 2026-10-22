# Batch 198_CYyTexMng: 1 functions of `CYyTexMng`

Name these functions: see `batches/README.md`. Write `data/names/198_CYyTexMng.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1003AA50` fn_1003AA50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `App_InitScene`

```cpp
XI_API undefined4 __thiscall fn_1003AA50(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYyTexMng_GetRuntimeClass_1032aa90;
  *(uint16_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint8_t *)((int)self_ + 0x10) = 0;
  return self_;
}
```
