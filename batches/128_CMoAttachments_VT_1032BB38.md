# Batch 128_CMoAttachments_VT_1032BB38: 1 functions of `CMoAttachments_VT_1032BB38`

Name these functions: see `batches/README.md`. Write `data/names/128_CMoAttachments_VT_1032BB38.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10063070` CMoAttachments_VT_1032BB38::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BB38` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BB38::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BB38_ScalarDeletingDtor((_DWORD *)this - 13, a2);
}
```
