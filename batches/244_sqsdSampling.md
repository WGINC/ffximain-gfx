# Batch 244_sqsdSampling: 1 functions of `sqsdSampling`

Name these functions: see `batches/README.md`. Write `data/names/244_sqsdSampling.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10299090` fn_10299090

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10281C20` x3, `sub_10281C50`, `sqgxTextureClamp`, `sqgxTextureClut` x2, `sqgxZMask` x2, `sqgxBlendAdd`, `sqgxBlendTrsp`, `sqgxBlendAddDestAlpha`, `sqgxBlendOff`, `fn_10283F20`, `sqgxPassAdd` x2, `sqgxContext_SelectShaderMode` x2, `sqDrawableDraw` x2, `fn_1029BCD0`, `fn_1029BD40`, `fn_102A1FA0`, `fn_102A34D0`
- **Called by:** `sub_102A1F20`

```cpp
XI_API void __cdecl fn_10299090(int param_1, int * param_2)
{
  uint32_t uVar1;
  int iVar2;
  int iVar3;
  uint32_t uVar4;
  uint16_t uVar5;
  
  uVar1 = *(uint32_t *)(param_1 + 0x4c);
  uVar4 = 0x40000;
  if ((((uVar1 & 2) == 0) || ((uVar1 & 4) == 0)) &&
     ((uVar1 == 0 || (iVar3 = orig::FUN_102a1fa0(*param_2), iVar3 != 2)))) {
    orig::FUN_102a34d0(param_1 + 0x14);
    orig::FUN_1029bd40(*(float *)(param_1 + 0x58));
    orig::FUN_1029bcd0(param_1 + 0x5c);
    if (*(int *)(param_1 + 0x50) == 0) {
      orig::sqgxTextureSet_v1(-1);
    }
    else {
      iVar3 = *(int *)(*(int *)(param_1 + 0x50) + 0x1c);
      iVar2 = *(int *)(iVar3 + 0x24);
      if ((((iVar2 == 2) || (iVar2 == 4)) || (*(int *)(iVar3 + 0x30) == 4)) &&
         ((*(uint8_t *)(param_1 + 0x164) & 1) != 0)) {
        orig::sqgxBlendTrsp_v1();
      }
      orig::sqgxTextureClamp_v1(*(uint32_t *)(param_1 + 0x168));
      orig::sqgxTextureDecal_v1(0);
      orig::sqgxTextureSet_v1(*(int *)(*(int *)(param_1 + 0x50) + 0x14));
      orig::sqgxTextureSetPalette(*(int *)(*(int *)(param_1 + 0x50) + 0x18));
      uVar4 = 0x40004;
    }
    uVar5 = (uint16_t)((uint32_t)uVar4 >> 0x10);
    if ((*(uint32_t *)(param_1 + 0x4c) & 2) == 0) {
      if ((*(uint32_t *)(param_1 + 0x4c) & 4) != 0) {
        uVar4 = ((uint32_t)(((uint32_t)(uint16_t)(uVar5) << 16) | (uint16_t)(((uint16_t)(((uint16_t)(uint8_t)(0x80) << 8) | (uint8_t)((char)uVar4))))));
      }
    }
    else {
      uVar4 = ((uint32_t)(((uint32_t)(uint16_t)(uVar5) << 16) | (uint16_t)(((uint16_t)(((uint16_t)(uint8_t)(0x40) << 8) | (uint8_t)((char)uVar4))))));
    }
    orig::sqgxShader_v1(uVar4,0);
    orig::sqDrawableDraw_v1(*param_2);
    orig::sqgxPassAdd_v1(1);
    orig::sqgxZMask_v1(0);
    if ((*(int *)(param_1 + 0x54) != 0) &&
       ((!NAN((long double)*(float *)(param_1 + 0x160)) && !NAN((long double)xi_k8(0x3F50624DD2F1A9FCull))) &&
        (long double)xi_k8(0x3F50624DD2F1A9FCull) < (long double)*(float *)(param_1 + 0x160))) {
      orig::FUN_10283f20(*(float *)(param_1 + 0x15c));
      if ((*(uint8_t *)(param_1 + 0x164) & 2) == 0) {
        orig::sqgxBlendAdd();
      }
      else {
        orig::sqgxBlendAddDestAlpha();
      }
      orig::sqgxTextureSet_v1(*(int *)(*(int *)(param_1 + 0x54) + 0x14));
      orig::sqgxTextureSetPalette(*(int *)(*(int *)(param_1 + 0x54) + 0x18));
      uVar4 = 0x10;
      if ((*(uint32_t *)(param_1 + 0x4c) & 2) == 0) {
        if ((*(uint32_t *)(param_1 + 0x4c) & 4) != 0) {
          uVar4 = 0x8010;
        }
      }
      else {
        uVar4 = 0x4010;
      }
      orig::sqgxShader_v1(uVar4,0);
      orig::sqDrawableDraw_v1(*param_2);
    }
    orig::sqgxBlendOff_v1();
    orig::sqgxPassAdd_v1(-1);
    orig::sqgxZMask_v1(1);
  }
  return;
}
```
