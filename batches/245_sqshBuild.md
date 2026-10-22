# Batch 245_sqshBuild: 1 functions of `sqshBuild`

Name these functions: see `batches/README.md`. Write `data/names/245_sqshBuild.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10299D60` fn_10299D60

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf`, `fn_1028A0F0` x5, `fn_102A9520`, `sub_102A9980`
- **Called by:** `sub_10299C30`, `sub_10299E40`, `sub_10299EE0`

```cpp
XI_API int __cdecl fn_10299D60(undefined4 * param_1, undefined4 * param_2, int param_3)
{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  int iVar4;
  char xi_frame[8] __attribute__((aligned(4)));
  int  &local_8 = *reinterpret_cast<int  *>(xi_frame + 0);
  int  &local_4 = *reinterpret_cast<int  *>(xi_frame + 4);
  
  iVar1 = param_3;
  puVar2 = orig::FUN_1028a0f0(&param_3,4,1,param_1,param_3);
  if (0 < param_3) {
    puVar3 = orig::FUN_102a9520();
    puVar3[5] = param_3;
    puVar2 = orig::FUN_1028a0f0(&local_4,4,1,puVar2,iVar1);
    iVar4 = 0;
    if (0 < local_4) {
      do {
        puVar2 = orig::FUN_1028a0f0(&param_1,4,1,puVar2,iVar1);
        puVar2 = orig::FUN_1028a0f0(&local_8,4,1,puVar2,iVar1);
        orig::FUN_102a9980((int)puVar3,(int)param_1,local_8);
        puVar2 = orig::FUN_1028a0f0((undefined4 *)puVar3[(int)param_1 + 0x15],4,local_8 * param_3,puVar2,
                              iVar1);
        iVar4 = iVar4 + 1;
      } while (iVar4 < local_4);
    }
    *param_2 = puVar2;
    return (int)puVar3;
  }
  orig::sqDebugPrintf_v3(s_____Error__Invalid_vertex_number_103bc210,param_3);
  *param_2 = puVar2;
  return 0;
}
```
