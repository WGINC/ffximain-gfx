# Batch 074_unit_100569D0_Scheduler: 3 functions of `unit_100569D0_Scheduler`

Name these functions: see `batches/README.md`. Write `data/names/074_unit_100569D0_Scheduler.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x10056AA0` fn_10056AA0

- **Gate:** none; **frame set:** stack
- **Facts:** entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10074CF0`
- **In vtables** (class_vtables.json): `CMoResource_VT_1032C644` slot 9 (+0x24)
- **Reads globals:** `0x1047D140`

```cpp
XI_API void __thiscall fn_10056AA0(void * self_)
{
  orig::FUN_10074cf0(dword_1047D140_as_v1,(int)self_,0x4d730000);
  orig::FUN_100720b0(self_);
  return;
}
```

## `0x10056CE0` fn_10056CE0

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10028FD0`, `fn_10057270`, `fn_10062830`, `fn_10062880`, `CMoTask::ResetFields`
- **Called by:** `sub_10056B20`, `sub_10056B80`, `sub_10056BB0`, `Scheduler_Execute`
- **Writes globals:** `0x1047D13C`

```cpp
XI_API int __thiscall fn_10056CE0(void * self_, int param_1, int param_2, undefined4 param_3, undefined4 param_4)
{
  uint32_t uVar1;
  void *pvVar2;
  uint64_t uVar3;
  
  uVar3 = orig::Mem_AllocFromArea_v1(0x14a,4);
  pvVar2 = (void *)uVar3;
  orig::CMoTask_ResetFields_v1(pvVar2);
  g_resourcesDirty_as_v0 = 1;
  if (pvVar2 == (void *)0x0) {
    pvVar2 = (void *)0x0;
  }
  else {
    pvVar2 = orig::FUN_10057270(pvVar2,self_,param_4,param_3);
  }
  uVar1 = *(uint32_t *)((int)self_ + 0x74);
  *(uint32_t *)((int)pvVar2 + 0x8c) = uVar1;
  *(uint32_t *)((int)pvVar2 + 0x88) = uVar1;
  if ((param_1 == 0) || (*(int *)(param_1 + 0x70) == 0)) {
    *(uint32_t *)((int)pvVar2 + 0x18) = 0;
  }
  else {
    *(uint32_t *)((int)pvVar2 + 0x18) = *(uint32_t *)(*(int *)(param_1 + 0x70) + 0x78);
  }
  *(uint32_t *)((int)pvVar2 + 0x1c) = 0x4d730000;
  if (param_1 != 0) {
    orig::FUN_10062830(pvVar2,param_1);
  }
  if (param_2 != 0) {
    orig::FUN_10062880(pvVar2,param_2);
  }
  *(uint32_t *)((int)pvVar2 + 0x94) = 0;
  *(uint32_t *)((int)pvVar2 + 0x98) = 0;
  return (int)pvVar2;
}
```

## `0x10056EB0` fn_10056EB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10074AB0`
- **Called by:** `CMoSchedularTask_Interpret`, `sub_1018A7A0`, `fn_1018ABF0`
- **Reads globals:** `0x1047D140`

```cpp
XI_API void __thiscall fn_10056EB0(void * self_)
{
  void *pvVar1;
  void *this_00;
  
LAB_10056eb9:
  this_00 = *(void **)(dword_1047D140 + 0xd50);
  do {
    if (this_00 == (void *)0x0) {
      return;
    }
    if (*(int *)((int)this_00 + 0x1c) == 0x4d730000) {
      if (*(int **)((int)this_00 + 0x80) == (int *)0x0) {
        pvVar1 = (void *)0x0;
      }
      else {
        pvVar1 = (void *)**(int **)((int)this_00 + 0x80);
      }
      if (pvVar1 == self_) break;
    }
    this_00 = *(void **)((int)this_00 + 0xc);
  } while( true );
  orig::FUN_10074ab0(this_00);
  goto LAB_10056eb9;
}
```
