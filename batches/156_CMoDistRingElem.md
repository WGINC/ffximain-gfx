# Batch 156_CMoDistRingElem: 2 functions of `CMoDistRingElem`

Name these functions: see `batches/README.md`. Write `data/names/156_CMoDistRingElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052FD0` fn_10052FD0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052FD0(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint8_t ***)self_ = &PTR_CMoDistRingElem_GetRuntimeClass_1032b4d8;
  *(uint16_t *)((int)self_ + 0x176) = 0x1ba;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10052FF0` CMoDistRingElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDistRingElem` slot 12 (+0x30)

```cpp
BOOL CMoDistRingElem::vf012(int a1)
{
  return a1 == 5;
}
```
