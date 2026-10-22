# Batch 185_CMoVuLineProgElem: 3 functions of `CMoVuLineProgElem`

Name these functions: see `batches/README.md`. Write `data/names/185_CMoVuLineProgElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052BC0` fn_10052BC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052BC0(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint32_t *)((int)self_ + 0x198) = 0;
  *(uint32_t *)((int)self_ + 0x1d4) = 0;
  *(uint16_t *)((int)self_ + 0x1da) = 0;
  *(uint8_t ***)self_ = &PTR_CMoVuLineProgElem_GetRuntimeClass_1032b178;
  *(uint16_t *)((int)self_ + 0x176) = 0x1e8;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10052BF0` CMoVuLineProgElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuLineProgElem` slot 12 (+0x30)

```cpp
BOOL CMoVuLineProgElem::vf012(int a1)
{
  return a1 == 9;
}
```

## `0x10064180` CMoVuLineProgElem::vf018

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoVuLineProgElem` slot 18 (+0x48)

```cpp
void CMoVuLineProgElem::vf018()
{
  ;
}
```
