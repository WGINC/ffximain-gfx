# Batch 147_CMoAttachments_VT_1032C628: 1 functions of `CMoAttachments_VT_1032C628`

Name these functions: see `batches/README.md`. Write `data/names/147_CMoAttachments_VT_1032C628.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10074600` CMoAttachments_VT_1032C628::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032C628` slot 1 (+0x4)

```cpp
unsigned int ** CMoAttachments_VT_1032C628::vf001(char a2)
{
  return orig::CMoAttachments_VT_1032C628_ScalarDeletingDtor((unsigned int **)this - 12, a2);
}
```
