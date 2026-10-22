# Batch r2_026_CMoProcessor_VT_1032C298: 2 functions of `CMoProcessor_VT_1032C298`

Name these functions: see `batches/README.md`. Write `data/names/r2_026_CMoProcessor_VT_1032C298.json`.
4 of them were sampled in a running frame (data/frame_set.json).

## `0x1006FC40` fn_1006FC40

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 27 (+0x6C)

```cpp
XI_API undefined4 __thiscall fn_1006FC40(void * self_, undefined4 param_1, float param_2, float param_3, float param_4)
{
  int iVar1;
  uint32_t a2;
  long double fVar2;
  float *a3;
  
  fVar2 = xi_castbits<long double>(fsin((long double)param_2));
  a3 = (float *)((int)self_ + 0x8d0);
  *(float *)((int)self_ + 0x868) = (float)fVar2;
  *(float *)((int)self_ + 0x874) = (float)-fVar2;
  fVar2 = xi_castbits<long double>(fcos((long double)param_2));
  *(float *)((int)self_ + 0x878) = (float)fVar2;
  *(float *)((int)self_ + 0x864) = (float)fVar2;
  fVar2 = xi_castbits<long double>(fcos((long double)param_3));
  *(float *)((int)self_ + 0x8b8) = (float)fVar2;
  *(float *)((int)self_ + 0x890) = (float)fVar2;
  fVar2 = xi_castbits<long double>(fsin((long double)param_3));
  *(float *)((int)self_ + 0x8b0) = (float)fVar2;
  *(float *)((int)self_ + 0x898) = (float)-fVar2;
  fVar2 = xi_castbits<long double>(fcos((long double)param_4));
  *(float *)((int)self_ + 0x8e4) = (float)fVar2;
  *a3 = (float)fVar2;
  fVar2 = xi_castbits<long double>(fsin((long double)param_4));
  *(float *)((int)self_ + 0x8d4) = (float)fVar2;
  *(float *)((int)self_ + 0x8e0) = (float)-fVar2;
  iVar1 = *(int *)self_;
  a2 = ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar1 + 0x24))))((void *)(self_),param_1,(int)self_ + 0x850,(float *)((int)self_ + 0x890));
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar1 + 0x24))))((void *)(self_),param_1,a2,a3);
  return param_1;
}
```

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
