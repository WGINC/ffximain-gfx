# Batch 289_unit_1005FFD0_AfterimageTask: 6 functions of `unit_1005FFD0_AfterimageTask`

Name these functions: see `batches/README.md`. Write `data/names/289_unit_1005FFD0_AfterimageTask.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10062770` fn_10062770

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0` x2
- **Called by:** `Scheduler_Kill`, `sub_10056F70`, `Scheduler_IsMoving`, `sub_100572E0`, `sub_100575A0`, `CMoSchedularTask_Interpret`

```cpp
XI_API uint64_t __thiscall fn_10062770(void * self_)
{
  char cVar1;
  void *this_00;
  uint64_t uVar2;
  uint64_t uVar3;
  
  uVar2 = orig::FUN_1003b6d0((void *)((int)self_ + 0x34));
  this_00 = (void *)uVar2;
  if (this_00 != (void *)0x0) {
    uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
    uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
    cVar1 = (char)uVar3;
    while (cVar1 != '\0') {
      uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
      this_00 = (void *)uVar2;
      if (this_00 == (void *)0x0) {
        uVar2 = orig::FUN_1003b6d0((void *)((int)self_ + 0x34));
        return uVar2;
      }
      uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
      uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
      cVar1 = (char)uVar3;
    }
  }
  return uVar2;
}
```

## `0x100627D0` fn_100627D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0`, `sub_1003B700`
- **Called by:** `Scheduler_Kill`, `sub_10056F70`, `Scheduler_IsMoving`, `sub_100572E0`, `sub_100575A0`, `CMoSchedularTask_Interpret`

```cpp
XI_API uint64_t __thiscall fn_100627D0(void * self_)
{
  char cVar1;
  void *this_00;
  uint64_t uVar2;
  uint64_t uVar3;
  
  uVar2 = orig::FUN_1003b700((void *)((int)self_ + 0x34));
  this_00 = (void *)uVar2;
  if (this_00 != (void *)0x0) {
    uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
    uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
    cVar1 = (char)uVar3;
    while (cVar1 != '\0') {
      uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
      this_00 = (void *)uVar2;
      if (this_00 == (void *)0x0) {
        uVar2 = orig::FUN_1003b700((void *)((int)self_ + 0x34));
        return uVar2;
      }
      uVar3 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
      uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar3 >> 0x20)) << 32) | (uint32_t)(this_00)));
      cVar1 = (char)uVar3;
    }
  }
  return uVar2;
}
```

## `0x10062830` fn_10062830

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1003B6D0` x4, `CMoAttachments::SetCaster`
- **Called by:** `fn_10056CE0`, `CMoSchedularTask_Interpret`

```cpp
XI_API void __thiscall fn_10062830(void * self_, int param_1)
{
  void *this_00;
  uint64_t uVar1;
  
  this_00 = (void *)((int)self_ + 0x34);
  uVar1 = orig::FUN_1003b6d0(this_00);
  if ((int)uVar1 != 0) {
    uVar1 = orig::FUN_1003b6d0(this_00);
    ((int (__thiscall *)(void *))((**(code **)(*(int *)uVar1 + 0x16c))))((void *)((int *)uVar1));
  }
  orig::CMoAttachments_SetCaster_v1(this_00,param_1);
  uVar1 = orig::FUN_1003b6d0(this_00);
  if ((int)uVar1 != 0) {
    uVar1 = orig::FUN_1003b6d0(this_00);
    ((int (__thiscall *)(void *))((**(code **)(*(int *)uVar1 + 0x168))))((void *)((int *)uVar1));
  }
  return;
}
```

## `0x10062880` fn_10062880

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1003B700` x4, `CMoAttachments::SetTarget`
- **Called by:** `fn_10056CE0`, `CMoSchedularTask_Interpret`

```cpp
XI_API void __thiscall fn_10062880(void * self_, int param_1)
{
  void *this_00;
  uint64_t uVar1;
  
  this_00 = (void *)((int)self_ + 0x34);
  uVar1 = orig::FUN_1003b700(this_00);
  if ((int)uVar1 != 0) {
    uVar1 = orig::FUN_1003b700(this_00);
    ((int (__thiscall *)(void *))((**(code **)(*(int *)uVar1 + 0x16c))))((void *)((int *)uVar1));
  }
  orig::CMoAttachments_SetTarget_v1(this_00,param_1);
  uVar1 = orig::FUN_1003b700(this_00);
  if ((int)uVar1 != 0) {
    uVar1 = orig::FUN_1003b700(this_00);
    ((int (__thiscall *)(void *))((**(code **)(*(int *)uVar1 + 0x168))))((void *)((int *)uVar1));
  }
  return;
}
```

## `0x10062E20` fn_10062E20

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Scheduler_Execute`, `CMoResource::FindByTypeAndId`, `sub_100731B0`
- **Called by:** `sub_1008ABE0`, `fn_1009F4C0`, `sub_100B4390`, `sub_100B8B20`, `sub_101420D0`, `sub_101D32C0`, `sub_101D52D0`, `sub_101D55D0`, `sub_10239240`, `sub_1023E4F0`, `sub_1023E550`, `sub_1023E5B0`, `sub_1023E5D0`, `sub_1024CAE0`, `__attribute__`
- **Reads globals:** `0x1047D128`

```cpp
XI_API uint64_t __cdecl fn_10062E20(undefined4 * param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5)
{
  uint8_t *pbVar1;
  int *piVar2;
  uint32_t *puVar3;
  uint64_t uVar4;
  
  puVar3 = (uint32_t *)orig::FUN_100731b0(g_pMeshResourceMng_as_v1,&param_1,param_1);
  piVar2 = (int *)*puVar3;
  pbVar1 = (uint8_t *)(*piVar2 + 0x1a);
  *pbVar1 = *pbVar1 | 0x10;
  uVar4 = orig::CMoResource_FindByTypeAndId_v1((void *)*piVar2,&param_1,7,param_2);
  puVar3 = (uint32_t *)*(int *)uVar4;
  uVar4 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar4 >> 0x20)) << 32) | (uint32_t)(puVar3)));
  if (puVar3 != (uint32_t *)0x0) {
    uVar4 = orig::Scheduler_Execute_v1((void *)*puVar3,param_3,param_4,param_5,0,(void *)*puVar3);
  }
  return uVar4;
}
```

## `0x10062E80` fn_10062E80

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Scheduler_Kill`, `CMoResource::FindByTypeAndId`, `sub_10073D80`
- **Called by:** `sub_1008ABE0`, `fn_1009F4C0`, `sub_100B49D0`
- **Reads globals:** `0x1047D128`

```cpp
XI_API uint64_t __cdecl fn_10062E80(undefined4 param_1, undefined4 param_2, int param_3, int param_4)
{
  uint32_t *puVar1;
  uint64_t uVar2;
  
  uVar2 = orig::FUN_10073d80(&param_1,param_1,1);
  puVar1 = (uint32_t *)*(int *)uVar2;
  uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar2 >> 0x20)) << 32) | (uint32_t)(puVar1)));
  if (puVar1 != (uint32_t *)0x0) {
    uVar2 = orig::CMoResource_FindByTypeAndId_v1((void *)*puVar1,&param_1,7,param_2);
    puVar1 = (uint32_t *)*(int *)uVar2;
    uVar2 = ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar2 >> 0x20)) << 32) | (uint32_t)(puVar1)));
    if (puVar1 != (uint32_t *)0x0) {
      uVar2 = orig::Scheduler_Kill((void *)*puVar1,param_3,param_4);
    }
  }
  return uVar2;
}
```
