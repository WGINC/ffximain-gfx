# Batch 173_CMoResource_VT_1032C828: 3 functions of `CMoResource_VT_1032C828`

Name these functions: see `batches/README.md`. Write `data/names/173_CMoResource_VT_1032C828.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10034170` CMoResource_VT_1032C828::vf004

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C828` slot 4 (+0x10)

```cpp
int CMoResource_VT_1032C828::vf004(int a2)
{
  *this->field_14 = a2;
  return a2;
}
```

## `0x10034180` CMoResource_VT_1032C828::vf005

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C828` slot 5 (+0x14)

```cpp
void CMoResource_VT_1032C828::vf005(int a1)
{
  ;
}
```

## `0x100719E0` CMoResource_VT_1032C828::vf009

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C828` slot 9 (+0x24)

```cpp
void CMoResource_VT_1032C828::vf009()
{
  ;
}
```
