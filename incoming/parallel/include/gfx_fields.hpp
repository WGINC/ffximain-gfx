// gfx_fields.hpp -- byte offsets of identified fields, so raw offsets in the decompiled bodies can be
// written by name without changing any expression: `*(self_ + 58668)` on an int* becomes
// `*(self_ + CTsZoneMap_lightSlotNamed / 4)` -- the same constant, so the same code.
// Evidence for every offset is in ffximain-dword-findings.md (FFXiMain.dll build 2026-09-03).
#pragma once

// ---- CTsZoneMap: zone (world geometry) renderer, g_pZoneMap; 0x395B4 bytes, ctor 0x10177D00 ----
enum : int {
  CTsZoneMap_elemA               = 0x60,     // 2 elements of 0x78 bytes, sub-object at +0x44 of each
  CTsZoneMap_elemA_stride        = 0x78,
  CTsZoneMap_elemA_sub           = 0x44,     // ctor 0x1016FC40 / dtor 0x1016FC80
  CTsZoneMap_subB                = 0x178,    // ctor 0x10167800 / dtor 0x101678D0
  CTsZoneMap_subC                = 0x34330,  // ctor 0x101718E0 / dtor 0x10171920
  CTsZoneMap_overlayTex          = 0x343E4,  // CYyTex* of the multiplicative overlay quad
  CTsZoneMap_subD                = 0x343E8,  // 0x40-byte sub-object, ctor 0x10027990
  CTsZoneMap_subE                = 0x34428,  // ctor 0x10167440 / dtor 0x101674B0
  CTsZoneMap_stateBlock          = 0x38D94,  // D3D state block handle
  CTsZoneMap_cullCW              = 0x390FC,  // one-sided batches: nonzero -> D3DCULL_CW, else CCW
  CTsZoneMap_field_39118         = 0x39118,  // reset to 0 at draw start (meaning not identified)
  CTsZoneMap_curTwoSided         = 0x39120,  // cached two-sided state of the last batch
  CTsZoneMap_field_39138         = 0x39138,  // reset to -1 at draw start (meaning not identified)
  CTsZoneMap_material            = 0x3913C,  // D3DMATERIAL8
  CTsZoneMap_vsHandleA           = 0x39454,  // vertex shader from g_zoneVSDeclA
  CTsZoneMap_vsHandleB           = 0x39458,  // vertex shader from g_zoneVSDeclB
  CTsZoneMap_vsHandles           = 0x3946C,  // 5 more vertex shader handles
  CTsZoneMap_psHandle            = 0x39494,  // pixel shader handle
  CTsZoneMap_useTwoDirLights     = 0x394A4,  // nonzero: two zone-environment directional lights
  CTsZoneMap_lightSlotDir0       = 0x394A8,  // D3D light index, directional light 0
  CTsZoneMap_lightSlotDir1       = 0x394AC,  // D3D light index, directional light 1
  CTsZoneMap_lightSlotNamed      = 0x394B0,  // D3D light index of zone-table light 'l_01'
  CTsZoneMap_field_3954C         = 0x3954C,  // float, set to 1.0f (meaning not identified)
  CTsZoneMap_overlayUseProjection= 0x39598,  // byte: overlay uses saved projection, else Z-bias
  CTsZoneMap_subF                = 0x3959C,  // ctor 0x10054FC0 / dtor 0x10055000
};

// ---- zone mesh draw data read by fn_101844C0 ----
enum : int {
  ZoneDrawDesc_mesh       = 0x18,   // ZoneMesh*
  ZoneDrawDesc_world      = 0x40,   // D3DMATRIX
  ZoneDrawDesc_cullCW     = 0xD4,   // int16: nonzero -> initial cull CW, else CCW

  ZoneMesh_vertexShader   = 0x20,   // FVF passed to SetVertexShader
  ZoneMesh_stride         = 0x24,   // SetStreamSource stride
  ZoneMesh_primType       = 0x2C,   // D3DPRIMITIVETYPE
  ZoneMesh_groupCount     = 0x44,
  ZoneMesh_groups         = 0x60,   // ZoneMeshGroup[groupCount]

  ZoneMeshGroup_batchCount= 0x00,
  ZoneMeshGroup_batches   = 0x04,   // ZoneMeshBatch[batchCount]
  ZoneMeshGroup_size      = 0x24,

  ZoneMeshBatch_tex       = 0x04,   // CYyTex*
  ZoneMeshBatch_primCount = 0x18,
  ZoneMeshBatch_alphaBlend= 0x20,   // int16 -> D3DRS_ALPHABLENDENABLE
  ZoneMeshBatch_twoSided  = 0x24,   // int16: nonzero -> D3DCULL_NONE
  ZoneMeshBatch_vb        = 0x28,   // IDirect3DVertexBuffer8*
  ZoneMeshBatch_size      = 0xC0,

  CYyTex_pTexture         = 0x40,   // released in CYyTex dtor 0x10039970; bound first
  CYyTex_pTextureAlt      = 0x44,   // released in the same dtor; bound if pTexture is NULL

  ZoneLight_light         = 0x08,   // D3DLIGHT8 inside a zone light table entry (0x70 bytes)
  ZoneLightTag_l_01       = 825253740,  // 0x31305F6C = 'l_01' as stored in memory

  ZoneCellList_cells      = 0x3C,   // ZoneCell[cellCount], 0x1AC bytes each
  ZoneCellList_cellCount  = 0x40,
  ZoneCellList_drawArg    = 0x64,   // passed through to ZoneMap_DrawMeshById
  ZoneCell_meshIds        = 0x198,  // int32 ids
  ZoneCell_meshIdCount    = 0x1A9,  // byte
  ZoneCell_size           = 0x1AC,
};

// ---- sqsnScene (dancer): names printed by sqsnScenePrint 0x1028E3B0 ----
enum : int {
  sqsnScene_numCameras  = 0x14,
  sqsnScene_cameras     = 0x18,   // 12-byte entries {sqrdCamera* obj, view matrix}
  sqsnScene_idCamera    = 0x1C,
  sqsnScene_shadowOn    = 0x2C,
  sqsnScene_numShadows  = 0x34,
  sqsnScene_fadeValue   = 0x38,   // float (not printed; PS2 sqsnSceneSetFadeValue)
  sqsnScene_numLayers   = 0x44,
  sqsnScene_layers      = 0x48,   // 44-byte layer records
  sqsnSceneCamera_size  = 12,
  sqsnSceneCamera_mat   = 4,

  sqsnLayer_snapShot    = 0x00,   // float (setter 0x1028F460; PS2 name candidate SetLayerSnapShot)
  sqsnLayer_fadeValue   = 0x14,   // float (sqsnSceneSetLayerFadeValue)
  sqsnLayer_visible     = 0x18,   // byte  (sqsnSceneSetLayerVisible)
  sqsnLayer_sort        = 0x19,   // byte  (sqsnSceneSetLayerSort)
  sqsnLayer_preDraw     = 0x1C,   // callback
  sqsnLayer_postDraw    = 0x20,   // callback
  sqsnLayer_size        = 0x2C,
};

// ---- sqgxContext (dancer): names printed by sqgxContext_Dump 0x102768E0 ----
enum : int {
  sqgxContext_matCam               = 0x188,
  sqgxContext_matObj               = 0x1C8,
  sqgxContext_invMatCam_PS2layout  = 0x208, // not printed; position of the PS2 field
  sqgxContext_matOL                = 0x248,
  sqgxContext_camO                 = 0x288,
  sqgxContext_displayX             = 0x2D4,
  sqgxContext_displayY             = 0x2D8,
  sqgxContext_pass                 = 0x2E8,
  sqgxContext_BGColor              = 0x778,  // 4 floats
  sqgxContext_revNegScale          = 0x754,
  sqgxContext_displayBuf           = 0x898,
  sqgxContext_drawBuf              = 0x89C,
  sqgxContext_zBuf                 = 0x8A0,
  sqgxContext_defaultDisplayBuf    = 0x8A4,
  sqgxContext_defaultDrawBuf       = 0x8A8,
  sqgxContext_defaultZBuf          = 0x8AC,
  sqgxContext_zTest_PS2layout      = 0x8E8, // not printed; position of the PS2 field
  sqgxContext_shader               = 0xA20,
  sqgxContext_vtx                  = 0xA30,
};
