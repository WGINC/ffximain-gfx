# Batch 241_sqsdCartoon: 1 functions of `sqsdCartoon`

Name these functions: see `batches/README.md`. Write `data/names/241_sqsdCartoon.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102997A0` fn_102997A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqgxBlendSub`, `sqgxPassAdd` x2, `sqgxContext_SelectShaderMode` x2, `sqDrawableDraw` x2, `fn_10298C50` x2, `fn_102A1FA0`, `fn_102A34D0`
- **Called by:** `sub_102A1F20`

```cpp
XI_API void __cdecl fn_102997A0(int param_1, int * param_2)
{
  int iVar1;
  
  if ((*(int *)(param_1 + 0x54) != 0) && (iVar1 = orig::FUN_102a1fa0(*param_2), iVar1 == 2)) {
    return;
  }
  orig::FUN_102a34d0(param_1 + 0x14);
  orig::FUN_10298c50(*(int *)(param_1 + 0x50));
  orig::sqgxShader_v1(0x10000,0);
  orig::sqDrawableDraw_v1(*param_2);
  orig::sqgxPassAdd_v1(2);
  orig::sqgxBlendSub();
  orig::FUN_10298c50(*(int *)(param_1 + 0x4c));
  orig::sqgxShader_v1(0x800000,0);
  orig::sqDrawableDraw_v1(*param_2);
  orig::sqgxPassAdd_v1(-2);
  orig::sqgxBlendOff_v1();
  return;
}
```
