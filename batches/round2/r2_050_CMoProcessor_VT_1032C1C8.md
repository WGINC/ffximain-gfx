# Batch r2_050_CMoProcessor_VT_1032C1C8: 2 functions of `CMoProcessor_VT_1032C1C8`

Name these functions: see `batches/README.md`. Write `data/names/r2_050_CMoProcessor_VT_1032C1C8.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1006CC40` CMoProcessor_VT_1032C1C8::vf008

- **Gate:** none; **frame set:** self
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 8 (+0x20), `CMoProcessor_VT_1032C298` slot 8 (+0x20), `CMoProcessor_VT_1032C1C8` slot 8 (+0x20)

```cpp
int CMoProcessor_VT_1032C1C8::vf008(void * a1, void * a2, void * a3)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, void *, void *, void *))(*(_DWORD *)self + 36))(self, a1, a2, a3);
}
```

## `0x1006CC60` CMoProcessor_VT_1032C1C8::vf040

- **Gate:** none; **frame set:** self
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 40 (+0xA0), `CMoProcessor_VT_1032C298` slot 40 (+0xA0), `CMoProcessor_VT_1032C1C8` slot 40 (+0xA0)

```cpp
int CMoProcessor_VT_1032C1C8::vf040(float * a1)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, float *))(*(_DWORD *)self + 164))(self, a1);
}
```
