# Batch 235_sqopSystem: 2 functions of `sqopSystem`

Name these functions: see `batches/README.md`. Write `data/names/235_sqopSystem.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10289620` fn_10289620

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10252C80`, `sub_10289600`, `fn_102896A0`

```cpp
XI_API void __cdecl fn_10289620(undefined4 * param_1)
{
  *param_1 = 0x1d0001;
  param_1[2] = &LAB_10289660;
  param_1[3] = XI_CODE(FUN_10289670);
  param_1[4] = &sqopSystemPrint;
  param_1[5] = 1;
  param_1[6] = 0xffffffff;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  return;
}
```

## `0x102896A0` fn_102896A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqFree`, `fn_10289620`, `fn_10289BA0`
- **Called by:** `fn_10253210`, `sub_10289670`

```cpp
XI_API void __cdecl fn_102896A0(undefined4 * param_1)
{
  int iVar1;
  int iVar2;
  
  if (param_1[9] != 0) {
    iVar2 = 0;
    if (0 < (int)param_1[7]) {
      iVar1 = 0;
      do {
        orig::FUN_10289ba0(param_1[9] + iVar1);
        iVar2 = iVar2 + 1;
        iVar1 = iVar1 + 0x10;
      } while (iVar2 < (int)param_1[7]);
    }
    orig::sqFree_v1(param_1[9],s_C__dev_dancer_modules_sqOpcode_s_103b9484,0x97);
  }
  orig::FUN_10289620(param_1);
  return;
}
```
