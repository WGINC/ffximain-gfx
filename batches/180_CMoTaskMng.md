# Batch 180_CMoTaskMng: 1 functions of `CMoTaskMng`

Name these functions: see `batches/README.md`. Write `data/names/180_CMoTaskMng.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10074BA0` fn_10074BA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoTask::ResetFields` x3, `fn_10074DF0`
- **Called by:** `App_InitScene`

```cpp
XI_API void __thiscall fn_10074BA0(void * self_)
{
  void *this_00;
  int iVar1;
  int iVar2;
  void *pvVar3;
  int *piVar4;
  
  this_00 = (void *)((int)self_ + 4);
  orig::CMoTask_ResetFields_v1(this_00);
  *(int *)((int)self_ + 0x10) = (int)self_ + 0x38;
  iVar2 = 0x3e;
  pvVar3 = this_00;
  do {
    orig::CMoTask_ResetFields_v1((void *)((int)pvVar3 + 0x34));
    *(void **)((int)pvVar3 + 0x3c) = pvVar3;
    *(int *)((int)pvVar3 + 0x40) = (int)pvVar3 + 0x68;
    iVar2 = iVar2 + -1;
    pvVar3 = (void *)((int)pvVar3 + 0x34);
  } while (iVar2 != 0);
  orig::CMoTask_ResetFields_v1((void *)((int)self_ + 0xcd0));
  *(int *)((int)self_ + 0xcd8) = (int)self_ + 0xc9c;
  *(void **)((int)self_ + 0xd58) = this_00;
  *(uint32_t *)((int)self_ + 0xd54) = 0;
  *(uint32_t *)((int)self_ + 0xd50) = 0;
  *(uint32_t *)((int)self_ + 0xd44) = 0;
  piVar4 = (int *)((int)self_ + 0xd04);
  iVar2 = 0x10;
  do {
    iVar1 = orig::FUN_10074df0(self_,0);
    *piVar4 = iVar1;
    piVar4 = piVar4 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(uint32_t *)((int)self_ + 0xd4c) = *(uint32_t *)((int)self_ + 0xd50);
  return;
}
```
