# Batch 148_CMoAttachments_VT_1032E7EC: 1 functions of `CMoAttachments_VT_1032E7EC`

Name these functions: see `batches/README.md`. Write `data/names/148_CMoAttachments_VT_1032E7EC.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100AB060` CMoAttachments_VT_1032E7EC::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032E7EC` slot 1 (+0x4)

```cpp
_DWORD * CMoAttachments_VT_1032E7EC::vf001(char a2)
{
  return orig::CMoTask_VT_1032E808_ScalarDeletingDtor((_DWORD *)this - 13, a2);
}
```
