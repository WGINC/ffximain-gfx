# Batch 152_CMoDisintegrateProgElem: 4 functions of `CMoDisintegrateProgElem`

Name these functions: see `batches/README.md`. Write `data/names/152_CMoDisintegrateProgElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100426C0` CMoDisintegrateProgElem::vf018

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDisintegrateProgElem` slot 18 (+0x48)

```cpp
void CMoDisintegrateProgElem::vf018()
{
  ;
}
```

## `0x100530F0` fn_100530F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_100530F0(void * self_, undefined4 param_1)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint32_t *)((int)self_ + 0x198) = 0;
  *(uint32_t *)((int)self_ + 0x1d4) = 0;
  *(uint16_t *)((int)self_ + 0x1da) = 0;
  *(uint32_t *)((int)self_ + 0x1dc) = param_1;
  *(uint8_t ***)self_ = &PTR_CMoDisintegrateProgElem_GetRuntimeClass_1032b598;
  *(uint16_t *)((int)self_ + 0x176) = 0x1e0;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10053130` CMoDisintegrateProgElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDisintegrateProgElem` slot 12 (+0x30)

```cpp
BOOL CMoDisintegrateProgElem::vf012(int a1)
{
  return a1 == 13;
}
```

## `0x10053140` CMoDisintegrateProgElem::vf008

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDisintegrateProgElem` slot 8 (+0x20)

```cpp
int CMoDisintegrateProgElem::vf008()
{
  return ((int (__thiscall *)(CMoDisintegrateProgElem *))(*(CMoTask_vtbl * *)((char *)this + 0x0))[1].vf10_CMoTask_vf010)(this);
}
```
