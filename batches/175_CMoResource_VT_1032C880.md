# Batch 175_CMoResource_VT_1032C880: 2 functions of `CMoResource_VT_1032C880`

Name these functions: see `batches/README.md`. Write `data/names/175_CMoResource_VT_1032C880.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001E050` CMoResource_VT_1032C880::vf004

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C880` slot 4 (+0x10)

```cpp
int CMoResource_VT_1032C880::vf004(int a2)
{
  *this->field_14 = a2;
  return a2;
}
```

## `0x1001E060` CMoResource_VT_1032C880::vf005

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C880` slot 5 (+0x14)

```cpp
void CMoResource_VT_1032C880::vf005(int a1)
{
  ;
}
```
