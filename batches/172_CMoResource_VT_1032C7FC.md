# Batch 172_CMoResource_VT_1032C7FC: 3 functions of `CMoResource_VT_1032C7FC`

Name these functions: see `batches/README.md`. Write `data/names/172_CMoResource_VT_1032C7FC.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10019A30` CMoResource_VT_1032C7FC::vf004

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C7FC` slot 4 (+0x10)

```cpp
int CMoResource_VT_1032C7FC::vf004(int a2)
{
  *this->field_14 = a2;
  return a2;
}
```

## `0x10019A40` CMoResource_VT_1032C7FC::vf005

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C7FC` slot 5 (+0x14)

```cpp
void CMoResource_VT_1032C7FC::vf005(int a1)
{
  ;
}
```

## `0x10071A00` CMoResource_VT_1032C7FC::vf009

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C7FC` slot 9 (+0x24)

```cpp
void CMoResource_VT_1032C7FC::vf009()
{
  ;
}
```
