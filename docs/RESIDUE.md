# Graphics functions not in the verified tree (build 2026-09-03)

Every function in the 768-function graphics set that is not a gate-verified member of `tree/` is listed here, with why and what to do with it. Descriptions in the last column are DancingMad's.

| Group | Count | Route |
|---|---|---|
| Microsoft D3DX 8.1 (statically linked) | 48 | Not game code. Byte-identical to `d3dx8.lib` from the DirectX 8.1 SDK; a standalone build links that library and includes `d3dx8.h`. In a hybrid build they keep running from the game image. |
| Constructs C++ cannot express | 5 | Stays original code (`orig::`). Listed below with the exact construct. |
| Did not compile, or compiles with no gate case | 0 | Translator gaps; listed below. |
| Gate FAIL (decompiled code is wrong) | 2 | Hex-Rays output differs from the original; the first divergence is given. Fix in IDA (types) and re-run, or keep `orig::`. |
| Verified as decompiled, not in the clean tree | 0 | Clean-stage gap (no exported symbol to gate); the raw translation is verified. |
| Gate INCONCLUSIVE (no comparable run) | 1 | The original never returned under fuzzed inputs; needs better input construction or a live memory overlay. |
| Gate UNRESOLVED (no definitive fact for a call) | 6 | A call the original makes has no definitive pop or argument count (`CLAUDE.md`), so no verdict is given. The call site is named; settle it in IDA (type the callee or the call) or from a live run, then re-gate. |
| Passes as decompiled, fails in the clean tree | 0 | Clean-stage regression; listed below. |

## Microsoft D3DX 8.1 (48)

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
| `0x102D538A` | sub_102D538A | `D3DX 8.1: sub_102D538A` |  | ? bytes, ? |
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

## Constructs C++ cannot express (5)

| Address | Name | Construct | What it does |
|---|---|---|---|
| `0x1000A6E0` | CDx_CreateTexture | unsupported construct retaddr | Sole texture-creation wrapper; format passed as variable. 10 calling functions. |
| `0x10057FB0` | CMoSchedularTask_Interpret | unsupported construct __asm | 23.6 KB scheduler opcode interpreter (jump table 0x1005DC1C), reached from CMoSchedularTask vtable slot 8 via ... |
| `0x10125360` | sub_10125360 | unsupported construct __asm |  |
| `0x1017C9C0` | sub_1017C9C0 | unsupported construct retaddr |  |
| `0x10184C90` | sub_10184C90 | unsupported construct __asm |  |

## Gate FAIL (2)

| Address | Name | First difference | Reconstruction line at the divergence | What it does |
|---|---|---|---|---|
| `0x100548D0` | sub_100548D0 | EAX 0x00000005 vs 0x30703005 |  |  |
| `0x1017DE20` | sub_1017DE20 | ended differently: original return  / reconstruction fault access 0x00000068 size 4 type 19 |  |  |

## Gate INCONCLUSIVE (1)

| Address | Name | Original outcomes | What it does |
|---|---|---|---|
| `0x10016320` | sub_10016320 | fault 99, import 1 |  |

## Gate UNRESOLVED (6)

| Address | Name | Call without a definitive fact | What it does |
|---|---|---|---|
| `0x10003890` | sub_10003890 | call 0x10003A50: pop count unknown (51 trials) |  |
| `0x10009F20` | CDx_PushRenderTargetTexture | call 0x10009F44: pop count unknown (99 trials); import DSOUND.dll!ord11: argument count unknown (not in import_nargs.json, call site not solved) (1 trial) | Texture->GetSurfaceLevel(0), device->SetRenderTarget; increments stack depth CDx+0x158. |
| `0x10047590` | CMoSkeletonElem_UpdateBoneTransform | call 0x10047623: pop count unknown (100 trials) | Normalizes local rotation (Euler wraparound), calls Mat4x4_Multiply to combine with the parent's matrix (confi... |
| `0x1006C140` | CMoOcclusionMng_Constructor | call 0x1006C16F: pop count unknown (100 trials) |  |
| `0x100CB4E0` | Actor_SelectNearestLights | call 0x100CB8CB: pop count unknown (100 trials) |  |
| `0x10280DE0` | sqgxSprite | call 0x102811DC: pop count unknown (83 trials) |  |

