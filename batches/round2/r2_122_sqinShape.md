# Batch r2_122_sqinShape: 1 functions of `sqinShape`

Name these functions: see `batches/README.md`. Write `data/names/r2_122_sqinShape.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102AEC50` fn_102AEC50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `SqinSkin_Init` (medium), `sub_102AEC30`
- **Named before** (a naming session's reading, low): `SqinShape_Reset`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sub_10288FF0` → `sub_1029AA80` → `SqinSkin_Create` → `SqinSkin_Init` → `SqinShape_Reset`
- **Named callers' callers:** `SqinSkin_Create`
- **How its callers call it** (their verified code):
  - in `SqinSkin_Init` (medium): `SqinShape_Reset(param_1 + 9);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqinSkin_Init` (medium): Sets the skin object's type code, callbacks and defaults.

```cpp
XI_API void __cdecl SqinShape_Reset(undefined4 * param_1)
{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[3] = param_1[3] & 0xfffffffe;
  }
  return;
}
```
