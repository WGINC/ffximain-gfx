# Batch 122_CMoAttachments_VT_1032AE94: 1 functions of `CMoAttachments_VT_1032AE94`

Name these functions: see `batches/README.md`. Write `data/names/122_CMoAttachments_VT_1032AE94.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10042690` CMoAttachments_VT_1032AE94::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032AE94` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032AE94::vf001(char a2)
{
  return orig::CMoOtTask_ScalarDeletingDtor((_DWORD *)this - 21, a2);
}
```
