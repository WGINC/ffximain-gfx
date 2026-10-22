# Batch r2_112_YmCombineWeather: 2 functions of `YmCombineWeather`

Name these functions: see `batches/README.md`. Write `data/names/r2_112_YmCombineWeather.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1018AAC0` fn_1018AAC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10072540`
- **Called by:** `fn_101875E0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `XiZone_SetTreeState1BC` → `fn_101875E0` → `fn_1018AAC0`
- **Named callers' callers:** `XiZone_SetTreeState1BC`
- **How its callers call it** (their verified code):
  - in `fn_101875E0`: `xi_eax = orig::FUN_1018aac0(this_00,xi_eax);`

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AAC0(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10072540(*(void **)((int)self_ + 0xc),5,0,&LAB_1018aae0,0,xi_eax);
  return uVar1;
}
```

## `0x1018AB40` fn_1018AB40

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10072540`
- **Called by:** `fn_101875C0`
- **From the frame root** (shortest chain of direct calls): `App_RenderFrameMaskedFPU` → `Frame_Render` → `App_DispatchSubState` → `sub_10014350` → `sub_10014390` → `XiZone_SetTreeState1BC` → `fn_101875C0` → `fn_1018AB40`
- **Named callers' callers:** `XiZone_SetTreeState1BC`
- **How its callers call it** (their verified code):
  - in `fn_101875C0`: `xi_eax = orig::FUN_1018ab40(this_00,xi_eax);`

```cpp
XI_API undefined4 __attribute__((stdcall, regparm(3))) fn_1018AB40(undefined4 xi_eax, uint32_t xi_edx_unused, void * self_)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10072540(*(void **)((int)self_ + 0xc),5,0,&LAB_1018ab60,0,xi_eax);
  return uVar1;
}
```
