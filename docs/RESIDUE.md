# Graphics functions not in the verified tree (build 2026-09-03)

Every function in the 768-function graphics set that is not a gate-verified member of `tree/` is listed here, with why and what to do with it. Descriptions in the last column are DancingMad's.

| Group | Count | Route |
|---|---|---|
| Microsoft D3DX 8.1 (statically linked) | 47 | Not game code. Byte-identical to `d3dx8.lib` from the DirectX 8.1 SDK; a standalone build links that library and includes `d3dx8.h`. In a hybrid build they keep running from the game image. |
| Constructs C++ cannot express | 9 | Stays original code (`orig::`). Listed below with the exact construct. |
| Did not compile, or compiles with no gate case | 0 | Translator gaps; listed below. |
| Gate FAIL (decompiled code is wrong) | 1 | Hex-Rays output differs from the original; the first divergence is given. Fix in IDA (types) and re-run, or keep `orig::`. |
| Verified as decompiled, not in the clean tree | 0 | Clean-stage gap (no exported symbol to gate); the raw translation is verified. |
| Gate INCONCLUSIVE (no comparable run) | 30 | The original never returned under fuzzed inputs; needs better input construction or a live memory overlay. |

## Microsoft D3DX 8.1 (47)

| Address | IDB name | Library symbol | Object | Match |
|---|---|---|---|---|
| `0x102D270D` | ?D3DXFindClosestDeviceFormat@@YA?AW4_D3DFORMAT@@PAUIDirect3DDevice8@@KW4_D3DRESOURCETYPE@@PBU_D3DXFORMAT_INFO@@@Z | `?D3DXFindClosestDeviceFormat@@YA?AW4_D3DFORMAT@@PAUIDirect3DDevice8@@KW4_D3DRESOURCETYPE@@PBU_D3DXFORMAT_INFO@@@Z` | d3dx8tex.obj | 226 bytes, name+bytes |
| `0x102D2868` | _D3DXLoadSurfaceFromMemory@40 | `_D3DXLoadSurfaceFromMemory@40` | d3dx8tex.obj | 223 bytes, name+bytes |
| `0x102D2A53` | _D3DXLoadVolumeFromMemory@44 | `_D3DXLoadVolumeFromMemory@44` | d3dx8tex.obj | 211 bytes, name+bytes |
| `0x102D2F40` | _D3DXCheckTextureRequirements@28 | `_D3DXCheckTextureRequirements@28` | d3dx8tex.obj | 40 bytes, name+bytes |
| `0x102D2F68` | _D3DXCheckCubeTextureRequirements@24 | `_D3DXCheckCubeTextureRequirements@24` | d3dx8tex.obj | 40 bytes, name+bytes |
| `0x102D2F90` | _D3DXCheckVolumeTextureRequirements@32 | `_D3DXCheckVolumeTextureRequirements@32` | d3dx8tex.obj | 41 bytes, name+bytes |
| `0x102D2FB9` | _D3DXCreateTexture@32 | `_D3DXCreateTexture@32` | d3dx8tex.obj | 102 bytes, name+bytes |
| `0x102D301F` | _D3DXCreateCubeTexture@28 | `_D3DXCreateCubeTexture@28` | d3dx8tex.obj | 95 bytes, name+bytes |
| `0x102D31EF` | _D3DXFillCubeTexture@12 | `_D3DXFillCubeTexture@12` | d3dx8tex.obj | 1738 bytes, name+bytes |
| `0x102D445D` | _D3DXLoadSurfaceFromFileInMemory@36 | `_D3DXLoadSurfaceFromFileInMemory@36` | d3dx8tex.obj | 184 bytes, name+bytes |
| `0x102D4515` | _D3DXLoadSurfaceFromSurface@32 | `_D3DXLoadSurfaceFromSurface@32` | d3dx8tex.obj | 422 bytes, name+bytes |
| `0x102D46BB` | _D3DXLoadVolumeFromFileInMemory@36 | `_D3DXLoadVolumeFromFileInMemory@36` | d3dx8tex.obj | 188 bytes, name+bytes |
| `0x102D4777` | _D3DXLoadVolumeFromVolume@32 | `_D3DXLoadVolumeFromVolume@32` | d3dx8tex.obj | 134 bytes, name+bytes |
| `0x102D4BF7` | _D3DXFilterTexture@16 | `_D3DXFilterTexture@16` | d3dx8tex.obj | 740 bytes, name+bytes |
| `0x102D59FD` | _D3DXCreateTextureFromFileInMemoryEx@60 | `_D3DXCreateTextureFromFileInMemoryEx@60` | d3dx8tex.obj | 64 bytes, name+bytes |
| `0x102D5A3D` | _D3DXCreateCubeTextureFromFileInMemoryEx@56 | `_D3DXCreateCubeTextureFromFileInMemoryEx@56` | d3dx8tex.obj | 64 bytes, name+bytes |
| `0x102D5A7D` | _D3DXCreateVolumeTextureFromFileInMemoryEx@64 | `_D3DXCreateVolumeTextureFromFileInMemoryEx@64` | d3dx8tex.obj | 65 bytes, name+bytes |
| `0x102D5ABE` | _D3DXCreateTextureFromFileExA@56 | `_D3DXCreateTextureFromFileExA@56` | d3dx8tex.obj | 99 bytes, name+bytes |
| `0x102D5B21` | _D3DXCreateTextureFromFileExW@56 | `_D3DXCreateTextureFromFileExW@56` | d3dx8tex.obj | 99 bytes, name+bytes |
| `0x102D5B84` | _D3DXCreateTextureFromResourceExA@60 | `_D3DXCreateTextureFromResourceExA@60` | d3dx8tex.obj | 104 bytes, name+bytes |
| `0x102D5BEC` | _D3DXCreateTextureFromResourceExW@60 | `_D3DXCreateTextureFromResourceExW@60` | d3dx8tex.obj | 104 bytes, name+bytes |
| `0x102D5C84` | _D3DXCreateCubeTextureFromFileExA@52 | `_D3DXCreateCubeTextureFromFileExA@52` | d3dx8tex.obj | 96 bytes, name+bytes |
| `0x102D5CE4` | _D3DXCreateCubeTextureFromFileExW@52 | `_D3DXCreateCubeTextureFromFileExW@52` | d3dx8tex.obj | 96 bytes, name+bytes |
| `0x102D5D44` | _D3DXCreateCubeTextureFromResourceExA@56 | `_D3DXCreateCubeTextureFromResourceExA@56` | d3dx8tex.obj | 101 bytes, name+bytes |
| `0x102D5DA9` | _D3DXCreateCubeTextureFromResourceExW@56 | `_D3DXCreateCubeTextureFromResourceExW@56` | d3dx8tex.obj | 101 bytes, name+bytes |
| `0x102D5E3E` | _D3DXCreateVolumeTextureFromFileExA@60 | `_D3DXCreateVolumeTextureFromFileExA@60` | d3dx8tex.obj | 102 bytes, name+bytes |
| `0x102D5EA4` | _D3DXCreateVolumeTextureFromFileExW@60 | `_D3DXCreateVolumeTextureFromFileExW@60` | d3dx8tex.obj | 102 bytes, name+bytes |
| `0x102D5F0A` | _D3DXCreateVolumeTextureFromResourceExA@64 | `_D3DXCreateVolumeTextureFromResourceExA@64` | d3dx8tex.obj | 107 bytes, name+bytes |
| `0x102D5F75` | _D3DXCreateVolumeTextureFromResourceExW@64 | `_D3DXCreateVolumeTextureFromResourceExW@64` | d3dx8tex.obj | 107 bytes, name+bytes |
| `0x102DAD3B` | _D3DXCreateFontIndirect@12 | `_D3DXCreateFontIndirect@12` | d3dx8core.obj | 111 bytes, name+bytes |
| `0x102DADAA` | _D3DXCreateSprite@8 | `_D3DXCreateSprite@8` | d3dx8core.obj | 108 bytes, name+bytes |
| `0x102E1006` | ?Lock@CD3DXLockSurface@@QAEJPAUD3DX_BLT@@PAUIDirect3DSurface8@@PBUtagPALETTEENTRY@@PBUtagRECT@@KK@Z | `?Lock@CD3DXLockSurface@@QAEJPAUD3DX_BLT@@PAUIDirect3DSurface8@@PBUtagPALETTEENTRY@@PBUtagRECT@@KK@Z` | cd3dxlock.obj | 1043 bytes, name+bytes |
| `0x102E141E` | ?Lock@CD3DXLockVolume@@QAEJPAUD3DX_BLT@@PAUIDirect3DVolume8@@PBUtagPALETTEENTRY@@PBU_D3DBOX@@KK@Z | `?Lock@CD3DXLockVolume@@QAEJPAUD3DX_BLT@@PAUIDirect3DVolume8@@PBUtagPALETTEENTRY@@PBU_D3DBOX@@KK@Z` | cd3dxlock.obj | 859 bytes, name+bytes |
| `0x102E7777` | ?Initialize@CD3DXFont@@QAEJPAUIDirect3DDevice8@@PBUtagLOGFONTA@@@Z | `?Initialize@CD3DXFont@@QAEJPAUIDirect3DDevice8@@PBUtagLOGFONTA@@@Z` | cd3dxfont.obj | 94 bytes, name+bytes |
| `0x102E77EC` | ?GetDevice@CD3DXFont@@UAGJPAPAUIDirect3DDevice8@@@Z | `?GetDevice@CD3DXFont@@UAGJPAPAUIDirect3DDevice8@@@Z` | cd3dxfont.obj | 38 bytes, name+bytes |
| `0x102E7985` | ?Initialize@CD3DXSprite@@QAEJPAUIDirect3DDevice8@@@Z | `?Initialize@CD3DXSprite@@QAEJPAUIDirect3DDevice8@@@Z` | cd3dxsprite.obj | 117 bytes, name+bytes |
| `0x102E79FA` | ?GetDevice@CD3DXRenderToEnvMap@@UAGJPAPAUIDirect3DDevice8@@@Z_0 | `?GetDevice@CD3DXSprite@@UAGJPAPAUIDirect3DDevice8@@@Z` | cd3dxsprite.obj | 38 bytes, bytes |
| `0x102E8011` | ?DrawTransform@CD3DXSprite@@UAGJPAUIDirect3DTexture8@@PBUtagRECT@@PBUD3DXMATRIX@@K@Z | `?DrawTransform@CD3DXSprite@@UAGJPAUIDirect3DTexture8@@PBUtagRECT@@PBUD3DXMATRIX@@K@Z` | cd3dxsprite.obj | 979 bytes, name+bytes |
| `0x102E8402` | ?Init@CD3DXRenderToSurface@@QAEJPAUIDirect3DDevice8@@IIW4_D3DFORMAT@@H1@Z | `?Init@CD3DXRenderToSurface@@QAEJPAUIDirect3DDevice8@@IIW4_D3DFORMAT@@H1@Z` | cd3dxrendertosurface.obj | 79 bytes, name+bytes |
| `0x102E84BF` | ?BeginScene@CD3DXRenderToSurface@@UAGJPAUIDirect3DSurface8@@PBU_D3DVIEWPORT8@@@Z | `?BeginScene@CD3DXRenderToSurface@@UAGJPAUIDirect3DSurface8@@PBU_D3DVIEWPORT8@@@Z` | cd3dxrendertosurface.obj | 570 bytes, name+bytes |
| `0x102E88DE` | ?Init@CD3DXRenderToEnvMap@@QAEJPAUIDirect3DDevice8@@IIW4_D3DFORMAT@@H1@Z | `?Init@CD3DXRenderToEnvMap@@QAEJPAUIDirect3DDevice8@@IIW4_D3DFORMAT@@H1@Z` | cd3dxrendertoenvmap.obj | 134 bytes, name+bytes |
| `0x102E8BD7` | ?BeginCube@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DCubeTexture8@@@Z | `?BeginCube@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DCubeTexture8@@@Z` | cd3dxrendertoenvmap.obj | 112 bytes, name+bytes |
| `0x102E8C47` | ?BeginSphere@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@@Z | `?BeginSphere@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@@Z` | cd3dxrendertoenvmap.obj | 132 bytes, name+bytes |
| `0x102E8CCB` | ?BeginHemisphere@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@0@Z | `?BeginHemisphere@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@0@Z` | cd3dxrendertoenvmap.obj | 213 bytes, name+bytes |
| `0x102E8DA0` | ?BeginParabolic@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@0@Z | `?BeginParabolic@CD3DXRenderToEnvMap@@UAGJPAUIDirect3DTexture8@@0@Z` | cd3dxrendertoenvmap.obj | 213 bytes, name+bytes |
| `0x102F4B2C` | ?Initialize@CD3DXText@@IAEJPAUIDirect3DDevice8@@@Z | `?Initialize@CD3DXText@@IAEJPAUIDirect3DDevice8@@@Z` | cd3dxtext.obj | 321 bytes, name+bytes |
| `0x102F5180` | ?Create@CD3DXText@@SAJPAUIDirect3DDevice8@@PAPAV1@@Z | `?Create@CD3DXText@@SAJPAUIDirect3DDevice8@@PAPAV1@@Z` | cd3dxtext.obj | 117 bytes, name+bytes |

## Constructs C++ cannot express (9)

| Address | Name | Construct | What it does |
|---|---|---|---|
| `0x1000C850` | CDx_CreateRampTexture1x16 | unsupported construct retaddr | Procedural 1x16 A8R8G8B8 ramp texture (D3DXCreateTexture + LockRect). |
| `0x10057FB0` | CMoSchedularTask_Interpret | unsupported construct __asm | 23.6 KB scheduler opcode interpreter (jump table 0x1005DC1C), reached from CMoSchedularTask vtable slot 8 via ... |
| `0x10125360` | sub_10125360 | unsupported construct __usercall |  |
| `0x101772E0` | sub_101772E0 | unsupported construct __userpurge |  |
| `0x10178250` | sub_10178250 | callee sub_101772E0 has no C++ type (int __userpurge@<eax>(const char **this@<ecx>, void *edi0@<edi>, int)) |  |
| `0x10184C90` | sub_10184C90 | unsupported construct __asm |  |
| `0x10281510` | sqgxTextureCreateFromImage | unsupported construct MEMORY[ |  |
| `0x1028AB50` | sub_1028AB50 | unsupported construct retaddr |  |
| `0x102D538A` | sub_102D538A | callee _D3DXFilterTexture@16 has no C++ type (int __userpurge@<eax>(unsigned int edi0@<edi>, struct IDirect3DSurface8 *, const struct tagPALETTEENTRY *, char *, int)) |  |

## Gate FAIL (1)

| Address | Name | First difference | Reconstruction line at the divergence | What it does |
|---|---|---|---|---|
| `0x100C63D0` | CXiSkeletonActor_OnMove | memory differs at 33 byte(s), first 0x30002A8C: original 16 / reconstruction unchanged |  | PS2 vtable slot 5 (OnMove) -> PC slot 8. The per-frame actor update. |

## Gate INCONCLUSIVE (30)

| Address | Name | Original outcomes | What it does |
|---|---|---|---|
| `0x10002130` | Gfx_ProbeDeviceCaps | fault 100 | CheckDeviceType + CheckDeviceFormat probes, including DXT1/DXT3/DXT5. |
| `0x10003040` | Gfx_DestroyD3DHolder | fault 100 |  |
| `0x10003890` | sub_10003890 | fault 100 |  |
| `0x100091B0` | CDx_Initialize | import 100 | CheckDeviceFormat x3, multisample checks x2, loads mousenor.ani. Queries shader-model support (verified D3DVS_... |
| `0x10009F20` | CDx_PushRenderTargetTexture | fault 100 | Texture->GetSurfaceLevel(0), device->SetRenderTarget; increments stack depth CDx+0x158. |
| `0x1000A6E0` | CDx_CreateTexture | fault 100 | Sole texture-creation wrapper; format passed as variable. 10 calling functions. |
| `0x1000C7C0` | CDx_ReleaseStereoResources | fault 100 | Releases +0x814/+0x81C/+0x824/+0x82C. |
| `0x10016320` | sub_10016320 | fault 100 |  |
| `0x10017810` | sub_10017810 | fault 100 |  |
| `0x10017C60` | sub_10017C60 | fault 100 |  |
| `0x10025260` | sub_10025260 | fault 100 |  |
| `0x10033710` | sub_10033710 | fault 100 |  |
| `0x1003F440` | CMoDistMorphElem_ApplyMorph | fault 100 | The actual morph-apply function, called with the cached handle and both morph field addresses from CMoDistMorp... |
| `0x10040C20` | CMoElemFamily_SharedEarlyExit | fault 100 | Shared no-op/early-exit path several CMoElem-family update functions tail-jump into when a flag indicates no w... |
| `0x100426F0` | sub_100426F0 | fault 100 |  |
| `0x10042D00` | CMoDistMorphElem_ValidateMorphData | fault 100 | Validates morph blend data; CMoDistMorphElem_UpdateMorphBlend early-exits if this returns false. |
| `0x100467E0` | sub_100467E0 | fault 100 |  |
| `0x10047590` | CMoSkeletonElem_UpdateBoneTransform | fault 100 | Normalizes local rotation (Euler wraparound), calls Mat4x4_Multiply to combine with the parent's matrix (confi... |
| `0x100548D0` | sub_100548D0 | fault 100 |  |
| `0x1006C140` | CMoOcclusionMng_Constructor | fault 100 |  |
| `0x1006C530` | sub_1006C530 | fault 100 | Draws the queried region into the 16x16 occlusion target: two CDx_DrawPrimitiveUP calls, state set directly on... |
| `0x1006E6B0` | CMoProcessor_ScratchMatrixOp | fault 100 | CORRECTED from an earlier name (Pool_AllocSlot) that undersold what this does. Full decompilation shows a self... |
| `0x10075280` | Gfx_InitCubeMapTextures | fault 100 |  |
| `0x100AC390` | sub_100AC390 | fault 100 |  |
| `0x100C4CE0` | sub_100C4CE0 | fault 100 |  |
| `0x100CB4E0` | Actor_SelectNearestLights | fault 100 |  |
| `0x1017B510` | sub_1017B510 | fault 100 | Sets D3DTS_PROJECTION plus fog table/vertex/range modes, fog color/start/end and ambient from the renderer blo... |
| `0x10183490` | sub_10183490 | fault 100 |  |
| `0x10183760` | sub_10183760 | fault 100 |  |
| `0x10280DE0` | sqgx_DrawRect_Candidate | fault 100 |  |

