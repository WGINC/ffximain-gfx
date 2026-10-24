# Batch r2_126_sqmdSort: 1 functions of `sqmdSort`

Name these functions: see `batches/README.md`. Write `data/names/r2_126_sqmdSort.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1029B310` fn_1029B310

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80` x2
- **Called by:** `sub_1029B3A0`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqModel\src\sqmdSort.c`
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029B3A0` → `fn_1029B310`
- **Named callees' callees:** `sqDebugPrintf`

```cpp
XI_API void __cdecl fn_1029B310(int * param_1, int * param_2, int param_3)
{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  
  iVar4 = **(int **)(param_3 + 0x48);
  piVar3 = *(int **)(param_3 + 0x48) + 1;
  *param_1 = iVar4;
  puVar2 = orig::FUN_10275d80(1,iVar4 * 4,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0xb3);
  *param_2 = (int)puVar2;
  while (iVar4 != 0) {
    iVar1 = *piVar3;
    iVar4 = iVar4 + -1;
    if (iVar1 < 1) {
      *(undefined4 *)(*param_2 + iVar4 * 4) = 0;
      iVar1 = iVar1 * -4;
    }
    else {
      iVar1 = iVar1 * 4;
      puVar2 = orig::FUN_10275d80(1,iVar1,s_C__dev_dancer_modules_sqModel_sr_103bc66c,0xb7);
      *(undefined4 **)(*param_2 + iVar4 * 4) = puVar2;
    }
    piVar3 = (int *)((int)piVar3 + iVar1 + 4);
  }
  return;
}
```
