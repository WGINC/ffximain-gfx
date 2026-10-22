# Batch r2_066_unit_100ACAB0_XiEvent: 3 functions of `unit_100ACAB0_XiEvent`

Name these functions: see `batches/README.md`. Write `data/names/r2_066_unit_100ACAB0_XiEvent.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x100C3020` fn_100C3020

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AX of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit100ACAB0_AddEvent` (low)
- **From the frame root** (shortest chain of direct calls): `sub_10080DC0` → `preprocessArg` → `fn_10158DB0` → `sub_1007BF30` → `sub_100A1AB0` → `XiEvent_Spawn` → `Unit100ACAB0_AddEvent` → `fn_100C3020`
- **Named callers' callers:** `XiEvent_Spawn`
- **How its callers call it** (their verified code):
  - in `Unit100ACAB0_AddEvent` (low): `uVar1 = orig::FUN_100c3020(param_2,xi_entry_eax);`
  - in `Unit100ACAB0_AddEvent` (low): `xi_eax = (void *)orig::FUN_100c3020(param_2,uVar2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit100ACAB0_AddEvent` (low): Creates an event record from actor ids and appends it to the list.

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100C3020(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  return ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)xi_eax >> 0x10)) << 16) | (uint16_t)(*(uint16_t *)((int)self_ + 0xbfe))));
}
```

## `0x100C3030` fn_100C3030

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AX of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `Unit100ACAB0_AddEvent` (low)
- **From the frame root** (shortest chain of direct calls): `sub_10080DC0` → `preprocessArg` → `fn_10158DB0` → `sub_1007BF30` → `sub_100A1AB0` → `XiEvent_Spawn` → `Unit100ACAB0_AddEvent` → `fn_100C3030`
- **Named callers' callers:** `XiEvent_Spawn`
- **How its callers call it** (their verified code):
  - in `Unit100ACAB0_AddEvent` (low): `uVar1 = orig::FUN_100c3030(pvVar3,xi_eax);`
  - in `Unit100ACAB0_AddEvent` (low): `param_2 = (void *)orig::FUN_100c3030(pvVar3,uVar2);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Unit100ACAB0_AddEvent` (low): Creates an event record from actor ids and appends it to the list.

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_100C3030(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  return ((uint32_t)(((uint32_t)(uint16_t)((short)((uint32_t)xi_eax >> 0x10)) << 16) | (uint16_t)(*(uint16_t *)((int)self_ + 0xc00))));
}
```

## `0x100C3480` fn_100C3480

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetFrameScale` x2, `sub_10087DF0`, `sub_100C2D00` x2, `sub_100C2FB0`, `sub_100C2FE0` x2, `sub_100C30F0`, `sub_100C3100` x2, `sub_100C3120`, `sub_100C3130` x15, `sub_100C3140` x2, `sub_100C3150`, `sub_100C3160`, `sub_100C3170`, `sub_10127C00`, `sub_10311C2C` x2
- **Called by:** `sub_100C33D0`
- **Writes globals:** `0x104807FC`
- **Reads globals:** `0x10456A28`, `0x104807FC`, `0x10578158`
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pSceneState`, `g_pTkMsgWinData`
- **Named callees' callees:** `Unit10037AC0_Create`, `Unit100D7420_BuildRecord`, `Unit100D7420_FindByKeyByte`, `GetCallNum`, `Unit100D7420_IsNearNamedActor`, `Text_EncodeQuotes`, `Unit101D0910_TestMask10663188`

Short callee `Camera_GetFrameScale`:
```cpp
XI_API long double __cdecl Camera_GetFrameScale(void)
{
  uint8_t bVar1;
  
  bVar1 = NAN((long double)*(float *)(g_pCamera_as_s32 + 0x28)) || NAN((long double)1.0f);
  if ((bVar1 || (long double)*(float *)(g_pCamera_as_s32 + 0x28) < (long double)1.0f) != bVar1) {
    return (long double)1.0f;
  }
  return (long double)*(float *)(g_pCamera_as_s32 + 0x28);
}
```

```cpp
XI_API undefined4 __thiscall fn_100C3480(void * self_)
{
  short sVar1;
  void *pvVar2;
  int *a2;
  int iVar3;
  uint32_t uVar4;
  char *pcVar5;
  char *pcVar6;
  uint32_t uVar7;
  int a5;
  uint32_t uVar8;
  uint8_t *pbVar9;
  int iVar10;
  uint8_t bVar11;
  long double fVar12;
  int64_t lVar13;
  char xi_frame[1588] __attribute__((aligned(4)));
  uint32_t  &uVar14 = *reinterpret_cast<uint32_t  *>(xi_frame + 0);
  char  (&local_61c)[1564] = *reinterpret_cast<char  (*)[1564]>(xi_frame + 24);
  
  DAT_104807fc = DAT_104807fc + 1;
  iVar3 = *(int *)((int)self_ + 0x54);
  if (iVar3 == 0) {
    iVar3 = orig::FUN_100c3130(self_);
    if (((((iVar3 != 1) && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 9)) &&
         (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0xb)) &&
        ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32 && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2cf))
        )) && ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32b &&
               (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32d)))) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
      return 0;
    }
    uVar4 = orig::FUN_100c3140(self_);
    if ((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] == 0) {
      *(uint16_t *)((int)self_ + 0x58) = 0x3c;
      *(uint32_t *)((int)self_ + 0x54) = 2;
      return 0;
    }
    *(uint16_t *)((int)self_ + 0x58) = 600;
    *(uint32_t *)((int)self_ + 0x54) = 1;
    return 0;
  }
  if (iVar3 == 1) {
    sVar1 = *(short *)((int)self_ + 0x58);
    fVar12 = orig::Camera_GetFrameScale_v1((int)sVar1);
    lVar13 = XI_CALL_ST0(0x10311C2Cu)((long double)(float)(long double)(int)sVar1 - fVar12);
    *(short *)((int)self_ + 0x58) = (short)lVar13;
    if ((short)lVar13 < 1) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
    }
    uVar4 = orig::FUN_100c3140(self_);
    if ((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] == 0) {
      *(uint16_t *)((int)self_ + 0x58) = 0;
      *(uint32_t *)((int)self_ + 0x54) = 2;
    }
    else {
      iVar3 = *(int *)((&ActerBuffPtr_as_v0)[uVar4 & 0xffff] + 0x170);
      if ((iVar3 == 2) || (iVar3 == 3)) {
        *(uint16_t *)((int)self_ + 0x58) = 0x3c;
        *(uint32_t *)((int)self_ + 0x54) = 2;
        return 0;
      }
    }
    return 0;
  }
  sVar1 = *(short *)((int)self_ + 0x58);
  fVar12 = orig::Camera_GetFrameScale_v1(iVar3);
  lVar13 = XI_CALL_ST0(0x10311C2Cu)((long double)(float)(long double)(int)sVar1 - fVar12);
  *(short *)((int)self_ + 0x58) = (short)lVar13;
  if (0 < (short)lVar13) {
    return 0;
  }
  pvVar2 = *(void **)((int)self_ + 0x40);
  iVar3 = orig::FUN_100c2fb0(pvVar2);
  if (iVar3 != 2) {
    return 0;
  }
  pcVar5 = orig::FUN_100c30f0(self_);
  pcVar5 = orig::FUN_100c2d00(local_61c,pcVar5);
  pcVar6 = (char *)orig::FUN_100c2fe0(pvVar2);
  orig::FUN_100c2d00(pcVar5,pcVar6);
  uVar4 = 0xffffffff;
  iVar3 = orig::FUN_100c3130(self_);
  if ((((iVar3 != 9) && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0xb)) &&
      (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32)) &&
     (((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2cf && (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x2d))
      && ((iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32b &&
          (iVar3 = orig::FUN_100c3130(self_), iVar3 != 0x32d)))))) goto LAB_100c37de;
  uVar7 = orig::FUN_100c3150(self_);
  iVar3 = (&ActerBuffPtr_as_v0)[uVar7 & 0xffff];
  if (iVar3 == 0) {
    bVar11 = false;
  }
  else {
    bVar11 = *(short *)(iVar3 + 0x210) != 0;
  }
  iVar10 = *(int *)(g_pSceneState + 8);
  if ((*(char *)(g_pSceneState + 0x18) < '\0') && ((iVar10 == 2 || (iVar10 == 3)))) {
    iVar10 = 1;
  }
  uVar7 = orig::FUN_100c3130(self_);
  if (uVar7 < 0x33) {
    if (uVar7 == 0x32) {
      uVar4 = 2;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 8) * 4);
    }
    else if (uVar7 == 9) {
      uVar4 = -(uint32_t)bVar11 & 4;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + uVar4 * 4) * 4);
    }
    else if (uVar7 == 0xb) {
      uVar4 = 1;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 4) * 4);
    }
    else {
      if ((uVar7 != 0x2d) || (!bVar11)) goto LAB_100c3793;
      uVar4 = 5;
      iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x14) * 4);
    }
  }
  else if (uVar7 == 0x2cf) {
    uVar4 = 3;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0xc) * 4);
  }
  else if (uVar7 == 0x32b) {
    uVar4 = 6;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x18) * 4);
  }
  else if (uVar7 == 0x32d) {
    uVar4 = 7;
    iVar10 = *(int *)(((char *)&DAT_1035d570) + (iVar10 + 0x1c) * 4);
  }
  else {
LAB_100c3793:
    iVar10 = -1;
  }
  if ((iVar3 != 0) && ((*(uint32_t *)(iVar3 + 0x120) >> 9 & 1) != 0)) {
    if (uVar4 != 0xffffffff) {
      a2 = *(int **)(iVar3 + 0xa0);
      iVar3 = *a2;
      a5 = orig::FUN_100c3100(self_);
      ((int (__stdcall *)(XI_ANY, XI_ANY, XI_ANY, XI_ANY, XI_ANY))((**(code **)(iVar3 + 0x2c4))))(iVar10,a2,0x6e69616d,local_61c,a5);
    }
    if (iVar10 != -1) {
      *(uint32_t *)((int)self_ + 0x54) = 0;
      return 1;
    }
  }
LAB_100c37de:
  uVar8 = orig::FUN_100c3120(self_);
  if ((char)uVar8 == '\0') {
    uVar4 = orig::FUN_100c3160(self_);
    if (((short)uVar4 != 0) &&
       (pvVar2 = (void *)(&ActerBuffPtr_as_v0)[uVar4 & 0xffff], pvVar2 != (void *)0x0)) {
      pcVar5 = (char *)orig::FUN_100c2fe0(*(void **)((int)self_ + 0x40));
      orig::FUN_10087df0(pvVar2,pcVar5);
    }
    iVar3 = 0;
    uVar14 = 1;
    uVar8 = 0;
    pbVar9 = (uint8_t *)orig::FUN_100c3100(self_);
    orig::ChatLog_AddMessage_v1(g_pTkMsgWinData_as_v1,local_61c,pbVar9,uVar8,uVar14,iVar3);
    *(uint32_t *)((int)self_ + 0x54) = 0;
    return 1;
  }
  orig::FUN_100c3170(local_61c);
  *(uint32_t *)((int)self_ + 0x54) = 0;
  return 1;
}
```
