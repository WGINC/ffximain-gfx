# Batch r2_130_sqsdGlow: 1 functions of `sqsdGlow`

Name these functions: see `batches/README.md`. Write `data/names/r2_130_sqsdGlow.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102999A0` fn_102999A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `SqgxContext_SetViewportVec4` (low), `sqgxContext_SelectShaderMode`, `Unit102A1F20_TestSeparationHalf` (low)
- **Called by:** `sqsdGlow_BakeOntoShape`, `sub_102A1F20`
- **Named before** (a naming session's reading, low): `SqsdGlow_ApplyShader`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_102528D0` → `sub_10287FB0` → `sqmdDMBBuildShapes` → `sqsdGlow_BakeOntoShape` → `SqsdGlow_ApplyShader`
- **Named callers' callers:** `sqmdDMBBuildShapes`, `SqsnScene_DrawMatrixShape`, `sqmdScriptCmd_skeleton_shape`
- **Named callees' callees:** `sqgxTextureClamp`, `SqgxContext_TestClipBounds`, `Unit10284EB0_NoOp`, `Unit1028D970_GetCenter`
- **How its callers call it** (their verified code):
  - in `sqsdGlow_BakeOntoShape`: `uVar1 = SqsdGlow_ApplyShader(param_1,(int)param_2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `SqgxContext_SetViewportVec4` (low): Stores a 4-component viewport value and marks it dirty.
  - `Unit102A1F20_TestSeparationHalf` (low): Tests an object's separation against half its difference length.

Short callee `SqgxContext_SetViewportVec4` (low):
```cpp
XI_API void __cdecl SqgxContext_SetViewportVec4(undefined4 * param_1)
{
  *(uint32_t *)(g_pSqgxContext_as_s32 + 0x9ac) = *param_1;
  *(uint32_t *)(g_pSqgxContext_as_s32 + 0x9b0) = param_1[1];
  *(uint32_t *)(g_pSqgxContext_as_s32 + 0x9b4) = param_1[2];
  *(uint32_t *)(g_pSqgxContext_as_s32 + 0x9b8) = param_1[3];
  g_sqgxViewportDirty_as_v0 = 1;
  return;
}
```

```cpp
XI_API undefined4 __cdecl SqsdGlow_ApplyShader(int param_1, int param_2)
{
  int iVar1;
  uint32_t uVar2;
  long double fVar3;
  char xi_frame[48] __attribute__((aligned(4)));
  uint32_t  &local_30 = *reinterpret_cast<uint32_t  *>(xi_frame + 0);
  float  &local_28 = *reinterpret_cast<float  *>(xi_frame + 8);
  float  &local_24 = *reinterpret_cast<float  *>(xi_frame + 12);
  float  &local_20 = *reinterpret_cast<float  *>(xi_frame + 16);
  float  (&local_1c)[7] = *reinterpret_cast<float  (*)[7]>(xi_frame + 20);
  
  local_1c[1] = 4.0f;
  local_1c[2] = 16.0f;
  local_1c[3] = 256.0f;
  local_1c[4] = 65536.0f;
  local_1c[5] = 4294969856.0f;
  local_1c[6] = 18446700093244440576.0f;
  if ((*(int *)(param_1 + 0x60) != 0) && (iVar1 = Unit102A1F20_TestSeparationHalf(param_2), iVar1 == 2)) {
    return 0;
  }
  local_30 = *(uint32_t *)(param_1 + 0x5c);
  fVar3 = (long double)*(float *)(param_1 + 0x58);
  if (local_30 != 0) {
    if (((int)local_30 < 0) || (5 < local_30)) {
      local_30 = 6;
    }
    fVar3 = fVar3 * (long double)local_1c[local_30];
  }
  local_28 = (float)(fVar3 * (long double)*(float *)(param_1 + 0x4c));
  local_24 = (float)(fVar3 * (long double)*(float *)(param_1 + 0x50));
  local_20 = (float)(fVar3 * (long double)*(float *)(param_1 + 0x54));
  local_1c[0] = (float)(long double)(int)local_30;
  SqgxContext_SetViewportVec4(&local_28);
  uVar2 = orig::sqgxShader_v1(0x400000,0);
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar2 >> 8)) << 8) | (uint8_t)(0xff)));
}
```
