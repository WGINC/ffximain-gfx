# Batch 087_unit_10003890_DirectInput: 14 functions of `unit_10003890_DirectInput`

Name these functions: see `batches/README.md`. Write `data/names/087_unit_10003890_DirectInput.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x10003890` fn_10003890

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CDx_SetTransform` x3, `CDx_GetTransformCached`, `CDx_DrawPrimitiveUP` x2, `Vec4_Copy`, `Vec3_Sub`, `fn_10027990` x3, `fn_100279A0` x3, `sub_100279B0` x2, `sub_10027BD0`, `Mat4_SetTranslation` x2
- **Called by:** `fn_10003710`
- **Writes globals:** `0x1034F72C`
- **Reads globals:** `0x1045666C`

```cpp
XI_API void __thiscall fn_10003890(_DWORD *self_)
{
  int v2;
  int v3;
  int v4;
  int v5;
  _DWORD *v6;
  alignas(16) unsigned char xi_frame[0x558];  // address-taken locals at their original offsets
  int &v7 = *reinterpret_cast<int *>(xi_frame + 0x84);
  float (&v8)[4] = *reinterpret_cast<float (*)[4]>(xi_frame + 0x88);
  _DWORD (&self_v)[16] = *reinterpret_cast<_DWORD (*)[16]>(xi_frame + 0x98);
  _DWORD (&v10)[16] = *reinterpret_cast<_DWORD (*)[16]>(xi_frame + 0xD8);
  _BYTE (&v11)[64] = *reinterpret_cast<_BYTE (*)[64]>(xi_frame + 0x118);

  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_FOGENABLE, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_LIGHTING, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLOROP, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_COLORARG1, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAOP, 2u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 0, D3DTSS_ALPHAARG1, 0);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_COLOROP, 1u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetTextureStageState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 1u, D3DTSS_ALPHAOP, 1u);
  orig::fn_10027990((Iostream_init *)self_v);
  orig::fn_10027990((Iostream_init *)v10);
  orig::fn_10027990((Iostream_init *)v11);
  orig::Mat4_SetIdentity(v10);
  orig::CDx_GetTransformCached(g_pCDx, 256, v11);
  v2 = 0;
  v3 = 0;
  v7 = 0;
  do
  {
    if ( *(_DWORD *)(*(self_ + 3) + v3 + 4) != -1 )
    {
      orig::Mat4_SetIdentity(self_v);
      orig::Mat4_PostRotateY(self_v, *(float *)(v2 + *(self_ + 4) + 12));
      orig::Mat4_SetTranslation(self_v, (_DWORD *)(v2 + *(self_ + 4)));
      orig::CDx_SetTransform(g_pCDx, 256, self_v);
      (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetVertexShader((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), 66u);
      orig::CDx_DrawPrimitiveUP(g_pCDx, 2, 3, (int)&g_debugAxisLineVerts, 16);
      v4 = ActerBuffPtr[*(_DWORD *)(*(self_ + 3) + v3)];
      if ( v4 != 0 && (*(_DWORD *)(v4 + 288) & 0x200) != 0 )
      {
        v5 = *(_DWORD *)(v4 + 160);
        if ( v5 != 0 )
        {
          g_debugLineVerts_v0_color = (*(unsigned __int8 (__thiscall **)(_DWORD))(*(_DWORD *)v5 + 408))(*(_DWORD *)(v4 + 160)) != 0
                                    ? -2147418113
                                    : -2130706433;
          v6 = (_DWORD *)(*(int (__thiscall **)(int))(*(_DWORD *)v5 + 444))(v5);
          orig::Vec4_Copy(v8, v6);
          orig::Vec3_Sub(g_debugLineVerts_v1, v8, (float *)(v2 + *(self_ + 4)));
          orig::Mat4_SetTranslation(v10, (_DWORD *)(v2 + *(self_ + 4)));
          orig::CDx_SetTransform(g_pCDx, 256, v10);
          orig::CDx_DrawPrimitiveUP(g_pCDx, 2, 1, (int)&g_debugLineVerts, 16);
          v3 = v7;
        }
      }
    }
    v3 += 8;
    v2 += 16;
    v7 = v3;
  }
  while ( v3 < 18432 );
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZWRITEENABLE, 1u);
  (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->lpVtbl->SetRenderState((*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC)), D3DRS_ZENABLE, 1u);
  orig::CDx_SetTransform(g_pCDx, 256, v11);
  orig::nullsub_1();
  orig::nullsub_1();
  orig::nullsub_1();
}
```

## `0x10005600` fn_10005600

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_101BCCB0`, `fn_101BCE30`
- **Writes globals:** `0x10455C37`, `0x10455C40`, `0x10455C41`, `0x10455C42`
- **Reads globals:** `0x10455C37`, `0x10455C41`, `0x10455C42`

```cpp
XI_API void __thiscall fn_10005600(void * self_, undefined4 param_1)
{
  char cVar1;
  
  cVar1 = DAT_10455c37;
  if ((char)param_1 == '\0') {
    if (DAT_10455c42 == '\0') {
      return;
    }
    DAT_10455c42 = 0;
    DAT_10455c37 = DAT_10455c41;
  }
  else {
    if (DAT_10455c42 != '\0') {
      return;
    }
    DAT_10455c37 = '\0';
    DAT_10455c41 = cVar1;
    DAT_10455c42 = 1;
  }
  if ((*(int *)((int)self_ + 0x90) != 0) && (DAT_10455c41 == '\x01')) {
    DAT_10455c40 = 1;
  }
  return;
}
```

## `0x10005660` fn_10005660

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `App_CreateInputSystem`
- **Writes globals:** `0x10455C3E`

```cpp
XI_API void * __thiscall fn_10005660(void * self_)
{
  int iVar1;
  uint32_t *puVar2;
  
  *(uint32_t *)((int)self_ + 0x90) = 0;
  *(uint8_t *)((int)self_ + 0xc99) = 0;
  *(uint32_t *)((int)self_ + 0x8c) = 0;
  *(uint32_t *)((int)self_ + 0x88) = 0;
  puVar2 = (uint32_t *)((int)self_ + 0x398);
  for (iVar1 = 0x100; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(uint32_t *)((int)self_ + 0x94) = 0x10;
  *(uint32_t *)((int)self_ + 0x8dc) = 0;
  *(uint32_t *)((int)self_ + 0xc94) = 0;
  *(uint32_t *)((int)self_ + 0xa30) = 0;
  puVar2 = (uint32_t *)((int)self_ + 0x98);
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = (uint32_t *)((int)self_ + 0x198);
  for (iVar1 = 0x40; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *(uint32_t *)((int)self_ + 0xdb0) = 0;
  *(uint32_t *)((int)self_ + 0xdb4) = 0;
  *(uint32_t *)((int)self_ + 0xdb8) = 0;
  *(uint8_t *)((int)self_ + 0xc98) = 0;
  *(uint32_t *)((int)self_ + 0xda4) = 0;
  *(uint8_t *)((int)self_ + 0xc9a) = 0;
  *(uint32_t *)((int)self_ + 0xdc0) = 0;
  DAT_10455c3e = 0;
  *(uint32_t *)((int)self_ + 0xa34) = 0;
  *(uint32_t *)((int)self_ + 0xa38) = 0;
  *(uint32_t *)((int)self_ + 0xa3c) = 0;
  *(uint32_t *)((int)self_ + 0xa40) = 0;
  *(uint32_t *)((int)self_ + 0xc9c) = 0xffffffff;
  return self_;
}
```

## `0x100059C0` fn_100059C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10005A40`
- **Reads globals:** `0x10455C35`

```cpp
XI_API undefined4 __cdecl fn_100059C0(uint32_t param_1)
{
  uint16_t uVar1;
  
  uVar1 = (uint16_t)(param_1 >> 0x10);
  if ((param_1 != 0xffffffff) && (DAT_10455c35 != '\0')) {
    if ((0x8ca < param_1) && (param_1 < 0x3d86)) {
      return ((uint32_t)(((uint32_t)(uint16_t)(uVar1) << 16) | (uint16_t)(0x7f)));
    }
    if ((0x4f1a < param_1) && (param_1 < 0x83d6)) {
      return ((uint32_t)(((uint32_t)(uint16_t)(uVar1) << 16) | (uint16_t)(0xff81)));
    }
  }
  return (uint32_t)uVar1 << 0x10;
}
```

## `0x10005A00` fn_10005A00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10005A40`

```cpp
XI_API undefined4 __cdecl fn_10005A00(uint32_t param_1)
{
  uint16_t uVar1;
  
  if (param_1 != 0xffffffff) {
    uVar1 = (uint16_t)(param_1 >> 0x10);
    if ((param_1 < 0x1a5e) || (0x7242 < param_1)) {
      return ((uint32_t)(((uint32_t)(uint16_t)(uVar1) << 16) | (uint16_t)(0xff81)));
    }
    if ((0x2bf2 < param_1) && (param_1 < 0x60ae)) {
      return ((uint32_t)(((uint32_t)(uint16_t)(uVar1) << 16) | (uint16_t)(0x7f)));
    }
  }
  return param_1 & 0xffff0000;
}
```

## `0x10007830` fn_10007830

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AX of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10005A40`

```cpp
XI_API undefined4 __cdecl fn_10007830(char * param_1, undefined4 param_2, undefined4 param_3)
{
  char cVar1;
  uint32_t xi_entry_eax = 0;
  uint32_t uVar2;
  uint16_t uVar3;
  int iVar4;
  
  iVar4 = 0;
  cVar1 = *param_1;
  uVar2 = ((uint32_t)(((uint32_t)((int32_t)((uint32_t)xi_entry_eax >> 8)) << 8) | (uint8_t)(cVar1)));
  while( true ) {
    uVar3 = (uint16_t)((uint32_t)uVar2 >> 0x10);
    if (cVar1 == '\0') {
      return ((uint32_t)(((uint32_t)(uint16_t)(uVar3) << 16) | (uint16_t)((uint16_t)param_2)));
    }
    if ((char)uVar2 == (char)param_3) break;
    cVar1 = param_1[iVar4 * 2 + 2];
    uVar2 = ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar2 >> 8)) << 8) | (uint8_t)(cVar1)));
    iVar4 = iVar4 + 1;
  }
  return ((uint32_t)(((uint32_t)(uint16_t)(uVar3) << 16) | (uint16_t)((uint16_t)(uint8_t)param_1[iVar4 * 2 + 1])));
}
```

## `0x10007860` fn_10007860

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10080DC0`, `sub_1015DD50`, `sub_1015DD70` x2, `fn_101B36F0`, `sub_10312AD3`, `sub_10312BDB`
- **Called by:** `sub_100016C0`
- **Reads globals:** `0x10578490`, `0x1062193C`, `0x10662D40`

```cpp
XI_API void __stdcall fn_10007860(uint32_t param_1, undefined4 xi_s2)
{
  uint8_t bVar1;
  int iVar2;
  char cVar3;
  uint8_t bVar4;
  int iVar5;
  uint32_t uVar6;
  uint32_t uVar7;
  uint32_t uVar8;
  uint32_t *puVar9;
  char *pcVar10;
  uint32_t *puVar11;
  char xi_frame[256] __attribute__((aligned(4)));
  uint32_t  (&local_100)[64] = *reinterpret_cast<uint32_t  (*)[64]>(xi_frame + 0);
  
  orig::FUN_101b36f0(DAT_10662d40,param_1);
  if (DAT_10578490 == 0) {
    return;
  }
  if (DAT_1062193c_as_s32 == 0) {
    return;
  }
  iVar2 = orig::FUN_10312bdb(param_1);
  iVar5 = DAT_10578490;
  if (iVar2 != 0) {
LAB_10007a18:
    iVar2 = -1;
    pcVar10 = (char *)(DAT_10578490 + 0x14);
    do {
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      cVar3 = *pcVar10;
      pcVar10 = pcVar10 + 1;
    } while (cVar3 != '\0');
    cVar3 = ~(uint8_t)iVar2 - 1;
    if (cVar3 != -1) {
      bVar4 = cVar3 - *(uint8_t *)(DAT_10578490 + 0x114);
      if (bVar4 != 0) {
        iVar2 = (uint32_t)*(uint8_t *)(DAT_10578490 + 0x114) + DAT_10578490;
        puVar9 = (uint32_t *)(iVar2 + 0x14);
        puVar11 = local_100;
        for (uVar7 = (uint32_t)(bVar4 >> 2); uVar7 != 0; uVar7 = uVar7 - 1) {
          *puVar11 = *puVar9;
          puVar9 = puVar9 + 1;
          puVar11 = puVar11 + 1;
        }
        for (uVar7 = bVar4 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
          *(uint8_t *)puVar11 = *(uint8_t *)puVar9;
          puVar9 = (uint32_t *)((int)puVar9 + 1);
          puVar11 = (uint32_t *)((int)puVar11 + 1);
        }
        puVar9 = local_100;
        puVar11 = (uint32_t *)(iVar2 + 0x15);
        for (uVar7 = (uint32_t)(bVar4 >> 2); uVar7 != 0; uVar7 = uVar7 - 1) {
          *puVar11 = *puVar9;
          puVar9 = puVar9 + 1;
          puVar11 = puVar11 + 1;
        }
        for (uVar7 = bVar4 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
          *(uint8_t *)puVar11 = *(uint8_t *)puVar9;
          puVar9 = (uint32_t *)((int)puVar9 + 1);
          puVar11 = (uint32_t *)((int)puVar11 + 1);
        }
      }
      *(uint8_t *)(*(uint8_t *)(iVar5 + 0x114) + 0x14 + iVar5) = (uint8_t)param_1;
      bVar1 = *(char *)(iVar5 + 0x114) + 1;
      *(uint8_t *)(iVar5 + 0x114) = bVar1;
      *(uint8_t *)(bVar4 + 0x14 + iVar5 + (uint32_t)bVar1) = 0;
    }
    orig::FUN_1015dd50(&g_menuMng_as_v0,param_1);
    return;
  }
  if (param_1 != 8) {
    if (param_1 == 0xd) {
      orig::FUN_1015dd70(&g_menuMng_as_v0,0xd);
      iVar5 = -1;
      pcVar10 = (char *)(DAT_10578490 + 0x14);
      do {
        if (iVar5 == 0) break;
        iVar5 = iVar5 + -1;
        cVar3 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar3 != '\0');
      if (iVar5 == -2) {
        return;
      }
      orig::CommandCalc((char *)(DAT_10578490 + 0x14),0);
    }
    else if (param_1 != 0x1b) {
      if (param_1 < 0x80) {
        return;
      }
      goto LAB_10007a18;
    }
    puVar9 = (uint32_t *)(DAT_10578490 + 0x14);
    for (iVar5 = 0x40; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar9 = 0;
      puVar9 = puVar9 + 1;
    }
    return;
  }
  if (*(uint8_t *)(DAT_10578490 + 0x114) != 0) {
    iVar2 = orig::FUN_10312ad3((int)*(char *)(*(uint8_t *)(DAT_10578490 + 0x114) + 0x12 + DAT_10578490));
    if (iVar2 == 0) {
      iVar2 = -1;
      uVar8 = 0;
      *(uint8_t *)(*(uint8_t *)(iVar5 + 0x114) + 0x13 + iVar5) = 0;
      uVar7 = (uint32_t)*(uint8_t *)(iVar5 + 0x114);
      pcVar10 = (char *)(uVar7 + 0x14 + iVar5);
      do {
        if (iVar2 == 0) break;
        iVar2 = iVar2 + -1;
        cVar3 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar3 != '\0');
      if (iVar2 != -2) {
        do {
          iVar2 = uVar7 + uVar8;
          uVar8 = uVar8 + 1;
          *(uint8_t *)(iVar2 + iVar5 + 0x13) = *(uint8_t *)(iVar2 + 0x14 + iVar5);
          uVar7 = (uint32_t)*(uint8_t *)(iVar5 + 0x114);
          uVar6 = 0xffffffff;
          pcVar10 = (char *)(uVar7 + 0x14 + iVar5);
          do {
            if (uVar6 == 0) break;
            uVar6 = uVar6 - 1;
            cVar3 = *pcVar10;
            pcVar10 = pcVar10 + 1;
          } while (cVar3 != '\0');
        } while (uVar8 < ~uVar6 - 1);
      }
      cVar3 = *(char *)(iVar5 + 0x114) + -1;
    }
    else {
      if (*(uint8_t *)(iVar5 + 0x114) < 2) goto LAB_100079f4;
      iVar2 = -1;
      uVar8 = 0;
      *(uint8_t *)(*(uint8_t *)(iVar5 + 0x114) + 0x12 + iVar5) = 0;
      uVar7 = (uint32_t)*(uint8_t *)(iVar5 + 0x114);
      pcVar10 = (char *)(uVar7 + 0x14 + iVar5);
      do {
        if (iVar2 == 0) break;
        iVar2 = iVar2 + -1;
        cVar3 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar3 != '\0');
      if (iVar2 != -2) {
        do {
          iVar2 = uVar7 + uVar8;
// ... 21 more lines (tree/src/unit_10003890_DirectInput.cpp)
```

## `0x10008770` fn_10008770

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `DirectInput_InstallJoystick`
- **Writes globals:** `0x10455C28`
- **Reads globals:** `0x10455C28`

```cpp
XI_API undefined4 __thiscall fn_10008770(void * self_, int param_1)
{
  if (param_1 < 0) {
    if (DAT_10455c28_as_v0 != (int *)0x0) {
      (**(code **)(*DAT_10455c28_as_v0 + 0x2c))(DAT_10455c28_as_v0);
      (**(code **)(*DAT_10455c28_as_v0 + 8))(DAT_10455c28_as_v0);
      DAT_10455c28_as_v0 = (int *)0x0;
    }
    param_1 = ((int (__stdcall *)(XI_ANY))((**(code **)(**(int **)((int)self_ + 0x90) + 8))))(*(int **)((int)self_ + 0x90));
    *(uint32_t *)((int)self_ + 0x90) = 0;
    DAT_10455c28_as_v0 = (int *)0x0;
  }
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)param_1 >> 8)) << 8) | (uint8_t)(1)));
}
```

## `0x100087D0` fn_100087D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100016C0`
- **Reads globals:** `0x10455C20`

```cpp
XI_API void __stdcall fn_100087D0(int param_1, int param_2, undefined4 xi_s3)
{
  int *piVar1;
  uint8_t bVar2;
  
  if ((param_1 == 6) && (param_2 != 0)) {
    bVar2 = true;
  }
  else {
    bVar2 = false;
  }
  piVar1 = *(int **)(g_pAppInputState + 0x88);
  if (piVar1 != (int *)0x0) {
    if (bVar2) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 0x1c))))(piVar1);
    }
    else {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 0x20))))(piVar1);
    }
  }
  piVar1 = *(int **)(g_pAppInputState + 0x90);
  if (piVar1 != (int *)0x0) {
    ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 0x1c))))(piVar1);
  }
  return;
}
```

## `0x10008830` fn_10008830

- **Gate:** none; **frame set:** static, self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Frame_Render`, `sub_10018850`, `sub_10069EE0`, `sub_10124150`, `sub_1018F080`, `sub_101B2810`
- **Reads globals:** `0x104E1D4C`

```cpp
XI_API undefined4 __thiscall fn_10008830(void * self_, uint32_t param_1)
{
  int iVar1;
  uint32_t uVar2;
  
  if ((char)param_1 != '\0') {
    uVar2 = orig::FUN_10124130(&DAT_104e1c40,param_1);
    return uVar2;
  }
  iVar1 = *(int *)((int)self_ + 0xc94);
  if ((iVar1 == 0) && (iVar1 = *(int *)((int)self_ + 0x8dc), iVar1 == 0)) {
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)g_pCursor_as_s32 >> 8)) << 8) | (uint8_t)(*(int *)(g_pCursor_as_s32 + 0x60) != 0)));
  }
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)iVar1 >> 8)) << 8) | (uint8_t)(1)));
}
```

## `0x100088F0` fn_100088F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10124930` x2, `fn_101249E0` x2, `fn_10124A00`, `fn_10124AB0`, `fn_10124AD0`, `fn_10124B50`
- **Called by:** `Frame_Render`
- **Reads globals:** `0x10455C3A`, `0x104E1D4C`

```cpp
XI_API void __thiscall fn_100088F0(void * self_)
{
  int iVar1;
  int *piVar2;
  uint32_t uVar3;
  
  uVar3 = 0;
  if (*(int *)((int)self_ + 0xa30) != 0) {
    piVar2 = (int *)((int)self_ + 0x8f0);
    do {
      if ((*(uint8_t *)(piVar2 + 1) & 0x80) == 0) {
LAB_10008963:
        iVar1 = *piVar2;
        if (iVar1 == 0xc) {
          if (DAT_10455c3a == '\0') {
            orig::FUN_101249e0(g_pCursor_as_v2);
          }
          else {
LAB_100089a9:
            orig::FUN_10124ab0(g_pCursor_as_v2);
          }
        }
        else if (iVar1 == 0xd) {
          if (DAT_10455c3a == '\0') goto LAB_100089a9;
          orig::FUN_101249e0(g_pCursor_as_v2);
        }
        else if (iVar1 == 0xe) {
          orig::FUN_10124b50(g_pCursor_as_v2);
        }
      }
      else {
        iVar1 = *piVar2;
        if (iVar1 == 0xc) {
          if (DAT_10455c3a == '\0') {
            orig::FUN_10124930(g_pCursor_as_v2);
          }
          else {
LAB_10008959:
            orig::FUN_10124a00(g_pCursor_as_v2);
          }
        }
        else if (iVar1 == 0xd) {
          if (DAT_10455c3a == '\0') goto LAB_10008959;
          orig::FUN_10124930(g_pCursor_as_v2);
        }
        else if (iVar1 == 0xe) {
          orig::FUN_10124ad0(g_pCursor_as_v2);
        }
        if ((*(uint8_t *)(piVar2 + 1) & 0x80) == 0) goto LAB_10008963;
      }
      uVar3 = uVar3 + 1;
      piVar2 = piVar2 + 5;
    } while (uVar3 < *(uint32_t *)((int)self_ + 0xa30));
  }
  return;
}
```

## `0x100089D0` fn_100089D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `DirectInput_InstallMouse`
- **Called by:** `sub_10005A40`
- **Writes globals:** `0x10456549`
- **Reads globals:** `0x1034F92C`, `0x10456549`

```cpp
XI_API void __thiscall fn_100089D0(void * self_)
{
  uint32_t *a4;
  int *a1;
  int iVar1;
  uint32_t *a3;
  
  a1 = *(int **)((int)self_ + 0x8c);
  if ((a1 == (int *)0x0) || (g_mouseAcquired == '\0')) {
    return;
  }
  a4 = (uint32_t *)((int)self_ + 0xa30);
  a3 = (uint32_t *)((int)self_ + 0x8f0);
  *a4 = 0x10;
  iVar1 = ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*a1 + 0x28))))(a1,0x14,a3,a4,0);
  if (iVar1 < -0x7ff8ff55) {
    if (((iVar1 == -0x7ff8ff56) || (iVar1 == -0x7ff8fff4)) || (iVar1 == -0x7ff8ffe2)) {
      if (DAT_10456549 != '\0') {
        ((int (__stdcall *)(XI_ANY))((**(code **)(**(int **)((int)self_ + 0x8c) + 0x1c))))(*(int **)((int)self_ + 0x8c));
        *a4 = 0;
        for (iVar1 = 0x50; iVar1 != 0; iVar1 = iVar1 + -1) {
          *a3 = 0;
          a3 = a3 + 1;
        }
        return;
      }
      DAT_10456549 = 1;
      ((int (__stdcall *)(XI_ANY))((**(code **)(**(int **)((int)self_ + 0x8c) + 8))))(*(int **)((int)self_ + 0x8c));
      orig::DirectInput_InstallMouse(self_);
      *a4 = 0;
      for (iVar1 = 0x50; iVar1 != 0; iVar1 = iVar1 + -1) {
        *a3 = 0;
        a3 = a3 + 1;
      }
      return;
    }
  }
  else if (iVar1 == 0) {
    DAT_10456549 = 0;
    return;
  }
  *a4 = 0;
  for (iVar1 = 0x50; iVar1 != 0; iVar1 = iVar1 + -1) {
    *a3 = 0;
    a3 = a3 + 1;
  }
  return;
}
```

## `0x10008D90` fn_10008D90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `DirectInput_InstallJoystick`
- **Imports:** `FreeLibrary`, `LoadLibraryA`
- **Reads globals:** `0x1034FB68`, `0x1034FB6C`, `0x1034FB70`, `0x1034FB74`, `0x1034FB76`, `0x103D3C64`, `0x10455C38`

```cpp
XI_API undefined4 __thiscall fn_10008D90(void * self_)
{
  char *a1;
  char cVar1;
  uint32_t xi_entry_eax = 0;
  uint32_t uVar2;
  uint32_t uVar3;
  char *pcVar4;
  int iVar5;
  char *pcVar6;
  
  uVar2 = ((uint32_t)(((uint32_t)((int32_t)((uint32_t)xi_entry_eax >> 8)) << 8) | (uint8_t)(DAT_10455c38)));
  if (DAT_10455c38 != '\0') {
    a1 = (char *)((int)self_ + 0xca0);
    *a1 = '\0';
    pcVar4 = *(char **)(g_pApp_as_s32 + 0x10);
    pcVar6 = a1;
    do {
      cVar1 = *pcVar4;
      pcVar4 = pcVar4 + 1;
      *pcVar6 = cVar1;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    iVar5 = -1;
    pcVar4 = a1;
    do {
      pcVar6 = pcVar4;
      if (iVar5 == 0) break;
      iVar5 = iVar5 + -1;
      pcVar6 = pcVar4 + 1;
      cVar1 = *pcVar4;
      pcVar4 = pcVar6;
    } while (cVar1 != '\0');
    *(uint32_t *)(pcVar6 + -1) = XI_PIECE(s_K___xinputdll_dll_1034fb65, 3, 4);
    *(uint32_t *)(pcVar6 + 3) = XI_PIECE(s_K___xinputdll_dll_1034fb65, 7, 4);
    *(uint32_t *)(pcVar6 + 7) = XI_PIECE(s_K___xinputdll_dll_1034fb65, 11, 4);
    *(uint16_t *)(pcVar6 + 0xb) = XI_PIECE(s_K___xinputdll_dll_1034fb65, 15, 2);
    pcVar6[0xd] = s_K___xinputdll_dll_1034fb65[0x11];
    iVar5 = LoadLibraryA(a1);
    uVar2 = 0;
    if (iVar5 != 0) {
      uVar3 = FreeLibrary(iVar5);
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar3 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  return uVar2 & 0xffffff00;
}
```

## `0x10008E10` fn_10008E10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `DirectInput_InstallJoystick`
- **Imports:** `LoadLibraryA`
- **Writes globals:** `0x1045654C`, `0x10456550`, `0x10456554`, `0x10456558`
- **Reads globals:** `0x10455C38`, `0x1045654C`

```cpp
XI_API undefined4 __thiscall fn_10008E10(void * self_)
{
  uint32_t xi_entry_eax = 0;
  uint32_t uVar1;
  
  uVar1 = ((uint32_t)(((uint32_t)((int32_t)((uint32_t)xi_entry_eax >> 8)) << 8) | (uint8_t)(DAT_10455c38)));
  if (DAT_10455c38 != '\0') {
    DAT_1045654c = (HMODULE)LoadLibraryA((int)self_ + 0xca0);
    uVar1 = 0;
    if (DAT_1045654c != (HMODULE)0x0) {
      DAT_10456550 = GetProcAddress(DAT_1045654c,s_XInputGetState__1034fb98);
      DAT_10456554 = GetProcAddress(DAT_1045654c,s_XInputSetState__1034fb88);
      DAT_10456558 = GetProcAddress(DAT_1045654c,s_XInputEnable__1034fb78);
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)DAT_10456558 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  return uVar1 & 0xffffff00;
}
```
