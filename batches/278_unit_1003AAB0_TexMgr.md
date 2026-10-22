# Batch 278_unit_1003AAB0_TexMgr: 2 functions of `unit_1003AAB0_TexMgr`

Name these functions: see `batches/README.md`. Write `data/names/278_unit_1003AAB0_TexMgr.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1003AAB0` fn_1003AAB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1003AAC0`, `fn_1003AAF0`, `sub_1003AB10`, `TexMgr_UploadPump`, `sub_1003AC40`, `CYyTexMng::FindByName`, `sub_1003ACD0`, `sub_1003AD40`, `sub_1003ADC0`, `sub_1003AE50`, `TexMgr_FindByName`, `CYyTexMng::FindByNameInResource`, `sub_10075520`, `sub_10075690`

```cpp
XI_API int __thiscall fn_1003AAB0(void * self_)
{
  return (int)self_ + 8;
}
```

## `0x1003AAF0` fn_1003AAF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1003AAB0`
- **Called by:** `TexMgr_Enqueue`

```cpp
XI_API int * __thiscall fn_1003AAF0(void * self_)
{
  int iVar1;
  int *piVar2;
  
  piVar2 = (int *)orig::FUN_1003aab0(self_);
  for (iVar1 = *piVar2; iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x38)) {
    piVar2 = (int *)(iVar1 + 0x38);
  }
  return piVar2;
}
```
