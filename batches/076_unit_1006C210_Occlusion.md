# Batch 076_unit_1006C210_Occlusion: 2 functions of `unit_1006C210_Occlusion`

Name these functions: see `batches/README.md`. Write `data/names/076_unit_1006C210_Occlusion.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1006CA20` fn_1006CA20

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1031450A`
- **Called by:** `sub_1003FEE0`, `sub_100553C0`, `fn_1006CA60`, `sub_1006CA90`, `sub_1006CAD0`, `CXiCollisionActor_OnMove`, `CXiSkeletonActor_OnMove`

```cpp
XI_API undefined4 __cdecl fn_1006CA20(float param_1)
{
  uint32_t uVar1;
  uint32_t uVar2;
  
  uVar1 = orig::FUN_1031450a(SUB84((double)(long double)param_1,0),
                       (uint32_t)(xi_castbits<uint64_t>((double)(long double)param_1) >> 0x20));
  uVar2 = uVar1;
  if ((int)uVar1 < 0x11) {
    if ((uVar1 != 0x10) &&
       (((uVar2 = 0, uVar1 != 1 && (uVar2 = 0, uVar1 != 2)) &&
        (uVar1 = uVar1 - 4, uVar2 = 0, uVar1 != 0)))) {
LAB_1006ca55:
      return ((uint32_t)(((uint32_t)((int32_t)(uVar1 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  else if ((uVar1 != 0x80) && (uVar1 != 0x200)) goto LAB_1006ca55;
  return uVar2 & 0xffffff00;
}
```

## `0x1006CA60` fn_1006CA60

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1006CA20`
- **Called by:** `fn_10021C10`, `sub_1006CB20`

```cpp
XI_API undefined4 __cdecl fn_1006CA60(undefined4 * param_1)
{
  uint32_t uVar1;
  uint32_t uVar2;
  int iVar3;
  
  iVar3 = 0;
  do {
    uVar1 = orig::FUN_1006ca20(xi_castbits<float>(*param_1));
    uVar2 = (uint32_t)((uint32_t)uVar1 >> 8);
    if ((char)uVar1 == '\0') {
      return (uint32_t)uVar2 << 8;
    }
    iVar3 = iVar3 + 1;
    param_1 = param_1 + 1;
  } while (iVar3 < 0x10);
  return ((uint32_t)(((uint32_t)(uVar2) << 8) | (uint8_t)(1)));
}
```
