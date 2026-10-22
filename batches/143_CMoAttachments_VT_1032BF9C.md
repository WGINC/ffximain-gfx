# Batch 143_CMoAttachments_VT_1032BF9C: 1 functions of `CMoAttachments_VT_1032BF9C`

Name these functions: see `batches/README.md`. Write `data/names/143_CMoAttachments_VT_1032BF9C.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10063010` CMoAttachments_VT_1032BF9C::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BF9C` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BF9C::vf001(char a2)
{
  _DWORD * self = reinterpret_cast<_DWORD *>(this);  // byte-exact view for the rest
  return orig::CMoAttachments_VT_1032BF9C_VectorDeletingDtor(self - 13, a2);
}
```
