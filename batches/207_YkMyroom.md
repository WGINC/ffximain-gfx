# Batch 207_YkMyroom: 1 functions of `YkMyroom`

Name these functions: see `batches/README.md`. Write `data/names/207_YkMyroom.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x101D2F00` fn_101D2F00

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `fn_1013A320`, `fn_10150CC0`, `fn_101594D0`, `__attribute__` x3, `fn_101D30B0`, `fn_101D31B0`, `fn_101D3DC0`, `operator_delete`
- **Called by:** `App_Destructor`, `sub_10014390`, `sub_10014AD0`, `fn_10093B70`
- **Writes globals:** `0x106631DC`, `0x106631E0`, `0x106631E4`
- **Reads globals:** `0x105781F8`, `0x1057847C`, `0x1057876C`, `0x106631DC`, `0x106631E0`, `0x106631E4`

```cpp
XI_API void __thiscall fn_101D2F00(void * self_)
{
  *(uint8_t ***)self_ = &PTR_YkMyroom_GetRuntimeClass_103384a4;
  orig::FUN_101d3dc0(self_);
  orig::FUN_101d30b0(self_);
  orig::FUN_101594d0(g_pTargetMng,0);
  if (DAT_106631dc != (undefined4 *)0x0) {
    orig::FUN_1015e8a0(&g_menuMng_as_v0,DAT_106631dc[2],DAT_106631dc[2]);
    if (DAT_106631dc != (undefined4 *)0x0) {
      ((int (__thiscall *)(void *, XI_ANY))((**(code **)*DAT_106631dc)))((void *)(DAT_106631dc),1);
      DAT_106631dc = (undefined4 *)0x0;
    }
  }
  if (DAT_106631e0 != (undefined4 *)0x0) {
    orig::FUN_1015e8a0(&g_menuMng_as_v0,DAT_106631e0[2],DAT_106631e0[2]);
    if (DAT_106631e0 != (undefined4 *)0x0) {
      ((int (__thiscall *)(void *, XI_ANY))((**(code **)*DAT_106631e0)))((void *)(DAT_106631e0),1);
      DAT_106631e0 = (undefined4 *)0x0;
    }
  }
  if (DAT_106631e4 != (undefined4 *)0x0) {
    orig::FUN_1015e8a0(&g_menuMng_as_v0,DAT_106631e4[2],DAT_106631e4[2]);
    if (DAT_106631e4 != (undefined4 *)0x0) {
      ((int (__thiscall *)(void *, XI_ANY))((**(code **)*DAT_106631e4)))((void *)(DAT_106631e4),1);
      DAT_106631e4 = (undefined4 *)0x0;
    }
  }
  orig::operator_delete_10311BB0_v1(*(LPVOID *)((int)self_ + 0x14));
  orig::FUN_101d31b0(self_);
  orig::thunk_FUN_10150ce0(g_pTkTitle,0,0);
  orig::FUN_1013a320(g_pTkHelp,0,0);
  return;
}
```
