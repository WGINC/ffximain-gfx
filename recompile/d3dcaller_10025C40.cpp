/* Task #119: 0x10025C40, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/unit_100252F0.cpp); written the way the game's bytes require (recompile/d3d_round_118.md, d3d_round_119.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
/* Direct3D 8, from tree/include/xi_d3d8.hpp (MinGW-w64 d3d8.h / d3d8types.h): the enums and structs used here;
   each interface in the SDK's C++ form (d3d8.h DECLARE_INTERFACE_ / STDMETHOD: __stdcall virtual methods in
   vtable order), declared through the slot each call uses */
typedef unsigned char undefined, undefined1, byte, uchar; typedef unsigned short undefined2, ushort, word;
typedef void *LPVOID; typedef unsigned long DWORD; typedef unsigned int _DWORD; typedef unsigned short _WORD;
#define DWORDn(x, n) (*((_DWORD *)&(x) + (n)))
#define LODWORD(x) DWORDn(x, 0)
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
struct IDirect3DVertexBuffer8 {
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
  virtual int __stdcall Lock(unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags) = 0;
  virtual int __stdcall Unlock() = 0;
  virtual int __stdcall GetDesc(void * pDesc) = 0;
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
struct Iostream_init; typedef struct Iostream_init Iostream_init;
extern int g_dword10456E74_10456E74;
#define dword_10456E74 g_dword10456E74_10456E74
extern int g_dword10456E78_10456E78;
#define dword_10456E78 g_dword10456E78_10456E78
extern int g_dword10456E88_10456E88;
#define dword_10456E88 g_dword10456E88_10456E88
extern int g_cfgGraphicsStabilization_10456A74;
#define g_cfgGraphicsStabilization g_cfgGraphicsStabilization_10456A74
extern int g_envMapFactor_10456E8C;
#define g_envMapFactor g_envMapFactor_10456E8C
extern int g_envMapPrevFactor_10456E7C;
#define g_envMapPrevFactor g_envMapPrevFactor_10456E7C
extern int g_envMapPrevTex0_10456E80;
#define g_envMapPrevTex0 g_envMapPrevTex0_10456E80
extern int g_envMapPrevTex1_10456E84;
#define g_envMapPrevTex1 g_envMapPrevTex1_10456E84
extern int g_pAppSettings_103D3CB8;
#define g_pAppSettings g_pAppSettings_103D3CB8
struct CDx; typedef struct CDx CDx;
extern CDx * g_pCDx_1045666C;
#define g_pCDx g_pCDx_1045666C
struct FUNt_10027990 { Iostream_init * f(void); };
struct FUNt_10075960 { int f(int); };
struct FUNt_100279B0 { int f(void); };
struct FUNt_10027E90 { int f(void *); };
struct FUNt_100279A0 { void f(void); };
// FUNCTION: FFXIMAIN 0x10025C40
extern "C" char __stdcall EnvMap_BeginPass(int *a1, int a2, float *a3)
{
  float v4[16];
  ((FUNt_10027990 *)((Iostream_init *)v4))->f();
  if ( dword_10456E88 == 0 )
  {
    ((FUNt_100279A0 *)v4)->f();
    return 0;
  }
  {
    g_envMapPrevTex0 = *a1;
    g_envMapPrevTex1 = a1[1];
    g_envMapPrevFactor = a2;
    if ( *(_DWORD *)dword_10456E88 != 0 )
      ((FUNt_10075960 *)((int *)(*(_DWORD *)dword_10456E88 + 48)))->f(0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_ALPHAFUNC, 8u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_ZWRITEENABLE, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_ADDRESSU, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_ADDRESSV, 1u);
    if ( *(_DWORD *)(g_pAppSettings + 92) == 0 && g_cfgGraphicsStabilization == 0 )
      (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_WRAP0, 7u);
    ((FUNt_100279B0 *)(v4))->f();
    LODWORD(v4[12]) = dword_10456E74;
    LODWORD(v4[13]) = dword_10456E78;
    ((FUNt_10027E90 *)(v4))->f(a3);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_LIGHTING, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_ALPHABLENDENABLE, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_SRCBLEND, 5u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_DESTBLEND, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTransform(D3DTS_TEXTURE0, v4);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0x10000u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_COLOROP, 5u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_COLORARG1, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_COLORARG2, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_ALPHAOP, 5u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_ALPHAARG1, 2u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(0, D3DTSS_ALPHAARG2, 0);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_COLOROP, 5u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_COLORARG1, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_COLORARG2, 3u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_ALPHAOP, 5u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_ALPHAARG1, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(1u, D3DTSS_ALPHAARG2, 3u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(2u, D3DTSS_COLOROP, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetTextureStageState(2u, D3DTSS_ALPHAOP, 1u);
    (*(IDirect3DDevice8 * *)((char *)g_pCDx + 0xC))->SetRenderState(D3DRS_TEXTUREFACTOR, g_envMapFactor);
    ((FUNt_100279A0 *)v4)->f();
    return 1;
  }
}
