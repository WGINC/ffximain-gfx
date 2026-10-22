# Batch 184_CMoVuDist: 3 functions of `CMoVuDist`

Name these functions: see `batches/README.md`. Write `data/names/184_CMoVuDist.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052DA0` fn_10052DA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052DA0(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint32_t *)((int)self_ + 0x19c) = 0;
  *(uint32_t *)((int)self_ + 0x1d8) = 0;
  *(uint16_t *)((int)self_ + 0x1de) = 0;
  *(uint8_t ***)self_ = &PTR_CMoVuDist_GetRuntimeClass_1032b2f8;
  *(uint16_t *)((int)self_ + 0x176) = 0x1e0;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10052DD0` CMoVuDist::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuDist` slot 12 (+0x30)

```cpp
BOOL CMoVuDist::vf012(int a1)
{
  return a1 == 10;
}
```

## `0x10052DE0` CMoVuDist::vf008

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuDist` slot 8 (+0x20)

```cpp
int CMoVuDist::vf008()
{
  return ((int (__thiscall *)(CMoVuDist *))(*(CMoTask_vtbl * *)((char *)this + 0x0))[1].vf10_CMoTask_vf010)(this);
}
```
