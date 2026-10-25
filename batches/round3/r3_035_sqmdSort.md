# Batch r3_035_sqmdSort: 1 functions of `sqmdSort`

Name these functions: see `batches/README.md`. Write `data/names/r3_035_sqmdSort.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029B3A0` fn_1029B3A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x2, `SqmdSort_Create` (medium), `SqmdSort_AllocLists` (medium)
- **Called by:** `fn_10288FF0`, `sub_102A1B00`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdSort.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `fn_102528D0` → `fn_10287FB0` → `fn_10288FF0` → `fn_1029B3A0`
- **Named callees' callees:** `SqmdSort_Init`
- **How its callers call it** (their verified code):
  - in `fn_10288FF0`: `iVar5 = orig::FUN_1029b3a0(*(int **)(*(int *)(param_1 + 0x4c) + iVar11 * 4),uVar12 & 0xf,`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 
  - `SqmdSort_Create` (medium): Allocates a Squirrel sort object and initialises it.
  - `SqmdSort_AllocLists` (medium): Allocates the sort lists a model's data describes.

Short callee `SqmdSort_Create` (medium):
```cpp
XI_API undefined4 * __cdecl SqmdSort_Create(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0x3c,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0x1d);
  orig::SqmdSort_Init(puVar1);
  return puVar1;
}
```

```cpp
XI_API int __cdecl fn_1029B3A0(int * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  undefined4 *puVar1;
  
  if (*param_1 != 0xa0009) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqModel_sr_103bc66c,199);
    orig::sqDebugPrintf_v1(s_sort_updater_must_use_tri_strips_103bc69c);
    return 0;
  }
  puVar1 = orig::SqmdSort_Create();
  puVar1[7] = param_2;
  puVar1[10] = param_3;
  puVar1[9] = param_1;
  puVar1[0xb] = param_4;
  orig::SqmdSort_AllocLists(puVar1 + 0xc,puVar1 + 0xd,(int)param_1);
  puVar1[0xe] = 1;
  return (int)puVar1;
}
```
