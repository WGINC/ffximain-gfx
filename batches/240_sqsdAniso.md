# Batch 240_sqsdAniso: 1 functions of `sqsdAniso`

Name these functions: see `batches/README.md`. Write `data/names/240_sqsdAniso.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102993B0` fn_102993B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10281C20` x2, `sub_10281C50` x2, `sqgxTextureClut` x2, `sqgxAlphaTest` x2, `fn_102827D0`, `sqgxBlendTrsp`, `sqgxBlendOff`, `sqgxContext_SelectShaderMode` x3, `sqDrawableDraw`, `fn_102912F0`, `fn_10298E10`, `fn_1029C460`, `fn_102A1FA0`, `fn_102A34D0`, `fn_102A3770`
- **Called by:** `sub_102A1F20`

```cpp
XI_API void __cdecl fn_102993B0(int * param_1, int * param_2)
{
  uint32_t uVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  uint32_t uVar5;
  char xi_frame[56] __attribute__((aligned(4)));
  uint32_t  &uVar6 = *reinterpret_cast<uint32_t  *>(xi_frame + 0);
  float  &local_24 = *reinterpret_cast<float  *>(xi_frame + 20);
  float  &local_20 = *reinterpret_cast<float  *>(xi_frame + 24);
  float  &local_1c = *reinterpret_cast<float  *>(xi_frame + 28);
  float  &local_18 = *reinterpret_cast<float  *>(xi_frame + 32);
  float  &local_14 = *reinterpret_cast<float  *>(xi_frame + 36);
  float  &local_10 = *reinterpret_cast<float  *>(xi_frame + 40);
  float  &local_c = *reinterpret_cast<float  *>(xi_frame + 44);
  float  &local_8 = *reinterpret_cast<float  *>(xi_frame + 48);
  float  &local_4 = *reinterpret_cast<float  *>(xi_frame + 52);
  
  uVar5 = 0;
  if ((param_1[0x14] == 0) || ((uVar1 = param_1[0x13], (uVar1 & 2) != 0 && ((uVar1 & 4) != 0)))) {
    return;
  }
  piVar2 = (int *)*param_2;
  if (*piVar2 == 0xa000b) {
    iVar4 = 0;
  }
  else {
    iVar4 = piVar2[0xd];
  }
  if ((uVar1 != 0) && (iVar3 = orig::FUN_102a1fa0((int)piVar2), iVar3 == 2)) {
    return;
  }
  if ((((param_1[0x16] != 0) && (*param_1 != 0xa000b)) && (iVar4 != 0)) &&
     (*(int *)(iVar4 + 100) == 0)) {
    orig::FUN_102912f0(*param_2);
  }
  if ((param_1[0x13] & 2U) != 0) {
    uVar5 = 0x4000;
  }
  if ((param_1[0x13] & 4U) != 0) {
    uVar5 = uVar5 | 0x8000;
  }
  orig::FUN_102a34d0((int)(param_1 + 5));
  if (param_1[0x18] == 0) {
    orig::sqgxShader_v1(uVar5 | 2,0);
  }
  else {
    orig::sqgxTextureSet_v1(*(int *)(param_1[0x18] + 0x14));
    orig::sqgxTextureSetPalette(*(int *)(param_1[0x18] + 0x18));
    orig::sqgxTextureDecal_v1(0);
    orig::sqgxShader_v1(uVar5 | 6,0);
    iVar4 = *(int *)(*(int *)(param_1[0x18] + 0x1c) + 0x24);
    if (((iVar4 == 2) || (iVar4 == 4)) || (*(int *)(*(int *)(param_1[0x18] + 0x1c) + 0x30) == 4)) {
      orig::sqgxBlendTrsp_v1();
    }
  }
  if (param_1[0x15] != 0) {
    orig::sqgxSetAlphaTest(1,0x5f);
  }
  orig::FUN_102a3770();
  orig::sqgxTextureDecal_v1(0);
  orig::FUN_102827d0();
  if (param_1[0x19] == 0) {
    local_c = (float)((long double)xi_castbits<float>(param_1[10]) * (long double)xi_castbits<float>(param_1[0xd]));
    local_8 = (float)((long double)xi_castbits<float>(param_1[0xb]) * (long double)xi_castbits<float>(param_1[0xd]));
    local_4 = (float)((long double)xi_castbits<float>(param_1[0xc]) * (long double)xi_castbits<float>(param_1[0xd]));
    local_18 = (float)((long double)xi_castbits<float>(param_1[10]) * (long double)xi_castbits<float>(param_1[0xe]));
    local_14 = (float)((long double)xi_castbits<float>(param_1[0xb]) * (long double)xi_castbits<float>(param_1[0xe]));
    local_10 = (float)((long double)xi_castbits<float>(param_1[0xc]) * (long double)xi_castbits<float>(param_1[0xe]));
    if ((char)param_1[0x12] == '\0') {
      local_24 = xi_castbits<float>(param_1[0xf]);
      local_20 = local_24;
      local_1c = local_24;
    }
    else {
      local_24 = (float)((long double)xi_castbits<float>(param_1[10]) * (long double)xi_castbits<float>(param_1[0xf]));
      local_20 = (float)((long double)xi_castbits<float>(param_1[0xb]) * (long double)xi_castbits<float>(param_1[0xf]));
      local_1c = (float)((long double)xi_castbits<float>(param_1[0xc]) * (long double)xi_castbits<float>(param_1[0xf]));
    }
    iVar4 = orig::FUN_10298e10(0x80,param_1[0x10],&local_c,&local_18,&local_24);
    param_1[0x19] = iVar4;
  }
  orig::sqgxShader_v1(uVar5 | 0x100000,0);
  orig::sqgxTextureSet_v1(*(int *)(param_1[0x19] + 0x14));
  orig::sqgxTextureSetPalette(-1);
  iVar4 = param_1[0x16];
  if (iVar4 == 0) {
LAB_102995bf:
    uVar6 = 1;
  }
  else {
    if (iVar4 != 1) {
      if (iVar4 != 2) goto LAB_102995c9;
      goto LAB_102995bf;
    }
    uVar6 = 0;
  }
  orig::FUN_1029c460(uVar6);
LAB_102995c9:
  orig::sqDrawableDraw_v1(*param_2);
  if (param_1[0x15] != 0) {
    orig::sqgxSetAlphaTest(0,0);
  }
  orig::sqgxBlendOff_v1();
  orig::FUN_102a3760();
  return;
}
```
