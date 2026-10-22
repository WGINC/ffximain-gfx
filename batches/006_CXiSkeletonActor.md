# Batch 006_CXiSkeletonActor: 33 functions of `CXiSkeletonActor`

Name these functions: see `batches/README.md`. Write `data/names/006_CXiSkeletonActor.json`.
11 of them were sampled in a running frame (data/frame_set.json).

## `0x100A47F0` CXiSkeletonActor::vf256

- **Gate:** none; **frame set:** self
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 256 (+0x400), `XiSkeletonActor2` slot 256 (+0x400), `CXiSkeletonActor` slot 256 (+0x400), `CXiMannequinActor` slot 256 (+0x400), `XiChocoboActor2` slot 256 (+0x400), `CXiGeomActor` slot 256 (+0x400), `CXiDancerActor` slot 256 (+0x400)

```cpp
_DWORD * CXiSkeletonActor::vf256(_DWORD *a2)
{
  *a2 = this->field_864;
  return a2;
}
```

## `0x100A4820` CXiSkeletonActor::vf257

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 257 (+0x404), `XiSkeletonActor2` slot 257 (+0x404), `CXiSkeletonActor` slot 257 (+0x404), `XiChocoboActor2` slot 257 (+0x404), `CXiGeomActor` slot 257 (+0x404), `CXiDancerActor` slot 257 (+0x404)

```cpp
char CXiSkeletonActor::vf257()
{
  return 0;
}
```

## `0x100A4850` CXiSkeletonActor::vf017

- **Gate:** none
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 17 (+0x44), `XiSkeletonActor2` slot 17 (+0x44), `CXiSkeletonActor` slot 17 (+0x44), `CXiMannequinActor` slot 17 (+0x44), `XiChocoboActor2` slot 17 (+0x44), `CXiGeomActor` slot 17 (+0x44), `CXiDancerActor` slot 17 (+0x44)

```cpp
int CXiSkeletonActor::vf017()
{
  return at<uint8_t>(this, 0x663);
}
```

## `0x100A4970` CXiSkeletonActor::vf258

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 258 (+0x408), `XiSkeletonActor2` slot 258 (+0x408), `CXiSkeletonActor` slot 258 (+0x408), `CXiMannequinActor` slot 258 (+0x408), `XiChocoboActor2` slot 258 (+0x408), `CXiGeomActor` slot 258 (+0x408), `CXiDancerActor` slot 258 (+0x408)

```cpp
_DWORD * CXiSkeletonActor::vf258(_DWORD *a2)
{
  *a2 = at<uint32_t>(this, 0x750);
  return a2;
}
```

## `0x100A4A00` CXiSkeletonActor::vf030

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 30 (+0x78), `XiSkeletonActor2` slot 30 (+0x78), `CXiSkeletonActor` slot 30 (+0x78), `CXiMannequinActor` slot 30 (+0x78), `XiChocoboActor2` slot 30 (+0x78), `CXiGeomActor` slot 30 (+0x78), `CXiDancerActor` slot 30 (+0x78)

```cpp
BOOL CXiSkeletonActor::vf030()
{
  return (this->effectFlags & 8) != 0;
}
```

## `0x100A4A90` CXiSkeletonActor::vf041

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 41 (+0xA4), `XiSkeletonActor2` slot 41 (+0xA4), `CXiSkeletonActor` slot 41 (+0xA4), `CXiMannequinActor` slot 41 (+0xA4), `XiChocoboActor2` slot 41 (+0xA4), `CXiGeomActor` slot 41 (+0xA4), `CXiDancerActor` slot 41 (+0xA4)

```cpp
char CXiSkeletonActor::vf041()
{
  return this->field_8A5;
}
```

## `0x100A4AA0` CXiSkeletonActor::vf042

- **Gate:** none
- **Facts:** `ret 4`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 42 (+0xA8), `XiSkeletonActor2` slot 42 (+0xA8), `CXiSkeletonActor` slot 42 (+0xA8), `CXiMannequinActor` slot 42 (+0xA8), `XiChocoboActor2` slot 42 (+0xA8), `CXiGeomActor` slot 42 (+0xA8), `CXiDancerActor` slot 42 (+0xA8)

```cpp
char CXiSkeletonActor::vf042(char a2)
{
  this->field_8A5 = a2;
  return a2;
}
```

## `0x100A4AB0` CXiSkeletonActor::vf043

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 43 (+0xAC), `XiSkeletonActor2` slot 43 (+0xAC), `CXiSkeletonActor` slot 43 (+0xAC), `CXiMannequinActor` slot 43 (+0xAC), `XiChocoboActor2` slot 43 (+0xAC), `CXiGeomActor` slot 43 (+0xAC), `CXiDancerActor` slot 43 (+0xAC)

```cpp
char CXiSkeletonActor::vf043()
{
  return this->field_8A6;
}
```

## `0x100A4AC0` CXiSkeletonActor::vf044

- **Gate:** none
- **Facts:** `ret 4`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 44 (+0xB0), `XiSkeletonActor2` slot 44 (+0xB0), `CXiSkeletonActor` slot 44 (+0xB0), `CXiMannequinActor` slot 44 (+0xB0), `XiChocoboActor2` slot 44 (+0xB0), `CXiGeomActor` slot 44 (+0xB0), `CXiDancerActor` slot 44 (+0xB0)

```cpp
char CXiSkeletonActor::vf044(char a2)
{
  this->field_8A6 = a2;
  return a2;
}
```

## `0x100A4AD0` CXiSkeletonActor::vf045

- **Gate:** none; **frame set:** self
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 45 (+0xB4), `XiSkeletonActor2` slot 45 (+0xB4), `CXiSkeletonActor` slot 45 (+0xB4), `CXiMannequinActor` slot 45 (+0xB4), `XiChocoboActor2` slot 45 (+0xB4), `CXiGeomActor` slot 45 (+0xB4), `CXiDancerActor` slot 45 (+0xB4)

```cpp
char CXiSkeletonActor::vf045()
{
  return this->field_8A7;
}
```

## `0x100A4AE0` CXiSkeletonActor::vf046

- **Gate:** none
- **Facts:** `ret 4`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 46 (+0xB8), `XiSkeletonActor2` slot 46 (+0xB8), `CXiSkeletonActor` slot 46 (+0xB8), `CXiMannequinActor` slot 46 (+0xB8), `XiChocoboActor2` slot 46 (+0xB8), `CXiGeomActor` slot 46 (+0xB8), `CXiDancerActor` slot 46 (+0xB8)

```cpp
char CXiSkeletonActor::vf046(char a2)
{
  this->field_8A7 = a2;
  return a2;
}
```

## `0x100A4AF0` CXiSkeletonActor::vf047

- **Gate:** none
- **Facts:** `ret 0`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 47 (+0xBC), `XiSkeletonActor2` slot 47 (+0xBC), `CXiSkeletonActor` slot 47 (+0xBC), `CXiMannequinActor` slot 47 (+0xBC), `XiChocoboActor2` slot 47 (+0xBC), `CXiGeomActor` slot 47 (+0xBC), `CXiDancerActor` slot 47 (+0xBC)

```cpp
char CXiSkeletonActor::vf047()
{
  return this->field_8A8;
}
```

## `0x100A4B20` CXiSkeletonActor::vf050

- **Gate:** none
- **Facts:** `ret 4`; sets only AL of EAX; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 50 (+0xC8), `XiSkeletonActor2` slot 50 (+0xC8), `CXiSkeletonActor` slot 50 (+0xC8), `CXiMannequinActor` slot 50 (+0xC8), `XiChocoboActor2` slot 50 (+0xC8), `CXiGeomActor` slot 50 (+0xC8), `CXiDancerActor` slot 50 (+0xC8)

```cpp
char CXiSkeletonActor::vf050(char a2)
{
  this->field_8A9 = a2;
  return a2;
}
```

## `0x100A4B30` CXiSkeletonActor::vf099

- **Gate:** none
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 99 (+0x18C), `XiSkeletonActor2` slot 99 (+0x18C), `CXiSkeletonActor` slot 99 (+0x18C), `CXiMannequinActor` slot 99 (+0x18C), `XiChocoboActor2` slot 99 (+0x18C), `CXiGeomActor` slot 99 (+0x18C), `CXiDancerActor` slot 99 (+0x18C)

```cpp
void CXiSkeletonActor::vf099()
{
  this->forceUpdateFlag = 1;
}
```

## `0x100A4B40` CXiSkeletonActor::vf100

- **Gate:** none
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 100 (+0x190), `XiSkeletonActor2` slot 100 (+0x190), `CXiSkeletonActor` slot 100 (+0x190), `CXiMannequinActor` slot 100 (+0x190), `XiChocoboActor2` slot 100 (+0x190), `CXiGeomActor` slot 100 (+0x190), `CXiDancerActor` slot 100 (+0x190)

```cpp
void CXiSkeletonActor::vf100()
{
  at<uint8_t>(this, 0xA04) = 1;
}
```

## `0x100CD4E0` fn_100CD4E0

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 28`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyMotionQue::SetSlotMotion`, `CYyModel::GetField44Addr` x2
- **Called by:** `CMoSchedularTask_Interpret`, `fn_100CDF90`, `sub_100CE0E0`

```cpp
XI_API void __thiscall fn_100CD4E0(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4, undefined4 param_5, undefined4 param_6, int param_7)
{
  int *piVar1;
  
  piVar1 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if (*piVar1 != 0) {
    piVar1 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
    orig::CYyMotionQue_SetSlotMotion_v1
              ((void *)(*piVar1 + 0x24),self_,param_1,param_2,param_3,param_4,param_5,param_6,param_7
               ,0,0);
  }
  return;
}
```

## `0x100CDF90` fn_100CDF90

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `Camera_GetFrameScale`, `ResourceHandle_IsResident`, `__attribute__`, `fn_100CD4E0`, `CXiSkeletonActor_IsReadCompleteBody`, `sub_103149A0`
- **Called by:** `CXiSkeletonActor_OnMove`
- **Writes globals:** `0x1048A38C`

```cpp
XI_API void __thiscall fn_100CDF90(void * self_)
{
  uint32_t xi_entry_eax = 0;
  int iVar1;
  uint32_t uVar2;
  uint8_t bVar3;
  long double fVar4;
  char xi_frame[8] __attribute__((aligned(4)));
  int * &local_8 = *reinterpret_cast<int * *>(xi_frame + 0);
  uint32_t  &local_4 = *reinterpret_cast<uint32_t  *>(xi_frame + 4);
  
  fVar4 = orig::Camera_GetFrameScale_v1(xi_entry_eax);
  fVar4 = (long double)*(float *)((int)self_ + 0x8b8) - fVar4;
  *(float *)((int)self_ + 0x8b8) = (float)fVar4;
  bVar3 = NAN(fVar4) || NAN((long double)0.0f);
  if ((bVar3 || fVar4 < (long double)0.0f) != bVar3) {
    iVar1 = orig::_rand();
    local_8 = (int *)(iVar1 % 0x1e0);
    *(float *)((int)self_ + 0x8b8) = (float)(long double)(int)local_8;
    if (*(char *)((int)self_ + 0x8aa) == '\0') {
      uVar2 = orig::CXiSkeletonActor_IsReadCompleteBody
                        (self_,((uint32_t)(((uint32_t)((int32_t)((uint32_t)(iVar1 / 0x1e0) >> 8)) << 8) | (uint8_t)(*(char *)((int)self_ + 0x8aa)))));
      if ((char)uVar2 != '\0') {
        DAT_1048a38c_as_v0 = 0;
        local_8 = (int *)((int (__thiscall *)(void *, XI_ANY))((**(code **)(*(int *)self_ + 0x1e0))))((void *)(self_),&local_4);
        local_8 = (int *)*local_8;
        if (local_8 != (int *)0x0) {
          uVar2 = orig::ResourceHandle_IsResident_v1(&local_8);
          if ((char)uVar2 != '\0') {
            if (local_8 == (int *)0x0) {
              iVar1 = 0;
            }
            else {
              iVar1 = *local_8;
            }
            local_8 = (int *)orig::CMoResource_FindInScope(&local_4,iVar1,0x2b,0x33657965,local_8);
            local_8 = (int *)*local_8;
            if (local_8 != (int *)0x0) {
              orig::FUN_100cd4e0(self_,local_8,0x3f800000,0x3f800000,1,0x3f800000,0x3f800000,3);
            }
          }
        }
      }
    }
  }
  return;
}
```

## `0x100CE160` fn_100CE160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1001B170`, `CYyModel::GetField44Addr`
- **Called by:** `fn_10087E40`, `sub_100B2DD0`

```cpp
XI_API void __thiscall fn_100CE160(void * self_)
{
  int *piVar1;
  
  piVar1 = (int *)orig::CYyModel_GetField44Addr_v1((void *)((int)self_ + 0x674));
  if (*piVar1 != 0) {
    orig::FUN_1001b170((void *)(*piVar1 + 0x74),8.0f);
  }
  return;
}
```

## `0x100CF100` fn_100CF100

- **Gate:** none; **frame set:** self, stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CXiActor::GetModelField170`, `sub_100D5FE0`, `sub_100D6180`, `fn_100D6240`
- **Called by:** `sub_1008C480`, `sub_100973F0`

```cpp
XI_API uint64_t __thiscall fn_100CF100(void * self_, undefined4 xi_s1)
{
  int iVar1;
  uint32_t uVar2;
  uint32_t uVar3;
  int *piVar4;
  uint64_t uVar5;
  
  piVar4 = (int *)0x0;
  do {
    if (*(short *)((int)self_ + 0xb2) == 0) {
      iVar1 = orig::CXiActor_GetModelField170_v1(self_);
      if (iVar1 != 1) goto LAB_100cf141;
      uVar2 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 800))))((void *)(self_));
      if ((short)uVar2 == 0) goto LAB_100cf141;
      uVar3 = orig::FUN_100d6240(self_,piVar4);
      if ((char)uVar3 != '\0') goto LAB_100cf141;
      uVar5 = orig::FUN_100d5fe0(self_,(int)piVar4);
    }
    else {
LAB_100cf141:
      uVar5 = orig::FUN_100d6180(self_,(int)piVar4);
    }
    piVar4 = (int *)((int)piVar4 + 1);
    if (9 < (int)piVar4) {
      return ((uint64_t)(((uint64_t)(uint32_t)((int)((uint64_t)uVar5 >> 0x20)) << 32) | (uint32_t)(((uint32_t)(((uint32_t)((int32_t)((uint64_t)uVar5 >> 8)) << 8) | (uint8_t)(1))))));
    }
  } while( true );
}
```

## `0x100CF200` CXiSkeletonActor::vf172

- **Gate:** none
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1002AC50`, `CYyModel::GetField44Addr`
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 172 (+0x2B0), `XiSkeletonActor2` slot 172 (+0x2B0), `CXiSkeletonActor` slot 172 (+0x2B0), `CXiMannequinActor` slot 172 (+0x2B0), `XiChocoboActor2` slot 172 (+0x2B0), `CXiGeomActor` slot 172 (+0x2B0), `CXiDancerActor` slot 172 (+0x2B0)

```cpp
void CXiSkeletonActor::vf172(int a2)
{
  char * self = reinterpret_cast<char *>(this);  // byte-exact view for the rest
  _DWORD *v2;
  v2 = *(_DWORD **)orig::CYyModel_GetField44Addr(self + 1652);
  if ( v2 != nullptr )
    orig::CYyModelBase_ForwardToField24(v2, a2, 0);
}
```

## `0x100CFA50` fn_100CFA50

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyModelBase::AddPartFromResource`, `sub_10071E90`, `ResourceIter_FindFirst`, `ResourceIter_FindNext`, `CXiActor_DrawResourceShadows`
- **Called by:** `sub_100D0B50`

```cpp
XI_API void __thiscall fn_100CFA50(void * self_, int * param_1, void * param_2)
{
  undefined4 *puVar1;
  int iVar2;
  undefined4 *xi_eax;
  char xi_frame[28] __attribute__((aligned(4)));
  uint8_t  (&local_1c)[20] = *reinterpret_cast<uint8_t  (*)[20]>(xi_frame + 0);
  int  &local_8 = *reinterpret_cast<int  *>(xi_frame + 20);
  
  if (param_1 == (int *)0x0) {
    iVar2 = 0;
  }
  else {
    iVar2 = *param_1;
  }
  orig::ResourceIter_FindFirst_v1(local_1c,iVar2,0x2a,0,0xffffffff);
  while (local_8 != 0) {
    puVar1 = *(undefined4 **)(local_8 + 0x14);
    xi_eax = orig::CYyModelBase_AddPartFromResource(param_2,puVar1);
    orig::CMoResource_AddRefChain_v1((void *)*puVar1,xi_eax);
    orig::ResourceIter_FindNext_v1(local_1c,0);
  }
  if (((uint8_t)*(undefined4 *)(*param_1 + 0x24) & 0x7f) == 1) {
    orig::CXiActor_SpawnResourceGenerators_v1(self_,param_1);
  }
  return;
}
```

## `0x100CFAD0` fn_100CFAD0

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100CFB40`
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 242 (+0x3C8), `XiSkeletonActor2` slot 242 (+0x3C8), `CXiSkeletonActor` slot 242 (+0x3C8), `CXiMannequinActor` slot 242 (+0x3C8), `XiChocoboActor2` slot 242 (+0x3C8), `CXiGeomActor` slot 242 (+0x3C8), `CXiDancerActor` slot 242 (+0x3C8)

```cpp
XI_API uint8_t * __thiscall fn_100CFAD0(void * self_, int param_1)
{
  uint16_t *puVar1;
  
  puVar1 = orig::FUN_100cfb40(self_);
  if (puVar1 == (uint16_t *)0x0) {
    if ((param_1 != 0) && (param_1 == 1)) {
      return (uint8_t *)0x0;
    }
    puVar1 = (uint16_t *)&DAT_1048a390;
  }
  return (uint8_t *)puVar1;
}
```

## `0x100D0D40` fn_100D0D40

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10085010`, `sub_100CF750`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`, `sub_100D4100`

```cpp
XI_API undefined4 __thiscall fn_100D0D40(void * self_, int * param_1, int param_2)
{
  if ((param_2 == *(int *)((int)self_ + 0x9ec)) && (param_1 != (int *)0x0)) {
    orig::CXiActor_set_field_B4(self_,param_1);
    param_2 = (int)orig::FUN_100cf750(self_,param_1);
  }
  return param_2;
}
```

## `0x100D0FC0` fn_100D0FC0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 8`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `__attribute__`

```cpp
XI_API void __thiscall fn_100D0FC0(void * self_, undefined4 xi_s1, undefined4 xi_s2)
{
  uint8_t bVar1;
  int iVar2;
  int iVar3;
  uint32_t uVar4;
  
  iVar2 = (**(code **)(*(int *)self_ + 0x394))();
  iVar3 = (**(code **)(*(int *)self_ + 0x378))();
  if ((iVar3 == 0) && (iVar3 = ((int (__thiscall *)(void *))((**(code **)(*(int *)self_ + 0x36c))))((void *)(self_)), iVar3 == 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  iVar3 = (**(code **)(*(int *)self_ + 0x3a4))();
  if (iVar2 == 0) {
    if (bVar1) {
      ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x74696e69,self_,0);
      return;
    }
    if (iVar3 == 0) {
      return;
    }
  }
  else {
    if (*(int *)((int)self_ + 0x70) == 0) {
      return;
    }
    uVar4 = *(uint32_t *)(*(int *)((int)self_ + 0x70) + 0x134) >> 0x15 & 0xf;
    if (uVar4 != 2) {
      if (uVar4 != 3) {
        return;
      }
      ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x316e6f63,self_,0);
      return;
    }
  }
  ((int (__thiscall *)(void *, XI_ANY, XI_ANY, XI_ANY))((**(code **)(*(int *)self_ + 0x298))))((void *)(self_),0x306e6f63,self_,0);
  return;
}
```

## `0x100D43E0` fn_100D43E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10087830`
- **Called by:** `CXiSkeletonActor_LoadSkeletonSync`

```cpp
XI_API int __cdecl fn_100D43E0(void)
{
  uint32_t uVar1;
  
  uVar1 = orig::FUN_10087830();
  uVar1 = uVar1 & 0xff;
  if (1 < uVar1) {
    uVar1 = 0;
  }
  return (uint16_t)(&DAT_10330f34)[uVar1] + 0xca53;
}
```

## `0x100D4410` fn_100D4410

- **Gate:** none; **frame set:** stack
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_1001B710`, `sub_1002C7E0`, `Mem_AllocDefault`
- **Called by:** `fn_100D14C0`, `sub_100D16C0`, `sub_100D1970`, `sub_100D1C80`, `sub_100D1F70`, `sub_100D2110`, `sub_100D2220`, `sub_100D2460`, `sub_100D25E0`, `sub_100D2800`, `sub_100D2A10`, `sub_100D2BC0`

```cpp
XI_API int __thiscall fn_100D4410(void * self_, undefined4 param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4)
{
  void *pvVar1;
  
  pvVar1 = (void *)orig::Mem_AllocDefault_v1(0x24);
  if (pvVar1 != (void *)0x0) {
    pvVar1 = (void *)orig::FUN_1001b710(pvVar1);
    if (pvVar1 != (void *)0x0) {
      *(uint32_t *)((int)pvVar1 + 0x10) = param_2;
      *(uint32_t *)((int)pvVar1 + 0x14) = param_3;
      *(uint32_t *)((int)pvVar1 + 0x1c) = param_1;
      *(uint32_t *)((int)pvVar1 + 0xc) = param_4;
      orig::FUN_1002c7e0(pvVar1,(uint32_t *)((int)self_ + 0x7e4));
      return (int)pvVar1;
    }
  }
  return 0;
}
```

## `0x100D4540` fn_100D4540

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_100D1190`

```cpp
XI_API int __thiscall fn_100D4540(void * self_, int param_1)
{
  int iVar1;
  
  iVar1 = *(int *)((int)self_ + 0x7e4);
  while( true ) {
    if (iVar1 == 0) {
      return 0;
    }
    if (*(int *)(iVar1 + 0xc) == param_1) break;
    iVar1 = *(int *)(iVar1 + 4);
  }
  return iVar1;
}
```

## `0x100D6240` fn_100D6240

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CYyModelBase::GetPartList`, `CYyModel::GetField44Addr`, `fn_1002C710`, `fn_1002C750`
- **Called by:** `CXiSkeletonActor_Init`, `fn_100CF100`

```cpp
XI_API uint32_t __thiscall fn_100D6240(void * self_, int * param_1)
{
  int *piVar1;
  void *this_00;
  int iVar2;
  uint32_t uVar3;
  void *this_01;
  int xi_s1;
  
  if ((-1 < (int)param_1) || ((int)param_1 < 10)) {
    piVar1 = param_1 + 0xc;
    this_01 = (void *)((int)self_ + 0x674);
    param_1 = (int *)orig::CYyModel_GetField44Addr_v1(this_01);
    for (this_00 = (void *)*param_1; this_00 != (void *)0x0; this_00 = *(void **)((int)this_00 + 4))
    {
      xi_s1 = 0;
      param_1 = (int *)orig::CYyModelBase_GetPartList_v1(this_00);
      for (iVar2 = *param_1; iVar2 != 0; iVar2 = *(int *)(iVar2 + 4)) {
        param_1 = (int *)orig::FUN_1002c710(this_01,xi_s1);
        if (param_1 == (int *)((int)piVar1 * 0x1000000 | 0x706577U)) {
          uVar3 = orig::FUN_1002c750(this_01,xi_s1);
          return uVar3;
        }
        xi_s1 = xi_s1 + 1;
      }
    }
  }
  return (uint32_t)param_1 & 0xffffff00;
}
```

## `0x100D6F90` CXiSkeletonActor::vf033

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 33 (+0x84), `XiSkeletonActor2` slot 33 (+0x84), `CXiSkeletonActor` slot 33 (+0x84), `CXiMannequinActor` slot 33 (+0x84), `XiChocoboActor2` slot 33 (+0x84), `CXiGeomActor` slot 33 (+0x84), `CXiDancerActor` slot 33 (+0x84)

```cpp
void CXiSkeletonActor::vf033()
{
  ;
}
```

## `0x100D6FA0` CXiSkeletonActor::vf034

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 34 (+0x88), `XiSkeletonActor2` slot 34 (+0x88), `CXiSkeletonActor` slot 34 (+0x88), `CXiMannequinActor` slot 34 (+0x88), `XiChocoboActor2` slot 34 (+0x88), `CXiGeomActor` slot 34 (+0x88), `CXiDancerActor` slot 34 (+0x88)

```cpp
void CXiSkeletonActor::vf034()
{
  ;
}
```

## `0x100D6FB0` CXiSkeletonActor::vf035

- **Gate:** none
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 35 (+0x8C), `XiSkeletonActor2` slot 35 (+0x8C), `CXiSkeletonActor` slot 35 (+0x8C), `CXiMannequinActor` slot 35 (+0x8C), `XiChocoboActor2` slot 35 (+0x8C), `CXiGeomActor` slot 35 (+0x8C), `CXiDancerActor` slot 35 (+0x8C)

```cpp
void CXiSkeletonActor::vf035(int a1, int a2)
{
  ;
}
```

## `0x100D6FC0` CXiSkeletonActor::vf036

- **Gate:** none
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 36 (+0x90), `XiSkeletonActor2` slot 36 (+0x90), `CXiSkeletonActor` slot 36 (+0x90), `CXiMannequinActor` slot 36 (+0x90), `XiChocoboActor2` slot 36 (+0x90), `CXiGeomActor` slot 36 (+0x90), `CXiDancerActor` slot 36 (+0x90)

```cpp
void CXiSkeletonActor::vf036(int a1)
{
  ;
}
```

## `0x100D6FD0` CXiSkeletonActor::vf037

- **Gate:** none
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **In vtables** (class_vtables.json): `CXiSklFurnitureActor` slot 37 (+0x94), `XiSkeletonActor2` slot 37 (+0x94), `CXiSkeletonActor` slot 37 (+0x94), `CXiMannequinActor` slot 37 (+0x94), `XiChocoboActor2` slot 37 (+0x94), `CXiGeomActor` slot 37 (+0x94), `CXiDancerActor` slot 37 (+0x94)

```cpp
void CXiSkeletonActor::vf037()
{
  ;
}
```
