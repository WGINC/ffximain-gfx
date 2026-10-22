# Batch 213_sqMatrix3: 1 functions of `sqMatrix3`

Name these functions: see `batches/README.md`. Write `data/names/213_sqMatrix3.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102967E0` fn_102967E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1027F0E0`, `sub_1027F650`, `sqMatrix4_1027FCC0`, `sub_102966C0`

```cpp
XI_API long double __cdecl fn_102967E0(float * param_1)
{
  return ((((long double)param_1[8] * (long double)*param_1 * (long double)param_1[4] +
           (long double)param_1[2] * (long double)param_1[3] * (long double)param_1[7] +
           (long double)param_1[6] * (long double)param_1[1] * (long double)param_1[5]) -
          (long double)*param_1 * (long double)param_1[7] * (long double)param_1[5]) -
         (long double)param_1[8] * (long double)param_1[1] * (long double)param_1[3]) -
         (long double)param_1[6] * (long double)param_1[2] * (long double)param_1[4];
}
```
