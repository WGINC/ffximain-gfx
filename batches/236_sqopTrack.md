# Batch 236_sqopTrack: 2 functions of `sqopTrack`

Name these functions: see `batches/README.md`. Write `data/names/236_sqopTrack.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10289B40` fn_10289B40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10289830`, `sqopSystemBuild`, `sub_10289AB0`, `sub_10289B20`, `fn_10289BA0`

```cpp
XI_API void __cdecl fn_10289B40(undefined4 * param_1)
{
  *param_1 = 0xffffffff;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0;
  return;
}
```

## `0x10289BA0` fn_10289BA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree`, `fn_10289B40`
- **Called by:** `fn_102896A0`, `sub_10289B70`

```cpp
XI_API void __cdecl fn_10289BA0(int param_1)
{
  if (*(int *)(param_1 + 0xc) != 0) {
    orig::sqFree_v1(*(int *)(param_1 + 0xc),s_C__dev_dancer_modules_sqOpcode_s_103b9618,0x69);
  }
  orig::FUN_10289b40((uint32_t *)param_1);
  return;
}
```
