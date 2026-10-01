# Organization of the former `gfx_free_*` functions

The tree's 270 free functions (functions with no proven class) used to sit in files cut by address: `gfx_free_NNN.cpp` held whatever lay in one 64 KB slice of FFXiMain.dll.

This version moves each one into the source unit it came from, **where the binary shows it**. The linker keeps each source file's functions together, so a function belongs to a unit when either:
- it is itself a known function of that unit (an assert from that source file, a DancingMad/IDB/PS2 name, or a constructor/destructor storing that class's vtable); or
- the nearest known functions on **both** sides are from that unit **and** both are within 4 KB.

Vtable slots were not used as evidence: subclass vtables contain inherited methods that live in the base class's file.

## Code unchanged
Both trees were built with `-ffunction-sections` and every function was compared individually. All **504** compiled functions are identical before and after: the same bytes, and relocations with the same offsets, types and targets. None is missing or added. The normal `build.sh` builds all files.

## Moves (56 functions into 28 units)

### `CDx.cpp`
- `0x10009720`: name CDx_Present

### `CMoCamera.cpp`
- `0x10071980`: name CMoCamera_GetSharedInstance

### `CMoD3mSpecularElem.cpp`
- `0x100410e0`: between 0x10041020 (tree member of CMoD3mSpecularElem) and 0x100410f0 (tree member of CMoD3mSpecularElem), both within 4 KB

### `CMoDisintegrateProgElem.cpp`
- `0x10053130`: between 0x100530f0 (stores CMoDisintegrateProgElem_vtbl into this (ctor/dtor)) and 0x10053150 (tree member of CMoDisintegrateProgElem), both within 4 KB

### `CMoDistModelElem.cpp`
- `0x10052f40`: between 0x10052f20 (stores CMoDistModelElem_vtbl into this (ctor/dtor)) and 0x10052f50 (tree member of CMoDistModelElem), both within 4 KB

### `CMoDistMorphElem.cpp`
- `0x10052e40`: between 0x10052e20 (stores CMoDistMorphElem_vtbl into this (ctor/dtor)) and 0x10052e50 (tree member of CMoDistMorphElem), both within 4 KB

### `CMoDistRingElem.cpp`
- `0x10052ff0`: between 0x10052fd0 (stores CMoDistRingElem_vtbl into this (ctor/dtor)) and 0x10053000 (tree member of CMoDistRingElem), both within 4 KB

### `CMoElemFamily.cpp`
- `0x10041120`: name CMoElemFamily_IsKind0
- `0x10044970`: name CMoElemFamily_SharedCleanup

### `CMoGeneratorClone.cpp`
- `0x100542e0`: between 0x10053e00 (tree member of CMoGeneratorClone) and 0x100542f0 (stores CMoGeneratorClone_vtbl into this (ctor/dtor)), both within 4 KB

### `CMoLfdElem.cpp`
- `0x10053050`: between 0x10053030 (stores CMoLfdElem_vtbl into this (ctor/dtor)) and 0x10053060 (tree member of CMoLfdElem), both within 4 KB

### `CMoPointLightProgElem.cpp`
- `0x10056360`: between 0x100562f0 (stores CMoPointLightProgElem_vtbl into this (ctor/dtor)) and 0x10056370 (tree member of CMoPointLightProgElem), both within 4 KB

### `CMoProcessor.cpp`
- `0x1006d380`: between 0x1006ce70 (tree member of CMoProcessor) and 0x1006d3b0 (name CMoProcessor_VT_1032C368_vf012), both within 4 KB
- `0x1006d3b0`: name CMoProcessor_VT_1032C368_vf012
- `0x1006d680`: between 0x1006d3b0 (name CMoProcessor_VT_1032C368_vf012) and 0x1006d980 (name CMoProcessor_VT_1032C368_vf021), both within 4 KB
- `0x1006d6b0`: between 0x1006d3b0 (name CMoProcessor_VT_1032C368_vf012) and 0x1006d980 (name CMoProcessor_VT_1032C368_vf021), both within 4 KB
- `0x1006d980`: name CMoProcessor_VT_1032C368_vf021

### `CMoVuAnmElem.cpp`
- `0x1003ef80`: between 0x1003ef20 (stores CMoVuAnmElem_vtbl into this (ctor/dtor)) and 0x1003efa0 (tree member of CMoVuAnmElem), both within 4 KB

### `CMoVuDist.cpp`
- `0x10052dd0`: between 0x10052da0 (stores CMoVuDist_vtbl into this (ctor/dtor)) and 0x10052df0 (tree member of CMoVuDist), both within 4 KB

### `CMoVuLineProgElem.cpp`
- `0x10052bf0`: between 0x10052bc0 (stores CMoVuLineProgElem_vtbl into this (ctor/dtor)) and 0x10052c00 (tree member of CMoVuLineProgElem), both within 4 KB

### `CTsZoneMap.cpp`
- `0x101779c0`: between 0x10177800 (name CTsZoneMap_LogZoneMetrics) and 0x10177d00 (stores CTsZoneMap_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10177b40`: between 0x10177800 (name CTsZoneMap_LogZoneMetrics) and 0x10177d00 (stores CTsZoneMap_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10177c50`: between 0x10177800 (name CTsZoneMap_LogZoneMetrics) and 0x10177d00 (stores CTsZoneMap_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10177cd0`: between 0x10177800 (name CTsZoneMap_LogZoneMetrics) and 0x10177d00 (stores CTsZoneMap_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10178070`: hand: operates on CTsZoneMap
- `0x101781d0`: hand: operates on CTsZoneMap
- `0x10178b00`: hand: operates on CTsZoneMap
- `0x101791f0`: hand: operates on CTsZoneMap
- `0x101792a0`: hand: operates on CTsZoneMap
- `0x1017ade0`: hand: operates on CTsZoneMap
- `0x101844c0`: hand: operates on CTsZoneMap
- `0x10184c90`: hand: operates on CTsZoneMap

### `CXiActor.cpp`
- `0x10084e50`: between 0x10084d40 (name CXiActor_IsMovingAction) and 0x10084e70 (name CXiActor_AddTimerTask), both within 4 KB

### `CXiActorNameDraw.cpp`
- `0x100817f0`: name CXiActorNameDraw_SetRenderState

### `CXiControlActor.cpp`
- `0x100a8e10`: between 0x100a8d60 (name CXiControlActor_IsMovingTechnic) and 0x100a9230 (name CXiControlActor_UseMagic_SchStatus), both within 4 KB

### `CXiOpening.cpp`
- `0x100186c0`: stores CXiOpening_vtbl into this (ctor/dtor)
- `0x100187d0`: stores CXiOpening_vtbl into this (ctor/dtor)

### `CXiSkeletonActor.cpp`
- `0x100820d0`: name CXiSkeletonActor_GetEquip
- `0x100a4820`: between 0x100a4810 (tree member of CXiSkeletonActor) and 0x100a4830 (tree member of CXiSkeletonActor), both within 4 KB
- `0x100cedf0`: name CXiSkeletonActor_ReplaceAttack
- `0x100d2f40`: name CXiSkeletonActor_OnSkeletonLoaded

### `CYyModelDt.cpp`
- `0x10022fa0`: between 0x100224d0 (name CYyModelDt_PrepareRenderState) and 0x100231c0 (tree member of CYyModelDt), both within 4 KB

### `CYyOs2VtxBuffer.cpp`
- `0x1002d680`: stores CYyOs2VtxBuffer_vtbl into this (ctor/dtor)
- `0x1002d700`: stores CYyOs2VtxBuffer_vtbl into this (ctor/dtor)

### `CYySoundElem.cpp`
- `0x10036a80`: between 0x100369f0 (stores CYySoundElem_vtbl into this (ctor/dtor)) and 0x10036af0 (stores CYySoundElem_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10036a90`: between 0x100369f0 (stores CYySoundElem_vtbl into this (ctor/dtor)) and 0x10036af0 (stores CYySoundElem_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10036aa0`: between 0x100369f0 (stores CYySoundElem_vtbl into this (ctor/dtor)) and 0x10036af0 (stores CYySoundElem_vtbl into this (ctor/dtor)), both within 4 KB
- `0x10036ab0`: between 0x100369f0 (stores CYySoundElem_vtbl into this (ctor/dtor)) and 0x10036af0 (stores CYySoundElem_vtbl into this (ctor/dtor)), both within 4 KB

### `sqgxBlend.cpp`
- `0x10283220`: PS2 name sqgxBlend*

### `sqgxDlist.cpp`
- `0x1029b9d0`: between 0x1029b620 (asserts sqgxDlist.c) and 0x1029bb60 (asserts sqgxDlist.c), both within 4 KB

### `sqgxTexture.cpp`
- `0x10281830`: PS2 name sqgxTexture*
- `0x102819d0`: between 0x10281830 (PS2 name sqgxTexture*) and 0x10281b60 (PS2 name sqgxTexture*), both within 4 KB
- `0x10281d60`: between 0x10281c80 (PS2 name sqgxTexture*) and 0x10282010 (PS2 name sqgxTexture*), both within 4 KB
- `0x10282010`: PS2 name sqgxTexture*
- `0x10282190`: PS2 name sqgxTexture*

### `sqsnScene.cpp`
- `0x1028f8c0`: sqsnSceneDraw (PS2 name, scene functions)

## Still unplaced (214)

These stay in `gfx_free_NNN.cpp`: there isn't enough evidence for where they came from yet. Each lists the nearest known units around it, as a lead rather than a conclusion.

- `0x10002040`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002be0`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002c00`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002c30`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002c80`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002cd0`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002d20`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10002f20`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x100031f0`: no unit: CApp at 0x100013c0 before, Debug at 0x100037d0 after
- `0x10003b60`: no unit: Debug at 0x100037d0 before, CDx at 0x10008f90 after
- `0x10010310`: no unit: CEnv at 0x1000d660 before, CMoTaskMng at 0x10011950 after
- `0x10011920`: no unit: CEnv at 0x1000d660 before, CMoTaskMng at 0x10011950 after
- `0x10011930`: no unit: CEnv at 0x1000d660 before, CMoTaskMng at 0x10011950 after
- `0x10011940`: no unit: CEnv at 0x1000d660 before, CMoTaskMng at 0x10011950 after
- `0x10013ad0`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x10013df0`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x10013f10`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x10014030`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x10014120`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x100141c0`: no unit: CMoTask_VT_10329C70 at 0x100119a0 before, CAtelIdleTask at 0x100142b0 after
- `0x100157e0`: no unit: CAtelIdleTask at 0x100142b0 before, CShader at 0x10017760 after
- `0x100161b0`: no unit: CAtelIdleTask at 0x100142b0 before, CShader at 0x10017760 after
- `0x10017230`: no unit: CAtelIdleTask at 0x100142b0 before, CShader at 0x10017760 after
- `0x10017750`: no unit: CAtelIdleTask at 0x100142b0 before, CShader at 0x10017760 after
- `0x10019a40`: no unit: CMoResource_VT_1032C7FC at 0x10019a30 before, CYyMotionQue at 0x1001a800 after
- `0x1001b800`: no unit: CYyResfList at 0x1001b760 before, CYyAfterImage at 0x1001b820 after
- `0x1001d0a0`: no unit: CYyAfterImage at 0x1001b820 before, CMoResource_VT_1032C8AC at 0x1001dc20 after
- `0x1001d240`: no unit: CYyAfterImage at 0x1001b820 before, CMoResource_VT_1032C8AC at 0x1001dc20 after
- `0x1001d350`: no unit: CYyAfterImage at 0x1001b820 before, CMoResource_VT_1032C8AC at 0x1001dc20 after
- `0x1001d3b0`: no unit: CYyAfterImage at 0x1001b820 before, CMoResource_VT_1032C8AC at 0x1001dc20 after
- `0x1001dd00`: no unit: CMoResource_VT_1032C8AC at 0x1001dc20 before, CMoResource_VT_1032C880 at 0x1001e050 after
- `0x1001e060`: no unit: CMoResource_VT_1032C880 at 0x1001e050 before, CYyCamMng2 at 0x1001e600 after
- `0x10025090`: no unit: CYyModelDt at 0x10025060 before, CYyModelDt_VT_1032A430 at 0x100250a0 after
- `0x10026990`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x10026ad0`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x10026c70`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x10027930`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x10029880`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x100298a0`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x100298c0`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x10029a50`: no unit: CYyIcon at 0x10026970 before, CYyModelBase at 0x10029d30 after
- `0x1002c360`: no unit: CXiSkeletonActor at 0x1002b6e0 before, CYyObject at 0x1002c8f0 after
- `0x1002c950`: no unit: CYyObject at 0x1002c8f0 before, CYyOs2VtxBuffer at 0x1002d640 after
- `0x1002ccb0`: no unit: CYyObject at 0x1002c8f0 before, CYyOs2VtxBuffer at 0x1002d640 after
- `0x1002f160`: no unit: CMoResource_VT_1032C854 at 0x1002d880 before, CYyOsmTaskXform at 0x10031010 after
- `0x1002f1a0`: no unit: CMoResource_VT_1032C854 at 0x1002d880 before, CYyOsmTaskXform at 0x10031010 after
- `0x10033a70`: no unit: CYyOsmTaskXform at 0x10031070 before, CMoResource_VT_1032C828 at 0x10034170 after
- `0x10034180`: no unit: CMoResource_VT_1032C828 at 0x10034170 before, CYySkl at 0x10034230 after
- `0x100353c0`: no unit: CXiSkeletonActor at 0x100343c0 before, CMoTask_VT_1032A904 at 0x10035ae0 after
- `0x10036b70`: no unit: CYySoundElem at 0x10036af0 before, CYyMusicLoadTask at 0x10037350 after
- `0x10037280`: no unit: CYySoundElem at 0x10036af0 before, CYyMusicLoadTask at 0x10037350 after
- `0x10038920`: no unit: CYyMusicLoadTask at 0x10037960 before, CYyTexBase at 0x100398b0 after
- `0x10039770`: no unit: CYyMusicLoadTask at 0x10037960 before, CYyTexBase at 0x100398b0 after
- `0x10039780`: no unit: CYyMusicLoadTask at 0x10037960 before, CYyTexBase at 0x100398b0 after
- `0x1003aa40`: no unit: CYyTex at 0x10039970 before, CYyTexMng at 0x1003aa50 after
- `0x1003b210`: no unit: CYyVbMng at 0x1003b1c0 before, CYyVb at 0x1003b420 after
- `0x1003b530`: no unit: CYyVb at 0x1003b4a0 before, CMoAttachments at 0x1003b5a0 after
- `0x1003b570`: no unit: CYyVb at 0x1003b4a0 before, CMoAttachments at 0x1003b5a0 after
- `0x1003b580`: no unit: CYyVb at 0x1003b4a0 before, CMoAttachments at 0x1003b5a0 after
- `0x1003b590`: no unit: CYyVb at 0x1003b4a0 before, CMoAttachments at 0x1003b5a0 after
- `0x1003c880`: no unit: CMoAttachments at 0x1003b5a0 before, CMoCamera at 0x1003cb30 after
- `0x1003c890`: no unit: CMoAttachments at 0x1003b5a0 before, CMoCamera at 0x1003cb30 after
- `0x1003cc60`: no unit: CMoCamera at 0x1003cb30 before, CMoCameraTask at 0x1003cc80 after
- `0x1003cc70`: no unit: CMoCamera at 0x1003cb30 before, CMoCameraTask at 0x1003cc80 after
- `0x1003d940`: no unit: CMoCameraTask at 0x1003d030 before, CMoCelestial at 0x1003d950 after
- `0x1003e690`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003e6a0`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003ee50`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003ee60`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003ef00`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003ef10`: no unit: CMoCelestial at 0x1003d980 before, CMoVuAnmElem at 0x1003ef20 after
- `0x1003f430`: no unit: CMoVuAnmElem at 0x1003efa0 before, CMoDistMorphElem at 0x1003f440 after
- `0x10041000`: no unit: CXi471011C8 at 0x10040fb0 before, CMoD3mSpecularElem at 0x10041020 after
- `0x10041010`: no unit: CXi471011C8 at 0x10040fb0 before, CMoD3mSpecularElem at 0x10041020 after
- `0x10042640`: no unit: CMoElem_VT_1032ADE8 at 0x10041fe0 before, CMoAttachments_VT_1032AE94 at 0x10042690 after
- `0x10042650`: no unit: CMoElem_VT_1032ADE8 at 0x10041fe0 before, CMoAttachments_VT_1032AE94 at 0x10042690 after
- `0x100426a0`: no unit: CMoAttachments_VT_1032AE94 at 0x10042690 before, CMoDistMorphElem at 0x10042d00 after
- `0x100426b0`: no unit: CMoAttachments_VT_1032AE94 at 0x10042690 before, CMoDistMorphElem at 0x10042d00 after
- `0x100426c0`: no unit: CMoAttachments_VT_1032AE94 at 0x10042690 before, CMoDistMorphElem at 0x10042d00 after
- `0x100426d0`: no unit: CMoAttachments_VT_1032AE94 at 0x10042690 before, CMoDistMorphElem at 0x10042d00 after
- `0x100426e0`: no unit: CMoAttachments_VT_1032AE94 at 0x10042690 before, CMoDistMorphElem at 0x10042d00 after
- `0x100430b0`: no unit: CMoDistMorphElem at 0x10042d00 before, CMoDistModelElem at 0x10043320 after
- `0x100430c0`: no unit: CMoDistMorphElem at 0x10042d00 before, CMoDistModelElem at 0x10043320 after
- `0x10043190`: no unit: CMoDistMorphElem at 0x10042d00 before, CMoDistModelElem at 0x10043320 after
- `0x100431a0`: no unit: CMoDistMorphElem at 0x10042d00 before, CMoDistModelElem at 0x10043320 after
- `0x10043570`: no unit: CMoDistModelElem at 0x10043320 before, CMoDistMorphElem at 0x10043590 after
- `0x10043580`: no unit: CMoDistModelElem at 0x10043320 before, CMoDistMorphElem at 0x10043590 after
- `0x10043700`: no unit: CMoDistMorphElem at 0x10043590 before, CMoDistRingElem at 0x10043720 after
- `0x10043710`: no unit: CMoDistMorphElem at 0x10043590 before, CMoDistRingElem at 0x10043720 after
- `0x10044870`: no unit: CMoTask_VT_1032AFA8 at 0x10043e20 before, CMoElem at 0x10044950 after
- `0x10044880`: no unit: CMoTask_VT_1032AFA8 at 0x10043e20 before, CMoElem at 0x10044950 after
- `0x100472e0`: no unit: CXi471011C8 at 0x10045d00 before, CMoSkeletonElem at 0x10047590 after
- `0x10047550`: no unit: CXi471011C8 at 0x10045d00 before, CMoSkeletonElem at 0x10047590 after
- `0x10047560`: no unit: CXi471011C8 at 0x10045d00 before, CMoSkeletonElem at 0x10047590 after
- `0x10047570`: no unit: CXi471011C8 at 0x10045d00 before, CMoSkeletonElem at 0x10047590 after
- `0x10047580`: no unit: CXi471011C8 at 0x10045d00 before, CMoSkeletonElem at 0x10047590 after
- `0x10047fd0`: no unit: CMoResource at 0x10047fc0 before, CMoGeneratorClone_VT_1032B114 at 0x10047fe0 after
- `0x10052d60`: no unit: CMoVuAnmElem at 0x10052cd0 before, CMoDist at 0x10052d70 after
- `0x10054480`: no unit: CMoTask_VT_1032B654 at 0x10054470 before, CMoLfdElem at 0x100548c0 after
- `0x10054490`: no unit: CMoTask_VT_1032B654 at 0x10054470 before, CMoLfdElem at 0x100548c0 after
- `0x100548a0`: no unit: CMoTask_VT_1032B654 at 0x10054470 before, CMoLfdElem at 0x100548c0 after
- `0x100548b0`: no unit: CMoTask_VT_1032B654 at 0x10054470 before, CMoLfdElem at 0x100548c0 after
- `0x10055290`: no unit: CMoLfdElem at 0x100548c0 before, CMoOT at 0x100552a0 after
- `0x10055550`: no unit: CMoOT at 0x100552e0 before, CMoPathObject at 0x10055780 after
- `0x10055560`: no unit: CMoOT at 0x100552e0 before, CMoPathObject at 0x10055780 after
- `0x10055a50`: no unit: CMoPathObject at 0x10055780 before, CMoSphRes at 0x100562b0 after
- `0x10056250`: no unit: CMoPathObject at 0x10055780 before, CMoSphRes at 0x100562b0 after
- `0x10056260`: no unit: CMoPathObject at 0x10055780 before, CMoSphRes at 0x100562b0 after
- `0x10056290`: no unit: CMoPathObject at 0x10055780 before, CMoSphRes at 0x100562b0 after
- `0x100562a0`: no unit: CMoPathObject at 0x10055780 before, CMoSphRes at 0x100562b0 after
- `0x100562c0`: no unit: CMoSphRes at 0x100562b0 before, CMoPointLightProgElem at 0x100562f0 after
- `0x100562d0`: no unit: CMoSphRes at 0x100562b0 before, CMoPointLightProgElem at 0x100562f0 after
- `0x100562e0`: no unit: CMoSphRes at 0x100562b0 before, CMoPointLightProgElem at 0x100562f0 after
- `0x10056930`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056940`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056950`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056960`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056970`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056980`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10056990`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x100569a0`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x100569b0`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x100569c0`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10057080`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10057090`: no unit: CXi471011C8 at 0x100565a0 before, CMoSchedularTask at 0x100571e0 after
- `0x10063450`: no unit: CMoTask_VT_1032B95C at 0x10063140 before, CMoSpline at 0x10063460 after
- `0x10064160`: no unit: CMoResource_VT_1032C55C at 0x10063c50 before, CMoDX at 0x10069d70 after
- `0x10064170`: no unit: CMoResource_VT_1032C55C at 0x10063c50 before, CMoDX at 0x10069d70 after
- `0x10064180`: no unit: CMoResource_VT_1032C55C at 0x10063c50 before, CMoDX at 0x10069d70 after
- `0x10069d60`: no unit: CMoResource_VT_1032C55C at 0x10063c50 before, CMoDX at 0x10069d70 after
- `0x1006b040`: no unit: CMoDX at 0x10069db0 before, CMoDxStatusMng at 0x1006b050 after
- `0x1006c130`: no unit: CMoDX at 0x1006b2b0 before, CMoOcclusionMng at 0x1006c140 after
- `0x1006c220`: no unit: CMoOcclusionMng at 0x1006c1e0 before, CMoProcessor_VT_1032C1C8 at 0x1006cc60 after
- `0x1006c280`: no unit: CMoOcclusionMng at 0x1006c1e0 before, CMoProcessor_VT_1032C1C8 at 0x1006cc60 after
- `0x1006ccd0`: no unit: CMoProcessor_VT_1032C1C8 at 0x1006cc60 before, CMoProcessor at 0x1006ce70 after
- `0x1006fa30`: no unit: CMoProcessor at 0x1006e6b0 before, CMoProcessor_VT_1032C298 at 0x10070020 after
- `0x10070040`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070780`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070860`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070aa0`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070ab0`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070ac0`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070ad0`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070d80`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070e20`: no unit: CMoProcessor_VT_1032C298 at 0x10070020 before, CMoProcessor_VT_1032C1C8 at 0x10070e40 after
- `0x10070e60`: no unit: CMoProcessor_VT_1032C1C8 at 0x10070e40 before, CMoTask_VT_1032C490 at 0x10070f60 after
- `0x100719c0`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x100719d0`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x100719e0`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x100719f0`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x10071a00`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x10071a10`: no unit: CMoCamera at 0x10071980 before, CMoResource_VT_1032C778 at 0x10071a20 after
- `0x10071d70`: no unit: CMoSphRes at 0x10071d00 before, CMoResource at 0x10071dc0 after
- `0x10071db0`: no unit: CMoSphRes at 0x10071d00 before, CMoResource at 0x10071dc0 after
- `0x10072d00`: no unit: CMoTask_VT_1032C908 at 0x10072a30 before, CMoResource_VT_1032C934 at 0x10073730 after
- `0x10074610`: no unit: CMoAttachments_VT_1032C628 at 0x10074600 before, CMoResource_VT_1032C588 at 0x10074620 after
- `0x10074730`: no unit: CMoResource_VT_1032C588 at 0x10074620 before, CMoTask at 0x10074770 after
- `0x10074b50`: no unit: CMoTask at 0x10074770 before, CMoTaskMng at 0x10074b60 after
- `0x100755a0`: no unit: CMoTaskMng at 0x10074b60 before, CXiActorDraw at 0x100817e0 after
- `0x10081710`: no unit: CMoTaskMng at 0x10074b60 before, CXiActorDraw at 0x100817e0 after
- `0x10081730`: no unit: CMoTaskMng at 0x10074b60 before, CXiActorDraw at 0x100817e0 after
- `0x10081750`: no unit: CMoTaskMng at 0x10074b60 before, CXiActorDraw at 0x100817e0 after
- `0x100ab070`: no unit: CMoAttachments_VT_1032E7EC at 0x100ab060 before, CXiGeomActor at 0x100ab1b0 after
- `0x100c5190`: no unit: XiFurnitureActor at 0x100c3ec0 before, CXiSkeletonActor at 0x100c5c00 after
- `0x100c51a0`: no unit: XiFurnitureActor at 0x100c3ec0 before, CXiSkeletonActor at 0x100c5c00 after
- `0x100c51b0`: no unit: XiFurnitureActor at 0x100c3ec0 before, CXiSkeletonActor at 0x100c5c00 after
- `0x100d6f90`: no unit: CXiSkeletonActor at 0x100d6464 before, CXiSkeletonActorRes at 0x100d6fe0 after
- `0x100d6fa0`: no unit: CXiSkeletonActor at 0x100d6464 before, CXiSkeletonActorRes at 0x100d6fe0 after
- `0x100d6fb0`: no unit: CXiSkeletonActor at 0x100d6464 before, CXiSkeletonActorRes at 0x100d6fe0 after
- `0x100d6fc0`: no unit: CXiSkeletonActor at 0x100d6464 before, CXiSkeletonActorRes at 0x100d6fe0 after
- `0x100d6fd0`: no unit: CXiSkeletonActor at 0x100d6464 before, CXiSkeletonActorRes at 0x100d6fe0 after
- `0x100d7140`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1011aac0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1011bbd0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1011bdc0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1011c1f0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1011c720`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x10120590`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x101207b0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x10124440`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x101252f0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x101604b0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x10160650`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x101620b0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x101630e0`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x10163400`: no unit: CXiSkeletonActorRes at 0x100d70a0 before, StAvatarCoordinator at 0x10163640 after
- `0x1016ddf0`: no unit: StAvatarCoordinator at 0x10163640 before, CMoResource_VT_10336698 at 0x10176ea0 after
- `0x10174ec0`: no unit: StAvatarCoordinator at 0x10163640 before, CMoResource_VT_10336698 at 0x10176ea0 after
- `0x10177240`: no unit: CMoResource_VT_103366F0 at 0x10177000 before, CTsZoneMap at 0x10177800 after
- `0x101773b0`: no unit: CMoResource_VT_103366F0 at 0x10177000 before, CTsZoneMap at 0x10177800 after
- `0x10177400`: no unit: CMoResource_VT_103366F0 at 0x10177000 before, CTsZoneMap at 0x10177800 after
- `0x10182f10`: no unit: CTsZoneMap at 0x1017ade0 before, CTsZoneMap at 0x101844c0 after
- `0x10188fa0`: no unit: CTsZoneMap at 0x10184c90 before, CMoResource_VT_103367E8 at 0x101893d0 after
- `0x101893f0`: no unit: CMoResource_VT_103367E8 at 0x101893d0 before, YmCombineWeather at 0x1018a7a0 after
- `0x102394a0`: no unit: CXiMovie at 0x1021d880 before, YkZip at 0x102441c0 after
- `0x102504d0`: no unit: StAvatar at 0x1024d2c0 before, StChannel at 0x10250e10 after
- `0x10251a80`: no unit: StChannel at 0x102512b0 before, StModel at 0x102524e0 after
- `0x10253f60`: no unit: XiPng at 0x102539e0 before, sqxfXformTD at 0x1026dee0 after
- `0x1025d750`: no unit: XiPng at 0x102539e0 before, sqxfXformTD at 0x1026dee0 after
- `0x1025d760`: no unit: XiPng at 0x102539e0 before, sqxfXformTD at 0x1026dee0 after
- `0x1025d7b0`: no unit: XiPng at 0x102539e0 before, sqxfXformTD at 0x1026dee0 after
- `0x10282350`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x10282470`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x102824c0`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x10282640`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x10282b90`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x10282ff0`: no unit: sqgxTexture at 0x10282190 before, sqgxBlend at 0x10283220 after
- `0x10283f70`: no unit: sqgxMatrix at 0x10283d60 before, sqgxShader at 0x102848a0 after
- `0x10283fc0`: no unit: sqgxMatrix at 0x10283d60 before, sqgxShader at 0x102848a0 after
- `0x10284410`: no unit: sqgxMatrix at 0x10283d60 before, sqgxShader at 0x102848a0 after
- `0x10284a80`: no unit: sqgxShader at 0x102848a0 before, sqmoChannel at 0x10284f50 after
- `0x10297cc0`: no unit: sqMatrix3 at 0x10296dc0 before, sqimImage at 0x102984b0 after
- `0x10297ec0`: no unit: sqMatrix3 at 0x10296dc0 before, sqimImage at 0x102984b0 after
- `0x102a2530`: no unit: sqsnShadow at 0x102a2430 before, sqrdMaterial at 0x102a3330 after

## Next step closer to source
Most of the moved game-side functions take the object as their first argument (`self_`), so in the original source they were almost certainly class methods (`CTsZoneMap::...`). Converting them into real members changes signatures, symbol names and bodies (`self_` -> `this`). So it has not been done here.
