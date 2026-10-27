# Batch r3_034_sqmdModel: 11 functions of `sqmdModel`

Name these functions: see `batches/README.md`. Write `data/names/r3_034_sqmdModel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10277A50` fn_10277A50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x4
- **Called by:** `sub_10273810`, `fn_10278490`, `fn_10278990`, `sub_10278A20`, `sub_10278AC0`, `fn_10278B50`, `fn_10278BF0`, `sub_10278C80`, `sub_10278D90`, `fn_10278E10`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `Unit102706F0_UpdateScene` → `StAvatar_UpdateModels` → `fn_10278490` → `fn_10277A50`
- **Named callers' callers:** `StAvatar_UpdateModels`, `Mdl_Register`, `Mdl_SetSlotVisible`, `Unit10272F20_ApplySceneToModel`
- **How its callers call it** (their verified code):
  - in `fn_10278490`: `uVar1 = orig::FUN_10277a50(param_1);`
  - in `fn_10278990`: `uVar1 = orig::FUN_10277a50(param_1);`
  - in `fn_10278B50`: `uVar1 = orig::FUN_10277a50(param_1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 

```cpp
XI_API uint32_t __cdecl fn_10277A50(int param_1)
{
  uint32_t uVar1;
  uint64_t uVar2;
  
  uVar1 = *(uint32_t *)(param_1 + 0xa8);
  if ((uVar1 & 1) == 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x77);
    uVar2 = orig::sqDebugPrintf_v1(s_model_is_not_loaded__validation_f_103b3848);
    return (uint32_t)(uint32_t)((uint64_t)uVar2 >> 8) << 8;
  }
  if ((uVar1 & 2) == 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x7d);
    uVar2 = orig::sqDebugPrintf_v1(s_model_is_not_linked__validation_f_103b381c);
    return (uint32_t)(uint32_t)((uint64_t)uVar2 >> 8) << 8;
  }
  return ((uint32_t)(((uint32_t)((int32_t)(uVar1 >> 8)) << 8) | (uint8_t)(0xff)));
}
```

## `0x10278490` fn_10278490

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275090` x8, `fn_10277A50`, `SqmdModel_GetItemKind2` (low) x6, `SqmdModel_GetItemKind3` (low), `Unit102857D0_GetFlagByte14` (low) x6, `Unit102857D0_InvokeCallback18` (low) x6, `sqDebugPrintf` (high) x2, `Unit10291200_CallHook2C` (low), `Unit10291200_CallHook3C` (low)
- **Called by:** `StAvatar_UpdateModels` (low), `sub_10273810`
- **Writes globals:** `0x109F8270`, `0x109F8274`, `0x109F8278`, `0x109F827C`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `sqmdModelUpdate`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqTimer.c`, `Warning in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `Unit102706F0_UpdateScene` → `StAvatar_UpdateModels` → `fn_10278490`
- **Named callers' callers:** `Mdl_Register`, `Mdl_SetSlotVisible`, `Unit102706F0_UpdateScene`
- **How its callers call it** (their verified code):
  - in `StAvatar_UpdateModels` (low): `orig::FUN_10278490(*(int *)(iVar5 + 0x10));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_UpdateModels` (low): Updates the avatar's models and accumulates their values.
  - `SqmdModel_GetItemKind2` (low): Returns the item of kind 2 for the given id.
  - `SqmdModel_GetItemKind3` (low): Returns the item of kind 3 for the given id.
  - `Unit102857D0_GetFlagByte14` (low): Returns the object's byte field at +0x14.
  - `Unit102857D0_InvokeCallback18` (low): Calls the object's callback at +0x18 when one is set.
  - `sqDebugPrintf` (high): 
  - `Unit10291200_CallHook2C` (low): Runs the object's hook at +0x2C.
  - `Unit10291200_CallHook3C` (low): Runs the object's hook at +0x3C.

Short callee `SqmdModel_GetItemKind2` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind2(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,2,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemKind3` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind3(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,3,param_2);
  return uVar1;
}
```

Short callee `Unit102857D0_GetFlagByte14` (low):
```cpp
XI_API undefined4 __cdecl Unit102857D0_GetFlagByte14(int param_1)
{
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)param_1 >> 8)) << 8) | (uint8_t)(*(uint8_t *)(param_1 + 0x14))));
}
```

Short callee `Unit102857D0_InvokeCallback18` (low):
```cpp
XI_API undefined4 __cdecl Unit102857D0_InvokeCallback18(int param_1)
{
  uint32_t uVar1;
  
  if (*(code **)(param_1 + 0x18) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x102857ff. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(param_1 + 0x18))();
    return uVar1;
  }
  return 0;
}
```

```cpp
XI_API void __cdecl fn_10278490(int param_1)
{
  uint32_t uVar1;
  int iVar2;
  int *piVar3;
  uint32_t uVar4;
  int iVar5;
  int iVar6;
  
  uVar1 = orig::FUN_10277a50(param_1);
  if ((char)uVar1 == '\0') {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x1dc)
    ;
    orig::sqDebugPrintf_v1(s_aborting___s__command__103b3ae4,s_sqmdModelUpdate_103b3afc);
    return;
  }
  if ((*(uint8_t *)(param_1 + 0xa8) & 4) != 0) {
    iVar2 = orig::FUN_10275090();
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if (((char)uVar4 != '\0') && (*piVar3 == 0x130001)) {
          orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if ((char)uVar4 != '\0') {
          iVar5 = *piVar3;
          if (iVar5 < 0x150002) {
            if (iVar5 != 0x150001) {
              if (iVar5 == 0xb0001) {
                if (piVar3[0xb] != -1) {
                  if (*(int **)(piVar3[7] + 0x68) != (int *)0x0) {
                    piVar3 = *(int **)(piVar3[7] + 0x68);
                  }
                  goto LAB_10278576;
                }
              }
              else if (iVar5 != 0x130001) {
LAB_10278576:
                orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
              }
            }
          }
          else if ((iVar5 < 0x1c0002) || (0x1c0003 < iVar5)) goto LAB_10278576;
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if ((((char)uVar4 != '\0') && (*piVar3 == 0x1c0002)) && (piVar3[7] == 2)) {
          orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if (((char)uVar4 != '\0') && (*piVar3 == 0x150001)) {
          orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    iVar6 = orig::FUN_10275090();
    DAT_109f8270_as_s32 = iVar6 - iVar2;
    iVar2 = orig::FUN_10275090();
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x58)) {
      do {
        if ((*(uint32_t *)(*(int *)(param_1 + 0x54) + iVar6 * 4) & 0x400) != 0) {
          iVar5 = orig::SqmdModel_GetItemKind3(param_1,iVar6);
          orig::Unit10291200_CallHook2C(iVar5);
          if ((*(uint32_t *)(*(int *)(param_1 + 0x54) + iVar6 * 4) & 0x1000) != 0) {
            orig::Unit10291200_CallHook3C(iVar5);
          }
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x58));
    }
    iVar6 = orig::FUN_10275090();
    DAT_109f8274 = iVar6 - iVar2;
    iVar2 = orig::FUN_10275090();
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if ((((char)uVar4 != '\0') && (*piVar3 == 0x1c0002)) && (piVar3[7] != 2)) {
          orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    _DAT_109f8278 = orig::FUN_10275090();
    _DAT_109f8278 = _DAT_109f8278 - iVar2;
    iVar2 = orig::FUN_10275090();
    iVar6 = 0;
    if (0 < *(int *)(param_1 + 0x44)) {
      do {
        piVar3 = (int *)orig::SqmdModel_GetItemKind2(param_1,iVar6);
        uVar4 = orig::Unit102857D0_GetFlagByte14((int)piVar3);
        if (((char)uVar4 != '\0') && (*piVar3 == 0x1c0003)) {
          orig::Unit102857D0_InvokeCallback18_v1((int)piVar3);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < *(int *)(param_1 + 0x44));
    }
    _DAT_109f827c = orig::FUN_10275090();
    _DAT_109f827c = _DAT_109f827c - iVar2;
  }
  return;
}
```

## `0x10278710` fn_10278710

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102787F0` x2, `SqmdModel_GetItemNameKind3` (low) x2, `sqDebugPrintf` (high) x2, `sub_10312940` x2
- **Called by:** `Unit10272F20_ApplySceneToModel` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `eyelash`, `link`
- **Strings its callees refer to:** `%s_sort`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278710`
- **Named callers' callers:** `Mdl_Register`
- **Named callees' callees:** `SqmdModel_GetItemKind1`, `SqmdModel_GetItemKind3`, `sqmdModel_GetItemName`, `SqmdModel_LookupKind2`, `SqsnScene_LayerAddItem`, `SqsnScene_ShadowAddMember`
- **How its callers call it** (their verified code):
  - in `Unit10272F20_ApplySceneToModel` (low): `orig::FUN_10278710(param_1,&DAT_1099b13c,param_2,0xff);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10272F20_ApplySceneToModel` (low): Applies the avatar scene's layer and lights to a model.
  - `SqmdModel_GetItemNameKind3` (low): Returns the name of the kind-3 item for the given id.
  - `sqDebugPrintf` (high): 

Short callee `SqmdModel_GetItemNameKind3` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemNameKind3(int param_1, int param_2)
{
  char *pcVar1;
  
  pcVar1 = orig::sqmdModel_GetItemName(param_1,3,param_2);
  return pcVar1;
}
```

```cpp
XI_API void __cdecl fn_10278710(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  char *pcVar1;
  int iVar2;
  uint64_t uVar3;
  char *pcVar4;
  size_t sVar5;
  
  if ((*(uint8_t *)(param_1 + 0xa8) & 1) == 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x286)
    ;
    orig::sqDebugPrintf_v1(s_model_is_not_loaded__aborting____103b3b14,&DAT_103b3b44);
    return;
  }
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x58)) {
    do {
      if ((*(uint32_t *)(*(int *)(param_1 + 0x54) + iVar2 * 4) & 0x2000) == 0) {
        sVar5 = 7;
        pcVar4 = s_eyelash_103b3b0c;
        pcVar1 = (char *)orig::SqmdModel_GetItemNameKind3(param_1,iVar2);
        uVar3 = orig::_strncmp(pcVar1,pcVar4,sVar5);
        if ((int)uVar3 != 0) {
          orig::FUN_102787f0(param_1,iVar2,param_2,param_3,param_4);
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x58));
  }
  iVar2 = 0;
  if (0 < *(int *)(param_1 + 0x58)) {
    do {
      if ((*(uint32_t *)(*(int *)(param_1 + 0x54) + iVar2 * 4) & 0x2000) == 0) {
        sVar5 = 7;
        pcVar4 = s_eyelash_103b3b0c;
        pcVar1 = (char *)orig::SqmdModel_GetItemNameKind3(param_1,iVar2);
        uVar3 = orig::_strncmp(pcVar1,pcVar4,sVar5);
        if ((int)uVar3 == 0) {
          orig::FUN_102787f0(param_1,iVar2,param_2,param_3,param_4);
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x58));
  }
  return;
}
```

## `0x102787F0` fn_102787F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10275E90` x2, `SqmdModel_GetItemKind1` (low), `SqmdModel_GetItemKind3` (low), `SqmdModel_GetItemNameKind3` (low), `SqmdModel_LookupKind2` (low), `sqDebugPrintf` (high) x2, `SqsnScene_LayerAddItem` (low), `SqsnScene_ShadowAddMember` (low), `sub_10311FF1`
- **Called by:** `fn_10278710`
- **Strings it refers to** (DLL bytes): `%s_sort`, `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `link`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqScene\src\sqsnScene.c`, `VALID`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278710` → `fn_102787F0`
- **Named callers' callers:** `Unit10272F20_ApplySceneToModel`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_HexDump`, `SqMem_FindArena`, `sqFree`, `sqmdModel_GetItemName`, `SqsnScene_LayerInit`
- **How its callers call it** (their verified code):
  - in `fn_10278710`: `orig::FUN_102787f0(param_1,iVar2,param_2,param_3,param_4);`
  - in `fn_10278710`: `orig::FUN_102787f0(param_1,iVar2,param_2,param_3,param_4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmdModel_GetItemKind1` (low): Returns the item of kind 1 for the given id.
  - `SqmdModel_GetItemKind3` (low): Returns the item of kind 3 for the given id.
  - `SqmdModel_GetItemNameKind3` (low): Returns the name of the kind-3 item for the given id.
  - `SqmdModel_LookupKind2` (low): Looks up a kind-2 item for the given arguments.
  - `sqDebugPrintf` (high): 
  - `SqsnScene_LayerAddItem` (low): Adds an item to a scene layer, creating the layer if needed, and returns its id.
  - `SqsnScene_ShadowAddMember` (low): Adds a member to a shadow entry's list.

Short callee `SqmdModel_GetItemKind1` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind1(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,1,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemKind3` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind3(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,3,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemNameKind3` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemNameKind3(int param_1, int param_2)
{
  char *pcVar1;
  
  pcVar1 = orig::sqmdModel_GetItemName(param_1,3,param_2);
  return pcVar1;
}
```

Short callee `SqmdModel_LookupKind2` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_LookupKind2(undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279bf0(param_1,param_2,2,param_3);
  return uVar1;
}
```

```cpp
XI_API void __cdecl fn_102787F0(int param_1, int param_2, int param_3, undefined4 param_4, undefined4 param_5)
{
  uint32_t uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  char xi_frame[264] __attribute__((aligned(4)));
  int  &local_108 = *reinterpret_cast<int  *>(xi_frame + 0);
  int  &local_104 = *reinterpret_cast<int  *>(xi_frame + 4);
  uint8_t  (&local_100)[256] = *reinterpret_cast<uint8_t  (*)[256]>(xi_frame + 8);
  
  if ((*(uint8_t *)(param_1 + 0xa8) & 1) == 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x2bf)
    ;
    orig::sqDebugPrintf_v1(s_model_is_not_loaded__aborting____103b3b14,&DAT_103b3b44);
    return;
  }
  local_104 = *(int *)(param_1 + 0xa4);
  if (*(int *)(param_1 + 0x5c) <= local_104) {
    uVar1 = local_104 * 4 + 4;
    puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0x9c),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x2c8);
    *(undefined4 **)(param_1 + 0x9c) = puVar2;
    puVar2 = orig::FUN_10275e90(*(undefined4 **)(param_1 + 0xa0),uVar1,
                          s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x2ca);
    *(undefined4 **)(param_1 + 0xa0) = puVar2;
  }
  uVar3 = orig::SqmdModel_GetItemKind1(param_1,*(undefined4 *)(*(int *)(param_1 + 0x98) + param_2 * 4));
  uVar6 = *(undefined4 *)(*(int *)(param_1 + 0x50) + param_2 * 4);
  uVar4 = orig::SqmdModel_GetItemKind3(param_1,param_2);
  iVar5 = orig::SqsnScene_LayerAddItem(param_3,uVar4,uVar6,uVar3,param_4);
  if ((char)param_5 != '\0') {
    if ((*(uint8_t *)(*(int *)(param_1 + 0x54) + param_2 * 4) & 0x40) != 0) {
      local_108 = 0;
      if (0 < *(int *)(param_3 + 0x34)) {
        do {
          orig::SqsnScene_ShadowAddMember(param_3,local_108,iVar5);
          local_108 = local_108 + 1;
        } while (local_108 < *(int *)(param_3 + 0x34));
      }
    }
    if ((*(uint32_t *)(*(int *)(param_1 + 0x54) + param_2 * 4) & 0x4000) != 0) {
      uVar6 = orig::SqmdModel_GetItemNameKind3(param_1,param_2);
      orig::_sprintf_v1(local_100,s__s_sort_103b3b4c,uVar6);
      iVar7 = orig::SqmdModel_LookupKind2(param_1,local_100,&local_108);
      *(int *)(iVar7 + 0x2c) = param_3;
    }
  }
  *(int *)(*(int *)(param_1 + 0x9c) + local_104 * 4) = iVar5;
  *(int *)(*(int *)(param_1 + 0xa0) + local_104 * 4) = param_2;
  *(int *)(param_1 + 0xa4) = *(int *)(param_1 + 0xa4) + 1;
  *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) | 2;
  return;
}
```

## `0x10278990` fn_10278990

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10277A50`, `sqDebugPrintf` (high) x2, `fn_1028F2E0`
- **Called by:** `Unit10272F20_ApplySceneToModel` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `sqmdModelAddIllumLight`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqScene\src\sqsnScene.c`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278990`
- **Named callers' callers:** `Mdl_Register`
- **How its callers call it** (their verified code):
  - in `Unit10272F20_ApplySceneToModel` (low): `orig::FUN_10278990(param_1,0x1099b13c,*piVar1,uVar2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10272F20_ApplySceneToModel` (low): Applies the avatar scene's layer and lights to a model.
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_10278990(int param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  uint32_t uVar1;
  int iVar2;
  
  uVar1 = orig::FUN_10277a50(param_1);
  if ((char)uVar1 != '\0') {
    iVar2 = 0;
    if (0 < *(int *)(param_1 + 0xa4)) {
      do {
        if (((char)param_4 == '\0') ||
           (-1 < *(char *)(*(int *)(param_1 + 0x54) +
                          *(int *)(*(int *)(param_1 + 0xa0) + iVar2 * 4) * 4))) {
          orig::FUN_1028f2e0(param_2,*(uint32_t *)(*(int *)(param_1 + 0x9c) + iVar2 * 4),param_3);
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 < *(int *)(param_1 + 0xa4));
    }
    return;
  }
  orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x301);
  orig::sqDebugPrintf_v1(s_aborting___s__command__103b3ae4,s_sqmdModelAddIllumLight_103b3b54);
  return;
}
```

## `0x10278B50` fn_10278B50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10277A50`, `sqDebugPrintf` (high) x2, `fn_1028F350`
- **Called by:** `Unit10272F20_ApplySceneToModel` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `sqmdModelAddShadowLightInLayer`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqScene\src\sqsnScene.c`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278B50`
- **Named callers' callers:** `Mdl_Register`
- **How its callers call it** (their verified code):
  - in `Unit10272F20_ApplySceneToModel` (low): `orig::FUN_10278b50(param_1,&DAT_1099b13c,param_2,DAT_1099b100,0xff);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10272F20_ApplySceneToModel` (low): Applies the avatar scene's layer and lights to a model.
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_10278B50(int param_1, undefined4 param_2, int param_3, undefined4 param_4, undefined4 param_5)
{
  uint32_t uVar1;
  int iVar2;
  
  uVar1 = orig::FUN_10277a50(param_1);
  if ((char)uVar1 != '\0') {
    iVar2 = 0;
    if (0 < *(int *)(param_1 + 0xa4)) {
      do {
        if ((((char)param_5 == '\0') ||
            (*(char *)(*(int *)(param_1 + 0x54) + *(int *)(*(int *)(param_1 + 0xa0) + iVar2 * 4) * 4
                      ) < '\0')) &&
           (uVar1 = *(uint32_t *)(*(int *)(param_1 + 0x9c) + iVar2 * 4), (int)uVar1 >> 0x10 == param_3))
        {
          orig::FUN_1028f350(param_2,uVar1,param_4);
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 < *(int *)(param_1 + 0xa4));
    }
    return;
  }
  orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x382);
  orig::sqDebugPrintf_v1(s_aborting___s__command__103b3ae4,s_sqmdModelAddShadowLightInLayer_103b3ba4);
  return;
}
```

## `0x10278BF0` fn_10278BF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10277A50`, `sqDebugPrintf` (high), `SqsnScene_GetEntryAddr` (low)
- **Called by:** `sub_10273810`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `sqmdModelShow`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `sub_10273810` → `fn_10278BF0`
- **Named callers' callers:** `Mdl_Register`, `Mdl_SetSlotVisible`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 
  - `SqsnScene_GetEntryAddr` (low): Returns the address of a scene entry from a packed index.

Short callee `SqsnScene_GetEntryAddr` (low):
```cpp
XI_API int __cdecl SqsnScene_GetEntryAddr(int param_1, uint32_t param_2)
{
  return (param_2 & 0x7fff) * 0x90 +
         *(int *)(*(int *)(param_1 + 0x48) + 0x28 + ((int)param_2 >> 0x10) * 0x2c);
}
```

```cpp
XI_API void __cdecl fn_10278BF0(int param_1, undefined4 param_2)
{
  uint32_t uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = orig::FUN_10277a50(param_1);
  if ((char)uVar1 != '\0') {
    iVar3 = 0;
    if (0 < *(int *)(param_1 + 0xa4)) {
      do {
        iVar2 = orig::SqsnScene_GetEntryAddr(param_2,*(uint32_t *)(*(int *)(param_1 + 0x9c) + iVar3 * 4));
        *(uint8_t *)(iVar2 + 0x1d) = 0xff;
        *(uint8_t *)(iVar2 + 0x1c) = 0xff;
        iVar3 = iVar3 + 1;
      } while (iVar3 < *(int *)(param_1 + 0xa4));
    }
    *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) | 4;
    return;
  }
  orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x3b0);
  orig::sqDebugPrintf_v1(s_aborting___s__command__103b3ae4);
  return;
}
```

## `0x10278E10` fn_10278E10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10277A50`, `sqDebugPrintf` (high)
- **Called by:** `Unit10272F20_ApplySceneToModel` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `, `sqmdModelSetLayerSort`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278E10`
- **Named callers' callers:** `Mdl_Register`
- **How its callers call it** (their verified code):
  - in `Unit10272F20_ApplySceneToModel` (low): `orig::FUN_10278e10(param_1,(uint32_t)(param_2 == 2));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10272F20_ApplySceneToModel` (low): Applies the avatar scene's layer and lights to a model.
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_10278E10(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10277a50(param_1);
  if ((char)uVar1 == '\0') {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x415)
    ;
    orig::sqDebugPrintf_v1(s_aborting___s__command__103b3ae4);
    return;
  }
  if (param_2 == -1) {
    *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) ^ 0x8000;
    return;
  }
  if (param_2 != 0) {
    *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) | 0x8000;
    return;
  }
  *(uint32_t *)(param_1 + 0xa8) = *(uint32_t *)(param_1 + 0xa8) & 0xffff7fff;
  return;
}
```

## `0x10279710` fn_10279710

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1027A1C0`, `sqDebugPrintf` (high) x6
- **Called by:** `SqmdModel_GetItemKind0` (low), `SqmdModel_GetItemKind1` (low), `SqmdModel_GetItemKind2` (low), `SqmdModel_GetItemKind3` (low), `SqmdModel_GetItemKind4` (low), `sub_102798D0`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **Strings its callees refer to:** `matrix`, `shader`, `shape`, `texture`, `undefined`, `updater`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `StAvatar_UpdateInput` → `StAvatar_UpdateCameraFollow` → `SqmdModel_GetItemKind2` → `fn_10279710`
- **Named callers' callers:** `StAvatar_UpdateCameraFollow`, `StAvatar_GetOffset`, `Unit10252A00_SetPositionAndScale`, `Unit10252A00_SetVec78`, `Mdl_FindNthItemB0001`, `sqmdModel_Dump`, `SqmdModel_LookAt`
- **How its callers call it** (their verified code):
  - in `SqmdModel_GetItemKind0` (low): `uVar1 = orig::FUN_10279710(param_1,0,param_2);`
  - in `SqmdModel_GetItemKind1` (low): `uVar1 = orig::FUN_10279710(param_1,1,param_2);`
  - in `SqmdModel_GetItemKind2` (low): `uVar1 = orig::FUN_10279710(param_1,2,param_2);`
  - in `SqmdModel_GetItemKind3` (low): `uVar1 = orig::FUN_10279710(param_1,3,param_2);`
  - in `SqmdModel_GetItemKind4` (low): `uVar1 = orig::FUN_10279710(param_1,4,param_2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmdModel_GetItemKind0` (low): Returns the item of kind 0 for the given id.
  - `SqmdModel_GetItemKind1` (low): Returns the item of kind 1 for the given id.
  - `SqmdModel_GetItemKind2` (low): Returns the item of kind 2 for the given id.
  - `SqmdModel_GetItemKind3` (low): Returns the item of kind 3 for the given id.
  - `SqmdModel_GetItemKind4` (low): Returns the item of kind 4 for the given id.
  - `sqDebugPrintf` (high): 

```cpp
XI_API undefined4 __cdecl fn_10279710(int param_1, undefined4 param_2, int param_3)
{
  char *pcVar1;
  
  if (param_3 < 0) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x5a4)
    ;
    orig::sqDebugPrintf_v1(s_negative_array_index__d__103b3d54,param_3);
    return 0;
  }
  switch(param_2) {
  case 0:
    if (param_3 < *(int *)(param_1 + 0x24)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x1c) + param_3 * 4);
    }
    break;
  case 1:
    if (param_3 < *(int *)(param_1 + 0x34)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x2c) + param_3 * 4);
    }
    break;
  case 2:
    if (param_3 < *(int *)(param_1 + 0x44)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x3c) + param_3 * 4);
    }
    break;
  case 3:
    if (param_3 < *(int *)(param_1 + 0x58)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x4c) + param_3 * 4);
    }
    break;
  case 4:
    if (param_3 < *(int *)(param_1 + 0x68)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x60) + param_3 * 4);
    }
    break;
  case 5:
    if (param_3 < *(int *)(param_1 + 0x58)) {
      return *(uint32_t *)(*(int *)(param_1 + 0x50) + param_3 * 4);
    }
    break;
  default:
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x5a9)
    ;
    orig::sqDebugPrintf_v1(s_illegal_component_type__d_103b3cfc,param_2);
    return 0;
  }
  orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x5c4);
  pcVar1 = orig::FUN_1027a1c0(param_2);
  orig::sqDebugPrintf_v1(s_index__d_is_larger_that_number_o_103b3d18,param_3,pcVar1);
  return 0;
}
```

## `0x10279BF0` fn_10279BF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqmdModel_GetItemKind0` (low), `SqmdModel_GetItemKind1` (low), `SqmdModel_GetItemKind2` (low), `SqmdModel_GetItemKind3` (low), `SqmdModel_GetItemKind4` (low), `fn_1027A1C0`, `sqDebugPrintf` (high) x4, `sqScript_TokenEquals`
- **Called by:** `sub_10279D10`, `sub_10279D30`, `SqmdModel_LookupKind2` (low), `sub_10279D70`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdModel.c`, `Error in %s, line %d: `
- **Strings its callees refer to:** `matrix`, `shader`, `shape`, `texture`, `undefined`, `updater`
- **From the frame root** (shortest chain of direct calls): `StAvatar_Teardown` → `StAvatar_Reset` → `fn_1026ECC0` → `Unit10272F20_ApplySceneToModel` → `fn_10278710` → `fn_102787F0` → `SqmdModel_LookupKind2` → `fn_10279BF0`
- **Named callers' callers:** `sqsdShader_ParseScript`, `sqmdScriptCmd_skeleton_shape`
- **How its callers call it** (their verified code):
  - in `SqmdModel_LookupKind2` (low): `uVar1 = orig::FUN_10279bf0(param_1,param_2,2,param_3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqmdModel_GetItemKind0` (low): Returns the item of kind 0 for the given id.
  - `SqmdModel_GetItemKind1` (low): Returns the item of kind 1 for the given id.
  - `SqmdModel_GetItemKind2` (low): Returns the item of kind 2 for the given id.
  - `SqmdModel_GetItemKind3` (low): Returns the item of kind 3 for the given id.
  - `SqmdModel_GetItemKind4` (low): Returns the item of kind 4 for the given id.
  - `SqmdModel_LookupKind2` (low): Looks up a kind-2 item for the given arguments.
  - `sqDebugPrintf` (high): 

Short callee `SqmdModel_GetItemKind0` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind0(int param_1, int param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,0,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemKind1` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind1(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,1,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemKind2` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind2(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,2,param_2);
  return uVar1;
}
```

Short callee `SqmdModel_GetItemKind3` (low):
```cpp
XI_API undefined4 __cdecl SqmdModel_GetItemKind3(undefined4 param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10279710(param_1,3,param_2);
  return uVar1;
}
```

```cpp
XI_API undefined4 __cdecl fn_10279BF0(int param_1, int param_2, int param_3, undefined4 * param_4)
{
  int iVar1;
  char *pcVar2;
  uint32_t uVar3;
  int iVar4;
  
  if (param_2 != 0) {
    iVar4 = 0;
    if (0 < *(int *)(param_1 + 0x7c)) {
      do {
        if ((*(int *)(*(int *)(param_1 + 0x74) + iVar4 * 4) == param_3) &&
           (iVar1 = orig::sqScript_TokenEquals
                              ((char *)param_2,*(char **)(*(int *)(param_1 + 0x70) + iVar4 * 4)),
           iVar1 == 0)) {
          iVar4 = *(int *)(*(int *)(param_1 + 0x78) + iVar4 * 4);
          *param_4 = iVar4;
          switch(param_3) {
          case 0:
            uVar3 = orig::SqmdModel_GetItemKind0(param_1,iVar4);
            return uVar3;
          case 1:
            uVar3 = orig::SqmdModel_GetItemKind1(param_1,iVar4);
            return uVar3;
          case 2:
            uVar3 = orig::SqmdModel_GetItemKind2(param_1,iVar4);
            return uVar3;
          case 3:
            uVar3 = orig::SqmdModel_GetItemKind3(param_1,iVar4);
            return uVar3;
          case 4:
            uVar3 = orig::SqmdModel_GetItemKind4(param_1,iVar4);
            return uVar3;
          default:
            orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,
                         s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x6ef);
            orig::sqDebugPrintf_v1(s_illegal_tag_type__d_103b3d7c,param_3);
            return 0;
          }
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < *(int *)(param_1 + 0x7c));
    }
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103b37ec,0x6ff)
    ;
    pcVar2 = orig::FUN_1027a1c0(param_3);
    orig::sqDebugPrintf_v1(s_object___s__of_type__s_not_found_103b3d94,param_2,pcVar2);
  }
  return 0;
}
```

## `0x1027A1C0` fn_1027A1C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10279710`, `fn_10279BF0`, `sqsdShader_ParseScript`, `sub_1029FA50`, `sqmdScriptCmd_skeleton_shape`, `sub_102A0950`, `sub_102A0F90`, `sub_102A12F0`, `sub_102A17B0`, `sub_102A1B00`
- **Strings it refers to** (DLL bytes): `matrix`, `shader`, `shape`, `texture`, `undefined`, `updater`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `StAvatar_UpdateInput` → `StAvatar_UpdateCameraFollow` → `SqmdModel_GetItemKind2` → `fn_10279710` → `fn_1027A1C0`
- **Named callers' callers:** `SqmdModel_GetItemKind0`, `SqmdModel_GetItemKind1`, `SqmdModel_GetItemKind2`, `SqmdModel_GetItemKind3`, `SqmdModel_GetItemKind4`, `SqmdModel_LookupKind2`
- **How its callers call it** (their verified code):
  - in `fn_10279710`: `pcVar1 = orig::FUN_1027a1c0(param_2);`
  - in `fn_10279BF0`: `pcVar2 = orig::FUN_1027a1c0(param_3);`

```cpp
XI_API char * __cdecl fn_1027A1C0(undefined4 param_1)
{
  switch(param_1) {
  case 0:
    return s_texture_103b3ee4;
  case 1:
    return s_shader_103b3edc;
  case 2:
    return s_updater_103b3ed4;
  case 3:
    return s_shape_103b3ecc;
  case 4:
    return(char*)(&DAT_1038a350);
  case 5:
    return s_matrix_103b3ec4;
  default:
    return s_undefined_103b3eb8;
  }
}
```
