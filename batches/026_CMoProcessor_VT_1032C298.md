# Batch 026_CMoProcessor_VT_1032C298: 8 functions of `CMoProcessor_VT_1032C298`

Name these functions: see `batches/README.md`. Write `data/names/026_CMoProcessor_VT_1032C298.json`.
5 of them were sampled in a running frame (data/frame_set.json).

## `0x1006D380` CMoProcessor_VT_1032C298::vf007

- **Gate:** none; **frame set:** self
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 7 (+0x1C), `CMoProcessor_VT_1032C298` slot 7 (+0x1C)

```cpp
float * CMoProcessor_VT_1032C298::vf007(float *a1, float a2)
{
  *a1 = a2 * *a1;
  a1[1] = a2 * a1[1];
  a1[2] = a2 * a1[2];
  return a1;
}
```

## `0x1006F5B0` CMoProcessor_VT_1032C298::vf014

- **Gate:** none
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 14 (+0x38)

```cpp
double CMoProcessor_VT_1032C298::vf014(float *a1, float *a2)
{
  return a1[2] * a2[2] + a1[1] * a2[1] + *a1 * *a2;
}
```

## `0x1006F800` fn_1006F800

- **Gate:** none; **frame set:** self
- **Facts:** `ret 12`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 20 (+0x50)

```cpp
XI_API float * __stdcall fn_1006F800(float * param_1, float * param_2, float * param_3)
{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  fVar4 = param_2[1];
  fVar5 = param_2[2];
  fVar6 = param_2[5];
  fVar7 = param_2[6];
  fVar8 = param_2[9];
  fVar9 = param_2[10];
  fVar10 = param_2[0xd];
  fVar11 = param_2[0xe];
  fVar1 = *param_3;
  fVar2 = param_3[1];
  fVar3 = param_3[2];
  *param_1 = fVar1 * *param_2 + fVar2 * param_2[4] + fVar3 * param_2[8] + param_2[0xc];
  param_1[1] = fVar1 * fVar4 + fVar2 * fVar6 + fVar3 * fVar8 + fVar10;
  param_1[2] = fVar1 * fVar5 + fVar2 * fVar7 + fVar3 * fVar9 + fVar11;
  return param_1;
}
```

## `0x1006FA30` CMoProcessor_VT_1032C298::vf021

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 21 (+0x54)

```cpp
float * CMoProcessor_VT_1032C298::vf021(float *a1, float *a2, float *a3, float a4)
{
  return orig::CMoProcessor_VT_1032C368_vf021(a1, a2, a3, a4);
}
```

## `0x1006FAE0` fn_1006FAE0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 22 (+0x58)

```cpp
XI_API undefined4 __thiscall fn_1006FAE0(void * self_, undefined4 param_1, float param_2, float param_3, float param_4)
{
  int iVar1;
  uint32_t a2;
  long double fVar2;
  int a3;
  
  fVar2 = xi_castbits<long double>(fsin((long double)param_2));
  a3 = (int)self_ + 0x850;
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
  *(float *)((int)self_ + 0x8d0) = (float)fVar2;
  fVar2 = xi_castbits<long double>(fsin((long double)param_4));
  *(float *)((int)self_ + 0x8d4) = (float)fVar2;
  *(float *)((int)self_ + 0x8e0) = (float)-fVar2;
  iVar1 = *(int *)self_;
  a2 = ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar1 + 0x24))
                 ))((void *)(self_),param_1,(float *)((int)self_ + 0x8d0),(float *)((int)self_ + 0x890));
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar1 + 0x24))))((void *)(self_),param_1,a2,a3);
  return param_1;
}
```

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

## `0x10070040` CMoProcessor_VT_1032C298::vf036

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C298` slot 36 (+0x90)

```cpp
CMoProcessor * CMoProcessor_VT_1032C298::vf036(float *a1, float *a2, int a3, int a4, const void *a5)
{
  return orig::CMoProcessor_CalcScreenBounds(a1, a2, a3, a4, a5);
}
```
