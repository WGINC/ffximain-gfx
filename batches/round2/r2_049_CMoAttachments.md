# Batch r2_049_CMoAttachments: 3 functions of `CMoAttachments`

Name these functions: see `batches/README.md`. Write `data/names/r2_049_CMoAttachments.json`.
2 of them were sampled in a running frame (data/frame_set.json).

## `0x1003B8B0` fn_1003B8B0

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0` x2
- **Called by:** `fn_1003C7D0`, `sub_100575A0`, `CMoSchedularTask_Interpret`
- **Named before** (a naming session's reading, low): `CMoAttachments_FindKindInChain`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `sub_1004E8D0` → `fn_1003C7D0` → `CMoAttachments_FindKindInChain`
- **Named callers' callers:** `CMoSchedularTask_Tick`
- **Named callees' callees:** `ActorHandle_Resolve`
- **How its callers call it** (their verified code):
  - in `fn_1003C7D0`: `if (((*param_2 & 2) == 0) && (iVar1 = CMoAttachments_FindKindInChain(self_), iVar1 != 0)) {`
  - in `fn_1003C7D0`: `piVar2 = (int *)CMoAttachments_FindKindInChain(self_);`

```cpp
XI_API int __thiscall CMoAttachments_FindKindInChain(void * self_)
{
  char cVar1;
  void *this_00;
  void *this_01;
  uint64_t uVar2;
  
  uVar2 = orig::FUN_1003b6d0(self_);
  this_00 = (void *)uVar2;
  if (this_00 == (void *)0x0) {
    return 0;
  }
  uVar2 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
  cVar1 = (char)uVar2;
  while( true ) {
    if (cVar1 == '\0') {
      return (int)this_00;
    }
    uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
    this_01 = (void *)uVar2;
    if (this_01 == (void *)0x0) break;
    uVar2 = orig::CYyObject_IsKindOf(this_01,0x1032f910);
    cVar1 = (char)uVar2;
    this_00 = this_01;
  }
  return (int)this_00;
}
```

## `0x1003B900` fn_1003B900

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyObject_IsKindOf` x2, `sub_1003B6D0`, `sub_1003B700`
- **Called by:** `fn_1003C7D0`, `sub_100575A0`, `CMoSchedularTask_Interpret`
- **Named before** (a naming session's reading, low): `CMoAttachments_FindKindFromRoot`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `sub_1004E8D0` → `fn_1003C7D0` → `CMoAttachments_FindKindFromRoot`
- **Named callers' callers:** `CMoSchedularTask_Tick`
- **Named callees' callees:** `ActorHandle_Resolve`
- **How its callers call it** (their verified code):
  - in `fn_1003C7D0`: `if (((*param_2 & 4) == 0) && (iVar1 = CMoAttachments_FindKindFromRoot(self_), iVar1 != 0)) {`
  - in `fn_1003C7D0`: `piVar2 = (int *)CMoAttachments_FindKindFromRoot(self_);`

```cpp
XI_API int __thiscall CMoAttachments_FindKindFromRoot(void * self_)
{
  char cVar1;
  void *this_00;
  void *this_01;
  uint64_t uVar2;
  
  uVar2 = orig::FUN_1003b700(self_);
  this_00 = (void *)uVar2;
  if (this_00 == (void *)0x0) {
    return 0;
  }
  uVar2 = orig::CYyObject_IsKindOf(this_00,0x1032f910);
  cVar1 = (char)uVar2;
  while( true ) {
    if (cVar1 == '\0') {
      return (int)this_00;
    }
    uVar2 = orig::FUN_1003b6d0((void *)((int)this_00 + 0xc4));
    this_01 = (void *)uVar2;
    if (this_01 == (void *)0x0) break;
    uVar2 = orig::CYyObject_IsKindOf(this_01,0x1032f910);
    cVar1 = (char)uVar2;
    this_00 = this_01;
  }
  return (int)this_00;
}
```

## `0x1003BD90` fn_1003BD90

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `YmGenerater_BuildAttachMatrix` (medium)
- **Named before** (a naming session's reading, low): `CMoAttachments_SetMappedSlot`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `fn_10036230` → `fn_100364A0` → `fn_100364E0` → `CMoGeneratorClone_ExecuteCmdList` → `sub_100496F0` → `YmGenerater_UpdateAttach` → `YmGenerater_BuildAttachMatrix` → `CMoAttachments_SetMappedSlot`
- **Named callers' callers:** `YmGenerater_UpdateAttach`
- **How its callers call it** (their verified code):
  - in `YmGenerater_BuildAttachMatrix` (medium): `CMoAttachments_SetMappedSlot((int)a2, v30, v31);`
  - in `YmGenerater_BuildAttachMatrix` (medium): `CMoAttachments_SetMappedSlot((int)a2, (unsigned __int8)HIBYTE(*(_WORD *)(self_ + 4)) >> 2, v32);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `YmGenerater_BuildAttachMatrix` (medium): 

```cpp
XI_API void __cdecl CMoAttachments_SetMappedSlot(undefined4 param_1, undefined4 param_2, int * param_3)
{
  switch(param_2) {
  case 0x36:
    param_2 = 0x7e;
    break;
  case 0x37:
    param_2 = 0x7f;
    break;
  case 0x38:
    param_2 = 0x66;
    break;
  case 0x39:
    param_2 = 0x67;
    break;
  case 0x3a:
    param_2 = 0x68;
    break;
  case 0x3b:
    param_2 = 0x69;
    break;
  case 0x3c:
    param_2 = 0x6a;
  }
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY))((**(code **)(*param_3 + 0x1d0))))((void *)(param_3),param_2,param_1);
  return;
}
```
