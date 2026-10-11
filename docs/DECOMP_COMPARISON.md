# An outside decompilation of FFXiMain.dll compared with ours

The owner received a write-up of the client's renderer: prose documents, one entry per function (769), and a source
tree of about 159,000 lines (1,381 files). This document records how its decompilation differs from this
repository's reconstruction (`tree/`), and how each difference was established. Nothing from it is in this
repository except function names, addresses and the counts below. The comparison scripts were one-off; their method
is given with each result, so it can be repeated.

## At a glance

| | Theirs | Ours (`tree/`) |
|---|---|---|
| Build | an older FFXiMain.dll (sha256 `392e847a...`, 6,206,976 bytes) | the analysed build (sha256 `1b9b59ae...`, 6,208,512 bytes, PE timestamp 0x6A995428; `data/binary.json`) |
| Scope | the 3D renderer and what it touches: frame, camera, zone, actors, models, effects, environment, textures, the Dancer library | the graphics functions of the IDA export: 777 functions |
| Form | hand-restructured C and C++: placeholder classes per vtable (`C_<vtable>`), every virtual call as a slot (`vfn_c8`), globals as `g_<address>` symbols, per-file call macros (`RS`, `TSS`, `CALL_FC`) | Hex-Rays output cleaned by `pipeline/hx2clean.py`: typed with the DirectX 8 headers, calls to other game functions as `orig::Name`, locals at their original frame offsets (`xi_frame`) |
| Checked how | each claim cites instructions or source lines; no tool runs the source against the game | every function compiled and run against the game's own code in the emulator (`fork.py gate` / `diffemu_gfx`): 774 of 777 PASS |
| Runs in the game | no: its globals and calls are plain symbols with no binding to the game's addresses | yes: `gfx_recon_ingame.dll`, swapped in function by function by the kit (`xikit.ini swap=1`) |
| Prose | an entry per function: what, how, cost, waste, hook, evidence | names and one-line descriptions (`docs/NAMES.md`), the system documents in `docs/` |

## Addresses: the two builds side by side

The builds differ, so their addresses are not ours. Each of their 769 entries was mapped to our build:
- **245 by signature.** The entry's own byte signature matched exactly one place in our build, and that place is a
  function start.
- **454 by drift.** The entry has no signature. The nearest signature-mapped entry gave the offset between the
  builds; the mapping was kept only where our function at that address has the same span (start to the next
  function's start) as the entry's stated size.
- **70 not mapped.**

Their "size" is that span, alignment padding included, not the code length. Where the two disagree, it is mostly
how functions are split: their 0x1002F100 (48 bytes) and 0x1002F130 (3,728) are our one 0x1002F1D0 (3,776). A few
are code that changed between the builds, for example the CPU detection 0x1000D400 (512 bytes in theirs, 544 in
ours).

The drift is not constant: 0 at the start of `.text`, then +0x40, +0xD0, +0xE0 and more further on (for example
their 0x100119C0 is our Frame_Render 0x10011A00, their 0x1006C7E0 our readback 0x1006C8B0). Data addresses moved
too: their `g_1047bf74` is our processor pointer 0x1047CF7C. Of their 769 entries, **176 are functions in our tree**.

## Accuracy, tested

### Instruction citations
Their entries and documents cite 3,943 instructions as "`text` at 0xADDRESS". Each was translated through the
address map and compared with our disassembly at that place: mnemonic, small constants, and call targets (themselves
mapped).
- **3,212 of the 3,318 that can be checked match outright (96.8%).**
- Of the other 106: 26 are `rep` forms or C expressions; 29 match within 16 bytes; 18 match elsewhere in the same
  function (the address points at a neighbouring instruction of the same statement).
- 33 have no literal match. The ones checked by hand are the same fact at a neighbouring address (both 0x39594 and
  0x39598, 0xDC0 and 0xDC4, 0x16C and 0x19C occur in those functions), or call targets our drift map placed on another
  function of the same size.
- No invented instruction was found.

### Signatures (176 functions in both)
- **Stack arguments.** For the 169 whose every `ret N` agrees, both declare N / 4 stack arguments: theirs 169 of 169,
  ours 169 of 169. One apparent mismatch of ours is a function-pointer parameter the counting split at its commas.
- **Return values.** 75 functions are `void` in theirs and return a value in ours (Hex-Rays declares EAX as returned).
  No direct caller of any of them reads EAX before writing it:
  - 60 have direct callers, none of which reads it;
  - 14 are only called indirectly, so this check cannot say;
  - for one, a read was reported but was `or eax, -1`, a write.
  The test was each call's following instructions, with `xor r,r`, `sub r,r` and `or r,-1` counted as writes. So
  their `void` is right in practice, and ours keeps a value the game never reads.

### Virtual calls (176 functions in both)
Each side's source was reduced to the vtable slots it calls and how often:
- theirs after running it through the C preprocessor, which expands their per-file call macros;
- ours from its typed methods and slot offsets.

**110 of 176 call the same slots the same number of times.** Of the Direct3D calls in the rest, counted against the
binary (`call [reg+N]`, and `call reg` after `mov reg, [x+N]`), ours agreed 23 times and theirs 11, and in 9 neither
did. The cases read by hand are differences of representation, not wrong calls:
- **Two source calls for one machine call.** Their `if (c) RS(0x1a, 1); else RS(0x1a, 0);` (0x10009920) is one
  call site in the binary with a computed value; Hex-Rays keeps the binary's shape.
- **Calls through named helpers or pointer arithmetic** that one side's counting did not resolve.
- **Function boundaries.** Our extent table can miss a chunk of a function, so the binary count is not always the
  referee.

No call that exists on one side and not in the game was confirmed.

### Their own source, where their prose says it is wrong
Their entries say so where the source and the instructions disagree, for example:
- 10069d00: the source stores zeros to +0xEA0, +0xEA4 and +0xEB4 but misses +0xEB8;
- 10053100: the source calls `fn_10053132`, an address inside the function, where the code computes the value inline.

The source is a readable rendering checked by reading; ours is checked by running it.

### Does it compile
One in four of their source files (280) was compiled with MinGW (`-fsyntax-only -fpermissive`, their include
directories): 227 pass and 53 fail, mostly on types they use and never declare. It does not link to the game:
`g_10455664->field_921` is a plain external symbol, not the game's memory. Ours compiles into the DLL the kit loads,
with every game address rebased where FFXiMain.dll is loaded (`XI_EA`, `XI_GAME`).

## Representation

| Aspect | Theirs | Ours |
|---|---|---|
| Direct3D | untyped: `dev->vfn_c8(state, value)`, the slot named in the prose (`docs/d3d8-slots.md`) | typed: `pDevice->SetRenderState(D3DRS_..., v)`, or the slot through the D3D8 headers |
| Objects | `C_<vtable address>` with `field_<offset>` members and `pad_` arrays | `this` as the IDB's class where known (`CYyModelDt`), else byte offsets (`at<uint32_t>(this, 0x5E)`) |
| Control flow | restructured by hand: early returns, named locals (`mesh`, `cmd`, `lit`) | as Hex-Rays recovers it, with the gotos and labels it needs to stay byte-exact |
| Calls to other game code | method calls on placeholder classes (`ChooseTransformRoutine()`) | `orig::Name(...)`, the game's own function at its address |
| Locals | named, typed by use | at their original stack offsets where their address is taken (`xi_frame`), so the emulator compares them |

## Names

Of the 176 shared functions, **38 still have placeholder names in ours** (`sub_...`); none in theirs. The
vocabularies differ:
- theirs is its own (`Renderer`, `XiZone`, `YmElem`, `MeshResource`), with class-record names where it found them
  (`XiDoorActor`);
- ours is the IDB's and our naming pass's (`CDx`, `CTsZoneMap`, `CMo...` from the class records).

Some disagree on what a function is, for example 0x1000A1D0 ("UnbindAndPopTarget" to them, `CDx_SetTexture` to
us) and 0x1000CA00 ("IsSecondViewDraw" / `CDx_GetStereoSecondEyeTarget`). Neither side's names were adopted without
checking the code; the full table is the appendix.

## What theirs found that this repository did not have, checked here

Each was verified in our build's code or the owner's traces before being written into our documents (HISTORY.md,
11 Oct):
- **The mirrored copy.** The second draw of a skinned model when `field_34 & 1` is a mirrored copy: one axis negated
  through a sign table at 0x103515E8 (SKINMERGE.md).
- **The foreground window.** The main loop forces the window to the foreground every frame in window mode 0, and its
  "restore" of the foreground lock timeout passes a buffer address (NAMES.md corrected).
- **Vertex-buffer warm-up.** The pool's 200 warm-up buffers are never reused.
- **Zone deferred elements.** With two layers loaded, the zone's deferred elements are drawn twice: the count at
  +0x38D40 is zeroed once, before the layer loop.
- **Camera latency.** The camera is applied once a frame, after Present (0x10012E1E, its one caller).
- **The effect drain.** In the plaza trace (frame 2, label 0) it drew 203 queued elements at 29 to 79 device calls
  each (mean 37.3, the element's own draw included; they say about 50 for the setup and restore). That is about 7,570
  calls, 18% of the frame's 41,093.

## What ours has that theirs does not

- Every function compiled and compared against the game's code (the gate), and swappable into the running game.
- Typed Direct3D, so a tool can read every device call (`render_map.json`, the kit's profiler and traces).
- Facts measured in the game: call sites, stack balances (`data/game_sp.json`), the owner's profiles, traces and
  captures.

## How to use theirs

As a map: its entries say what a function does, why, and where it costs. Its citations are 97% exact, and its
function boundaries and argument counts agree with the binary. Its source is a readable rendering, not code to build
on: it is not verified by execution, part of it does not compile, and its prose itself notes where it is wrong. Its
performance ranking is not measured (its own caveat), and the owner's measurements reorder it.

## Appendix: the 176 functions in both

| Theirs | Ours | Their name | Our name |
|---|---|---|---|
| `0x10001520` | `0x10001520` | Application::RunMessageLoop | CApp_RunMainLoop |
| `0x10002f20` | `0x10002F20` | CreateD3DWrapper | Gfx_CreateD3DHolder |
| `0x10003040` | `0x10003040` | DestroyD3DWrapper | Gfx_DestroyD3DHolder |
| `0x100031f0` | `0x100031F0` | D3DWrapper::CreateD3DListModes | Gfx_D3DBootstrap |
| `0x100033c0` | `0x100033C0` | D3DWrapper::SearchFormatPairs | Gfx_TryCreateDeviceForMode |
| `0x100034b0` | `0x100034B0` | D3DWrapper::CreateDevice | Gfx_CreateDevice |
| `0x100091b0` | `0x100091B0` | Renderer::ProbeCapsLoadCursor | CDx_Initialize |
| `0x10009620` | `0x10009620` | Renderer::SaveBackBuffer | CDx_SetDefaultRenderTargets |
| `0x10009640` | `0x10009640` | Renderer::StoreClearParams | CDx_SetClearParams |
| `0x10009670` | `0x10009670` | Renderer::SetClearColor | CDx_SetClearColor |
| `0x100096b0` | `0x100096B0` | Renderer::ClearBetweenScenes | CDx_EndClearBegin |
| `0x10009710` | `0x10009710` | Renderer::IsDeviceReady | CDx_TestCooperativeLevel |
| `0x10009720` | `0x10009720` | Renderer::Present | CDx_Present |
| `0x10009730` | `0x10009730` | Renderer::BeginScene | CDx_BeginScene |
| `0x10009740` | `0x10009740` | Renderer::EndScene | CDx_EndScene |
| `0x10009920` | `0x10009920` | Renderer::ResetRenderStates | CDx_SetDefaultRenderState |
| `0x10009dc0` | `0x10009DC0` | Renderer::SetViewportRecord | CDx_SetViewport |
| `0x10009e00` | `0x10009E00` | GetCurrentSlot | CDx_GetRenderTargetTop |
| `0x10009e20` | `0x10009E20` | Renderer::PushTargetOrigin | CDx_PushRenderTarget |
| `0x1000a1d0` | `0x1000A1D0` | Renderer::UnbindAndPopTarget | CDx_SetTexture |
| `0x1000a5a0` | `0x1000A5A0` | Renderer::DrawTexturedRect | CDx_DrawTexturedScreenQuad |
| `0x1000a6e0` | `0x1000A6E0` | Renderer::CreateTargetTexture | CDx_CreateTexture |
| `0x1000a800` | `0x1000A800` | Renderer::PushTargetReturnOne | CDx_PushRenderTargetRet1 |
| `0x1000a820` | `0x1000A820` | Renderer::DrawColorOverlay | CDx_DrawFadeQuad |
| `0x1000aab0` | `0x1000AAB0` | Renderer::DrawTextureAtDepth | Post_DepthBlurComposite |
| `0x1000bb90` | `0x1000BB90` | Renderer::DrawBottomStrip | CDx_DrawStereoOddFrameOverlay |
| `0x1000bd50` | `0x1000BD50` | Renderer::SetTransformCached | CDx_SetTransform |
| `0x1000bdb0` | `0x1000BDB0` | Renderer::GetCachedTransform | CDx_GetTransformCached |
| `0x1000bf30` | `0x1000BF30` | Renderer::UploadLight | CDx_SetLight |
| `0x1000c0b0` | `0x1000C0B0` | Renderer::CompositeWorldTexture | CDx_CompositeSceneToBackBuffer |
| `0x1000c6e0` | `0x1000C6E0` | Renderer::CreateSecondViewTextures | CDx_CreateStereoResources |
| `0x1000c7c0` | `0x1000C7C0` | Renderer::ReleaseSecondViewTextures | CDx_ReleaseStereoResources |
| `0x1000c850` | `0x1000C850` | Renderer::CreateStripTexture | CDx_CreateRampTexture1x16 |
| `0x1000c900` | `0x1000C900` | Renderer::CreateColumnPatternTextures | CDx_CreateRampTextures1024x2 |
| `0x1000ca00` | `0x1000CA00` | Renderer::IsSecondViewDraw | CDx_GetStereoSecondEyeTarget |
| `0x1000cb20` | `0x1000CB20` | Renderer::DrawPrimitivesStream | CDx_DrawPrimitive |
| `0x1000cc00` | `0x1000CC00` | Renderer::DrawIndexedBound | CDx_DrawIndexedPrimitive |
| `0x1000cd00` | `0x1000CD00` | Renderer::DrawPrimitivesMemory | CDx_DrawPrimitiveUP |
| `0x1000cf20` | `0x1000CF20` | Renderer::SetProjectionShifted | CDx_ApplyStereoEyeProjection |
| `0x1000cf90` | `0x1000CF90` | Renderer::ResendProjection | CDx_RestoreProjection |
| `0x10010d20` | `0x10010D20` | Game::StepStartup | App_InitScene |
| `0x100119c0` | `0x10011A00` | Game::Update | Frame_Render |
| `0x10012f00` | `0x10012F40` | DrawFrameDissolve | Post_Afterimage |
| `0x10013270` | `0x100132B0` | Game::DrawColorWashTrail | Post_FlashAndZoomFeedback |
| `0x10013a90` | `0x10013AD0` | Game::StepStartupState5 | App_StepGameState5Connect |
| `0x10014180` | `0x100141C0` | Game::InGameSubState0 | sub_100141C0 |
| `0x100154d0` | `0x10015510` | UpdateCameraProjection | Camera_ApplyViewProjection |
| `0x100160e0` | `0x100161B0` | CreateRendererSaveBackBuffer | App_CreateRendererAndApp |
| `0x10017160` | `0x10017230` | Renderer::UpdateGammaRamp | GammaRamp_ApplyIfChanged |
| `0x1001d170` | `0x1001D240` | ActorRedirect::Begin | Model_BeginTranslucentPass |
| `0x1001d280` | `0x1001D350` | ActorRedirect::End | Model_TranslucentUnbindTexture |
| `0x1001d2e0` | `0x1001D3B0` | ActorRedirect::BlendToScene | Model_CompositeTranslucentPass |
| `0x10022400` | `0x100224D0` | Type1MeshNode::DrawNode | CYyModelDt_PrepareRenderState |
| `0x100230f0` | `0x100231C0` | Type1MeshNode::ShiftIfPosed | CYyModelDt_PrepareRenderStateSimple |
| `0x10024f60` | `0x10025030` | Type1MeshNode::Draw | CYyModelDt_PrepareRenderStateEntry |
| `0x10024f90` | `0x10025060` | Type1MeshNode::Update | CYyModelDt_PrepareRenderStateSimpleEntry |
| `0x10025260` | `0x10025330` | ScreenPass::SetupRepeatDraw | ProjTex_BeginPass |
| `0x100254f0` | `0x100255C0` | ScreenPass::UndoRepeatDraw | ProjTex_EndPass |
| `0x100257a0` | `0x10025870` | ScreenPass::CopyWorldTexture | ProjTex_CaptureToTarget |
| `0x10025b70` | `0x10025C40` | SetupOverlayRepeatDraw | EnvMap_BeginPass |
| `0x10025ea0` | `0x10025F70` | UndoOverlayRepeatDraw | EnvMap_EndPass |
| `0x10027620` | `0x100276F0` | ProjectPoint | Gfx_ProjectWorldToClip |
| `0x10029980` | `0x10029A50` | CreateSharedVertexBuffers | sub_10029A50 |
| `0x1002a2e0` | `0x1002A3B0` | KzMotionMan::Draw | CYyModelBase_DrawLit |
| `0x1002b610` | `0x1002B6E0` | ModelOwner::Draw | CXiSkeletonActor_DrawModel |
| `0x1002c290` | `0x1002C360` | ModelOwner::DrawMeshNodes | CXiSkeletonActor_PrepareModelsOnce |
| `0x1002cbe0` | `0x1002CCB0` | MeshObject::CreateIndexAndUvBuffers | CYyOs2VtxBuffer_CreateBuffersStep |
| `0x1002d5b0` | `0x1002D680` | MeshObject::MeshObject | CYyOs2VtxBuffer_Constructor |
| `0x1002d630` | `0x1002D700` | MeshObject::~MeshObject | CYyOs2VtxBuffer_Destructor |
| `0x1002db40` | `0x1002DC10` | MeshResource::DrawSkinned | CYyModelDt_SkinAndDrawV2 |
| `0x1002f0d0` | `0x1002F1A0` | ApplyVertexShader | Gfx_ApplyCachedVertexShader |
| `0x10033730` | `0x10033800` | ModelShadow::BeginSilhouetteDraw | Model_BeginOffscreenPass |
| `0x100339a0` | `0x10033A70` | ModelShadow::EndSilhouetteDraw | Model_EndOffscreenPassBindTextures |
| `0x10038450` | `0x10038520` | FlushTextRecords | Font_DrawPass |
| `0x10038650` | `0x10038720` | DrawQueuedText | Font_DrawString |
| `0x10038860` | `0x10038930` | CreateHelperTexture | Post_CreateScreenEffectTargets |
| `0x100389b0` | `0x10038A80` | ActorImageHelper::ShrinkImage | Post_CaptureSceneToTarget |
| `0x10038d40` | `0x10038E10` | ActorImageHelper::DrawTintedRing | Post_DrawRotatingGhostLayers |
| `0x100399b0` | `0x10039A80` | TextureCacheEntry::CreateAndFill | TexObj_Create |
| `0x10039d80` | `0x10039E50` | TextureCacheEntry::DecodeAndLoadMips | TexObj_Upload |
| `0x1003b140` | `0x1003B210` | VertexBufferPool::Allocate | CYyVbMng_AcquireVertexBuffer |
| `0x1003b4c0` | `0x1003B590` | ActorRef::ReturnZero | CMoAttachments_vf006 |
| `0x1003bd40` | `0x1003BE10` | YmGenerater::BuildAttachMatrix | sub_1003BE10 |
| `0x1003d8d0` | `0x1003D9A0` | DrawSkyDomePassUnused | Sky_DrawDome |
| `0x1003da90` | `0x1003DB60` | DrawSkyDomePass | Sky_DrawDomeViewCentered |
| `0x1003e8d0` | `0x1003E9A0` | YmElem::SetD3ADrawState | sub_1003E9A0 |
| `0x1003ea60` | `0x1003EB30` | YmElem::DrawD3AFromBatch | sub_1003EB30 |
| `0x1003eec0` | `0x1003EF90` | YmElem::StepForward | CMoVuAnmElem_vf008 |
| `0x1003f370` | `0x1003F440` | MorphModel::DrawExpanded | CMoDistMorphElem_ApplyMorph |
| `0x1003f7d0` | `0x1003F8A0` | MorphModel::BlendTargets | CMoDistMorphElem_ComputeMorphHandle |
| `0x100405a0` | `0x10040670` | ModelMesh::DrawDistortionPieces | CMoDistModelElem_DrawMesh |
| `0x10040b50` | `0x10040C20` | YmElem::DrawZoneModel | CMoElemFamily_SharedEarlyExit |
| `0x100411e0` | `0x100412B0` | YmElem::DrawSpecular | sub_100412B0 |
| `0x10041a20` | `0x10041AF0` | YmElem::DrawFloatingText | sub_10041AF0 |
| `0x10041b50` | `0x10041C20` | DrawFloatingTextString | Font_DrawProjectedText3D |
| `0x10041f50` | `0x10042020` | YmDamValueProgElem::Step | sub_10042020 |
| `0x10042570` | `0x10042640` | Task::StepDefault | CMoTask_vf008 |
| `0x10042620` | `0x100426F0` | YmElem::DrawScreenDistortion | sub_100426F0 |
| `0x10042c30` | `0x10042D00` | CaptureWorldTexture | CMoDistMorphElem_ValidateMorphData |
| `0x10043d80` | `0x10043E50` | VertexBatch::AcquireDynamicBuffer | DynVB_Create |
| `0x10043e70` | `0x10043F40` | VertexBatch::BindAndDraw | sub_10043F40 |
| `0x100440f0` | `0x100441C0` | CameraCache::CacheViewProjection | sub_100441C0 |
| `0x10044140` | `0x10044210` | CameraCache::ResendView | Gfx_SetViewMatrixFromField10 |
| `0x10044160` | `0x10044230` | CameraCache::ResendViewNoTranslation | Gfx_SetViewMatrixFromField90 |
| `0x100441a0` | `0x10044270` | BuildCubemapRenderContext | Mo_InitSharedRenderResources |
| `0x10044e40` | `0x10044F10` | YmElem::ComputeViewDepthKey | sub_10044F10 |
| `0x10045850` | `0x10045920` | YmElem::BuildDrawMatrixAndTest | sub_10045920 |
| `0x10046710` | `0x100467E0` | YmElem::SetupDevice | sub_100467E0 |
| `0x10047220` | `0x100472F0` | YmElem::RestoreDevice | sub_100472F0 |
| `0x1004a7a0` | `0x1004A870` | GateEffectBoxProjection | sub_1004A870 |
| `0x1004e560` | `0x1004E630` | ScaleElemVisibilityByProbe | sub_1004E630 |
| `0x10052d10` | `0x10052DE0` | YmElem::StepForwardB | CMoVuDist_vf008 |
| `0x10053070` | `0x10053140` | YmElem::StepForwardC | CMoDisintegrateProgElem_vf008 |
| `0x100535f0` | `0x100536C0` | YmGenerater::CloneAndStart | CMoGeneratorClone_CreateInstance |
| `0x10053810` | `0x100538E0` | YmGenerater::Stop | sub_100538E0 |
| `0x10054ca0` | `0x10054D70` | BeginEffectOffscreenPass | sub_10054D70 |
| `0x10054eb0` | `0x10054F80` | EndEffectOffscreenPass | sub_10054F80 |
| `0x100564d0` | `0x100565A0` | YmElem::DrawRingMesh | ? |
| `0x10057ee0` | `0x10057FB0` | YmSchedulerTask::ExecuteTag | CMoSchedularTask_Interpret |
| `0x10063bb0` | `0x10063C80` | YmElem::ForEachInstance | sub_10063C80 |
| `0x1006ae30` | `0x1006AF00` | StateBlockHolder::ApplyRenderStates | sub_1006AF00 |
| `0x1006aec0` | `0x1006AF90` | StateBlockHolder::ApplyTextureStageStates | StateBlock_ApplyTSS |
| `0x1006b1e0` | `0x1006B2B0` | CreateEffectWorld | CMoDX_CreateAndInitSystem |
| `0x1006b3b0` | `0x1006B480` | EffectRefreshTask::Step | sub_1006B480 |
| `0x1006c1b0` | `0x1006C280` | ScreenProbe::TestRectVisible | Occlusion_QueryScreenRect |
| `0x1006c230` | `0x1006C300` | ScreenProbe::ClipDrawReadBack | Occlusion_QueryScreenRectImpl |
| `0x1006c460` | `0x1006C530` | DrawScreenProbe | sub_1006C530 |
| `0x1006c8e0` | `0x1006C9B0` | ScreenProbe::Prepare | OcclusionMng_Prepare |
| `0x1006cda0` | `0x1006CE70` | MatrixPool::MatrixPool | CMoProcessor_InitRotationMatrices |
| `0x1006ff50` | `0x10070020` | MatrixPool::ProjectAndBoundSse | CMoProcessor_VT_1032C298_vf035 |
| `0x10070d50` | `0x10070E20` | ProjectVertexBatch3DNow | CMoProcessor_VT_1032C1C8_vf033 |
| `0x10070d70` | `0x10070E40` | MatrixPool::ProjectAndBound3DNow | CMoProcessor_VT_1032C1C8_vf035 |
| `0x10071ca0` | `0x10071D70` | GetResourceClassRecord | CMoResource_GetRuntimeClass_variant3 |
| `0x10075630` | `0x10075700` | TextureHolder::BindWarped | sub_10075700 |
| `0x10075890` | `0x10075960` | TextureHolder::Bind | TexRef_SetTexture |
| `0x100758f0` | `0x100759C0` | TextureHolder::BindTinted | sub_100759C0 |
| `0x10081720` | `0x100817F0` | ActorLabelTask::Step | CXiActorNameDraw_SetRenderState |
| `0x10082e50` | `0x10082F20` | CXiActor::GetNameplateFirstString | CXiSkeletonActor_GetEnglishName |
| `0x10083210` | `0x100832E0` | CXiActor::DecrementCounter | CXiSkeletonActor_SubRef |
| `0x10086140` | `0x10086210` | DrawNameplateFirstString | Actor_DrawWorldText |
| `0x10086830` | `0x10086900` | DrawNameplateSecondString | sub_10086900 |
| `0x100c6290` | `0x100C63D0` | XiSkeletonActor::Update | CXiSkeletonActor_OnMove |
| `0x100cb3a0` | `0x100CB4E0` | XiSkeletonActor::SelectLightsAndFog | Actor_SelectNearestLights |
| `0x100cbc20` | `0x100CBD60` | XiSkeletonActor::DrawSlot | CXiSkeletonActor_RunStaggeredUpdates |
| `0x100cbca0` | `0x100CBDE0` | XiSkeletonActor::Draw | PerActor_FourBucketStaggeredUpdate |
| `0x100cf0d0` | `0x100CF210` | XiSkeletonActor::SetModel | CXiSkeletonActor_SetModel |
| `0x100d0380` | `0x100D04C0` | XiSkeletonActor::PlaceNameplate | CXiSkeletonActor_GetNamePos |
| `0x100d03d0` | `0x100D0510` | XiSkeletonActor::DrawLabel | CXiSkeletonActor_OnDrawName |
| `0x100d4420` | `0x100D4560` | XiSkeletonActor::GetNameplateAnchor | CXiSkeletonActor_GetElem |
| `0x1011b980` | `0x1011BAE0` | TextQueue::BindGlyphAtlas | Font_BindGlyphTexture |
| `0x10120430` | `0x10120590` | FontObject::BindPageTexture | ShapeMng_BindFontTexture |
| `0x10120650` | `0x101207B0` | FontObject::BindFloatingTextPage | ShapeMng_BindDmgNumTexture |
| `0x1016fb60` | `0x1016FCD0` | ParseZoneMeshData | sub_1016FCD0 |
| `0x10174d50` | `0x10174EC0` | DrawNodeBoxesWithLabels | Dbg_DrawOrientedBoxes |
| `0x10177290` | `0x10177400` | SetZBiasRenderState | Gfx_SetZBias |
| `0x10177b60` | `0x10177CD0` | XiZone::DrawMarkerForward | sub_10177CD0 |
| `0x10178060` | `0x101781D0` | XiZone::SetDepthBias | CTsZoneMap_InitZBiasPresets |
| `0x10179080` | `0x101791F0` | XiZone::DrawIndexedQuads24 | ZoneMap_DrawBox |
| `0x10179130` | `0x101792A0` | XiZone::CreateVertexDeclarations | ZoneMap_CreateVertexShaders |
| `0x1017ac70` | `0x1017ADE0` | XiZone::DrawLayerPostPass | ZoneMap_DrawMeshGroupsWithFog |
| `0x1017b3a0` | `0x1017B510` | XiZone::Draw | sub_1017B510 |
| `0x1017c850` | `0x1017C9C0` | XiZone::DrawElement | sub_1017C9C0 |
| `0x10182590` | `0x10182700` | XiZone::ProjectTextureOnCollision | CTsZoneMap_DrawProjectedTexture |
| `0x10183320` | `0x10183490` | XiZone::DrawModelAtMatrix | sub_10183490 |
| `0x101835f0` | `0x10183760` | XiZone::DrawEffectElementModel | sub_10183760 |
| `0x10183c30` | `0x10183DA0` | XiZone::DrawPostPassItem | ZoneMap_DrawMeshById |
| `0x10184350` | `0x101844C0` | XiZone::DrawActorMesh | ModelActor_Draw |
| `0x10184b20` | `0x10184C90` | XiZone::DrawGroundShadowQuad | sub_10184C90 |
| `0x10184e70` | `0x10184FE0` | XiZone::DrawShadowOnTriangles | CTsZoneMap_DrawProjectedShadow |
| `0x10185b30` | `0x10185CA0` | XiZone::DrawDecalOnTriangles | sub_10185CA0 |
| `0x10188c90` | `0x10188E00` | XiArea::PackAmbientColor | Env_ComputeAmbient |
| `0x101d0630` | `0x101D0B00` | DrawCornerTextLine | sub_101D0B00 |
| `0x1024c920` | `0x1024CF50` | StAvatarInit | sub_1024CF50 |
| `0x1024f2d0` | `0x1024F900` | StAvatarCameraTransfer | StAvatar_SetViewFromDancerCamera |
| `0x1024fb90` | `0x102501C0` | StAvatarRender | sub_102501C0 |
| `0x10251450` | `0x10251A80` | StDancer::_dancerInit | Dancer_InitDevice |
