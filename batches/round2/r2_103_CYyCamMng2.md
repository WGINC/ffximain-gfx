# Batch r2_103_CYyCamMng2: 4 functions of `CYyCamMng2`

Name these functions: see `batches/README.md`. Write `data/names/r2_103_CYyCamMng2.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1001E2F0` fn_1001E2F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `CYyCamMng2_Construct` (medium), `sub_100A6240`
- **Writes globals:** `0x10456DB0`, `0x10456DB1`, `0x10456DB4`
- **Reads globals:** `0x10456DB0`
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `CYyCamMng2::CreateGlobal` → `CYyCamMng2_Construct` → `fn_1001E2F0`
- **Named callers' callers:** `CYyCamMng2::CreateGlobal`
- **How its callers call it** (their verified code):
  - in `CYyCamMng2_Construct` (medium): `orig::FUN_1001e2f0(0,1,0,xi_eax);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYyCamMng2_Construct` (medium): Constructs the camera manager and clears its global state.

```cpp
XI_API undefined4 __attribute__((cdecl, regparm(1))) fn_1001E2F0(undefined4 xi_eax, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  if (((DAT_10456db0 == '\0') && ((char)param_1 != '\0')) || ((char)param_2 != '\0')) {
    DAT_10456db1 = 0;
  }
  DAT_10456db0 = (char)param_1;
  _DAT_10456db4 = param_3;
  return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)xi_eax >> 8)) << 8) | (uint8_t)((char)param_1)));
}
```

## `0x1001E4D0` fn_1001E4D0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetField50` x2
- **Called by:** `sub_1001EE60`, `Camera_GetWrappedYaw` (low)
- **Named before** (a naming session's reading, low): `CYyCamMng2_GetYaw`; name it again from this dossier, or leave it out to keep that name
- **Named callers' callers:** `XiControlActor_UpdateCamera`
- **How its callers call it** (their verified code):
  - in `Camera_GetWrappedYaw` (low): `fVar3 = CYyCamMng2_GetYaw();`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `Camera_GetWrappedYaw` (low): Returns a camera-relative angle wrapped to the range minus pi to pi.

Short callee `Camera_GetField50`:
```cpp
XI_API undefined4 __cdecl Camera_GetField50(void)
{
  return *(uint32_t *)(g_pCamera_as_s32 + 0x50);
}
```

```cpp
XI_API long double __cdecl CYyCamMng2_GetYaw(void)
{
  int iVar1;
  int iVar2;
  long double fVar3;
  
  iVar1 = orig::Camera_GetField50_v1();
  iVar2 = orig::Camera_GetField50_v1();
  fVar3 = xi_castbits<long double>(fpatan(-(long double)*(float *)(iVar2 + 0x2c),(long double)*(float *)(iVar1 + 0x24)));
  return fVar3;
}
```

## `0x1001E4F0` fn_1001E4F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10015290`, `Vec3_Set` (medium), `Vec3_Copy` x4
- **Called by:** `CYyCamMng2_SetDefaultDistances` (low), `sub_1018B500`
- **Named before** (a naming session's reading, low): `CYyCamMng2_Reset`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `CYyCamMng2::CreateGlobal` → `CYyCamMng2_SetDefaultDistances` → `CYyCamMng2_Reset`
- **Named callers' callers:** `CYyCamMng2::CreateGlobal`, `CYyCamMng2_Construct`
- **Named callees' callees:** `Camera_SetField2F8`
- **How its callers call it** (their verified code):
  - in `CYyCamMng2_SetDefaultDistances` (low): `uVar1 = CYyCamMng2_Reset(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYyCamMng2_SetDefaultDistances` (low): Resets the camera and sets its default distances.
  - `Vec3_Set` (medium): Sets a 3-component vector from three floats and returns it.

Short callee `fn_10015290`:
```cpp
XI_API void __cdecl fn_10015290(undefined4 param_1)
{
  Camera_SetField2F8(g_pCamera_as_v2,param_1);
  return;
}
```

Short callee `Vec3_Set` (medium):
```cpp
XI_API undefined4 __cdecl Vec3_Set(float * param_1, float param_2, float param_3, float param_4)
{
  *param_1 = (float)(long double)param_2;
  param_1[1] = param_3;
  param_1[2] = param_4;
  return param_1;
}
```

Short callee `Vec3_Copy`:
```cpp
XI_API undefined4 __cdecl Vec3_Copy(undefined4 * param_1, undefined4 * param_2)
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
XI_API undefined4 __thiscall CYyCamMng2_Reset(void * self_)
{
  uint32_t *puVar1;
  
  puVar1 = (uint32_t *)((int)self_ + 0x44);
  orig::Vec3_Copy_v1(puVar1,(uint32_t *)&DAT_1035122c);
  orig::FUN_10015290(0x43af0000);
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x50),puVar1);
  *(float *)((int)self_ + 0x58) = (float)((long double)*(float *)((int)self_ + 0x58) + (long double)1.0f);
  *(uint32_t *)((int)self_ + 0x5c) = 0;
  Vec3_Set((float *)((int)self_ + 0xa8),0.0f,(-1.0f),0.0f);
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x6c),(uint32_t *)((int)self_ + 0x50));
  orig::Vec3_Copy_v1((uint32_t *)((int)self_ + 0x60),puVar1);
  *(uint32_t *)((int)self_ + 0x78) = *(uint32_t *)((int)self_ + 0x5c);
  *(uint32_t *)((int)self_ + 0x84) = 0;
  *(uint32_t *)((int)self_ + 0x80) = 0;
  *(uint32_t *)((int)self_ + 0x7c) = 0;
  *(uint8_t *)((int)self_ + 0x88) = 0;
  *(uint8_t *)((int)self_ + 0x89) = 0;
  *(uint8_t *)((int)self_ + 0x8a) = 0;
  *(uint8_t *)((int)self_ + 0xf4) = 0;
  return *(uint32_t *)((int)self_ + 0x5c);
}
```

## `0x1001E590` fn_1001E590

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyCamMng2_Reset` (low)
- **Called by:** `CYyCamMng2::CreateGlobal`, `CYyCamMng2_Construct` (medium)
- **Named before** (a naming session's reading, low): `CYyCamMng2_SetDefaultDistances`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `sub_10013AD0` → `CYyCamMng2::CreateGlobal` → `CYyCamMng2_SetDefaultDistances`
- **Named callers' callers:** `App_InitCamera`
- **Named callees' callees:** `Vec3_Set`, `Vec3_Copy`
- **How its callers call it** (their verified code):
  - in `CYyCamMng2::CreateGlobal`: `CYyCamMng2_SetDefaultDistances(DAT_10456d6c);`
  - in `CYyCamMng2_Construct` (medium): `xi_eax = CYyCamMng2_SetDefaultDistances(self_);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `CYyCamMng2_Reset` (low): Resets the camera manager's position and state vectors to their defaults.
  - `CYyCamMng2_Construct` (medium): Constructs the camera manager and clears its global state.

```cpp
XI_API undefined4 __thiscall CYyCamMng2_SetDefaultDistances(void * self_)
{
  uint32_t uVar1;
  
  uVar1 = CYyCamMng2_Reset(self_);
  *(uint32_t *)((int)self_ + 0xb4) = 0x40400000;
  *(uint32_t *)((int)self_ + 0xb8) = 0x40c00000;
  return uVar1;
}
```
