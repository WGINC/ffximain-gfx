# Batch 260_unit_100231F0_SkinVerticesFinish: 1 functions of `unit_100231F0_SkinVerticesFinish`

Name these functions: see `batches/README.md`. Write `data/names/260_unit_100231F0_SkinVerticesFinish.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10025010` fn_10025010

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10029C90`
- **Called by:** `CYyModelBase::AddPartFromResource`

```cpp
XI_API void * __thiscall fn_10025010(void * self_)
{
  orig::FUN_10029c90(self_);
  *(uint8_t ***)self_ = &PTR_CYyModelDt_GetRuntimeClass_1032a430;
  *(uint32_t *)((int)self_ + 0x40) = 0;
  *(uint8_t *)((int)self_ + 0x3c) = 1;
  return self_;
}
```
