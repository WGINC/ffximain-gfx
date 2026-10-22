# Batch 154_CMoDistModelElem: 3 functions of `CMoDistModelElem`

Name these functions: see `batches/README.md`. Write `data/names/154_CMoDistModelElem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100431B0` fn_100431B0

- **Gate:** none
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CDx_SetTransform`, `RegisterWithProcessor`, `fn_1006AD90`, `StateBlock_ApplyTSS`, `MoProcessor_CallVSlot49`, `XiZone_DrawModelAtMatrix`, `sub_10313486`
- **Direct3D 8:** `IDirect3DDevice8::SetRenderState`, `IDirect3DDevice8::SetTextureStageState` x4
- **Writes globals:** `0x1047BB74`
- **Reads globals:** `0x1047BB74`, `0x1047BC0C`, `0x1047BFA8`, `0x1047CF7C`, `0x10631F24`

```cpp
XI_API int __thiscall fn_100431B0(void *self_)
{
  int ScratchMatrixStackTop;
  float *v3;
  CMoProcessor *v4;
  int v5;
  if ( (*((_BYTE *)self_ + 379) & 3) != 0 )
  {
    ScratchMatrixStackTop = (*(int *)((char *)g_pProcessor + 0xA90));
    v3 = ((float * *)((char *)g_pProcessor + 0xA10))[ScratchMatrixStackTop];
    (*(int *)((char *)g_pProcessor + 0xA90)) = ScratchMatrixStackTop + 1;
    orig::MoProcessor_CallVSlot49(v3);
    v3[8] = *((float *)self_ + 72);
    v3[9] = *((float *)self_ + 73);
    (*(void (__stdcall **)(_DWORD, _DWORD, int, int))(**(_DWORD **)(g_pResourceMng + 3772) + 252))(
      *(_DWORD *)(g_pResourceMng + 3772),
      0,
      24,
      2);
    orig::CDx_SetTransform(*(_DWORD ***)(g_pResourceMng + 3528), 16, v3);
    v4 = g_pProcessor;
    v5 = (*(int *)((char *)g_pProcessor + 0xA90)) - 1;
    (*(int *)((char *)g_pProcessor + 0xA90)) = v5;
    ((float * *)((char *)v4 + 0xA10))[v5] = v3;
  }
  else
  {
    (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g_pResourceMng + 3772) + 252))(
      *(_DWORD *)(g_pResourceMng + 3772),
      0,
      11,
      0);
    (*(void (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g_pResourceMng + 3772) + 252))(
      *(_DWORD *)(g_pResourceMng + 3772),
      0,
      24,
      0);
  }
  if ( (Fn100431B0_staticGuards & 1) == 0 )
  {
    Fn100431B0_staticGuards |= 1u;
    orig::fn_1006AD90((exception *)&Fn100431B0_stateBlock);
    orig::_atexit(XI_CODE(unknown_libname_49));
  }
  orig::StateBlock_ApplyTSS(&Fn100431B0_stateBlock, (int)&Fn100431B0_textureStageStates);
  (*(void (__stdcall **)(_DWORD, int, int))(**(_DWORD **)(g_pResourceMng + 3772) + 200))(
    *(_DWORD *)(g_pResourceMng + 3772),
    60,
    g_scratchValue);
  orig::RegisterWithProcessor((int)self_);
  orig::XiZone_DrawModelAtMatrix((float *)g_pZoneMap, (float *)self_ + 40, *((_DWORD *)self_ + 65));
  return (*(int (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(**(_DWORD **)(g_pResourceMng + 3772) + 252))(
           *(_DWORD *)(g_pResourceMng + 3772),
           0,
           24,
           0);
}
```

## `0x10052F20` fn_10052F20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoElem::Constructor`
- **Called by:** `sub_1004E8D0`

```cpp
XI_API uint64_t __thiscall fn_10052F20(void * self_)
{
  uint64_t uVar1;
  
  uVar1 = orig::CMoElem_Constructor(self_);
  *(uint8_t ***)self_ = &PTR_CMoDistModelElem_GetRuntimeClass_1032b418;
  *(uint16_t *)((int)self_ + 0x176) = 0x198;
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar1 >> 0x20)) << 32) | (uint32_t)(self_)));
}
```

## `0x10052F40` CMoDistModelElem::vf012

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoDistModelElem` slot 12 (+0x30)

```cpp
BOOL CMoDistModelElem::vf012(int a1)
{
  return a1 == 4;
}
```
