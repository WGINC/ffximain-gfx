# Batch r2_101_CXiDancerActor: 2 functions of `CXiDancerActor`

Name these functions: see `batches/README.md`. Write `data/names/r2_101_CXiDancerActor.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x100AB750` fn_100AB750

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiDancerActor_ApplyStateBlock` (low)
- **Called by:** `sub_100AB1B0`, `sub_100AB360`, `sub_100AB590`, `CXiDancerActor_Construct` (medium)
- **Writes globals:** `0x10487FB4`
- **Reads globals:** `0x10487FB4`
- **Named before** (a naming session's reading, low): `CXiDancerActor_InitBind`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiDancerActor_InitBind`
- **Named callees' callees:** `Acter_CopySixDwordsTo7C`, `CXiDancerActor_SetPosition`, `Unit100ABC20_CopyReturnZ`
- **How its callers call it** (their verified code):
  - in `CXiDancerActor_Construct` (medium): `CXiDancerActor_InitBind(self_,param_1,param_2,param_3,param_4);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CXiDancerActor_Construct` (medium): Constructs a dancer actor and loads its skeleton resource.
  - `CXiDancerActor_ApplyStateBlock` (low): Applies a state block to the dancer actor and marks the changed fields.

```cpp
XI_API void __thiscall CXiDancerActor_InitBind(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, uint32_t param_4)
{
  void *pvVar1;
  uint32_t uVar2;
  uint32_t uVar3;
  uint32_t local_4;
  
  uVar2 = local_4 & 0xffffff | 0x4000000;
  if ((int)param_4 < 0) {
    *(void **)((int)self_ + 0x70) = (void *)((int)self_ + 0xa0c);
    DAT_10487fb4 = DAT_10487fb4 + 1;
    uVar3 = DAT_10487fb4 ^ uVar2;
    *(uint32_t *)((int)self_ + 0xa80) = DAT_10487fb4;
    *(uint32_t *)((int)self_ + 0xa84) = uVar2 ^ uVar3 & 0xffffff;
    CXiDancerActor_ApplyStateBlock(self_,(void *)((int)self_ + 0xa0c),param_1,param_2,param_3);
    return;
  }
  pvVar1 = (void *)(&ActerBuffPtr_as_v0)[param_4];
  *(void **)((int)self_ + 0x70) = pvVar1;
  *(uint32_t *)((int)pvVar1 + 0x74) = param_4;
  *(uint32_t *)((int)pvVar1 + 0x78) = uVar2 ^ (uVar2 ^ param_4) & 0xffffff;
  *(void **)((int)pvVar1 + 0xa0) = self_;
  CXiDancerActor_ApplyStateBlock(self_,pvVar1,param_1,param_2,param_3);
  *(uint32_t *)((int)pvVar1 + 0x120) = *(uint32_t *)((int)pvVar1 + 0x120) | 0x200;
  return;
}
```

## `0x100AB9A0` fn_100AB9A0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Acter_CopySixDwordsTo7C` (low), `CXiDancerActor_SetPosition` (medium), `sub_100AB880`, `Unit100ABC20_CopyReturnZ` (low) x2
- **Called by:** `CXiDancerActor_InitBind` (low)
- **Named before** (a naming session's reading, low): `CXiDancerActor_ApplyStateBlock`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `CXiDancerActor_Construct` → `CXiDancerActor_InitBind` → `CXiDancerActor_ApplyStateBlock`
- **Named callers' callers:** `CXiDancerActor_Construct`
- **Named callees' callees:** `Vec4_Copy`, `Acter_CopyPosToCached`
- **How its callers call it** (their verified code):
  - in `CXiDancerActor_InitBind` (low): `CXiDancerActor_ApplyStateBlock(self_,(void *)((int)self_ + 0xa0c),param_1,param_2,param_3);`
  - in `CXiDancerActor_InitBind` (low): `CXiDancerActor_ApplyStateBlock(self_,pvVar1,param_1,param_2,param_3);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Acter_CopySixDwordsTo7C` (low): Copies six dwords into the block at +0x7C when a word at +0x210 is zero.
  - `CXiDancerActor_InitBind` (low): Binds the dancer actor to its slot or its own sub-object.
  - `CXiDancerActor_SetPosition` (medium): Sets the dancer actor's position and its stored vector.
  - `Unit100ABC20_CopyReturnZ` (low): Copies a three-dword value and returns its last element.

Short callee `Unit100ABC20_CopyReturnZ` (low):
```cpp
XI_API undefined4 __cdecl Unit100ABC20_CopyReturnZ(undefined4 * param_1, undefined4 * param_2)
{
  uint32_t uVar1;
  
  *param_1 = *param_2;
  param_1[1] = param_2[1];
  uVar1 = param_2[2];
  param_1[2] = uVar1;
  return uVar1;
}
```

```cpp
XI_API void __thiscall CXiDancerActor_ApplyStateBlock(void * self_, void * param_1, undefined4 param_2, undefined4 param_3, undefined4 * param_4)
{
  short sVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  uint32_t uVar7;
  short local_14;
  short sStack_12;
  short local_10;
  short sStack_e;
  short local_c;
  short sStack_a;
  short local_8;
  short sStack_6;
  
  puVar5 = param_4;
  Acter_CopySixDwordsTo7C(param_1,param_4);
  Unit100ABC20_CopyReturnZ((undefined4 *)((int)param_1 + 4),param_2);
  CXiDancerActor_SetPosition(self_,param_2);
  uVar6 = Unit100ABC20_CopyReturnZ((undefined4 *)((int)param_1 + 0x14),param_3);
  orig::FUN_100ab880(self_,param_3,uVar6);
  *(uint8_t *)((int)param_1 + 0xee) = 0;
  param_4 = (undefined4 *)(uint32_t)*(uint8_t *)(param_4 + 9);
  uVar7 = *(uint32_t *)((int)param_1 + 0x120) & 0x3fffffff ^ (int)param_4 << 0x1e;
  *(uint32_t *)((int)param_1 + 0x120) = uVar7;
  uVar6 = puVar5[4];
  uVar2 = puVar5[5];
  uVar3 = puVar5[6];
  uVar4 = puVar5[7];
  sVar1 = *(short *)(puVar5 + 8);
  if ((uint32_t)*(uint8_t *)((int)param_1 + 0xef) != (int)*(char *)((int)puVar5 + 0x22)) {
    *(char *)((int)param_1 + 0xef) = *(char *)((int)puVar5 + 0x22);
    *(uint32_t *)((int)param_1 + 0x120) = uVar7 | 1;
  }
  local_14 = (short)uVar6;
  if (*(short *)((int)param_1 + 0xfc) != local_14) {
    *(short *)((int)param_1 + 0xfc) = local_14;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 1;
  }
  sStack_12 = (short)((uint32_t)uVar6 >> 0x10);
  if (*(short *)((int)param_1 + 0xfe) != sStack_12) {
    *(short *)((int)param_1 + 0xfe) = sStack_12;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 2;
  }
  local_10 = (short)uVar2;
  if (*(short *)((int)param_1 + 0x100) != local_10) {
    *(short *)((int)param_1 + 0x100) = local_10;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 4;
  }
  sStack_e = (short)((uint32_t)uVar2 >> 0x10);
  if (*(short *)((int)param_1 + 0x102) != sStack_e) {
    *(short *)((int)param_1 + 0x102) = sStack_e;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 8;
  }
  local_c = (short)uVar3;
  if (*(short *)((int)param_1 + 0x104) != local_c) {
    *(short *)((int)param_1 + 0x104) = local_c;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 0x10;
  }
  sStack_a = (short)((uint32_t)uVar3 >> 0x10);
  if (*(short *)((int)param_1 + 0x106) != sStack_a) {
    *(short *)((int)param_1 + 0x106) = sStack_a;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 0x20;
  }
  local_8 = (short)uVar4;
  if (*(short *)((int)param_1 + 0x108) != local_8) {
    *(short *)((int)param_1 + 0x108) = local_8;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 0x40;
  }
  sStack_6 = (short)((uint32_t)uVar4 >> 0x10);
  if (*(short *)((int)param_1 + 0x10a) != sStack_6) {
    *(short *)((int)param_1 + 0x10a) = sStack_6;
    *(uint8_t *)((int)param_1 + 0xf4) = *(uint8_t *)((int)param_1 + 0xf4) | 0x80;
  }
  if (*(short *)((int)param_1 + 0x10c) != sVar1) {
    *(short *)((int)param_1 + 0x10c) = sVar1;
    *(uint8_t *)((int)param_1 + 0xf5) = *(uint8_t *)((int)param_1 + 0xf5) | 1;
  }
  *(uint8_t *)((int)param_1 + 0x250) = *(uint8_t *)((int)puVar5 + 0x22);
  *(short *)((int)param_1 + 0x252) = local_14;
  *(short *)((int)param_1 + 0x25c) = sStack_a;
  *(short *)((int)param_1 + 0x254) = sStack_12;
  *(short *)((int)param_1 + 0x256) = local_10;
  *(short *)((int)param_1 + 600) = sStack_e;
  *(short *)((int)param_1 + 0x25a) = local_c;
  *(short *)((int)param_1 + 0x25e) = local_8;
  *(short *)((int)param_1 + 0x260) = sStack_6;
  *(short *)((int)param_1 + 0x262) = sVar1;
  *(uint32_t *)((int)param_1 + 0x120) = *(uint32_t *)((int)param_1 + 0x120) | 0x100000;
  return;
}
```
