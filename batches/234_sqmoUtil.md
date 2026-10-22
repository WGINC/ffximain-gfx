# Batch 234_sqmoUtil: 1 functions of `sqmoUtil`

Name these functions: see `batches/README.md`. Write `data/names/234_sqmoUtil.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1027D650` fn_1027D650

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` x2
- **Called by:** `fn_10293000`, `fn_102944E0`

```cpp
XI_API void __cdecl fn_1027D650(int param_1, int param_2, int param_3, uint8_t * param_4)
{
  int iVar1;
  
  if ((param_3 < 0) || (param_2 <= param_3)) {
    orig::sqDebugPrintf_v2(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqMotion_s_103b4584,0x21e
                 );
    orig::sqDebugPrintf_v2(s_entry__d_out_of_range__d_103b45b4,param_3,param_2);
  }
  else {
    iVar1 = 0;
    *(uint8_t *)(param_1 + param_3) = 0xff;
    *param_4 = 0;
    if (0 < param_2) {
      do {
        if (*(char *)(iVar1 + param_1) == '\0') {
          *param_4 = 0xff;
          return;
        }
        iVar1 = iVar1 + 1;
      } while (iVar1 < param_2);
      return;
    }
  }
  return;
}
```
