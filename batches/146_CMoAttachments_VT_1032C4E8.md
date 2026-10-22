# Batch 146_CMoAttachments_VT_1032C4E8: 1 functions of `CMoAttachments_VT_1032C4E8`

Name these functions: see `batches/README.md`. Write `data/names/146_CMoAttachments_VT_1032C4E8.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100745D0` CMoAttachments_VT_1032C4E8::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032C4E8` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032C4E8::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032C4E8_ScalarDeletingDtor((_DWORD *)this - 17, a2);
}
```
