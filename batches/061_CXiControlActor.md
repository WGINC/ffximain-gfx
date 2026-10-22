# Batch 061_CXiControlActor: 4 functions of `CXiControlActor`

Name these functions: see `batches/README.md`. Write `data/names/061_CXiControlActor.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x100A5770` fn_100A5770

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Vec3_Copy`
- **Called by:** `fn_100A89E0`

```cpp
XI_API void __thiscall fn_100A5770(void * self_, undefined4 * param_1)
{
  uint32_t uVar1;
  int iVar2;
  
  iVar2 = 4;
  *(uint32_t *)((int)self_ + 8) = 0;
  do {
    uVar1 = *(uint32_t *)((int)self_ + 8) & 0x80000003;
    if ((int)uVar1 < 0) {
      uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
    }
    orig::Vec3_Copy_v1((uint32_t *)((int)self_ + (uVar1 + 1) * 0xc),param_1);
    iVar2 = iVar2 + -1;
    *(int *)((int)self_ + 8) = *(int *)((int)self_ + 8) + 1;
  } while (iVar2 != 0);
  return;
}
```

## `0x100A57C0` fn_100A57C0

- **Gate:** none; **frame set:** static, self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10014390`, `CMoSkeletonElem_UpdateBoneTransform`, `sub_10047960`, `CMoSchedularTask_Interpret`, `CMoPathDriveActorTask::vf8`, `fn_1005EEF0`, `sub_10085E40`, `fn_1008CD90`, `sub_1008F740`, `fn_100929D0`, `sub_10092C20`, `sub_10092F90`, `CXiCollisionActor_OnMove`, `sub_100A57F0`, `sub_100C3900`, `sub_100C47F0`, `sub_100C8ED0`, `sub_100C9330`, `sub_100C9810`, `sub_100C9C10` and 5 more

```cpp
XI_API undefined4 __thiscall fn_100A57C0(void * self_, undefined4 * param_1)
{
  uint32_t uVar1;
  
  *(uint32_t *)((int)self_ + 0xd4) = *param_1;
  *(uint32_t *)((int)self_ + 0xd8) = param_1[1];
  *(uint32_t *)((int)self_ + 0xdc) = param_1[2];
  uVar1 = param_1[3];
  *(uint32_t *)((int)self_ + 0xe0) = uVar1;
  return uVar1;
}
```

## `0x100A89E0` fn_100A89E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `ActorHandle_Constructor`, `sub_100814F0`, `ActorHandle_Resolve`, `fn_10081CF0`, `fn_100A5770`, `Actor_SetSlotTableEntry` x2
- **Called by:** `CXiCollisionActor::Constructor`, `sub_100AC8E0`, `sub_100C3990`, `sub_100C41B0`, `sub_100C41F0`
- **Reads globals:** `0x1035BC0C`, `0x1035BC10`, `0x1035BC14`, `0x1035BC18`, `0x1035BC1C`, `0x1035BC20`, `0x1035BC24`, `0x1035BC28`, `0x1035BC2C`, `0x1035BC30`, `0x1035BC34`, `0x1035BC38`, `0x1057876C`

```cpp
XI_API void * __thiscall fn_100A89E0(void * self_)
{
  uint32_t uVar1;
  int iVar2;
  uint32_t *puVar3;
  int iVar4;
  uint32_t *puVar5;
  uint64_t uVar6;
  
  orig::FUN_10081cf0(self_);
  *(uint8_t ***)self_ = &PTR_CXiAtelActor_GetRuntimeClass_variant0_1032e3c8;
  orig::ActorHandle_Constructor_v1((void *)((int)self_ + 0x5a0));
  *(uint8_t ***)self_ = &PTR_CXiControlActor_GetRuntimeClass_variant1_1032dfc8;
  *(uint32_t *)((int)self_ + 0xf4) = 0;
  *(uint32_t *)((int)self_ + 0xe0) = 0x3f800000;
  *(uint8_t *)((int)self_ + 0xf8) = 0;
  *(uint8_t *)((int)self_ + 0xf9) = 1;
  *(uint8_t *)((int)self_ + 0xfb) = 0;
  *(uint8_t *)((int)self_ + 0x103) = 0;
  *(uint8_t *)((int)self_ + 0x104) = 0;
  *(uint8_t *)((int)self_ + 0x102) = 0;
  *(uint32_t *)((int)self_ + 0xfc) = 0;
  *(uint8_t *)((int)self_ + 0x100) = 0;
  *(uint32_t *)((int)self_ + 0x594) = 0;
  *(uint32_t *)((int)self_ + 0x598) = 0;
  *(uint32_t *)((int)self_ + 0x174) = 0;
  *(uint32_t *)((int)self_ + 0x178) = 0;
  *(uint8_t *)((int)self_ + 0x17c) = 0;
  *(uint32_t *)((int)self_ + 0x160) = 0;
  *(uint32_t *)((int)self_ + 0x164) = 0xbf800000;
  *(uint32_t *)((int)self_ + 0x168) = 0;
  *(uint32_t *)((int)self_ + 0x16c) = 0x3f800000;
  if (g_pTargetMng_as_s32 != 0) {
    orig::Actor_SetSlotTableEntry_v1(3,&LAB_100a8c20);
    orig::Actor_SetSlotTableEntry_v1(2,&LAB_100a8c40);
    if (*(char *)(g_pTargetMng_as_s32 + 0x50) == '\x01') {
      *(uint8_t *)((int)self_ + 0xfa) = 1;
    }
    else {
      *(uint8_t *)((int)self_ + 0xfa) = 0;
    }
  }
  orig::FUN_100814f0((void *)((int)self_ + 0x5a0),0);
  *(uint16_t *)((int)self_ + 0x5ac) = 0;
  *(uint16_t *)((int)self_ + 0x5ae) = 0;
  *(uint8_t *)((int)self_ + 0x5b0) = 0;
  *(uint32_t *)((int)self_ + 0x108) = 0;
  *(uint32_t *)((int)self_ + 0x10c) = 0;
  *(uint32_t *)((int)self_ + 0x110) = 0;
  *(uint32_t *)((int)self_ + 0x114) = 0;
  *(uint32_t *)((int)self_ + 0x130) = DAT_1035bc0c;
  *(uint32_t *)((int)self_ + 0x134) = DAT_1035bc10;
  *(uint32_t *)((int)self_ + 0x138) = DAT_1035bc14;
  *(uint32_t *)((int)self_ + 0x13c) = DAT_1035bc18;
  *(uint32_t *)((int)self_ + 0x140) = 0x3e000000;
  *(uint32_t *)((int)self_ + 0x148) = 0;
  *(uint32_t *)((int)self_ + 0x144) = 0;
  *(uint32_t *)((int)self_ + 0x150) = DAT_1035bc1c;
  *(uint32_t *)((int)self_ + 0x154) = DAT_1035bc20;
  *(uint32_t *)((int)self_ + 0x158) = DAT_1035bc24;
  *(uint32_t *)((int)self_ + 0x15c) = DAT_1035bc28;
  *(uint32_t *)((int)self_ + 0x14c) = 0;
  *(uint8_t *)((int)self_ + 0x101) = 0;
  *(uint32_t *)((int)self_ + 0x118) = 0;
  *(uint32_t *)((int)self_ + 0x11c) = 0;
  *(uint32_t *)((int)self_ + 0x120) = DAT_1035bc2c;
  *(uint32_t *)((int)self_ + 0x124) = DAT_1035bc30;
  *(uint32_t *)((int)self_ + 0x128) = DAT_1035bc34;
  uVar1 = DAT_1035bc38;
  *(uint8_t *)((int)self_ + 0x5b1) = 0;
  *(uint32_t *)((int)self_ + 300) = uVar1;
  iVar4 = 5;
  puVar3 = (uint32_t *)((int)self_ + 0x388);
  do {
    puVar5 = puVar3 + -0x82;
    for (iVar2 = 0x1a; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    }
    iVar4 = iVar4 + -1;
    puVar5 = puVar3;
    for (iVar2 = 0x1a; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar5 = 0;
      puVar5 = puVar5 + 1;
    }
    puVar3 = puVar3 + 0x1a;
  } while (iVar4 != 0);
  uVar6 = orig::ActorHandle_Resolve_v1(&g_targetActorHandle_as_v0);
  if ((void *)uVar6 == self_) {
    orig::FUN_100a5770(&DAT_10487f1c,(uint32_t *)((int)self_ + 0xd4));
  }
  return self_;
}
```

## `0x100AAEF0` fn_100AAEF0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100A6240`, `sub_100A79A0`, `sub_100A7B80`, `sub_100A8770`, `sub_100CD040`

```cpp
XI_API long double __stdcall fn_100AAEF0(int * param_1)
{
  char xi_frame[16] __attribute__((aligned(4)));
  float  (&local_10)[2] = *reinterpret_cast<float  (*)[2]>(xi_frame + 0);
  float  &fStack_8 = *reinterpret_cast<float  *>(xi_frame + 8);
  
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY))((**(code **)(*param_1 + 0x1c8))))((void *)(param_1),0x2c,local_10);
  return SQRT((long double)local_10[0] * (long double)local_10[0] + (long double)fStack_8 * (long double)fStack_8) *
         (long double)0.800000011920928955078125f;
}
```
