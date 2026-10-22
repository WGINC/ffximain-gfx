# Batch 200_StChannel: 5 functions of `StChannel`

Name these functions: see `batches/README.md`. Write `data/names/200_StChannel.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10251070` fn_10251070

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_10017D20`, `fn_1026EF00`, `sub_1026EFB0`, `sub_1026F080`, `fn_1026F4F0`, `fn_1026F9A0`, `sub_10311C2C`
- **Called by:** `fn_1024D6B0`, `sub_1024DA10`

```cpp
XI_API uint32_t __cdecl fn_10251070(uint32_t * param_1, int param_2, float param_3)
{
  long double fVar1;
  uint32_t *puVar2;
  uint32_t *puVar3;
  uint32_t uVar4;
  long double fVar5;
  int64_t lVar6;
  
  puVar2 = param_1;
  if (param_1[6] == 0) {
    orig::nullsub_13_v1(s_____Error__StChannelItemRegister_10389830);
  }
  else {
    uVar4 = orig::FUN_1026ef00(param_1[2],param_1[6]);
    puVar2[3] = uVar4;
    if (-1 < (int)uVar4) {
      *puVar2 = *puVar2 | 0x10;
      orig::FUN_1026f9a0(uVar4,0x200,1);
      if (param_2 < 0) {
        orig::FUN_1026f080(puVar2[2],puVar2[3],0);
      }
      else {
        orig::FUN_1026efb0(puVar2[3],param_2);
      }
      orig::FUN_1026f4f0((int *)&param_1,(int *)0x0);
      puVar3 = param_1;
      uVar4 = puVar2[3];
      param_1[uVar4 * 0x30 + 0x28] = 0x3f800000;
      if ((!NAN((long double)param_3) && !NAN((long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x29]))) &&
          (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x29]) <= (long double)param_3) {
        fVar5 = (long double)param_3;
        fVar1 = (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x2a]);
        if (fVar5 <= fVar1) {
          param_1[uVar4 * 0x30 + 0x2b] = xi_castbits<uint32_t>(param_3);
          lVar6 = XI_CALL_ST0(0x10311C2Cu)((long double)param_3 / (long double)xi_castbits<float>(param_1[uVar4 * 0x30 + 0x2c]));
          puVar3[uVar4 * 0x30 + 0x26] = (int)lVar6 * 2;
        }
      }
      return puVar2[3];
    }
  }
  return 0xffffffff;
}
```

## `0x10251160` fn_10251160

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1024D400`

```cpp
XI_API undefined4 __cdecl fn_10251160(uint32_t * param_1)
{
  param_1[3] = 0xffffffff;
  *param_1 = *param_1 & 0xffffffed;
  return 0;
}
```

## `0x10251180` fn_10251180

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_101C4A30`
- **Called by:** `StAvatar_StopAndFreeBuffers`
- **Reads globals:** `0x10662DCC`

```cpp
XI_API undefined4 __cdecl fn_10251180(uint32_t * param_1)
{
  uint32_t uVar1;
  
  if (((*param_1 & 1) != 0) && (-1 < (int)param_1[1])) {
    uVar1 = orig::FUN_101c4a30(g_pFileMng_as_v1,param_1[1]);
    if ((char)uVar1 != '\0') {
      *param_1 = *param_1 | 0x20;
    }
  }
  return 0;
}
```

## `0x102511B0` fn_102511B0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026F4F0`
- **Called by:** `fn_1024FC10`

```cpp
XI_API undefined4 __cdecl fn_102511B0(uint8_t * param_1, float param_2)
{
  int iVar1;
  uint8_t *pbVar2;
  char xi_frame[4] __attribute__((aligned(4)));
  int  &local_4 = *reinterpret_cast<int  *>(xi_frame + 0);
  
  pbVar2 = param_1;
  if ((*param_1 & 1) != 0) {
    orig::FUN_1026f4f0(&local_4,(int *)&param_1);
    iVar1 = *(int *)(pbVar2 + 0xc);
    if ((-1 < iVar1) && (iVar1 < (int)param_1)) {
      *(float *)(iVar1 * 0xc0 + 0x9c + local_4) = (float)(long double)param_2;
    }
  }
  return 0;
}
```

## `0x10251200` fn_10251200

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1026F4F0`, `fn_1027DD70`, `fn_102857D0`, `sub_102857E0` x2, `fn_102857F0`
- **Called by:** `fn_1024DB60`

```cpp
XI_API undefined4 __cdecl fn_10251200(uint8_t * param_1, undefined4 param_2, int param_3)
{
  uint8_t *pbVar1;
  uint32_t uVar2;
  uint32_t *puVar3;
  
  pbVar1 = param_1;
  if ((param_1 != (uint8_t *)0x0) && ((*param_1 & 0x10) != 0)) {
    orig::FUN_1026f4f0((int *)&param_1,(int *)0x0);
    puVar3 = (uint32_t *)(param_1 + *(int *)(pbVar1 + 0xc) * 0xc0);
    if ((*puVar3 & 2) == 0) {
      *puVar3 = *puVar3 | 2;
      orig::FUN_1027dd70((uint8_t *)puVar3,param_2);
      if ((param_3 == 0) && (puVar3[0x2f] != 0)) {
        uVar2 = orig::FUN_102857d0(puVar3[0x2f]);
        if ((char)uVar2 == '\0') {
          orig::FUN_102857e0(puVar3[0x2f],1,uVar2);
          uVar2 = orig::FUN_102857f0(puVar3[0x2f]);
          orig::FUN_102857e0(puVar3[0x2f],0,uVar2);
        }
      }
      *puVar3 = *puVar3 & 0xfffffffd;
    }
    return 0;
  }
  return 0xffffffff;
}
```
