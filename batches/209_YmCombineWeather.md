# Batch 209_YmCombineWeather: 1 functions of `YmCombineWeather`

Name these functions: see `batches/README.md`. Write `data/names/209_YmCombineWeather.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1018ABF0` fn_1018ABF0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10056EB0`, `sub_100742A0`, `ResourceIter_FindFirst` x3, `fn_10074310`, `ResourceIter_FindNext` x6, `fn_101880C0`, `sub_1018ABB0` x2, `fn_1018AD50`, `fn_1018AD60`, `fn_1018AD80`, `fn_1018ADA0`
- **Called by:** `sub_101874F0`, `fn_10188A10`, `fn_1018ADE0`
- **Reads globals:** `0x1047D128`

```cpp
XI_API void __thiscall fn_1018ABF0(void * self_)
{
  uint8_t bVar1;
  int iVar2;
  void *pvVar3;
  char xi_frame[28] __attribute__((aligned(4)));
  uint8_t  (&local_1c)[28] = *reinterpret_cast<uint8_t  (*)[28]>(xi_frame + 0);
  
  *(uint8_t ***)self_ = &PTR_YmCombineWeather_GetRuntimeClass_10336814;
  orig::FUN_100742a0(local_1c);
  if (*(int *)(*(int *)((int)self_ + 4) + 0x20) == *(int *)((int)self_ + 0x2c)) {
    iVar2 = orig::FUN_1018ad50(self_);
    *(uint32_t *)(iVar2 + 0x20) = 0;
  }
  bVar1 = false;
  orig::ResourceIter_FindFirst_v1(local_1c,*(int *)(*(int *)((int)self_ + 4) + 0x14),1,0,0xffffffff);
  iVar2 = orig::ResourceIter_FindNext_v1(local_1c,0);
  do {
    if (iVar2 == 0) {
LAB_1018ac60:
      do {
        orig::FUN_10074310(local_1c,g_pMeshResourceMng_as_v0,5,0,0xffffffff);
        do {
          pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
          if (pvVar3 == (void *)0x0) {
            do {
              orig::ResourceIter_FindFirst_v1(local_1c,*(int *)((int)self_ + 0xc),5,0,0xffffffff);
              do {
                pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                if (pvVar3 == (void *)0x0) {
                  if (*(int *)((int)*(void **)((int)self_ + 4) + 0x18) == *(int *)((int)self_ + 0x20))
                  {
                    orig::FUN_101880c0(*(void **)((int)self_ + 4),(uint32_t *)0x0);
                  }
                  orig::FUN_1018ada0(self_);
                  pvVar3 = orig::FUN_1018ad80(self_);
                  if (pvVar3 == (void *)0x0) {
                    orig::FUN_1018ad60(self_,*(uint32_t *)((int)self_ + 8));
                  }
                  else {
                    *(uint32_t *)((int)pvVar3 + 8) = *(uint32_t *)((int)self_ + 8);
                  }
                  orig::ResourceIter_FindFirst_v1(local_1c,*(int *)((int)self_ + 0xc),7,0,0xffffffff);
                  pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                  while (pvVar3 != (void *)0x0) {
                    orig::FUN_10056eb0(pvVar3);
                    pvVar3 = (void *)orig::ResourceIter_FindNext_v1(local_1c,0);
                  }
                  return;
                }
              } while ((*(void **)((int)pvVar3 + 100) != self_) ||
                      (*(uint32_t *)((int)pvVar3 + 100) = 0, !bVar1));
              orig::FUN_1018abb0(pvVar3,pvVar3);
            } while( true );
          }
        } while ((*(void **)((int)pvVar3 + 100) != self_) ||
                (*(uint32_t *)((int)pvVar3 + 100) = 0, !bVar1));
        orig::FUN_1018abb0(pvVar3,pvVar3);
      } while( true );
    }
    if (iVar2 == *(int *)((int)self_ + 0xc)) {
      bVar1 = true;
      goto LAB_1018ac60;
    }
    iVar2 = orig::ResourceIter_FindNext_v1(local_1c,0);
  } while( true );
}
```
