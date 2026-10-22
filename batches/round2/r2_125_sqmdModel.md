# Batch r2_125_sqmdModel: 1 functions of `sqmdModel`

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
