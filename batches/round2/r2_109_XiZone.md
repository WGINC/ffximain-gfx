# Batch r2_109_XiZone: 3 functions of `XiZone`

Name these functions: see `batches/README.md`. Write `data/names/r2_109_XiZone.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10188B10` fn_10188B10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1004A980`, `sub_1004E8D0`
- **Reads globals:** `0x1065CB04`
- **Named before** (a naming session's reading, low): `XiZone_TestHeadFlag`; name it again from this dossier, or leave it out to keep that name
- **Named globals** (TREE/include/gfx_globals.hpp): `g_pXiZone`
- **From the frame root** (shortest chain of direct calls): `Frame_Render` → `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `sub_1004E8D0` → `XiZone_TestHeadFlag`
- **Named callers' callers:** `CMoElem_AdvanceTimer`

```cpp
XI_API uint32_t __thiscall XiZone_TestHeadFlag(void * self_)
{
  uint8_t *pbVar1;
  void *pvVar2;
  
  if ((*(int *)(g_pXiZone_as_s32 + 0x1b4) != 0xb6) && (*(int *)(g_pXiZone_as_s32 + 0x1b4) != 0x84)) {
    if (*(uint8_t **)((int)self_ + 0x18) != (uint8_t *)0x0) {
      return (uint8_t)~**(uint8_t **)((int)self_ + 0x18) & 1;
    }
    return 0;
  }
  pbVar1 = *(uint8_t **)((int)self_ + 0x18);
  pvVar2 = (void *)0x0;
  if (pbVar1 != (uint8_t *)0x0) {
    if ((*pbVar1 & 1) == 0) {
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)pbVar1 >> 8)) << 8) | (uint8_t)(1)));
    }
    pvVar2 = *(void **)(g_pXiZone_as_s32 + 0x1c4);
    if ((pvVar2 != (void *)0x0) && (self_ == pvVar2)) {
      return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)pvVar2 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  return (uint32_t)pvVar2 & 0xffffff00;
}
```

## `0x1018B5E0` fn_1018B5E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101875C0`, `fn_101875E0`, `XiZone_UpdateTree` (medium)
- **Called by:** `sub_10014390`, `sub_100940F0`
- **Named before** (a naming session's reading, low): `XiZone_SetTreeState1BC`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `XiZone_SetTreeState1BC`
- **Named callees' callees:** `Env_UpdateTimeKeys`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `XiZone_UpdateTree` (medium): Updates the zone's child and each entry of its list.

Short callee `fn_101875C0`:
```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101875C0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  void *this_00;
  
  for (this_00 = *(void **)((int)self_ + 0x10); this_00 != (void *)0x0;
      this_00 = *(void **)((int)this_00 + 8)) {
    xi_eax = orig::FUN_1018ab40(this_00,xi_eax);
  }
  return xi_eax;
}
```

Short callee `fn_101875E0`:
```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_101875E0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  void *this_00;
  
  for (this_00 = *(void **)((int)self_ + 0x10); this_00 != (void *)0x0;
      this_00 = *(void **)((int)this_00 + 8)) {
    xi_eax = orig::FUN_1018aac0(this_00,xi_eax);
  }
  return xi_eax;
}
```

```cpp
XI_API void __thiscall XiZone_SetTreeState1BC(void * self_, int param_1)
{
  uint32_t xi_eax;
  int xi_eax_00;
  
  xi_eax_00 = *(int *)((int)self_ + 0x1bc);
  if (xi_eax_00 != param_1) {
    *(int *)((int)self_ + 0x1bc) = param_1;
    do {
      xi_eax = XiZone_UpdateTree(self_,xi_eax_00);
      if (param_1 == 0) {
        xi_eax_00 = orig::FUN_101875e0(self_,xi_eax);
      }
      else {
        xi_eax_00 = orig::FUN_101875c0(self_,xi_eax);
      }
      self_ = *(void **)((int)self_ + 4);
    } while (self_ != (void *)0x0);
  }
  return;
}
```

## `0x1018B8B0` fn_1018B8B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10036170`
- **Called by:** `XiZone_DrawScene`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `XiZone_DrawScene` → `fn_1018B8B0`
- **Named callers' callers:** `Frame_Render`
- **How its callers call it** (their verified code):
  - in `XiZone_DrawScene`: `orig::FUN_1018b8b0(*piVar1);`
  - in `XiZone_DrawScene`: `orig::FUN_1018b8b0(0);`

```cpp
XI_API void __stdcall fn_1018B8B0(int param_1)
{
  orig::FUN_10036170(param_1);
  return;
}
```
