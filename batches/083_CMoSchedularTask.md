# Batch 083_CMoSchedularTask: 4 functions of `CMoSchedularTask`

Name these functions: see `batches/README.md`. Write `data/names/083_CMoSchedularTask.json`.
1 of them were sampled in a running frame (data/frame_set.json).

## `0x10057270` fn_10057270

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1003B540`, `fn_10055640`, `fn_100573B0`, `sub_100573E0`, `sub_10074740`, `sub_10074A00`
- **Called by:** `fn_10056CE0`

```cpp
XI_API void * __thiscall fn_10057270(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3)
{
  uint32_t *puVar1;
  int iVar2;
  
  orig::CMoTask_Constructor_v1(self_);
  orig::FUN_1003b540((uint32_t *)((int)self_ + 0x34));
  orig::FUN_10055640((void *)((int)self_ + 0xb4));
  puVar1 = (uint32_t *)((int)self_ + 0xc0);
  iVar2 = 0x10;
  do {
    *puVar1 = 0;
    puVar1 = puVar1 + 2;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  *(uint8_t ***)self_ = &PTR_CMoSchedularTask_GetRuntimeClass_1032b860;
  *(uint32_t *)((int)self_ + 0x34) = &PTR_LAB_1032b844;
  orig::FUN_100573b0(self_);
  orig::FUN_100573e0(self_,param_1,param_2,param_3);
  orig::FUN_10074a00();
  return self_;
}
```

## `0x100573B0` fn_100573B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100571E0`, `fn_10057270`, `sub_100572E0`, `sub_10057D60`

```cpp
XI_API void __thiscall fn_100573B0(void * self_)
{
  *(uint8_t *)((int)self_ + 0x141) = 0;
  *(uint8_t *)((int)self_ + 0x143) = 0;
  *(uint8_t *)((int)self_ + 0x144) = 0;
  *(uint8_t *)((int)self_ + 0x140) = 0;
  *(uint8_t *)((int)self_ + 0x145) = 0;
  *(uint32_t *)((int)self_ + 0x146) = 0;
  return;
}
```

## `0x10057CC0` fn_10057CC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100573E0`

```cpp
XI_API void __thiscall fn_10057CC0(void * self_, short * param_1)
{
  if (param_1 != (short *)0x0) {
    *(short **)((int)self_ + 0x90) = param_1;
    *param_1 = *param_1 + 1;
    orig::FUN_100684f0((void *)((int)self_ + 0xc0),0);
    return;
  }
  return;
}
```

## `0x10057E10` CMoSchedularTask::vf8

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10057C20`, `CMoSchedularTask_Interpret`, `sub_10062DE0`, `ResourceHandle_IsResident`
- **In vtables** (class_vtables.json): `CMoSchedularTask` slot 8 (+0x20)
- **Writes globals:** `0x1047BEF8`
- **Reads globals:** `0x1047BFA8`, `0x1065CB0C`

```cpp
undefined4 __thiscall CMoSchedularTask::vf8(void * self)
{
  uint32_t uVar1;
  uint32_t uVar2;
  uint16_t uVar5;
  int iVar3;
  uint32_t *puVar4;
  uint8_t bVar6;
  uint8_t bVar7;
  long double fVar8;
  long double fVar9;
  float fStack_8;
  
  if (*(char *)((int)self + 0x145) != '\0') {
    uVar1 = ((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self + 0x18))))((void *)(self),1);
LAB_10057e27:
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)uVar1 >> 8)) << 8) | (uint8_t)(1)));
  }
  uVar2 = orig::ResourceHandle_IsResident_v1((void *)((int)self + 0x80));
  if ((char)uVar2 == '\0') {
LAB_10057ef9:
    return uVar2 & 0xffffff00;
  }
  uVar1 = ((uint32_t)(((uint32_t)((int32_t)(uVar2 >> 8)) << 8) | (uint8_t)(*(char *)((int)self + 0x140))));
  fStack_8 = 0.0f;
  if (*(char *)((int)self + 0x140) == '\0') {
    fVar8 = (long double)*(float *)(g_pResourceMng + 0xeb0);
    *(float *)((int)self + 0x98) = (float)((long double)*(float *)((int)self + 0x98) - fVar8);
  }
  else {
    *(float *)((int)self + 0x98) =
         (float)((long double)*(float *)((int)self + 0x98) - (long double)_DAT_1065cb0c);
    fVar8 = (long double)_DAT_1065cb0c;
  }
  *(float *)((int)self + 0x94) = (float)(fVar8 + (long double)*(float *)((int)self + 0x94));
  Fn10057E10_static_BEF8 = 0;
  do {
    bVar6 = NAN((long double)*(float *)((int)self + 0x98)) || NAN((long double)0.0f);
    uVar5 = (uint16_t)((uint32_t)uVar1 >> 0x10);
    if ((bVar6 || (long double)*(float *)((int)self + 0x98) < (long double)0.0f) != bVar6) {
      fStack_8 = (float)-(long double)*(float *)((int)self + 0x98);
      *(uint32_t *)((int)self + 0x98) = 0;
    }
    bVar6 = NAN((long double)*(float *)((int)self + 0x98)) || NAN((long double)0.0f);
    bVar7 = bVar6 || (long double)*(float *)((int)self + 0x98) == (long double)0.0f;
    iVar3 = *(int *)((int)self + 0x88);
    if (iVar3 == 0) {
      if (bVar6 != bVar7) {
        uVar2 = orig::FUN_10057c20(self);
        if ((char)uVar2 != '\0') {
          *(uint8_t *)((int)self + 0x145) = 1;
        }
        goto LAB_10057ef9;
      }
    }
    else if (bVar6 != bVar7) {
      while (iVar3 != 0) {
        *(float *)((int)self + 0x98) =
             (float)((long double)(int)*(short *)(iVar3 + 4) * (long double)*(float *)((int)self + 0x9c) +
                    (long double)*(float *)((int)self + 0x98));
        iVar3 = orig::CMoSchedularTask_Interpret(self);
        uVar1 = 0;
        if (iVar3 == 1) goto LAB_10057e27;
        uVar2 = 0;
        if (iVar3 == 2) goto LAB_10057ef9;
        uVar2 = **(uint32_t **)((int)self + 0x88);
        if ((char)uVar2 == '\0') {
          puVar4 = (uint32_t *)0x0;
        }
        else {
          puVar4 = *(uint32_t **)((int)self + 0x88) + (uVar2 >> 8 & 0x1f);
        }
        uVar1 = orig::FUN_10062de0(self,(char *)puVar4);
        bVar6 = NAN((long double)*(float *)((int)self + 0x98)) || NAN((long double)0.0f);
        uVar5 = (uint16_t)((uint32_t)uVar1 >> 0x10);
        if (bVar6 == (bVar6 || (long double)*(float *)((int)self + 0x98) == (long double)0.0f)) {
          *(float *)((int)self + 0x98) =
               (float)((long double)*(float *)((int)self + 0x98) - (long double)fStack_8);
          break;
        }
        iVar3 = *(int *)((int)self + 0x88);
      }
    }
    fVar9 = (long double)*(float *)((int)self + 0x98);
    fVar8 = (long double)0.0f;
    bVar7 = NAN(fVar9) || NAN(fVar8);
    bVar6 = bVar7 || fVar9 < fVar8;
    uVar1 = ((uint32_t)(((uint32_t)(uint16_t)(uVar5) << 16) | (uint16_t)((uint16_t)bVar6 << 8 | (uint16_t)bVar7 << 10 |
                           (uint16_t)(bVar7 || fVar9 == fVar8) << 0xe)));
    if (bVar6 == bVar7) {
      return uVar1;
    }
  } while( true );
}
```
