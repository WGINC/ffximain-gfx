# Batch 058_CMoProcessor_VT_1032C1C8: 15 functions of `CMoProcessor_VT_1032C1C8`

Name these functions: see `batches/README.md`. Write `data/names/058_CMoProcessor_VT_1032C1C8.json`.
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

## `0x1006CC50` CMoProcessor_VT_1032C1C8::vf010

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 10 (+0x28), `CMoProcessor_VT_1032C298` slot 10 (+0x28), `CMoProcessor_VT_1032C1C8` slot 10 (+0x28)

```cpp
int CMoProcessor_VT_1032C1C8::vf010(void * a1, void * a2, void * a3)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, void *, void *, void *))(*(_DWORD *)self + 44))(self, a1, a2, a3);
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

## `0x1006CC70` CMoProcessor_VT_1032C1C8::vf038

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 38 (+0x98), `CMoProcessor_VT_1032C298` slot 38 (+0x98), `CMoProcessor_VT_1032C1C8` slot 38 (+0x98)

```cpp
int CMoProcessor_VT_1032C1C8::vf038(float * a1, float * a2)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, float *, float *))(*(_DWORD *)self + 156))(self, a1, a2);
}
```

## `0x1006CC80` CMoProcessor_VT_1032C1C8::vf046

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 46 (+0xB8), `CMoProcessor_VT_1032C298` slot 46 (+0xB8), `CMoProcessor_VT_1032C1C8` slot 46 (+0xB8)

```cpp
int CMoProcessor_VT_1032C1C8::vf046(void * a1)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, void *))(*(_DWORD *)self + 192))(self, a1);
}
```

## `0x1006CC90` CMoProcessor_VT_1032C1C8::vf045

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C368` slot 45 (+0xB4), `CMoProcessor_VT_1032C298` slot 45 (+0xB4), `CMoProcessor_VT_1032C1C8` slot 45 (+0xB4)

```cpp
int CMoProcessor_VT_1032C1C8::vf045(void * a1, void * a2)
{
  void * self = reinterpret_cast<void *>(this);  // byte-exact view for the rest
  return (*(int (__thiscall **)(void *, void *, void *))(*(_DWORD *)self + 188))(self, a1, a2);
}
```

## `0x10070780` CMoProcessor_VT_1032C1C8::vf012

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 12 (+0x30)

```cpp
float * CMoProcessor_VT_1032C1C8::vf012(float *a1, float *a2, float a3)
{
  return orig::CMoProcessor_VT_1032C368_vf012(a1, a2, a3);
}
```

## `0x10070790` CMoProcessor_VT_1032C1C8::vf014

- **Gate:** none
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 14 (+0x38)

```cpp
double CMoProcessor_VT_1032C1C8::vf014(float *a1, float *a2)
{
  return a1[2] * a2[2] + a1[1] * a2[1] + *a1 * *a2;
}
```

## `0x10070AA0` CMoProcessor_VT_1032C1C8::vf018

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 18 (+0x48)

```cpp
float * CMoProcessor_VT_1032C1C8::vf018(float *a1, float *a2, float *a3)
{
  return orig::CMoProcessor_Vec4TransformB(a1, a2, a3);
}
```

## `0x10070AB0` CMoProcessor_VT_1032C1C8::vf017

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 17 (+0x44)

```cpp
float * CMoProcessor_VT_1032C1C8::vf017(float *a1, float *a2, float *a3)
{
  return orig::CMoProcessor_Vec4Transform(a1, a2, a3);
}
```

## `0x10070AC0` CMoProcessor_VT_1032C1C8::vf016

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 16 (+0x40)

```cpp
float * CMoProcessor_VT_1032C1C8::vf016(float *a1, float *a2, float *a3)
{
  return orig::CMoProcessor_Vec3Transform(a1, a2, a3);
}
```

## `0x10070AD0` CMoProcessor_VT_1032C1C8::vf021

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 21 (+0x54)

```cpp
float * CMoProcessor_VT_1032C1C8::vf021(float *a1, float *a2, float *a3, float a4)
{
  return orig::CMoProcessor_VT_1032C368_vf021(a1, a2, a3, a4);
}
```

## `0x10070E20` CMoProcessor_VT_1032C1C8::vf033

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 33 (+0x84)

```cpp
float * CMoProcessor_VT_1032C1C8::vf033(float *a1, float *a2, int a3, float *a4, float *a5, float a6, float a7)
{
  return orig::CMoProcessor_ProjectVertsAndBounds(a1, a2, a3, a4, a5, a6, a7);
}
```

## `0x10070E40` CMoProcessor_VT_1032C1C8::vf035

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 35 (+0x8C)

```cpp
CMoProcessor * CMoProcessor_VT_1032C1C8::vf035(float *a2, float *a3, int a4, int a5, int a6, float a7, float a8)
{
  return orig::CMoProcessor_CalcScreenBoundsAndVisible((float *)this, a2, a3, a4, a5, a6, a7, a8);
}
```

## `0x10070E60` CMoProcessor_VT_1032C1C8::vf036

- **Gate:** none
- **Facts:** entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoProcessor_VT_1032C1C8` slot 36 (+0x90)

```cpp
CMoProcessor * CMoProcessor_VT_1032C1C8::vf036(float *a1, float *a2, int a3, int a4, const void *a5)
{
  return orig::CMoProcessor_CalcScreenBounds(a1, a2, a3, a4, a5);
}
```
