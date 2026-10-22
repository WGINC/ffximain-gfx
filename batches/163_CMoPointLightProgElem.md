# Batch 163_CMoPointLightProgElem: 2 functions of `CMoPointLightProgElem`

Name these functions: see `batches/README.md`. Write `data/names/163_CMoPointLightProgElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100562F0` fn_100562F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_100562F0(void * self_, uint32_t param_1, undefined4 param_2)
{
  orig::CMoElem_Constructor(self_);
  *(uint32_t *)((int)self_ + 0x194) = param_1;
  *(uint8_t ***)self_ = &PTR_CMoPointLightProgElem_GetRuntimeClass_1032b758;
  *(uint16_t *)((int)self_ + 0x176) = 0x1b4;
  *(uint8_t *)((int)self_ + 0x1b0) = 0;
  *(uint32_t *)((int)self_ + 0x1ac) = 0xffffffff;
  *(uint32_t *)((int)self_ + 0x198) = param_2;
  if (((param_1 & 0xff) != 99) && ((param_1 & 0xff) != 0x43)) {
    *(uint8_t *)((int)self_ + 0x1b1) = 0;
    return ((uint64_t)(((uint64_t)(uint32_t)(param_2) << 32) | (uint32_t)(self_)));
  }
  *(uint8_t *)((int)self_ + 0x1b1) = 1;
  return ((uint64_t)(((uint64_t)(uint32_t)(param_2) << 32) | (uint32_t)(self_)));
}
```

## `0x10056360` CMoPointLightProgElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoPointLightProgElem` slot 12 (+0x30)

```cpp
BOOL CMoPointLightProgElem::vf012(int a1)
{
  return a1 == 11;
}
```
