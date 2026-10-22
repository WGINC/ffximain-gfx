# Batch r2_155_unit_1003AAB0_TexMgr: 1 functions of `unit_1003AAB0_TexMgr`

Name these functions: see `batches/README.md`. Write `data/names/r2_155_unit_1003AAB0_TexMgr.json`.
0 of them were sampled in a running frame (data/frame_set.json).

## `0x1003AAB0` fn_1003AAB0

- **Gate:** none; **frame set:** static
- **Facts:** `ret 0`; entry ECX read; entry EDX never read (purges, ret_width, ecx_use, edx_use)
- **Called by:** `sub_1003AAC0`, `TexMgr_GetTailLink` (medium), `sub_1003AB10`, `TexMgr_UploadPump`, `sub_1003AC40`, `CYyTexMng::FindByName`, `sub_1003ACD0`, `sub_1003AD40`, `sub_1003ADC0`, `sub_1003AE50`, `TexMgr_FindByName`, `CYyTexMng::FindByNameInResource`, `sub_10075520`, `sub_10075690`
- **Named before** (a naming session's reading, low): `TexMgr_GetField8`; name it again from this dossier, or leave it out to keep that name
- **From the frame root** (shortest chain of direct calls): `CApp::RunMainLoop` → `App_RenderFrameMaskedFPU` → `Frame_Render` → `TexMgr_UploadPumpEntry` → `TexMgr_UploadPump` → `TexMgr_GetField8`
- **Named callers' callers:** `TexRes_LoadBm2`, `Tex_DrawNamedTexturePreview`, `CYyModelDt_PrepareRenderState`, `CYyModelBase::DrawLit`, `CYyModelDt_PrepareRenderStateV2`, `TexObj_Release`, `TexMgr_UploadPumpEntry`, `TexMgr_Enqueue`, `Mo_InitSharedRenderResources`, `Font_BindGlyphTexture`, `Cell_ResolveTextureUV`, `Cell_DrawAt` and 5 more
- **How its callers call it** (their verified code):
  - in `TexMgr_GetTailLink` (medium): `piVar2 = (int *)TexMgr_GetField8(self_);`
  - in `TexMgr_UploadPump`: `piVar2 = (int *)TexMgr_GetField8(self_);`
  - in `CYyTexMng::FindByName`: `piVar2 = (int *)TexMgr_GetField8(self);`
- **Neighbours' readings** (naming sessions' summaries, not facts):
  - `TexMgr_GetTailLink` (medium): Returns the address of the last link in the texture list, for appending a texture.

```cpp
XI_API int __thiscall TexMgr_GetField8(void * self_)
{
  return (int)self_ + 8;
}
```
