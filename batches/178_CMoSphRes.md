# Batch 178_CMoSphRes: 2 functions of `CMoSphRes`

Name these functions: see `batches/README.md`. Write `data/names/178_CMoSphRes.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100562B0` CMoSphRes::vf008

- **Gate:** none
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoSphRes` slot 8 (+0x20)

```cpp
char * CMoSphRes::vf008()
{
  return orig::CMoSphRes_FixupOffsets(((unsigned __int8 *)((char *)this + 0x30)));
}
```

## `0x100562C0` CMoSphRes::vf009

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CMoSphRes` slot 9 (+0x24)

```cpp
void CMoSphRes::vf009()
{
  ;
}
```
