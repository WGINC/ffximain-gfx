# Batch 013_CMoElem: 12 functions of `CMoElem`

Name these functions: see `batches/README.md`. Write `data/names/013_CMoElem.json`.
7 of them were sampled in a running frame (data/frame_set.json).

## `0x10036A80` CMoElem::vf013

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 13 (+0x34), `CMoD3aLightmapElem` slot 13 (+0x34), `CXi471011C8_VT_1032AD80` slot 13 (+0x34), `CMoPointLightProgElem` slot 13 (+0x34), `CXi471011C8_VT_1032B3B8` slot 13 (+0x34), `CMoD3aElem` slot 13 (+0x34), `CMoSkeletonElem` slot 13 (+0x34), `CMoNullProgElem` slot 13 (+0x34) and 9 more

```cpp
void CMoElem::vf013(int a1)
{
  ;
}
```

## `0x10036A90` CMoElem::vf014

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 14 (+0x38), `CXi471011C8_VT_1032AD80` slot 14 (+0x38), `CMoPointLightProgElem` slot 14 (+0x38), `CMoD3aElem` slot 14 (+0x38), `CMoSkeletonElem` slot 14 (+0x38), `CMoNullProgElem` slot 14 (+0x38), `CMoDistMorphElem` slot 14 (+0x38), `CMoDist` slot 14 (+0x38) and 12 more

```cpp
void CMoElem::vf014(int a1)
{
  ;
}
```

## `0x10036AA0` CMoElem::vf015

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 15 (+0x3C), `CXi471011C8_VT_1032AD80` slot 15 (+0x3C), `CMoPointLightProgElem` slot 15 (+0x3C), `CMoD3aElem` slot 15 (+0x3C), `CMoSkeletonElem` slot 15 (+0x3C), `CMoNullProgElem` slot 15 (+0x3C), `CMoDistMorphElem` slot 15 (+0x3C), `CMoDist` slot 15 (+0x3C) and 12 more

```cpp
int CMoElem::vf015()
{
  return 0;
}
```

## `0x10036AB0` CMoElem::vf019

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3aLightmapElem` slot 19 (+0x4C), `CXi471011C8_VT_1032AD80` slot 19 (+0x4C), `CMoPointLightProgElem` slot 19 (+0x4C), `CXi471011C8_VT_1032B3B8` slot 19 (+0x4C), `CMoD3aElem` slot 19 (+0x4C), `CMoSkeletonElem` slot 19 (+0x4C), `CMoNullProgElem` slot 19 (+0x4C), `CMoDistMorphElem` slot 19 (+0x4C) and 13 more

```cpp
void CMoElem::vf019()
{
  ;
}
```

## `0x10044AA0` fn_10044AA0

- **Gate:** none; **frame set:** static, self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10071E90`
- **Called by:** `fn_100369F0`, `sub_1004E8D0`

```cpp
XI_API undefined4 __thiscall fn_10044AA0(void * self_, int param_1)
{
  uint32_t uVar1;
  
  if (param_1 != 0) {
    *(uint32_t *)((int)self_ + 0xfc) = *(uint32_t *)(param_1 + 0x14);
    uVar1 = orig::FUN_10048100((int)self_);
    return uVar1;
  }
  return self_;
}
```

## `0x10044AC0` fn_10044AC0

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 16 (+0x40), `CMoD3aLightmapElem` slot 16 (+0x40), `CXi471011C8_VT_1032AD80` slot 16 (+0x40), `CMoPointLightProgElem` slot 16 (+0x40), `CXi471011C8_VT_1032B3B8` slot 16 (+0x40), `CMoD3aElem` slot 16 (+0x40), `CMoSkeletonElem` slot 16 (+0x40), `CMoNullProgElem` slot 16 (+0x40) and 13 more

```cpp
XI_API unsigned char __thiscall fn_10044AC0(void * self_)
{
  void *this_00;
  uint32_t uVar1;
  
  if (*(uint32_t **)((int)self_ + 0xfc) == (uint32_t *)0x0) {
    this_00 = (void *)0x0;
  }
  else {
    this_00 = (void *)**(uint32_t **)((int)self_ + 0xfc);
  }
  if (this_00 != (void *)0x0) {
    uVar1 = orig::FUN_100539e0(this_00);
    return SUB41(uVar1,0);
  }
  return false;
}
```

## `0x10044AE0` fn_10044AE0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_100473F0`, `sub_1004A980`, `fn_1004E720`, `__attribute__`
- **Called by:** `fn_100368B0`, `fn_10045A40`, `sub_10045AE0`, `sub_10045BB0`
- **Reads globals:** `0x1047BFA8`

```cpp
XI_API undefined4 __thiscall fn_10044AE0(void * self_)
{
  uint32_t *puVar1;
  uint32_t uVar2;
  uint32_t uVar3;
  uint32_t uVar4;
  int xi_eax;
  void *this_00;
  uint8_t bVar5;
  uint8_t bVar6;
  long double fVar7;
  long double fVar8;
  
  puVar1 = *(uint32_t **)((int)self_ + 0xfc);
  if (puVar1 == (uint32_t *)0x0) {
    this_00 = (void *)0x0;
  }
  else {
    this_00 = (void *)*puVar1;
  }
  fVar7 = (long double)*(float *)((int)self_ + 0x114);
  fVar8 = (long double)0.0f;
  bVar5 = NAN(fVar7) || NAN(fVar8);
  bVar6 = bVar5 || fVar7 == fVar8;
  *(uint8_t *)((int)self_ + 0x178) = 1;
  uVar4 = ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)puVar1 >> 0x10)) << 16) | (uint16_t)((uint16_t)(bVar5 || fVar7 < fVar8) << 8 | (uint16_t)bVar5 << 10 |
                   (uint16_t)bVar6 << 0xe)));
  if (bVar5 == bVar6) {
    fVar8 = (long double)*(float *)((int)self_ + 0x110) - (long double)*(float *)(g_pResourceMng + 0xeb0);
    *(float *)((int)self_ + 0x110) = (float)fVar8;
    bVar5 = NAN(fVar8) || NAN((long double)1.0f);
    if ((bVar5 || fVar8 < (long double)1.0f) != bVar5) {
      if (this_00 == (void *)0x0) {
LAB_10044b8c:
        uVar3 = ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x18))))((void *)(self_),1);
        return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar3 >> 8)) << 8) | (uint8_t)(1)));
      }
      orig::FUN_1004e720(this_00,(int)self_);
      bVar5 = NAN((long double)*(float *)((int)self_ + 0x110)) || NAN((long double)1.0f);
      if ((bVar5 || (long double)*(float *)((int)self_ + 0x110) < (long double)1.0f) != bVar5) {
        *(uint8_t *)((int)self_ + 0x178) = 0;
        if ((*(uint32_t *)((int)self_ + 0x10c) >> 0x1d & 1) == 0) {
          uVar3 = ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x18))))((void *)(self_),1);
          return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar3 >> 8)) << 8) | (uint8_t)(1)));
        }
        *(uint32_t *)((int)self_ + 0x110) = 0;
        if ((*(int **)((int)self_ + 0x164) != (int *)0x0) &&
           ((*(uint16_t *)(**(int **)((int)self_ + 0x164) + 0x18) & 0x7fff) < 2)) goto LAB_10044b8c;
        *(uint8_t *)((int)self_ + 0x178) = 1;
      }
    }
    uVar4 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x40))))((void *)(self_));
    if ((char)uVar4 != '\0') {
      uVar2 = *(uint32_t *)((int)this_00 + 0xd8);
      goto LAB_10044bda;
    }
  }
  else {
    if (this_00 == (void *)0x0) goto LAB_10044bfd;
    uVar2 = *(uint32_t *)((int)this_00 + 0xd8);
LAB_10044bda:
    if ((uVar2 >> 9 & 1) != 0) {
      xi_eax = orig::FUN_100473f0(self_);
      uVar4 = orig::FUN_10053dd0(this_00,xi_eax,xi_eax);
    }
  }
  if (this_00 != (void *)0x0) {
    uVar4 = orig::FUN_1004a980(this_00,self_);
    return uVar4 & 0xffffff00;
  }
LAB_10044bfd:
  *(uint32_t *)((int)self_ + 0x54) = 0;
  *(uint32_t *)((int)self_ + 0x58) = 0;
  *(uint32_t *)((int)self_ + 0x5c) = 0;
  *(uint32_t *)((int)self_ + 0x188) = 0;
  *(uint32_t *)((int)self_ + 0x18c) = 0;
  *(uint32_t *)((int)self_ + 400) = 0;
  return uVar4 & 0xffffff00;
}
```

## `0x10044C20` fn_10044C20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10071E90`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API void __thiscall fn_10044C20(void * self_, int param_1)
{
  *(uint32_t *)((int)self_ + 0x100) = *(uint32_t *)(param_1 + 0x14);
  orig::FUN_10048190((int)self_);
  return;
}
```

## `0x10045800` fn_10045800

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10045190`, `YmElem_SetupDevice`

```cpp
XI_API void __cdecl fn_10045800(float * param_1)
{
  float fVar1;
  int iVar2;
  int iVar3;
  float *pfVar4;
  float *pfVar5;
  float *pfVar6;
  float *pfVar7;
  float *pfVar8;
  float *pfVar9;
  uint8_t bVar10;
  long double fVar11;
  long double fVar12;
  int local_10;
  float *local_c;
  float *local_8;
  
  local_10 = 0;
  local_8 = param_1;
  local_c = param_1;
  pfVar8 = param_1;
  do {
    fVar11 = (long double)*local_c;
    bVar10 = NAN(fVar11) || NAN((long double)0.0f);
    if (bVar10 != (bVar10 || fVar11 == (long double)0.0f)) {
      fVar11 = (long double)1.0f;
    }
    fVar11 = (long double)1.0f / fVar11;
    bVar10 = NAN(fVar11) || NAN((long double)0.0f);
    if (bVar10 != (bVar10 || fVar11 == (long double)0.0f)) {
      fVar11 = (long double)1.0f;
    }
    iVar2 = 4;
    pfVar5 = pfVar8;
    do {
      pfVar6 = pfVar5 + 1;
      iVar2 = iVar2 + -1;
      *pfVar5 = (float)(fVar11 * (long double)*pfVar5);
      pfVar5 = pfVar6;
    } while (iVar2 != 0);
    *local_c = (float)fVar11;
    iVar2 = 0;
    pfVar5 = local_8;
    pfVar9 = param_1;
    do {
      if (iVar2 != local_10) {
        fVar1 = *pfVar5;
        iVar3 = 0;
        pfVar4 = pfVar9;
        pfVar7 = pfVar8;
        do {
          if (iVar3 == local_10) {
            fVar12 = -((long double)fVar1 * fVar11);
          }
          else {
            fVar12 = (long double)*pfVar4 - (long double)fVar1 * (long double)*pfVar7;
          }
          *pfVar4 = (float)fVar12;
          iVar3 = iVar3 + 1;
          pfVar7 = pfVar7 + 1;
          pfVar4 = pfVar4 + 1;
        } while (iVar3 < 4);
      }
      iVar2 = iVar2 + 1;
      pfVar5 = pfVar5 + 4;
      pfVar9 = pfVar9 + 4;
    } while (iVar2 < 4);
    local_10 = local_10 + 1;
    local_c = local_c + 5;
    local_8 = local_8 + 1;
    pfVar8 = pfVar6;
  } while (local_10 < 4);
  return;
}
```

## `0x100458F0` fn_100458F0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100555B0`
- **Called by:** `fn_10045A40`, `sub_10045AE0`

```cpp
XI_API undefined4 __thiscall fn_100458F0(void * self_)
{
  uint32_t uVar1;
  uint32_t uVar2;
  
  uVar1 = (uint32_t)(!((long double)*(float *)((int)self_ + 0x138) >= (long double)0.00999999977648258209228515625f));
  if (uVar1 == 0) {
    uVar2 = orig::FUN_100555b0(self_,(float)(long double)*(float *)((int)self_ + 300));
    return uVar2;
  }
  *(uint32_t *)((int)self_ + 0x138) = 0x3f800000;
  return uVar1 << 8;
}
```

## `0x10045A40` fn_10045A40

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10044AE0`, `fn_10044D40`, `YmElem_ComputeViewDepthKey`, `sub_10045190`, `fn_100458F0`, `YmElem_BuildDrawMatrixAndTest`
- **In vtables** (class_vtables.json): `CMoD3mSpecularElem` slot 8 (+0x20), `CMoD3aLightmapElem` slot 8 (+0x20), `CXi471011C8_VT_1032AD80` slot 8 (+0x20), `CMoPointLightProgElem` slot 8 (+0x20), `CXi471011C8_VT_1032B3B8` slot 8 (+0x20), `CMoD3aElem` slot 8 (+0x20), `CMoNullProgElem` slot 8 (+0x20), `CMoDistMorphElem` slot 8 (+0x20) and 9 more
- **Called by:** `sub_10047830`

```cpp
XI_API undefined4 __thiscall fn_10045A40(void * self_)
{
  float fVar1;
  float fVar2;
  float fVar3;
  long double fVar4;
  uint32_t uVar5;
  uint32_t uVar6;
  uint8_t bVar7;
  uint8_t bVar8;
  long double fVar9;
  
  fVar1 = *(float *)((int)self_ + 0x54);
  fVar2 = *(float *)((int)self_ + 0x58);
  fVar3 = *(float *)((int)self_ + 0x5c);
  uVar5 = orig::FUN_10044ae0(self_);
  if ((char)uVar5 != '\0') {
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar5 >> 8)) << 8) | (uint8_t)(1)));
  }
  uVar5 = orig::YmElem_ComputeViewDepthKey(self_);
  fVar9 = (long double)*(float *)((int)self_ + 0x138);
  fVar4 = (long double)0.0f;
  bVar7 = NAN(fVar9) || NAN(fVar4);
  bVar8 = bVar7 || fVar9 == fVar4;
  if (bVar7 != bVar8) {
    *(uint32_t *)((int)self_ + 0x138) = 0x3f800000;
    return ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)uVar5 >> 0x10)) << 16) | (uint16_t)((uint16_t)(bVar7 || fVar9 < fVar4) << 8 | (uint16_t)bVar7 << 10 |
                    (uint16_t)bVar8 << 0xe)));
  }
  orig::FUN_10044d40(self_);
  uVar5 = orig::FUN_10045190(self_,fVar1,fVar2,fVar3);
  uVar6 = orig::YmElem_BuildDrawMatrixAndTest(self_,uVar5);
  if ((char)uVar6 != '\0') {
    uVar6 = orig::FUN_100458f0(self_);
  }
  return uVar6 & 0xffffff00;
}
```

## `0x100472E0` CMoElem::vf018

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoSkeletonElem` slot 18 (+0x48), `CMoNullProgElem` slot 18 (+0x48), `CYySoundElem` slot 18 (+0x48), `CMoLfdElem` slot 18 (+0x48), `CMoElem` slot 18 (+0x48)

```cpp
void CMoElem::vf018()
{
  ;
}
```
