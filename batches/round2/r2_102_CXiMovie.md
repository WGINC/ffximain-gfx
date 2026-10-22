# Batch r2_102_CXiMovie: 4 functions of `CXiMovie`

Name these functions: see `batches/README.md`. Write `data/names/r2_102_CXiMovie.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1021D440` fn_1021D440

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMovie`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `fn_1021D440`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API undefined4 __cdecl fn_1021D440(void)
{
  uint32_t xi_entry_eax = 0;
  uint32_t uVar1;
  
  if (g_pMovie != 0) {
    uVar1 = orig::FUN_1021d960();
    return uVar1;
  }
  return xi_entry_eax & 0xffffff00;
}
```

## `0x1021D460` fn_1021D460

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1021D9A0`
- **Called by:** `App_Destructor`, `sub_10018850`, `CXiMovie_Create` (medium)
- **Writes globals:** `0x10666E7C`
- **Reads globals:** `0x10666E7C`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMovie`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `fn_1021D460`
- **Named callers' callers:** `Frame_Render`
- **How its callers call it** (their verified code):
  - in `CXiMovie_Create` (medium): `uVar2 = orig::FUN_1021d460(uVar2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiMovie_Create` (medium): Creates the movie object for a file and stores it as the current movie.

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1021D460(undefined4 xi_eax)
{
  if (g_pMovie_as_v0 != (int *)0x0) {
    xi_eax = orig::FUN_1021d9a0(xi_eax);
    if (g_pMovie_as_v0 != (int *)0x0) {
      xi_eax = ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*g_pMovie_as_v0 + 0x18))))((void *)(g_pMovie_as_v0),1);
      g_pMovie_as_v0 = (int *)0x0;
    }
  }
  return xi_eax;
}
```

## `0x1021D490` fn_1021D490

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`
- **Named before** (a naming session's reading, low): `CXiMovie_GetField4`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMovie`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `CXiMovie_GetField4`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API undefined4 __cdecl CXiMovie_GetField4(void)
{
  if (g_pMovie != 0) {
    return *(uint32_t *)(g_pMovie + 4);
  }
  return 0;
}
```

## `0x1021D4A0` fn_1021D4A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10018850`
- **Reads globals:** `0x10666E7C`
- **Named before** (a naming session's reading, low): `CXiMovie_GetDoubleField8`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pMovie`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `CXiMovie_GetDoubleField8`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API long double __cdecl CXiMovie_GetDoubleField8(void)
{
  if (g_pMovie != 0) {
    return (long double)*(double *)(g_pMovie + 8);
  }
  return (long double)0.0f;
}
```
