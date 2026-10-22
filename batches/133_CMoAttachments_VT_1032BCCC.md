# Batch 133_CMoAttachments_VT_1032BCCC: 1 functions of `CMoAttachments_VT_1032BCCC`

Name these functions: see `batches/README.md`. Write `data/names/133_CMoAttachments_VT_1032BCCC.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10062F50` CMoAttachments_VT_1032BCCC::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BCCC` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BCCC::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BCCC_VectorDeletingDtor((_DWORD *)this - 13, a2);
}
```
