# Batch r2_125_sqmdModel: 10 functions of `sqmdModel`

Name these functions: see `batches/README.md`. Write `data/names/r2_125_sqmdModel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10277B70` fn_10277B70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree` x23, `SqmdModel_FreeItems` (medium), `SqgxTexture_ClearPaletteUsed` (low), `fn_10281A70`
- **Called by:** `SqmdModel_Free` (medium)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`
- **Strings its callees refer to:** `VALID`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `StAvatar_SetActive` → `StAvatarFini` → `mdl_1026E900` → `SqmdModel_Free` → `fn_10277B70`
- **Named callers' callers:** `StModelItemClear`, `mdl_1026E900`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_GetPayloadEnd`, `SqMem_FindArena`, `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `SqmdModel_Free` (medium): `orig::FUN_10277b70(*param_1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmdModel_Free` (medium): Releases a sqmdModel and its buffers, checking its DMB header when it has one.
  - `SqmdModel_FreeItems` (medium): Releases the model's item entries that it owns.
  - `SqgxTexture_ClearPaletteUsed` (low): Marks the texture palette slot at the index as unused.

Short callee `SqgxTexture_ClearPaletteUsed` (low):
```cpp
XI_API void __cdecl SqgxTexture_ClearPaletteUsed(int param_1)
{
  ((uint8_t *)((&g_sqgxPaletteUsed_as_v0)))[param_1] = 0;
  return;
}
```

```cpp
XI_API void __cdecl fn_10277B70(int param_1)
{
  int iVar1;
  int iVar2;
  int iVar3;
  
  SqmdModel_FreeItems((int *)param_1);
  if ((*(int *)(param_1 + 0x24) != 0) || (*(int *)(param_1 + 0x28) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x1c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x1c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xdd);
        *(uint32_t *)(param_1 + 0x1c) = 0;
      }
      if (*(int *)(param_1 + 0x20) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x20),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xde);
        *(uint32_t *)(param_1 + 0x20) = 0;
      }
    }
    else {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x24)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x1c) + iVar3 * 4);
          iVar2 = *(int *)(iVar1 + 0x14);
          if (-1 < iVar2) {
            orig::FUN_10281a70(iVar2);
          }
          iVar1 = *(int *)(iVar1 + 0x18);
          if (-1 < iVar1) {
            SqgxTexture_ClearPaletteUsed(iVar1);
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x24));
        *(uint32_t *)(param_1 + 0x28) = 0;
        *(uint32_t *)(param_1 + 0x24) = 0;
        goto LAB_10277c2c;
      }
    }
    *(uint32_t *)(param_1 + 0x28) = 0;
    *(uint32_t *)(param_1 + 0x24) = 0;
  }
LAB_10277c2c:
  if ((*(int *)(param_1 + 0x34) != 0) || (*(int *)(param_1 + 0x38) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x2c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x2c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xe6);
        *(uint32_t *)(param_1 + 0x2c) = 0;
      }
      if (*(int *)(param_1 + 0x30) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x30),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xe7);
        *(uint32_t *)(param_1 + 0x30) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x38) = 0;
    *(uint32_t *)(param_1 + 0x34) = 0;
  }
  if ((*(int *)(param_1 + 0x58) != 0) || (*(int *)(param_1 + 0x5c) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x58)) {
        do {
          if (((*(uint8_t *)(*(int *)(param_1 + 0x54) + iVar3 * 4) & 0x20) != 0) &&
             (iVar1 = *(int *)(*(int *)(param_1 + 0x50) + iVar3 * 4), iVar1 != 0)) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf1);
            *(uint32_t *)(*(int *)(param_1 + 0x50) + iVar3 * 4) = 0;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x58));
      }
      if (*(int *)(param_1 + 0x4c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x4c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf4);
        *(uint32_t *)(param_1 + 0x4c) = 0;
      }
      if (*(int *)(param_1 + 0x50) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x50),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf5);
        *(uint32_t *)(param_1 + 0x50) = 0;
      }
      if (*(int *)(param_1 + 0x54) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x54),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xf6);
        *(uint32_t *)(param_1 + 0x54) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x5c) = 0;
    *(uint32_t *)(param_1 + 0x58) = 0;
  }
  if ((*(int *)(param_1 + 0x44) != 0) || (*(int *)(param_1 + 0x48) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x3c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x3c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xfe);
        *(uint32_t *)(param_1 + 0x3c) = 0;
      }
      if (*(int *)(param_1 + 0x40) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x40),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0xff);
        *(uint32_t *)(param_1 + 0x40) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x48) = 0;
    *(uint32_t *)(param_1 + 0x44) = 0;
  }
  if ((*(int *)(param_1 + 0x68) != 0) || (*(int *)(param_1 + 0x6c) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      if (*(int *)(param_1 + 0x60) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x60),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x107);
        *(uint32_t *)(param_1 + 0x60) = 0;
      }
      if (*(int *)(param_1 + 100) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 100),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x108);
        *(uint32_t *)(param_1 + 100) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x6c) = 0;
    *(uint32_t *)(param_1 + 0x68) = 0;
  }
  if ((*(int *)(param_1 + 0x7c) != 0) || (*(int *)(param_1 + 0x80) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x7c)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x70) + iVar3 * 4);
          if (iVar1 != 0) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x111);
            *(uint32_t *)(*(int *)(param_1 + 0x70) + iVar3 * 4) = 0;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x7c));
      }
      if (*(int *)(param_1 + 0x70) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x70),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x113);
        *(uint32_t *)(param_1 + 0x70) = 0;
      }
      if (*(int *)(param_1 + 0x74) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x74),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x114);
        *(uint32_t *)(param_1 + 0x74) = 0;
      }
      if (*(int *)(param_1 + 0x78) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x78),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x115);
        *(uint32_t *)(param_1 + 0x78) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x80) = 0;
    *(uint32_t *)(param_1 + 0x7c) = 0;
  }
  if ((*(int *)(param_1 + 0x90) != 0) || (*(int *)(param_1 + 0x94) != 0)) {
    if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
      iVar3 = 0;
      if (0 < *(int *)(param_1 + 0x90)) {
        do {
          iVar1 = *(int *)(*(int *)(param_1 + 0x84) + iVar3 * 4);
          if (iVar1 != 0) {
            orig::sqFree_v1(iVar1,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x11e);
            *(uint32_t *)(*(int *)(param_1 + 0x84) + iVar3 * 4) = 0;
          }
          iVar3 = iVar3 + 1;
        } while (iVar3 < *(int *)(param_1 + 0x90));
      }
      if (*(int *)(param_1 + 0x84) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x84),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x120);
        *(uint32_t *)(param_1 + 0x84) = 0;
      }
      if (*(int *)(param_1 + 0x88) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x88),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x121);
        *(uint32_t *)(param_1 + 0x88) = 0;
      }
      if (*(int *)(param_1 + 0x8c) != 0) {
        orig::sqFree_v1(*(int *)(param_1 + 0x8c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x122);
        *(uint32_t *)(param_1 + 0x8c) = 0;
      }
    }
    *(uint32_t *)(param_1 + 0x94) = 0;
    *(uint32_t *)(param_1 + 0x90) = 0;
  }
  if ((*(uint32_t *)(param_1 + 0xa8) & 0x200) == 0) {
    if (*(int *)(param_1 + 0x98) != 0) {
      orig::sqFree_v1(*(int *)(param_1 + 0x98),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x129);
      *(uint32_t *)(param_1 + 0x98) = 0;
    }
    if (*(int *)(param_1 + 0x9c) != 0) {
      orig::sqFree_v1(*(int *)(param_1 + 0x9c),s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x12a);
      *(uint32_t *)(param_1 + 0x9c) = 0;
    }
    if (*(int *)(param_1 + 0xa0) != 0) {
      orig::sqFree_v1(*(int *)(param_1 + 0xa0),s_C__dev_dancer_modules_sqModel_sr_103b37ec,299);
      *(uint32_t *)(param_1 + 0xa0) = 0;
    }
  }
  *(uint32_t *)(param_1 + 0xa4) = 0;
  *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) & 0x200;
  return;
}
```

## `0x10279530` fn_10279530

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x2
- **Called by:** `sub_1026F200`, `sub_102700C0`, `sub_1029FFF0`, `sub_102A0950`, `sub_102A0F90`, `sub_102A12F0`, `sub_102A17B0`, `sub_102A1B00`
- **Named before** (a naming session's reading, low): `SqmdModel_AppendPair44`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`
- **Strings its callees refer to:** `VALID`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_1024DA10` → `sub_102700C0` → `SqmdModel_AppendPair44`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_FindArena`, `sqFree`, `sqDebugPrintf`

```cpp
XI_API void __cdecl SqmdModel_AppendPair44(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar2 = *(int *)(param_1 + 0x44);
  if (*(int *)(param_1 + 0x48) <= iVar2) {
    uVar1 = iVar2 * 4 + 4;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x40),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x547);
    *(undefined4 **)(param_1 + 0x40) = puVar3;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x3c),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x549);
    *(undefined4 **)(param_1 + 0x3c) = puVar3;
  }
  *(undefined4 *)(*(int *)(param_1 + 0x40) + iVar2 * 4) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x3c) + iVar2 * 4) = param_2;
  *(int *)(param_1 + 0x44) = *(int *)(param_1 + 0x44) + 1;
  return;
}
```

## `0x102796A0` fn_102796A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275E90` x2
- **Called by:** `sub_1027A220`
- **Named before** (a naming session's reading, low): `SqmdModel_AppendPair68`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`
- **Strings its callees refer to:** `VALID`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_1024DA10` → `sub_102700C0` → `sub_1027A220` → `SqmdModel_AppendPair68`
- **Named callers' callers:** `sqmdDMBBuildShapes`, `sqsdShader_ParseScript`, `sqmdScriptCmd_skeleton_shape`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_FindArena`, `sqFree`, `sqDebugPrintf`

```cpp
XI_API void __cdecl SqmdModel_AppendPair68(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  int iVar2;
  undefined4 *puVar3;
  
  iVar2 = *(int *)(param_1 + 0x68);
  if (*(int *)(param_1 + 0x6c) <= iVar2) {
    uVar1 = iVar2 * 4 + 4;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 100),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x58f);
    *(undefined4 **)(param_1 + 100) = puVar3;
    puVar3 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x60),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x591);
    *(undefined4 **)(param_1 + 0x60) = puVar3;
  }
  *(undefined4 *)(*(int *)(param_1 + 100) + iVar2 * 4) = 0;
  *(undefined4 *)(*(int *)(param_1 + 0x60) + iVar2 * 4) = param_2;
  *(int *)(param_1 + 0x68) = *(int *)(param_1 + 0x68) + 1;
  return;
}
```

## `0x10279830` fn_10279830

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10279BF0`, `sub_10279E60`, `sub_1028C410`
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemKind0`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `StAvatar_Reset` → `sub_1026ECC0` → `fn_102736E0` → `sub_10278710` → `sub_102787F0` → `SqmdModel_LookupKind2` → `sub_10279BF0` → `SqmdModel_GetItemKind0`
- **Named callers' callers:** `SqmdModel_LookupKind2`
- **Named callees' callees:** `sqDebugPrintf`

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind0(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,0,param_2);
  return uVar1;
}
```

## `0x10279850` fn_10279850

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_102787F0`, `sub_10279BF0`, `sub_10279ED0`, `sub_1027A4B0`, `sub_1028C4F0`, `sub_1028C540`
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemKind1`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `sub_1026ECC0` → `fn_102736E0` → `sub_10278710` → `sub_102787F0` → `SqmdModel_GetItemKind1`
- **Named callers' callers:** `SqmdModel_LookupKind2`
- **Named callees' callees:** `sqDebugPrintf`

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind1(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,1,param_2);
  return uVar1;
}
```

## `0x10279870` fn_10279870

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `StAvatar_UpdateCameraFollow` (low), `StAvatar_GetOffset` (low), `Unit10252A00_SetPositionAndScale` (low), `Unit10252A00_SetVec78` (low), `sub_10252BC0`, `fn_1026F160`, `sub_102700C0`, `sub_10270320`, `sub_102704C0`, `sqmdModel_Dump`, `sub_10278490`, `SqmdModel_LookAt` (medium), `sub_10279BF0`, `sub_10279F40`, `sub_10287990`, `sub_1028CBE0`, `sub_1028CE20`, `sub_1028CEB0`, `sub_1028D030`, `sub_1028D2C0` and 1 more
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemKind2`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `StAvatar_UpdateInput` → `StAvatar_UpdateCameraFollow` → `SqmdModel_GetItemKind2`
- **Named callers' callers:** `StAvatar_UpdateInput`, `StAvatar_SetFlagsBoth`, `StAvatar_AdvanceTimers`, `StAvatar_ProcessTriggers`, `StAvatar_UpdateModels`, `SqmdModel_LookupKind2`
- **Named callees' callees:** `sqDebugPrintf`
- **How its callers call it** (their verified code):
  - in `StAvatar_UpdateCameraFollow` (low): `piVar2 = (int *)SqmdModel_GetItemKind2(iVar3,local_80);`
  - in `StAvatar_GetOffset` (low): `piVar4 = (int *)SqmdModel_GetItemKind2(iVar2,iVar5);`
  - in `Unit10252A00_SetPositionAndScale` (low): `piVar3 = (int *)SqmdModel_GetItemKind2(iVar1,iVar4);`
  - in `Unit10252A00_SetVec78` (low): `piVar3 = (int *)SqmdModel_GetItemKind2(iVar1,iVar4);`
  - in `fn_1026F160`: `piVar2 = (int *)SqmdModel_GetItemKind2(iVar1,iVar3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_UpdateCameraFollow` (low): Moves the camera to follow the avatar's target object.
  - `StAvatar_GetOffset` (low): Returns the avatar's offset vector for the current pose.
  - `Unit10252A00_SetPositionAndScale` (low): Sets the position and scale on the matching sub-objects.
  - `Unit10252A00_SetVec78` (low): Sets a uniform vector on the matching sub-objects.
  - `SqmdModel_LookAt` (medium): Aims a model item at a target, clamping the angle to the given limit.

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind2(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,2,param_2);
  return uVar1;
}
```

## `0x10279890` fn_10279890

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10278490`, `sub_102787F0`, `sub_10279BF0`, `sub_10279FB0`, `sub_1027A4B0`, `sub_10287990`, `sub_1028C1F0`, `sub_1028CE20`, `sub_1028D2C0`
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemKind3`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `fn_10270F30` → `StAvatar_UpdateModels` → `sub_10278490` → `SqmdModel_GetItemKind3`
- **Named callers' callers:** `StAvatar_UpdateModels`, `SqmdModel_LookupKind2`
- **Named callees' callees:** `sqDebugPrintf`

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind3(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,3,param_2);
  return uVar1;
}
```

## `0x102798B0` fn_102798B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279710`
- **Called by:** `sqmdModel_Dump`, `sub_10279BF0`, `sub_1027A020`
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemKind4`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `StAvatar_Reset` → `sub_1026ECC0` → `fn_102736E0` → `sub_10278710` → `sub_102787F0` → `SqmdModel_LookupKind2` → `sub_10279BF0` → `SqmdModel_GetItemKind4`
- **Named callers' callers:** `SqmdModel_LookupKind2`
- **Named callees' callees:** `sqDebugPrintf`

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind4(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,4,param_2);
  return uVar1;
}
```

## `0x10279B40` fn_10279B40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqmdModel_GetItemName`
- **Called by:** `sub_10278710`, `sub_102787F0`, `sub_1027A4B0`, `sub_1028CBE0`, `sub_1028D2C0`, `sub_1028D350`, `sub_1028D530`
- **Named before** (a naming session's reading, low): `SqmdModel_GetItemNameKind3`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `untitled`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `sub_1026ECC0` → `fn_102736E0` → `sub_10278710` → `SqmdModel_GetItemNameKind3`

```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemNameKind3(int param_1, int param_2)
{
  char *pcVar1;
  
  pcVar1 = orig::sqmdModel_GetItemName(param_1,3,param_2);
  return pcVar1;
}
```

## `0x10279D50` fn_10279D50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10279BF0`
- **Called by:** `sub_102787F0`, `sqmdScriptCmd_skeleton_shape`, `sub_102A0950`, `sub_102A12F0`, `sub_102A17B0`
- **Named before** (a naming session's reading, low): `SqmdModel_LookupKind2`; name it again from this dossier, or leave it out to keep that name
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `sub_1026ECC0` → `fn_102736E0` → `sub_10278710` → `sub_102787F0` → `SqmdModel_LookupKind2`
- **Named callees' callees:** `SqmdModel_GetItemKind0`, `SqmdModel_GetItemKind1`, `SqmdModel_GetItemKind2`, `SqmdModel_GetItemKind3`, `SqmdModel_GetItemKind4`, `sqDebugPrintf`, `sqScript_TokenEquals`

```cpp
XI_API undefined4 __cdecl SqmdModel_LookupKind2(undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279bf0(param_1,param_2,2,param_3);
  return uVar1;
}
```
