# Batch 166_CMoResource_VT_1032C588: 3 functions of `CMoResource_VT_1032C588`

Name these functions: see `batches/README.md`. Write `data/names/166_CMoResource_VT_1032C588.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100745E0` CMoResource_VT_1032C588::vf011

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoResource_VT_1032C588::vf011()
{
  return (int)orig::CMoCamera_GetRuntimeClass();
}
```

## `0x100745F0` CMoResource_VT_1032C588::vf012

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)

```cpp
int CMoResource_VT_1032C588::vf012(int a1)
{
  Vt_1032C5B4 * self = reinterpret_cast<Vt_1032C5B4 *>(this);  // byte-exact view for the rest
  return (int)orig::CMoResource_VT_1032C588_ScalarDeletingDtor_10071C60((unsigned int **)self - 12, a1);
}
```

## `0x10074610` CMoResource_VT_1032C588::vf008

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C588` slot 8 (+0x20)

```cpp
void CMoResource_VT_1032C588::vf008()
{
  ;
}
```
