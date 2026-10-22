# Batch 220_sqgxTexture: 2 functions of `sqgxTexture`

Name these functions: see `batches/README.md`. Write `data/names/220_sqgxTexture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102819C0` fn_102819C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10277B70`

```cpp
XI_API void __cdecl fn_102819C0(int param_1)
{
  ((uint8_t *)((&g_sqgxPaletteUsed_as_v0)))[param_1] = 0;
  return;
}
```

## `0x10281A70` fn_10281A70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10277B70`, `fn_10282590`, `fn_1028E1B0`, `sub_10298BD0`, `sub_102A2E30`
- **Writes globals:** `0x109A261C`
- **Reads globals:** `0x109A261C`

```cpp
XI_API void __cdecl fn_10281A70(int param_1)
{
  int *piVar1;
  
  if ((&g_sqgxTex_format_as_v0)[param_1 * 10] != -1) {
    piVar1 = *(int **)(((char *)&g_sqgxTex_texture_as_v0) + param_1 * 0x28);
    (&g_sqgxTex_format_as_v0)[param_1 * 10] = 0xffffffff;
    if (piVar1 != (int *)0x0) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
    }
    piVar1 = *(int **)(((char *)&g_sqgxTex_surface_as_v0) + param_1 * 0x28);
    if (piVar1 != (int *)0x0) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
    }
    if (0 < g_counter_109A261C) {
      piVar1 = (int *)(((char *)&DAT_1099d608) + g_counter_109A261C * 0x28);
      do {
        if (*piVar1 != -1) {
          return;
        }
        g_counter_109A261C = g_counter_109A261C + -1;
        piVar1 = piVar1 + -10;
      } while (0 < g_counter_109A261C);
    }
  }
  return;
}
```
