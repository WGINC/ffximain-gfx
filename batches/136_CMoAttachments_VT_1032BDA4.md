# Batch 136_CMoAttachments_VT_1032BDA4: 1 functions of `CMoAttachments_VT_1032BDA4`

Name these functions: see `batches/README.md`. Write `data/names/136_CMoAttachments_VT_1032BDA4.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10063030` CMoAttachments_VT_1032BDA4::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BDA4` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BDA4::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BDA4_VectorDeletingDtor((_DWORD *)this - 13, a2);
}
```
