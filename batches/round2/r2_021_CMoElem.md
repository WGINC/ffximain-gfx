# Batch r2_021_CMoElem: 3 functions of `CMoElem`

Name these functions: see `batches/README.md`. Write `data/names/r2_021_CMoElem.json`.
5 of them were sampled in a running frame (data/frame_set.json).

## `0x10036AB0` CMoElem::vf019

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3aLightmapElem` slot 19 (+0x4C), `CXi471011C8_VT_1032AD80` slot 19 (+0x4C), `CMoPointLightProgElem` slot 19 (+0x4C), `CXi471011C8_VT_1032B3B8` slot 19 (+0x4C), `CMoD3aElem` slot 19 (+0x4C), `CMoSkeletonElem` slot 19 (+0x4C), `CMoNullProgElem` slot 19 (+0x4C), `CMoDistMorphElem` slot 19 (+0x4C) and 13 more

```cpp
void CMoElem::vf019()
{
  ;
}
```

## `0x10044AC0` fn_10044AC0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 16 (+0x40), `CMoD3aLightmapElem` slot 16 (+0x40), `CXi471011C8_VT_1032AD80` slot 16 (+0x40), `CMoPointLightProgElem` slot 16 (+0x40), `CXi471011C8_VT_1032B3B8` slot 16 (+0x40), `CMoD3aElem` slot 16 (+0x40), `CMoSkeletonElem` slot 16 (+0x40), `CMoNullProgElem` slot 16 (+0x40) and 13 more

```cpp
XI_API unsigned char __thiscall fn_10044AC0(void * self_)
{
  void *this_00;
  uint32_t uVar1;
  
  if (*(uint32_t **)((int)self_ + 0xfc) == (uint32_t *)0x0) {
    this_00 = (void *)0x0;
  }
  else {
    this_00 = (void *)**(uint32_t **)((int)self_ + 0xfc);
  }
  if (this_00 != (void *)0x0) {
    uVar1 = orig::FUN_100539e0(this_00);
    return SUB41(uVar1,0);
  }
  return false;
}
```

## `0x100458F0` fn_100458F0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100555B0`
- **Called by:** `CMoElem_PrepareDraw` (medium), `sub_10045AE0`
- **How its callers call it** (their verified code):
  - in `CMoElem_PrepareDraw` (medium): `uVar6 = orig::FUN_100458f0(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoElem_PrepareDraw` (medium): Computes the element's draw key and draw matrix for this frame, and tests it.

```cpp
XI_API undefined4 __thiscall fn_100458F0(void * self_)
{
  uint32_t uVar1;
  uint32_t uVar2;
  
  uVar1 = (uint32_t)(!((long double)*(float *)((int)self_ + 0x138) >= (long double)0.00999999977648258209228515625f));
  if (uVar1 == 0) {
    uVar2 = orig::FUN_100555b0(self_,(float)(long double)*(float *)((int)self_ + 300));
    return uVar2;
  }
  *(uint32_t *)((int)self_ + 0x138) = 0x3f800000;
  return uVar1 << 8;
}
```
