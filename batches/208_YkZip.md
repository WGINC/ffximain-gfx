# Batch 208_YkZip: 1 functions of `YkZip`

Name these functions: see `batches/README.md`. Write `data/names/208_YkZip.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10244DF0` fn_10244DF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10126320`, `fn_10244F20`
- **Called by:** `GameSystems_InitializeAll`

```cpp
XI_API void * __thiscall fn_10244DF0(void * self_)
{
  orig::FUN_10126320((void *)((int)self_ + 4));
  *(uint16_t *)((int)self_ + 0xe) = 0xffff;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint8_t *)((int)self_ + 0xc) = 1;
  *(uint8_t ***)self_ = &PTR_FUN_1033cb98;
  orig::FUN_10244f20();
  return self_;
}
```
