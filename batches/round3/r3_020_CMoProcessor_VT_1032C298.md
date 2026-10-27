# Batch r3_020_CMoProcessor_VT_1032C298: 1 functions of `CMoProcessor_VT_1032C298`

Name these functions: see `batches/README.md`. Write `data/names/r3_020_CMoProcessor_VT_1032C298.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x10070020` CMoProcessor_VT_1032C298::vf035

- **Gate:** none; **frame set:** self
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 35 (+0x8C)

```cpp
CMoProcessor * CMoProcessor_VT_1032C298::vf035(float *a2, float *a3, int a4, int a5, int a6, float a7, float a8)
{
  float * self = reinterpret_cast<float *>(this);  // byte-exact view for the rest
  return orig::CMoProcessor_CalcScreenBoundsAndVisible(self, a2, a3, a4, a5, a6, a7, a8);
}
```
