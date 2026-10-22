# Batch r2_120_sqgxTexture: 1 functions of `sqgxTexture`

Name these functions: see `batches/README.md`. Write `data/names/r2_120_sqgxTexture.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10281A70` fn_10281A70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10277B70`, `SqgxContext_ResetHandles` (low), `fn_1028E1B0`, `sub_10298BD0`, `sub_102A2E30`
- **Writes globals:** `0x109A261C`
- **Reads globals:** `0x109A261C`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_counter_109A261C`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `StAvatar_SetActive` → `StAvatar_ReleaseMemory` → `SqgxContext_ResetHandles` → `fn_10281A70`
- **Named callers' callers:** `StAvatar_ReleaseMemory`, `scnFini`, `SqmdModel_Free`
- **How its callers call it** (their verified code):
  - in `fn_10277B70`: `orig::FUN_10281a70(iVar2);`
  - in `SqgxContext_ResetHandles` (low): `orig::FUN_10281a70(*(int *)(g_pSqgxContext_as_s32 + 0x8a4));`
  - in `SqgxContext_ResetHandles` (low): `orig::FUN_10281a70(*(int *)(g_pSqgxContext_as_s32 + 0x8a8));`
  - in `fn_1028E1B0`: `orig::FUN_10281a70(g_sceneFadeMaskRT);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqgxContext_ResetHandles` (low): Releases three handles and resets the context's index tables.

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
