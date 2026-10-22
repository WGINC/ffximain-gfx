# Batch 155_CMoDistMorphElem: 2 functions of `CMoDistMorphElem`

Name these functions: see `batches/README.md`. Write `data/names/155_CMoDistMorphElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052E20` fn_10052E20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXi471011C8_VT_1032AC90_Ctor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052E20(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CXi471011C8_VT_1032AC90_Ctor(self_);
  *(uint8_t ***)self_ = &PTR_CMoDistMorphElem_GetRuntimeClass_1032b358;
  *(uint16_t *)((int)self_ + 0x176) = 0x1c4;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10052E40` CMoDistMorphElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDistMorphElem` slot 12 (+0x30)

```cpp
BOOL CMoDistMorphElem::vf012(int a1)
{
  return a1 == 6;
}
```
