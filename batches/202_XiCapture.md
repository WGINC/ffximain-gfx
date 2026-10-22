# Batch 202_XiCapture: 1 functions of `XiCapture`

Name these functions: see `batches/README.md`. Write `data/names/202_XiCapture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10017BB0` fn_10017BB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `App_InitScene`

```cpp
XI_API undefined4 __thiscall fn_10017BB0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_LAB_10329f34;
  *(uint32_t *)((int)self_ + 4) = 0;
  *(uint32_t *)((int)self_ + 8) = 0;
  return self_;
}
```
