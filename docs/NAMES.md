# Names

How the graphics functions, globals and fields got their names, with the evidence for each. The IDB is the
source of truth; `data/field_names.json` holds the field names (applied by the generator) for classes that
have no IDB struct yet.

## 1 Oct 2026: function names

This round named the graphics functions that were still `sub_XXXX`: 154 in `tree/`, plus the 417 unnamed functions they call. That is 571 in all.

### Method

- **Who read the code:** eight parallel readers each read one batch. Each function came with its Hex-Rays pseudocode, callers and callees, strings, IDA comment and DancingMad entry (when it had one), and its class or source unit. They proposed a name, a one-sentence summary and the evidence for it.
- **Confidence levels:**
  - **high:** a string names the purpose, or the code is small and unambiguous;
  - **medium:** the purpose is clear from what the function calls or sets, but the exact wording is an inference;
  - **low:** the purpose was not pinned down, so no name was given.
- **Spot checks:** I compared a sample against the code myself. All matched, for example:
  - `Rand_SeedMT19937`: the Mersenne Twister seed routine `sgenrand` (LCG 69069, 624 words, seeded with 4357);
  - `CDx_PopRenderTarget`;
  - `GammaRamp_Constructor`;
  - `sqDebugPrintf`.
- **Applied in the IDB:** 262 high and 283 medium names, 26 left unnamed (low). Every one of the 571 has an IDB comment `[name ... 2026-10-01, <confidence> confidence, from reading the code] <summary> Evidence: <facts>`.
- **Collisions:** when a name already existed elsewhere (another copy of `Vec3_Add` or `Mat4_Copy`), the address was appended.
- **Kinds:** 360 semantic, 82 dtor, 60 accessor, 25 thunk, 24 mechanical, 20 ctor.
- **Truncated input:** 11 large functions were cut off at 7,000 characters of pseudocode. Their names rest on the part that was read, and they are marked in the table.

### Corrections to earlier names

- **`g_timeOfDayTintEnabled` is renamed `g_windowMode`.**
  - Values: 0 is exclusive fullscreen, 1 windowed, 2 popup, 3 popup at screen size.
  - `App_CreateMainWindow` switches on it for the window style.
  - `App_WinMain` passes `g_windowMode == 0` to `Gfx_CreateD3DHolder` as the fullscreen flag.
  - `SetGammaRamp` only runs in fullscreen.
  - In windowed modes, with no gamma ramp available, `Env_ComputeAmbient` brightens dark ambient by the gamma value instead. That is why it looked like a tint switch.
- **Where the code disagrees with DancingMad:**
  - `0x1002DC10` "CYyModelDt_PrepareRenderStateV2" also draws: it locks the vertex buffers, CPU-skins them and calls DrawIndexedPrimitive per material. It is now `CYyModelDt_SkinAndDrawV2`.
  - `0x10085060` "CXiActor_DrawResourceShadows" makes no draw calls. It starts `CMoGeneratorClone` instances for flagged resources, so it is now `CXiActor_SpawnResourceGenerators`.
  - `0x1021D4C0` "Draw2DLayer" draws the current movie frame, so it is now `CXiMovie_DrawFrame`.
  - `0x1002C2D0` `Model_AnimateEntry` keeps its name, but it loops over every model in the list and stops at the first failure. It is not a single unconditional call.
- **Wrong library names:** `unknown_libname_18`/`19`, plus two more `unknown_libname` functions, are atexit handlers. They destroy static shader objects and state blocks.

### Highlights

- Render-target stack: `CDx_PushRenderTarget`/`CDx_PopRenderTarget`, with the default back buffer and depth surface stored at CDx+0x90/+0x94.
- A gamma-ramp object sits at CDx+0x1F4 (`GammaRamp_*`). It holds requested and applied RGB gamma and three 256-entry ramps.
- Translucent models are drawn into a viewport-sized render target, then blended back (`Model_*Translucent*`).
- Projected-texture and environment-map passes generate texture coordinates from camera-space position or normals (`ProjTex_*`, `EnvMap_*`).
- The memory-area allocator is `Mem_AllocFromArea` (CYyUpper/Lower/Elem/Work/Load/Menu/ExMemory). It exits with code 2002 when an allocation fails.
- A vertex-buffer pool (`CYyVbMng_Acquire/ReleaseVertexBuffer`) and a double-buffered dynamic vertex buffer (`DynVB_*`) that locks with DISCARD/NOOVERWRITE.
- State blocks are recorded from SetRenderState/SetTextureStageState lists and applied with ApplyStateBlock.
- The sky dome is built from 8 colours and heights in the zone environment (`Sky_DrawGradientDome`).
- Stencil shadow volumes (`sqgx...`) accept only `sqshTriStrip` shapes. Planar shadows use the same matrix as D3DXMatrixShadow.
- 2D/UI: laid out for the PS2 resolution of 512x448 and scaled to a 640x480 virtual screen. The font (`FontMoji`) is a 64x128 glyph grid; the damage numbers are a sprite sheet (`dmgnum`); PS2-style blend modes and colours (128 means 1.0).
- Cursor object `dword_104E1D4C` (`Cursor_*`): hardware cursor through CDx, or a software-drawn cursor shape.
- `sqDebugPrintf` (419 callers) is `_vsnprintf` + `OutputDebugStringA`. Naming it makes hundreds of other functions easier to read.

### Every function (571)

| Address | Name | Conf. | In tree | Summary |
|---|---|---|---|---|
| `0x10001240` | `CApp_Constructor` | high |  | Constructs CApp: stores CApp_vtbl, duplicates two strings and the data-manager version string, installs a low-level keyboard hook, resets g_pCamera and creates g_pEnv. |
| `0x100014A0` | `App_ForceForegroundWindow` | high |  | Forces the game window to the foreground: attaches thread input, temporarily zeroes the foreground lock timeout, calls SetForegroundWindow and restores the timeout; skipped unless window mode is 0. |
| `0x10001520` | `CApp_RunMainLoop` | high | yes | CApp main loop: initialises CDx and input, creates the app/camera object, runs App_InitScene until done, then pumps PeekMessage and renders frames until WM_QUIT, finally running shutdown. |
| `0x100016C0` | `App_PreTranslateInputMessage` | medium |  | Per-message input pre-handler called before TranslateMessage: forwards IME messages (0x10F, 0x281-0x283), WM_CHAR/WM_ACTIVATE/WM_KEYDOWN to the input and IME objects and mouse messages (0x200-0x20A) to the mouse handler, with client-area coordinate scaling. |
| `0x10001B50` | `App_RenderFrameMaskedFPU` | medium |  | Masks all FPU exceptions, calls Frame_Render(g_pCamera), restores the FPU control word, and on success runs an update on off_103601BC; returns success. |
| `0x10001BB0` | `App_CreateInputSystem` | medium |  | Allocates a 0xDC4-byte input object into dword_10455C20, constructs it and initialises it with the HINSTANCE. |
| `0x10001CC0` | `CApp_CopyTextToClipboard` | high |  | Copies a string to the Windows clipboard as CF_TEXT using the app's HWND. |
| `0x10001E00` | `App_SetCommandLine` | high |  | Stores a heap copy of the given string in dword_103D3C88 (the command line), returning 0 on allocation failure. |
| `0x10001E10` | `App_FreeCommandLine` | high |  | Frees the command-line copy in dword_103D3C88 and clears it. |
| `0x10002040` | `Gfx_CopySurfaceRectToTexture` | medium | yes | Creates a texture the size of a rect (falling back to 512x512), copies a source-surface region into level 0 with D3DXLoadSurfaceFromSurface, passes the surface to a follow-up routine, then releases everything. |
| `0x10002460` | `App_DetectDat11` | medium |  | If 11.DAT exists in the current directory, sets dword_103D3C8C to 11. |
| `0x10002B20` | `App_CreateConfig` | high |  | Creates the 0xB4-byte startup config object (dword_103D3CB8) and turns sound off (dword_10456A88=0) if the '-<id>sound' command-line switch contains 'off'. |
| `0x10002BC0` | `App_DestroyConfig` | high |  | Destroys the config object dword_103D3CB8 through its virtual deleting destructor and clears the pointer. |
| `0x10002BE0` | `Gfx_IsGammaSupported` | medium | yes | Returns CDx byte +0x915 (0 if there is no CDx); the flag that gates gamma-ramp save and restore. |
| `0x10002C00` | `Gfx_SetGammaRGB` | high | yes | Sets the R/G/B gamma values of the CDx gamma-ramp object. |
| `0x10002C30` | `Gfx_SetGammaR` | high | yes | Sets only the red gamma component, keeping green and blue. |
| `0x10002C80` | `Gfx_SetGammaG` | high | yes | Sets only the green gamma component, keeping red and blue. |
| `0x10002CD0` | `Gfx_SetGammaB` | high | yes | Sets only the blue gamma component, keeping red and green. |
| `0x10002D20` | `Gfx_GetGammaRGB` | high | yes | Reads the three gamma components into the caller's out-pointers. |
| `0x10002D80` | — | low |  | Returns 1.0 if byte_10487EF5 is set, otherwise returns the float global flt_10456A68. |
| `0x10003040` | `Gfx_DestroyD3DHolder` | high |  | Releases and deletes g_pD3DHolder through its virtual deleting destructor and clears it. |
| `0x10003070` | `D3DHolder_Constructor` | high |  | Constructs a D3DHolder: sets the vtable and zeroes the D3D/device pointers and the 0x42C-byte adapter-identifier buffer. |
| `0x10003110` | `D3DHolder_Initialize` | high |  | Runs Gfx_D3DBootstrap; on success queries GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL) into this+0x48. On failure sets error code 2000 or 2001 depending on device caps. |
| `0x10003460` | `D3DHolder_CheckDepthFormat` | high |  | Checks that a depth-stencil format is usable with the given adapter/render-target formats on adapter 0 HAL. |
| `0x10003B60` | `Debug_DrawTextTable` | medium | yes | Sets up alpha-blended, fog-off text render states, then draws each active entry of a 2304-entry debug text table with 0x10003D70, and restores the Z states. |
| `0x10003D70` | `Debug_DrawWorldText` | medium | yes | Draws a text string as textured glyph quads at a given world transform: builds the world matrix, looks up glyphs in font dword_104E1BF8, handles newlines, and calls DrawPrimitiveUP. |
| `0x10009120` | `CDx_Destructor` | high |  | CDx destructor body: restores CDx_vtbl, deletes two state blocks, releases stereo resources, and destroys member sub-objects (CAcc array, etc.). |
| `0x10009620` | `CDx_SetDefaultRenderTargets` | high | yes | Stores the default back-buffer and depth-stencil surfaces in CDx (+0x90/+0x94) and returns 1. |
| `0x10009640` | `CDx_SetClearParams` | high | yes | Stores Clear() parameters (flags, color, Z, stencil) in CDx fields +0x164..+0x170. |
| `0x10009670` | `CDx_SetClearColor` | high | yes | Sets only the stored clear colour (CDx +0x168). |
| `0x10009780` | `CDx_ShowCursor` | high |  | Shows or hides the mouse cursor: uses IDirect3DDevice8::ShowCursor when a hardware cursor is active, otherwise SetCursor(NULL), depending on window mode. |
| `0x100097F0` | `CDx_SetCursorShape` | medium | yes | When the hardware cursor is enabled, maps a cursor state (0-6) to one of the cached HCURSOR slots at CDx+0xA04, loading IDC_ARROW on first use, and calls SetCursor. |
| `0x1000A290` | `CDx_PopRenderTargetDefault` | high |  | Calls CDx_PopRenderTarget (0x1000A2B0) with the extra two arguments set to 1. |
| `0x1000A2B0` | `CDx_PopRenderTarget` | high |  | Pops the CDx render-target stack: decrements the depth byte, rebinds the previous colour target (texture surface level 0 or surface) and depth surface, or the defaults at depth 0, then restores the saved viewport. |
| `0x1000A4F0` | `CDx_DrawTextureToViewport` | medium |  | Draws a texture (for example the scene render target) as a screen quad covering the current viewport, with full UVs and the given diffuse colour. |
| `0x1000A5A0` | `CDx_DrawTexturedScreenQuad` | high |  | Draws a pre-transformed textured quad: builds 4 XYZRHW\|DIFFUSE\|TEX1 vertices from a pixel rect (with -0.5 offset), UV rect and colour, binds the texture to stage 0 and draws a triangle strip. |
| `0x1000A800` | `CDx_PushRenderTargetRet1` | high | yes | Calls CDx_PushRenderTarget with the same arguments and returns 1. |
| `0x1000AD70` | `CDx_CreateDepthSurfaceLikeMain` | high |  | Creates a depth-stencil surface of the requested size using the format and multisample type of CDx's main depth surface; returns it or 0. |
| `0x1000AE40` | `CDx_StepFadeCounter` | medium |  | Fade counter at CDx+0x174: mode 0 resets it to 128.0; mode 1 subtracts a2*a3 (minimum 0.01) and returns 1 once it reaches 0. |
| `0x1000D040` | `CAcc2_Constructor` | high |  | Constructs a CAcc2 object: sets CAcc2_vtbl and zeroes six fields. |
| `0x10010370` | `App_RegisterSystemDats` | medium |  | Registers about 18 named system DAT resources (IS/IS2, BS, CL, TI, MB, SB, SK, CA, WR..WR8, B2) with their handles through 0x1021C070, then sets flag bit 2 on dword_104E0AF4. |
| `0x10010CE0` | `App_SetSeqStep` | high |  | Sets the sequence step (word +4) and clears the sub-counter (word +6). |
| `0x10010D00` | `App_AdvanceSeqStep` | high |  | Increments the sequence step word (+4) and clears the sub-counter (+6). |
| `0x10013AD0` | `App_StepGameState5Connect` | medium | yes | Step function for Frame_Render game state 5: sets up fog render states, initialises game systems and starts the lobby thread, polls the connection (updating the zone map around the player), then runs a 60-frame countdown before finishing. |
| `0x10013DF0` | `App_StepGameState6` | medium | yes | Frame_Render state-6 sequence: registers system DATs, loads the 'menu chdummy' resource, sets clear params, resets game data, then issues requests 6, 7 and 9 to off_103601BC, waiting for each to finish (>200 = failure). |
| `0x10013F10` | `App_StepGameState7` | medium | yes | Frame_Render state-7 sequence: like state 6 but without the DAT registration, issuing requests 6, 8 and 9. |
| `0x10014030` | `App_StepGameState8` | medium | yes | Frame_Render state-8 sequence: registers DATs and resets data like state 6, skips request 6, then issues requests 8 and 9. |
| `0x10014120` | `App_StepGameState9` | medium | yes | Frame_Render state-9 action: registers DATs, loads 'menu chdummy', sets clear params and resets game data in one call; returns the reset result. |
| `0x100141C0` | — | low | yes | Substep of the state-10 handler: copies config+8 (+1) into word_104568F8, ticks the CDx fade counter and advances the sequence step. |
| `0x10014C10` | — | low |  | Shutdown or teardown helper run when the main loop exits: calls several subsystem routines and returns true only if two later checks succeed. |
| `0x10014CF0` | `Camera_GetFrameScale` | medium |  | Returns the float at g_pCamera+0x28, clamped to a minimum of 1.0. |
| `0x10014D70` | `Camera_GetFloat2DC` | high |  | Returns the float at g_pCamera+0x2DC. |
| `0x10014D80` | `Camera_GetFloat2E0` | high |  | Returns the float at g_pCamera+0x2E0. |
| `0x10014D90` | `Camera_SetClearColorRGB` | medium |  | Sets g_pCamera's background clear colour (+0x308) from r,g,b bytes with alpha 0x80. |
| `0x100150D0` | `Camera_SetViewMatrix` | medium |  | Passes a 4x4 matrix by value to 0x10015120 on g_pCamera, which copies it into several matrix slots (+0x54/+0x94/+0xD4), resets a vector and builds a derived matrix at +0x294. |
| `0x100151F0` | `Camera_SetMatrix154` | medium |  | Passes a 4x4 matrix by value into g_pCamera+0x154. |
| `0x10015220` | `Camera_GetViewMatrixPtr` | medium |  | Returns a pointer to the matrix at g_pCamera+0x94. |
| `0x10015250` | `Camera_GetField50` | high |  | Returns the dword at g_pCamera+0x50. |
| `0x10015270` | `Camera_GetMatrix154Ptr` | high |  | Returns a pointer to the matrix at g_pCamera+0x154. |
| `0x100157E0` | `App_WinMain` | high | yes | Process-level startup and shutdown: reads the command line and config, picks the data path, checks the OS, creates the 'FinalFantasyXI' window, IME/keyinput DLLs and D3D holder (retrying), inits sound and runs the app, then tears everything down in reverse. |
| `0x10015AB0` | `App_RunWithCOM` | medium |  | Sets the process priority class, masks the FPU, calls CoInitialize, creates a helper object, creates the renderer/app and runs it, then cleans up and calls CoUninitialize. |
| `0x10015CB0` | `App_InitDataPaths` | medium |  | Creates the data/ROM path manager (dword_10662DCC) according to the install mode in the config: current directory, the registry path, or the dev path 'C:\image\ffxi\config\data'. |
| `0x10015F50` | `App_DestroyDataPaths` | high |  | Shuts down and deletes the data path manager dword_10662DCC. |
| `0x10015F80` | `App_CheckOSVersion` | high |  | Requires Windows 98 or later (4.10) or NT 5.0 or later via 0x10015FC0, plus a second check; sets error 6 or 7 and returns 0 on failure. |
| `0x100160C0` | `App_IsRegistryFlagSet` | medium |  | Opens an HKLM key chosen by index (0-2) from the lpSubKey table and returns true if its DWORD value named by a0001 is nonzero. |
| `0x10016150` | `App_InitSoundSystem` | high |  | Initialises the sound system with the main HWND and the current directory. |
| `0x100161B0` | `App_CreateRendererAndApp` | high | yes | Creates CDx (2640 bytes) from the D3DHolder device, stores the back-buffer size and default back-buffer/depth surfaces, constructs CApp (dword_103D3C64), begins the timer period and creates helper objects. |
| `0x10016400` | `App_SetGlobal_104569C8` | high |  | Stores its argument in dword_104569C8. |
| `0x10016490` | `App_SetGlobal_104569E8` | high |  | Stores its argument in dword_104569E8. |
| `0x100164C0` | `App_SetErrorCode` | medium |  | Stores a startup error code in dword_104569F8. |
| `0x100165B0` | `App_CreateMainWindow` | high |  | Registers the window class (WndProc 0x10016870), picks style and size from the window mode (1 windowed, 2 popup, 3 popup at full screen size), creates the window, removes Close from the system menu and adds a localised menu item from a resource DLL. |
| `0x10016820` | `App_DestroyMainWindow` | high |  | Destroys the main window, unregisters its class and drains the message queue. |
| `0x10016E40` | `App_LoadImeUiDll` | high |  | Loads the IME UI DLL from a directory, resolves ImeUi_Initialize/Uninitialize/SetState and calls ImeUi_Initialize(hwnd,0). |
| `0x10016F10` | `App_UninitImeUi` | high |  | Calls ImeUi_Uninitialize if the IME UI DLL is loaded. |
| `0x10016F40` | `App_EnableImeUi` | high |  | Calls ImeUi_SetState(1) if the IME UI DLL is loaded. |
| `0x10016F60` | `App_LoadKeyInputDll` | high |  | Loads the key-input helper DLL from a directory and resolves its 'KeyInput' export. |
| `0x10016FF0` | `App_UnloadKeyInputDll` | high |  | Frees the key-input helper DLL if it is loaded. |
| `0x10017030` | `GammaRamp_Constructor` | high |  | Constructs the CDx gamma-ramp object: vtable off_10329E88, requested RGB gamma 1.5 and applied RGB 1.0, then builds the initial ramps. |
| `0x100170A0` | `GammaRamp_SetRGB` | high |  | Stores the three requested gamma values (+1540/+1544/+1548). |
| `0x100170D0` | `GammaRamp_GetRGB` | high |  | Reads the three requested gamma values into out-pointers. |
| `0x10017100` | `GammaRamp_GetChannelSamples` | medium |  | Returns one sample from each of the R, G and B ramp tables, scaled to floats by 1/28494. |
| `0x10017160` | `GammaRamp_BuildRamps` | medium |  | Builds the R, G and B 256-entry gamma tables from the applied gamma values. |
| `0x10017230` | `GammaRamp_ApplyIfChanged` | high | yes | If the requested RGB gamma differs from the applied value, copies it over, rebuilds the ramps and calls SetGammaRamp (fullscreen mode 0 only). |
| `0x100177A0` | `CShader_Destructor` | high | yes | CShader destructor body: restores CShader_vtbl, releases the member at +8 and deletes the vertex shader handle at +0x10. |
| `0x100186C0` | `CXiOpening_Constructor` | high | yes | Constructs CXiOpening: saves the current gamma and forces it to 1.0 when supported, copies the name, and creates the subtitle (region-dependent) and helper sub-objects. |
| `0x100187D0` | `CXiOpening_Destructor` | high | yes | CXiOpening destructor body: deletes the two sub-objects and restores the saved gamma values. |
| `0x10018A50` | `OpeningSubtitle_Constructor` | medium |  | Constructs the opening-subtitle object: picks a mesh/resource id by region (39531/55747/56167), looks it up in g_pMeshResourceMng, and loads the 'screens subtitle' font texture. |
| `0x10018B30` | `OpeningSubtitle_Reset` | medium |  | If the subtitle resource is loaded, resets its two progress fields (+0xC/+0x10). |
| `0x10018C50` | `Vt10329FE0_Constructor` | medium |  | Constructs a small CXiOpening helper object (vtable off_10329FE0) and zeroes its state. |
| `0x10018CE0` | `Vt10329FE0_Reset` | medium |  | Zeroes the helper object's state fields (+4..+0x1C, +0x34, +0x38) and returns 0. |
| `0x10019140` | `App_BeginTimerPeriod` | high |  | Sets the multimedia timer resolution to the smallest supported period (at least 1 ms) and records it in uPeriod. |
| `0x100191D0` | — | low |  | Allocates a 32-byte object, constructs it with 0x10019490 and calls its virtual slot 7 (init); returns it or null. |
| `0x1001A660` | `CYyMotionQue_SetSlotMotion` | medium |  | Sets motion slot a9 (0-6, 5 dwords each) of a motion queue for an actor, skipping it when the actor rejects motion, and notifies the actor for slot 0. |
| `0x1001D0A0` | `Model_CreateTranslucentTargets` | medium | yes | Creates a viewport-sized render-target texture for translucent model drawing, plus a second one when g_pEnv+24 (stereo) is set. |
| `0x1001D240` | `Model_BeginTranslucentPass` | medium | yes | If model alpha is below opaque, pushes the offscreen model render target (and the stereo one) with the matching depth surface and clears it, marking the pass active. |
| `0x1001D350` | `Model_TranslucentUnbindTexture` | medium | yes | When the translucent model pass applies, unbinds texture stage 0 and returns 1. |
| `0x1001D3B0` | `Model_CompositeTranslucentPass` | medium | yes | Ends the translucent model pass: draws the offscreen texture as a full-target quad tinted by the model alpha colour with alpha blending, including a stereo copy, then restores Z/alpha states. |
| `0x1001D940` | `TexObj_InitFromHeader` | medium | yes | Fills a texture object descriptor from an image header: copies 16-byte name, width/height/bpp, palette and pixel pointers, flags 'cubemap ' textures, and tags DXT-style formats when the device supports them. |
| `0x1001DC70` | `Tex_DrawNamedTexturePreview` | medium | yes | Looks up a texture by its 16-char name in g_pTextureMng, binds it to stage 0 and draws it as a 256x256 screen quad at (x, y+16). |
| `0x1001DD00` | `Gfx_DrawScreenQuad256` | high | yes | Draws a 256x256 pre-transformed textured quad at (x,y) with Z and alpha test disabled, then restores them. |
| `0x1001E0A0` | `Tex_HeightToNormalMap` | medium |  | Converts an 8-bit heightmap into a packed normal map using Sobel-style neighbour gradients with wrap-around, writing one 32-bit texel per pixel. |
| `0x1001E470` | `CYyCamMng2_CreateGlobal` | medium |  | Allocates and constructs a 280-byte object, stores it in global dword_10456D6C, initialises it and returns the result of sub_10015100 on it. |
| `0x1001E4B0` | `CYyCamMng2_CallGlobalVfunc1` | medium |  | If the global object dword_10456D6C exists, calls its vtable slot 1 with &unk_10351224. |
| `0x10022120` | `Actor_GetPercentScaleByIndex` | medium |  | Returns byte this[10+idx] (idx<4, else asserts and uses 0) as a percentage scale factor: 0xFF means 1.0, otherwise value*0.01. |
| `0x10022190` | `Actor_LookupPercentScaleA` | medium |  | Looks up an entry via sub_10022170 and returns its signed byte at +10 times 0.01, or 1.0 if not found. |
| `0x100221E0` | `Actor_LookupPercentScaleB` | medium |  | Looks up an entry via sub_10022170 and returns its signed byte at +11 times 0.01, or 1.0 if not found. |
| `0x10022230` | `Actor_InitIndexTable` | medium |  | Runs base init sub_100220E0 then fills byte slots +16..+19, +20..+35 and +36..+37 with 0xFF (unset) and returns this. |
| `0x10022270` | `Actor_SetPair14` | high |  | Stores two dwords from a2 into this+0x14 and this+0x18. |
| `0x10022300` | `Actor_SetPair1C` | high |  | Stores two dwords from a2 into this+0x1C and this+0x20. |
| `0x10022340` | `Actor_LookupByteField1C` | medium |  | Looks up an entry via sub_10022320 and returns its byte at +28, or 0 if not found or 0xFF. |
| `0x10022FA0` | `CYyModelDt_SetVertexShader` | high | yes | Caches a vertex shader/FVF handle in g_vertexShader_6E4C and sets it on the device when non-zero. |
| `0x10023010` | `CYyModelDt_ExpandTriList` | medium |  | Expands a list of indexed triangles (15-word records: 3 vertex indices + per-corner UVs) into a 32-byte-stride vertex array, returning the vertex count; skipped when flag bit +52&1 is set. |
| `0x100230D0` | `CYyModelDt_ExpandTriStrip` | medium |  | Expands an indexed strip (first triangle with 3 corners, then one new vertex+UV per 5-word record) into a 32-byte-stride vertex array, returning the vertex count. |
| `0x100250C0` | `CYyModelDt_Destructor` | medium |  | Destructor: stores CYyModelDt vtable, frees the buffer at +0x40, then runs the base destructor sub_10029CD0. |
| `0x100252F0` | `ProjTex_Destructor` | medium |  | Destructor for the projected-texture helper (vtable off_1032A460): releases the COM texture at +4 and surface at +8. |
| `0x10025330` | `ProjTex_BeginPass` | medium | yes | Sets up an additive projected-texture pass: binds the helper's texture, saves the material textures and colour, and generates stage-0 coords from camera-space position through the supplied matrix. |
| `0x100255C0` | `ProjTex_EndPass` | medium | yes | Restores the normal material pass after the projected-texture pass: rebinds the saved base textures to stages 0/1 and resets blend, Z write, addressing, texgen and the stage-1 combiner. |
| `0x10025860` | `TexObj_GetD3DTexture` | high |  | Returns the D3D texture at +0x40, or the fallback one at +0x44 if the first is null. |
| `0x10025870` | `ProjTex_CaptureToTarget` | medium | yes | Stores the projection parameters in globals and copies a source texture into the helper's own render target with a full-target screen quad. |
| `0x10025C40` | `EnvMap_BeginPass` | medium | yes | Sets up an additive environment-map pass: binds the global env texture, saves base textures, and generates stage-0 coords from camera-space normals through a 2D texture matrix. |
| `0x10025F70` | `EnvMap_EndPass` | medium | yes | Restores the normal material pass after the environment-map pass: rebinds saved textures, re-enables lighting and Z write, clears WRAP0, blend and texgen. |
| `0x100262C0` | `Dbg_DrawGroundGrid` | high | yes | Draws a 17x17 line grid spanning -8..8 on the XZ plane with an identity world transform. |
| `0x100263F0` | `Dbg_DrawViewAxisGizmo` | medium | yes | Draws red/green/blue X/Y/Z axis lines placed 4 units in front of the camera using the inverted view matrix, then restores the world matrix. |
| `0x100266C0` | `Dbg_DrawAxesAt` | medium | yes | Draws red/green/blue unit X/Y/Z axis lines at the given world position, then restores the world matrix. |
| `0x10026890` | `CYyIcon_Constructor` | high |  | CYyIcon constructor: sets vtable, initialises a 4-vertex screen quad (rhw 1, white, UVs), builds a TexObj from the icon image data and queues it in the texture manager. |
| `0x10026990` | `CYyIcon_Draw` | medium | yes | Draws the icon's texture as a screen quad at (x,y) sized to the texture with the given colour, optionally with alpha blending. |
| `0x10026C70` | `CYyIcon_DrawSubRect` | medium | yes | Draws a sub-rectangle of the icon's texture (source x,y,w,h) to a destination rectangle on screen with the given colour. |
| `0x10026E10` | `Quat_SetIdentity` | medium |  | Sets a 4-float vector to (0,0,0,1), i.e. an identity quaternion. |
| `0x10026E30` | `Vec4_Set` | high |  | Sets a 4-float vector from four arguments. |
| `0x10026E90` | `Vec4_Copy` | high |  | Copies 4 dwords from a2 to a1. |
| `0x10026FA0` | `Vec3_Add` | high |  | a1 = a2 + a3 componentwise for 3 floats (returns a2). |
| `0x10026FD0` | `Vec2_Add` | high |  | a1 = a2 + a3 componentwise for 2 floats. |
| `0x10027050` | `Vec2_AddScalar` | high |  | Adds a scalar to both components of a 2-float vector in place. |
| `0x10027120` | `Vec3_Sub` | high |  | a1 = a2 - a3 componentwise for 3 floats. |
| `0x100272E0` | `Vec3_Scale` | high |  | a1 = a2 * s for a 3-float vector. |
| `0x10027310` | `Vec3_MulInPlace` | high |  | Componentwise multiply of a1 by a2 in place (3 floats). |
| `0x100273D0` | `Vec2_Mul` | high |  | a1 = a2 * a3 componentwise for 2 floats. |
| `0x100276F0` | `Gfx_ProjectWorldToClip` | medium | yes | Transforms a point by the current World*View*Projection matrices with homogeneous divide. |
| `0x100277E0` | `Gfx_TransformWorldToView` | medium | yes | Transforms a 4D point by the current World*View matrix. |
| `0x10027870` | `Gfx_ProjectViewToClip` | medium | yes | Transforms a view-space point by the current projection matrix with homogeneous divide. |
| `0x100278E0` | `Gfx_NdcToScreen` | medium | yes | Converts normalised device X/Y (-1..1) to pixel coordinates of the current render target in place. |
| `0x10027930` | `Gfx_ScreenDeltaToNdc` | medium | yes | Scales a pixel-space 2D vector into NDC units using the current viewport size (y flipped). |
| `0x100279B0` | `Mat4_SetIdentity` | high |  | Sets a 4x4 float matrix to identity. |
| `0x10027A60` | `Mat4_PostScale` | medium |  | Multiplies the matrix (in ECX) on the right by a diagonal scale matrix built from a 4-vector. |
| `0x10027B30` | `Mat4_SetScale` | high |  | Sets the matrix to a scale matrix with diagonal (s.x, s.y, s.z, 1). |
| `0x10027B80` | `Mat4_PostRotateX` | high |  | Multiplies the matrix on the right by a rotation about X by the given angle. |
| `0x10027BD0` | `Mat4_PostRotateY` | high |  | Multiplies the matrix on the right by a rotation about Y by the given angle. |
| `0x10027C20` | `Mat4_PostRotateZ` | high |  | Multiplies the matrix on the right by a rotation about Z by the given angle. |
| `0x10027CD0` | `Mat4_SetRow3` | high |  | Copies a 4-vector into the fourth row (translation row incl. w) of the matrix. |
| `0x10027CF0` | `Mat4_SetTranslation` | high |  | Copies a 3-vector into the translation row of the matrix. |
| `0x10027D10` | `Mat4_PostMultiply` | high |  | this = this * a2 for 4x4 row-major matrices. |
| `0x10027E90` | `Mat4_PreMultiply` | high |  | this = a2 * this for 4x4 row-major matrices. |
| `0x10028140` | `Mat4_TransformVec4InPlace` | medium |  | Transforms a 4-vector by the matrix in place (via a temp copy). |
| `0x10028200` | `Mat4_TransformPointInPlace` | medium |  | Transforms a 3D point by the matrix in place (via a temp copy). |
| `0x10028290` | `Mat4_TransformNormalInPlace` | high |  | Transforms a 3D direction by the matrix's 3x3 part in place (via a temp copy). |
| `0x100282C0` | `Mat4_TransformNormal` | high |  | out = in * M3x3: transforms a direction vector by the upper 3x3 of the matrix (no translation). |
| `0x10028320` | `Mat4_TransformCoordInPlace` | high |  | Transforms a 4-vector by the matrix and divides x,y,z by w (0 if w==0). |
| `0x100283C0` | `Mat4_InvertOrthonormal` | high |  | Inverts a rigid (rotation+translation) matrix in place by transposing the 3x3 and setting translation to -R^T*t. |
| `0x10028640` | `Mem_AllocObject` | medium |  | Allocates size+4 bytes from the given memory area, writes a CYyObject vtable tag in the first dword and returns the pointer after it. |
| `0x100286B0` | `Mem_AllocWithFallback` | medium |  | Allocates from the given memory area; for the elem/work areas (2,3) falls back to the upper area 0, exiting with code 2002 if that also fails. |
| `0x10028700` | `Mem_FreeObject` | high |  | Frees a block allocated by Mem_AllocObject by passing the pointer minus the 4-byte tag to the pool free. |
| `0x10028B90` | `MemBlock_GetSize` | medium |  | Returns the payload size of a heap block as next-block address minus this minus the 32-byte header, or 0 for the last block. |
| `0x10028FD0` | `Mem_AllocFromArea` | high |  | Allocates a block from one of the named memory areas (upper, lower, elem, work, load, menu, ex), tags its 32-byte header with the area class vtable and returns the payload. |
| `0x100295A0` | `Mem_DrawHeapUsageBar` | medium | yes | Debug overlay: walks a heap block list and draws one coloured screen-space bar segment per block with width proportional to block size (512 px total). |
| `0x10029CD0` | `CYyModelDtBase_Destructor` | medium |  | Base destructor in the CYyModelDt hierarchy: sets base vtable, releases the resource at +8, deletes the owned object at +4 and destroys the member at +16. |
| `0x10029DD0` | `CYyModelBase_Destructor` | high |  | CYyModelBase destructor: sets vtable, releases its resource, deletes owned objects at +4 and +0x20, destroys two 7-element member arrays and the list at +8. |
| `0x10029E50` | `CYyModelBase_SetResourceAndRebuild` | medium |  | Stores a resource handle at +0xC, refreshes the list at +8 and rebuilds the model. |
| `0x10029E70` | `CYyModelBase_GetPartList` | medium |  | Returns the address of the part list at this+0x20. |
| `0x10029F70` | `CYyModelBase_AddPartFromResource` | medium |  | Creates a model part for a resource handle (60-byte CYyModelDt for type 0, 456-byte object for type 1), links it into the model and rebuilds. |
| `0x1002A3B0` | `CYyModelBase_DrawLit` | medium | yes | Draws all model parts with scene lighting: sets ambient and up to N lights from the light table, ensures per-part dynamic vertex buffers, calls each part's draw method, then disables lights 0-3. |
| `0x1002AC50` | `CYyModelBase_ForwardToField24` | medium |  | Forwards a2 to sub_1001B0B0 on the member at this+0x24 (first element of the 7-entry array). |
| `0x1002B4B0` | `CYyModel_Constructor` | high |  | CYyModel constructor: constructs members, sets the CYyModel vtable and zeroes/initialises fields. |
| `0x1002B550` | `CYyModel_Destructor` | high |  | CYyModel destructor: sets vtable, deletes the owned object at +0x44 and destroys members in reverse order. |
| `0x1002B5A0` | `CYyModel_GetField44Addr` | medium |  | Returns the address of the field at this+0x44. |
| `0x1002B6A0` | `CYyModel_CreateModelBase` | medium |  | Allocates and constructs a 200-byte model-base object and attaches it to this model. |
| `0x1002B6E0` | `CXiSkeletonActor_DrawModel` | medium |  | Per-actor model draw: caches transforms, sets material colour, optional env-map and projected-screen-texture setup, default render states and fog, then animates and draws the model. (pseudocode truncated) |
| `0x1002C2D0` | `CXiSkeletonActor_AnimateModels` | medium |  | Walks the actor's model list (sub_1002B5A0) calling Model_AnimateAndPose on each; if all succeed sets flags(+0x30)\|=0x12 and returns 1, else 0. |
| `0x1002C360` | `CXiSkeletonActor_PrepareModelsOnce` | medium | yes | Once per update (guarded by flag 4 at this+48) calls sub_1002A3B0 on every model in the actor's list, then resets texture stage 1 TEXTURETRANSFORMFLAGS/TEXCOORDINDEX to 0. |
| `0x1002C3E0` | `CXiSkeletonActor_GetElemThunk` | medium |  | Forwards (this,a3,a4,a5) to sub_1002C400, dropping the other arguments. |
| `0x1002C570` | `CXiSkeletonActor_GetModelEidMatrix` | medium |  | If the actor is posed (flag 0x10), finds the a2-th model in its list and returns sub_1002A830(model,a3,a4). |
| `0x1002C5C0` | `CXiSkeletonActor_GetModelElem` | medium |  | Finds the a3-th model in the actor's model list and returns sub_1002A9B0(model, idx, a2) (an element pointer). |
| `0x1002C7C0` | `List_GetTail` | high |  | Follows the next pointer at node+4 until null and returns the last node (null-safe). |
| `0x1002C890` | `Mem_AllocDefault` | medium |  | Allocates a1 bytes via sub_10028FD0 with fixed pool/flag argument 4. |
| `0x1002CCB0` | `CYyOs2VtxBuffer_CreateBuffersStep` | high | yes | Incremental state machine (state at this+60) that allocates a 0xFFFF-filled index shadow, creates a D3D INDEX16 index buffer, then acquires a pooled vertex buffer; on failure frees everything. Returns 1 when finished. |
| `0x1002D680` | `CYyOs2VtxBuffer_Constructor` | high | yes | Calls base ctor, stores CYyOs2VtxBuffer_vtbl, zeroes buffer fields and creates the vertex shader from g_vsDecl_1628. |
| `0x1002D700` | `CYyOs2VtxBuffer_Destructor` | high | yes | Restores the vtable, frees the index shadow, releases the index buffer and pooled VB, deletes the vertex shader, frees other heap blocks, removes itself from a 16-slot global table and calls the base dtor. |
| `0x1002DC10` | `CYyModelDt_SkinAndDrawV2` | medium | yes | Locks the model's position/normal VBs (Sleep(0) retry), runs CPU skinning (sub_1002F1D0), unlocks, then sets texture-factor/stage state and walks the model's command stream issuing SetStreamSource/SetIndices/DrawIndexedPrimitive per material. (pseudocode truncated) |
| `0x1002F130` | `Gfx_SetStreamSourceCached` | high |  | Caches the vertex buffer and stride for stream a1 in global arrays and applies it to the device. |
| `0x1002F160` | `Gfx_ApplyCachedStreamSource` | high | yes | If a vertex buffer is cached for stream a1, calls IDirect3DDevice8::SetStreamSource with it and the cached stride. |
| `0x1002F1A0` | `Gfx_ApplyCachedVertexShader` | high | yes | Calls SetVertexShader with the cached global vertex shader handle if non-zero. |
| `0x1002F1C0` | `Gfx_SetVertexShaderCached` | high |  | Stores a vertex shader handle in the global cache and applies it. |
| `0x1002F1D0` | `Model_SkinVerticesCpu` | medium |  | Software skinning: for each single-bone and two-bone-weighted vertex group, transforms positions/normals by bone matrices from the bone table (64-byte stride) into the locked VB pointers; separate path when model flag bit 0 is set. (pseudocode truncated) |
| `0x10032FA0` | — | low |  | Builds a temporary matrix from this (sub_10032FE0) and combines it into a2 via sub_10027D10. |
| `0x10033800` | `Model_BeginOffscreenPass` | medium | yes | When the effect mode is active and a render target exists, saves two texture pointers and a texture factor, pushes this+8 as render target (clearing on first use), sets flat constant-color unlit/no-Z state and a narrow 11.25-degree perspective projection plus view/world transforms. |
| `0x10033A70` | `Model_EndOffscreenPassBindTextures` | medium | yes | Sets stage 0/1 blend ops (variant chosen by a2 in 1..2), restores the saved texture factor and, in the offscreen mode, the scene transforms and Z/fog/lighting states, then binds the two textures saved by the begin call to stages 0 and 1. |
| `0x10033EA0` | `Shadow_SmoothLightDirection` | medium |  | Frame-rate-scaled lerp of a stored direction (this+8) toward a2 (defaulting to (0.45,-0.9,0) if a2 points up), clamps its elevation to at least 60 degrees below horizontal, then lerps a second vector (this+5) toward it and normalizes. |
| `0x10035B00` | `CMoTask_VT_1032A904_Destructor` | high |  | Restores the vtable, and for task kind 1/2/4/8 (this+52) calls the matching finish function with this+68 and clears that kind's global singleton slot before calling the base dtor. |
| `0x10035F00` | `MoTask_KillAllGlobalSlots` | medium |  | Deletes the four global task singletons (via vtable+24 deleting dtor) and then calls the four per-slot finish functions with a1. |
| `0x10036070` | `MoTask_KillGlobalSlot1` | medium |  | Deletes the kind-1 global task singleton if present and calls its finish function sub_100360A0(a1). |
| `0x100384B0` | `Font_QueueText` | high |  | Appends a text command (optional scale opcode 3, x, y, zero-terminated string) to the global text queue later drawn by Font_DrawPass. |
| `0x10038520` | `Font_DrawPass` | high | yes | Sets 2D alpha-blended, no-Z/fog/lighting state, draws every queued text string via Font_DrawString, clears the queue and restores blend/Z. |
| `0x10038720` | `Font_DrawString` | high | yes | Draws a string glyph by glyph as textured quads from a 64x128-cell font atlas, advancing x by glyph width and handling newline (16px*scale line height). |
| `0x10038930` | `Post_CreateScreenEffectTargets` | medium | yes | Creates a 256x256 render-target texture, and a second one when the env flag at g_pEnv+24 is set (stereo), storing them in globals. |
| `0x10038A80` | `Post_CaptureSceneToTarget` | medium | yes | Copies the current scene texture into the 256x256 effect target with a tinted full quad (alpha blended unless tint alpha is 0x80), plus a second eye copy in stereo mode. |
| `0x10038E10` | `Post_DrawRotatingGhostLayers` | medium | yes | Draws the captured effect texture back over the screen multiple times, each pass offset on a circle (angle advancing with time, radius and color from a table), with the last pass alpha-tested and opaque. (pseudocode truncated) |
| `0x10039810` | `Util_Copy16Bytes` | high |  | Copies 16 bytes from a2 to this. |
| `0x100398D0` | `CYyTexBase_Destructor` | high |  | Restores the CYyTexBase vtable (empty base destructor). |
| `0x100398E0` | `CYyTexBase_SetName` | medium |  | Copies a 16-byte name/key from a2 into this+12. |
| `0x10039970` | `CYyTex_Destructor` | high |  | Restores the vtable, unlinks the texture from g_pTextureMng's list, releases its two D3D textures (+64,+68), and calls the base dtor. |
| `0x1003AA90` | `CYyTexMng_Destructor` | high |  | Restores the vtable and deletes the owned object at this+8 via its deleting destructor. |
| `0x1003AC80` | `CYyTexMng_FindByName` | high |  | Walks the texture manager's linked list and returns the first texture (not flagged 8) whose 16-byte name at +12 equals a2. |
| `0x1003AF10` | `CYyTexMng_FindByNameInResource` | medium |  | Walks a resource chain (type 32 lookups) to find the resource whose 16-byte name matches a2, then returns the texture in the manager list owned by that resource. |
| `0x1003B210` | `CYyVbMng_AcquireVertexBuffer` | high | yes | Rounds the size up to 2000 bytes and reuses a free pooled vertex buffer of matching FVF/usage/pool (aging and evicting unused ones when over 300), otherwise creates a new D3D vertex buffer and wraps it in a CYyVb. |
| `0x1003B3C0` | `CYyVbMng_ReleaseVertexBuffer` | high |  | Finds the pooled entry in use (state 3) holding the given D3D vertex buffer and decrements its state, returning it to the pool. |
| `0x1003B450` | `CYyVb_Constructor` | high |  | Stores CYyVb vtable, links it (sub_1003B4D0), and records size, FVF, usage, pool, in-use state 3 and the D3D VB pointer. |
| `0x1003B640` | `CMoAttachments_Reset` | medium |  | Clears the caster/target pointers and auxiliary fields and resets both handle members to their default values. |
| `0x1003B750` | `CMoAttachments_ClearCaster` | high |  | Optionally unlinks from the current caster, then nulls the caster pointer (this+3) and resets its handle. |
| `0x1003B780` | `CMoAttachments_ClearTarget` | high |  | Optionally unlinks from the current target, then nulls the target pointer (this+4) and resets its handle. |
| `0x1003B7B0` | `CMoAttachments_SetCaster` | high |  | If the caster changes, notifies the old caster (vtable+1000 detach), stores the new caster at this+3, captures its handle and registers this attachment with it. |
| `0x1003B830` | `CMoAttachments_SetTarget` | high |  | If the target changes, notifies the old target (vtable+1004 detach), stores the new target at this+4, captures its handle and registers this attachment with it. |
| `0x1003D030` | `CMoCameraTask_Destructor` | high |  | Restores the vtable and mixin vtable, frees owned path data and buffer, clears the global current-camera-task pointer if it is this, and calls the base dtor. |
| `0x1003D640` | `Sky_DrawGradientDome` | medium |  | Generates and draws a dome of radius a1 in horizontal bands between elevation keys, a3 segments around, interpolating vertex colors between the key colors, as triangle strips. |
| `0x1003D980` | `CMoCelestial_Destructor` | high |  | Restores the CMoCelestial vtable (empty destructor body). |
| `0x1003D9A0` | `Sky_DrawDome` | medium | yes | If the zone environment has more than 2 sky bands, sets the sky shader and view, converts 8 sky colors, draws the gradient dome with radius far-near-1, then restores render states. |
| `0x1003DB50` | `Sky_DestroyDomeShaderStatic` | medium |  | atexit handler that destroys the function-local static shader object used by Sky_DrawDome. |
| `0x1003DB60` | `Sky_DrawDomeViewCentered` | medium | yes | Variant of the sky draw that builds a view matrix with translation removed (camera-centered), draws the gradient dome at 0.8*(far-near), then restores render states and the scratch matrix stack. |
| `0x1003DDF0` | `Sky_DestroyViewCenteredShaderStatic` | medium |  | atexit handler that destroys the static shader object used by the view-centered sky draw. |
| `0x1003E280` | `Color_SetFromU32SwapRB` | high |  | Stores a 32-bit color into 4 bytes with red and blue swapped (ARGB <-> ABGR byte order). |
| `0x1003E2A0` | `Color_CopySwapRB` | high |  | Copies a 4-byte color from a2 swapping bytes 0 and 2 (red/blue). |
| `0x1003EF20` | `CMoVuAnmElem_Destructor` | high |  | Restores the vtable, frees the optional animation buffer, releases sub-objects and runs the shared CMoElem cleanup. |
| `0x1003FCB0` | — | low |  | Returns true when field +52 is 0 and the element type (flags+48 & 0xF) is 5..7. |
| `0x1003FD70` | `MoElem_GetVertexData` | medium |  | Returns this+128 for element types outside 5..7, otherwise the 16-byte entry indexed by +52 in the array from sub_1003FCF0. |
| `0x10040670` | `CMoDistModelElem_DrawMesh` | medium |  | Sets alpha test/texture factor and the element's shader constants and transforms, then draws the distorted model mesh as triangle lists, handling the type-5..7 offset case, and restores states. |
| `0x10041C20` | `Font_DrawProjectedText3D` | medium | yes | Projects a 3D position through a matrix and, if in front of the camera, draws up to 16 characters of a string as centered screen-space glyph quads. |
| `0x10041F30` | `StaticDtor_StateBlock_1047BADC` | high |  | atexit handler that destroys the static state-block object at dword_1047BADC. |
| `0x100425F0` | `CMoOtTask_ScalarDeletingDtor` | medium |  | Scalar deleting destructor for a CMoOtTask-family task with a CMoAttachments mixin at +0x54. |
| `0x100439B0` | `StaticDtor_StateBlock_1047BB7C` | high |  | atexit handler that destroys the static state-block object at dword_1047BB7C. |
| `0x10043AB0` | `CMoTask_VT_1032AF60_Destructor` | high |  | Destructor of task class CMoTask_VT_1032AF60: notifies the attached target (sub_100ACDB0 with fields +0x7C/+0x8C), releases the attachment and runs the CMoTask base dtor. |
| `0x10043CB0` | `CMoTask_VT_1032AFA8_Destructor` | high |  | Destructor of task class CMoTask_VT_1032AFA8: notifies the attached target (sub_100ACEA0), releases the attachment and runs the CMoTask base dtor. |
| `0x10043DF0` | `DynVB_Constructor` | medium |  | Constructs a 0x48-byte double-buffered dynamic vertex buffer object (vtable off_1032AFD4) with two 32-byte slots zeroed. |
| `0x10043E40` | `DynVB_Destructor` | medium |  | Destructor of the dynamic vertex buffer object: restores vtable off_1032AFD4 and releases buffers via sub_10043F00. |
| `0x10043E50` | `DynVB_Create` | high |  | Releases old buffers and creates N dynamic vertex buffers (or takes cached ones) of given vertex count and FVF. |
| `0x10044010` | `DynVB_Reserve` | high |  | Reserves N vertices in the current slot's buffer, computing lock offset/size and choosing DISCARD when the ring wraps. |
| `0x10044130` | `DynVB_Lock` | high |  | Locks the reserved range of the current dynamic VB slot and returns the data pointer. |
| `0x100441A0` | `DynVB_Unlock` | high |  | Unlocks the current dynamic VB slot. |
| `0x10044210` | `Gfx_SetViewMatrixFromField10` | medium |  | Sets the D3D view transform from the matrix at this+0x10. |
| `0x10044230` | `Gfx_SetViewMatrixFromField90` | medium |  | Sets the D3D view transform from the matrix at this+0x90. |
| `0x10044270` | `Mo_InitSharedRenderResources` | medium | yes | One-time init of shared effect render resources: seeds the RNG, creates a 256x256 'cubemap spec' texture/target pair and five dynamic VBs with different FVFs, plus a CYyObject. |
| `0x10044890` | `CMoElem_Constructor` | high |  | Base constructor of CMoElem: calls parent ctor, inits occlusion query member, stores CMoElem vtable and zeroes/initializes fields. |
| `0x10047900` | `CMoSkeletonElem_Destructor` | high |  | Destructor of CMoSkeletonElem: releases its resource ref and deletes the owned object at +469, then shared elem cleanup. |
| `0x10047F80` | `CMoGeneratorClone_VT_1032B114_Constructor` | medium |  | Constructor for the generator base resource class (CMoResource-derived with CMoAttachments mixin at +0x30). |
| `0x10048000` | `CMoGeneratorClone_VT_1032B114_Destructor` | medium |  | Destructor of the generator base: frees resources of type 5 that reference this generator, detaches, deletes owned objects and runs the CMoResource base dtor. |
| `0x10048140` | `CMoGeneratorClone_UnlinkElemListA` | medium |  | Unlinks an element from the generator's first doubly linked element list (links at elem+0x14C/+0x150, head at this+0xB0) and releases a resource ref. |
| `0x100481D0` | `CMoGeneratorClone_UnlinkElemListB` | medium |  | Unlinks an element from the generator's second doubly linked element list (links at elem+0x154/+0x158, head at this+0xAC) and releases a resource ref. |
| `0x100492F0` | `CMoGeneratorClone_FindCmdByType` | medium |  | Scans the generator's packed command stream (this+0xE4) for the first record whose type byte equals a2. |
| `0x10049570` | `CMoGeneratorClone_ResolveTarget` | medium |  | Resolves and caches the generator's target object from its header (type code + id), with a zone-map fallback for type 31. |
| `0x10053450` | `CMoGeneratorClone_CopyParams` | medium |  | Copies generator parameter fields (+0x20..+0xEC, including bitfields at +0xD8) from a source generator, clearing pointer members +0xA4..+0xB0. |
| `0x100541C0` | `CMoTask_VT_1032B654_Destructor` | high |  | Destructor of task CMoTask_VT_1032B654: clears the owner's back-pointer (+0xA8) and runs CMoTask base dtor. |
| `0x100542F0` | `CMoGeneratorClone_Destructor` | high |  | Destructor of CMoGeneratorClone: detaches, deletes owned object, clears the back-pointer in its element and frees its handle before chaining to the generator base dtor. |
| `0x100543A0` | `CMoGeneratorClone_Kill` | medium |  | Stops and destroys a generator: detaches, deletes its owned object, drops all references and frees its resource. |
| `0x100548C0` | `CMoLfdElem_Destructor` | high |  | Destructor of CMoLfdElem: restores vtable and runs shared elem cleanup. |
| `0x10055000` | `Vt1032B6D8_Destructor` | medium |  | Destructor for an object (vtable off_1032B6D8) that owns a block containing three CMoSpline members; destroys them and frees the block. |
| `0x10055240` | `LinkNode_Init` | medium |  | Zeroes the prev/next/owner fields (+4, +0x10, +0x14) of an intrusive list node. |
| `0x10055250` | `LinkNode_Unlink` | medium |  | Unlinks an intrusive list node from its neighbours if it is in a list (owner at +0x14 non-null). |
| `0x100552E0` | `CMoOT_Destructor` | high |  | Destructor of CMoOT: frees the buffer at +4. |
| `0x100555D0` | — | low |  | Allocates a 24-byte object into dword_1047BC94, initializes it with (512, 0.0, 512.0) and registers callback sub_10055620 in the dword_1047D140 manager. |
| `0x10056220` | `CMoSphRes_FixupOffsets` | medium |  | Converts four self-relative offsets (+0x10..+0x1C) in a loaded resource into absolute pointers. |
| `0x10056390` | `CMoPointLightProgElem_Destructor` | high |  | Destructor of CMoPointLightProgElem: unregisters its point light (sub_10178610) then runs shared elem cleanup. |
| `0x100574A0` | `CMoSchedularTask_Destructor` | high |  | Destructor of CMoSchedularTask: notifies attached objects, clears back-references in other 'Ms' tasks, frees owned data, invokes an end callback and destroys 16 sub-entries. |
| `0x1005F2C0` | `CMoPathDriveActorTask_ScalarDeletingDtor` | high |  | Scalar deleting destructor of CMoPathDriveActorTask; calls the attached actor's vtbl+380 hook before release. |
| `0x1005F3F0` | `CMoLockColorDriveTask_ScalarDeletingDtor` | high |  | Scalar deleting destructor of CMoLockColorDriveTask; if active, resets the actor color to 0x80808080 (neutral). |
| `0x1005F660` | `CMoLockLookAtDriveTask_ScalarDeletingDtor` | high |  | Scalar deleting destructor of CMoLockLookAtDriveTask; clears the look-at lock flag (bit 1 of +0x840) on the actor. |
| `0x1005F990` | `CMoActorColorDriveTask_ScalarDeletingDtor` | high |  | Scalar deleting destructor of CMoActorColorDriveTask; restores the actor's saved color via vtbl+96 or vtbl+48. |
| `0x1005FE30` | `CMoActorRotationDriveTask_ScalarDeletingDtor` | high |  | Scalar deleting destructor of CMoActorRotationDriveTask; finalizes the actor rotation (wraps Euler angles to [-pi,pi] and copies the saved quaternion/vector) or camera state. |
| `0x10060190` | `CMoAttachments_VT_1032B940_ScalarDeletingDtor` | medium |  | Scalar deleting destructor of an afterimage-unit task; resets an actor subsystem (sub_1018BC40(actor+0x5FC, 0)) before release. |
| `0x100602D0` | `CMoAttachments_VT_1032BAF0_ScalarDeletingDtor` | medium |  | Scalar deleting destructor of a task that calls sub_10083310 on the attached actor before release. |
| `0x100608B0` | `CMoAttachments_VT_1032BB38_ScalarDeletingDtor` | medium |  | Scalar deleting destructor of a task that calls the attached object's vtbl+356 before release. |
| `0x10060BB0` | `CMoAttachments_VT_1032BBAC_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 124-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10060E60` | `CMoAttachments_VT_1032BBF4_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 144-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061010` | `CMoAttachments_VT_1032BC68_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061140` | `CMoAttachments_VT_1032BC84_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061300` | `CMoAttachments_VT_1032BCCC_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 132-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x100614B0` | `CMoAttachments_VT_1032BD14_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 136-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x100616B0` | `CMoAttachments_VT_1032BD5C_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 136-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061830` | `CMoAttachments_VT_1032BDA4_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 124-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061A00` | `CMoAttachments_VT_1032BDEC_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 128-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061B80` | `CMoAttachments_VT_1032BE34_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061D00` | `CMoAttachments_VT_1032BE7C_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10061EA0` | `CMoAttachments_VT_1032BEC4_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 124-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10062090` | `CMoAttachments_VT_1032BF0C_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10062210` | `CMoAttachments_VT_1032BF54_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x100623B0` | `CMoAttachments_VT_1032BF9C_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10062550` | `CMoAttachments_VT_1032BFE4_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x10062700` | `CMoAttachments_VT_1032C02C_VectorDeletingDtor` | high |  | MSVC vector deleting destructor for 120-byte task objects: flag&2 destroys a counted array, else a single object; frees when flag&1. |
| `0x100634A0` | `CMoSpline_Destructor` | high |  | Destructor of CMoSpline: restores vtable and frees spline data. |
| `0x10063C60` | `MoProgElem_FreeBuffer40` | medium |  | Frees the buffer pointer at this+0x40 and nulls it. |
| `0x10069D70` | `CMoDX_Destructor` | high |  | Destructor of CMoDX: unbinds any textures it set on stages 0-7. |
| `0x10069DD0` | `CMoDX_Initialize` | high |  | Initializes CMoDX with the CDx renderer: raises timer resolution, records start time, caches device caps and device pointer. |
| `0x1006A150` | `CMoDX_CreateTexture` | high |  | Creates a WxH single-level texture through the CDx renderer. |
| `0x1006A480` | `CMoDX_SetWorldMatrix` | high |  | Sets the D3D world transform. |
| `0x1006A4A0` | `CMoDX_GetWorldMatrix` | high |  | Reads the cached D3D world transform. |
| `0x1006A4C0` | `CMoDX_PushViewMatrix` | medium |  | Saves the current view matrix onto an 8-deep stack in CMoDX. |
| `0x1006A4F0` | `CMoDX_PopViewMatrix` | high |  | Pops the view-matrix stack and restores the saved matrix as D3D view transform. |
| `0x1006A520` | `CMoDX_SetViewMatrix` | high |  | Sets the D3D view transform. |
| `0x1006A560` | `Gfx_CalcFVFStride` | high |  | Computes the vertex size in bytes for a D3D FVF code. |
| `0x1006AE30` | `StateBlock_ApplyRSTSS` | high |  | Applies a cached D3D state block, recording it first (from a render-state list and a texture-stage-state list) if the token is -1. |
| `0x1006AF90` | `StateBlock_ApplyTSS` | high |  | Applies a cached D3D state block, recording it first from a texture-stage-state list if the token is -1. |
| `0x1006B050` | `CMoDxStatusMng_Constructor` | high |  | Constructor of CMoDxStatusMng: initializes its embedded state-block member and stores the vtable. |
| `0x1006B090` | `CMoDxStatusMng_Destructor` | high |  | Destructor of CMoDxStatusMng: frees its linked list of entries and destroys its embedded state block. |
| `0x1006B0C0` | `VBPool_Constructor` | medium |  | Constructs the global 1192-byte CYyObject-derived pool that DynVB_Create searches for reusable vertex buffers. |
| `0x1006B250` | `CMoTask_VT_10329C70_Destructor` | high |  | Destructor of task CMoTask_VT_10329C70: calls sub_1006B3B0 cleanup then the CMoTask base dtor. |
| `0x1006B2B0` | `CMoDX_CreateAndInitSystem` | high | yes | Creates the CMoDX singleton (g_pResourceMng) and initializes the effect/model subsystem: VB pool, mesh resource root, shared render resources, status manager and occlusion manager. |
| `0x1006BD90` | `Rand_SeedMT19937` | high |  | Seeds the Mersenne Twister (MT19937) state table. |
| `0x1006C0F0` | `Mem_AllocWithHeader12` | medium |  | Allocates a block with a 12-byte header and returns the pointer past the header. |
| `0x1006C110` | `Mem_FreeWithHeader12` | medium |  | Frees a block allocated with a 12-byte header. |
| `0x1006C140` | `CMoOcclusionMng_Constructor` | high |  | Constructor of CMoOcclusionMng: creates a 16x16 lockable render target in the scene texture's format. |
| `0x1006C1E0` | `CMoOcclusionMng_Destructor` | high |  | Destructor of CMoOcclusionMng: releases the 16x16 render target and destroys the CAcc member. |
| `0x1006C260` | `OcclusionQuery_Init` | medium |  | Zeroes an occlusion query member (surface pointer +4, word +2). |
| `0x1006C270` | `OcclusionQuery_Release` | medium |  | Releases the surface reference held by an occlusion query member. |
| `0x1006C9B0` | `OcclusionMng_Prepare` | high | yes | Creates the 16x16 occlusion target on first use, reads the scene target surface desc, and caches an AddRef'd pointer to the target in the query. |
| `0x1006D5D0` | `CMoProcessor_Vec3Normalize` | high |  | Normalizes a 3-float vector in place (no-op on zero length). |
| `0x1006D7E0` | `CMoProcessor_Vec4TransformB` | medium |  | Transforms a 4D vector by a 4x4 matrix (row vector * M). |
| `0x1006D870` | `CMoProcessor_Vec4Transform` | high |  | Transforms a 4D vector by a 4x4 matrix (row vector * M). |
| `0x1006D900` | `CMoProcessor_Vec3Transform` | high |  | Transforms a 3D point (w=1) by a 4x4 matrix to a 4D result. |
| `0x1006E7D0` | `CMoProcessor_ProjectPointsToScreen` | medium |  | Projects an array of 3D points through a world and a view-projection matrix to screen x/y, storing w as the third value. |
| `0x1006E970` | `CMoProcessor_CalcScreenBounds` | high |  | Computes the screen-space bounding rectangle of a strided point array under a matrix and the current view-projection. |
| `0x1006EB70` | `CMoProcessor_ProjectVertsAndBounds` | medium |  | Projects 9-float vertices to screen, writing screen x/y into each vertex (+28/+32), and returns their bounding rectangle. |
| `0x1006ED10` | `CMoProcessor_CalcScreenBoundsAndVisible` | medium |  | Computes the screen bounds and depth range of a point set and counts points that land inside the viewport. |
| `0x10070F80` | `CMoTask_VT_1032C490_Destructor` | high |  | Destructor of task CMoTask_VT_1032C490: restores vtable and runs the CMoTask base dtor. |
| `0x10071050` | `ResHandle_FreeIfUnreferenced` | medium |  | Forwards the resource handle stored in *this to MoRes_FreeIfUnreferenced. |
| `0x10071180` | `CMoAttachments_VT_1032C4E8_ScalarDeletingDtor` | high |  | Scalar deleting destructor of a task with a CMoAttachments mixin at +0x44: releases the attachment, runs the task dtor and frees. |
| `0x10071C10` | `CMoAttachments_VT_1032C628_ScalarDeletingDtor` | high |  | Scalar deleting destructor of a CMoResource subclass with attachments mixin at +0x30. |
| `0x10071C60` | `CMoResource_VT_1032C588_ScalarDeletingDtor` | high |  | Scalar deleting destructor of the CMoResource subclass with vtable 1032C588 (mixin at +0x30). |
| `0x10071DE0` | `CMoResource_Destructor` | high |  | Base destructor of CMoResource: destroys/unlinks the resource and frees its handle slot in the mesh resource manager. |
| `0x10071E90` | `CMoResource_AddRefChain` | medium |  | Increments the 15-bit reference count of a resource and every parent up to the root. |
| `0x10071EC0` | `CMoResource_ReleaseChain` | medium |  | Decrements the 15-bit reference count of a resource and its parents up to the root, clearing bit 15 at the root when it reaches zero. |
| `0x10071F50` | — | low |  | Gets a related resource via sub_10072240, increments its field +0x1C and returns a pointer to its +4 member. |
| `0x10072030` | `CMoResource_RegisterInTypeList` | medium |  | Registers a resource once: allocates its handle and links it at the head of its per-type list and the global resource list. |
| `0x100721F0` | `CMoResource_Destroy` | medium |  | Destroys a resource once: marks it dead, unlinks it and, if it owns its handle, frees the handle and deletes the object. |
| `0x10072270` | `CMoResource_GetParent` | medium |  | Returns the parent resource from the parent handle at +0x28, or this if the resource is its own root. |
| `0x10072290` | `CMoResource_FindByTypeAndId` | medium |  | Looks up a resource of a given type and FourCC id under this resource with a fresh search serial; returns the handle via out param. |
| `0x10072430` | `CMoResource_FindInScope` | medium |  | Walks the sibling chain from a resource, respecting nested open(type 1)/close(type 0) blocks, to find one matching type and/or id; returns its handle. |
| `0x100729E0` | `CMoTask_VT_1032C908_Destructor` | high |  | Destructor of task CMoTask_VT_1032C908: frees two owned buffers and decrements a global instance counter. |
| `0x10072ED0` | `MeshResourceMng_LoadSystemResources` | medium |  | Loads the root resource set into the mesh resource manager and caches well-known system resources (type 27 block with 'vmad','vsim','vcur' entries, and id 52743). |
| `0x10073B20` | `MoRes_FreeIfUnreferenced` | medium |  | Destroys a resource (and, if its root is not locked, the root's children) when its reference count is zero. |
| `0x10074140` | `MeshResourceMng_AllocHandle` | high |  | Returns the resource's handle, allocating one from the manager's free list if it has none. |
| `0x10074180` | `NodePool_ReleaseNode` | medium |  | Pushes a node onto the pool's free list at this+4 and decrements the live count at this+0x4000C (clamped at 0), guarded by a range check against the 65536-dword inline pool. |
| `0x10074290` | `GetGlobal_1047D114` | high |  | Returns the global dword_1047D114. |
| `0x100746A0` | `MoProcessor_CallVSlot49` | medium |  | Forwards `this` to virtual slot 49 (+0xC4) of the g_pProcessor CMoProcessor singleton. |
| `0x10074740` | `CMoTask_Constructor` | high |  | Stores the CMoTask vtable, then (unless byte_1047D13D is set) resets the fields and registers the task, and returns this. |
| `0x100747C0` | `CMoTask_Destructor` | high |  | Restores the CMoTask vtable and calls sub_10074AD0(this,1), which unlinks the task. |
| `0x100748F0` | `CMoTask_ResetFields` | medium |  | Zeroes the CMoTask fields at +4..+0x28 and copies the one-shot global flag byte_1047D13C into +0x20, then clears that global. |
| `0x100749B0` | `CMoTask_RegisterWithMng` | medium |  | Passes this task and an argument to the task manager singleton dword_1047D140 through sub_10074F80, most likely to register or enqueue it. |
| `0x10074A10` | `CMoTaskMng_Forward10075120` | medium |  | Calls sub_10075120 on the task manager singleton dword_1047D140 with a1. |
| `0x10074B60` | `CMoTaskMng_Destructor` | high |  | Stores the CMoTaskMng vtable, frees the dynamically allocated task chain at +0xD50, then destroys the 64 embedded CMoTask slots in reverse order. |
| `0x10075280` | `Gfx_InitCubeMapTextures` | medium |  | Creates render-target cube textures of size 32<<i (3 sizes), then clears each face except one by drawing a screen quad into it. |
| `0x10075520` | `TexRef_ResolveByName` | medium |  | Resolves a texture reference by name: it looks the name up in g_pTextureMng (sub_1003AF10), and if that misses it scans the manager's list for an entry whose 8-byte name matches this+8..+15, then stores the found handle in *this. |
| `0x100755A0` | `TexRef_EnsureRenderTarget` | medium | yes | Makes sure a cached CAcc render-target helper (dword_1047D158) matches the width and height (+36/+38) of the referenced texture. If the size differs, it releases the helper and recreates it with a surface from sub_1006A150 and an object from sub_1000AD70. |
| `0x10075960` | `TexRef_SetTexture` | high |  | Binds the referenced texture (the D3D texture at +64, falling back to +68) to texture stage a2, or binds NULL when the reference is empty. |
| `0x100814A0` | `ActorHandle_Constructor` | medium |  | Initializes a small handle object: stores vtable off_1032CF5C, clears +4, keeps the top byte of +8 and sets byte +11 to 4. |
| `0x100814E0` | `ActorHandle_Destructor` | medium |  | Restores the off_1032CF5C vtable (an empty destructor body). |
| `0x10081A50` | `Actor_ResetDrawLists` | medium |  | Resets the actor draw-list head, the update-bucket counters and related globals (one is set to 50), and clears the actor handle at dword_1047D600. |
| `0x10082B10` | `Actor_UnlinkAllAttachLists` | medium |  | Empties two intrusive lists at +0x68 and +0x6C by repeatedly unlinking the head (sub_1003B5E0 / sub_1003B610) until each is null. |
| `0x10084140` | `CXiActor_IsModelMissingOrFlag2000` | medium |  | Returns true when the actor has no model data at +0x70, or when that data's flags at +0x120 have bit 0x2000 set. |
| `0x10084330` | `CXiActor_ModelTest95790` | medium |  | If model data (+0x70) exists, forwards a2 to sub_10095790 on it and returns the bool; otherwise returns false. |
| `0x10084350` | `CXiActor_ModelTest95680` | medium |  | If model data (+0x70) exists, forwards a2 to sub_10095680 on it and returns the bool; otherwise returns false. |
| `0x10084370` | `CXiActor_ModelTest956A0` | medium |  | If model data (+0x70) exists, forwards a2 to sub_100956A0 on it and returns the bool; otherwise returns false. |
| `0x10084390` | `CXiActor_GetModelField170` | high |  | Returns the dword at model+0x170, or the default dword_1047D60C when there is no model. |
| `0x100843B0` | `CXiActor_HasNonzeroField150or158` | medium |  | Returns true when model(+0x70)->+0xD4->+0x260 has a nonzero float at +0x150 or +0x158. |
| `0x10084400` | `CXiActor_GetModelByteEE` | high |  | Returns the signed byte at model+0xEE, or the default dword_1047D610 when there is no model. |
| `0x10084420` | `CXiActor_GetModelByteEF` | medium |  | Returns a byte that comes from virtual +956 when virtual +948 says so, otherwise from model+0xEF, otherwise the default dword_1047D614. |
| `0x10084480` | `CXiActor_GetModelPartWord` | medium |  | Returns the u16 part id for slot a2: from virtual +964 when virtual +948 says so, otherwise model+0xFC[a2], otherwise the default (a2<<12)\|table[a2]. |
| `0x10084500` | `CXiActor_GetModelFlags120Top2` | high |  | Returns bits 30-31 of model+0x120, or 1 when there is no model. |
| `0x10084520` | `CXiActor_GetModelFlags130Top2` | high |  | Returns bits 30-31 of model+0x130, or 0 when there is no model. |
| `0x10084540` | `CXiActor_GetModelFloat9AEC0` | medium |  | Returns sub_1009AEC0(model) as a double, or 0.0 when there is no model. |
| `0x10084580` | `CXiActor_IsModelMissingOrSuppressed` | medium |  | Returns true when there is no model, when actor byte +0x88 has bit 8 set, or when model+0x128 has bit 0x10 set. |
| `0x100845B0` | `CXiActor_TestModelFlag128_8` | medium |  | Returns model+0x128 bit 8, or, when there is no model, whether byte +9 of g_pCamera is nonzero. |
| `0x10084640` | `CXiActor_TestModelFlags100000_200000_4000` | medium |  | Returns true when model data exists and has flag 0x100000 or 0x200000 at +0x120, or flag 0x4000 at +0x128. |
| `0x10084720` | `CXiActor_GetModelByte1DB` | high |  | Returns the unsigned byte at model+0x1DB, or 0 when there is no model. |
| `0x10085060` | `CXiActor_SpawnResourceGenerators` | medium |  | Unless the actor is suppressed (vfunc+292 or flag +178&2), it walks the actor's type-5 resources. For each one of the listed kinds with flag 0x1000 at +216 it calls sub_10053A00, then collects up to 128 of them and creates a CMoGeneratorClone instance for each. |
| `0x10085490` | `CXiActor_IsInModelSlotTable` | medium |  | Returns true when this actor pointer appears in any of the 12 actor slots at model+0xA4..+0xD0. |
| `0x10085570` | `CXiActor_KillGeneratorClones` | high |  | Walks the actor's two attachment lists (+0x68 and +0x6C). For each entry whose owner (vfunc+24) is a CMoGeneratorClone that sub_100539E0 does not flag, it kills the clone (sub_100543A0) and restarts the scan. |
| `0x10086210` | `Actor_DrawWorldText` | medium |  | Draws a text string at a 3D position. It sets the world transform from matrix a1, projects the point, culls it if it falls more than 100px off-screen, scales by viewport/426.7*a3, expands special glyph codes 0xC8-0xCD, measures the glyphs through the font (sub_10120510 on dword_104E1BF8) and draws the quads with DrawPrimitiveUP. (pseudocode truncated) |
| `0x1008A0D0` | `Call1008A0E0With2` | high |  | Calls sub_1008A0E0(2). |
| `0x10093720` | `Unit10086E90_ResetGlobals` | medium |  | Bulk-resets a large set of module globals (tables, flags, counters; some set to 600, 0x7F007F or -1), runs a series of sub-resets and then calls sub_101C80C0(dword_10489948, sub_1025B560(37)). |
| `0x10093DB0` | `Net_SendPacket00COnce` | medium |  | On first call (gated by dword_104859E4) it allocates a 12-byte outgoing packet with header 0x060C (id 0x00C, size 3 dwords), clears the gate and sets several ready flags, then calls sub_10097FD0(0,0). |
| `0x10097840` | `Entity_BuildDisplayName` | medium |  | Builds the display name in the static buffer byte_104855B0: up to 2 prefix icon chars plus status chars, a space, and then the name from +532 (or +124 when the u16 +528 is 0) via strncat(…,128). It also sets the name colour at (+160)->+124 (0x80808080, or +468 when the prefix is 0x92). |
| `0x10097A40` | `Entity_GetIndexedPtr` | high |  | Returns dword_10480AF0[u16 at +0x2A2], or 0 when the index is 0. |
| `0x10098320` | `Entity_IsModelIdB22toB31` | high |  | Returns true when the u16 at +0xFC is in the range 0xB22..0xB31. |
| `0x100A2810` | `CMoTask_VT_1032D6D4_Destructor` | high |  | Destructor for a CMoTask subclass: stores vtable CMoTask_vtbl_ambiguous35, marks byte +1 of the linked object at +0x48 as 0xFF if it is 0, clears the singleton dword_10487F08 when it points to this, and then runs CMoTask_Destructor. |
| `0x100A4C20` | `CXiCollisionActor_Constructor` | high |  | Builds the CXiControlActor base, stores the CXiCollisionActor vtable and initializes collision fields, including two random 0..119 timers and an orientation (0,-1,0,1) at +0x160. |
| `0x100A8CA0` | `CXiControlActor_Destructor` | high |  | Stores the CXiControlActor vtable, destroys the embedded ActorHandle at +0x5A0 and then runs the base destructor (sub_10082B40). |
| `0x100A9D30` | `CMoTask_VT_1032E808_Destructor` | high |  | Destructor for a CMoTask subclass: deletes the owned CXiSchStatus at +0x7C, releases the embedded CMoAttachments sub-object at +0x34 and then runs CMoTask_Destructor. |
| `0x100C5410` | — | low |  | Resolves the actor handle dword_1047D600, calls its virtual +820 with 1 and clears the global dword_10456DAC. |
| `0x100C5440` | — | low |  | Resolves the actor at dword_1047D600 and calls its virtual +820(0). It then picks the current target (sub_10157CF0 or sub_10081600) and calls the target's virtual +1016. If the actor's GetModelByteEF is 5 or 6 it scales the global float dword_10456DA8 by 0.8, and it sets dword_10456DAC=0. |
| `0x100C54C0` | — | low |  | Same as sub_100C5440, except that it sets the global dword_10456DAC to 5.0f (0x40A00000) instead of 0. |
| `0x100CB4E0` | `Actor_SelectNearestLights` | medium |  | Picks up to 4 of the actor's 5 candidate lights. It ranks point lights by 1/(dist^2*atten) and blends directional lights toward their targets, then copies the chosen D3DLIGHT8 records into g_lightTable. Point lights are converted to directional lights pointing from the light to the actor and scaled by their weight. (pseudocode truncated) |
| `0x100CCE60` | — | low |  | If vfunc+340 returns true, it reads two joint or point positions (ids 23 and 24) and a matrix (id 28) from the actor, writes them into the object returned by sub_10015250 (+140/+152 with flags +136..138), and passes an angle (x*60 degrees in radians) and a scaled value (y*200) on to sub_10015290. |
| `0x100D0880` | `CXiSkeletonActor_DrawNameIfVisible` | medium |  | Calls Actor_DrawWorldText(a2..a6) unless the actor's u16 at +0xB2 is nonzero. |
| `0x100D08B0` | `CXiSkeletonActor_DrawName2IfVisible` | medium |  | Calls sub_10086900(a2,a3,a4), the 3-argument text-draw variant, unless the actor's u16 at +0xB2 is nonzero. |
| `0x100D44F0` | `CXiSkeletonActor_DeleteOwnedList` | medium |  | Walks the owned list at +0x7E4, unlinking each node (sub_1002C800) and deleting it through its virtual slot +24 with argument 1 (deleting destructor). |
| `0x100D54C0` | `CXiSkeletonActor_CalcGroundTiltMatrix` | medium |  | Builds a smoothed orientation matrix in a2. It takes the ground normal (vfunc+528), clamps it to within 30 degrees of up (0,-1,0), blends it by this[522] and over time by a3 into this[523], and combines it with the actor yaw (vfunc+448). The tilt weight this[522] then comes from the aspect ratio of the bounding extents (vfunc+968). |
| `0x100D5B10` | `CXiSkeletonActor_UpdateHeadLook` | medium |  | Chooses a look-at target (the current target or sub_100845E0) when the actor's motion and category allow it. It takes joint 3 of the target (lowered by 1.2 when target+0xB2 is set), and ramps a look weight at controller+188 up or down at about 0.04 per step depending on whether the target is in front and near. |
| `0x100D6D30` | `Actor_PrepareDrawGlobals` | medium |  | Lazily caches the actor's per-draw resources: a texture named by off_1035F60C, and resources of kind 10/'evev', 94 and 10/'ohno' via vfunc+492, recorded by flags in +0x840. It then publishes them and other per-actor values (light count, colour 0x80808080-style) into globals used by the draw path. |
| `0x100DB100` | `GameMode_GetField98` | high |  | Returns the dword at +0x98 (the mode or enabled field tested by GameMode_RequestState). |
| `0x100DB140` | `GameMode_PollPendingState` | medium |  | Returns 100 when no state change is pending (+0x90==100). Otherwise it returns 101 if the requested state (+0x8C) is negative, or copies the requested state into +0x14 and returns it. |
| `0x100DB170` | `GameMode_RequestState` | medium |  | State machine transition: validates request a2 against the current state (+0x88) using per-state allowed-predecessor sets. It records the new state and parameter a3 (+0x80 or +0x84), clears the pending marker (+0x90=100) and resets the +0xC timer. An invalid request recurses into error state 201. |
| `0x100DBB50` | `GameMode_OpenTacticsUrl` | high |  | Formats a POL 'ffxi.tactics' web URL from 17 fields of record a2 (or a fixed URL when a2 is null) and hands it to sub_10017630 to open. |
| `0x1011AAC0` | `Gfx_ApplyViewport1B70` | high | yes | Applies the saved viewport g_viewport_1B70 through CDx_SetViewport. |
| `0x1011BAE0` | `Font_BindGlyphTexture` | high | yes | Lazily looks up the 'FontMoji' font texture into +0x74 (and the high-res variant at +0x78 via sub_1011C720 when +0x80==1), then binds the active one to texture stage 0. |
| `0x1011BBD0` | `Font_DrawGlyphQuad` | high | yes | Fills a 4-vertex screen quad for glyph code a2 from a 64x128 glyph grid (UV steps 1/64, 1/128), sized 16*a5 by 16*a6 at position a3 and tinted by colour a4. It draws the quad as a 2-triangle strip and then restores the vertex colours. |
| `0x1011BDC0` | `Gfx_SetAdditiveBlend` | high | yes | Enables additive alpha blending (ONE, ONE). |
| `0x1011BF40` | `Font_DrawChar` | medium |  | Sets the vertex colour of the shared font quad. Codes >= 0x2115 (8469..8515) are drawn as icon sprites from table dword_10621934 (sub_1011FB60), after which additive blend and the glyph texture are restored. Other codes are drawn as glyph quads at the cursor dword_104E1B88/8C. |
| `0x1011C080` | `Obj_104E1B90_DrawString` | high | yes | Draws a string: it sets the blend mode from flag bits of a6, positions the cursor at (a1,a2), and loops over the characters (decoding with sub_1011CE80) to draw each with Font_DrawChar, advancing by the scaled glyph width. Afterwards it resets the vertex colours to 0x80808080, sets ALPHAOP=5 and disables alpha blending. |
| `0x1011C1F0` | `Gfx_SetCellBlendMode` | high | yes | Sets the alpha-blend state for a 'cell' compositing mode chosen by three 2-bit selectors (zero/frame/texture combos). It programs SRCBLEND/DESTBLEND/BLENDOP and logs unsupported combos by name. |
| `0x1011C5D0` | `Font_AllocHighResImage` | medium |  | Allocates and zeroes a 61-byte image header plus a2*a3*4 bytes of 32-bit pixels, named 'FontHigh'. It fills in the dimensions and format fields, stores the result in *a1 and returns 32. |
| `0x1011C670` | `Tex_LoadSurfaceFromDatFile` | medium |  | Gets level-0 surface of the texture held by a1 (+64/+68), reads archive file a2 from the file system dword_10662DCC into a temporary buffer, and loads it into that surface with D3DXLoadSurfaceFromFileInMemory. Returns true on success. |
| `0x1011C720` | `Font_LoadHighResTexture` | medium | yes | Picks a font-texture tier from a config value and device max texture size, builds a 'font high' texture header, creates the texture object and fills its surface from a DAT file; releases everything on failure. |
| `0x1011C810` | `Font_ReleaseHighResTexture` | medium |  | Destroys the texture object at +0x78 through its virtual deleting dtor, frees the header buffer at +0x7C and clears the loaded flag at +0x80. |
| `0x1011C850` | `Font_IsHighResTextureLoaded` | medium |  | Returns true when flag +0x80 is 1 and the texture object at +0x78 has a surface at +8. |
| `0x1011CE80` | `Font_DecodeNextChar` | medium |  | Reads one character from a string cursor: a single-byte char advances 1 and returns ch-32; otherwise it advances 2 and maps the double-byte code through sub_1011C940. |
| `0x1011D840` | `Font_AddGlyphAdvance` | medium |  | Adds a glyph's advance width to *a2: codes 8469..8515 use cell rect width (x2-x1) from table dword_10621934, codes < 0x440 use the byte width table byte_1036C428, and others use a fixed 14. |
| `0x1011DA80` | `Sjis_GetCharByteLen` | medium |  | Returns sub_1011DA30(first byte)+1, which gives the byte length of the character at a1 (1 or 2). |
| `0x1011E800` | `Cell_DrawAt` | medium | yes | Binds the cell's texture (lazily looked up by its lowercased 16-char name at +50 in g_pTextureMng), writes the 4 quad vertex positions as the cell rect (shorts +192..+206) offset by (x,y) minus 0.5, then draws with the cell's stored blend parameters. |
| `0x1011EDE0` | `Cell_DrawTransformed` | medium | yes | Draws a cell with each vertex's color modulated by a20 and its position transformed by a matrix argument plus (x,y). The blend mode comes from packed a21 or the cell's defaults, and the original vertex colors are restored afterwards. |
| `0x1011EF60` | `Cell_DrawStretchedWrapped` | medium | yes | Sets texture addressing to WRAP, then draws the cell stretched to a given size and scale. UVs come from the texture dimensions with flip flags in byte +28, colors are modulated by a10, and addressing is restored to CLAMP afterwards. |
| `0x1011F2D0` | `Cell_DrawWithDefaultBlend` | high |  | Calls sub_1011F300 with the cell's vertex array (+80) and its stored blend bytes +45..+48. |
| `0x1011F300` | `Cell_DrawQuadBlended` | high | yes | Draws the cell's 4-vertex quad as a triangle strip after mapping three PS2-GS-style alpha operands (texture/frame/zero) onto D3D SRCBLEND/DESTBLEND/BLENDOP. Unsupported combinations only log a 'cell ...' debug string. (pseudocode truncated) |
| `0x1011FC80` | `Shape_DrawScaled` | medium |  | Iterates the shape's cell list (count at +20), skipping the first a9 cells, and draws each cell scaled by (sx,sy), optionally centered on the shape's rect. When the scale is 1.0 it takes the unscaled fast path sub_1011FB60. |
| `0x1011FF70` | `Shape_DrawTransformed` | medium |  | Iterates the shape's cell list and calls Cell_DrawTransformed on each cell with the same matrix, color and blend arguments. |
| `0x101200B0` | `Color_ModulateArgb128` | high |  | Multiplies two ARGB colors per channel as (a*b)>>7, the PS2 convention where 128 means 1.0, and clamps each channel to 255. |
| `0x101201C0` | `ShapeMng_Init` | medium |  | Initializes the shape manager: clears its two font and two damage-number texture slots, sets their first-glyph thresholds to -1, clears the cached shape pointers at +44/+64, then initializes the list. |
| `0x10120450` | `ShapeMng_FindShape` | high |  | Searches the manager's list for an entry whose 16-char name at +36 matches (case-insensitive) and returns its data via sub_1011E290, or NULL. |
| `0x10120510` | `ShapeMng_GetFontGlyph` | medium |  | Looks up and caches the 'font fontshp' shape, takes the cell for character a2 (index a2-32), returns its width/height (+32/+36) and passes it to sub_1011FA80. |
| `0x10120590` | `ShapeMng_BindFontTexture` | medium | yes | Lazily loads the font textures, picks texture page 0 or 1 by comparing glyph code a2 with each page's first code, and binds it with SetTexture. |
| `0x10120660` | `ShapeMng_LoadFontTextures` | high |  | Walks the 'font fontshp' cells in order and records up to 2 distinct texture names (looked up in g_pTextureMng) together with the first glyph code (starting at 32) that uses each. |
| `0x10120730` | `ShapeMng_GetDmgNumGlyph` | medium |  | Damage-number counterpart of ShapeMng_GetFontGlyph: caches the 'menu dmgnum' shape, takes the cell for digit a2 (index a2-48), returns its width/height and passes it to sub_1011FA80. |
| `0x101207B0` | `ShapeMng_BindDmgNumTexture` | medium | yes | Damage-number counterpart of ShapeMng_BindFontTexture: lazily loads up to two textures and binds the one that matches code a2. |
| `0x10120880` | `ShapeMng_LoadDmgNumTextures` | high |  | Walks the 'menu dmgnum' cells and records up to 2 distinct textures from g_pTextureMng together with the first code that uses each. |
| `0x10124440` | `Cursor_Init` | medium | yes | Resets the menu cursor: clears state, centers its position on the screen rect, looks up the 'anc ancscro' shape and the cursor animation shape table, hides the hardware cursor, and sets the initial position. |
| `0x101245C0` | `Cursor_GetHwCursorSlot` | medium |  | Returns the word at +0xA6. |
| `0x10124700` | `Cursor_SetPosition` | high | yes | Stores a new cursor position (and optional value a3 at +72). In windowed or mouse mode it converts the virtual-screen position to desktop coordinates using the window rect and frame metrics, then warps the OS mouse. |
| `0x10124EB0` | `Cursor_Draw` | medium | yes | When visible, advances the cursor's animation frame. With a hardware cursor it sets and shows the OS cursor through CDx. Otherwise it hides it and draws the cursor shape transformed (scale matrix) at +12/+16; state 6 also draws the scroll-anchor shape. |
| `0x101252F0` | `Cursor_SetVisible` | medium | yes | Sets the cursor visible flag at +78. In hardware-cursor mode it shows or hides the device cursor and selects the cursor shape when becoming visible; otherwise it keeps the device cursor hidden. |
| `0x101262A0` | `Cursor_StoreScreenCenter` | medium |  | Stores half the screen-rect width and height in fields +0x38/+0x3C. |
| `0x10127C00` | `ChatLog_AddMessage` | medium |  | Adds a text message to the chat log: applies a message filter, optional text conversion and a sound cue, then forwards the line to the chat windows dword_1062193C/dword_10621940 (or special handlers for modes 0x96/0x97). |
| `0x10157660` | — | low |  | Changes a 2-bit mode in field +92 (values 0/1/2). Depending on the mode and on whether a resolved actor handle differs from this object's handle, it first calls sub_10159420 with 1/0/4. |
| `0x10159400` | `Actor_SetSlotTableEntry` | medium |  | Stores a2 into global table dword_10578770[a1] for a1<5 and returns success. |
| `0x1015D900` | `MenuMng_Init` | medium |  | Initializes the UI manager: virtual size 640x480 (+128/+130), offsets and flags cleared, pointers to the chat/log globals stored at +140..+156, then calls sub_1015D9A0. |
| `0x1015E350` | `MenuMng_OpenByName` | medium |  | Resolves a menu/window name. If an entry exists it re-initializes and reactivates it; otherwise it allocates a new 0xD4-byte entry, names it, sets type 14 and registers it. Returns the entry. |
| `0x10160250` | `MenuMng_GetUiScale` | medium |  | Returns the X and Y UI scale as width/512 and height/448, i.e. the current UI size relative to the PS2 512x448 layout. |
| `0x10160290` | `MenuMng_GetOffsetX` | medium |  | Returns float +0x84, or 0 when the scene state is 64 and a2 is false. |
| `0x101602C0` | `MenuMng_GetOffsetY` | medium |  | Returns float +0x88, or 0 when the scene state is 64 and a2 is false. |
| `0x101604B0` | `Gfx_SetDefault2DStates` | high | yes | Sets the standard 2D UI pipeline: fog off, alpha test GREATER 0, linear min / point mag filtering, WRAP addressing, stage-0 MODULATE for color and alpha, stage 1 disabled, no texture transform. |
| `0x10160650` | `Gfx_DisableFogAndAlphaBlend` | high | yes | Turns off fog and alpha blending and resets BLENDOP to ADD. |
| `0x101620B0` | `Gfx_ApplyViewport1904` | medium | yes | Applies the global viewport g_viewport_1904 through CDx_SetViewport. |
| `0x101630E0` | `Obj_10621944_Draw` | medium | yes | If the object has content at +12, fills its background rect, then draws +12 via sub_1019EBB0 with MAGFILTER temporarily set to linear. |
| `0x10163390` | `Obj_10621944_DrawBgRect` | high |  | If the color at +8 is non-zero, draws the object's rect (shorts at +0) as a solid quad of that color. |
| `0x10163400` | `Gfx_DrawSolidRect2D` | high | yes | Draws a screen-space solid-color rectangle from 4 shorts (x1,y1,x2,y2) with color a2: z, fog, lighting and blending off, diffuse color only, then restores defaults. |
| `0x10164B80` | `Game_DestroyGlobalObjects` | medium |  | Deletes about 40 global singleton objects through their virtual scalar-deleting destructor (arg 1), nulls each pointer, then tears down dword_1062194C. (pseudocode truncated) |
| `0x101674B0` | `Vt10336614_Destructor` | medium |  | Destructor that only restores vtable off_10336614 (no members to release). |
| `0x101678D0` | `Vt10336634_Destructor` | medium |  | Destructor: restores vtable off_10336634, then releases members via sub_10167950. |
| `0x10167940` | `GetPtr8IfField60Set` | high |  | Returns this+8 if the dword at +0x60 is non-zero, else NULL. |
| `0x10168D20` | `Dbg_DrawWireMesh` | medium |  | On the given device, draws a wireframe of a static face table (triangles or quads indexing a vertex table) scaled by a4 and translated to position a2, as closed line strips in color a3 with lighting off. |
| `0x1016C060` | `Mat44_Multiply` | high |  | Multiplies two 4x4 float matrices (a2 * a3) into a temporary and copies the result to a1, so in-place use is safe. |
| `0x1016DDF0` | `Dbg_DrawTriangleLines` | high | yes | Draws the 3 edges of a triangle (points a1,a2,a3) as a line list with an identity world transform, no texture, no lighting. |
| `0x1016FC80` | `Vt10336668_Destructor` | medium |  | Destructor: restores vtable off_10336668, then calls sub_10170CD0(this, NULL) to release members. |
| `0x10171920` | `Vt1033668C_Destructor` | medium |  | Destructor: restores vtable off_1033668C, then releases members via sub_10174A60. |
| `0x10174B10` | `Dbg_DrawOverheadMapMarkers` | medium | yes | Draws a 512x512 screen-space top-down plot centered at (256,256), scale 256/1400: one entity list as small triangles, a second list in another color, and the position a2 as a larger diamond. |
| `0x10174EC0` | `Dbg_DrawOrientedBoxes` | medium | yes | For every node in the list dword_10621A08, builds a world matrix per entry (scale, Y-axis rotation, translation) and draws a unit cube (+/-0.5 vertices) untextured and unlit; also calls the text routine sub_10086210. (pseudocode truncated) |
| `0x10175740` | `Mat4_SetIdentity_2` | high |  | Writes a 4x4 identity matrix (1.0f on the diagonal, 0 elsewhere) into the argument and returns it. |
| `0x10175780` | `Mat4_MulTranslation` | medium |  | Computes out = M * T, where T is a translation matrix built from the vec3 a3, and writes the result to a1. |
| `0x10176EC0` | `CMoResource_VT_10336698_Destructor` | high |  | Destructor body: restores this class's vtable, then chains to the base destructor sub_10071DE0. |
| `0x10176FA0` | `CMoResource_VT_103366C4_Destructor` | high |  | Destructor body: restores this class's vtable, then chains to the base destructor sub_10071DE0. |
| `0x10177020` | `CMoResource_VT_103366F0_Destructor` | high |  | Destructor body: restores this class's vtable, then chains to the base destructor sub_10071DE0. |
| `0x10177240` | `Gfx_AssembleVertexShader` | high | yes | Assembles the vertex-shader source text held by this object, then creates a D3D8 vertex shader from the bytecode and the given declaration. |
| `0x10177340` | `Gfx_ShaderSrc_GetText` | medium |  | Returns the shader source string pointer stored at this+4. |
| `0x10177350` | `Gfx_ShaderSrc_GetTextLength` | medium |  | Returns strlen of the shader source string stored at this+4. |
| `0x101779C0` | `Gfx_SetupLineDrawState` | medium | yes | Prepares the device to draw untextured, unlit, vertex-coloured primitives: identity world matrix, no texture, FVF 0x142, and stage 0 selecting the current colour and alpha. |
| `0x10177B40` | `Gfx_DrawQuadOutline` | high | yes | Draws a closed outline through four 3D points as a LINESTRIP, using one ARGB colour; when the last flag is 0 it turns the depth test off for the draw. |
| `0x10177C50` | `Gfx_DrawLine3D` | high | yes | Draws a single coloured 3D line segment between two points. |
| `0x10177CD0` | — | low | yes | Thunk that forwards g_pCDx->pDevice and its three arguments to sub_10168D20. |
| `0x10178070` | `CTsZoneMap_Destructor` | high | yes | CTsZoneMap destructor: restores the vtable, tears down the zone, deletes its vertex and pixel shaders, and destroys member subobjects. |
| `0x101781D0` | `CTsZoneMap_InitZBiasPresets` | medium | yes | Initialises the global z-bias presets (zone mesh, aux, overlay, restore) and uses a smaller set when a CDx capability flag is set. |
| `0x101791F0` | `ZoneMap_DrawBox` | medium | yes | Draws an indexed box mesh (24 vertices, 12 triangles) from two vertex streams with no texture and clockwise culling. |
| `0x101792A0` | `ZoneMap_CreateVertexShaders` | high | yes | Creates the zone map's two fixed-function vertex shaders from static declarations. |
| `0x10179390` | — | low |  | Caches a value computed from the subobject at this+376, and if sub_10015250() returns an object, copies that object's position (+68) into this+0x38D4C along with the cached value. |
| `0x10179410` | `ZoneMap_CopyField342C8ToField39450` | medium |  | Copies the dword at this+0x342C8 into this+0x39450. |
| `0x1017ADE0` | `ZoneMap_DrawMeshGroupsWithFog` | medium | yes | Sets the fog mode (table fog, vertex fog or range fog, chosen from CDx capability flags), then draws every mesh id listed in each group of the given zone block. |
| `0x101813F0` | `ZoneMap_FindGroundHeight` | medium |  | Finds the ground height at (x, z) by casting a vertical segment against the collision map at this+376; on a miss it retries in the other direction, and on a hit it writes the hit Y to *a6. |
| `0x10182700` | `CTsZoneMap_DrawProjectedTexture` | medium | yes | Projects texture a4 onto the zone meshes in the 3x3 grid of 4-unit cells around position a2, using a texture matrix built from translate(-pos), scale(1/a5) and an offset. (pseudocode truncated) |
| `0x10182F10` | `Gfx_DrawWireBox` | high | yes | Draws an axis-aligned bounding box as a wireframe: two closed rectangles (bottom and top faces) plus four vertical edges, unlit, colour 0x8000FFC0. |
| `0x10184FE0` | `CTsZoneMap_DrawProjectedShadow` | medium | yes | Projects a darkening texture (multiplicative blend) onto the zone meshes in the 3x3 grid around position a1 with radius a2. This looks like a ground shadow or decal. (pseudocode truncated) |
| `0x10186F20` | `Env_GetColorPtr84` | medium |  | Returns this+0x84, the secondary RGB colour block that Env_ComputeAmbient uses when its mode is 1. |
| `0x10186FA0` | `Env_GetAmbientColorPtr` | medium |  | Returns this+0x104, the ambient RGB float triple of the environment. |
| `0x10188E00` | `Env_ComputeAmbient` | high | yes | Computes the zone ambient colour as packed RGB (float*128 per channel), using the colour block selected by mode and applying the time-of-day tint to dark colours; returns -1 when the environment is not active. |
| `0x10188FA0` | `Env_ComputeAmbientFloat4` | medium | yes | The float4 version of the ambient computation: it outputs the time-of-day-tinted ambient RGB scaled by 0.5 with alpha 0, or 255 in every channel when the environment is inactive. |
| `0x1018CA20` | — | low |  | If the linked object at this+12 exists and its short at +28 differs from the byte at this+3, calls sub_1018C980(this, byte); otherwise returns 0. |
| `0x10193850` | `Settings_GetValue` | high |  | Reads an in-game setting by index from the settings object, returning -1 when the object does not exist. |
| `0x1019EBB0` | — | low |  | Unless the flag at +84 is set, gathers three values from the object and calls sub_1019EDB0 with colour 0x80808080 and 37; on failure it sets the flag. |
| `0x101AB6D0` | — | low |  | Calls sub_1018EB00 on two subobjects (+409668 of dword_10578158/1057815C), runs two operations on the settings object 0x106626CC, and clears byte +8 of dword_10662DB0. |
| `0x101AB720` | — | low |  | Calls sub_101954F0 on the settings object when the byte at +10 of that object equals 1, and returns 1. |
| `0x101C4AF0` | — | low |  | A checked call: it runs sub_101C1B20(this[1], a2) and, if the result is negative, reports the error and terminates the process with code 2003. |
| `0x101C8DA0` | `Gfx_ProjectWorldToScreen` | medium | yes | Projects the world-space point a1 to integer screen coordinates (rounded) in *a2 and *a3, saving and restoring the VIEW and WORLD transforms. |
| `0x101CC4F0` | — | low |  | Returns the entry of table dword_10480AF0 indexed by the global word_10485F7A, or 0 when the index is 0. |
| `0x1021D4C0` | `CXiMovie_DrawFrame` | medium | yes | Draws the current movie frame texture as a screen-space textured quad (no Z, fog, lighting or blending), with the rectangle scaled from the surface description. |
| `0x1021D980` | — | low |  | Polls the movie or stream object: when sub_1021DD50 returns 1 it sets state this+4 to 2 and returns 0, otherwise it returns 1. |
| `0x1021DDF0` | `Movie_GetCurrentTexture` | medium |  | Returns the texture for the current frame from the movie's ring of textures. |
| `0x1021E060` | `Movie_CreateFrameTextures` | high | yes | Records the video frame dimensions from the stream format and creates 4 managed 512x512 A8R8G8B8 textures for frames, then checks that the texture format is A8R8G8B8 or A1R5G5B5. |
| `0x10239460` | `Ui_KeyPrompt_Init` | medium |  | Resets a small UI prompt or overlay state and sets its mode to a2 (intensity 128, ids -1, counters 0). |
| `0x102394A0` | `Ui_DrawKeyPrompt` | medium | yes | Draws an animated 'menu keytops3' key-prompt sprite near the bottom-right of the screen (5-frame cycle with fade-in), or in mode 0 draws a named resource texture with grey intensity. |
| `0x10245B60` | `Lobby_DestroyThreadPool` | medium |  | Shuts down the lobby thread pool and deletes it through its virtual deleting destructor when present. |
| `0x1024D790` | `StAvatar_StopAndFreeBuffers` | medium |  | If active, waits for the worker queue to drain (Sleep(2) loop), clears the active flag, and frees the avatar's buffer subobjects. |
| `0x1024F900` | `StAvatar_SetViewFromDancerCamera` | medium | yes | Builds a D3D view matrix from the dancer camera entry (index 6 when flag 0x100 is set, otherwise a1+28) and applies it with SetTransform(VIEW). |
| `0x102504D0` | `StAvatar_Shutdown` | medium | yes | Shuts down the StAvatar subsystem: stops and frees buffers, releases models and resources, resets two parameters, and deletes two D3D state blocks. |
| `0x10253F60` | `Gfx_DrawSpriteAlphaTested` | medium | yes | Draws the sprite or texture held at this+520 with linear filtering and alpha test (GREATER than 16), then restores point mag filtering and alpha ref 0. |
| `0x1025D750` | `FontGlyphCache_GetDevice` | high | yes | Returns the renderer's IDirect3DDevice8. |
| `0x1025D760` | `FontGlyphCache_CreateTexture` | high | yes | Wraps IDirect3DDevice8::CreateTexture, returning the texture or NULL on failure. |
| `0x1025D7B0` | `FontGlyphCache_CreateVertexBuffer` | high | yes | Wraps IDirect3DDevice8::CreateVertexBuffer, returning the buffer or NULL on failure. |
| `0x10271B40` | — | low |  | Releases dancer-side state: calls sub_10285740 on dword_1099CCF8 when it is set, calls sub_1028E1B0(dword_1099B13C), and clears bit 0 of the flag word dword_1099B0D8. |
| `0x10271E90` | `Dancer_GetFlags` | medium |  | Copies the dancer state flag word dword_1099B0D8 to *a1 when a1 is non-null. |
| `0x10273370` | `Dancer_GetCameraEntry` | medium |  | Returns pointers to entry a1 (the current index when -1) in two parallel static tables of 52-byte and 124-byte records. |
| `0x10275150` | `Dancer_ResetMemPools` | high |  | Resets every dancer memory-pool descriptor (96-byte entries) and clears the pool bookkeeping globals. |
| `0x10275B60` | `Dancer_SetMemPool` | high |  | Registers memory block a2 of size a3 as dancer pool slot a1 with name a4, first aligning the block to 16 bytes. |
| `0x1027EB10` | `Mat4_Copy` | high |  | Copies a 4x4 float matrix (64 bytes). |
| `0x1027EF20` | `Mat4_TransformPoint` | high |  | Transforms the vec3 a3 by the 3x4 part of matrix a2 (dot of each row with the point, plus the translation in column 3) into a1. |
| `0x1027F070` | `Mat4_Transpose` | high |  | Writes the transpose of 4x4 matrix a2 into a1. |
| `0x10281510` | `sqgxTextureCreateFromImage` | medium |  | Creates a managed D3D texture for sqgxTexture slot a1 and fills it from the slot's image data, converting RGB24 or RGBA32 to BGRA; 8-bit images get an identity palette. |
| `0x102816E0` | `sqgxTextureAddDepthStencil` | high | yes | Finds or creates a depth/stencil surface entry (w, h, depth) in the sqgxTexture table and returns its index (maximum 512). |
| `0x10281810` | `sqgxCeilLog2` | high |  | Returns the smallest n such that 2^n >= a1. |
| `0x10281830` | `sqgxTextureAdd` | high | yes | Finds or creates a texture slot (w, h, depth) in the sqgxTexture table and returns its index (maximum 512). With image data it uploads it; without data it creates a render-target texture. |
| `0x102819D0` | `sqgxTextureAddPalette` | medium | yes | Allocates a free palette slot (1-255) and loads 256 entries from RGB or RGBA source data (stride a2) into the device palette, returning the slot. |
| `0x10281D60` | `sqgxTextureSetPalette` | medium | yes | Selects texture palette a1, or for -1 selects palette 0 when the current context texture is 8-bit. |
| `0x102820E0` | `sqgxTextureSetRenderTarget` | medium | yes | Makes sqgxTexture slot a1 the current render target, using the back buffer when the slot is the default frame buffer, and pairs it with the slot's depth surface or the default one. |
| `0x102823B0` | `sqgxSetContextField8F0Pair` | medium |  | Updates the two sqgx context fields at +0x8F0 and +0x8F4 to (a1, a2) when they change, and sets the dirty flag dword_109A2618. |
| `0x102823F0` | `sqgxSetAlphaTest` | high | yes | Sets the alpha test (enable a1, reference a2, function GREATEREQUAL), skipping redundant changes through the sqgx context cache. |
| `0x10282470` | `sqgxSetZWrite` | high | yes | Enables or disables depth writes, cached in the sqgx context to avoid redundant render-state calls. |
| `0x102824C0` | `sqgxColorMask` | high | yes | Sets D3DRS_COLORWRITEENABLE from four PS2-style per-channel mask flags (nonzero = channel masked), cached at ctx+0x904..0x910. |
| `0x10282640` | — | low | yes | Texture-stage-only blend setup: color = CURRENT, alpha = TEXTURE on the extra stage; does nothing without a spare stage. |
| `0x102826C0` | `sqgxBlendAdd` | medium | yes | Additive blend mode: framebuffer ADD ONE,ONE, or stage ADD(CURRENT,TEXTURE). |
| `0x102827D0` | — | low | yes | Additive blend variant: stage MULTIPLYADD (CURRENT + TEXTURE*(1-DIFFUSE)); framebuffer fallback plain ONE,ONE add. |
| `0x10282910` | `sqgxBlendSub` | medium | yes | Subtractive blend: dest - src via BLENDOP REVSUBTRACT ONE,ONE, or stage SUBTRACT(TEXTURE,CURRENT). |
| `0x10282CB0` | `sqgxBlendTrspInv` | medium | yes | Inverse-alpha translucent blend: src*(1-srcA) + dst*srcA, or stage BLENDTEXTUREALPHA. |
| `0x10282DC0` | `sqgxBlendAddDestAlpha` | medium | yes | Additive blend weighted by alpha: framebuffer src*destA + dst, or stage MODULATEALPHA_ADDCOLOR. |
| `0x10282ED0` | — | low | yes | Blend src + dst*srcA (framebuffer ONE,SRCALPHA), stage path uses MODULATEALPHA_ADDCOLOR with TFACTOR=0. |
| `0x10282FF0` | `sqgxBlendZeroSrcAlpha` | medium | yes | Blend that scales the destination by source alpha (framebuffer ZERO,SRCALPHA); stage variant with TFACTOR=0. |
| `0x10283110` | `sqgxBlendMul` | medium | yes | Multiplicative blend: dst*src via ZERO,SRCCOLOR, or stage MODULATE(TEXTURE,CURRENT). |
| `0x10283380` | `sqgxSetCtxField768` | medium |  | Stores a float to sqgx context +0x768 and sets dirty flag dword_109A2720. |
| `0x102838A0` | `sqgxSetAspectRatio` | medium |  | Sets ctx+0x2DC to a1/a2 (aspect ratio) and rescales the derived projection factor at ctx+0x74C to compensate. |
| `0x10283A10` | `sqgxSetViewport` | high | yes | Sets the D3D viewport (x,y,w,h, Z 0..1), stores width/height and recomputes projection scale terms. |
| `0x10283E00` | `sqgxMatrixWorld` | medium |  | Copies a 4x4 matrix into the context world matrix (ctx+0x1C8) and marks transforms dirty. |
| `0x10283F70` | `sqgxZBiasAdd` | high | yes | Adds a delta to the cached Z bias and applies D3DRS_ZBIAS. |
| `0x10283FC0` | `sqgxZBiasSet` | high | yes | Sets D3DRS_ZBIAS to an absolute value if it differs from the cached ctx+0x2E8. |
| `0x10284410` | `sqgxFlushTransformsLights` | medium | yes | Uploads projection/view/world matrices and 4 directional + 4 point lights to the device, computes object-space eye position, clears the transform-dirty flag. |
| `0x10284950` | `sqgxVertexShaderCreate` | medium |  | Builds a D3D8 vertex declaration from the current vertex-format flags (one stream per attribute) and creates and sets the vertex shader. |
| `0x10284A80` | `sqgxVertexShaderSelect` | medium | yes | Looks up a cached vertex shader for the OR of current stage format flags; creates one via sqgxVertexShaderCreate on a miss, else sets the cached handle. |
| `0x10284EF0` | `sqgxVertexShaderRelease` | medium | yes | Deletes the vertex shader handle stored at ctx+0xA18 and zeroes it. |
| `0x10285850` | `sqCallHook30` | medium |  | Calls the function pointer at a1+0x30 if non-null. |
| `0x10285890` | `sqDebugPrintf` | high |  | printf-style debug output: formats into a 256-byte buffer and sends it to OutputDebugStringA. |
| `0x1028AA70` | `sqgxShapeIndexBufferBegin` | medium | yes | Lazily creates a 3072-entry dynamic 16-bit index buffer (or malloc in software mode), sets it as indices and locks it with DISCARD, resetting the write count. |
| `0x1028F8C0` | `sqsnSceneDrawLayers` | medium | yes | Draws all scene layers: renders snapshot layers into ping-pong 1024x512 render targets and composites them, draws normal/shadowed layers with pre/post callbacks, and applies a stencil-masked fade when scene alpha < 1. |
| `0x102979E0` | `sqsnShadowVolumeExtrudeTriList` | medium |  | For a triangle-list strip group, emits shadow-volume geometry per triangle using light-facing flags and adjacency (-1 = open edge) via sub_10297530. |
| `0x10297B10` | `sqsnShadowVolumeExtrudeTriStrip` | medium |  | Triangle-strip counterpart: per strip triangle (count-2) with alternating winding, emits shadow-volume geometry via sub_10297530. |
| `0x10297CC0` | `sqsnShadowVolumeSetPass` | high | yes | Sets render state for stencil shadow-volume passes: 0 = front faces INCR, 1 = back faces DECR, 2 = restore (stencil off, Z/color writes on, Gouraud). |
| `0x10297EC0` | `sqsnShadowStencilTest` | medium | yes | Enables/disables a stencil test that restricts drawing to pixels with stencil 0 (outside shadow volumes); stores the flag at ctx+0x124. |
| `0x10297F90` | `sqsnShadowComputeFacing` | medium |  | Computes per-triangle light-facing flags: dot(faceNormal, vertex - lightPos) <= 0 for point lights, or dot(normal, lightDir) <= 0 for directional. |
| `0x102981E0` | `sqsnShadowVolumeBeginVB` | high | yes | Prepares the shadow-volume vertex buffer: creates a 720000-byte XYZ dynamic VB if needed, binds it (stride 12, FVF XYZ) and locks it with DISCARD. |
| `0x10298350` | `sqgxMatrixPlanarShadow` | high |  | Builds a planar projection shadow matrix from the plane at ctx+0x12C and the normalized directional light 0 direction (D3DXMatrixShadow form, L.w=0). |
| `0x102A21F0` | `sqsnShadowVolumeDraw` | high | yes | Draws a stencil shadow volume for a tri-strip shape: transforms light to object space, computes facing, extrudes strips/lists into the VB, then renders it in INCR and DECR passes. |
| `0x102A2530` | `sqsnSceneLayerDrawPlanarShadows` | medium | yes | Draws planar projected shadows for a layer's casters: multiply blend with (1 - light color), Z bias +4, no Z write/test, stencil EQUAL+INCR to avoid double darkening, each caster drawn with shader flag bit31 (planar shadow matrix). |
| `0x1031077E` | `Thunk_FnTable694` | medium |  | Calls the function pointer at offset 0x694 of the table pointed to by dword_10456A2C. |

## 4 Oct 2026: the export's unnamed globals

Every global the 867-function export references under a placeholder (`dword_`, `flt_`, `unk_`, `byte_`, `off_`,
`word_`, `stru_`): 663. Each has a name or a stated reason for not having one (`data/global_names.json`); the
names are in `data/names_pending_idb.json` (`"kind": "global"`) until the next IDA run applies them.

### Evidence

`pipeline/global_dossier.py` collects, for each global, from the binary and the export only:
- every reference in the whole image (the relocation chain, each resolved to its instruction through
  `data/func_extents.json`, or to the data address holding it) and what the instruction does with it: read,
  write, float read/write, address taken, compare, call through, at which width;
- the section and the initial bytes (as an integer, a float, a string, or the target of a pointer);
- the export's pseudocode lines that mention it.

Read in address order (neighbours are often one object). The helpers in `pipeline/global_naming/` show the
context (`ctx.py`), what is left (`todo.py`), and apply the three rules below.

### Rules (applied mechanically, each a fact of the code)

- **Vertex arrays** (`addarr.py`): several "globals" are the fields of one vertex array a function fills and
  passes to `DrawPrimitiveUP`. The vertex format comes from the function's `SetVertexShader`/`Gfx_CalcFVFStride`
  value (0x44 = XYZRHW|DIFFUSE, 20 bytes; 0x144 = +TEX1, 28; 0x142 = XYZ|DIFFUSE|TEX1, 24), so each global is
  named `<array>_v<k>_<field>`. Where the layout is not established, nothing is named.
- **State blocks** (`stateblocks.py`): the handle and tables passed to `StateBlock_ApplyTSS`,
  `StateBlock_ApplyRSTSS` and `sub_1006AF00` (which records `SetRenderState` only) are named
  `<Function>_stateBlock` / `_renderStates` / `_textureStageStates`, as the IDB already names `Sky_DrawDome_*`.
  The roles are those calls' code: device slots 52/50/63/53/54 (Begin/SetRenderState/SetTextureStageState/
  End/ApplyStateBlock), tables of pairs or triples ending 0x7FFFFFFF, the handle -1 until recorded.
- **Scope** (`rules2.py`): a byte whose every use is `(g & bit) == 0` then `g |= bit` in one function is that
  function's MSVC static-init guards (`<Function>_staticGuards`); a global every reference to which, in the
  whole image, is in one function is that function's static (`<Function>_static_XXXX`). These names state the
  scope only, not a meaning.

### Confidence

- **high (283):** a string or exact bytes say what it is (the 3 D3D8 interface IDs equal `d3d8.h` in all 16
  bytes; "DXT1".."DXT5"; the Shift-JIS messages, translated in the evidence), a declared role (the `pUp` of
  `D3DXMatrixLookAtRH`, the argument of `SetMaterial`, register c15..c18 of `SetVertexShaderConstant`), one of
  the rules above, or a fact already established in `docs/RENDERER.md` (the frame-step ring);
- **medium (239):** what it is follows from its uses, but the wording is an inference (`g_fadeQuadVerts` for
  the quad `CDx_DrawFadeQuad` fills, `g_screenFadeLevel` for the level centred on 0x80);
- **low:** not named. 141 globals are unnamed, each with its reason in `data/global_names.json`: mostly
  singletons used by hundreds of functions outside the export (one is read 1,052 times), app and configuration
  flags whose readers are not exported, and 7 tables of Microsoft's D3DX, which is not game code.

### Checks

- No new name equals an existing function, global, parameter or local name, or a C++ keyword (globals are
  macros in the tree).
- The clean tree regenerated with the names builds the same `gfx_recon.dll`, byte for byte, as without them:
  names do not change code. 377 of the 522 names appear in it (the others are used only by functions the tree
  does not hold); no placeholder of a named global is left.

## 2 Oct 2026: globals and callees

This round named the placeholders left in `tree/`: 24 `sub_` callees and 283 data symbols (`dword_`, `byte_`, `flt_`, `unk_`, `off_`, `word_`). 193 were named (86 high, 107 medium confidence); 13 `sub_` and 105 data placeholders remain because the code does not say enough about them.

### Where the names live

The names were kept in `data/names_pending_idb.json` while the IDB could not be edited; `pipeline/hx2clean.py`
applies them when it generates the tree (`canonicalise_names`). `ida/ffxi_ida_fix.py` pushed them into the
IDB on 3 Oct, each with a comment giving confidence and evidence. Its third run found all of them already
there except one: 0x10285890 is `sqPrint` in the IDB (wanted `sqDebugPrintf`), left as the IDB has it.

The same pass also fixes **stale names in the export**. Some records were exported before an IDB rename and still print the placeholder (for example `dword_1099D200`, which 600 other references print as `g_pSqgxContext`). Each address now gets one name across the whole export. The function's own record name is not touched, because it decides class placement.

### Checks

- The regenerated tree builds all 174 files.
- Its `.text`, `.data` and `.rdata` sections are **byte-identical** to the build of the previous tree, so every gate result (633 PASS) still applies.
- No new name collides with any label in the IDB (checked against the IDA listing of 2 Oct).

### Method

- Usage context for each symbol came from the Hex-Rays export: every function that reads or writes it, with the lines.
- The IDA listing `FFXiMain_recon.asm` (2 Oct) supplied what the export lacks:
  - initial values;
  - the Shift-JIS strings;
  - the bodies of callees outside the graphics set;
  - names already changed in the IDB (`scnFini`, the `D3DMATERIAL8` at 0x104571E0).
- The settings globals are named from the settings loader's table of `{&global, registry key}` pairs, so `g_cfgHardwareMouse` is key `0021`.

### Corrections made after reading the listing

The first pass was based on the export alone. These names changed once the listing was available:

- `dword_1045F040`/`dword_1045F04C` are not constant zero/unit vectors. They are uninitialised and written by `MotionQueue_UpdateAllChannels`, so they are `g_poseTranslation` and `g_poseScale`.
- `sub_101C4AF0` does not return a file size. It resolves a file and calls `exit(2003)` on failure, so it is `FileMng_ResolveOrExit`.
- `dword_1047D130` is a recursion depth: it is incremented on entry to `CMoResource_FindByTypeAndId`. It is not a budget.
- `dword_103515BC` is the constant 0x80808080, PS2 white (`c_ps2White`).

### Notable finds

- **The actor factory's error strings** name the sub-actor kinds: chocobo, ground target, range, border, chair, fishing rod and magic circle. For example, `aSubActorNoMainFishingRod` is "Treated as a sub-actor (fishing rod) but there is no main actor".
- **The high-res font table** has rows of `{DAT file id, width, height}`, starting with file 0x9A77 at 1024x2048. The row is selected by registry key 0036 (font compression).
- **The sqgx texture table** at 0x1099D618 has 10-int entries: name, width, height, kind, log2 width, log2 height, format, palette slot, texture and surface.

### Every name (193)

| Address | Was | Name | Conf. | Evidence |
|---|---|---|---|---|
| `0x10032FA0` | `sub_10032FA0` | `BoneXform_PostMultiplyInto` | medium | builds a temporary 4x4 from this (sub_10032FE0) and Mat4_PostMultiply(arg, tmp) |
| `0x100C5410` | `sub_100C5410` | `TargetActor_SlotHandler1` | medium | resolves g_targetActorHandle, calls its vfunc +0x334 with 1, clears dword_10456DAC; installed in actor slot table entry 1 by every CXiSkeletonActor constructor |
| `0x100C5440` | `sub_100C5440` | `TargetActor_SlotHandler0` | medium | resolves g_targetActorHandle, vfunc +0x334(0), scales dword_10456DA8 for model kinds 5/6, sets dword_10456DAC = 0; slot table entry 0 |
| `0x100C54C0` | `sub_100C54C0` | `TargetActor_SlotHandler4` | medium | as TargetActor_SlotHandler0 but sets dword_10456DAC = 5.0f; slot table entry 4 |
| `0x1018CA20` | `sub_1018CA20` | `Sch_GetStatusEntry` | medium | called with the schedular status in CMoSchedularTask_Interpret and CXiSkeletonActor_SetAttack_SchStatus; result used as an entry pointer |
| `0x101C4AF0` | `sub_101C4AF0` | `FileMng_ResolveOrExit` | medium | thiscall on g_pFileMng: calls sub_101C1B20(id) on this+4; on failure reports (id, id>>8) with sub_10253670 and calls exit(2003) |
| `0x1021D980` | `sub_1021D980` | `Movie_Update` | medium | thiscall on g_pMovie: when sub_1021DD50 returns 1 sets state (+4) = 2 and returns 0, else returns 1; called before Movie_GetCurrentTexture |
| `0x10271B40` | `sub_10271B40` | `scnFini` | high | current IDB name (IDA listing 2026-10-02); the export record predates it |
| `0x10281510` | `sub_10281510` | `sqgxTextureCreateFromImage` | high | IDB name |
| `0x10281810` | `sub_10281810` | `sqgxCeilLog2` | high | IDB name |
| `0x10285890` | `sub_10285890` | `sqDebugPrintf` | high | IDB name (export record exported before the rename) |
| `0x10333000` | `dword_10333000` | `g_hiResFont_fileId` | high | row 0 = 0x9A77, row 1 = 0x9A78: DAT file id; rows of 3 ints {file id, width, height} indexed by g_cfgFontCompression-1 in Font_LoadHighResTexture |
| `0x10333004` | `dword_10333004` | `g_hiResFont_width` | high | second int of each high-res font row (row 0: 0x400) |
| `0x10333008` | `dword_10333008` | `g_hiResFont_height` | high | third int of each high-res font row (row 0: 0x800) |
| `0x10341DBC` | `unk_10341DBC` | `GUID_SysMouse_ref` | medium | GUID passed to IDirectInput::CreateDevice in DirectInput_InstallMouse |
| `0x10349EF4` | `unk_10349EF4` | `c_dfDIMouse_ref` | medium | data-format struct passed to IDirectInputDevice::SetDataFormat (vtable +0x2C) in DirectInput_InstallMouse |
| `0x1034F92C` | `byte_1034F92C` | `g_mouseAcquired` | medium | set 1/0 around DirectInput device acquisition in DirectInput_InstallMouse |
| `0x103501A8` | `flt_103501A8` | `g_feedbackQuad` | medium | start of the 20-byte-stride XYZRHW\|TEX1 vertex array drawn with DrawPrimitiveUP(TRIANGLESTRIP, 2) in Post_FlashAndZoomFeedback |
| `0x103515BC` | `dword_103515BC` | `c_ps2White` | high | initial value 0x80808080: PS2 colour 1.0 on all channels; multiplied with g_modelColor >>7 when the mesh has no colour |
| `0x10351D60` | `dword_10351D60` | `Sky_DrawDome_textureStageStates` | medium | 3rd arg of StateBlock_ApplyRSTSS in Sky_DrawDome: texture-stage-state list |
| `0x10351DB8` | `dword_10351DB8` | `Sky_DrawDome_renderStates` | medium | 2nd arg of StateBlock_ApplyRSTSS in Sky_DrawDome: render-state list (int[16]) |
| `0x10351DF8` | `dword_10351DF8` | `Sky_DrawDomeViewCentered_textureStageStates` | medium | texture-stage-state list passed to StateBlock_ApplyRSTSS in Sky_DrawDomeViewCentered |
| `0x10351E4C` | `dword_10351E4C` | `Sky_DrawDomeViewCentered_renderStates` | medium | render-state list passed to StateBlock_ApplyRSTSS in Sky_DrawDomeViewCentered |
| `0x10352D30` | `dword_10352D30` | `Font_DrawProjectedText3D_textureStageStates` | medium | TSS list passed to StateBlock_ApplyTSS in Font_DrawProjectedText3D |
| `0x10353378` | `dword_10353378` | `CMoDistRingElem_textureStageStates` | medium | TSS list passed to StateBlock_ApplyTSS in CMoDistRingElem_AllocRingGeometry |
| `0x1035F638` | `byte_1035F638` | `aNoOwnerChocobo` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: チョコボの相手がいません。チョコボコンストラクタをスキップします。 "No owner for the chocobo; skipping the chocobo constructor." |
| `0x1035F680` | `byte_1035F680` | `aNoOwnerGroundTarget` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: グランドターゲットを持つ相手がいません。ターゲットコンストラクタをスキップします。 "No owner holding a ground target; skipping the target constructor." |
| `0x1035F6D8` | `byte_1035F6D8` | `aNoOwnerRange` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: レンジを持つ相手がいません。ターゲットコンストラクタをスキップします。 "No owner holding a range; skipping the target constructor." |
| `0x1035F728` | `byte_1035F728` | `aNoOwnerBorder` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: ボーダーを持つ相手がいません。ボーダーコンストラクタをスキップします。 "No owner holding a border; skipping the border constructor." |
| `0x1035F778` | `byte_1035F778` | `aNoOwnerChair` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: 椅子を持つ相手がいません。椅子コンストラクタをスキップします。 "No owner holding a chair; skipping the chair constructor." |
| `0x1035F7C0` | `byte_1035F7C0` | `aNoOwnerFishingRod` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: 釣り竿を持つ相手がいません。釣り竿コンストラクタをスキップします。 "No owner holding a fishing rod; skipping the fishing-rod constructor." |
| `0x1035F808` | `byte_1035F808` | `aNoOwnerMagicCircle` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_Constructor_Factory: 魔方陣を持つ相手がいません。魔方陣コンストラクタをスキップします。 "No owner holding a magic circle; skipping the magic-circle constructor." |
| `0x1035F870` | `byte_1035F870` | `aSubActorNoMainChocobo` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_SetModel: サブアクター扱い(チョコボ)なのに、メインアクターがいません。 "Treated as a sub-actor (Chocobo) but there is no main actor." |
| `0x1035F8B0` | `byte_1035F8B0` | `aSubActorNoMainMagicCircle` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_SetModel: サブアクター扱い(魔方陣)なのに、メインアクターがいません。 "Treated as a sub-actor (MagicCircle) but there is no main actor." |
| `0x1035F8EC` | `byte_1035F8EC` | `aSubActorNoMainChair` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_SetModel: サブアクター扱い(椅子)なのに、メインアクターがいません。 "Treated as a sub-actor (Chair) but there is no main actor." |
| `0x1035F928` | `byte_1035F928` | `aSubActorNoMainRange` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_SetModel: サブアクター扱い(レンジ)なのに、メインアクターがいません。 "Treated as a sub-actor (Range) but there is no main actor." |
| `0x1035F964` | `byte_1035F964` | `aSubActorNoMainFishingRod` | high | Shift-JIS string (read from the IDA listing), passed only to nullsub_12, a compiled-out debug print, in CXiSkeletonActor_SetModel: サブアクター扱い(釣り竿)なのに、メインアクターがいません。 "Treated as a sub-actor (FishingRod) but there is no main actor." |
| `0x103601BC` | `off_103601BC` | `g_pGameMode` | high | object passed to GameMode_RequestState/GameMode_PollPendingState/GameMode_OpenTacticsUrl in App_StepGameState5..9 |
| `0x1036E4EC` | `dword_1036E4EC` | `g_cursorShapes` | medium | cursor shape table, 16 ints per shape indexed by cursor+0x4C; Cursor_Init fills it when empty |
| `0x1036E4F4` | `dword_1036E4F4` | `g_cursorShapeRotation` | medium | float in the cursor shape record, passed to Mat4_PostRotateZ in Cursor_Draw |
| `0x1036E4F8` | `word_1036E4F8` | `g_cursorShapeFrame` | medium | animation frame counter in the cursor shape record, incremented and wrapped in Cursor_Draw |
| `0x103B4910` | `dword_103B4910` | `g_sqgxVertexShaderHandle` | high | int[1024] of handles passed to SetVertexShader in sqgxVertexShaderSelect |
| `0x103B5910` | `dword_103B5910` | `g_sqgxVertexShaderKeyA` | medium | first word of the (key A, key B) pair searched for in sqgxVertexShaderSelect |
| `0x103B5914` | `dword_103B5914` | `g_sqgxVertexShaderKeyB` | medium | second word of the key pair in sqgxVertexShaderSelect |
| `0x103D3C64` | `dword_103D3C64` | `g_pApp` | high | holds the CApp created by App_CreateRendererAndApp (stores CApp vtable, data/global_objects.json); +0x18 is the HWND passed to GetWindowRect, +0x1C an active flag; released through vtable slot 6 in sub_10016320 |
| `0x103D3CB8` | `dword_103D3CB8` | `g_pAppSettings` | medium | pointer to the settings block read at startup: +0x58 passed to Gfx_CreateD3DHolder as its last flag, +0x5C gates the env-map pass, +0x28 is the network/login state set to 3 after _snprintf("net3 port 54001") |
| `0x10456908` | `dword_10456908` | `g_modelDrawCallCount` | medium | reset to 0 in Frame_Render and incremented once per DrawIndexedPrimitive in CYyModelDt_SkinAndDrawV2 |
| `0x10456918` | `dword_10456918` | `g_afterimageDepth` | medium | depth surface returned by CDx_CreateTexture next to g_afterimageTex; pushed with it in Post_Afterimage |
| `0x10456920` | `dword_10456920` | `g_afterimageState` | medium | 1 set by CMoSchedularTask_Interpret to start, 2 while running in Post_Afterimage |
| `0x10456924` | `dword_10456924` | `g_afterimageBufferState` | medium | per-buffer state (1/2) in Post_Afterimage |
| `0x10456928` | `dword_10456928` | `g_afterimagePhase` | medium | 1/2 phase flag of Post_Afterimage |
| `0x1045692C` | `dword_1045692C` | `g_feedbackBufferIndex` | medium | ping-pong index in Post_FlashAndZoomFeedback, reset by CMoSchedularTask_Interpret |
| `0x10456930` | `dword_10456930` | `g_feedbackBufferState` | medium | per-buffer state (1/2) in Post_FlashAndZoomFeedback |
| `0x10456934` | `dword_10456934` | `g_feedbackActive` | medium | 0/1 in Post_FlashAndZoomFeedback |
| `0x10456940` | `dword_10456940` | `g_feedbackDepth` | medium | depth surfaces created in App_InitScene, pushed with g_feedbackTexHalfRes in Post_FlashAndZoomFeedback |
| `0x104569B0` | `dword_104569B0` | `g_cmdLine` | medium | string passed to App_SetCommandLine in App_WinMain |
| `0x104569B8` | `dword_104569B8` | `g_installDir` | medium | path string passed to App_LoadImeUiDll and App_LoadKeyInputDll (DLL search directory) and to the CApp constructor |
| `0x10456A74` | `dword_10456A74` | `g_cfgGraphicsStabilization` | medium | registry setting key "0040" (config table at .rdata: {&global, key} pairs read by the settings loader); meaning from the public FFXI settings list; EnvMap_BeginPass reads it |
| `0x10456A88` | `dword_10456A88` | `g_cfgSoundEnabled` | high | registry setting key "0007" (config table at .rdata: {&global, key} pairs read by the settings loader); App_WinMain calls App_InitSoundSystem only when nonzero |
| `0x10456AA0` | `dword_10456AA0` | `g_cfgFontCompression` | high | registry setting key "0036" (config table at .rdata: {&global, key} pairs read by the settings loader); Font_LoadHighResTexture uses value-1 as the row of the {file id, width, height} high-res font table; 0 keeps the compressed font |
| `0x10456ACC` | `dword_10456ACC` | `g_shadowSetting` | medium | not a registry key; set by the setter sub_10002D50 (default 0); 1 skips actor shadows, 2 allows render-to-texture shadows up to distance 40 |
| `0x10456BF8` | `dword_10456BF8` | `g_cfgHardwareMouse` | high | registry setting key "0021" (config table at .rdata: {&global, key} pairs read by the settings loader); gates the hardware-cursor path in CDx_SetCursorShape, Cursor_SetPosition, Cursor_Draw, Cursor_SetVisible |
| `0x10456C00` | `dword_10456C00` | `g_cfg0020` | high | registry setting key "0020" (config table at .rdata: {&global, key} pairs read by the settings loader); meaning not identified; switched on in PerActor_FourBucketStaggeredUpdate; default 1 |
| `0x10456D48` | `byte_10456D48` | `g_translucentForce` | medium | when 0 and the model colour alpha is 0x80 (opaque in PS2 units) the translucent pass is skipped |
| `0x10456D4C` | `dword_10456D4C` | `g_translucentColor` | medium | PS2 colour (0x80 = 1.0); its alpha decides whether Model_*Translucent* run, written into the composite quad vertices |
| `0x10456D54` | `dword_10456D54` | `g_translucentTargetEye2` | medium | second-eye texture from Model_CreateTranslucentTargets, returned by CDx_GetStereoSecondEyeTarget |
| `0x10456D58` | `dword_10456D58` | `g_translucentTarget` | medium | texture from Model_CreateTranslucentTargets; the translucent models are drawn into it |
| `0x10456D5C` | `dword_10456D5C` | `g_translucentDepth` | medium | depth surface returned by CDx_CreateTexture with the translucent target |
| `0x10456DD0` | `flt_10456DD0` | `g_modelLight` | high | 0x68-byte D3DLIGHT8 copy passed to CDx_SetLight in CYyModelDt_PrepareRenderState |
| `0x10456DD4` | `flt_10456DD4` | `g_modelLightDiffuseR` | high | g_modelLight+4 = D3DLIGHT8.Diffuse.r, scaled per mesh |
| `0x10456DD8` | `flt_10456DD8` | `g_modelLightDiffuseG` | high | g_modelLight+8 = Diffuse.g |
| `0x10456DDC` | `flt_10456DDC` | `g_modelLightDiffuseB` | high | g_modelLight+0xC = Diffuse.b |
| `0x10456E50` | `dword_10456E50` | `g_projTexFactor` | medium | TEXTUREFACTOR set by ProjTex_BeginPass, captured by ProjTex_CaptureToTarget |
| `0x10456E58` | `dword_10456E58` | `g_projTexPrevTex0` | medium | texture slot 0 saved by ProjTex_BeginPass, restored by ProjTex_EndPass |
| `0x10456E5C` | `dword_10456E5C` | `g_projTexPrevTex1` | medium | texture slot 1 saved by ProjTex_BeginPass, restored by ProjTex_EndPass |
| `0x10456E64` | `dword_10456E64` | `g_projTexTarget` | medium | texture captured in ProjTex_CaptureToTarget and bound to stage 0 |
| `0x10456E68` | `dword_10456E68` | `g_projTexPrevFactor` | medium | TEXTUREFACTOR saved by ProjTex_BeginPass, restored by ProjTex_EndPass |
| `0x10456E7C` | `dword_10456E7C` | `g_envMapPrevFactor` | medium | TEXTUREFACTOR saved by EnvMap_BeginPass, restored by EnvMap_EndPass |
| `0x10456E80` | `dword_10456E80` | `g_envMapPrevTex0` | medium | texture slot 0 saved by EnvMap_BeginPass, restored by EnvMap_EndPass |
| `0x10456E84` | `dword_10456E84` | `g_envMapPrevTex1` | medium | texture slot 1 saved by EnvMap_BeginPass, restored by EnvMap_EndPass |
| `0x10456E8C` | `dword_10456E8C` | `g_envMapFactor` | medium | TEXTUREFACTOR set by EnvMap_BeginPass |
| `0x104570FC` | `byte_104570FC` | `g_envMapEnabled` | medium | gates EnvMap_BeginPass in CYyModelDt_SkinAndDrawV2 and CXiSkeletonActor_DrawModel; cleared per actor |
| `0x10457108` | `dword_10457108` | `g_texture1Matrix` | medium | identity set in DrawModel; set as D3DTS_TEXTURE1 transform |
| `0x10457158` | `dword_10457158` | `g_envMapTexMatrix` | high | matrix built in CXiSkeletonActor_DrawModel and passed to EnvMap_BeginPass |
| `0x10457198` | `flt_10457198` | `g_shadowTexMatrix` | medium | texture-1 transform built from g_lightViewMatrix in CXiSkeletonActor_DrawModel |
| `0x104571D8` | `unk_104571D8` | `g_ambientColor` | high | D3DRS_AMBIENT value in CYyModelBase_DrawLit; filled by XiZone_GetAmbient in Actor_SelectNearestLights |
| `0x104571D9` | `byte_104571D9` | `g_ambientColorG` | high | byte 1 of g_ambientColor (ARGB little-endian: green) |
| `0x104571DA` | `byte_104571DA` | `g_ambientColorR` | high | byte 2 of g_ambientColor (red) |
| `0x104571DB` | `byte_104571DB` | `g_ambientColorA` | high | byte 3 of g_ambientColor (alpha) |
| `0x104571E0` | `unk_104571E0` | `g_modelMaterial` | high | D3DMATERIAL8 passed to SetMaterial; initialised from unk_10351518 |
| `0x10457210` | `flt_10457210` | `g_modelMaterialEmissiveR` | high | g_modelMaterial (D3DMATERIAL8 at 0x104571E0) + 0x30/0x34/0x38 = Emissive.r/g/b; set to (mesh value - 1.5) in CYyModelDt_PrepareRenderState |
| `0x10457214` | `flt_10457214` | `g_modelMaterialEmissiveG` | high | g_modelMaterial (D3DMATERIAL8 at 0x104571E0) + 0x30/0x34/0x38 = Emissive.r/g/b; set to (mesh value - 1.5) in CYyModelDt_PrepareRenderState |
| `0x10457218` | `flt_10457218` | `g_modelMaterialEmissiveB` | high | g_modelMaterial (D3DMATERIAL8 at 0x104571E0) + 0x30/0x34/0x38 = Emissive.r/g/b; set to (mesh value - 1.5) in CYyModelDt_PrepareRenderState |
| `0x104572B0` | `byte_104572B0` | `g_projTexEnabled` | medium | gates ProjTex_BeginPass in CYyModelDt_SkinAndDrawV2; cleared per actor |
| `0x104574A8` | `dword_104574A8` | `g_modelPrimitiveCount` | medium | accumulated per DrawIndexedPrimitive in CYyModelDt_SkinAndDrawV2, reset and stored to actor+0xB4 in DrawModel |
| `0x104574AC` | `dword_104574AC` | `g_modelColor` | high | per-draw model colour in PS2 units: CXiSkeletonActor_DrawModel stores a8 (or a8 alpha \| 0x808080) and the draw code multiplies channels by it >>7 |
| `0x104574B8` | `dword_104574B8` | `g_identityWorldMatrix` | medium | Mat4_SetIdentity then set as D3DTS_WORLD before drawing actors |
| `0x104574F8` | `dword_104574F8` | `g_projTexMatrix` | high | matrix built in CXiSkeletonActor_DrawModel and passed to ProjTex_BeginPass |
| `0x10457554` | `dword_10457554` | `g_streamSourceStride` | high | int[5]: stride argument of SetStreamSource(stream, vb, stride[stream]) in Gfx_ApplyCachedStreamSource |
| `0x10457568` | `flt_10457568` | `g_skinLight` | high | 0x68-byte D3DLIGHT8 copy passed to CDx_SetLight in CYyModelDt_SkinAndDrawV2 |
| `0x1045756C` | `flt_1045756C` | `g_skinLightDiffuseR` | high | g_skinLight+4 = Diffuse.r |
| `0x10457570` | `flt_10457570` | `g_skinLightDiffuseG` | high | g_skinLight+8 = Diffuse.g |
| `0x10457574` | `flt_10457574` | `g_skinLightDiffuseB` | high | g_skinLight+0xC = Diffuse.b |
| `0x1045B62C` | `dword_1045B62C` | `g_lineVertexCursor` | medium | running vertex offset (x32 bytes) into a 1024-vertex buffer in CYyModelDt_PrepareRenderState; reset to 0 on wrap |
| `0x1045B630` | `dword_1045B630` | `g_stripVertexCursor` | medium | start vertex for CDx_DrawPrimitive(TRIANGLESTRIP) in a 512-vertex buffer; reset on wrap |
| `0x1045B650` | `dword_1045B650` | `g_os2VtxBufferSlots` | medium | int[16] of CYyOs2VtxBuffer pointers; CYyOs2VtxBuffer_Destructor clears the slot that holds this |
| `0x1045B690` | `dword_1045B690` | `g_streamSourceVB` | high | vertex buffer argument of SetStreamSource in Gfx_ApplyCachedStreamSource, indexed by stream |
| `0x1045B77A` | `word_1045B77A` | `g_actorShadowMode` | medium | per-actor shadow mode set in PerActor_FourBucketStaggeredUpdate: 1 = render-to-texture shadow, 0/2 = blob (2 if actor+0x17C), 3 = none |
| `0x1045B780` | `dword_1045B780` | `g_offscreenWorldMatrix` | medium | set as D3DTS_WORLD (256) in Model_BeginOffscreenPass |
| `0x1045B7C0` | `flt_1045B7C0` | `g_lightViewMatrix` | medium | D3DXMatrixLookAtRH from the light position; set as D3DTS_VIEW in Model_BeginOffscreenPass |
| `0x1045B800` | `dword_1045B800` | `g_offscreenPrevTex0` | medium | texture slot 0 saved by Model_BeginOffscreenPass, rebound by Model_EndOffscreenPassBindTextures |
| `0x1045B804` | `dword_1045B804` | `g_offscreenPrevTex1` | medium | texture slot 1 saved by Model_BeginOffscreenPass, rebound by Model_EndOffscreenPassBindTextures |
| `0x1045B80C` | `flt_1045B80C` | `g_shadowCasterDistance` | medium | actor distance (+0xBC); shadow pass only when < 4.0 (40.0 when quality setting is 2) |
| `0x1045EC28` | `dword_1045EC28` | `g_boneTouched` | medium | int[256] flags set per bone index in CXiSkeletonActor_PoseCompose |
| `0x1045F040` | `dword_1045F040` | `g_poseTranslation` | medium | uninitialised vec3 written by MotionQueue_UpdateAllChannels and applied with Mat4_SetTranslation in CXiSkeletonActor_PoseCompose |
| `0x1045F04C` | `dword_1045F04C` | `g_poseScale` | medium | uninitialised vec3 written by MotionQueue_UpdateAllChannels and applied with Mat4_SetScale in CXiSkeletonActor_PoseCompose |
| `0x1047B958` | `dword_1047B958` | `g_sceneCaptureTex` | medium | 256x256 texture created in Post_CreateScreenEffectTargets; target of Post_CaptureSceneToTarget, drawn by Post_DrawRotatingGhostLayers |
| `0x1047B95C` | `dword_1047B95C` | `g_sceneCaptureTexEye2` | medium | second capture texture, used only when CEnv_IsStereoMode |
| `0x1047B960` | `dword_1047B960` | `g_sceneCaptureDepth` | medium | depth surface returned with g_sceneCaptureTex |
| `0x1047B964` | `dword_1047B964` | `g_sceneCaptureDepthEye2` | medium | depth surface returned with g_sceneCaptureTexEye2 |
| `0x1047B974` | `dword_1047B974` | `g_pVbMng` | high | CYyVbMng singleton (data/global_objects.json); first arg of CYyVbMng_AcquireVertexBuffer |
| `0x1047B984` | `byte_1047B984` | `Sky_DrawDomeViewCentered_staticGuard` | high | `if ((g & 1) == 0) { g \|= 1; ... }` at entry of Sky_DrawDomeViewCentered: MSVC function-local static init guard; guards dword_1047B994 state block |
| `0x1047B985` | `byte_1047B985` | `Sky_DrawDome_staticGuard` | high | MSVC static init guard in Sky_DrawDome; guards the dword_1047B988 state block |
| `0x1047B988` | `dword_1047B988` | `Sky_DrawDome_stateBlock` | high | static object constructed under Sky_DrawDome_staticGuard, then StateBlock_ApplyRSTSS(it, rs, tss) |
| `0x1047B994` | `dword_1047B994` | `Sky_DrawDomeViewCentered_stateBlock` | high | static state block applied with StateBlock_ApplyRSTSS in Sky_DrawDomeViewCentered |
| `0x1047BAC0` | `byte_1047BAC0` | `Font_DrawProjectedText3D_staticGuard` | high | MSVC static init guard in Font_DrawProjectedText3D |
| `0x1047BADC` | `dword_1047BADC` | `Font_DrawProjectedText3D_stateBlock` | high | static state block applied with StateBlock_ApplyTSS in Font_DrawProjectedText3D |
| `0x1047BB78` | `byte_1047BB78` | `CMoDistRingElem_AllocRingGeometry_staticGuard` | high | MSVC static init guard in CMoDistRingElem_AllocRingGeometry |
| `0x1047BB7C` | `dword_1047BB7C` | `CMoDistRingElem_stateBlock` | high | static state block applied with StateBlock_ApplyTSS in CMoDistRingElem_AllocRingGeometry |
| `0x1047BB88` | `dword_1047BB88` | `g_pMoAccumTarget` | medium | CAcc object from Mo_InitSharedRenderResources; +4 is a 256x256 texture from CMoDX_CreateTexture and +8 its depth surface |
| `0x1047BB90` | `dword_1047BB90` | `g_cubemapSpecName` | high | 16-byte resource name "cubemap spec    " resolved with TexRef_ResolveByName in Mo_InitSharedRenderResources |
| `0x1047BBA0` | `dword_1047BBA0` | `g_pDynVB_FVF42` | medium | DynVB created with FVF 0x42 (XYZ\|DIFFUSE), 4096 vertices, in Mo_InitSharedRenderResources |
| `0x1047BBA4` | `dword_1047BBA4` | `g_pDynVB_FVF44` | medium | DynVB created with FVF 0x44 (XYZRHW\|DIFFUSE), 4096 vertices |
| `0x1047BBA8` | `dword_1047BBA8` | `g_pDynVB_FVF144` | medium | DynVB created with FVF 0x144 (XYZRHW\|DIFFUSE\|TEX1), 4096 vertices |
| `0x1047BBAC` | `dword_1047BBAC` | `g_pDynVB_FVF142` | medium | DynVB created with FVF 0x142 (XYZ\|DIFFUSE\|TEX1), 4096 vertices |
| `0x1047BBB0` | `dword_1047BBB0` | `g_pDynVB_FVF152` | medium | DynVB created with FVF 0x152 (XYZ\|NORMAL\|DIFFUSE\|TEX1), 0x2000 vertices; used by the CMoDist mesh builders |
| `0x1047BBB4` | `dword_1047BBB4` | `g_pMoCamera` | medium | created in Mo_InitSharedRenderResources; Sky_DrawDome sets the view matrix from its fields, Camera_ApplyViewProjection updates it |
| `0x1047BFF4` | `dword_1047BFF4` | `g_pDxStatusMng` | high | result of CMoDxStatusMng_Constructor in CMoDX_CreateAndInitSystem |
| `0x1047BFF8` | `dword_1047BFF8` | `g_pVbPool` | high | result of VBPool_Constructor in CMoDX_CreateAndInitSystem |
| `0x1047C004` | `dword_1047C004` | `g_pOcclusionMng` | high | CMoOcclusionMng singleton created in CMoDX_CreateAndInitSystem; passed to OcclusionMng_CreateTarget/Prepare |
| `0x1047CF31` | `byte_1047CF31` | `CMoProcessor_InitRotationMatrices_staticGuard` | high | MSVC static init guard in CMoProcessor_InitRotationMatrices |
| `0x1047D130` | `dword_1047D130` | `g_resourceFindDepth` | high | incremented on entry of CMoResource_FindByTypeAndId and decremented on each exit path of the CXiSkeletonActor resource-search functions |
| `0x1047D13C` | `byte_1047D13C` | `g_resourcesDirty` | medium | set to 1 after resource loads in App_InitScene, CMoSchedularTask_Interpret and CXiSkeletonActor_LoadSkeletonResource |
| `0x1047D150` | `dword_1047D150` | `g_cubeMapTargetHeight` | medium | compared with the requested height (+0x26) and passed as height to CMoDX_CreateTexture |
| `0x1047D158` | `dword_1047D158` | `g_pCubeMapTarget` | medium | render target recreated by TexRef_EnsureRenderTarget when the requested size changes; +4 texture from CMoDX_CreateTexture |
| `0x1047D200` | `dword_1047D200` | `g_cubeMapTargetWidth` | medium | compared with the requested width (+0x24) in TexRef_EnsureRenderTarget |
| `0x1047D580` | `byte_1047D580` | `g_actorNameBuffer` | high | static buffer CXiSkeletonActor_GetEnglishName strcpy()s the display name into and returns |
| `0x1047D600` | `dword_1047D600` | `g_targetActorHandle` | medium | handle resolved with ActorHandle_Resolve and compared with actors in OnMove/OnDrawName/DestructorBody |
| `0x1048A308` | `dword_1048A308` | `g_nearestLight` | medium | pointer to the selected light record (actor + 0x388 + 104*i) chosen in Actor_SelectNearestLights |
| `0x1048A30C` | `dword_1048A30C` | `g_lightLookAtTarget` | medium | static vec3 (x at +0, y +4, z +8): LookAt target for g_lightViewMatrix |
| `0x1048A318` | `dword_1048A318` | `g_lightLookAtEye` | medium | static vec3: LookAt eye for g_lightViewMatrix |
| `0x1048A328` | `byte_1048A328` | `PerActor_FourBucketStaggeredUpdate_staticGuards` | high | bits 1 and 2 are MSVC static init guards for the two static vectors/matrices of PerActor_FourBucketStaggeredUpdate (dword_1048A30C.., dword_1048A32C..) |
| `0x1048A338` | `dword_1048A338` | `g_lightLookAtUp` | medium | static vec3 (0,0,-1): LookAt up vector for g_lightViewMatrix |
| `0x1048A350` | `dword_1048A350` | `g_actorUpdateCounter` | medium | incremented per actor and compared with dword_1048A354 for the staggered update |
| `0x1048A354` | `dword_1048A354` | `g_actorUpdateInterval` | medium | limit for g_actorUpdateCounter |
| `0x104E1B88` | `dword_104E1B88` | `g_textCursorX` | medium | Obj_104E1B90_DrawString stores its x argument here and advances it by each glyph width |
| `0x104E1B8C` | `dword_104E1B8C` | `g_textCursorY` | medium | Obj_104E1B90_DrawString stores its y argument here |
| `0x104E1B90` | `dword_104E1B90` | `g_pFontGlyphs` | medium | singleton created in App_InitScene; Font_DrawPass binds it with Font_BindGlyphTexture |
| `0x104E1BF8` | `dword_104E1BF8` | `g_shapeMng` | high | static object passed to ShapeMng_Init, ShapeMng_GetFontGlyph, ShapeMng_BindFontTexture, ShapeMng_GetDmgNumGlyph, ShapeMng_BindDmgNumTexture |
| `0x104E1D4C` | `dword_104E1D4C` | `g_pCursor` | high | cursor singleton (1 Oct, above); passed to Cursor_GetHwCursorSlot |
| `0x10578158` | `dword_10578158` | `g_pChatLog` | high | first arg of ChatLog_AddMessage in CXiSkeletonActor_SetAttack; created in App_InitScene |
| `0x1057876C` | `dword_1057876C` | `g_pTargetMng` | medium | object used by Actor_SelectNearestLights, Frame_Render and CXiSkeletonActor_DestructorBody |
| `0x10621838` | `word_10621838` | `g_menuMng` | high | static menu manager: first arg of MenuMng_OpenByName(.., "menu    chdummy", ..) in Frame_Render and App_StepGameState6..8 |
| `0x106218B4` | `dword_106218B4` | `g_screenRectTopLeft` | medium | packed int16 x (low word) / y (high word); every reader takes g_screenRectBottomRight - this for width/height (Cursor_Init, Cursor_SetPosition) |
| `0x106218B8` | `dword_106218B8` | `g_screenRectBottomRight` | medium | packed int16 x/y; minus g_screenRectTopLeft gives the screen width/height; sub_10124640 clamps the cursor to it |
| `0x10621A08` | `dword_10621A08` | `g_dbgBoxList` | medium | linked list walked by Dbg_DrawOrientedBoxes |
| `0x10624DD3` | `unk_10624DD3` | `c_div2000Magic` | high | not data: the address is the magic constant 0x10624DD3 of the unsigned divide-by-2000 sequence in CYyVbMng_AcquireVertexBuffer (round up to multiple of 2000) |
| `0x10639088` | `unk_10639088` | `g_zoneProjVerts` | medium | vertex buffer (24-byte stride) drawn with DrawPrimitiveUP(TRIANGLELIST) by the CTsZoneMap projected texture/shadow functions |
| `0x10662DCC` | `dword_10662DCC` | `g_pFileMng` | medium | object passed to the file functions sub_101C4AF0/CC0/CD0/D10; Font_LoadHighResTexture loads through it |
| `0x10666E7C` | `dword_10666E7C` | `g_pMovie` | high | object passed to Movie_GetCurrentTexture in CXiMovie_DrawFrame; Frame_Render skips the scene while it is set |
| `0x10668FD8` | `dword_10668FD8` | `g_pKeyPrompt` | medium | object passed to Ui_KeyPrompt_Init in Frame_Render and App_StepGameState6..9 |
| `0x10669400` | `dword_10669400` | `g_dancerMainMemSize` | high | size argument of Dancer_SetMemPool(0, base, size, "mainmem") in Dancer_InitDevice |
| `0x10669404` | `dword_10669404` | `g_dancerMainMemBase` | high | base argument of Dancer_SetMemPool(0, base, size, "mainmem") |
| `0x10669408` | `dword_10669408` | `g_dancerMemBlock` | medium | the allocation Dancer_InitDevice carves the mainmem/workmem pools from |
| `0x1066940C` | `dword_1066940C` | `g_dancerWorkMemSize` | high | set to 0x200000 and passed as size to Dancer_SetMemPool(1, base, size, "workmem") |
| `0x10669410` | `dword_10669410` | `g_dancerWorkMemBase` | high | mainmem base + mainmem size; base argument of Dancer_SetMemPool(1, .., "workmem") |
| `0x1099D200` | `dword_1099D200` | `g_pSqgxContext` | high | IDB name (parallel session, accepted) |
| `0x1099D618` | `dword_1099D618` | `g_sqgxTex_name` | medium | sqgx texture table entry +0 (stride 10 ints): a4 of the texture-add function |
| `0x1099D61C` | `dword_1099D61C` | `g_sqgxTex_width` | high | entry +4: width (CreateTexture width; divides context width in sqgxTextureDraw) |
| `0x1099D620` | `dword_1099D620` | `g_sqgxTex_height` | high | entry +8: height (CreateTexture height; divides context height in sqgxTextureDraw) |
| `0x1099D624` | `dword_1099D624` | `g_sqgxTex_kind` | medium | entry +0xC: kind; 1 means paletted (sqgxTextureSetPalette only acts when 1), 4 a level-chain upload |
| `0x1099D628` | `dword_1099D628` | `g_sqgxTex_log2Width` | high | entry +0x10 = sqgxCeilLog2(width) |
| `0x1099D62C` | `dword_1099D62C` | `g_sqgxTex_log2Height` | high | entry +0x14 = sqgxCeilLog2(height) |
| `0x1099D630` | `dword_1099D630` | `g_sqgxTex_format` | high | entry +0x18: D3DFORMAT passed to CreateTexture |
| `0x1099D634` | `dword_1099D634` | `g_sqgxTex_paletteSlot` | medium | entry +0x1C: initialised to -2 (no palette) |
| `0x1099D638` | `dword_1099D638` | `g_sqgxTex_texture` | high | entry +0x20: IDirect3DTexture8* passed to SetTexture |
| `0x1099D63C` | `dword_1099D63C` | `g_sqgxTex_surface` | high | entry +0x24: surface passed to SetRenderTarget in sqgxTextureSetRenderTarget/sqgxTextureDraw |
| `0x109A2618` | `dword_109A2618` | `g_sqgxRenderStateDirty` | medium | set by sqgxZTest and sqgxSetAlphaTest, consumed and cleared by sub_10281DB0 |
| `0x109A261C` | `dword_109A261C` | `g_counter_109A261C` | high | IDB name |
| `0x109A2620` | `byte_109A2620` | `g_sqgxPaletteUsed` | high | byte[256]: sqgxTextureAddPalette takes the first entry that is 0 and sets it to 1 |
| `0x109A2730` | `dword_109A2730` | `g_sqgxViewportDirty` | medium | set by sqgxSetViewport/sqgxZBiasAdd/sqgxZBiasSet, cleared by sqgxFlushTransformsLights |
| `0x109A2740` | `dword_109A2740` | `g_sqgxShaderDirty` | medium | set by sqgxVertexShaderCreate and sqgxContext_SelectShaderMode |
| `0x109F81A4` | `byte_109F81A4` | `CMoCamera_GetSharedInstance_staticGuard` | high | MSVC static init guard of the function-local static instance in CMoCamera_GetSharedInstance |
