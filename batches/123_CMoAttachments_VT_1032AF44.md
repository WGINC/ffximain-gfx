# Batch 123_CMoAttachments_VT_1032AF44: 1 functions of `CMoAttachments_VT_1032AF44`

Name these functions: see `batches/README.md`. Write `data/names/123_CMoAttachments_VT_1032AF44.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10043DB0` CMoAttachments_VT_1032AF44::vf001

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoAttachments_VT_1032AF44` slot 1 (+0x4)

```cpp
void * CMoAttachments_VT_1032AF44::vf001(char a2)
{
  return orig::CMoTask_VT_1032AF60_ScalarDeletingDtor((char *)this - 52, a2);
}
```
