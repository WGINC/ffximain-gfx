# Graphics functions not in the verified tree (build 2026-09-03)

Every function in the 768-function graphics set that is not a gate-verified member of `tree/` is listed here, with why and what to do with it. Descriptions in the last column are DancingMad's.

| Group | Count | Route |
|---|---|---|
| Microsoft D3DX 8.1 (statically linked) | 47 | Not game code. Byte-identical to `d3dx8.lib` from the DirectX 8.1 SDK; a standalone build links that library and includes `d3dx8.h`. In a hybrid build they keep running from the game image. |
| Constructs C++ cannot express | 9 | Stays original code (`orig::`). Listed below with the exact construct. |
| Did not compile, or compiles with no gate case | 3 | Translator gaps; listed below. |
| Gate FAIL (decompiled code is wrong) | 43 | Hex-Rays output differs from the original; the first divergence is given. Fix in IDA (types) and re-run, or keep `orig::`. |
| Verified as decompiled, not in the clean tree | 0 | Clean-stage gap (no exported symbol to gate); the raw translation is verified. |
| Gate INCONCLUSIVE (no comparable run) | 33 | The original never returned under fuzzed inputs; needs better input construction or a live memory overlay. |

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
| `0x10281510` | sub_10281510 | unsupported construct MEMORY[ |  |
| `0x1028AB50` | sub_1028AB50 | unsupported construct retaddr |  |
| `0x102D538A` | sub_102D538A | callee _D3DXFilterTexture@16 has no C++ type (int __userpurge@<eax>(unsigned int edi0@<edi>, struct IDirect3DSurface8 *, const struct tagPALETTEENTRY *, char *, int)) |  |

## Did not compile / no gate case (3)

| Address | Name | Reason | What it does |
|---|---|---|---|
| `0x10010D20` | App_InitScene | did not compile as C++: cannot convert '_DWORD*' {aka 'unsigned int*'} to '_WORD*' {aka 'short unsigned int*'} in  | Scene/app init: creates every render target (scene A/B CAcc2 @CDx+0x190, 256x256 CAcc @CDx+0x1BC, menu-resolut... |
| `0x10011A00` | Frame_Render | did not compile as C++: typedef 'T_1032919C' is initialized (use 'decltype' instead) | Per-frame render function: TestCooperativeLevel, BeginScene, ~180 calls, EndScene, Present. 18-case state mach... |
| `0x101771B0` | sub_101771B0 | did not compile as C++: cannot convert 'char*' to 'LPCWSTR' {aka 'const short unsigned int*'} in argument passing |  |

## Gate FAIL (43)

| Address | Name | First difference | Reconstruction line at the divergence | What it does |
|---|---|---|---|---|
| `0x10009920` | CDx_SetDefaultRenderState | stubbed call #18 differs: original 0x100098E0(ecx=0x30000F50, 0x00000003, 0x30001140, 0x00000028) / reconstruc | `F_100098E0(&self->base.vftable, 3, v2, v5);` | 345 instructions of SetRenderState/SetTextureStageState defaults; called at frame start. |
| `0x1000BB90` | CDx_DrawStereoOddFrameOverlay | AL 40 vs 1 | `G_10456690 = 0;` | Stereo mode only: draws the 1x16 texture CDx+0x82C stretched as a screen-space quad (FVF 0x144, Z off) over a ... |
| `0x1000C0B0` | CDx_CompositeSceneToBackBuffer | ended differently: original return  / reconstruction fault access 0x00000000 size 4 type 19 | `F_10009E20((CDx *)self, v6, 0, 0);` | Final composite: draws scene target A over the window viewport with LINEAR filtering and MODULATE2X - this is ... |
| `0x10015510` | Camera_ApplyViewProjection | x87 result differs: (0, 0) vs (0, 0) |  | Builds and sets D3DTS_VIEW then D3DTS_PROJECTION from the camera object at 0x104568FC. |
| `0x10017310` | sub_10017310 | memory differs at 1 byte(s), first 0x10350760: original 120 / reconstruction 119 | `G_10350760 = 480.0 - (double)*(unsigned __int16 *)(a1 + 2 * v2 + 2) * 0.000015259022 * 480` |  |
| `0x1001C930` | sub_1001C930 | stubbed call #19 differs: original 0xC0000332(ecx=0x6E080300, 0x00000089, 0x00000001) / reconstruction 0xC0000 | `G_1045666C->pDevice->lpVtbl->SetVertexShader(G_1045666C->pDevice, 66u);` |  |
| `0x10029A50` | sub_10029A50 | ended differently: original return  / reconstruction fault access 0x00000000 size 1 type 21 |  |  |
| `0x1002B6E0` | sub_1002B6E0 | ended differently: original return  / reconstruction fault access 0x000000D9 size 1 type 20 |  | Per-actor model draw: resets state, transforms, fog modes, animates via Model_AnimateEntry (both branches), dr... |
| `0x1003CB30` | CMoCamera_SetPathData | ended differently: original return / reconstruction runs away (instruction limit every trial; gate timed out) |  | Copies a fixed position/direction pair plus a variable-length waypoint array (packed count field, same format ... |
| `0x10047C30` | CMoGeneratorClone_InitCloneFromSource | ended differently: original return / reconstruction runs away (instruction limit every trial; gate timed out) |  | Zeroes a block, copies transform fields (fixed + a variable-length waypoint-style array) from a source struct. |
| `0x100820B0` | CXiSkeletonActor_GetWalkSpeed | x87 result differs: (9252071032088428544, 49265) vs (9252070941396647936, 49265) |  | PS2 slot 55 -> PC slot 108. Confirmed derived, not stored: reads GetMoveSpeed's own field (+0x98) and scales b... |
| `0x10086210` | sub_10086210 | stubbed call #6 differs: original 0x100278E0(ecx=0x00000000, 0x5717C000) / reconstruction 0x100279A0(ecx=0x000 | `F_100279A0();` |  |
| `0x10086900` | sub_10086900 | stubbed call #6 differs: original 0x100278E0(ecx=0x00000000, 0x5717C000) / reconstruction 0x100279A0(ecx=0x000 | `F_100279A0();` |  |
| `0x100C52A0` | CXiSkeletonActor_Constructor_1Arg | memory differs at 1 byte(s), first 0x3000178C: original unchanged / reconstruction 1 | `*(_DWORD *)(self + 1840) = 72;` |  |
| `0x100C5560` | CXiSkeletonActor_Constructor_2Arg | memory differs at 1 byte(s), first 0x3000178C: original unchanged / reconstruction 1 | `*(_DWORD *)(self + 1868) = 0;` |  |
| `0x100C5D60` | CXiSkeletonActor_Init | memory differs at 12 byte(s), first 0x30000F94: original unchanged / reconstruction 111 |  | Init/reset. Sets opacity (+0x59C, likely XiControlActor::alpha_base) to 0 so actors fade in; color +0x660 = 0x... |
| `0x100C63D0` | CXiSkeletonActor_OnMove | ended differently: original return  / reconstruction fault access 0x00000004 size 4 type 19 | `*((float *)self + 559) = *((float *)self + 559) - F_10014CF0();` | PS2 vtable slot 5 (OnMove) -> PC slot 8. The per-frame actor update. |
| `0x100CCF50` | CXiSkeletonActor_OcclusionClip | stubbed call #3 differs: original no call / reconstruction 0x10015690(ecx=0x00000000, ) | `ViewportHeight = (float)(unsigned __int16)F_10015690();` | PS2 slot 95 -> PC 191 (0 args). Projects a screen rect from +0x8F0..+0x90C using the camera viewport and runs ... |
| `0x100CFC10` | CXiSkeletonActor_Slot244_Unknown | memory differs at 12 byte(s), first 0x8A514C9D: original 178 / reconstruction unchanged | `*(float *)(a2 + 4) = v35;` | RETRACTED as SetAction candidate (2026-09-26): the real SetAction is 0x100CEE40, found by callee identity. Thi... |
| `0x100D08E0` | CXiSkeletonActor_GetWidthScale | x87 result differs: (9717509057937408000, 16420) vs (9717509073530558464, 16420) |  | PS2 slot 34 -> PC slot 38. |
| `0x100D09B0` | CXiSkeletonActor_GetHeightScale | x87 result differs: (16982182230178660352, 16436) vs (16982182285153601536, 16436) |  | PS2 slot 35 -> PC slot 39. |
| `0x100D0A80` | CXiSkeletonActor_GetDepthScale | x87 result differs: (16297105444927700992, 16492) vs (16297105497684901888, 16492) |  | PS2 slot 36 -> PC slot 40. |
| `0x100D2F60` | CXiSkeletonActor_LoadSkeletonSync | stubbed call #4 differs: original 0x100C51C0(ecx=0x00000000, 0x00000001, 0x00000001) / reconstruction 0x100C51 | `v32 = F_100C51C0(1, v31);` | Synchronous skeleton-load path taken by CXiSkeletonActor_LoadSkeletonResource for special-case actors (vtable ... |
| `0x100D62C0` | CXiSkeletonActor_DrawResourceShadows | ended differently: original return  / reconstruction fault access 0x00000001 size 4 type 19 | `result = F_1002B5A0(self + 1652);` | AUTHOR-INFERRED, PC slot 194: for each resident resource of the model sub-object (+0x674), draws it through th... |
| `0x1011AA40` | sub_1011AA40 | ended differently: original return  / reconstruction fault access 0x000022B6 size 1 type 19 |  |  |
| `0x1011E980` | sub_1011E980 | stubbed call #8 differs: original 0x1011F300(ecx=0x30001390, 0x300013E0, 0x00000001, 0x00000000, 0x00000002) / | `*(v56 - 1) = *(_DWORD *)&v61[i + 16];` |  |
| `0x10124640` | sub_10124640 | memory differs at 4 byte(s), first 0x30000F58: original 41 / reconstruction 3 |  |  |
| `0x10162010` | sub_10162010 | ended differently: original return  / reconstruction fault access 0x00000000 size 1 type 19 |  |  |
| `0x1017C9C0` | sub_1017C9C0 | stubbed call #1 differs: original 0x10181BB0(ecx=0x00000000, 0x3003A36C, 0xC43D691C) / reconstruction no call |  |  |
| `0x1017DE20` | sub_1017DE20 | stubbed call #1 differs: original 0x10181BB0(ecx=0x00000000, 0x3003A36C, 0x444FB08E) / reconstruction no call |  |  |
| `0x10183DA0` | ZoneMap_DrawMeshById | stubbed call #1 differs: original 0x10181BB0(ecx=0x00000000, 0x30039F2C, 0x43824568) / reconstruction no call |  |  |
| `0x10185CA0` | sub_10185CA0 | ended differently: original return  / reconstruction fault access 0x00000008 size 4 type 19 | `pDevice = G_1045666C->pDevice;` |  |
| `0x101EEBD0` | sub_101EEBD0 | ended differently: original return  / reconstruction fault access 0x00000000 size 1 type 21 | `LOBYTE(v14) = F_10150CC0((void *)G_1057847C, v15, 2);` |  |
| `0x102214D0` | sub_102214D0 | stubbed call #1 differs: original 0x10220EF0(ecx=0x30001390, 0xE1CE863B, 0x00000010, 0x00000001) / reconstruct | `F_10220EF0(self, *(_DWORD *)(self + 88) + *(_DWORD *)(self + 96) / 2, *(_DWORD *)(self + 9` |  |
| `0x1024CF50` | sub_1024CF50 | stubbed call #23 differs: original 0x10271C80(ecx=0x00000000, 0x00000000, 0x00000040, 0x00000001, 0x00000040)  | `a1[11] = 0;` |  |
| `0x102501C0` | sub_102501C0 | stubbed call #12 differs: original 0x10282A30(ecx=0x00000000, 0x3D08887B) / reconstruction 0x10282A30(ecx=0x00 | `F_10282A30(v2);` |  |
| `0x10281DB0` | sub_10281DB0 | ended differently: original return  / reconstruction fault access 0xB7774858 size 4 type 19 | `G_1066941C->lpVtbl->SetTexture(G_1066941C, v2, (&G_1099D638)[10 * v3]);` |  |
| `0x10282A30` | sub_10282A30 | x87 result differs: None vs (12302071664836870144, 49167) |  |  |
| `0x10283420` | sub_10283420 | stubbed call #3 differs: original 0xC0000332(ecx=0x6E080300, 0x00000094, 0x00000001) / reconstruction 0xC00003 | `result = ((int (__cdecl *)(IDirect3DDevice8 *, signed int, float *))G_1066941C->lpVtbl->Se` |  |
| `0x10284950` | sub_10284950 | ended differently: original return  / reconstruction fault Invalid memory read (UC_ERR_READ_UNMAPPED) |  |  |
| `0x1028AFE0` | sqgxShape_1028AFE0 | ended differently: original return  / reconstruction limit  | `F_1028BB10(i);` |  |
| `0x1029B620` | sub_1029B620 | memory differs at 5 byte(s), first 0x30E70A40: original 0 / reconstruction 3 | `*v2 = *(_DWORD *)((char *)v2 + 2400 - (_DWORD)v0 + G_1099D200);` |  |
| `0x1029B9D0` | sub_1029B9D0 | ended differently: original return  / reconstruction limit  | `G_1066941C->lpVtbl->SetStreamSource(G_1066941C, v7, a1[v4 - 651], 8u);` |  |

## Gate INCONCLUSIVE (33)

| Address | Name | Original outcomes | What it does |
|---|---|---|---|
| `0x10002130` | Gfx_ProbeDeviceCaps | fault 100 | CheckDeviceType + CheckDeviceFormat probes, including DXT1/DXT3/DXT5. |
| `0x10003040` | sub_10003040 | fault 99, limit 1 |  |
| `0x10003890` | sub_10003890 | fault 100 |  |
| `0x100091B0` | CDx_Initialize | import 100 | CheckDeviceFormat x3, multisample checks x2, loads mousenor.ani. Queries shader-model support (verified D3DVS_... |
| `0x10009F20` | CDx_PushRenderTargetTexture | fault 99, limit 1 | Texture->GetSurfaceLevel(0), device->SetRenderTarget; increments stack depth CDx+0x158. |
| `0x1000A6E0` | CDx_CreateTexture | fault 100 | Sole texture-creation wrapper; format passed as variable. 10 calling functions. |
| `0x1000A820` | CDx_DrawFadeQuad | fault 100 | Global fade quad: alpha <= 0x80 darkens (fade to black), > 0x80 adds (fade to white). |
| `0x1000C7C0` | CDx_ReleaseStereoResources | fault 100 | Releases +0x814/+0x81C/+0x824/+0x82C. |
| `0x10016320` | sub_10016320 | fault 100 |  |
| `0x10017810` | sub_10017810 | fault 100 |  |
| `0x10017C60` | sub_10017C60 | fault 100 |  |
| `0x10025260` | sub_10025260 | fault 100 |  |
| `0x10033710` | sub_10033710 | fault 100 |  |
| `0x1003F440` | CMoDistMorphElem_ApplyMorph | fault 100 | The actual morph-apply function, called with the cached handle and both morph field addresses from CMoDistMorp... |
| `0x10040C20` | CMoElemFamily_SharedEarlyExit | fault 99, limit 1 | Shared no-op/early-exit path several CMoElem-family update functions tail-jump into when a flag indicates no w... |
| `0x100426F0` | sub_100426F0 | fault 100 |  |
| `0x10042D00` | CMoDistMorphElem_ValidateMorphData | fault 100 | Validates morph blend data; CMoDistMorphElem_UpdateMorphBlend early-exits if this returns false. |
| `0x100467E0` | sub_100467E0 | fault 100 |  |
| `0x10047590` | CMoSkeletonElem_UpdateBoneTransform | fault 100 | Normalizes local rotation (Euler wraparound), calls Mat4x4_Multiply to combine with the parent's matrix (confi... |
| `0x100548D0` | sub_100548D0 | fault 99, limit 1 |  |
| `0x1006C140` | sub_1006C140 | fault 100 |  |
| `0x1006C530` | sub_1006C530 | fault 100 | Draws the queried region into the 16x16 occlusion target: two CDx_DrawPrimitiveUP calls, state set directly on... |
| `0x1006E6B0` | CMoProcessor_ScratchMatrixOp | fault 99, limit 1 | CORRECTED from an earlier name (Pool_AllocSlot) that undersold what this does. Full decompilation shows a self... |
| `0x10075280` | sub_10075280 | fault 100 |  |
| `0x100AC390` | sub_100AC390 | fault 100 |  |
| `0x100C4CE0` | sub_100C4CE0 | fault 98, limit 2 |  |
| `0x100CB4E0` | sub_100CB4E0 | fault 100 |  |
| `0x100CE480` | CXiSkeletonActor_ReverseFindRes | fault 100 | PS2 XiSkeletonActor::ReverseFindRes(RES_TYPE, uint, uint, int) (0x1CF770). Called with (7, id, -1, ...) by Set... |
| `0x1016FCD0` | sub_1016FCD0 | limit 26, fault 74 |  |
| `0x1017B510` | sub_1017B510 | fault 100 | Sets D3DTS_PROJECTION plus fog table/vertex/range modes, fog color/start/end and ambient from the renderer blo... |
| `0x10183490` | sub_10183490 | fault 100 |  |
| `0x10183760` | sub_10183760 | fault 100 |  |
| `0x10280DE0` | sqgx_DrawRect_Candidate | fault 99, limit 1 |  |

