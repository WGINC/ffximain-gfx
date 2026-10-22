# Batch 205_YkJpeg: 11 functions of `YkJpeg`

Name these functions: see `batches/README.md`. Write `data/names/205_YkJpeg.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101CF870` fn_101CF870

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101CF820` x4, `fn_101D09C0` x4, `fn_101D09E0` x8, `fn_101D0A20` x4, `fn_101D0A40` x4
- **Called by:** `fn_101D0370`, `sub_101D0640`

```cpp
XI_API void __thiscall fn_101CF870(void * self_)
{
  orig::FUN_101cf820((int)self_ + 0x8bc,(int)self_ + 0x69c,0x1037e6c4,0x1037e6b0);
  orig::FUN_101cf820((int)self_ + 0x8dc,(int)self_ + 0x6ac,0x1037e708,0x1037e6f0);
  orig::FUN_101cf820((int)self_ + 0xadc,(int)self_ + 0x7ac,0x1037e6e4,0x1037e6d0);
  orig::FUN_101cf820((int)self_ + 0xafc,(int)self_ + 0x7bc,0x1037e7c0,0x1037e7ac);
  orig::FUN_101d0a40(*(void **)((int)self_ + 8),0xc4);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),0x1f);
  orig::FUN_101d09c0(*(void **)((int)self_ + 8),0);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e6b1,0x10);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e6c4,0xc);
  orig::FUN_101d0a40(*(void **)((int)self_ + 8),0xc4);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),0xb5);
  orig::FUN_101d09c0(*(void **)((int)self_ + 8),0x10);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e6f1,0x10);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e708,0xa2);
  orig::FUN_101d0a40(*(void **)((int)self_ + 8),0xc4);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),0x1f);
  orig::FUN_101d09c0(*(void **)((int)self_ + 8),1);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e6d1,0x10);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e6e4,0xc);
  orig::FUN_101d0a40(*(void **)((int)self_ + 8),0xc4);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),0xb5);
  orig::FUN_101d09c0(*(void **)((int)self_ + 8),0x11);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e7ad,0x10);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e7c0,0xa2);
  return;
}
```

## `0x101CFDB0` fn_101CFDB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 24`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_101D0370`, `sub_101D0780`

```cpp
XI_API void __stdcall fn_101CFDB0(int param_1, int param_2, int param_3, int param_4, int param_5, int param_6)
{
  uint32_t uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = 0;
  iVar4 = 0;
  iVar3 = param_1 * param_5;
  iVar2 = param_4;
  do {
    if ((((-1 < iVar2) && (-1 < param_5)) && (iVar2 < param_1)) && (param_5 < param_2)) {
      uVar1 = *(uint32_t *)(param_3 + (iVar3 + iVar2) * 4);
    }
    iVar2 = iVar2 + 1;
    *(uint32_t *)(param_6 + iVar4 * 4) = uVar1;
    if (param_4 + 0x10 <= iVar2) {
      param_5 = param_5 + 1;
      iVar3 = iVar3 + param_1;
      iVar2 = param_4;
    }
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x100);
  return;
}
```

## `0x101CFE20` fn_101CFE20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_101CFEB0`

```cpp
XI_API int __stdcall fn_101CFE20(uint32_t param_1, int * param_2, int * param_3, int param_4)
{
  int iVar1;
  uint32_t uVar2;
  
  uVar2 = param_1 >> 0x10;
  if (param_2 != (int *)0x0) {
    param_4 = (((uVar2 & 0xf800) << 3 | uVar2 & 0x7e0) << 2 | uVar2 & 0x1f) * 8;
    *param_2 = param_4;
  }
  if (param_3 != (int *)0x0) {
    iVar1 = (((param_1 & 0xf800) << 3 | param_1 & 0x7e0) << 2 | param_1 & 0x1f) * 8;
    *param_3 = iVar1;
    return iVar1;
  }
  return param_4;
}
```

## `0x101CFEB0` fn_101CFEB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 24`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101CFE20` x2
- **Called by:** `fn_101D0370`

```cpp
XI_API void __thiscall fn_101CFEB0(void * self_, int param_1, int param_2, int param_3, int param_4, int param_5, int * param_6)
{
  uint32_t uVar1;
  int iVar2;
  uint32_t uVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = 0;
  uVar3 = 0;
  iVar4 = param_1 * param_5;
  iVar5 = param_4;
  do {
    if ((((iVar5 < 0) || (param_5 < 0)) || (param_1 <= iVar5)) || (param_2 <= param_5)) {
      *param_6 = iVar2;
    }
    else {
      uVar1 = *(uint32_t *)(param_3 + ((iVar4 + iVar5) / 2) * 4);
      if ((uVar3 & 1) == 0) {
        iVar2 = orig::FUN_101cfe20(uVar1,param_6,(int *)0x0,iVar2);
      }
      else {
        iVar2 = orig::FUN_101cfe20(uVar1,(int *)0x0,param_6,iVar2);
      }
    }
    iVar5 = iVar5 + 1;
    if (param_4 + 0x10 <= iVar5) {
      param_5 = param_5 + 1;
      iVar4 = iVar4 + param_1;
      iVar5 = param_4;
    }
    uVar3 = uVar3 + 1;
    param_6 = param_6 + 1;
  } while ((int)uVar3 < 0x100);
  return;
}
```

## `0x101CFF60` fn_101CFF60

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_101D0370`, `sub_101D0780`

```cpp
XI_API void __stdcall fn_101CFF60(int param_1)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int local_4;
  
  iVar8 = param_1;
  piVar1 = (int *)(param_1 + 8);
  local_4 = 8;
  do {
    iVar7 = piVar1[-2] + piVar1[5];
    iVar5 = piVar1[-2] - piVar1[5];
    iVar4 = piVar1[-1] + piVar1[4];
    iVar2 = piVar1[-1] - piVar1[4];
    iVar9 = piVar1[3] + *piVar1;
    iVar3 = *piVar1 - piVar1[3];
    iVar10 = piVar1[2] + piVar1[1];
    iVar11 = piVar1[2];
    iVar6 = iVar10 + iVar7;
    iVar7 = iVar7 - iVar10;
    piVar1[-2] = iVar9 + iVar4 + iVar6;
    piVar1[2] = iVar6 - (iVar9 + iVar4);
    iVar4 = ((iVar7 - iVar9) + iVar4) * 0xb5 >> 8;
    piVar1[4] = iVar7 - iVar4;
    *piVar1 = iVar4 + iVar7;
    iVar4 = (piVar1[1] - iVar11) + iVar3;
    iVar7 = iVar5 + iVar2;
    iVar6 = (iVar4 - iVar7) * 0x62 >> 8;
    iVar4 = (iVar4 * 0x8b >> 8) + iVar6;
    iVar6 = (iVar7 * 0x14e >> 8) + iVar6;
    iVar11 = (iVar3 + iVar2) * 0xb5 >> 8;
    iVar7 = iVar11 + iVar5;
    iVar5 = iVar5 - iVar11;
    piVar1[1] = iVar5 - iVar4;
    piVar1[3] = iVar5 + iVar4;
    piVar1[-1] = iVar7 + iVar6;
    piVar1[5] = iVar7 - iVar6;
    piVar1 = piVar1 + 8;
    local_4 = local_4 + -1;
  } while (local_4 != 0);
  param_1 = 8;
  piVar1 = (int *)(iVar8 + 0x40);
  do {
    iVar8 = piVar1[-0x10] + piVar1[0x28];
    iVar3 = piVar1[-0x10] - piVar1[0x28];
    iVar7 = piVar1[-8] + piVar1[0x20];
    iVar11 = piVar1[-8] - piVar1[0x20];
    iVar5 = piVar1[0x18] + *piVar1;
    iVar2 = *piVar1 - piVar1[0x18];
    iVar9 = piVar1[0x10] + piVar1[8];
    iVar6 = piVar1[0x10];
    iVar4 = iVar9 + iVar8;
    iVar8 = iVar8 - iVar9;
    piVar1[-0x10] = iVar5 + iVar7 + iVar4;
    piVar1[0x10] = iVar4 - (iVar5 + iVar7);
    iVar7 = ((iVar8 - iVar5) + iVar7) * 0xb5 >> 8;
    piVar1[0x20] = iVar8 - iVar7;
    *piVar1 = iVar7 + iVar8;
    iVar7 = (piVar1[8] - iVar6) + iVar2;
    iVar8 = iVar3 + iVar11;
    iVar4 = (iVar7 - iVar8) * 0x62 >> 8;
    iVar7 = (iVar7 * 0x8b >> 8) + iVar4;
    iVar4 = (iVar8 * 0x14e >> 8) + iVar4;
    iVar6 = (iVar2 + iVar11) * 0xb5 >> 8;
    iVar8 = iVar6 + iVar3;
    iVar3 = iVar3 - iVar6;
    piVar1[8] = iVar3 - iVar7;
    piVar1[0x18] = iVar3 + iVar7;
    piVar1[-8] = iVar8 + iVar4;
    piVar1[0x28] = iVar8 - iVar4;
    piVar1 = piVar1 + 1;
    param_1 = param_1 + -1;
  } while (param_1 != 0);
  return;
}
```

## `0x101D0210` fn_101D0210

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101D0970` x6
- **Called by:** `fn_101D0370`, `sub_101D0780`

```cpp
XI_API void __thiscall fn_101D0210(void * self_, int * param_1, char * param_2)
{
  uint16_t *puVar1;
  uint32_t uVar2;
  int iVar3;
  uint16_t uVar4;
  int iVar5;
  uint32_t uVar6;
  uint32_t uVar7;
  uint32_t uVar8;
  int *local_4;
  
  if (param_2 == (char *)0x0) {
    param_2 = (char *)((int)self_ + 0x6ac);
    iVar3 = (int)self_ + 0x69c;
    iVar5 = (int)self_ + 0x8bc;
    puVar1 = (uint16_t *)((int)self_ + 0x8dc);
  }
  else {
    param_2 = (char *)((int)self_ + 0x7bc);
    iVar3 = (int)self_ + 0x7ac;
    iVar5 = (int)self_ + 0xadc;
    puVar1 = (uint16_t *)((int)self_ + 0xafc);
  }
  uVar6 = *param_1;
  uVar2 = uVar6;
  if ((int)uVar6 < 0) {
    uVar2 = -uVar6;
    uVar6 = uVar6 - 1;
  }
  uVar7 = 0;
  for (; uVar2 != 0; uVar2 = (int)uVar2 >> 1) {
    uVar7 = uVar7 + 1;
  }
  orig::FUN_101d0970(*(void **)((int)self_ + 8),
               ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)iVar5 >> 0x10)) << 16) | (uint16_t)(*(uint16_t *)(iVar5 + uVar7 * 2)))),
               (int)*(char *)(uVar7 + iVar3));
  if (uVar7 != 0) {
    orig::FUN_101d0970(*(void **)((int)self_ + 8),uVar6,uVar7);
  }
  uVar6 = 0;
  local_4 =(int*)(&DAT_1037e334);
  do {
    uVar2 = param_1[*local_4];
    uVar4 = (uint16_t)((uint32_t)param_2 >> 0x10);
    if (uVar2 == 0) {
      uVar6 = uVar6 + 1;
    }
    else {
      if (0xf < (int)uVar6) {
        uVar7 = uVar6 >> 4;
        uVar6 = uVar6 + uVar7 * -0x10;
        do {
          orig::FUN_101d0970(*(void **)((int)self_ + 8),((uint32_t)(((uint32_t)(uint16_t)(uVar4) << 16) | (uint16_t)(puVar1[0xf0]))),(int)param_2[0xf0]);
          uVar7 = uVar7 - 1;
        } while (uVar7 != 0);
      }
      uVar7 = uVar2;
      if ((int)uVar2 < 0) {
        uVar7 = -uVar2;
        uVar2 = uVar2 - 1;
      }
      uVar8 = 1;
      while (uVar7 = (int)uVar7 >> 1, uVar7 != 0) {
        uVar8 = uVar8 + 1;
      }
      iVar3 = uVar6 * 0x10 + uVar8;
      orig::FUN_101d0970(*(void **)((int)self_ + 8),((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)iVar3 >> 0x10)) << 16) | (uint16_t)(puVar1[iVar3]))),
                   (int)param_2[iVar3]);
      orig::FUN_101d0970(*(void **)((int)self_ + 8),uVar2,uVar8);
      uVar6 = 0;
    }
    local_4 = local_4 + 1;
  } while ((int)local_4 < 0x1037e430);
  if (0 < (int)uVar6) {
    orig::FUN_101d0970(*(void **)((int)self_ + 8),((uint32_t)(((uint32_t)(uint16_t)(uVar4) << 16) | (uint16_t)(*puVar1))),(int)*param_2);
  }
  return;
}
```

## `0x101D0370` fn_101D0370

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Mem_AllocDefault`, `fn_101CF660`, `fn_101CF870`, `sub_101CFA00`, `fn_101CFDB0`, `fn_101CFEB0`, `fn_101CFF60` x3, `sub_101D01B0` x3, `fn_101D0210` x3, `fn_101D05E0` x2, `fn_101D0860`, `fn_101D0880`, `fn_101D08E0`, `fn_101D09E0` x5, `fn_101D0A20` x3, `fn_101D0A40`, `fn_101D0A60`, `Allocator_Generic`
- **Called by:** `fn_101D0600`

```cpp
XI_API undefined4 __thiscall fn_101D0370(void * self_, uint32_t param_1, uint32_t param_2, int param_3, int param_4)
{
  char cVar1;
  void *pvVar2;
  char *pcVar3;
  uint8_t *puVar4;
  uint32_t uVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  uint8_t bVar9;
  uint64_t uVar10;
  char xi_frame[1028] __attribute__((aligned(4)));
  int  &local_404 = *reinterpret_cast<int  *>(xi_frame + 0);
  int  (&local_400)[256] = *reinterpret_cast<int  (*)[256]>(xi_frame + 4);
  
  if (*(int *)((int)self_ + 4) != 0) {
    orig::FUN_101d0880(self_);
  }
  orig::FUN_101d0860(self_);
  bVar9 = param_4 < 0;
  if (bVar9) {
    param_4 = -param_4;
  }
  uVar10 = orig::Allocator_Generic_v1(0xa00000);
  *(int *)((int)self_ + 4) = (int)uVar10;
  if ((int)uVar10 == 0) {
    return 1;
  }
  pvVar2 = (void *)orig::Mem_AllocDefault_v1(0x14);
  if (pvVar2 == (void *)0x0) {
    pvVar2 = (void *)0x0;
  }
  else {
    pvVar2 = (void *)orig::FUN_101d08e0(pvVar2,*(uint32_t *)((int)self_ + 4),0xa00000);
  }
  *(void **)((int)self_ + 8) = pvVar2;
  if (pvVar2 == (void *)0x0) {
    return 1;
  }
  orig::FUN_101d09e0(pvVar2,0x1037e2f4,0x19);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),param_2 & 0xffff);
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),param_1 & 0xffff);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e310,10);
  orig::FUN_101d0a40(*(void **)((int)self_ + 8),0xfe);
  pcVar3 = orig::FUN_101d05e0();
  uVar5 = 0xffffffff;
  do {
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + 1;
  } while (cVar1 != '\0');
  uVar5 = ~uVar5;
  orig::FUN_101d0a20(*(void **)((int)self_ + 8),uVar5 + 2);
  puVar4 = orig::FUN_101d05e0();
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),(int)puVar4,uVar5);
  orig::FUN_101cf660(self_,param_4);
  orig::FUN_101cf870(self_);
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e31c,0xe);
  local_404 = 0;
  if (0 < (int)param_2) {
    do {
      iVar7 = 0;
      if (0 < (int)param_1) {
        do {
          if (bVar9) {
            orig::FUN_101cfeb0(self_,param_1,param_2,param_3,iVar7,local_404,local_400);
          }
          else {
            orig::FUN_101cfdb0(param_1,param_2,param_3,iVar7,local_404,(int)local_400);
          }
          orig::YkJpeg_CutColor(self_,local_400);
          piVar8 = (int *)((int)self_ + 0x1c);
          iVar6 = 4;
          do {
            orig::FUN_101cff60((int)piVar8);
            orig::FUN_101d01b0(self_,piVar8,0);
            orig::FUN_101d0210(self_,piVar8,(char *)0x0);
            piVar8 = piVar8 + 0x40;
            iVar6 = iVar6 + -1;
          } while (iVar6 != 0);
          piVar8 = (int *)((int)self_ + 0x41c);
          orig::FUN_101cff60((int)piVar8);
          orig::FUN_101d01b0(self_,piVar8,1);
          orig::FUN_101d0210(self_,piVar8,(char *)0x1);
          piVar8 = (int *)((int)self_ + 0x51c);
          orig::FUN_101cff60((int)piVar8);
          orig::FUN_101d01b0(self_,piVar8,2);
          orig::FUN_101d0210(self_,piVar8,(char *)0x2);
          iVar7 = iVar7 + 0x10;
        } while (iVar7 < (int)param_1);
      }
      local_404 = local_404 + 0x10;
    } while (local_404 < (int)param_2);
  }
  orig::FUN_101d0a60(*(void **)((int)self_ + 8));
  orig::FUN_101d09e0(*(void **)((int)self_ + 8),0x1037e32a,2);
  return (uint32_t)(*(int *)(*(int *)((int)self_ + 8) + 0x10) <= *(int *)(*(int *)((int)self_ + 8) + 0xc))
  ;
}
```

## `0x101D05E0` fn_101D05E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101C8D70`, `sub_10311FF1`
- **Called by:** `fn_101D0370`, `sub_101D0640`

```cpp
XI_API uint8_t * __cdecl fn_101D05E0(void)
{
  orig::FUN_101c8d70();
  orig::_sprintf_v3(&DAT_10662f78,s__C__SQUARE_ENIX_1037e878);
  return(uint8_t*)(&DAT_10662f78);
}
```

## `0x101D0600` fn_101D0600

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101D0370`, `fn_101D0880`
- **Called by:** `fn_101D1060`

```cpp
XI_API int __thiscall fn_101D0600(void * self_, uint32_t param_1, uint32_t param_2, int param_3, int param_4)
{
  int iVar1;
  
  iVar1 = orig::FUN_101d0370(self_,param_1,param_2,param_3,param_4);
  if (iVar1 != 0) {
    orig::FUN_101d0880(self_);
  }
  return iVar1;
}
```

## `0x101D08C0` fn_101D08C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101D0860`
- **Called by:** `sub_101D1000`, `sub_101D1100`

```cpp
XI_API undefined4 __thiscall fn_101D08C0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_YkJpeg_GetRuntimeClass_10337e80;
  orig::FUN_101d0860(self_);
  return self_;
}
```

## `0x101D08D0` fn_101D08D0

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_101D1000`, `sub_101D1100`

```cpp
XI_API void __thiscall fn_101D08D0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_YkJpeg_GetRuntimeClass_10337e80;
  orig::FUN_101d0880(self_);
  return;
}
```
