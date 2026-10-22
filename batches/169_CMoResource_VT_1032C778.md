# Batch 169_CMoResource_VT_1032C778: 1 functions of `CMoResource_VT_1032C778`

Name these functions: see `batches/README.md`. Write `data/names/169_CMoResource_VT_1032C778.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10071A30` CMoResource_VT_1032C778::vf009

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C778` slot 9 (+0x24)

```cpp
char CMoResource_VT_1032C778::vf009()
{
  char * self = reinterpret_cast<char *>(this);  // byte-exact view for the rest
  int v1;
  int v2;
  char *v3;
  v3 = self + 48;
  v1 = (int)(((uint32_t)(v1) & 0xFFFFFF00u) | ((uint32_t)(uint8_t)(v3[28]) << 0));
  if ( (_BYTE)v1 != 0 )
  {
    v2 = *((_DWORD *)v3 + 5);
    v3[28] = 0;
    *((_DWORD *)v3 + 5) = v2 - (_DWORD)v3;
    v1 = *((_DWORD *)v3 + 6) - (_DWORD)v3;
    *((_DWORD *)v3 + 6) = v1;
  }
  return v1;
}
```
