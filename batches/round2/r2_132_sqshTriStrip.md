# Batch r2_132_sqshTriStrip: 1 functions of `sqshTriStrip`

Name these functions: see `batches/README.md`. Write `data/names/r2_132_sqshTriStrip.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102ABEB0` fn_102ABEB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10299C30`
- **Reads globals:** `0x109AAAC0`
- **Named before** (a naming session's reading, low): `SqshTriStrip_GetGlobal`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sqmdDMBBuildShapes` → `sub_10299B30` → `sub_10299C30` → `SqshTriStrip_GetGlobal`

```cpp
XI_API undefined4 __cdecl SqshTriStrip_GetGlobal(void)
{
  return DAT_109aaac0;
}
```
