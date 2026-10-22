# Batch 151_CMoD3mSpecularElem: 1 functions of `CMoD3mSpecularElem`

Name these functions: see `batches/README.md`. Write `data/names/151_CMoD3mSpecularElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100410E0` CMoD3mSpecularElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 12 (+0x30)

```cpp
BOOL CMoD3mSpecularElem::vf012(int a1)
{
  return a1 == 7;
}
```
