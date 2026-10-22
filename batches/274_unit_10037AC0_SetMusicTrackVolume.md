# Batch 274_unit_10037AC0_SetMusicTrackVolume: 4 functions of `unit_10037AC0_SetMusicTrackVolume`

Name these functions: see `batches/README.md`. Write `data/names/274_unit_10037AC0_SetMusicTrackVolume.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10037D70` fn_10037D70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `fn_10157B40`, `fn_10158DB0`, `sub_1024C710`
- **Writes globals:** `0x1046279C`
- **Reads globals:** `0x10351700`, `0x1046279C`, `0x1047D128`

```cpp
XI_API void __cdecl fn_10037D70(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_1046279c == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x39303030);
    DAT_1046279c = (undefined4 *)*(int *)uVar1;
    if (DAT_1046279c == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_1046279c;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x10037E30` fn_10037E30

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `fn_101585F0`, `fn_10158DB0`
- **Writes globals:** `0x104627CC`
- **Reads globals:** `0x10351700`, `0x104627CC`, `0x1047D128`

```cpp
XI_API void __cdecl fn_10037E30(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_104627cc == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x31313030);
    DAT_104627cc = (undefined4 *)*(int *)uVar1;
    if (DAT_104627cc == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_104627cc;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x10037E90` fn_10037E90

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10311C2C`
- **Called by:** `sub_101306F0`, `sub_10130E40`, `sub_101322B0`, `sub_10134250`, `sub_10135270`, `sub_10135BD0`, `sub_10139250`, `sub_1013E880`, `sub_1013F2E0`, `sub_101431B0`, `sub_10143CD0`, `sub_101515E0`, `fn_10157E20`, `sub_101A4950`, `sub_101A5010`, `sub_101A5690`, `sub_101A6180`, `sub_101A6350`, `sub_101DB690`, `sub_101DFA50` and 18 more
- **Writes globals:** `0x10462690`
- **Reads globals:** `0x10351700`, `0x10462690`, `0x1047D128`

```cpp
XI_API void __cdecl fn_10037E90(void)
{
  void *self_;
  uint64_t uVar1;
  int64_t lVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  uint8_t  (&local_4)[4] = *reinterpret_cast<uint8_t  (*)[4]>(xi_frame + 0);
  
  if (DAT_10462690 == (undefined4 *)0x0) {
    uVar1 = orig::CMoResource_FindByTypeAndId_v1
                      ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),local_4,0x3d,
                       0x33313030);
    DAT_10462690 = (undefined4 *)*(int *)uVar1;
    if (DAT_10462690 == (undefined4 *)0x0) {
      return;
    }
  }
  self_ = (void *)*DAT_10462690;
  if (self_ != (void *)0x0) {
    lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351700 * (long double)127.0f);
    orig::FUN_10036f00(self_,0x40,(int)lVar2);
  }
  return;
}
```

## `0x100380F0` fn_100380F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10028FD0`, `fn_10036F00`, `CMoResource::FindByTypeAndId`, `sub_10074740`, `CMoTask::ResetFields`, `sub_10311C2C`
- **Called by:** `sub_10127C00`
- **Writes globals:** `0x10463938`, `0x1047D13C`
- **Reads globals:** `0x10351710`, `0x10463938`, `0x1047D128`

```cpp
XI_API void __cdecl fn_100380F0(uint32_t param_1, undefined4 param_2, undefined4 param_3)
{
  void *self_;
  undefined4 *this_00;
  uint64_t uVar1;
  int64_t lVar2;
  
  if (DAT_10463938 == (undefined4 *)0x0) {
    uVar1 = orig::Mem_AllocFromArea_v1(0x40,4);
    this_00 = (undefined4 *)uVar1;
    orig::CMoTask_ResetFields_v1(this_00);
    g_resourcesDirty_as_v0 = 1;
    if (this_00 != (undefined4 *)0x0) {
      orig::CMoTask_Constructor_v1(this_00);
      *this_00 = &PTR_CMoTask_GetRuntimeClass_variant12_1032a9bc;
      DAT_10463938 = this_00;
      uVar1 = orig::CMoResource_FindByTypeAndId_v1
                        ((void *)**(undefined4 **)(g_pMeshResourceMng + 0x40008),&param_1,0x3d,
                         (param_1 & 0xff00 | param_1 << 0x10 | param_1 >> 0x10 & 0xff) << 8 |
                         param_1 >> 0x18);
      if (((undefined4 *)*(int *)uVar1 != (undefined4 *)0x0) &&
         (self_ = *(void **)*(int *)uVar1, self_ != (void *)0x0)) {
        lVar2 = XI_CALL_ST0(0x10311C2Cu)((long double)DAT_10351710 * (long double)param_2 * (long double)127.0f);
        uVar1 = orig::FUN_10036f00(self_,0x40,(int)lVar2);
        this_00[0xe] = (int)uVar1;
        this_00[0xd] = 0x42f00000;
        *(uint8_t *)(this_00 + 0xf) = (uint8_t)param_3;
        return;
      }
      this_00[0xd] = 0;
      *(uint8_t *)(this_00 + 0xf) = (uint8_t)param_3;
    }
  }
  return;
}
```
