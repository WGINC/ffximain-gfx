# Batch r2_193_unit_10244EA0_LobbyThread: 10 functions of `unit_10244EA0_LobbyThread`

Name these functions: see `batches/README.md`. Write `data/names/r2_193_unit_10244EA0_LobbyThread.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10245040` fn_10245040

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `LobbyThread_Construct` (medium)
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `GameSystems_InitializeAll` → `LobbyThread_Construct` → `fn_10245040`
- **Named callers' callers:** `GameSystems_InitializeAll`
- **How its callers call it** (their verified code):
  - in `LobbyThread_Construct` (medium): `orig::FUN_10245040(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_Construct` (medium): Constructs the lobby-thread object and sets its vtable and defaults.

```cpp
XI_API void __thiscall fn_10245040(void * self_)
{
  *(uint32_t *)((int)self_ + 0x14) = 2;
  return;
}
```

## `0x10245BB0` fn_10245BB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10014CB0`, `CYyCamMng2::CreateGlobal`, `XiZone_GetInstance`, `XiZone_ResetState` (medium)
- **Called by:** `thunk_sub_10245BB0`
- **Reads globals:** `0x104568FC`, `0x1065CB04`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pCamera`, `g_pXiZone`
- **Strings its callees refer to:** `CAtelIdleTask`
- **Named callees' callees:** `CYyCamMng2_SetDefaultDistances`, `CYyCamMng2_Construct`, `Mem_AllocDefault`, `Unit10081A30_ReleaseQueued`, `ZoneMap_Reset`, `ZoneMap_ClearSlots`, `ZoneMap_ReleaseChain`, `XiZone_DestroyChildList`, `XiZone_Constructor`, `Time_Normalize`
- **How its callers call it** (their verified code):
  - in `thunk_sub_10245BB0`: `orig::FUN_10245bb0();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `XiZone_ResetState` (medium): Resets the zone's state and the camera and zone-map values it uses.

```cpp
XI_API void __cdecl fn_10245BB0(void)
{
  if (g_pXiZone != (void *)0x0) {
    XiZone_ResetState(g_pXiZone);
    orig::FUN_10014cb0();
    XiZone_GetInstance();
    orig::CYyCamMng2_CreateGlobal();
    orig::Actor_ResetDrawLists();
    return;
  }
  return;
}
```

## `0x10245BE0` fn_10245BE0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10039590`
- **Called by:** `Unit10237880_WaitForState1` (low)
- **Reads globals:** `0x10669098`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10014AD0` → `Unit10237880_ForwardFlag1` → `sub_1023BE10` → `Unit10237880_WaitForState1` → `fn_10245BE0`
- **Named callers' callers:** `LobbyThread_Proc`
- **How its callers call it** (their verified code):
  - in `Unit10237880_WaitForState1` (low): `uVar2 = orig::FUN_10245be0();`
  - in `Unit10237880_WaitForState1` (low): `uVar2 = orig::FUN_10245be0();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit10237880_WaitForState1` (low): Waits until a game system reaches state 1.

```cpp
XI_API undefined4 __cdecl fn_10245BE0(void)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10039590(DAT_10669098,1);
  return uVar1;
}
```

## `0x10246030` fn_10246030

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `LobbyThread_NextSequence` (medium), `fn_1024C060` x2
- **Called by:** `sub_10038330`
- **Reads globals:** `0x106690B0`
- **Strings it refers to** (DLL bytes): `SeSep`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `sub_100016C0` → `sub_101B29F0` → `SetMusicTrackVolume_C` → `fn_10036F00` → `sub_10036F50` → `sub_10038330` → `fn_10246030`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_NextSequence` (medium): Advances the thread's sequence number, skipping zero.

Short callee `LobbyThread_NextSequence` (medium):
```cpp
XI_API undefined4 __thiscall LobbyThread_NextSequence(void * self_)
{
  uint32_t uVar1;
  
  uVar1 = *(int *)((int)self_ + 8) + 1U & 0x7fffffff;
  *(uint32_t *)((int)self_ + 8) = uVar1;
  if (uVar1 == 0) {
    *(uint32_t *)((int)self_ + 8) = 1;
  }
  return *(uint32_t *)((int)self_ + 8);
}
```

```cpp
XI_API uint64_t __thiscall fn_10246030(void * self_, char * param_1, uint32_t param_2, uint32_t param_3)
{
  int iVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  uint8_t bVar5;
  uint64_t uVar6;
  
  iVar1 = LobbyThread_NextSequence(self_);
  bVar5 = true;
  iVar2 = 5;
  pcVar3 = param_1;
  pcVar4 = s_SeSep_1035d5f8;
  do {
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    bVar5 = *pcVar3 == *pcVar4;
    pcVar3 = pcVar3 + 1;
    pcVar4 = pcVar4 + 1;
  } while (bVar5);
  if (bVar5) {
    uVar6 = orig::FUN_1024c060(2,*(int **)(param_1 + 8),iVar1,param_2 & 0x7f,param_3 & 0x7f,0,0,0);
    return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar6 >> 0x20)) << 32) | (uint32_t)(iVar1)));
  }
  uVar6 = orig::FUN_1024c060(1,(int *)param_1,iVar1,param_2 & 0x7f,param_3 & 0x7f,0,0,0);
  return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar6 >> 0x20)) << 32) | (uint32_t)(iVar1)));
}
```

## `0x10246160` fn_10246160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10036230`, `sub_10038330`, `sub_10245EB0`
- **Reads globals:** `0x106690C8`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `fn_10036230` → `fn_10246160`
- **Named callers' callers:** `Frame_Render`
- **How its callers call it** (their verified code):
  - in `fn_10036230`: `(uVar10 = orig::FUN_10246160(0,*(int *)((int)pvVar7 + 0x1e0),-1), iVar4 = DAT_104638fc,`

```cpp
XI_API uint64_t __stdcall fn_10246160(undefined4 param_1, int param_2, int param_3)
{
  int iVar1;
  uint32_t uVar2;
  uint32_t uVar3;
  
  iVar1 = -1;
  uVar2 = DAT_106690c8;
  switch(param_1) {
  case 0:
    if (param_2 == 0) {
      while ((*(int *)(uVar2 + 0xc0) < 1 || ((-1 < param_3 && (*(int *)(uVar2 + 0xc4) != param_3))))
            ) {
        uVar2 = uVar2 + 0xf4;
        if (DAT_106690c8 + 0x1310 <= uVar2) {
          return (uint64_t)DAT_106690c8 << 0x20;
        }
      }
    }
    else {
      while (((-1 < param_2 || (*(int *)(uVar2 + 0xc0) < 1)) && (*(int *)(uVar2 + 0xc0) != param_2))
            ) {
        uVar2 = uVar2 + 0xf4;
        if (DAT_106690c8 + 0x1310 <= uVar2) {
          return (uint64_t)DAT_106690c8 << 0x20;
        }
      }
    }
    return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(1)));
  case 1:
    if (0 < param_2) {
      do {
        if (*(int *)(uVar2 + 0xc0) == param_2) {
          return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(*(int *)(uVar2 + 0x70) >> 0xc)));
        }
        uVar2 = uVar2 + 0xf4;
      } while (uVar2 < DAT_106690c8 + 0x1310);
      return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(0xffffffff)));
    }
    break;
  case 2:
    if (0 < param_2) {
      do {
        if (*(int *)(uVar2 + 0xc0) == param_2) {
          return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(*(int *)(uVar2 + 0x80) >> 0xc)));
        }
        uVar2 = uVar2 + 0xf4;
      } while (uVar2 < DAT_106690c8 + 0x1310);
      return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(0xffffffff)));
    }
    break;
  case 3:
    if (0 < param_2) {
      do {
        if (*(int *)(uVar2 + 0xc0) == param_2) {
          return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(*(uint32_t *)(uVar2 + 0x6c))));
        }
        uVar2 = uVar2 + 0xf4;
      } while (uVar2 < DAT_106690c8 + 0x1310);
      return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(0xffffffff)));
    }
    break;
  case 4:
    if (0 < param_2) {
      do {
        if (*(int *)(uVar2 + 0xc0) == param_2) {
          return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(*(uint32_t *)(uVar2 + 0x7c))));
        }
        uVar2 = uVar2 + 0xf4;
      } while (uVar2 < DAT_106690c8 + 0x1310);
      return ((uint64_t)(((uint64_t)(uint32_t)(DAT_106690c8) << 32) | (uint32_t)(0xffffffff)));
    }
    break;
  case 5:
    iVar1 = 0;
    uVar3 = DAT_106690c8;
    do {
      if (*(int *)(uVar3 + 0x3c) != 0) {
        iVar1 = iVar1 + 1;
      }
      uVar3 = uVar3 + 0xf4;
      uVar2 = DAT_106690c8 + 0x1310;
    } while (uVar3 < DAT_106690c8 + 0x1310);
  }
  return ((uint64_t)(((uint64_t)(uint32_t)(uVar2) << 32) | (uint32_t)(iVar1)));
}
```

## `0x102463F0` fn_102463F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1024C060`, `operator_delete`, `Allocator_Generic`, `sub_10311F30`, `sub_10311FDE`, `sub_1031217F`
- **Called by:** `sub_10018850`
- **Reads globals:** `0x106690B0`
- **Strings it refers to** (DLL bytes): `BGMStream`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10018850` → `fn_102463F0`
- **Named callers' callers:** `Frame_Render`

```cpp
XI_API undefined4 __stdcall fn_102463F0(undefined4 param_1, uint32_t param_2, undefined4 param_3)
{
  char *pcVar1;
  int iVar2;
  uint32_t uVar3;
  char *pcVar4;
  char *pcVar5;
  uint8_t bVar6;
  uint64_t uVar7;
  
  uVar3 = 0xffffffff;
  uVar7 = orig::Allocator_Generic_v1(0x30);
  pcVar1 = (char *)uVar7;
  if (pcVar1 != (char *)0x0) {
    pcVar4 = pcVar1;
    for (iVar2 = 0xc; iVar2 != 0; iVar2 = iVar2 + -1) {
      pcVar4[0] = '\0';
      pcVar4[1] = '\0';
      pcVar4[2] = '\0';
      pcVar4[3] = '\0';
      pcVar4 = pcVar4 + 4;
    }
    iVar2 = orig::FUN_10311fde(param_1,&DAT_1034f2d0);
    if (iVar2 != 0) {
      orig::FUN_1031217f(pcVar1,1,0x30,iVar2);
      orig::FUN_10311f30(iVar2);
    }
    iVar2 = 10;
    bVar6 = true;
    pcVar4 = pcVar1;
    pcVar5 = s_BGMStream_103888d0;
    do {
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      bVar6 = *pcVar4 == *pcVar5;
      pcVar4 = pcVar4 + 1;
      pcVar5 = pcVar5 + 1;
    } while (bVar6);
    if (bVar6) {
      uVar3 = *(uint32_t *)(pcVar1 + 0x14);
    }
    else {
      uVar3 = 0x400;
    }
    orig::FUN_1024c060(7,param_1,param_2 & 0x7f,param_3,0,0,0,0);
    orig::operator_delete_10311BB0_v1(pcVar1);
  }
  return uVar3;
}
```

## `0x102465E0` fn_102465E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1024C060`, `operator_delete` x2, `Allocator_Generic` x2, `sub_10311F30`, `sub_10311FDE`, `sub_10311FF1`
- **Called by:** `fn_100382A0`
- **Reads globals:** `0x106690B0`, `0x10669108`
- **Strings it refers to** (DLL bytes): `%s\music%3.3u.bgw`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `fn_1024DB60` → `sub_1024D850` → `sub_100378B0` → `fn_100382A0` → `fn_102465E0`
- **How its callers call it** (their verified code):
  - in `fn_100382A0`: `orig::FUN_102465e0(param_1,param_2,0);`

```cpp
XI_API uint32_t __stdcall fn_102465E0(uint32_t param_1, uint32_t param_2, undefined4 param_3)
{
  char cVar1;
  uint8_t *puVar2;
  int *piVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  uint64_t uVar7;
  char xi_frame[64] __attribute__((aligned(4)));
  uint32_t  (&auStack_40)[16] = *reinterpret_cast<uint32_t  (*)[16]>(xi_frame + 0);
  
  pcVar5 = (char *)(DAT_10669108 + 4);
  if (*pcVar5 != '\0') {
    param_1 = param_1 & 0x3ff;
    uVar7 = orig::Allocator_Generic_v1(0x104);
    piVar3 = (int *)uVar7;
    if (piVar3 != (int *)0x0) {
      iVar6 = 0;
      do {
        uVar7 = orig::Allocator_Generic_v1(0x104);
        auStack_40[iVar6] = (int)uVar7;
        iVar6 = iVar6 + 1;
      } while (iVar6 < 0x10);
      iVar6 = 0;
      while( true ) {
        puVar2 = (uint8_t *)auStack_40[iVar6];
        if (puVar2 != (uint8_t *)0x0) {
          orig::_sprintf_v1(puVar2,s__s_music_3_3u_bgw_103888dc,pcVar5,param_1);
        }
        iVar4 = orig::FUN_10311fde(puVar2,&DAT_1034f2d0);
        if (iVar4 != 0) break;
        pcVar5 = pcVar5 + 0x108;
        iVar6 = iVar6 + 1;
        if (0xf < iVar6) goto LAB_10246691;
      }
      pcVar5 = (char *)auStack_40[iVar6];
      iVar6 = (int)piVar3 - (int)pcVar5;
      do {
        cVar1 = *pcVar5;
        pcVar5[iVar6] = cVar1;
        pcVar5 = pcVar5 + 1;
      } while (cVar1 != '\0');
LAB_10246691:
      if (iVar4 != 0) {
        orig::FUN_10311f30(iVar4);
        orig::FUN_1024c060(7,piVar3,param_2 & 0x7f,param_3,0,0,0,0);
      }
      orig::operator_delete_10311BB0_v1(piVar3);
      iVar6 = 0;
      do {
        if ((LPVOID)auStack_40[iVar6] != (LPVOID)0x0) {
          orig::operator_delete_10311BB0_v1((LPVOID)auStack_40[iVar6]);
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 0x10);
    }
  }
  return param_1;
}
```

## `0x1024C060` fn_1024C060

- **Gate:** none; **frame set:** static
- **Facts:** `ret 32`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10036BE0`, `LobbyThread_SendCmd0` (low), `fn_10246030`, `sub_102460A0`, `sub_102460E0`, `LobbyThread_SendCmd3` (low), `LobbyThread_SendCmd4` (low), `LobbyThread_SendCmd6` (low), `fn_102463F0`, `sub_102464A0`, `fn_102465E0`, `LobbyThread_SendCmd8` (low), `LobbyThread_SendCmd9` (low), `sub_102467D0`, `LobbyThread_SendCmd11` (low), `sub_10246820`, `sub_102468B0`
- **Writes globals:** `0x106690C4`
- **Reads globals:** `0x106690C4`, `0x106690E8`, `0x106690F0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `fn_10014C10` → `Acter_ShutdownGlobals` → `LobbyThread_SendCmd11` → `fn_1024C060`
- **Named callers' callers:** `Acter_ShutdownGlobals`, `LobbyThread_Proc`
- **How its callers call it** (their verified code):
  - in `LobbyThread_SendCmd0` (low): `uVar1 = orig::FUN_1024c060(0,param_1,param_2,0,0,0,0,0);`
  - in `fn_10246030`: `uVar6 = orig::FUN_1024c060(2,*(int **)(param_1 + 8),iVar1,param_2 & 0x7f,param_3 & 0x7f,0,0,0);`
  - in `fn_10246030`: `uVar6 = orig::FUN_1024c060(1,(int *)param_1,iVar1,param_2 & 0x7f,param_3 & 0x7f,0,0,0);`
  - in `LobbyThread_SendCmd3` (low): `orig::FUN_1024c060(3,param_1,0,param_2 & 0x7f,param_3,0,0,0);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_SendCmd0` (low): Sends command 0 to the lobby thread and returns its result.
  - `LobbyThread_SendCmd3` (low): Sends command 3 to the lobby thread.
  - `LobbyThread_SendCmd4` (low): Sends command 4 to the lobby thread and returns its result.
  - `LobbyThread_SendCmd6` (low): Sends command 6 to the lobby thread.
  - `LobbyThread_SendCmd8` (low): Sends command 8 to the lobby thread.
  - `LobbyThread_SendCmd9` (low): Sends command 9 to the lobby thread.
  - `LobbyThread_SendCmd11` (low): Sends command 0xB to the lobby thread and returns its result.

```cpp
XI_API uint64_t __stdcall fn_1024C060(int param_1, int * param_2, int param_3, int param_4, int param_5, int param_6, int param_7, int param_8)
{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  
  piVar2 = DAT_106690f0;
  if (DAT_106690f0 <= DAT_106690c4) {
    piVar2 = DAT_106690f0 + 0x800;
  }
  if (param_1 == 7) {
    param_1 = (int)(DAT_106690c4 + 0x48);
    if (piVar2 <= (uint32_t)param_1) goto LAB_1024c154;
    *DAT_106690c4 = 7;
    DAT_106690c4[1] = param_3;
    DAT_106690c4[2] = param_4;
    piVar2 = DAT_106690c4 + 8;
    iVar4 = 8;
    do {
      piVar1 = piVar2;
      if (DAT_106690e8 + 0x800 <= piVar2) {
        piVar1 = DAT_106690e8;
      }
      piVar2 = piVar1 + 8;
      iVar4 = iVar4 + -1;
      piVar5 = param_2;
      for (iVar3 = 8; iVar3 != 0; iVar3 = iVar3 + -1) {
        *piVar1 = *piVar5;
        piVar5 = piVar5 + 1;
        piVar1 = piVar1 + 1;
      }
      param_2 = param_2 + 8;
    } while (iVar4 != 0);
    piVar1 = DAT_106690e8 + 0x800;
    if (piVar1 <= piVar2) {
      piVar2 = DAT_106690e8;
    }
  }
  else {
    if (piVar2 <= DAT_106690c4 + 8) {
LAB_1024c154:
      return ((uint64_t)(((uint64_t)(uint32_t)(param_1) << 32) | (uint32_t)((int)DAT_106690c4 - (int)DAT_106690e8 >> 5)));
    }
    *DAT_106690c4 = param_1;
    DAT_106690c4[1] = (int)param_2;
    DAT_106690c4[2] = param_3;
    DAT_106690c4[3] = param_4;
    DAT_106690c4[4] = param_5;
    DAT_106690c4[5] = param_6;
    DAT_106690c4[6] = param_7;
    DAT_106690c4[7] = param_8;
    piVar1 = DAT_106690e8 + 0x800;
    piVar2 = DAT_106690c4 + 8;
    if (piVar1 <= DAT_106690c4 + 8) {
      DAT_106690c4 = DAT_106690e8;
      return ((uint64_t)(uint32_t)(piVar1)) << 0x20;
    }
  }
  DAT_106690c4 = piVar2;
  return ((uint64_t)(((uint64_t)(uint32_t)(piVar1) << 32) | (uint32_t)((int)piVar2 - (int)DAT_106690e8 >> 5)));
}
```

## `0x1024C210` fn_1024C210

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10126320`, `LobbyThread_ConstructQueue` (medium), `Allocator_Generic`
- **Called by:** `sub_10165070`
- **Writes globals:** `0x10669144`
- **Reads globals:** `0x10669144`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_InitScene` → `sub_10165070` → `fn_1024C210`
- **Named callers' callers:** `App_InitScene`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `LobbyThread_ConstructQueue` (medium): Constructs the lobby message queue and sets its vtable and defaults.

Short callee `fn_10126320`:
```cpp
XI_API undefined4 __thiscall fn_10126320(void * self_)
{
  *(uint16_t *)((int)self_ + 2) = 0;
  *(uint16_t *)self_ = 0;
  return self_;
}
```

Short callee `LobbyThread_ConstructQueue` (medium):
```cpp
XI_API void * __thiscall LobbyThread_ConstructQueue(void * self_)
{
  orig::FUN_10126320((void *)((int)self_ + 4));
  *(uint16_t *)((int)self_ + 0xe) = 0xffff;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint8_t *)((int)self_ + 0xc) = 1;
  *(uint8_t ***)self_ = &PTR_FUN_1033ccf0;
  return self_;
}
```

```cpp
XI_API void * __thiscall fn_1024C210(void * self_)
{
  int iVar1;
  uint32_t *puVar2;
  uint64_t uVar3;
  
  orig::FUN_10126320((void *)((int)self_ + 4));
  *(uint16_t *)((int)self_ + 0xe) = 0xffff;
  *(uint32_t *)((int)self_ + 8) = 0;
  *(uint8_t *)((int)self_ + 0xc) = 1;
  *(uint8_t ***)self_ = &PTR_FUN_1033cca8;
  *(uint32_t *)((int)self_ + 0x14) = 0;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  *(uint32_t *)((int)self_ + 0x1c) = 0;
  *(uint32_t *)((int)self_ + 0x20) = 0;
  *(uint32_t *)((int)self_ + 0x24) = 0;
  *(uint32_t *)((int)self_ + 0x28) = 0;
  *(uint32_t *)((int)self_ + 0x2c) = 0;
  *(uint32_t *)((int)self_ + 0x30) = 0;
  *(uint32_t *)((int)self_ + 0x74) = 0;
  puVar2 = (uint32_t *)((int)self_ + 0x34);
  for (iVar1 = 0x10; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  if (DAT_10669144 == (void *)0x0) {
    uVar3 = orig::Allocator_Generic_v1(0x14);
    if ((void *)uVar3 != (void *)0x0) {
      DAT_10669144 = LobbyThread_ConstructQueue((void *)uVar3);
      return self_;
    }
    DAT_10669144 = (void *)0x0;
  }
  return self_;
}
```

## `0x1024D130` fn_1024D130

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `StAvatar_SetEntryFlag` (low), `StAvatar_SetEntryFlag2` (low), `StAvatar_SetEntryTransform` (low), `StAvatar_ToggleFlag168` (low)
- **Called by:** `StAvatarInit` (medium)
- **Reads globals:** `0x10389304`, `0x10389308`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatarInit` → `fn_1024D130`
- **Named callers' callers:** `Dancer_WorldBegin`
- **Named callees' callees:** `Unit1027A4B0_Copy`, `Unit1027B060_Mul`, `SqsnScene_SetByteAt28`
- **How its callers call it** (their verified code):
  - in `StAvatarInit` (medium): `orig::sub_1024D130();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `StAvatarInit` (medium): 
  - `StAvatar_SetEntryFlag` (low): Sets or clears the scene flag for one avatar part.
  - `StAvatar_SetEntryFlag2` (low): Sets or clears the second flag of an avatar part.
  - `StAvatar_SetEntryTransform` (low): Sets an avatar part's values and transform from its source records.
  - `StAvatar_ToggleFlag168` (low): Sets or toggles the avatar's +0x168 flag.

Short callee `StAvatar_ToggleFlag168` (low):
```cpp
XI_API void __cdecl StAvatar_ToggleFlag168(int param_1)
{
  if (param_1 == -1) {
    DAT_1099b168_as_v0 = DAT_1099b168_as_v0 ^ 1;
    return;
  }
  DAT_1099b168_as_v0 = param_1 != 0;
  return;
}
```

```cpp
XI_API void __cdecl fn_1024D130(void)
{
  int iVar1;
  
  iVar1 = 0;
  do {
    StAvatar_SetEntryTransform(iVar1,(int)(&PTR_DAT_1038930c)[iVar1],(int)(&PTR_DAT_10389320)[iVar1]);
    StAvatar_SetEntryFlag2(iVar1,*(int *)(PTR_DAT_10389304 + iVar1 * 4));
    StAvatar_SetEntryFlag(iVar1,*(int *)(PTR_DAT_10389308 + iVar1 * 4));
    iVar1 = iVar1 + 1;
  } while (iVar1 < 5);
  StAvatar_ToggleFlag168(1);
  return;
}
```
