# Batch r2_106_StChannel: 5 functions of `StChannel`

Name these functions: see `batches/README.md`. Write `data/names/r2_106_StChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10251070` fn_10251070

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10017D20`, `fn_1026EF00`, `sub_1026EFB0`, `sub_1026F080`, `Mdl_GetEntryTable` (medium), `Mdl_UpdateEntryFlags` (low), `sub_10311C2C`
- **Called by:** `StAvatar_ApplyChannels` (low), `sub_1024DA10`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `StAvatar_ApplyChannels` → `fn_10251070`
- **Named callers' callers:** `StAvatar_Unload`
- **Named callees' callees:** `scnCameraGetInfo`, `Unit1027D980_InitDefaults`, `Chn_RegisterItem`, `sqDebugPrintf`, `Unit1028D820_CreateFromFields`
- **How its callers call it** (their verified code):
  - in `StAvatar_ApplyChannels` (low): `orig::FUN_10251070(puVar3,iVar1 >> 1,*(float *)(param_1 + 0x78));`
  - in `StAvatar_ApplyChannels` (low): `orig::FUN_10251070((uint32_t *)(pbVar2 + -8),-1,*pfVar4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_ApplyChannels` (low): Applies the avatar's channel values for the current time.
  - `Mdl_GetEntryTable` (medium): Returns the entry table and its count when the table is populated.
  - `Mdl_UpdateEntryFlags` (low): Sets, clears or toggles the flag mask 0x32C in a table entry by mode.

Short callee `fn_10017D20`:
```cpp
XI_API void __cdecl fn_10017D20(undefined4 xi_s1, ...)
{
  return;
}
```

```cpp
XI_API uint32_t __cdecl fn_10251070(uint32_t * param_1, int param_2, float param_3)
{
  long double fVar1;
  uint32_t *puVar2;
  uint32_t *puVar3;
  uint32_t uVar4;
  long double fVar5;
  int64_t lVar6;
  
  puVar2 = param_1;
  if (param_1[6] == 0) {
    orig::nullsub_13_v1(s_____Error__StChannelItemRegister_10389830);
  }
  else {
    uVar4 = orig::FUN_1026ef00(param_1[2],param_1[6]);
    puVar2[3] = uVar4;
    if (-1 < (int)uVar4) {
      *puVar2 = *puVar2 | 0x10;
      Mdl_UpdateEntryFlags(uVar4,0x200,1);
      if (param_2 < 0) {
        orig::FUN_1026f080(puVar2[2],puVar2[3],0);
      }
      else {
        orig::FUN_1026efb0(puVar2[3],param_2);
      }
      Mdl_GetEntryTable((int *)&param_1,(int *)0x0);
      puVar3 = param_1;
      uVar4 = puVar2[3];
      param_1[uVar4 * 0x30 + 0x28] = 0x3f800000;
      if ((!NAN((long double)param_3) && !NAN((long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x29]))) &&
          (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x29]) <= (long double)param_3) {
        fVar5 = (long double)param_3;
        fVar1 = (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x2a]);
        if (fVar5 <= fVar1) {
          param_1[uVar4 * 0x30 + 0x2b] = xi_castbits<uint32_t>(param_3);
          lVar6 = XI_CALL_ST0(0x10311C2Cu)((long double)param_3 / (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x2c]));
          puVar3[uVar4 * 0x30 + 0x26] = (int)lVar6 * 2;
        }
      }
      return puVar2[3];
    }
  }
  return 0xffffffff;
}
```

## `0x10251160` fn_10251160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `StAvatar_Reset` (medium)
- **Named before** (a naming session's reading, low): `StChannel_ClearFlags`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_SetStateTeardown` → `StAvatar_Teardown` → `StAvatar_Reset` → `StChannel_ClearFlags`
- **Named callers' callers:** `StAvatar_Teardown`, `StAvatar_Unload`
- **How its callers call it** (their verified code):
  - in `StAvatar_Reset` (medium): `StChannel_ClearFlags(puVar2);`
  - in `StAvatar_Reset` (medium): `StChannel_ClearFlags(puVar2 + -0x1c);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_Reset` (medium): Resets an avatar's indices and channel entries.

```cpp
XI_API undefined4 __cdecl StChannel_ClearFlags(uint32_t * param_1)
{
  param_1[3] = 0xffffffff;
  *param_1 = *param_1 & 0xffffffed;
  return 0;
}
```

## `0x10251180` fn_10251180

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101C4A30`
- **Called by:** `StAvatar_StopAndFreeBuffers`
- **Reads globals:** `0x10662DCC`
- **Named before** (a naming session's reading, low): `StChannel_MarkIfLoaded`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pFileMng`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `StAvatar_SetActive` → `StAvatarFini` → `StAvatar_StopAndFreeBuffers` → `StChannel_MarkIfLoaded`
- **Named callers' callers:** `StAvatar_Teardown`, `StAvatar_SetPending`, `StAvatarFini`
- **How its callers call it** (their verified code):
  - in `StAvatar_StopAndFreeBuffers`: `StChannel_MarkIfLoaded(puVar4);`
  - in `StAvatar_StopAndFreeBuffers`: `StChannel_MarkIfLoaded(puVar4 + -0x1c);`

Short callee `fn_101C4A30`:
```cpp
XI_API undefined4 __thiscall fn_101C4A30(void * self_, uint32_t param_1)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_101c13a0(*(void **)((int)self_ + 4),param_1);
  return uVar1;
}
```

```cpp
XI_API undefined4 __cdecl StChannel_MarkIfLoaded(uint32_t * param_1)
{
  uint32_t uVar1;
  
  if (((*param_1 & 1) != 0) && (-1 < (int)param_1[1])) {
    uVar1 = orig::FUN_101c4a30(g_pFileMng_as_v1,param_1[1]);
    if ((char)uVar1 != '\0') {
      *param_1 = *param_1 | 0x20;
    }
  }
  return 0;
}
```

## `0x102511B0` fn_102511B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Mdl_GetEntryTable` (medium)
- **Called by:** `StAvatar_AdvanceTimers` (low)
- **Named before** (a naming session's reading, low): `StChannel_SetValue9C`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `StAvatar_AdvanceTimers` → `StChannel_SetValue9C`
- **How its callers call it** (their verified code):
  - in `StAvatar_AdvanceTimers` (low): `StChannel_SetValue9C((uint8_t *)(puVar4 + -2),(float)((long double)1.0f / (long double)fVar1));`
  - in `StAvatar_AdvanceTimers` (low): `StChannel_SetValue9C((uint8_t *)(puVar4 + -2),(float)((long double)1.0f / (long double)fVar1));`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatar_AdvanceTimers` (low): Advances the avatar's timers and clamps them to their range.
  - `Mdl_GetEntryTable` (medium): Returns the entry table and its count when the table is populated.

```cpp
XI_API undefined4 __cdecl StChannel_SetValue9C(uint8_t * param_1, float param_2)
{
  int iVar1;
  uint8_t *pbVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  int  &local_4 = *reinterpret_cast<int  *>(xi_frame + 0);
  
  pbVar2 = param_1;
  if ((*param_1 & 1) != 0) {
    Mdl_GetEntryTable(&local_4,(int *)&param_1);
    iVar1 = *(int *)(pbVar2 + 0xc);
    if ((-1 < iVar1) && (iVar1 < (int)param_1)) {
      *(float *)(iVar1 * 0xc0 + 0x9c + local_4) = (float)(long double)param_2;
    }
  }
  return 0;
}
```

## `0x10251200` fn_10251200

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Mdl_GetEntryTable` (medium), `Chn_ForwardIfFlag2` (low), `Unit102857D0_GetFlagByte14` (low), `sub_102857E0` x2, `Unit102857D0_InvokeCallback18` (low)
- **Called by:** `fn_1024DB60`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `fn_10251200`
- **Named callers' callers:** `Dancer_WorldBegin`
- **How its callers call it** (their verified code):
  - in `fn_1024DB60`: `orig::FUN_10251200(pbVar4,uVar2,0);`
  - in `fn_1024DB60`: `orig::FUN_10251200(param_1 + 0x160,uVar2,0);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Mdl_GetEntryTable` (medium): Returns the entry table and its count when the table is populated.
  - `Chn_ForwardIfFlag2` (low): Forwards to a handler when the channel's flag 2 is set.
  - `Unit102857D0_GetFlagByte14` (low): Returns the object's byte field at +0x14.
  - `Unit102857D0_InvokeCallback18` (low): Calls the object's callback at +0x18 when one is set.

Short callee `Chn_ForwardIfFlag2` (low):
```cpp
XI_API void __cdecl Chn_ForwardIfFlag2(uint8_t * param_1, undefined4 xi_s2)
{
  if ((*param_1 & 2) != 0) {
    orig::FUN_1027dd90((uint32_t *)param_1,xi_s2);
    return;
  }
  return;
}
```

Short callee `Unit102857D0_GetFlagByte14` (low):
```cpp
XI_API undefined4 __cdecl Unit102857D0_GetFlagByte14(int param_1)
{
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)param_1 >> 8)) << 8) | (uint8_t)(*(uint8_t *)(param_1 + 0x14))));
}
```

Short callee `Unit102857D0_InvokeCallback18` (low):
```cpp
XI_API undefined4 __cdecl Unit102857D0_InvokeCallback18(int param_1)
{
  uint32_t uVar1;
  
  if (*(code **)(param_1 + 0x18) != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x102857ff. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(param_1 + 0x18))();
    return uVar1;
  }
  return 0;
}
```

```cpp
XI_API undefined4 __cdecl fn_10251200(uint8_t * param_1, undefined4 param_2, int param_3)
{
  uint8_t *pbVar1;
  uint32_t uVar2;
  uint32_t *puVar3;
  
  pbVar1 = param_1;
  if ((param_1 != (uint8_t *)0x0) && ((*param_1 & 0x10) != 0)) {
    Mdl_GetEntryTable((int *)&param_1,(int *)0x0);
    puVar3 = (uint32_t *)(param_1 + *(int *)(pbVar1 + 0xc) * 0xc0);
    if ((*puVar3 & 2) == 0) {
      *puVar3 = *puVar3 | 2;
      Chn_ForwardIfFlag2((uint8_t *)puVar3,param_2);
      if ((param_3 == 0) && (puVar3[0x2f] != 0)) {
        uVar2 = Unit102857D0_GetFlagByte14(puVar3[0x2f]);
        if ((char)uVar2 == '\0') {
          orig::FUN_102857e0(puVar3[0x2f],1,uVar2);
          uVar2 = Unit102857D0_InvokeCallback18(puVar3[0x2f]);
          orig::FUN_102857e0(puVar3[0x2f],0,uVar2);
        }
      }
      *puVar3 = *puVar3 & 0xfffffffd;
    }
    return 0;
  }
  return 0xffffffff;
}
```
