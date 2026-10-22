# Batch 139_CMoAttachments_VT_1032BE7C: 1 functions of `CMoAttachments_VT_1032BE7C`

Name these functions: see `batches/README.md`. Write `data/names/139_CMoAttachments_VT_1032BE7C.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100630D0` CMoAttachments_VT_1032BE7C::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BE7C` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BE7C::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BE7C_VectorDeletingDtor((_DWORD *)this - 13, a2);
}
```
