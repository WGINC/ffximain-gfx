# Batch 085_CYyMotionQue: 1 functions of `CYyMotionQue`

Name these functions: see `batches/README.md`. Write `data/names/085_CYyMotionQue.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x1001B2E0` fn_1001B2E0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1001B0F0`, `fn_1001B5D0`
- **Called by:** `fn_1001A3E0`

```cpp
XI_API void __thiscall fn_1001B2E0(void * self_, int * param_1, int param_2)
{
  int *piVar1;
  int *this_00;
  int *this_01;
  int iVar2;
  
  this_01 = param_1;
  this_00 = *(int **)((int)self_ + 4);
  while (this_00 != (int *)0x0) {
    piVar1 = (int *)this_00[1];
    iVar2 = orig::FUN_1001b5d0(this_00);
    if (iVar2 == 0) {
      if ((piVar1 == (int *)0x0) && (param_2 < 3)) {
        this_00 = piVar1;
        if (this_01 != (int *)0x0) {
          ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*this_01 + 0x1fc))))((void *)(this_01),1);
        }
      }
      else {
        orig::FUN_1001b0f0(self_,this_00);
        this_00 = piVar1;
      }
    }
    else {
      ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*this_00 + 0x24))))((void *)(this_00),&param_1);
      this_00 = piVar1;
    }
  }
  return;
}
```
