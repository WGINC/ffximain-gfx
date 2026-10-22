# Batch 153_CMoDist: 1 functions of `CMoDist`

Name these functions: see `batches/README.md`. Write `data/names/153_CMoDist.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10052D60` CMoDist::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDist` slot 12 (+0x30)

```cpp
BOOL CMoDist::vf012(int a1)
{
  return a1 == 1;
}
```
