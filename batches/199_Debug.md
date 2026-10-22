# Batch 199_Debug: 2 functions of `Debug`

Name these functions: see `batches/README.md`. Write `data/names/199_Debug.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10003660` fn_10003660

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_100035F0`

```cpp
XI_API void __thiscall fn_10003660(void * self_)
{
  *(uint8_t ***)self_ = &PTR_FUN_10329660;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  *(uint32_t *)((int)self_ + 0x10) = 0;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  return;
}
```

## `0x10003710` fn_10003710

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100037D0`, `fn_10003890`, `Debug_DrawTextTable`, `Allocator_Generic` x4
- **Called by:** `DebugOverlay_Draw`

```cpp
XI_API void __thiscall fn_10003710(void * self_)
{
  int iVar1;
  uint32_t uVar2;
  uint32_t *puVar3;
  uint64_t uVar4;
  char xi_frame[8] __attribute__((aligned(4)));
  uint32_t  (&local_8)[2] = *reinterpret_cast<uint32_t  (*)[2]>(xi_frame + 0);
  
  if (*(char *)((int)self_ + 4) != '\0') {
    if (*(int *)((int)self_ + 8) == 0) {
      uVar4 = orig::Allocator_Generic_v1(0x4800);
      *(int *)((int)self_ + 0xc) = (int)uVar4;
      uVar4 = orig::Allocator_Generic_v1(0x9000);
      *(int *)((int)self_ + 0x10) = (int)uVar4;
      uVar4 = orig::Allocator_Generic_v1(0x240000);
      *(int *)((int)self_ + 0x14) = (int)uVar4;
      uVar4 = orig::Allocator_Generic_v1(0x2400);
      *(int *)((int)self_ + 0x18) = (int)uVar4;
      puVar3 = *(uint32_t **)((int)self_ + 0x14);
      for (iVar1 = 0x90000; iVar1 != 0; iVar1 = iVar1 + -1) {
        *puVar3 = 0;
        puVar3 = puVar3 + 1;
      }
      uVar2 = 0;
      do {
        local_8[1] = 0xffffffff;
        local_8[0] = uVar2;
        orig::FUN_100037d0(self_,local_8,0.0f,0.0f,0.0f,0.0f,&DAT_1034f6bc,0x80808080,local_8);
        uVar2 = uVar2 + 1;
      } while ((int)uVar2 < 0x900);
      *(int *)((int)self_ + 8) = *(int *)((int)self_ + 8) + 1;
    }
    else if (*(int *)((int)self_ + 8) == 1) {
      orig::FUN_10003890(self_);
      orig::Debug_DrawTextTable(self_);
      return;
    }
  }
  return;
}
```
