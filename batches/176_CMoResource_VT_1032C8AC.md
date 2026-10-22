# Batch 176_CMoResource_VT_1032C8AC: 1 functions of `CMoResource_VT_1032C8AC`

Name these functions: see `batches/README.md`. Write `data/names/176_CMoResource_VT_1032C8AC.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001DC20` CMoResource_VT_1032C8AC::vf004

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C8AC` slot 4 (+0x10)

```cpp
int CMoResource_VT_1032C8AC::vf004(int a2)
{
  *this->field_14 = a2;
  return a2;
}
```
