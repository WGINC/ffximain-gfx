# Batch 197_CYySoundElem: 1 functions of `CYySoundElem`

Name these functions: see `batches/README.md`. Write `data/names/197_CYySoundElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100369F0` fn_100369F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036980`, `CMoElem::Constructor`, `fn_10044AA0`
- **Called by:** `fn_10037020`, `sub_100370D0`, `sub_1004E8D0`

```cpp
XI_API undefined4 __thiscall fn_100369F0(void * self_)
{
  orig::CMoElem_Constructor(self_);
  *(uint8_t ***)self_ = &PTR_LAB_1032a930;
  *(uint32_t *)((int)self_ + 0x138) = 0;
  *(uint32_t *)((int)self_ + 0x1e0) = 0;
  *(uint32_t *)((int)self_ + 0x1c0) = 0;
  *(uint32_t *)((int)self_ + 0x1c4) = 0;
  *(uint32_t *)((int)self_ + 0x1c8) = 0;
  *(uint32_t *)((int)self_ + 0x1cc) = 0;
  *(uint16_t *)((int)self_ + 0x1bc) = 0;
  *(uint32_t *)((int)self_ + 0x198) = 0;
  orig::FUN_10044aa0(self_,0);
  *(uint32_t *)((int)self_ + 0x19c) = 0;
  *(uint32_t *)((int)self_ + 0x108) = 0;
  *(uint8_t *)((int)self_ + 0x1e8) = 0;
  *(uint32_t *)((int)self_ + 0x1e9) = 0;
  *(uint32_t *)((int)self_ + 0x1e4) = 0x40400000;
  orig::FUN_10036980(self_);
  *(uint16_t *)((int)self_ + 0x176) = 0x1ed;
  return self_;
}
```
