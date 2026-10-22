# Batch 195_CYyObject_VT_1032A9F8: 2 functions of `CYyObject_VT_1032A9F8`

Name these functions: see `batches/README.md`. Write `data/names/195_CYyObject_VT_1032A9F8.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10039550` fn_10039550

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_100396A0`
- **Called by:** `Lobby_DestroyThreadPool`

```cpp
XI_API void __thiscall fn_10039550(void * self_)
{
  int *piVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = (int *)((int)self_ + 0xc);
  iVar2 = 8;
  piVar1 = piVar3;
  do {
    if (*piVar1 != 0) {
      *(uint8_t *)(*piVar1 + 5) = 1;
    }
    piVar1 = piVar1 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  iVar2 = 8;
  do {
    if ((void *)*piVar3 != (void *)0x0) {
      orig::FUN_100396a0((void *)*piVar3);
    }
    piVar3 = piVar3 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  return;
}
```

## `0x100395D0` fn_100395D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `ThreadPool_StartThread`

```cpp
XI_API undefined4 __thiscall fn_100395D0(void * self_)
{
  *(uint8_t ***)self_ = &PTR_CYyObject_VT_1032A9F8_ScalarDeletingDtor_2_1032aa14;
  *(uint8_t *)((int)self_ + 4) = 1;
  *(uint8_t *)((int)self_ + 5) = 0;
  *(uint8_t *)((int)self_ + 6) = 0;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint32_t *)((int)self_ + 0xc) = 0;
  return self_;
}
```
