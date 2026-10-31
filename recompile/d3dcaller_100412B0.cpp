/* Checked by the integrator: the pooled constants match.py does not compare are, in the game's .rdata, pi/2 (0x1032ADDC, 3fc90fdb). */
/* Task #118/#119: 0x100412B0, written from the game's bytes (no tree source). A __thiscall method (reads ECX,
   pops 0: data/ecx_use.json, data/purges.json). It tail-jumps to 0x10040C20 when +0x179 is set (written as a call
   and return: VC6 emitted the jmp); otherwise it builds the entity's world matrix on g_pProcessor's matrix stack
   (vtable slot 22 rotate with 4 args, slot 9 multiply with 3 args: both pop their arguments, the caller never adds
   esp), sets it (IDirect3DDevice8 slot 37 SetTransform), applies one of four function-local static state blocks
   (rule 16; constructor 0x1006AD90, destructor 0x1006ADB0, $E functions 0x10041A00-0x10041A30), sets texture stage
   states (slot 63) and draws the mesh's index ranges through 0x10043F40 (ret 20). 0x1006AE30 pops its two arguments
   by the caller's stack balance (no purges.json entry; it ends in a tail jump to ApplyStateBlock); 0x1006AF90 rets 4,
   0x10075700 rets 16, 0x1000BD50 rets 8, 0x10015230 is cdecl with no arguments (ECX not read). The mesh pointer at
   +0x104 is read twice through an inline accessor (`p ? *p : 0`, as in d3dcaller_1003FBB0.cpp); the bit-28 test is
   written byte-sized (`(unsigned char)(x >> 28) & 1`, the game's shr/test al,1); the matrix copies' x/y go through
   inline float accessors (rule 10). Constant read from the game: 0x1032ADDC = 3FC90FDB (1.5707964f); the -pi/2
   pushed as 0xBFC90FDB is an immediate (compared). Classes are stand-ins. */
// SYMBOL: ?$S1@?BJ@??Render@XEnt@@QAEXXZ@4EA 0x1047BA18
// SYMBOL: ?sb4@?BJ@??Render@XEnt@@QAEXXZ@4UXStateBlock@@A 0x1047BAB4
// SYMBOL: ?sb2@?CK@??Render@XEnt@@QAEXXZ@4UXStateBlock@@A 0x1047BA44
// SYMBOL: ?sb3@?DG@??Render@XEnt@@QAEXXZ@4UXStateBlock@@A 0x1047BA2C
// SYMBOL: ?sb1@?FA@??Render@XEnt@@QAEXXZ@4UXStateBlock@@A 0x1047BA38
// SYMBOL: ??0XStateBlock@@QAE@XZ 0x1006AD90
// SYMBOL: ??1XStateBlock@@QAE@XZ 0x1006ADB0
// SYMBOL: _$E2 0x10041A30
// SYMBOL: _$E3 0x10041A20
// SYMBOL: _$E4 0x10041A10
// SYMBOL: _$E5 0x10041A00
// SYMBOL: _atexit 0x10313486
/* the tree's type names (pipeline/vc6/sweep.py vc6_shim.h), those used here */
typedef unsigned char uint8_t; typedef signed char int8_t; typedef unsigned short uint16_t; typedef short int16_t;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
/* Direct3D 8, from tree/include/xi_d3d8.hpp (MinGW-w64 d3d8.h / d3d8types.h): the enums and structs used here;
   each interface in the SDK's C++ form (d3d8.h DECLARE_INTERFACE_ / STDMETHOD: __stdcall virtual methods in
   vtable order), declared through the slot each call uses */
struct IDirect3D8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DDevice8;
struct IDirect3DIndexBuffer8;
struct IDirect3DSurface8;
struct IDirect3DSwapChain8;
struct IDirect3DTexture8;
struct IDirect3DVertexBuffer8;
struct IDirect3DVolumeTexture8;
#define MAKEFOURCC(a, b, c, d) ((unsigned int)(unsigned char)(a) | ((unsigned int)(unsigned char)(b) << 8) | \
                                ((unsigned int)(unsigned char)(c) << 16) | ((unsigned int)(unsigned char)(d) << 24))
enum D3DRENDERSTATETYPE {
  D3DRS_ZENABLE = 7,
  D3DRS_FILLMODE = 8,
  D3DRS_SHADEMODE = 9,
  D3DRS_LINEPATTERN = 10,
  D3DRS_ZWRITEENABLE = 14,
  D3DRS_ALPHATESTENABLE = 15,
  D3DRS_LASTPIXEL = 16,
  D3DRS_SRCBLEND = 19,
  D3DRS_DESTBLEND = 20,
  D3DRS_CULLMODE = 22,
  D3DRS_ZFUNC = 23,
  D3DRS_ALPHAREF = 24,
  D3DRS_ALPHAFUNC = 25,
  D3DRS_DITHERENABLE = 26,
  D3DRS_ALPHABLENDENABLE = 27,
  D3DRS_FOGENABLE = 28,
  D3DRS_SPECULARENABLE = 29,
  D3DRS_ZVISIBLE = 30,
  D3DRS_FOGCOLOR = 34,
  D3DRS_FOGTABLEMODE = 35,
  D3DRS_FOGSTART = 36,
  D3DRS_FOGEND = 37,
  D3DRS_FOGDENSITY = 38,
  D3DRS_EDGEANTIALIAS = 40,
  D3DRS_ZBIAS = 47,
  D3DRS_RANGEFOGENABLE = 48,
  D3DRS_STENCILENABLE = 52,
  D3DRS_STENCILFAIL = 53,
  D3DRS_STENCILZFAIL = 54,
  D3DRS_STENCILPASS = 55,
  D3DRS_STENCILFUNC = 56,
  D3DRS_STENCILREF = 57,
  D3DRS_STENCILMASK = 58,
  D3DRS_STENCILWRITEMASK = 59,
  D3DRS_TEXTUREFACTOR = 60,
  D3DRS_WRAP0 = 128,
  D3DRS_WRAP1 = 129,
  D3DRS_WRAP2 = 130,
  D3DRS_WRAP3 = 131,
  D3DRS_WRAP4 = 132,
  D3DRS_WRAP5 = 133,
  D3DRS_WRAP6 = 134,
  D3DRS_WRAP7 = 135,
  D3DRS_CLIPPING = 136,
  D3DRS_LIGHTING = 137,
  D3DRS_AMBIENT = 139,
  D3DRS_FOGVERTEXMODE = 140,
  D3DRS_COLORVERTEX = 141,
  D3DRS_LOCALVIEWER = 142,
  D3DRS_NORMALIZENORMALS = 143,
  D3DRS_DIFFUSEMATERIALSOURCE = 145,
  D3DRS_SPECULARMATERIALSOURCE = 146,
  D3DRS_AMBIENTMATERIALSOURCE = 147,
  D3DRS_EMISSIVEMATERIALSOURCE = 148,
  D3DRS_VERTEXBLEND = 151,
  D3DRS_CLIPPLANEENABLE = 152,
  D3DRS_SOFTWAREVERTEXPROCESSING = 153,
  D3DRS_POINTSIZE = 154,
  D3DRS_POINTSIZE_MIN = 155,
  D3DRS_POINTSPRITEENABLE = 156,
  D3DRS_POINTSCALEENABLE = 157,
  D3DRS_POINTSCALE_A = 158,
  D3DRS_POINTSCALE_B = 159,
  D3DRS_POINTSCALE_C = 160,
  D3DRS_MULTISAMPLEANTIALIAS = 161,
  D3DRS_MULTISAMPLEMASK = 162,
  D3DRS_PATCHEDGESTYLE = 163,
  D3DRS_PATCHSEGMENTS = 164,
  D3DRS_DEBUGMONITORTOKEN = 165,
  D3DRS_POINTSIZE_MAX = 166,
  D3DRS_INDEXEDVERTEXBLENDENABLE = 167,
  D3DRS_COLORWRITEENABLE = 168,
  D3DRS_TWEENFACTOR = 170,
  D3DRS_BLENDOP = 171,
  D3DRS_POSITIONORDER = 172,
  D3DRS_NORMALORDER = 173
};
enum D3DTRANSFORMSTATETYPE {
  D3DTS_VIEW = 2,
  D3DTS_PROJECTION = 3,
  D3DTS_TEXTURE0 = 16,
  D3DTS_TEXTURE1 = 17,
  D3DTS_TEXTURE2 = 18,
  D3DTS_TEXTURE3 = 19,
  D3DTS_TEXTURE4 = 20,
  D3DTS_TEXTURE5 = 21,
  D3DTS_TEXTURE6 = 22,
  D3DTS_TEXTURE7 = 23
};
enum D3DTEXTURESTAGESTATETYPE {
  D3DTSS_COLOROP = 1,
  D3DTSS_COLORARG1 = 2,
  D3DTSS_COLORARG2 = 3,
  D3DTSS_ALPHAOP = 4,
  D3DTSS_ALPHAARG1 = 5,
  D3DTSS_ALPHAARG2 = 6,
  D3DTSS_BUMPENVMAT00 = 7,
  D3DTSS_BUMPENVMAT01 = 8,
  D3DTSS_BUMPENVMAT10 = 9,
  D3DTSS_BUMPENVMAT11 = 10,
  D3DTSS_TEXCOORDINDEX = 11,
  D3DTSS_ADDRESSU = 13,
  D3DTSS_ADDRESSV = 14,
  D3DTSS_BORDERCOLOR = 15,
  D3DTSS_MAGFILTER = 16,
  D3DTSS_MINFILTER = 17,
  D3DTSS_MIPFILTER = 18,
  D3DTSS_MIPMAPLODBIAS = 19,
  D3DTSS_MAXMIPLEVEL = 20,
  D3DTSS_MAXANISOTROPY = 21,
  D3DTSS_BUMPENVLSCALE = 22,
  D3DTSS_BUMPENVLOFFSET = 23,
  D3DTSS_TEXTURETRANSFORMFLAGS = 24,
  D3DTSS_ADDRESSW = 25,
  D3DTSS_COLORARG0 = 26,
  D3DTSS_ALPHAARG0 = 27,
  D3DTSS_RESULTARG = 28
};
enum D3DPRIMITIVETYPE {
  D3DPT_POINTLIST = 1,
  D3DPT_LINELIST = 2,
  D3DPT_LINESTRIP = 3,
  D3DPT_TRIANGLELIST = 4,
  D3DPT_TRIANGLESTRIP = 5,
  D3DPT_TRIANGLEFAN = 6
};
enum D3DFORMAT {
  D3DFMT_UNKNOWN = 0,
  D3DFMT_R8G8B8 = 20,
  D3DFMT_A8R8G8B8 = 21,
  D3DFMT_X8R8G8B8 = 22,
  D3DFMT_R5G6B5 = 23,
  D3DFMT_X1R5G5B5 = 24,
  D3DFMT_A1R5G5B5 = 25,
  D3DFMT_A4R4G4B4 = 26,
  D3DFMT_R3G3B2 = 27,
  D3DFMT_A8 = 28,
  D3DFMT_A8R3G3B2 = 29,
  D3DFMT_X4R4G4B4 = 30,
  D3DFMT_A2B10G10R10 = 31,
  D3DFMT_G16R16 = 34,
  D3DFMT_A8P8 = 40,
  D3DFMT_P8 = 41,
  D3DFMT_L8 = 50,
  D3DFMT_A8L8 = 51,
  D3DFMT_A4L4 = 52,
  D3DFMT_V8U8 = 60,
  D3DFMT_L6V5U5 = 61,
  D3DFMT_X8L8V8U8 = 62,
  D3DFMT_Q8W8V8U8 = 63,
  D3DFMT_V16U16 = 64,
  D3DFMT_W11V11U10 = 65,
  D3DFMT_A2W10V10U10 = 67,
  D3DFMT_UYVY = MAKEFOURCC('U',
  'Y',
  'V',
  'Y'),
  D3DFMT_YUY2 = MAKEFOURCC('Y',
  'U',
  'Y',
  '2'),
  D3DFMT_DXT1 = MAKEFOURCC('D',
  'X',
  'T',
  '1'),
  D3DFMT_DXT2 = MAKEFOURCC('D',
  'X',
  'T',
  '2'),
  D3DFMT_DXT3 = MAKEFOURCC('D',
  'X',
  'T',
  '3'),
  D3DFMT_DXT4 = MAKEFOURCC('D',
  'X',
  'T',
  '4'),
  D3DFMT_DXT5 = MAKEFOURCC('D',
  'X',
  'T',
  '5'),
  D3DFMT_D16_LOCKABLE = 70,
  D3DFMT_D32 = 71,
  D3DFMT_D15S1 = 73,
  D3DFMT_D24S8 = 75,
  D3DFMT_D16 = 80,
  D3DFMT_D24X8 = 77,
  D3DFMT_D24X4S4 = 79,
  D3DFMT_VERTEXDATA = 100,
  D3DFMT_INDEX16 = 101,
  D3DFMT_INDEX32 = 102
};
enum D3DPOOL {
  D3DPOOL_DEFAULT = 0,
  D3DPOOL_MANAGED = 1,
  D3DPOOL_SYSTEMMEM = 2,
  D3DPOOL_SCRATCH = 3
};
enum D3DRESOURCETYPE {
  D3DRTYPE_SURFACE = 1,
  D3DRTYPE_VOLUME = 2,
  D3DRTYPE_TEXTURE = 3,
  D3DRTYPE_VOLUMETEXTURE = 4,
  D3DRTYPE_CUBETEXTURE = 5,
  D3DRTYPE_VERTEXBUFFER = 6,
  D3DRTYPE_INDEXBUFFER = 7
};
enum D3DMULTISAMPLE_TYPE {
  D3DMULTISAMPLE_NONE = 0,
  D3DMULTISAMPLE_2_SAMPLES = 2,
  D3DMULTISAMPLE_3_SAMPLES = 3,
  D3DMULTISAMPLE_4_SAMPLES = 4,
  D3DMULTISAMPLE_5_SAMPLES = 5,
  D3DMULTISAMPLE_6_SAMPLES = 6,
  D3DMULTISAMPLE_7_SAMPLES = 7,
  D3DMULTISAMPLE_8_SAMPLES = 8,
  D3DMULTISAMPLE_9_SAMPLES = 9,
  D3DMULTISAMPLE_10_SAMPLES = 10,
  D3DMULTISAMPLE_11_SAMPLES = 11,
  D3DMULTISAMPLE_12_SAMPLES = 12,
  D3DMULTISAMPLE_13_SAMPLES = 13,
  D3DMULTISAMPLE_14_SAMPLES = 14,
  D3DMULTISAMPLE_15_SAMPLES = 15,
  D3DMULTISAMPLE_16_SAMPLES = 16
};
enum D3DBACKBUFFER_TYPE {
  D3DBACKBUFFER_TYPE_MONO = 0,
  D3DBACKBUFFER_TYPE_LEFT = 1,
  D3DBACKBUFFER_TYPE_RIGHT = 2
};
enum D3DSTATEBLOCKTYPE {
  D3DSBT_ALL = 1,
  D3DSBT_PIXELSTATE = 2,
  D3DSBT_VERTEXSTATE = 3
};
struct D3DLOCKED_RECT { int Pitch; void *pBits; };
struct IDirect3DSurface8 {
  virtual int __stdcall QueryInterface(unsigned int riid, void ** ppvObject) = 0;
  virtual int __stdcall AddRef() = 0;
  virtual int __stdcall Release() = 0;
  virtual int __stdcall GetDevice(IDirect3DDevice8 ** ppDevice) = 0;
  virtual int __stdcall SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) = 0;
  virtual int __stdcall GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall FreePrivateData(unsigned int refguid) = 0;
  virtual int __stdcall GetContainer(unsigned int riid, void ** ppContainer) = 0;
  virtual int __stdcall GetDesc(void * pDesc) = 0;
  virtual int __stdcall LockRect(D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) = 0;
  virtual int __stdcall UnlockRect() = 0;
};
struct IDirect3DTexture8 {
  virtual int __stdcall QueryInterface(unsigned int riid, void ** ppvObject) = 0;
  virtual int __stdcall AddRef() = 0;
  virtual int __stdcall Release() = 0;
  virtual int __stdcall GetDevice(IDirect3DDevice8 ** ppDevice) = 0;
  virtual int __stdcall SetPrivateData(unsigned int refguid, void * data, unsigned int data_size, unsigned int flags) = 0;
  virtual int __stdcall GetPrivateData(unsigned int refguid, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall FreePrivateData(unsigned int refguid) = 0;
  virtual int __stdcall SetPriority(unsigned int PriorityNew) = 0;
  virtual int __stdcall GetPriority() = 0;
  virtual int __stdcall PreLoad() = 0;
  virtual int __stdcall GetType() = 0;
  virtual int __stdcall SetLOD(unsigned int LODNew) = 0;
  virtual int __stdcall GetLOD() = 0;
  virtual int __stdcall GetLevelCount() = 0;
  virtual int __stdcall GetLevelDesc(unsigned int Level, void * pDesc) = 0;
  virtual int __stdcall GetSurfaceLevel(unsigned int Level, IDirect3DSurface8 ** ppSurfaceLevel) = 0;
  virtual int __stdcall LockRect(unsigned int level, D3DLOCKED_RECT *locked_rect, void * rect, unsigned int flags) = 0;
  virtual int __stdcall UnlockRect(unsigned int Level) = 0;
  virtual int __stdcall AddDirtyRect(void * dirty_rect) = 0;
};
struct IDirect3DDevice8 {
  virtual int __stdcall QueryInterface(unsigned int riid, void ** ppvObject) = 0;
  virtual int __stdcall AddRef() = 0;
  virtual int __stdcall Release() = 0;
  virtual int __stdcall TestCooperativeLevel() = 0;
  virtual int __stdcall GetAvailableTextureMem() = 0;
  virtual int __stdcall ResourceManagerDiscardBytes(unsigned int Bytes) = 0;
  virtual int __stdcall GetDirect3D(IDirect3D8 ** ppD3D8) = 0;
  virtual int __stdcall GetDeviceCaps(void * pCaps) = 0;
  virtual int __stdcall GetDisplayMode(void * pMode) = 0;
  virtual int __stdcall GetCreationParameters(void * pParameters) = 0;
  virtual int __stdcall SetCursorProperties(unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface8 * pCursorBitmap) = 0;
  virtual int __stdcall SetCursorPosition(unsigned int XScreenSpace, unsigned int YScreenSpace, unsigned int Flags) = 0;
  virtual int __stdcall ShowCursor(unsigned int bShow) = 0;
  virtual int __stdcall CreateAdditionalSwapChain(void * pPresentationParameters, IDirect3DSwapChain8 ** pSwapChain) = 0;
  virtual int __stdcall Reset(void * pPresentationParameters) = 0;
  virtual int __stdcall Present(void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region) = 0;
  virtual int __stdcall GetBackBuffer(unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer) = 0;
  virtual int __stdcall GetRasterStatus(void * pRasterStatus) = 0;
  virtual int __stdcall SetGammaRamp(unsigned int flags, void * ramp) = 0;
  virtual int __stdcall GetGammaRamp(void * pRamp) = 0;
  virtual int __stdcall CreateTexture(unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 ** ppTexture) = 0;
  virtual int __stdcall CreateVolumeTexture(unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 ** ppVolumeTexture) = 0;
  virtual int __stdcall CreateCubeTexture(unsigned int EdgeLength, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 ** ppCubeTexture) = 0;
  virtual int __stdcall CreateVertexBuffer(unsigned int Length, unsigned int Usage, unsigned int FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 ** ppVertexBuffer) = 0;
  virtual int __stdcall CreateIndexBuffer(unsigned int Length, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 ** ppIndexBuffer) = 0;
  virtual int __stdcall CreateRenderTarget(unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, unsigned int Lockable, IDirect3DSurface8 ** ppSurface) = 0;
  virtual int __stdcall CreateDepthStencilSurface(unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 ** ppSurface) = 0;
  virtual int __stdcall CreateImageSurface(unsigned int Width, unsigned int Height, D3DFORMAT Format, IDirect3DSurface8 ** ppSurface) = 0;
  virtual int __stdcall CopyRects(IDirect3DSurface8 * src_surface, void * src_rects, unsigned int rect_count, IDirect3DSurface8 * dst_surface, void * dst_points) = 0;
  virtual int __stdcall UpdateTexture(IDirect3DBaseTexture8 * pSourceTexture, IDirect3DBaseTexture8 * pDestinationTexture) = 0;
  virtual int __stdcall GetFrontBuffer(IDirect3DSurface8 * pDestSurface) = 0;
  virtual int __stdcall SetRenderTarget(IDirect3DSurface8 * pRenderTarget, IDirect3DSurface8 * pNewZStencil) = 0;
  virtual int __stdcall GetRenderTarget(IDirect3DSurface8 ** ppRenderTarget) = 0;
  virtual int __stdcall GetDepthStencilSurface(IDirect3DSurface8 ** ppZStencilSurface) = 0;
  virtual int __stdcall BeginScene() = 0;
  virtual int __stdcall EndScene() = 0;
  virtual int __stdcall Clear(unsigned int rect_count, void * rects, unsigned int flags, unsigned int color, float z, unsigned int stencil) = 0;
  virtual int __stdcall SetTransform(D3DTRANSFORMSTATETYPE state, void * matrix) = 0;
  virtual int __stdcall GetTransform(D3DTRANSFORMSTATETYPE State, void * pMatrix) = 0;
  virtual int __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE state, void * matrix) = 0;
  virtual int __stdcall SetViewport(void * viewport) = 0;
  virtual int __stdcall GetViewport(void * pViewport) = 0;
  virtual int __stdcall SetMaterial(void * material) = 0;
  virtual int __stdcall GetMaterial(void * pMaterial) = 0;
  virtual int __stdcall SetLight(unsigned int index, void * light) = 0;
  virtual int __stdcall GetLight(unsigned int Index, void * pLight) = 0;
  virtual int __stdcall LightEnable(unsigned int Index, unsigned int Enable) = 0;
  virtual int __stdcall GetLightEnable(unsigned int Index, void * pEnable) = 0;
  virtual int __stdcall SetClipPlane(unsigned int index, void * plane) = 0;
  virtual int __stdcall GetClipPlane(unsigned int Index, void * pPlane) = 0;
  virtual int __stdcall SetRenderState(D3DRENDERSTATETYPE State, unsigned int Value) = 0;
  virtual int __stdcall GetRenderState(D3DRENDERSTATETYPE State, void * pValue) = 0;
  virtual int __stdcall BeginStateBlock() = 0;
  virtual int __stdcall EndStateBlock(void * pToken) = 0;
  virtual int __stdcall ApplyStateBlock(unsigned int Token) = 0;
  virtual int __stdcall CaptureStateBlock(unsigned int Token) = 0;
  virtual int __stdcall DeleteStateBlock(unsigned int Token) = 0;
  virtual int __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type, void * pToken) = 0;
  virtual int __stdcall SetClipStatus(void * clip_status) = 0;
  virtual int __stdcall GetClipStatus(void * pClipStatus) = 0;
  virtual int __stdcall GetTexture(unsigned int Stage, IDirect3DBaseTexture8 ** ppTexture) = 0;
  virtual int __stdcall SetTexture(unsigned int Stage, IDirect3DBaseTexture8 * pTexture) = 0;
  virtual int __stdcall GetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, void * pValue) = 0;
  virtual int __stdcall SetTextureStageState(unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value) = 0;
  virtual int __stdcall ValidateDevice(void * pNumPasses) = 0;
  virtual int __stdcall GetInfo(unsigned int DevInfoID, void * pDevInfoStruct, unsigned int DevInfoStructSize) = 0;
  virtual int __stdcall SetPaletteEntries(unsigned int palette_idx, void * entries) = 0;
  virtual int __stdcall GetPaletteEntries(unsigned int PaletteNumber, void * pEntries) = 0;
  virtual int __stdcall SetCurrentTexturePalette(unsigned int PaletteNumber) = 0;
  virtual int __stdcall GetCurrentTexturePalette(void * PaletteNumber) = 0;
  virtual int __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) = 0;
  virtual int __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, unsigned int minIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount) = 0;
  virtual int __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE primitive_type, unsigned int primitive_count, void * data, unsigned int stride) = 0;
  virtual int __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE primitive_type, unsigned int min_vertex_idx, unsigned int vertex_count, unsigned int primitive_count, void * index_data, D3DFORMAT index_format, void * data, unsigned int stride) = 0;
  virtual int __stdcall ProcessVertices(unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer8 * pDestBuffer, unsigned int Flags) = 0;
  virtual int __stdcall CreateVertexShader(void * declaration, void * byte_code, void * shader, unsigned int usage) = 0;
  virtual int __stdcall SetVertexShader(unsigned int Handle) = 0;
  virtual int __stdcall GetVertexShader(void * pHandle) = 0;
  virtual int __stdcall DeleteVertexShader(unsigned int Handle) = 0;
  virtual int __stdcall SetVertexShaderConstant(unsigned int reg_idx, void * data, unsigned int count) = 0;
  virtual int __stdcall GetVertexShaderConstant(unsigned int Register, void * pConstantData, unsigned int ConstantCount) = 0;
  virtual int __stdcall GetVertexShaderDeclaration(unsigned int Handle, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall GetVertexShaderFunction(unsigned int Handle, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer8 * pStreamData, unsigned int Stride) = 0;
  virtual int __stdcall GetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer8 ** ppStreamData, void * pStride) = 0;
  virtual int __stdcall SetIndices(IDirect3DIndexBuffer8 * pIndexData, unsigned int BaseVertexIndex) = 0;
  virtual int __stdcall GetIndices(IDirect3DIndexBuffer8 ** ppIndexData, void * pBaseVertexIndex) = 0;
  virtual int __stdcall CreatePixelShader(void * byte_code, void * shader) = 0;
  virtual int __stdcall SetPixelShader(unsigned int Handle) = 0;
  virtual int __stdcall GetPixelShader(void * pHandle) = 0;
  virtual int __stdcall DeletePixelShader(unsigned int Handle) = 0;
  virtual int __stdcall SetPixelShaderConstant(unsigned int reg_idx, void * data, unsigned int count) = 0;
  virtual int __stdcall GetPixelShaderConstant(unsigned int Register, void * pConstantData, unsigned int ConstantCount) = 0;
  virtual int __stdcall GetPixelShaderFunction(unsigned int Handle, void * pData, void * pSizeOfData) = 0;
  virtual int __stdcall DrawRectPatch(unsigned int handle, void * segment_count, void * patch_info) = 0;
  virtual int __stdcall DrawTriPatch(unsigned int handle, void * segment_count, void * patch_info) = 0;
  virtual int __stdcall DeletePatch(unsigned int Handle) = 0;
};
struct D3DSURFACE_DESC { D3DFORMAT Format; D3DRESOURCETYPE Type; unsigned int Usage; D3DPOOL Pool; unsigned int Size;
  D3DMULTISAMPLE_TYPE MultiSampleType; unsigned int Width; unsigned int Height; };
extern int g_pResourceMng_1047BFA8;
struct XProc {
  virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
  virtual void v06(); virtual void v07(); virtual void v08();
  virtual void v09(float *out, float *a, float *b);
  virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
  virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
  virtual void v22(float *m, float x, float y, float z);
};
extern XProc *g_pProcessor_1047CF7C;
extern unsigned long DAT_1047BC0C;
extern unsigned int g_Fn100412B0RenderStates3_103529B0[], g_Fn100412B0TextureStageStates4_103528F0[];
extern unsigned int g_Fn100412B0RenderStates_10352B24[], g_Fn100412B0TextureStageStates2_103529C8[];
extern unsigned int g_Fn100412B0RenderStates2_10352C60[], g_Fn100412B0TextureStageStates3_10352B40[];
extern unsigned int g_Fn100412B0TextureStageStates_10352C78[];
struct XM64 { float m[16]; };
extern "C" XM64 *__cdecl FUN_10015230(void);
struct XVB_10043F40 { void FUN_10043F40(int a, int b, int c, int d, int e); };
struct XMeshVB { char pad[4]; XVB_10043F40 *vb; };
struct XSub { char pad[16]; void FUN_10075700(int a, int b, int c, int d); };
struct XMesh {
  XMeshVB *FUN_1003FCF0();
  unsigned short *FUN_1003FCE0();
};
struct XStateBlock {
  unsigned int handle, a, b;
  XStateBlock();
  ~XStateBlock();
  int FUN_1006AE30(unsigned int *rs, unsigned int *tss);
  int FUN_1006AF90(unsigned int *tss);
};
struct FUNt_100746A0 { void f(); };
struct FUNt_1000BD50 { int f(int, void *); };
struct XP { XMesh **p; XMesh *get() { return p ? *p : 0; } };
struct XEnt {
  float gx() { return *(float *)((char *)this + 0x120); } float gy() { return *(float *)((char *)this + 0x124); }
  void FUN_10040C20();
  void Render();
};
#define B(o, t) (*(t *)((char *)this + (o)))
#define DEVB(g) (*(IDirect3DDevice8 **)((g) + 0xebc))
#define RS(s, v) { int g_ = g_pResourceMng_1047BFA8; DEVB(g_)->SetRenderState((D3DRENDERSTATETYPE)(s), (v)); }
#define TSS(st, s, v) { int g_ = g_pResourceMng_1047BFA8; DEVB(g_)->SetTextureStageState((st), (D3DTEXTURESTAGESTATETYPE)(s), (v)); }
#define PR ((char *)g_pProcessor_1047CF7C)
// FUNCTION: FFXIMAIN 0x100412B0
void XEnt::Render()
{
  XMesh *o, *o2;
  float *m, *m2;
  XSub *s;
  unsigned short *idx;
  int i, start, op;
  if (B(0x104, XMesh **) == 0) return;
  if (B(0x179, char)) { FUN_10040C20(); return; }
  o = ((XP *)((char *)this + 0x104))->get();
  if (o->FUN_1003FCF0()->vb == 0) return;
  o2 = ((XP *)((char *)this + 0x104))->get();
  RS(7, 1);
  RS(60, DAT_1047BC0C);
  { int top = *(int *)(PR + 0xA90); m = ((float **)(PR + 0xA10))[top]; *(int *)(PR + 0xA90) = top + 1; }
  if (B(0x198, unsigned char) & 1)
  {
    g_pProcessor_1047CF7C->v22(m, -B(0x19c, float), -(B(0x1a0, float) + 1.5707964f), -B(0x1a4, float));
    if (B(0x198, unsigned char) & 2)
    {
      { int top = *(int *)(PR + 0xA90); m2 = ((float **)(PR + 0xA10))[top]; *(int *)(PR + 0xA90) = top + 1; }
      *(XM64 *)m2 = *FUN_10015230();
      m2[12] = 0.0f; m2[13] = 0.0f; m2[14] = 0.0f; m2[15] = 1.0f;
      g_pProcessor_1047CF7C->v09(m, m, m2);
      { char *pr = PR; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m2; }
    }
  }
  else
  {
    g_pProcessor_1047CF7C->v22(m, 0.0f, -1.5707964f, 0.0f);
  }
  DEVB(g_pResourceMng_1047BFA8)->SetTransform((D3DTRANSFORMSTATETYPE)(B(0x1bd, signed char) + 16), m);
  if (*((char *)o2 + 0x34) == 0)
  {
    static XStateBlock sb4;
    sb4.FUN_1006AE30(g_Fn100412B0RenderStates3_103529B0, g_Fn100412B0TextureStageStates4_103528F0);
    if (B(0x198, unsigned char) & 8) { TSS(1, 13, 3); TSS(1, 14, 3); }
    else { TSS(1, 13, 1); TSS(1, 14, 1); }
    o->FUN_1003FCF0()->vb->FUN_10043F40(0, *(unsigned short *)((char *)o2 + 0x36), 4, 0, -1);
  }
  else
  {
    if (B(0x16c, unsigned char) == 100 || ((unsigned char)(B(0x10c, unsigned int) >> 28) & 1)) op = 2;
    else op = 6;
    if (B(0x1b8, unsigned char) & 2)
    {
      static XStateBlock sb2;
      sb2.FUN_1006AE30(g_Fn100412B0RenderStates_10352B24, g_Fn100412B0TextureStageStates2_103529C8);
      TSS(1, 4, op);
      if (B(0x198, unsigned char) & 8) { TSS(2, 13, 3); TSS(2, 14, 3); }
      else { TSS(2, 13, 1); TSS(2, 14, 1); }
    }
    else
    {
      static XStateBlock sb3;
      sb3.FUN_1006AE30(g_Fn100412B0RenderStates2_10352C60, g_Fn100412B0TextureStageStates3_10352B40);
      TSS(1, 4, op);
      if (B(0x198, unsigned char) & 8) { TSS(1, 13, 3); TSS(1, 14, 3); }
      else { TSS(1, 13, 1); TSS(1, 14, 1); }
    }
    if (B(0x17b, unsigned char) & 3)
    {
      float *m3;
      { int top = *(int *)(PR + 0xA90); m3 = ((float **)(PR + 0xA10))[top]; *(int *)(PR + 0xA90) = top + 1; }
      ((FUNt_100746A0 *)m3)->f();
      m3[8] = gx();
      m3[9] = gy();
      TSS(B(0x1bc, signed char), 13, 1); TSS(B(0x1bc, signed char), 14, 1);
      TSS(B(0x1bc, signed char), 11, 0); TSS(B(0x1bc, signed char), 24, 2);
      ((FUNt_1000BD50 *)*(void **)(g_pResourceMng_1047BFA8 + 0xdc8))->f(B(0x1bc, signed char) + 16, m3);
      { char *pr = PR; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m3; }
    }
    else
    {
      DEVB(g_pResourceMng_1047BFA8)->SetTransform((D3DTRANSFORMSTATETYPE)(B(0x1bc, signed char) + 16), (PR + 0x810));
    }
    start = 0;
    idx = o->FUN_1003FCE0();
    s = (XSub *)o->FUN_1003FCF0();
    for (i = 0; i < *((unsigned char *)o2 + 0x34); i++)
    {
      s[i].FUN_10075700(B(0x1bc, signed char), B(0x140, int), B(0x144, int), B(0x148, int));
      o->FUN_1003FCF0()->vb->FUN_10043F40(start, idx[i], 4, 0, -1);
      start += idx[i] * 3;
    }
    if (*((char *)o2 + 0x35))
    {
      static XStateBlock sb1;
      sb1.FUN_1006AF90(g_Fn100412B0TextureStageStates_10352C78);
      o->FUN_1003FCF0()->vb->FUN_10043F40(start, *(unsigned short *)((char *)o2 + 0x36) - start / 3, 4, 0, -1);
    }
    TSS(0, 28, 1); TSS(0, 1, 1); TSS(0, 4, 1);
    TSS(B(0x1bd, signed char), 11, 0); TSS(B(0x1bd, signed char), 24, 0);
    RS(143, 0);
    if (B(0x17b, unsigned char) & 3)
    {
      TSS(B(0x1bc, signed char), 24, 0);
      ((FUNt_1000BD50 *)*(void **)(g_pResourceMng_1047BFA8 + 0xdc8))->f(B(0x1bc, signed char) + 16, PR + 0x810);
    }
  }
  { char *pr = PR; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m; }
}
