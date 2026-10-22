# Batch 201_StTrigger: 1 functions of `StTrigger`

Name these functions: see `batches/README.md`. Write `data/names/201_StTrigger.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10252EA0` fn_10252EA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1024E280`, `fn_1024F5E0`

```cpp
XI_API int __cdecl fn_10252EA0(uint32_t * param_1, int param_2, int param_3)
{
  int *piVar1;
  int iVar2;
  int *piVar3;
  
  if ((((-1 < param_2) && (param_2 < (int)param_1[0xd])) && ((*param_1 & 4) != 0)) &&
     ((*param_1 & 8) != 0)) {
    param_1[1] = 0;
    piVar1 = (int *)(param_2 * 0x10 + param_1[0xf]);
    param_1[2] = 0;
    if (piVar1[3] != 0) {
      iVar2 = 0;
      if (0 < piVar1[1]) {
        piVar3 = (int *)(piVar1[3] + 4);
        do {
          if (param_3 <= *piVar3) {
            *piVar1 = iVar2;
            return iVar2;
          }
          piVar3 = piVar3 + 3;
          iVar2 = iVar2 + 1;
        } while (iVar2 < piVar1[1]);
      }
    }
  }
  return -1;
}
```
