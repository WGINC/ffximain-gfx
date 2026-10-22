# Batch 066_unit_10019A50_MotionQueue: 2 functions of `unit_10019A50_MotionQueue`

Name these functions: see `batches/README.md`. Write `data/names/066_unit_10019A50_MotionQueue.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1001A3E0` fn_1001A3E0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1001AC40`, `fn_1001B1F0`, `fn_1001B2E0`
- **Called by:** `sub_1002A130`

```cpp
XI_API void __thiscall fn_1001A3E0(void * self_, undefined4 param_1)
{
  int iVar1;
  void *this_00;
  
  iVar1 = 6;
  this_00 = (void *)((int)self_ + 0x78);
  do {
    orig::FUN_1001b2e0(this_00,param_1,iVar1);
    orig::FUN_1001ac40(this_00);
    orig::FUN_1001b1f0(this_00,param_1,iVar1);
    iVar1 = iVar1 + -1;
    this_00 = (void *)((int)this_00 + -0x14);
  } while (-1 < iVar1);
  return;
}
```

## `0x1001A6E0` fn_1001A6E0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 40`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1001AD00`, `sub_10084620`
- **Called by:** `CXiSkeletonActor::SetMotion`

```cpp
XI_API void __thiscall fn_1001A6E0(void * self_, int * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, undefined4 param_7, int param_8, undefined4 param_9, undefined4 param_10)
{
  uint32_t uVar1;
  uint32_t uVar2;
  
  uVar1 = orig::FUN_10084620(param_1);
  if ((uVar1 != 0) && (uVar2 = ((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x200))))((void *)(param_1)), (char)uVar2 == '\0')) {
    return;
  }
  if ((((-1 < param_8) && (param_8 < 7)) &&
      (orig::KzMotionQueList_AppendSync((void *)((int)self_ + param_8 * 0x14),param_2,param_3,param_4,param_5,param_6,
                    param_7,param_9,param_10), param_8 == 0)) && (param_1 != (int *)0x0)) {
    ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*param_1 + 0x1fc))))((void *)(param_1),0);
  }
  return;
}
```
