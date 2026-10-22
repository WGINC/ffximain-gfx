# Batch 174_CMoResource_VT_1032C854: 1 functions of `CMoResource_VT_1032C854`

Name these functions: see `batches/README.md`. Write `data/names/174_CMoResource_VT_1032C854.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1002D880` CMoResource_VT_1032C854::vf004

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C854` slot 4 (+0x10)

```cpp
int CMoResource_VT_1032C854::vf004(int a2)
{
  unsigned int v2;
  unsigned int v3;
  unsigned int v4;
  unsigned int v5;
  unsigned int v6;
  v2 = (this->field_3C - (unsigned int)this - 48) >> 1;
  this->field_36 = (this->field_36 - (unsigned int)this - 48) >> 1;
  v3 = this->field_42;
  this->field_3C = v2;
  v4 = (this->field_48 - (unsigned int)this - 48) >> 1;
  this->field_42 = (v3 - (unsigned int)this - 48) >> 1;
  v5 = this->field_4E;
  this->field_48 = v4;
  v6 = (this->field_54 - (unsigned int)this - 48) >> 1;
  this->field_4E = (v5 - (unsigned int)this - 48) >> 1;
  this->field_54 = v6;
  *(_DWORD *)(*(unsigned __int32 *)((char *)this + 0x14)) = a2;
  return a2;
}
```
