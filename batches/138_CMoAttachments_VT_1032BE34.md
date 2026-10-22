# Batch 138_CMoAttachments_VT_1032BE34: 1 functions of `CMoAttachments_VT_1032BE34`

Name these functions: see `batches/README.md`. Write `data/names/138_CMoAttachments_VT_1032BE34.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100630C0` CMoAttachments_VT_1032BE34::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BE34` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BE34::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BE34_VectorDeletingDtor((_DWORD *)this - 13, a2);
}
```
