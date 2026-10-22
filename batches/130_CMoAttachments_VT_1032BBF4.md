# Batch 130_CMoAttachments_VT_1032BBF4: 1 functions of `CMoAttachments_VT_1032BBF4`

Name these functions: see `batches/README.md`. Write `data/names/130_CMoAttachments_VT_1032BBF4.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10062FD0` CMoAttachments_VT_1032BBF4::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032BBF4` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032BBF4::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032BBF4_VectorDeletingDtor((_DWORD *)this - 13, a2);
}
```
