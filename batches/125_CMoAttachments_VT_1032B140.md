# Batch 125_CMoAttachments_VT_1032B140: 1 functions of `CMoAttachments_VT_1032B140`

Name these functions: see `batches/README.md`. Write `data/names/125_CMoAttachments_VT_1032B140.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10054450` CMoAttachments_VT_1032B140::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032B140` slot 1 (+0x4)

```cpp
void * CMoAttachments_VT_1032B140::vf001(char a2)
{
  return orig::CMoGeneratorClone_VT_1032B114_ScalarDeletingDtor((char *)this - 48, a2);
}
```
