# The renderer: what it calls, where, and what that means for the goals

The goals are a performance patch, a possible graphical overhaul, and a renderer port to D3D11. This
document maps the parts of the renderer they depend on. Each fact names its source. A fact rests on one
of three kinds of evidence, and the text says which:

- **proven**: the code alone settles it (com_calls.py, purges, the relocation table);
- **accepted**: rests on the owner's accepted declaration that CDx+0xC is the device (data/com_fields.json);
- **observed**: seen in the gate's runs of the original code (diffemu_gfx.py, fuzz and four memory dumps).
  An observation shows what those inputs reached, not what every execution does. The runs' inputs are
  partly fuzzed, so an observed argument value can come from fuzz.

Data: `data/render_map.json` (pipeline/render_map.py, from `FFXI_COM_SITES=1` gate runs plus
data/com_calls.json).

## 1. Direct3D call sites

1,633 call sites in the code have a known Direct3D 8 method:

| Evidence | Sites |
|---|---|
| proven | 217 |
| accepted (CDx device) | 151 |
| observed only | 1,265 |

Where both a proof and an observation exist for a site, they never disagree (`conflicts: []`). 19 further
sites reached a real object of a dump whose interface is not known here. That is fuzz reaching dump
memory; no method is claimed for them.

Of the 2,662 indirect calls in the exported graphics functions:
- 1,506 are Direct3D calls with a known method;
- 139 are resolved game virtual calls (data/vcalls.json);
- 1,017 are neither. Those are function pointers, unresolved virtual calls, and Direct3D calls no run reached.

By method (sites): SetRenderState 636, SetTextureStageState 600, SetTexture 87, SetVertexShader 63,
LightEnable 47, SetStreamSource 22, CreateVertexBuffer 17, SetTransform 16, and fewer for the rest.

### Draws are few and funnelled

Only 12 call sites draw:

| Site | Function | Method | Evidence |
|---|---|---|---|
| 0x1000CB6E, 0x1000CBAC, 0x1000CBED | CDx_DrawPrimitive | DrawPrimitive | accepted |
| 0x1000CC58, 0x1000CCA0, 0x1000CCEB | CDx_DrawIndexedPrimitive | DrawIndexedPrimitive | accepted |
| 0x1000CD53, 0x1000CD96, 0x1000CDDC | CDx_DrawPrimitiveUP | DrawPrimitiveUP | accepted |
| 0x1029BB0E, 0x1029BB42 | sqgxDlistDraw | DrawIndexedPrimitive (strip, list) | observed |
| 0x1028AFA4 | sub_1028AB50 | DrawPrimitive | observed |

Each CDx wrapper has three sites. One is the plain draw. The other two draw once per eye when
CDx_GetStereoSecondEyeTarget returns a second-eye target (tree/src/CDx.cpp, CDx::DrawPrimitive).
Callers of the wrappers across the whole binary (direct calls):
- CDx_DrawPrimitiveUP: 50;
- CDx_DrawPrimitive: 8;
- CDx_DrawIndexedPrimitive: 4.

So two layers issue the draws:
- the CDx wrappers;
- the `sqgx` display-list layer (`sqgxDlistDraw`).

A D3D11 backend, and per-draw profiling, need these few places, not 1,600.

### State the game sets

- **Render states**: 56 render states are set somewhere. Most frequent by site count: ALPHABLENDENABLE,
  ZWRITEENABLE, TEXTUREFACTOR, ZBIAS, ZENABLE, ALPHAREF, CULLMODE, SRC/DESTBLEND, LIGHTING,
  ALPHATESTENABLE, FOG*.
  - The stencil states appear too: `sqsnShadowVolumeSetPass` (0x10297D38 to 0x10297EB2, constants in
    its code) sets stencil-always with STENCILPASS DECR and CULLMODE CCW in one pass, and INCR with CW in
    the other. That is a two-pass stencil shadow volume.
- **Texture-stage states**: 15 types. COLOROP/ALPHAOP and their ARG0/1/2 dominate (the fixed-function
  combiners), then filtering and addressing, TEXCOORDINDEX, TEXTURETRANSFORMFLAGS and RESULTARG.
  - A D3D11 port has to express these combiners in shaders.
- **Vertex shaders and FVF codes**: CreateVertexShader is called at 7 sites, so programmable vertex
  shaders exist alongside FVF draws.
  - Values observed at SetVertexShader sites that decode as FVF codes (D3D8 SDK d3d8types.h; bit 0 clear):
    0x144, 0x142, 0x44, 0x42, 0x152, 0x2, 0x6.
  - In decoded form: XYZRHW|DIFFUSE|TEX1, XYZ|DIFFUSE|TEX1, XYZRHW|DIFFUSE, XYZ|DIFFUSE,
    XYZ|NORMAL|DIFFUSE|TEX1, XYZ, XYZB1.
  - Other observed values are fuzz or shader handles. Which is which is not settled here.

## 2. The frame (Frame_Render, 0x10011A00)

App_RenderFrameMaskedFPU (0x10001B50) masks FPU exceptions (`_controlfp(0x8001F, 0x8001F)`) and calls
Frame_Render with `this = [0x104568FC]` (the IDB calls that global g_pCamera; it holds the 0x33C-byte
object CApp_RunMainLoop constructs). In code order:

1. TestCooperativeLevel (on failure: error code, return), BeginScene, CDx_SetDefaultRenderState,
   texture upload pump (TexMgr_UploadPumpEntry).
2. Stereo mode: push the second-eye target. Then push the main scene target and clear it
   (CDx_PushRenderTargetRet1, sub_1000A390).
3. The game-state machine. In the world: the zone and actor update/draw calls (sub_1018B130,
   sub_1018C2B0, StDancer_Update, sub_1018B860(g_pXiZone), ...). These are outside the exported set and
   reach the device through CMoDX (section 4).
4. Post effects: depth blur (Post_DepthBlurComposite), Post_Afterimage, Post_FlashAndZoomFeedback.
5. CDx_SetViewport, CDx_CompositeSceneToBackBuffer.
6. UI: menus (Gfx_SetDefault2DStates, the menu manager), cursor, fade quad, movie frame, text
   (Font_DrawPass), stereo overlay.
7. EndScene, Present (CDx_Present).
8. Frame pacing (section 3), then Camera_ApplyViewProjection and GammaRamp_ApplyIfChanged.

## 3. Frame pacing: the game's own limit is 30 fps (15 in some states)

**The wait loop.** From the code (0x10012B05 to 0x10012BC6) and the timer object at `[this+0x1C]`
(vtable 0x1032A118):
- Timer method 9 (0x10019550) returns `1000.0 / (timeGetTime() - [t+0xC] - [t+0x1C])`, with a 0 difference
  counted as 1. Method 12 (0x10019230) returns the float at `[t+8]`.
- After Present, the frame computes `v = 60 / (method12() * method9())` and stores it at `this+0x28` and
  `this+0x2C`. With `[t+8] = 1.0`, `v` is the elapsed time in 1/60-second ticks.
- While `v < (int)[this+0x30]` it calls `Sleep(1)` and `pause`, then measures again (0x10012B12 to 0x10012B64).
- So `[this+0x30]` is the minimum number of 1/60 s ticks per frame. 2 is 30 fps, 1 is 60 fps.

**Who sets the minimum.** The game sets it in one function, 0x1008A0E0, which stores its argument in
`this+0x30` and in the global 0x1035B5E8, and prints the debug line `FPS制限[%d]` ("FPS limit"). It is
called only with constants:
- `2` at 0x1008A0D2 (in 0x1008A0D0), 0x10088A55 and 0x10088AC7 (in 0x100883E0), and 0x1009AAB9;
- `4` at 0x100884B3 (in 0x100883E0).

The global's initial value in the binary is 2. So the game's own code allows 30 fps, or 15 fps in the
state 0x100884B3 handles. That state is not identified here.

**In the four dumps:**
- the global 0x1035B5E8 is 2, but `[this+0x30]` is 1. Something other than the game's setter wrote 1 into
  `this+0x30` (an FPS unlock outside the game's code; which tool did it is not known here);
- `[t+8] = 1.0`;
- `[this+0x28]` is 2.0 in the two town dumps and 1.0 in combat and zonetoinn;
- `[t+4]` holds `1000/elapsed`, the last measured rate: 24.39 in the town dump.

What follows:
- **With the minimum at 1, the loop holds frames to 60 fps.** A pacer in front of Present (speedfix_hook's
  `frame_cap_fps`) cannot get more than 60 while that is so.
- **A minimum of 0 would remove the wait** (`v < 0` never holds). What the rest of the frame then does is
  not established here. The elapsed time is split into whole ticks (`this+0x38`) and a carried fraction
  (`this+0x3C`), and the four-frame average at `this+0x28` is raised to at least `[this+0x30]`. Whether the
  simulation advances on frames with no whole tick needs the consumers of `this+0x38` and `this+0x28`
  read. That is the question for going past 60.
- **What the dumps show:** the town dumps were taken at about 24 fps (41 ms per frame), below either limit.

## 4. Coverage gaps that matter for the goals

- **The CMoDX device users.** 99 functions outside the export use the device (data/gfx_device_users.json):
  - 93 of them through the CMoDX object at 0x1047BFA8, whose +0xEBC is the device (proven,
    com_fields.json);
  - 26 hold proven D3D8 calls;
  - 10 call CDx's draw wrappers.

  They include the occlusion readback (0x1006C8B0) and the stream setup at 0x10043F40. The next IDA run
  exports them (867 functions).
- **The 3D world path.** The world update/draw functions called from the state machine (section 2) are
  outside the export.
- **The x87 comparison artifacts.** The `__builtin_isunordered` chains in the tree (Frame_Render's pacing
  loop has five) are the x87 compare idiom. The next IDA export's per-comparison instructions are meant
  to remove them.

## 5. Measured costs (owner's speedfix_hook, on dgVoodoo / D3D11 / Parallels)

These numbers are the owner's measurements, recorded in speedfix_hook.cpp. They were not measured here.

- **Occlusion readback** (0x1006C8B0):
  - What it does: CopyRects of a shared 16x16 target, then a read-only LockRect. The CPU waits for the GPU there.
  - Cost: about 4 to 5 ms for the first readback in a frame, about 1.2 ms for each further one; about
    2.3 ms per readback in another measurement.
  - Callers: six. Two are actor paths (0x100CC688, 0x100CD011); the others include 0x1004A960, 0x1004E6FF,
    0x10054AE2 (in 0x100548D0) and 0x100AC894.
  - Checked here: Occlusion_QueryScreenRect (0x1006C280) has those six callers and pops 36 (`ret 0x24`);
    0x1006C8B0 has the one caller 0x1006C4D7.
- **DrawPrimitiveUP:** measured at about 20% of the frame's traced stall time.
- **Redundant sets:** measured redundant fractions, by call (not yet cached by default):

  | Call | Redundant |
  |---|---|
  | SetLight | 83% |
  | SetIndices | 70% |
  | SetViewport | 62% |
  | LightEnable | 33% |
  | SetStreamSource | 20% |
  | SetTransform | 5 to 9% |

What the map adds to these: per call site attribution. kit/d3dprof counts each method per call site, and
the call sites map to the functions in data/render_map.json and the reconstructed tree.

## 6. What this means for each goal

- **Performance patch.** The occlusion readback is a GPU-to-CPU synchronisation on every query. D3D8 has
  no asynchronous occlusion query, so speedfix staggers and skips it.
  - In a D3D11 backend it becomes an `ID3D11Query` (D3D11_QUERY_OCCLUSION) read a frame or more later,
    with no stall.
  - The reconstructed 0x1006C8B0 and its callers are where that change is made once the next export
    brings them in.
- **D3D11 port.** The draws come from two layers (CDx wrappers and sqgx display lists). The state is
  fixed-function: render states, texture-stage combiners, FVF and a few vertex shaders.
  - A backend at the CDx/sqgx layer sees each draw with its purpose, which a D3D8-to-D3D11 wrapper like
    dgVoodoo cannot.
  - The fixed-function combiner states seen in section 1 are the shader permutations it has to cover.
- **Frame rate:** the game's own setter allows 30 (or 15); 60 needs `this+0x30 = 1`, as the dumps already
  show. Above 60 depends on how the frame's tick consumers behave with no whole tick (section 3).
