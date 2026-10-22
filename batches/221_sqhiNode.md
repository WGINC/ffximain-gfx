# Batch 221_sqhiNode: 5 functions of `sqhiNode`

Name these functions: see `batches/README.md`. Write `data/names/221_sqhiNode.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x10291B70` fn_10291B70

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sub_10275D80`, `fn_10291BA0`
- **Called by:** `sub_10288FF0`, `sub_1029FFF0`

```cpp
XI_API undefined4 * __cdecl fn_10291B70(void)
{
  undefined4 *puVar1;
  
  puVar1 = orig::FUN_10275d80(1,0xf8,s_C__dev_dancer_modules_sqHierarch_103baf34,0x1a);
  orig::FUN_10291ba0(puVar1);
  return puVar1;
}
```

## `0x10291BA0` fn_10291BA0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1027EB00`, `sub_1027EB10`, `sub_1027EBD0`
- **Called by:** `fn_1027C4C0`, `fn_10291B70`, `sub_10291E90`

```cpp
XI_API void __cdecl fn_10291BA0(undefined4 * param_1)
{
  *param_1 = 0xd0001;
  param_1[2] = &LAB_10291c10;
  param_1[3] = XI_CODE(FUN_10291c20);
  param_1[4] = XI_CODE(FUN_10291c60);
  *(uint8_t *)(param_1 + 5) = 0xff;
  param_1[6] = XI_CODE(FUN_10291fe0);
  param_1[7] = 0xffffffff;
  param_1[8] = 0;
  param_1[0x19] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0;
  orig::FUN_1027ebd0(param_1 + 9);
  *(uint8_t *)(param_1 + 0x1d) = 0xff;
  orig::FUN_1027eb00_v1(param_1 + 0x1e);
  orig::M4fSet_v1(param_1 + 0x2e,param_1 + 9);
  return;
}
```

## `0x10291C50` fn_10291C50

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `fn_1027C4C0`, `sub_10291C20`

```cpp
XI_API void __cdecl fn_10291C50(void)
{
  return;
}
```

## `0x10291F00` fn_10291F00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_10288FF0`, `sub_1029FFF0`

```cpp
XI_API void __cdecl fn_10291F00(int param_1, undefined4 param_2)
{
  if (param_1 != 0) {
    *(uint32_t *)(param_1 + 100) = param_2;
  }
  return;
}
```

## `0x10291F10` fn_10291F10

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1027B7A0`, `sub_1027BA00`, `sub_10288FF0`, `sub_1029AF20`, `sub_1029FFF0`

```cpp
XI_API void __cdecl fn_10291F10(int param_1, int param_2)
{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = *(int *)(param_1 + 0x6c);
  iVar2 = *(int *)(param_1 + 0x6c);
  while (iVar1 = iVar3, iVar1 != 0) {
    iVar2 = iVar1;
    iVar3 = *(int *)(iVar1 + 0x70);
  }
  if (iVar2 != 0) {
    *(int *)(iVar2 + 0x70) = param_2;
    *(int *)(param_2 + 0x68) = param_1;
    return;
  }
  *(int *)(param_1 + 0x6c) = param_2;
  *(int *)(param_2 + 0x68) = param_1;
  return;
}
```
