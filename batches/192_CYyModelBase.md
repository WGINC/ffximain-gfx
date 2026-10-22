# Batch 192_CYyModelBase: 1 functions of `CYyModelBase`

Name these functions: see `batches/README.md`. Write `data/names/192_CYyModelBase.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10029D30` fn_10029D30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1001ABF0`, `fn_10034230`
- **Called by:** `CYyModel::CreateModelBase`
- **Reads globals:** `0x103514F4`, `0x103514F8`, `0x103514FC`

```cpp
XI_API void * __thiscall fn_10029D30(void * self_)
{
  int iVar1;
  void *this_00;
  
  *(uint8_t ***)self_ = &CYyObject_vtbl_ambiguous0_as_v0;
  orig::FUN_10034230((void *)((int)self_ + 8));
  this_00 = (void *)((int)self_ + 0x24);
  iVar1 = 7;
  do {
    orig::FUN_1001abf0(this_00);
    this_00 = (void *)((int)this_00 + 0x14);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  *(uint8_t ***)self_ = &PTR_CYyModelBase_GetRuntimeClass_1032a710;
  *(uint32_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 0x20) = 0;
  *(uint32_t *)((int)self_ + 0xc0) = 0;
  *(uint32_t *)((int)self_ + 0xb0) = DAT_103514f4;
  *(uint32_t *)((int)self_ + 0xb4) = DAT_103514f8;
  *(uint32_t *)((int)self_ + 0xb8) = DAT_103514fc;
  *(uint32_t *)((int)self_ + 0xbc) = 0xbf800000;
  *(uint32_t *)((int)self_ + 0xc4) = 0xffffffff;
  return self_;
}
```
