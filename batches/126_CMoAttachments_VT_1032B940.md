# Batch 126_CMoAttachments_VT_1032B940: 1 functions of `CMoAttachments_VT_1032B940`

Name these functions: see `batches/README.md`. Write `data/names/126_CMoAttachments_VT_1032B940.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100630E0` CMoAttachments_VT_1032B940::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032B940` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032B940::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032B940_ScalarDeletingDtor((_DWORD *)this - 13, a2);
}
```
