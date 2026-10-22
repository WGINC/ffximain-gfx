# Batch r2_104_CYyObject_VT_1032A9F8: 2 functions of `CYyObject_VT_1032A9F8`

Name these functions: see `batches/README.md`. Write `data/names/r2_104_CYyObject_VT_1032A9F8.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10039550` fn_10039550

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Thread_ResumeAndWait` (medium)
- **Called by:** `Lobby_DestroyThreadPool`
- **Named before** (a naming session's reading, low): `ThreadPool_Shutdown`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `Lobby_DestroyThreadPool` → `ThreadPool_Shutdown`
- **How its callers call it** (their verified code):
  - in `Lobby_DestroyThreadPool`: `ThreadPool_Shutdown(DAT_10669098_as_v0);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Thread_ResumeAndWait` (medium): Resumes a worker thread when requested and waits for it to finish.

```cpp
XI_API void __thiscall ThreadPool_Shutdown(void * self_)
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
      Thread_ResumeAndWait((void *)*piVar3);
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
- **Named before** (a naming session's reading, low): `CYyObject_VT_1032A9F8_Construct`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `LobbyThread_Start` → `ThreadPool_StartThread` → `CYyObject_VT_1032A9F8_Construct`
- **Named callers' callers:** `LobbyThread_Start`
- **How its callers call it** (their verified code):
  - in `ThreadPool_StartThread`: `this_00 = (void *)CYyObject_VT_1032A9F8_Construct((void *)uVar1);`

```cpp
XI_API undefined4 __thiscall CYyObject_VT_1032A9F8_Construct(void * self_)
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
