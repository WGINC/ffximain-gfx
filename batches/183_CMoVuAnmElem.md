# Batch 183_CMoVuAnmElem: 3 functions of `CMoVuAnmElem`

Name these functions: see `batches/README.md`. Write `data/names/183_CMoVuAnmElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1003EF80` CMoVuAnmElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuAnmElem` slot 12 (+0x30)

```cpp
BOOL CMoVuAnmElem::vf012(int a1)
{
  return a1 == 8;
}
```

## `0x1003EF90` CMoVuAnmElem::vf008

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuAnmElem` slot 8 (+0x20)

```cpp
int CMoVuAnmElem::vf008()
{
  return ((int (__thiscall *)(CMoVuAnmElem *))(*(CMoTask_vtbl * *)((char *)this + 0x0))[1].vf10_CMoTask_vf010)(this);
}
```

## `0x10052CD0` fn_10052CD0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `DynVB_Constructor`, `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052CD0(void * self_)
{
  uint64_t uVar1;
  
  orig::CMoElem_Constructor(self_);
  *(uint8_t ***)self_ = &PTR_CMoD3aElem_GetRuntimeClass_variant0_1032b238;
  *(uint32_t *)((int)self_ + 0x198) = 0;
  *(uint32_t *)((int)self_ + 0x1d4) = 0;
  *(uint16_t *)((int)self_ + 0x1da) = 0;
  uVar1 = orig::DynVB_Constructor_v1((void *)((int)self_ + 0x1dd));
  *(uint8_t ***)self_ = &PTR_CMoVuAnmElem_GetRuntimeClass_1032ac10;
  *(uint16_t *)((int)self_ + 0x176) = 0x231;
  *(uint8_t *)((int)self_ + 0x1dc) = 0;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```
