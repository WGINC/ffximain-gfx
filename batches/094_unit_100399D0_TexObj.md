# Batch 094_unit_100399D0_TexObj: 1 functions of `unit_100399D0_TexObj`

Name these functions: see `batches/README.md`. Write `data/names/094_unit_100399D0_TexObj.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x100399D0` fn_100399D0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1003AB10`
- **Called by:** `sub_1001DBD0`, `sub_1001E000`
- **Reads globals:** `0x1047B970`

```cpp
XI_API void __thiscall fn_100399D0(void * self_)
{
  int *piVar1;
  
  piVar1 = orig::FUN_1003ab10(g_pTextureMng_as_v2,(int)self_);
  if (piVar1 != (int *)0x0) {
    *piVar1 = *(int *)(*piVar1 + 0x38);
    piVar1 = *(int **)((int)self_ + 0x40);
    if (piVar1 != (int *)0x0) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
      *(uint32_t *)((int)self_ + 0x40) = 0;
    }
    piVar1 = *(int **)((int)self_ + 0x44);
    if (piVar1 != (int *)0x0) {
      ((int (__stdcall *)(XI_ANY))((**(code **)(*piVar1 + 8))))(piVar1);
      *(uint32_t *)((int)self_ + 0x44) = 0;
    }
    *(uint32_t *)((int)self_ + 0x38) = 0;
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x18))))((void *)(self_),1);
  }
  return;
}
```
