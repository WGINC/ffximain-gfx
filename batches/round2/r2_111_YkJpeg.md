# Batch r2_111_YkJpeg: 3 functions of `YkJpeg`

Name these functions: see `batches/README.md`. Write `data/names/r2_111_YkJpeg.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101CFEB0` fn_101CFEB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 24`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `YkJpeg_Unpack565` (medium) x2
- **Called by:** `YkJpeg_Encode` (medium)
- **Named before** (a naming session's reading, low): `YkJpeg_UnpackBlock565`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `sub_101D0C70` → `sub_101D0EA0` → `sub_101D1000` → `fn_101D1060` → `YkJpeg_EncodeChecked` → `YkJpeg_Encode` → `YkJpeg_UnpackBlock565`
- **Named callers' callers:** `YkJpeg_EncodeChecked`
- **How its callers call it** (their verified code):
  - in `YkJpeg_Encode` (medium): `YkJpeg_UnpackBlock565(self_,param_1,param_2,param_3,iVar7,local_404,local_400);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `YkJpeg_Unpack565` (medium): Expands a 16-bit 5-6-5 colour into 8-bit components.
  - `YkJpeg_Encode` (medium): Encodes an image as a JPEG into an in-memory buffer.

```cpp
XI_API void __thiscall YkJpeg_UnpackBlock565(void * self_, int param_1, int param_2, int param_3, int param_4, int param_5, int * param_6)
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
        iVar2 = YkJpeg_Unpack565(uVar1,param_6,(int *)0x0,iVar2);
      }
      else {
        iVar2 = YkJpeg_Unpack565(uVar1,(int *)0x0,param_6,iVar2);
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

## `0x101D0600` fn_101D0600

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `YkJpeg_Encode` (medium), `Unit101CF660_Free` (low)
- **Called by:** `fn_101D1060`
- **Named before** (a naming session's reading, low): `YkJpeg_EncodeChecked`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_101D0C70` → `sub_101D0EA0` → `sub_101D1000` → `fn_101D1060` → `YkJpeg_EncodeChecked`
- **Named callees' callees:** `Mem_AllocDefault`, `thunk_sub_10028AA0`, `Unit101CF660_BuildTables`, `YkJpeg_InitTables`, `YkJpeg_CopyBlock`, `YkJpeg_UnpackBlock565`, `YkJpeg_ForwardDct`, `YkJpeg_EncodeBlock`, `YkJpeg_GetCopyrightString`, `Unit101CF660_Reset`, `YkJpgBit_Init`, `Unit101D0910_PutWordBE` and 3 more
- **How its callers call it** (their verified code):
  - in `fn_101D1060`: `iVar2 = YkJpeg_EncodeChecked(param_1,param_2,param_3,(int)pvVar1,0x5a);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `YkJpeg_Encode` (medium): Encodes an image as a JPEG into an in-memory buffer.
  - `Unit101CF660_Free` (low): Frees a bit buffer's two allocations.

Short callee `Unit101CF660_Free` (low):
```cpp
XI_API void __thiscall Unit101CF660_Free(void * self_)
{
  if (*(int *)((int)self_ + 8) != 0) {
    orig::thunk_sub_10028AA0_v1(*(int *)((int)self_ + 8));
    *(uint32_t *)((int)self_ + 8) = 0;
  }
  if (*(LPVOID *)((int)self_ + 4) != (LPVOID)0x0) {
    orig::operator_delete_10311BB0_v1(*(LPVOID *)((int)self_ + 4));
    *(uint32_t *)((int)self_ + 4) = 0;
  }
  return;
}
```

```cpp
XI_API int __thiscall YkJpeg_EncodeChecked(void * self_, uint32_t param_1, uint32_t param_2, int param_3, int param_4)
{
  int iVar1;
  
  iVar1 = YkJpeg_Encode(self_,param_1,param_2,param_3,param_4);
  if (iVar1 != 0) {
    Unit101CF660_Free(self_);
  }
  return iVar1;
}
```

## `0x101D08D0` fn_101D08D0

- **Gate:** none; **frame set:** static
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_101D1000`, `sub_101D1100`
- **Named before** (a naming session's reading, low): `YkJpeg_Release`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_101D0C70` → `sub_101D0EA0` → `sub_101D1000` → `YkJpeg_Release`

```cpp
XI_API void __thiscall YkJpeg_Release(void * self_)
{
  *(uint8_t ***)self_ = &PTR_YkJpeg_GetRuntimeClass_10337e80;
  Unit101CF660_Free(self_);
  return;
}
```
