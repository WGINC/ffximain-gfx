# Batch 044_unit_10073750_ResourceIter: 9 functions of `unit_10073750_ResourceIter`

Name these functions: see `batches/README.md`. Write `data/names/044_unit_10073750_ResourceIter.json`.
3 of them were sampled in a running frame (data/frame_set.json).

## `0x10073A60` fn_10073A60

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 8`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10072030`
- **Called by:** `MeshResourceMng_ResolveTree`, `sub_10073750`

```cpp
XI_API void __stdcall fn_10073A60(int * param_1, undefined4 param_2)
{
  uint32_t uVar1;
  
  orig::CMoResource_RegisterInTypeList_v1(param_1);
  uVar1 = param_1[9] & 0x7f;
  if (((param_2 & 1) != 0) &&
     ((((uVar1 == 0x58 || (uVar1 == 0x3d)) || (uVar1 == 0x20)) ||
      (((uVar1 == 0x5d || (uVar1 == 0x1b)) || (uVar1 == 0x18)))))) {
    ((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x20))))((void *)(param_1));
    *(uint8_t *)((int)param_1 + 0x1a) = *(uint8_t *)((int)param_1 + 0x1a) | 8;
    return;
  }
  if ((((param_2 & 2) != 0) &&
      ((((uVar1 == 10 || (uVar1 == 0x2e)) ||
        ((uVar1 == 0x36 ||
         ((((uVar1 == 0x2a || (uVar1 == 0x29)) || (uVar1 == 0x2b)) ||
          ((uVar1 == 0x1f || (uVar1 == 0x21)))))))) ||
       (((uVar1 == 0x25 || ((uVar1 == 0x39 || (uVar1 == 0x31)))) ||
        (((uVar1 == 0x30 || (((uVar1 == 0x13 || (uVar1 == 0x49)) || (uVar1 == 0x44)))) ||
         (uVar1 == 0x3e)))))))) ||
     (((param_2 & 4) != 0 && ((*(uint8_t *)((int)param_1 + 0x1a) >> 3 & 1) == 0)))) {
    ((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x20))))((void *)(param_1));
    *(uint8_t *)((int)param_1 + 0x1a) = *(uint8_t *)((int)param_1 + 0x1a) | 8;
  }
  return;
}
```

## `0x10073B80` fn_10073B80

- **Gate:** none; **frame set:** static, stack
- **Facts:** `ret 4`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_100720B0`
- **Called by:** `__attribute__`, `sub_10072C90`

```cpp
XI_API undefined4 __stdcall fn_10073B80(int * param_1)
{
  uint32_t uVar1;
  
  orig::FUN_100720b0(param_1);
  uVar1 = ((int (__thiscall *)(void *))((**(code **)(*param_1 + 0x24))))((void *)(param_1));
  return uVar1;
}
```

## `0x10073BA0` fn_10073BA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 12`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1001AC50`, `sub_10073CD0`, `sub_10073E50`, `sub_100D4BC0`, `fn_101ADDC0`
- **Reads globals:** `0x1047D124`

```cpp
XI_API undefined4 __stdcall fn_10073BA0(undefined4 * param_1, int param_2, int param_3)
{
  int iVar1;
  int *piVar2;
  
  if (param_2 == 0) {
    for (piVar2 = *(int **)(*DAT_1047d124 + 0x14); piVar2 != (int *)0x0;
        piVar2 = *(int **)(*piVar2 + 0x2c)) {
      if (*(int *)(*piVar2 + 0x20) == param_3) goto LAB_10073c18;
    }
  }
  else {
    if ((int *)(&DAT_1047cf80)[param_2] == (int *)0x0) {
      iVar1 = 0;
    }
    else {
      iVar1 = *(int *)(&DAT_1047cf80)[param_2];
    }
    if (iVar1 != 0) {
      while( true ) {
        if (param_3 == 0) {
          *param_1 = *(uint32_t *)(iVar1 + 0x14);
          return param_1;
        }
        if (*(int *)(iVar1 + 0x20) == param_3) break;
        if (*(int **)(iVar1 + 0x10) == (int *)0x0) {
          iVar1 = 0;
        }
        else {
          iVar1 = **(int **)(iVar1 + 0x10);
        }
        if (iVar1 == 0) {
          *param_1 = 0;
          return param_1;
        }
      }
      piVar2 = *(int **)(iVar1 + 0x14);
LAB_10073c18:
      *param_1 = piVar2;
      return param_1;
    }
  }
  *param_1 = 0;
  return param_1;
}
```

## `0x10074110` fn_10074110

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10072C30`

```cpp
XI_API uint64_t __thiscall fn_10074110(void * self_, undefined4 * param_1)
{
  uint32_t *puVar1;
  
  puVar1 = *(uint32_t **)((int)self_ + 4);
  if (puVar1 == (uint32_t *)0x0) {
    *param_1 = 0;
    return ((uint64_t)(uint32_t)(param_1));
  }
  *(uint32_t *)((int)self_ + 4) = *puVar1;
  *puVar1 = 0;
  *param_1 = puVar1;
  return ((uint64_t)(((uint64_t)(uint32_t)(puVar1) << 32) | (uint32_t)(param_1)));
}
```

## `0x100741E0` fn_100741E0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 4`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10070FC0`, `ResourceHandle_IsResident`, `CMoResource::ReleaseChain`

```cpp
XI_API undefined4 __thiscall fn_100741E0(void * self_, undefined4 * param_1)
{
  undefined4 *puVar1;
  
  if (((undefined4 *)((int)self_ + 8U) <= param_1) &&
     (param_1 <= (undefined4 *)((int)self_ + 0x40004U))) {
    if (param_1 == (undefined4 *)0x0) {
      puVar1 = (undefined4 *)0x0;
    }
    else {
      puVar1 = (undefined4 *)*param_1;
    }
    if ((undefined4 *)((int)self_ + 8U) <= puVar1) {
      if (param_1 == (undefined4 *)0x0) {
        param_1 = (undefined4 *)0x0;
      }
      else {
        param_1 = (undefined4 *)*param_1;
      }
      if (param_1 <= (undefined4 *)((int)self_ + 0x40004U)) goto LAB_10074218;
    }
    return ((uint32_t)(((uint32_t)((int32_t)((uint32_t)param_1 >> 8)) << 8) | (uint8_t)(1)));
  }
LAB_10074218:
  return (uint32_t)param_1 & 0xffffff00;
}
```

## `0x10074280` fn_10074280

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_10072A70`, `sub_10072FD0`, `sub_100731B0`, `sub_100732F0`
- **Writes globals:** `0x1047D114`

```cpp
XI_API void __cdecl fn_10074280(undefined4 param_1)
{
  DAT_1047d114 = param_1;
  return;
}
```

## `0x10074310` fn_10074310

- **Gate:** none; **frame set:** static
- **Facts:** `ret 16`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10074380`, `fn_100743D0`
- **Called by:** `fn_100446A0`, `sub_10048000`, `fn_10053A00`, `sub_10055B90`, `fn_101671D0`, `fn_1018ABF0`, `fn_1021E3B0`
- **Writes globals:** `0x1047D130`
- **Reads globals:** `0x1047D130`

```cpp
XI_API void __thiscall fn_10074310(void * self_, undefined4 param_1, int param_2, uint32_t param_3, uint32_t param_4)
{
  uint32_t uVar1;
  uint32_t uVar2;
  
  g_resourceFindDepth = g_resourceFindDepth + 1;
  *(uint32_t *)((int)self_ + 4) = param_1;
  *(uint32_t *)self_ = 0;
  *(int *)((int)self_ + 8) = param_2;
  *(uint32_t *)((int)self_ + 0xc) = param_3 & param_4;
  *(uint32_t *)((int)self_ + 0x10) = param_4;
  *(uint32_t *)((int)self_ + 0x18) = 0;
  if ((uint32_t *)(&DAT_1047cf80)[param_2] == (uint32_t *)0x0) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(uint32_t *)(&DAT_1047cf80)[param_2];
  }
  *(uint32_t *)((int)self_ + 0x14) = uVar1;
  uVar2 = orig::FUN_10074380(self_);
  if ((char)uVar2 == '\0') {
    orig::FUN_100743d0(self_);
  }
  return;
}
```

## `0x10074380` fn_10074380

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `ResourceIter_FindFirst`, `fn_10074310`

```cpp
XI_API uint32_t __thiscall fn_10074380(void * self_)
{
  uint32_t *puVar1;
  uint32_t uVar2;
  
  uVar2 = *(uint32_t *)((int)self_ + 0x14);
  if (uVar2 != 0) {
    if (*(uint32_t *)((int)self_ + 0xc) == 0) {
      puVar1 = (uint32_t *)(uVar2 + 0x24);
      uVar2 = *(uint32_t *)((int)self_ + 8);
      if ((*puVar1 & 0x7f) == uVar2) {
        return ((uint32_t)(((uint32_t)((int32_t)(uVar2 >> 8)) << 8) | (uint8_t)(1)));
      }
    }
    else if (((*(uint32_t *)(uVar2 + 0x24) & 0x7f) == *(uint32_t *)((int)self_ + 8)) &&
            (uVar2 = *(uint32_t *)(uVar2 + 0x20) & *(uint32_t *)((int)self_ + 0x10),
            uVar2 == *(uint32_t *)((int)self_ + 0xc))) {
      return ((uint32_t)(((uint32_t)((int32_t)(uVar2 >> 8)) << 8) | (uint8_t)(1)));
    }
  }
  return uVar2 & 0xffffff00;
}
```

## `0x100743D0` fn_100743D0

- **Gate:** none; **frame set:** static, self, stack
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `CMoResource::GetParent` x2
- **Called by:** `ResourceIter_FindFirst`, `fn_10074310`, `sub_100745B0`, `ResourceIter_FindNext`
- **Reads globals:** `0x1047D130`

```cpp
XI_API void __thiscall fn_100743D0(void * self_)
{
  int *piVar1;
  int iVar2;
  void *pvVar3;
  int iVar4;
  
  iVar2 = g_resourceFindDepth;
  iVar4 = *(int *)((int)self_ + 0x14);
  if (*(int *)self_ == 0) {
    if (iVar4 != 0) {
      piVar1 = *(int **)(iVar4 + 0x10);
      if (*(uint32_t *)((int)self_ + 0xc) != 0) {
        if (piVar1 == (int *)0x0) {
          iVar4 = 0;
        }
        else {
          iVar4 = *piVar1;
        }
        *(int *)((int)self_ + 0x14) = iVar4;
        if (iVar4 != 0) {
          do {
            if ((*(uint32_t *)(*(int *)((int)self_ + 0x14) + 0x20) & *(uint32_t *)((int)self_ + 0x10)) ==
                *(uint32_t *)((int)self_ + 0xc)) {
              return;
            }
            piVar1 = *(int **)(*(int *)((int)self_ + 0x14) + 0x10);
            if (piVar1 == (int *)0x0) {
              iVar4 = 0;
            }
            else {
              iVar4 = *piVar1;
            }
            *(int *)((int)self_ + 0x14) = iVar4;
          } while (iVar4 != 0);
        }
        *(uint32_t *)((int)self_ + 0x14) = 0;
        return;
      }
      if (piVar1 != (int *)0x0) {
        *(int *)((int)self_ + 0x14) = *piVar1;
        return;
      }
      *(uint32_t *)((int)self_ + 0x14) = 0;
    }
    return;
  }
  if (iVar4 != 0) {
    piVar1 = *(int **)(iVar4 + 8);
    if (*(int *)((int)self_ + 0xc) == 0) {
      if (piVar1 == (int *)0x0) {
        iVar4 = 0;
      }
      else {
        iVar4 = *piVar1;
      }
      *(int *)((int)self_ + 0x14) = iVar4;
      while (iVar4 != 0) {
        iVar4 = *(int *)((int)self_ + 0x14);
        if ((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == 1) {
          if (*(int *)(iVar4 + 0x38) != iVar2) {
            *(int *)(iVar4 + 0x38) = iVar2;
            *(int *)((int)self_ + 0x18) = *(int *)((int)self_ + 0x18) + 1;
            goto LAB_100744f8;
          }
          if (*(uint32_t **)(iVar4 + 0x3c) == (uint32_t *)0x0) {
            *(uint32_t *)((int)self_ + 0x14) = 0;
          }
          else {
            *(uint32_t *)((int)self_ + 0x14) = **(uint32_t **)(iVar4 + 0x3c);
          }
        }
        else {
LAB_100744f8:
          if ((*(uint8_t *)((int)*(void **)((int)self_ + 0x14) + 0x24) & 0x7f) == 0) {
            pvVar3 = orig::CMoResource_GetParent_v1(*(void **)((int)self_ + 0x14));
            *(int *)((int)pvVar3 + 0x38) = iVar2;
            iVar4 = *(int *)((int)self_ + 0x18) + -1;
            *(int *)((int)self_ + 0x18) = iVar4;
            if (iVar4 < 0) break;
          }
        }
        if ((*(uint32_t *)(*(int *)((int)self_ + 0x14) + 0x24) & 0x7f) == *(uint32_t *)((int)self_ + 8)) {
          return;
        }
        piVar1 = *(int **)(*(int *)((int)self_ + 0x14) + 8);
        if (piVar1 == (int *)0x0) {
          iVar4 = 0;
        }
        else {
          iVar4 = *piVar1;
        }
        *(int *)((int)self_ + 0x14) = iVar4;
      }
    }
    else {
      if (piVar1 == (int *)0x0) {
        iVar4 = 0;
      }
      else {
        iVar4 = *piVar1;
      }
      *(int *)((int)self_ + 0x14) = iVar4;
      if (iVar4 != 0) {
        do {
          iVar4 = *(int *)((int)self_ + 0x14);
          if ((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == 1) {
            if (*(int *)(iVar4 + 0x38) != iVar2) {
              *(int *)(iVar4 + 0x38) = iVar2;
              *(int *)((int)self_ + 0x18) = *(int *)((int)self_ + 0x18) + 1;
              goto LAB_1007444e;
            }
            if (*(uint32_t **)(iVar4 + 0x3c) == (uint32_t *)0x0) {
              *(uint32_t *)((int)self_ + 0x14) = 0;
            }
            else {
              *(uint32_t *)((int)self_ + 0x14) = **(uint32_t **)(iVar4 + 0x3c);
            }
          }
          else {
LAB_1007444e:
            if ((*(uint8_t *)((int)*(void **)((int)self_ + 0x14) + 0x24) & 0x7f) == 0) {
              pvVar3 = orig::CMoResource_GetParent_v1(*(void **)((int)self_ + 0x14));
              *(int *)((int)pvVar3 + 0x38) = iVar2;
              iVar4 = *(int *)((int)self_ + 0x18) + -1;
              *(int *)((int)self_ + 0x18) = iVar4;
              if (iVar4 < 0) break;
            }
          }
          iVar4 = *(int *)((int)self_ + 0x14);
          if (((*(uint32_t *)(iVar4 + 0x24) & 0x7f) == *(uint32_t *)((int)self_ + 8)) &&
             ((*(uint32_t *)(iVar4 + 0x20) & *(uint32_t *)((int)self_ + 0x10)) == *(uint32_t *)((int)self_ + 0xc))
             ) {
            return;
          }
          if (*(int **)(iVar4 + 8) == (int *)0x0) {
            iVar4 = 0;
          }
          else {
            iVar4 = **(int **)(iVar4 + 8);
          }
          *(int *)((int)self_ + 0x14) = iVar4;
          if (iVar4 == 0) {
            *(uint32_t *)((int)self_ + 0x14) = 0;
            return;
          }
        } while( true );
      }
    }
  }
// ... 3 more lines (tree/src/unit_10073750_ResourceIter.cpp)
```
