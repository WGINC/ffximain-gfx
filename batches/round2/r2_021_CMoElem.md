# Batch r2_021_CMoElem: 6 functions of `CMoElem`

Name these functions: see `batches/README.md`. Write `data/names/r2_021_CMoElem.json`.
5 of them were sampled in a running frame (data/frame_set.json).

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
- **Called by:** `CYySoundElem_Constructor` (medium), `sub_1004E8D0`
- **Named before** (a naming session's reading, low): `CMoElem_SetSourceAt0FC`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `sub_1004E8D0` → `CMoElem_SetSourceAt0FC`
- **How its callers call it** (their verified code):
  - in `CYySoundElem_Constructor` (medium): `CMoElem_SetSourceAt0FC(self_,0);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYySoundElem_Constructor` (medium): Constructs a CYySoundElem and sets its default state.

```cpp
XI_API undefined4 __thiscall CMoElem_SetSourceAt0FC(void * self_, int param_1)
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

## `0x10044C20` fn_10044C20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10071E90`
- **Called by:** `sub_1004E8D0`
- **Named before** (a naming session's reading, low): `CMoElem_SetSourceAt100`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `sub_1004E8D0` → `CMoElem_SetSourceAt100`

```cpp
XI_API void __thiscall CMoElem_SetSourceAt100(void * self_, int param_1)
{
  *(uint32_t *)((int)self_ + 0x100) = *(uint32_t *)(param_1 + 0x14);
  orig::FUN_10048190((int)self_);
  return;
}
```

## `0x10045800` fn_10045800

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10045190`, `YmElem_SetupDevice` (medium)
- **Named before** (a naming session's reading, medium): `CMoElem_InvertInPlace`; name it again from this dossier, or leave it out to keep that name
- **Named callers' callers:** `CXi471011C8_VT_1032B3B8_RebuildBlendWeights`, `CMoElem_PrepareDraw`
- **How its callers call it** (their verified code):
  - in `YmElem_SetupDevice` (medium): `CMoElem_InvertInPlace(v10);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `YmElem_SetupDevice` (medium): 

```cpp
XI_API void __cdecl CMoElem_InvertInPlace(float * param_1)
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
- **Called by:** `CMoElem_PrepareDraw` (medium), `sub_10045AE0`
- **How its callers call it** (their verified code):
  - in `CMoElem_PrepareDraw` (medium): `uVar6 = orig::FUN_100458f0(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CMoElem_PrepareDraw` (medium): Computes the element's draw key and draw matrix for this frame, and tests it.

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
