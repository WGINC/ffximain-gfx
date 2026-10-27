# Batch r4_012_unit_10037AC0_SetMusicTrackVolume: 1 functions of `unit_10037AC0_SetMusicTrackVolume`

Name these functions: see `batches/README.md`. Write `data/names/r4_012_unit_10037AC0_SetMusicTrackVolume.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10038330` fn_10038330

- **Gate:** GATE: PASS -- 100 trials, 100 returned normally, 0 D3D calls compared (diffemu_gfx, clean build); **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Acter_IsEvent` (low), `LobbyThread_QueueSoundCmd` (low), `LobbyThread_QueryChannels` (low), `sub_10311C2C`
- **Called by:** `Unit10036B90_SetVolumeVector` (low), `sub_100382C0`
- **Writes globals:** `0x104638F4`
- **Reads globals:** `0x10456A94`, `0x104638F4`, `0x10463924`, `0x1046393C`
- **Strings its callees refer to:** `SeSep`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `sub_100016C0` → `sub_101B29F0` → `SetMusicTrackVolume_C` → `Unit10036B90_SetVolumeScalar` → `Unit10036B90_SetVolumeVector` → `fn_10038330`
- **Named callers' callers:** `Unit10036B90_SetVolumeScalar`
- **Named callees' callees:** `LobbyThread_NextSequence`, `LobbyThread_PushCmd`
- **How its callers call it** (their verified code):
  - in `Unit10036B90_SetVolumeVector` (low): `uVar2 = orig::FUN_10038330((int)self_ + 0x30,param_1,param_2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10036B90_SetVolumeVector` (low): Applies a volume vector to the object's sound when sound is active.
  - `Acter_IsEvent` (low): Returns the result of IsEvent for the given argument with 0xff as the first operand.
  - `LobbyThread_QueueSoundCmd` (low): Queues a sound command (1, or 2 for an "SeSep" entry) and returns its sequence number.
  - `LobbyThread_QueryChannels` (low): Answers queries about the channel records by id (found, values, active count).

Short callee `Acter_IsEvent` (low):
```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) Acter_IsEvent(undefined4 xi_eax)
{
  uint64_t uVar1;
  
  uVar1 = orig::IsEvent(0xff,xi_eax);
  return (int)uVar1;
}
```

```cpp
XI_API undefined4 __cdecl fn_10038330(int param_1, float * param_2, int param_3)
{
  uint32_t uVar1;
  int iVar2;
  int iVar3;
  int64_t lVar4;
  
  iVar3 = DAT_10456a94;
  if (DAT_1046393c_as_v0 != (void *)0x0) {
    if (param_3 < 0) {
      param_3 = 0;
    }
    else if (0x7f < param_3) {
      param_3 = 0x7f;
    }
    uVar1 = orig::Acter_IsEvent(DAT_1046393c_as_v0);
    if (((char)uVar1 != '\0') && (DAT_10463924 != '\0')) {
      iVar3 = 0x14;
    }
    iVar2 = orig::LobbyThread_QueryChannels_v1(5,-1,-1);
    if (iVar2 < iVar3) {
      if ((*(int *)(param_1 + 8) == 0x8bf) || (*(int *)(param_1 + 8) == 0x80f)) {
        if (DAT_104638f4_as_s8 != '\0') {
          return 0;
        }
        DAT_104638f4_as_s8 = '\x01';
      }
      lVar4 = XI_CALL_ST0(0x10311C2Cu)((long double)*param_2);
      uVar1 = orig::LobbyThread_QueueSoundCmd(DAT_1046393c_as_v0,(char *)param_1,(uint32_t)lVar4,param_3);
      return uVar1;
    }
  }
  return 0xffffffff;
}
```
