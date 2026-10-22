# Batch r2_051_CMoProcessor_VT_1032C368: 2 functions of `CMoProcessor_VT_1032C368`

Name these functions: see `batches/README.md`. Write `data/names/r2_051_CMoProcessor_VT_1032C368.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1006D3E0` fn_1006D3E0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 48 (+0xC0), `CMoProcessor_VT_1032C1C8` slot 48 (+0xC0)
- **Called by:** `Mat4x4_SafeTranspose`
- **Named before** (a naming session's reading, medium): `CMoProcessor_TransposeInPlace`; name it again from this dossier, or leave it out to keep that name
- **How its callers call it** (their verified code):
  - in `Mat4x4_SafeTranspose`: `CMoProcessor_TransposeInPlace((int)param_1);`

```cpp
XI_API void __stdcall CMoProcessor_TransposeInPlace(int param_1)
{
  float fVar1;
  float fVar2;
  uint32_t uVar3;
  uint32_t uVar4;
  uint32_t uVar5;
  uint32_t uVar6;
  
  uVar3 = *(uint32_t *)(param_1 + 0xc);
  uVar4 = *(uint32_t *)(param_1 + 0x18);
  fVar1 = *(float *)(param_1 + 4);
  fVar2 = *(float *)(param_1 + 8);
  uVar5 = *(uint32_t *)(param_1 + 0x1c);
  uVar6 = *(uint32_t *)(param_1 + 0x2c);
  *(uint32_t *)(param_1 + 4) = *(uint32_t *)(param_1 + 0x10);
  *(uint32_t *)(param_1 + 8) = *(uint32_t *)(param_1 + 0x20);
  *(float *)(param_1 + 0x10) = (float)(long double)fVar1;
  *(uint32_t *)(param_1 + 0xc) = *(uint32_t *)(param_1 + 0x30);
  *(uint32_t *)(param_1 + 0x18) = *(uint32_t *)(param_1 + 0x24);
  *(float *)(param_1 + 0x20) = (float)(long double)fVar2;
  *(uint32_t *)(param_1 + 0x1c) = *(uint32_t *)(param_1 + 0x34);
  *(uint32_t *)(param_1 + 0x2c) = *(uint32_t *)(param_1 + 0x38);
  *(uint32_t *)(param_1 + 0x24) = uVar4;
  *(uint32_t *)(param_1 + 0x30) = uVar3;
  *(uint32_t *)(param_1 + 0x34) = uVar5;
  *(uint32_t *)(param_1 + 0x38) = uVar6;
  return;
}
```

## `0x1006D580` fn_1006D580

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 37 (+0x94)
- **Named before** (a naming session's reading, medium): `CMoProcessor_LengthSafe`; name it again from this dossier, or leave it out to keep that name

```cpp
XI_API long double __stdcall CMoProcessor_LengthSafe(float * param_1)
{
  uint8_t bVar1;
  long double fVar2;
  
  fVar2 = (long double)param_1[2] * (long double)param_1[2] +
          (long double)param_1[1] * (long double)param_1[1] + (long double)*param_1 * (long double)*param_1;
  bVar1 = NAN(fVar2) || NAN((long double)0.0);
  if (bVar1 != (bVar1 || fVar2 == (long double)0.0)) {
    return (long double)0.0f;
  }
  return SQRT(fVar2);
}
```
