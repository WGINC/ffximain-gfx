# Batch 223_sqinShape: 1 functions of `sqinShape`

Name these functions: see `batches/README.md`. Write `data/names/223_sqinShape.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AEC50` fn_102AEC50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1029A000`, `sub_102AEC30`

```cpp
XI_API void __cdecl fn_102AEC50(undefined4 * param_1)
{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[3] = param_1[3] & 0xfffffffe;
  }
  return;
}
```
