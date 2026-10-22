# Batch 190_CXiTimerLow: 1 functions of `CXiTimerLow`

Name these functions: see `batches/README.md`. Write `data/names/190_CXiTimerLow.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10019490` fn_10019490

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10019210`, `sub_100194F0`
- **Called by:** `fn_100191D0`
- **Imports:** `timeGetTime`

```cpp
XI_API void * __thiscall fn_10019490(void * self_)
{
  DWORD DVar1;
  
  orig::FUN_10019210(self_);
  *(uint8_t ***)self_ = &PTR_CXiTimerLow_GetRuntimeClass_1032a118;
  DVar1 = timeGetTime();
  *(DWORD *)((int)self_ + 0xc) = DVar1;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  *(uint32_t *)((int)self_ + 0x1c) = 0;
  orig::FUN_100194f0(self_);
  return self_;
}
```
