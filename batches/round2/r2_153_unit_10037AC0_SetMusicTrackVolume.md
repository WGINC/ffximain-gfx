# Batch r2_153_unit_10037AC0_SetMusicTrackVolume: 2 functions of `unit_10037AC0_SetMusicTrackVolume`

Name these functions: see `batches/README.md`. Write `data/names/r2_153_unit_10037AC0_SetMusicTrackVolume.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100382A0` fn_100382A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_102465E0`
- **Called by:** `sub_10037400`, `sub_100378B0`
- **Reads globals:** `0x1046393C`
- **Strings its callees refer to:** `%s\music%3.3u.bgw`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_100378B0` → `fn_100382A0`
- **Named callees' callees:** `operator_delete`, `Allocator_Generic`

```cpp
XI_API void __attribute__((cdecl, regparm(1))) fn_100382A0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2)
{
  if (DAT_1046393c != 0) {
    orig::FUN_102465e0(param_1,param_2,0);
  }
  return;
}
```

## `0x10038470` fn_10038470

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `LobbyThread_SendCmd4` (low)
- **Called by:** `fn_10035FB0`, `fn_100360F0`, `fn_100364E0`, `sub_10036C10`
- **Reads globals:** `0x1046393C`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `fn_10038470`
- **How its callers call it** (their verified code):
  - in `fn_10035FB0`: `uVar6 = orig::FUN_10038470(DAT_10463910_as_s32,(int)lVar5,uVar6,DAT_10463910_as_s32);`
  - in `fn_100360F0`: `uVar2 = orig::FUN_10038470(DAT_10463920,(int)lVar5,uVar2,DAT_10463920);`
  - in `fn_100364E0`: `uVar4 = orig::FUN_10038470(*(int *)((int)self_ + 0x1e0),local_20,0,local_20);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_SendCmd4` (low): Sends command 4 to the lobby thread and returns its result.

Short callee `LobbyThread_SendCmd4` (low):
```cpp
XI_API undefined4 __stdcall LobbyThread_SendCmd4(undefined4 param_1, uint32_t param_2, undefined4 param_3)
{
  uint64_t uVar1;
  
  uVar1 = orig::FUN_1024c060(4,param_1,0,param_2 & 0x7f,param_3,0,0,0);
  return (int)uVar1;
}
```

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_10038470(undefined4 xi_eax, int param_1, int param_2, undefined4 param_3)
{
  if (-1 < param_1) {
    if (param_2 < 0) {
      xi_eax = 0;
    }
    else {
      xi_eax = param_2;
      if (0x7f < param_2) {
        xi_eax = 0x7f;
      }
    }
    if (DAT_1046393c != 0) {
      xi_eax = LobbyThread_SendCmd4(param_1,xi_eax,param_3);
    }
  }
  return xi_eax;
}
```
