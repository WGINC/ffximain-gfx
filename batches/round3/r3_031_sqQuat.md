# Batch r3_031_sqQuat: 1 functions of `sqQuat`

Name these functions: see `batches/README.md`. Write `data/names/r3_031_sqQuat.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1027AFB0` fn_1027AFB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Unit1027A4B0_SetIdentity` (medium), `sub_1027B480`, `sqDebugPrintf` (high) x2
- **Called by:** `StAvatar_SetEntryFlag2` (low)
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqBase\src\sqQuat.c`, `Warning in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatarInit` → `StAvatar_InitEntriesFromTables` → `StAvatar_SetEntryFlag2` → `fn_1027AFB0`
- **Named callers' callers:** `StAvatar_InitEntriesFromTables`
- **Named callees' callees:** `Sq_PrintIndent`
- **How its callers call it** (their verified code):
  - in `StAvatar_SetEntryFlag2` (low): `orig::FUN_1027afb0(local_10);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_SetEntryFlag2` (low): Sets or clears the second flag of an avatar part.
  - `Unit1027A4B0_SetIdentity` (medium): Sets a quaternion to the identity rotation.
  - `sqDebugPrintf` (high): 

Short callee `Unit1027A4B0_SetIdentity` (medium):
```cpp
XI_API void __cdecl Unit1027A4B0_SetIdentity(undefined4 * param_1)
{
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  param_1[3] = 0x3f800000;
  return;
}
```

```cpp
XI_API undefined4 __cdecl fn_1027AFB0(float * param_1)
{
  long double fVar1;
  
  fVar1 = (long double)param_1[3] * (long double)param_1[3] +
          (long double)param_1[2] * (long double)param_1[2] +
          (long double)param_1[1] * (long double)param_1[1] + (long double)*param_1 * (long double)*param_1;
  if (fVar1 > (long double)0.0f) {
    fVar1 = (long double)1.0f / SQRT(fVar1);
    *param_1 = (float)-(fVar1 * (long double)*param_1);
    param_1[1] = (float)-(fVar1 * (long double)param_1[1]);
    param_1[2] = (float)-(fVar1 * (long double)param_1[2]);
    param_1[3] = (float)(fVar1 * (long double)param_1[3]);
    return 0xff;
  }
  orig::sqDebugPrintf_v1(s_Warning_in__s__line__d__103b27fc,s_C__dev_dancer_modules_sqBase_src_103b40c0,0x222)
  ;
  orig::sqDebugPrintf_v1(s_QfInvert__can_t_invert_the_singu_103b408c);
  orig::FUN_1027b480(param_1,0,0);
  orig::Unit1027A4B0_SetIdentity(param_1);
  return 0;
}
```
