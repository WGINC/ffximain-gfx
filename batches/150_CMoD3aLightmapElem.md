# Batch 150_CMoD3aLightmapElem: 1 functions of `CMoD3aLightmapElem`

Name these functions: see `batches/README.md`. Write `data/names/150_CMoD3aLightmapElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052C30` fn_10052C30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052C30(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint8_t ***)self_ = &PTR_CMoD3aLightmapElem_GetRuntimeClass_1032b1d8;
  *(uint16_t *)((int)self_ + 0x176) = 0x198;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```
