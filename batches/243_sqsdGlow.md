# Batch 243_sqsdGlow: 1 functions of `sqsdGlow`

Name these functions: see `batches/README.md`. Write `data/names/243_sqsdGlow.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102999A0` fn_102999A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10283970`, `sqgxContext_SelectShaderMode`, `fn_102A1FA0`
- **Called by:** `sqsdGlow_BakeOntoShape`, `sub_102A1F20`

```cpp
XI_API undefined4 __cdecl fn_102999A0(int param_1, int param_2)
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
  if ((*(int *)(param_1 + 0x60) != 0) && (iVar1 = orig::FUN_102a1fa0(param_2), iVar1 == 2)) {
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
  orig::FUN_10283970(&local_28);
  uVar2 = orig::sqgxShader_v1(0x400000,0);
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar2 >> 8)) << 8) | (uint8_t)(0xff)));
}
```
