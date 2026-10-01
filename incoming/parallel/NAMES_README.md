# FFXiMain graphics tree: placeholders replaced with real names

This is `tree/` from `ffximain-gfx-cpp.zip` with every identified `dword_`/`sub_`/`flt_`/... placeholder and raw offset replaced by its name. **The code is unchanged.**

## Proof the code is unchanged
I built both trees with the repo's own `build.sh` and compared every object file (132):
- the `.text`, `.rdata`, `.data` and `.bss` bytes are identical;
- the relocation offsets and types are identical.

Result: **0 differences**. Renamed identifiers resolve to the same addresses. Named offsets are compile-time constants with the same values, written as `*(self_ + CTsZoneMap_lightSlotNamed / 4)` where the original had `*(self_ + 58668)`. So no expression changed shape and no type changed.

## What was replaced
- **316 placeholders** renamed (`RENAMES.csv` gives each old name, new name and the evidence), plus `g_pCurrentCamera` -> `g_pXiZone`. These cover:
  - the zone renderer (`g_pZoneMap`, Z-bias presets, the zone box buffers)
  - dancer sqgx/sqsn functions (`sqgxTextureAdd`, `sqsnSceneLayerSort`, ...)
  - `XiZone` environment queries
  - vertex buffers, vertex shader handles, state blocks, viewports and matrices
  - 191 fields of `DrawPrimitiveUP` vertex arrays (`g_upVerts_1C90_v0_diffuse`, ...)
  - 16 identity-matrix elements
  - countdowns, counters, constants
  - dancer source modules
  - device-call wrappers named for the call they make (`Dev_SetRenderState_102824C0`)
- **Raw offsets** -> named constants in `include/gfx_fields.hpp`:
  - `CTsZoneMap` fields
  - the zone draw descriptor, mesh, group, batch and cell records
  - `CYyTex` texture slots
  - `sqsnScene` and layer fields
  - `sqgxContext` fields, named by the game's own `sqgxContext_Dump`
- **`CDx` fields** `field_912`..`field_938` -> `supportsP8Textures`, `supportsFullscreenGamma`, `isAtiAdapter`, `supportsTableFog`, `supportsVertexFog`, `supportsRangeFog`, `texPow2Only`, `texSquareOnly`, `maxTextureWidth`, `maxTextureHeight`. Every `g_pCDx->_pad_10[N]` that reached one of them now uses the name.

## What is still a placeholder (333, listed in `UNRESOLVED.csv`)
244 functions and 89 globals still have no established meaning, so they keep their original names rather than invented ones. Some raw offsets on unidentified objects are also untouched, including 31 `_pad_10[N]` that reach `CDx` fields not yet identified. Each unresolved entry lists what the binary does show (writers, values, readers, calls), as a starting point.

Names marked `_Candidate` (`sqgxBlendTrsp_Candidate`, `sqgx_DrawRect_Candidate`) are best matches, not confirmed.
