# Batch r3_037_sqsdAnisoCommon: 1 functions of `sqsdAnisoCommon`

Name these functions: see `batches/README.md`. Write `data/names/r3_037_sqsdAnisoCommon.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x102778F0` fn_102778F0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX never read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Calls:** `sqDebugPrintf` (high) x4
- **Called by:** `sub_1024DA10`, `sqsdShader_ParseScript`
- **Strings it refers to** (DLL bytes): `C:\dev\dancer\modules\sqShader\src\sqsdAnisoCommon.c`, `Error in %s, line %d: `
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `Dancer_WorldBegin` → `StAvatar_Update` → `sub_1024D850` → `sub_1024DA10` → `fn_102778F0`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `sqDebugPrintf` (high): 

```cpp
XI_API void __cdecl fn_102778F0(int param_1, int param_2)
{
  if (param_2 < 1) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqShader_s_103b37b4,0x3e);
    orig::sqDebugPrintf_v1(s_Number_of_triangles___d__must_be_103b3758,param_2);
    param_2 = 1;
  }
  else if (4 < param_2) {
    orig::sqDebugPrintf_v1(s_Error_in__s__line__d__103b379c,s_C__dev_dancer_modules_sqShader_s_103b37b4,0x41);
    orig::sqDebugPrintf_v1(s_sqsdAnisoSetNumDrawTris__Number_o_103b36f8,param_2);
    param_2 = 4;
  }
  *(uint32_t *)(param_1 + 0x68) = param_2 * 2 - 2U | *(uint32_t *)(param_1 + 0x68) & 0xfffffff9;
  return;
}
```
