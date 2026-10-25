# Batch r3_038_sqshBuild: 3 functions of `sqshBuild`

Name these functions: see `batches/README.md`. Write `data/names/r3_038_sqshBuild.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10299C30` fn_10299C30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqMem_AllocZeroed` (medium), `sqDebugPrintf` (high) x2, `Unit1028A0A0_ReadBlock` (medium) x4, `sub_1028DA20`, `SqshBuild_Read` (medium), `SqVtx_PackFloatsToBytes` (low), `SqshTriStrip_Create` (medium), `SqshTriStrip_GetGlobal` (low), `sub_102ABEC0`
- **Called by:** `fn_10299B30`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqShape\src\sqshBuild.c`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqBase\src\sqVtx.c`, `C:\dev\dancer\modules\sqShape\src\sqshTriStrip.c`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `sqmdDMBBuildShapes` → `fn_10299B30` → `fn_10299C30`
- **Named callers' callers:** `sqmdDMBBuildShapes`
- **Named callees' callees:** `sqFree`, `Unit1028A0A0_ReturnZero`, `Unit10292470_SwapNibblesXor`, `SqVtx_Create`, `SqshTriStrip_Init`
- **How its callers call it** (their verified code):
  - in `fn_10299B30`: `iVar3 = orig::FUN_10299c30(puVar2,uVar4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqMem_AllocZeroed` (medium): Allocates a zeroed block of count times size bytes.
  - `sqDebugPrintf` (high): 
  - `Unit1028A0A0_ReadBlock` (medium): Reads a block from a stream into a buffer and returns the position after it.
  - `SqshBuild_Read` (medium): Reads a shape build block from a stream, vertex by vertex.
  - `SqVtx_PackFloatsToBytes` (low): Packs float channel values into bytes and replaces the stored buffer.
  - `SqshTriStrip_Create` (medium): Creates a sqshTriStrip object.
  - `SqshTriStrip_GetGlobal` (low): Returns the value of a global shape-strip setting.

Short callee `SqshTriStrip_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqshTriStrip_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::SqMem_AllocZeroed(1,0x60,s_C__dev_dancer_modules_sqShape_sr_103be980,0x28);
  orig::SqshTriStrip_Init(puVar1);
  return puVar1;
}
```

Short callee `SqshTriStrip_GetGlobal` (low):
```cpp
XI_API undefined4 __cdecl SqshTriStrip_GetGlobal(void)
{
  return DAT_109aaac0;
}
```

```cpp
XI_API int __cdecl fn_10299C30(undefined4 * param_1, int param_2)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  
  iVar2 = param_2;
  param_1 = orig::Unit1028A0A0_ReadBlock(&param_2,4,1,param_1,param_2);
  if (2 < param_2) {
    orig::sqDebugPrintf_v1(s_____Error__TriStrip_version_mism_103bc1d8,param_2,2);
    return 0;
  }
  param_1 = param_1 + 0x14;
  iVar3 = orig::SqshTriStrip_Create();
  if (iVar3 == 0) {
    orig::sqDebugPrintf_v1(s_____Error__sqshTriStripNew___cau_103bc1a4);
    return 0;
  }
  iVar4 = orig::SqshBuild_Read(param_1,&param_1,iVar2);
  if (iVar4 == 0) {
    return 0;
  }
  piVar1 = (int *)(iVar3 + 0x4c);
  param_1 = orig::Unit1028A0A0_ReadBlock(piVar1,4,1,param_1,iVar2);
  puVar5 = orig::SqMem_AllocZeroed(*piVar1,4,s_C__dev_dancer_modules_sqShape_sr_103bc174,0x84);
  *(undefined4 **)(iVar3 + 0x48) = puVar5;
  param_1 = orig::Unit1028A0A0_ReadBlock(puVar5,4,*piVar1,param_1,iVar2);
  orig::FUN_1028da20((float *)(iVar3 + 0x14),*(float **)(iVar4 + 0x54),*(int *)(iVar4 + 0x14),
               *(float **)(iVar4 + 0x54));
  if (param_2 < 2) {
    *(undefined4 *)(iVar3 + 0x50) = 0;
  }
  else {
    param_1 = orig::Unit1028A0A0_ReadBlock((undefined4 *)(iVar3 + 0x50),4,1,param_1,iVar2);
  }
  orig::FUN_102abec0(iVar3,(uint8_t *)0x0,iVar4);
  uVar6 = orig::SqshTriStrip_GetGlobal();
  *(undefined4 *)(iVar3 + 0x44) = uVar6;
  *(int *)(iVar3 + 0x34) = iVar4;
  orig::SqVtx_PackFloatsToBytes(iVar4,3);
  return iVar3;
}
```

## `0x10299E40` fn_10299E40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2, `Unit1028A0A0_ReadBlock` (medium), `sub_1028DA20`, `SqshBuild_Read` (medium), `SqshPoints_Create` (medium)
- **Called by:** `fn_10299B30`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqShape\src\sqshPoints.c`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `sqmdDMBBuildShapes` → `fn_10299B30` → `fn_10299E40`
- **Named callers' callers:** `sqmdDMBBuildShapes`
- **Named callees' callees:** `SqMem_AllocZeroed`, `Unit1028A0A0_ReturnZero`, `Unit10292470_SwapNibblesXor`, `SqVtx_Create`, `SqshPoints_Init`
- **How its callers call it** (their verified code):
  - in `fn_10299B30`: `iVar3 = orig::FUN_10299e40(puVar2,uVar4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 
  - `Unit1028A0A0_ReadBlock` (medium): Reads a block from a stream into a buffer and returns the position after it.
  - `SqshBuild_Read` (medium): Reads a shape build block from a stream, vertex by vertex.
  - `SqshPoints_Create` (medium): Allocates a Squirrel points-shape object and initialises it.

Short callee `SqshPoints_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqshPoints_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::SqMem_AllocZeroed(1,0x44,s_C__dev_dancer_modules_sqShape_sr_103bec10,0x1d);
  orig::SqshPoints_Init(puVar1);
  return puVar1;
}
```

```cpp
XI_API int __cdecl fn_10299E40(undefined4 * param_1, int param_2)
{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = param_2;
  puVar1 = orig::Unit1028A0A0_ReadBlock(&param_2,4,1,param_1,param_2);
  if (param_2 != 1) {
    orig::sqDebugPrintf_v1(s_____Error__Points_version_mismat_103bc26c,param_2,1);
    return 0;
  }
  puVar2 = orig::SqshPoints_Create();
  if (puVar2 == (undefined4 *)0x0) {
    orig::sqDebugPrintf_v1(s_____Error__sqshPointsNew___cause_103bc238);
    return 0;
  }
  iVar3 = orig::SqshBuild_Read(puVar1 + 0x14,&param_1,iVar3);
  if (iVar3 == 0) {
    return 0;
  }
  puVar2[0xd] = iVar3;
  orig::FUN_1028da20((float *)(puVar2 + 5),*(float **)(iVar3 + 0x54),*(int *)(iVar3 + 0x14),
               *(int *)(iVar3 + 0x14));
  return (int)puVar2;
}
```

## `0x10299EE0` fn_10299EE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqMem_AllocZeroed` (medium), `sqDebugPrintf` (high) x2, `Unit1028A0A0_ReadBlock` (medium) x4, `sub_1028DA20`, `SqshBuild_Read` (medium), `SqshCurves_Create` (medium), `SqshCurves_RebuildIndex` (medium)
- **Called by:** `fn_10299B30`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqShape\src\sqshBuild.c`
- **Strings its callees refer to:** `C:\dev\dancer\modules\sqShape\src\sqshCurves.c`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `sqmdDMBBuildShapes` → `fn_10299B30` → `fn_10299EE0`
- **Named callers' callers:** `sqmdDMBBuildShapes`
- **Named callees' callees:** `sqFree`, `Unit1028A0A0_ReturnZero`, `Unit10292470_SwapNibblesXor`, `SqVtx_Create`, `SqVtx_ClearEntries`, `SqVtx_ClearEntriesAlt`, `SqshCurves_Init`
- **How its callers call it** (their verified code):
  - in `fn_10299B30`: `iVar3 = orig::FUN_10299ee0(puVar2,uVar4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqMem_AllocZeroed` (medium): Allocates a zeroed block of count times size bytes.
  - `sqDebugPrintf` (high): 
  - `Unit1028A0A0_ReadBlock` (medium): Reads a block from a stream into a buffer and returns the position after it.
  - `SqshBuild_Read` (medium): Reads a shape build block from a stream, vertex by vertex.
  - `SqshCurves_Create` (medium): Creates and initialises a new sqshCurves object.
  - `SqshCurves_RebuildIndex` (medium): Rebuilds the curve index array from the largest value in its source array.

Short callee `SqshCurves_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqshCurves_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::SqMem_AllocZeroed(1,0x6c,s_C__dev_dancer_modules_sqShape_sr_103bef68,0x24);
  orig::SqshCurves_Init(puVar1);
  return puVar1;
}
```

```cpp
XI_API int __cdecl fn_10299EE0(undefined4 * param_1, int param_2)
{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  
  iVar2 = param_2;
  puVar3 = orig::Unit1028A0A0_ReadBlock(&param_2,4,1,param_1,param_2);
  param_1 = puVar3;
  if (param_2 != 1) {
    orig::sqDebugPrintf_v1(s_____Error__Curves_version_mismat_103bc2d8,param_2,1);
    return 0;
  }
  puVar4 = orig::SqshCurves_Create();
  if (puVar4 == (undefined4 *)0x0) {
    orig::sqDebugPrintf_v1(s_____Error__sqshCurvesNew___cause_103bc2a4);
    return 0;
  }
  iVar5 = orig::SqshBuild_Read(puVar3,&param_1,iVar2);
  if (iVar5 == 0) {
    return 0;
  }
  puVar4[0xd] = iVar5;
  orig::FUN_1028da20((float *)(puVar4 + 5),*(float **)(iVar5 + 0x54),*(int *)(iVar5 + 0x14),
               *(int *)(iVar5 + 0x14));
  puVar3 = orig::Unit1028A0A0_ReadBlock(puVar4 + 0x13,4,1,param_1,iVar2);
  piVar1 =(int*)(puVar4 + 0x11);
  param_1 = orig::Unit1028A0A0_ReadBlock(piVar1,4,1,puVar3,iVar2);
  puVar3 = orig::SqMem_AllocZeroed(*piVar1,4,s_C__dev_dancer_modules_sqShape_sr_103bc174,0xe1);
  puVar4[0x12] = puVar3;
  orig::Unit1028A0A0_ReadBlock(puVar4 + 0x12,4,*piVar1,param_1,iVar2);
  orig::SqshCurves_RebuildIndex((int)puVar4);
  return (int)puVar4;
}
```
