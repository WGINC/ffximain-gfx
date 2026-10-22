# Batch r2_134_sqskSkeleton: 5 functions of `sqskSkeleton`

Name these functions: see `batches/README.md`. Write `data/names/r2_134_sqskSkeleton.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1027B580` fn_1027B580

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `SqskSkeleton_Init` (medium)
- **Called by:** `sub_1027B7A0`, `sub_1027BA00`, `sub_1029AF20`
- **Named before** (a naming session's reading, medium): `SqskSkeleton_Create`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkeleton\src\sqskSkeleton.c`
- **From the frame root** (shortest chain of direct calls): `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029ADF0` → `sub_1029AF20` → `SqskSkeleton_Create`
- **Named callees' callees:** `sqDebugPrintf`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqskSkeleton_Init` (medium): Initialises a skeleton record and its function table.

```cpp
XI_API undefined4 * __cdecl SqskSkeleton_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x30,s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x1d);
  SqskSkeleton_Init(puVar1);
  return puVar1;
}
```

## `0x1027B5A0` fn_1027B5A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `SqskSkeleton_Create` (medium)
- **Named before** (a naming session's reading, medium): `SqskSkeleton_Init`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029ADF0` → `sub_1029AF20` → `SqskSkeleton_Create` → `SqskSkeleton_Init`
- **How its callers call it** (their verified code):
  - in `SqskSkeleton_Create` (medium): `SqskSkeleton_Init(puVar1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqskSkeleton_Create` (medium): Allocates and initialises a skeleton record.

```cpp
XI_API void __cdecl SqskSkeleton_Init(undefined4 * param_1)
{
  *param_1 = 0xb0001;
  param_1[2] = &LAB_1027b5e0;
  param_1[3] = XI_CODE(FUN_1027b610);
  param_1[4] = &LAB_1027b650;
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = XI_CODE(SqskSkeleton_Release);
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  return;
}
```

## `0x1027C4C0` fn_1027C4C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqMem_AllocZeroed` (medium) x2, `sqFree` x2, `SqskJoint_Init` (medium), `fn_10291460`, `SqhiNode_Init` (medium), `fn_10291C50`
- **Called by:** `sub_1027B640`, `sub_1027B7A0`, `sub_1027BA00`, `sub_1029AF20`
- **Named before** (a naming session's reading, medium): `SqskSkeleton_SetNodeCount`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkeleton\src\sqskSkeleton.c`
- **Strings its callees refer to:** `VALID`
- **From the frame root** (shortest chain of direct calls): `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029ADF0` → `sub_1029AF20` → `SqskSkeleton_SetNodeCount`
- **Named callees' callees:** `Unit10274050_ZeroDwords`, `Unit10274050_SetXyz`, `SqMem_ClassifyBlock`, `SqMem_GetPayloadEnd`, `SqMem_FindArena`, `Unit1027E120_Zero`, `sqDebugPrintf`, `SqxfXformM_Init`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqMem_AllocZeroed` (medium): Allocates a zeroed block of count times size bytes.
  - `SqskJoint_Init` (medium): Initialises a sqskJoint object with its callbacks, sub-objects and unit scale.
  - `SqhiNode_Init` (medium): Initialises a sqhiNode object and its transform matrix.

Short callee `fn_10291460`:
```cpp
XI_API void __cdecl fn_10291460(void)
{
  return;
}
```

Short callee `fn_10291C50`:
```cpp
XI_API void __cdecl fn_10291C50(void)
{
  return;
}
```

```cpp
XI_API void __cdecl SqskSkeleton_SetNodeCount(int param_1, int param_2)
{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = *(int *)(param_1 + 0x28);
  if (iVar2 != 0) {
    if (-1 < iVar2 + -1) {
      do {
        orig::FUN_10291c50();
        orig::FUN_10291460();
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
    }
    orig::sqFree_v1(*(int *)(param_1 + 0x20),s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x266);
    orig::sqFree_v1(*(int *)(param_1 + 0x24),s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x267);
    *(undefined4 *)(param_1 + 0x28) = 0;
  }
  if (0 < param_2) {
    *(int *)(param_1 + 0x28) = param_2;
    puVar1 = SqMem_AllocZeroed(param_2,0xf8,s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x270);
    *(undefined4 **)(param_1 + 0x20) = puVar1;
    puVar1 = SqMem_AllocZeroed(*(int *)(param_1 + 0x28),0x9c,s_C__dev_dancer_modules_sqSkeleton_103b40ec,
                          0x271);
    *(undefined4 **)(param_1 + 0x24) = puVar1;
    iVar2 = 0;
    if (0 < *(int *)(param_1 + 0x28)) {
      iVar4 = 0;
      iVar3 = 0;
      do {
        SqhiNode_Init((undefined4 *)(*(int *)(param_1 + 0x20) + iVar4));
        SqskJoint_Init((undefined4 *)(*(int *)(param_1 + 0x24) + iVar3));
        *(int *)(*(int *)(param_1 + 0x20) + 100 + iVar4) = *(int *)(param_1 + 0x24) + iVar3;
        iVar2 = iVar2 + 1;
        iVar3 = iVar3 + 0x9c;
        *(uint8_t *)(*(int *)(param_1 + 0x20) + 0x74 + iVar4) = 0;
        iVar4 = iVar4 + 0xf8;
      } while (iVar2 < *(int *)(param_1 + 0x28));
    }
    *(undefined4 *)(param_1 + 0x2c) = 0;
    *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0x20);
  }
  return;
}
```

## `0x1027C980` fn_1027C980

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2
- **Called by:** `Unit10288FF0_ResolveSkelRefs` (medium), `sub_1028D490`, `sqmdScriptCmd_skeleton_shape`, `sub_102A12F0`
- **Named before** (a naming session's reading, medium): `SqskSkeleton_GetNode`; name it again from this dossier, or leave it out to keep that name
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqSkeleton\src\sqskSkeleton.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `Unit10288FF0_ResolveSkelRefs` → `SqskSkeleton_GetNode`
- **How its callers call it** (their verified code):
  - in `Unit10288FF0_ResolveSkelRefs` (medium): `iVar2 = SqskSkeleton_GetNode(param_2,uVar1);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 
  - `Unit10288FF0_ResolveSkelRefs` (medium): Resolves the skeleton references of a DMB entry, clearing those that fail.

```cpp
XI_API int __cdecl SqskSkeleton_GetNode(int param_1, int param_2)
{
  if ((-1 < param_2) && (param_2 < *(int *)(param_1 + 0x28))) {
    return *(int *)(param_1 + 0x20) + param_2 * 0xf8;
  }
  orig::sqDebugPrintf_v2(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqSkeleton_103b40ec,0x3b4);
  orig::sqDebugPrintf_v2(s_node_index__d_is_out_of_range__0_103b4520,param_2,*(uint32_t *)(param_1 + 0x28))
  ;
  return 0;
}
```

## `0x1027C9D0` fn_1027C9D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1028E060`, `fn_1029A450`
- **Named before** (a naming session's reading, low): `SqskSkeleton_Release`; name it again from this dossier, or leave it out to keep that name
- **Named callers' callers:** `SqinSkin_ReleaseIfHasData`

```cpp
XI_API void __cdecl SqskSkeleton_Release(int param_1)
{
  if (*(int *)(param_1 + 0x1c) != 0) {
                    /* WARNING: Could not recover jumptable at 0x1027c9df. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(*(int *)(param_1 + 0x1c) + 0x18))();
    return;
  }
  return;
}
```
