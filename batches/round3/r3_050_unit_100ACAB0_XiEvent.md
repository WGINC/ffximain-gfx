# Batch r3_050_unit_100ACAB0_XiEvent: 1 functions of `unit_100ACAB0_XiEvent`

Name these functions: see `batches/README.md`. Write `data/names/r3_050_unit_100ACAB0_XiEvent.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100AD7C0` fn_100AD7C0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_102203B0`
- **Reads globals:** `0x10482F43`, `0x104DFD98`

```cpp
XI_API undefined4 __cdecl fn_100AD7C0(void)
{
  uint32_t xi_entry_eax = 0;
  uint32_t uVar1;
  
  uVar1 = (uint32_t)((uint32_t)xi_entry_eax >> 8);
  if (DAT_10482f43 == '\0') {
    return (uint32_t)uVar1 << 8;
  }
  if (DAT_104dfd98 == 0) {
    return (uint32_t)uVar1 << 8;
  }
  uVar1 = (uint32_t)(*(uint16_t *)(DAT_104dfd98 + 0x40d7c) - 1 >> 8);
  switch(*(uint16_t *)(DAT_104dfd98 + 0x40d7c)) {
  case 1:
  case 3:
  case 0x2e:
  case 0x2f:
  case 0x3a:
  case 0x3b:
  case 0x3c:
  case 0xdc:
  case 0xdd:
  case 0xdf:
  case 0xe0:
  case 0xe1:
  case 0xe2:
  case 0xe3:
  case 0xe4:
    return (uint32_t)uVar1 << 8;
  default:
    return ((uint32_t)(((uint32_t)(uVar1) << 8) | (uint8_t)(1)));
  }
}
```
