# Batch r3_028_StModel: 2 functions of `StModel`

Name these functions: see `batches/README.md`. Write `data/names/r3_028_StModel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10252610` fn_10252610

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit10017C10_NoOpVarargsA` (medium) x6, `Unit1018BA60_FileMngQueueFileNumKind2` (low) x2
- **Called by:** `StAvatar_Teardown`, `StAvatar_Unload` (medium)
- **Reads globals:** `0x10662DCC`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pFileMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `fn_10252610`
- **Named callers' callers:** `StAvatar_SetPending`, `StAvatar_SetStateTeardown`
- **How its callers call it** (their verified code):
  - in `StAvatar_Teardown`: `orig::FUN_10252610((int)(param_1 + 0x2a),uVar1);`
  - in `StAvatar_Teardown`: `orig::FUN_10252610((int)(param_1 + 0x33),*(int *)(((char *)&DAT_10389538) + param_2 * 4) + 4 + param_3 * 2);`
  - in `StAvatar_Unload` (medium): `orig::FUN_10252610((int)(param_1 + 0x33),*(int *)(((char *)&DAT_10389538) + param_1[2] * 4) + 4 + param_1[3] * 2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10017C10_NoOpVarargsA` (medium): Takes any arguments and does nothing.
  - `Unit1018BA60_FileMngQueueFileNumKind2` (low): Has the file manager queue a kind-2 request for a file number.
  - `StAvatar_Unload` (medium): Releases an avatar's pose data and resets it to the unloaded state.

Short callee `Unit10017C10_NoOpVarargsA` (medium):
```cpp
XI_API void __cdecl Unit10017C10_NoOpVarargsA(undefined4 xi_s1, ...)
{
  return;
}
```

Short callee `Unit1018BA60_FileMngQueueFileNumKind2` (low):
```cpp
XI_API undefined4 __thiscall Unit1018BA60_FileMngQueueFileNumKind2(void * self_, uint32_t param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, int param_5, uint8_t * param_6)
{
  int iVar1;
  
  if (param_6 == (uint8_t *)0x0) {
    param_6 =(uint8_t*)(&LAB_101c4d50);
  }
  iVar1 = orig::Unit1018BA60_QueueFileNumKind2(*(void **)((int)self_ + 4),param_1,param_2,param_3,param_4,param_5,
                       (int)param_6);
  return iVar1;
}
```

```cpp
XI_API undefined4 __cdecl fn_10252610(int param_1, undefined4 param_2)
{
  int iVar1;
  
  if (((*(int *)(param_1 + 0x18) != 0) || (*(int *)(param_1 + 0x1c) != 0)) ||
     (*(int *)(param_1 + 0x20) != 0)) {
    orig::Unit10017C10_NoOpVarargsA_v1(s_____Error__StModelItemReadStart__10389b28);
    orig::Unit10017C10_NoOpVarargsA_v1(g_10389B0C,*(uint32_t *)(param_1 + 0x18));
    orig::Unit10017C10_NoOpVarargsA_v1(g_10389AF4,*(uint32_t *)(param_1 + 0x1c));
    orig::Unit10017C10_NoOpVarargsA_v1(g_10389ADC,*(uint32_t *)(param_1 + 0x20));
    return 0xffffffff;
  }
  *(int *)(param_1 + 4) = param_2;
  iVar1 = Unit1018BA60_FileMngQueueFileNumKind2(g_pFileMng_as_v1,param_2,0,0,param_1,&LAB_10252840,XI_CODE(FUN_102526f0));
  if (iVar1 < 0) {
    orig::Unit10017C10_NoOpVarargsA_v1(s_____Error__DMB_file_not_found____10389b8c,param_2);
    return 0xffffffff;
  }
  iVar1 = Unit1018BA60_FileMngQueueFileNumKind2(g_pFileMng_as_v1,param_2 + 1,0,0,param_1,&LAB_10252880,XI_CODE(FUN_10252790));
  if (iVar1 < 0) {
    orig::Unit10017C10_NoOpVarargsA_v1(s_____Error__DMP_file_not_found____10389b68,param_2 + 1);
    return 0xffffffff;
  }
  return 0;
}
```

## `0x102528D0` fn_102528D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit10017C10_NoOpVarargsA` (medium) x4, `Unit1018BA60_FileMngReleaseAt1F0` (low), `Unit1018BA60_FileMngGetValueAt1F0` (low), `sub_10275090` x2, `sqFree` x3, `sqDebugPrintf` (high), `fn_10287FB0`
- **Called by:** `sub_1024D850`
- **Reads globals:** `0x10662DCC`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pFileMng`
- **Strings it refers to** (DLL bytes): `D:\build0001\FFXi_Win\Main\dancer\StModel.cpp`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqTimer.c`, `VALID`, `Warning in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `fn_102528D0`
- **Named callers' callers:** `StAvatar_StopAndFreeBuffers`, `StAvatar_Update`
- **Named callees' callees:** `SqMem_ClassifyBlock`, `SqMem_GetPayloadEnd`, `SqMem_HexDump`, `SqMem_FindArena`, `SqmdDMB_TraceDelta`, `SqmdDMB_AlignMem`, `SqmdDMB_ResolveMatrices`, `SqmdDMB_ResolveStrings`, `SqmdDMB_ClearTables`, `SqmdDMB_ResetCounts`, `sqmdDMBBuildShapes`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10017C10_NoOpVarargsA` (medium): Takes any arguments and does nothing.
  - `Unit1018BA60_FileMngReleaseAt1F0` (low): Releases the file manager's inner +0x1F0 object.
  - `Unit1018BA60_FileMngGetValueAt1F0` (low): Calls the file manager's inner +0x1F0 getter.
  - `sqDebugPrintf` (high): 

Short callee `Unit10017C10_NoOpVarargsA` (medium):
```cpp
XI_API void __cdecl Unit10017C10_NoOpVarargsA(undefined4 xi_s1, ...)
{
  return;
}
```

Short callee `Unit1018BA60_FileMngReleaseAt1F0` (low):
```cpp
XI_API void __thiscall Unit1018BA60_FileMngReleaseAt1F0(void * self_)
{
  orig::Unit1018BA60_ReleaseAt1F0(*(void **)((int)self_ + 4));
  return;
}
```

Short callee `Unit1018BA60_FileMngGetValueAt1F0` (low):
```cpp
XI_API void __thiscall Unit1018BA60_FileMngGetValueAt1F0(void * self_)
{
  orig::Unit1018BA60_GetValueAt1F0(*(void **)((int)self_ + 4));
  return;
}
```

```cpp
XI_API undefined4 __cdecl fn_102528D0(uint32_t * param_1)
{
  uint32_t uVar1;
  int iVar2;
  
  if ((param_1 != (uint32_t *)0x0) && ((*param_1 & 4) != 0)) {
    orig::Unit10017C10_NoOpVarargsA_v1(s_____StModelItemResolve____called_10389c28);
    return 0xffffffff;
  }
  if ((param_1[6] != 0) && (param_1[7] != 0)) {
    uVar1 = orig::FUN_10275090();
    param_1[5] = uVar1;
    orig::Unit1018BA60_FileMngReleaseAt1F0(g_pFileMng_as_v1);
    uVar1 = orig::FUN_10287fb0((char *)param_1[6],param_1[7]);
    param_1[8] = uVar1;
    orig::Unit1018BA60_FileMngGetValueAt1F0(g_pFileMng_as_v1);
    iVar2 = orig::FUN_10275090();
    param_1[5] = iVar2 - param_1[5];
    if (param_1[8] == 0) {
      orig::sqDebugPrintf_v1(s_____Error__sqmdDMBResolve__p___p_10389bfc,param_1[6],param_1[7]);
      param_1[3] = 0;
      param_1[4] = 0;
      uVar1 = orig::sqFree_v1(param_1[6],s_D__build0001_FFXi_Win_Main_dance_10389a74,0x152);
      param_1[6] = uVar1;
      uVar1 = orig::sqFree_v1(param_1[7],s_D__build0001_FFXi_Win_Main_dance_10389a74,0x153);
      param_1[7] = uVar1;
      return 0xffffffff;
    }
    param_1[6] = 0;
    uVar1 = orig::sqFree_v1(param_1[7],s_D__build0001_FFXi_Win_Main_dance_10389a74,0x159);
    param_1[7] = uVar1;
    *param_1 = *param_1 & 0xfffffffd | 4;
    return 0;
  }
  orig::Unit10017C10_NoOpVarargsA_v1(s_____StModelItemResolve____called_10389bc8);
  orig::Unit10017C10_NoOpVarargsA_v1(g_10389B0C,param_1[6]);
  orig::Unit10017C10_NoOpVarargsA_v1(g_10389AF4,param_1[7]);
  return 0xffffffff;
}
```
