struct tagBITMAPINFO;
struct _iobuf;
struct tagPOINT;
struct IDirect3D8;
struct IDirect3DVolume8;
struct IDirect3DSwapChain8;
struct IDirect3DSurface8;
struct IDirect3DVertexBuffer8;
struct IDirect3DIndexBuffer8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DTexture8;
struct IDirect3DVolumeTexture8;
struct IDirect3DDevice8;
struct IDirect3D8Vtbl;
struct IDirect3DVolume8Vtbl;
struct IDirect3DSwapChain8Vtbl;
struct IDirect3DSurface8Vtbl;
struct IDirect3DVertexBuffer8Vtbl;
struct IDirect3DIndexBuffer8Vtbl;
struct IDirect3DBaseTexture8Vtbl;
struct IDirect3DCubeTexture8Vtbl;
struct IDirect3DTexture8Vtbl;
struct IDirect3DVolumeTexture8Vtbl;
struct IDirect3DDevice8Vtbl;

/* 65 */
typedef struct tagBITMAPINFO BITMAPINFO;

/* 3 */
typedef unsigned int DWORD;

/* 41 */
typedef int LONG;

/* 38 */
typedef unsigned __int16 WORD;

/* 68 */
struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
};

/* 67 */
typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER;

/* 62 */
typedef unsigned __int8 BYTE;

/* 70 */
struct tagRGBQUAD
{
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
};

/* 69 */
typedef struct tagRGBQUAD RGBQUAD;

/* 66 */
struct tagBITMAPINFO
{
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[1];
};

/* 33 */
typedef int BOOL;

/* 237 */
#pragma pack(push, 1)
/// recon: tree/include/CYyObject.hpp, sizeof 0x4 from the binary's descriptor
struct CYyObject
{
  void **vftable;
};
#pragma pack(pop)

/* 244 */
#pragma pack(push, 1)
struct CDx
{
  CYyObject base;
  unsigned __int8 _pad_4[4];
  IDirect3D8 *pD3D;
  IDirect3DDevice8 *pDevice;
  unsigned __int8 _pad_10[2624];
};
#pragma pack(pop)

/* 489 */
struct IDirect3D8
{
  struct IDirect3D8Vtbl *lpVtbl;
};

/* 500 */
struct IDirect3DDevice8
{
  struct IDirect3DDevice8Vtbl *lpVtbl;
};

/* 485 */
enum D3DDEVTYPE
{
  D3DDEVTYPE_HAL = 0x1,
  D3DDEVTYPE_REF = 0x2,
  D3DDEVTYPE_SW = 0x3,
};

/* 484 */
enum D3DRESOURCETYPE
{
  D3DRTYPE_SURFACE = 0x1,
  D3DRTYPE_VOLUME = 0x2,
  D3DRTYPE_TEXTURE = 0x3,
  D3DRTYPE_VOLUMETEXTURE = 0x4,
  D3DRTYPE_CUBETEXTURE = 0x5,
  D3DRTYPE_VERTEXBUFFER = 0x6,
  D3DRTYPE_INDEXBUFFER = 0x7,
};

/* 486 */
enum D3DMULTISAMPLE_TYPE
{
  D3DMULTISAMPLE_NONE = 0x0,
  D3DMULTISAMPLE_2_SAMPLES = 0x2,
  D3DMULTISAMPLE_3_SAMPLES = 0x3,
  D3DMULTISAMPLE_4_SAMPLES = 0x4,
  D3DMULTISAMPLE_5_SAMPLES = 0x5,
  D3DMULTISAMPLE_6_SAMPLES = 0x6,
  D3DMULTISAMPLE_7_SAMPLES = 0x7,
  D3DMULTISAMPLE_8_SAMPLES = 0x8,
  D3DMULTISAMPLE_9_SAMPLES = 0x9,
  D3DMULTISAMPLE_10_SAMPLES = 0xA,
  D3DMULTISAMPLE_11_SAMPLES = 0xB,
  D3DMULTISAMPLE_12_SAMPLES = 0xC,
  D3DMULTISAMPLE_13_SAMPLES = 0xD,
  D3DMULTISAMPLE_14_SAMPLES = 0xE,
  D3DMULTISAMPLE_15_SAMPLES = 0xF,
  D3DMULTISAMPLE_16_SAMPLES = 0x10,
};

/* 501 */
struct IDirect3D8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3D8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3D8 *This);
  int (__stdcall *Release)(IDirect3D8 *This);
  int (__stdcall *RegisterSoftwareDevice)(IDirect3D8 *This, void *pInitializeFunction);
  int (__stdcall *GetAdapterCount)(IDirect3D8 *This);
  int (__stdcall *GetAdapterIdentifier)(IDirect3D8 *This, unsigned int Adapter, unsigned int Flags, void *pIdentifier);
  int (__stdcall *GetAdapterModeCount)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *EnumAdapterModes)(IDirect3D8 *This, unsigned int Adapter, unsigned int Mode, void *pMode);
  int (__stdcall *GetAdapterDisplayMode)(IDirect3D8 *This, unsigned int Adapter, void *pMode);
  int (__stdcall *CheckDeviceType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, unsigned int Windowed);
  int (__stdcall *CheckDeviceFormat)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, unsigned int Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
  int (__stdcall *CheckDeviceMultiSampleType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, unsigned int Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
  int (__stdcall *CheckDepthStencilMatch)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
  int (__stdcall *GetDeviceCaps)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, void *pCaps);
  int (__stdcall *GetAdapterMonitor)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *CreateDevice)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, unsigned int hFocusWindow, unsigned int BehaviorFlags, void *pPresentationParameters, IDirect3DDevice8 **ppReturnedDeviceInterface);
};

/* 487 */
enum D3DBACKBUFFER_TYPE
{
  D3DBACKBUFFER_TYPE_MONO = 0x0,
  D3DBACKBUFFER_TYPE_LEFT = 0x1,
  D3DBACKBUFFER_TYPE_RIGHT = 0x2,
};

/* 483 */
enum D3DPOOL
{
  D3DPOOL_DEFAULT = 0x0,
  D3DPOOL_MANAGED = 0x1,
  D3DPOOL_SYSTEMMEM = 0x2,
  D3DPOOL_SCRATCH = 0x3,
};

/* 480 */
enum D3DTRANSFORMSTATETYPE
{
  D3DTS_VIEW = 0x2,
  D3DTS_PROJECTION = 0x3,
  D3DTS_TEXTURE0 = 0x10,
  D3DTS_TEXTURE1 = 0x11,
  D3DTS_TEXTURE2 = 0x12,
  D3DTS_TEXTURE3 = 0x13,
  D3DTS_TEXTURE4 = 0x14,
  D3DTS_TEXTURE5 = 0x15,
  D3DTS_TEXTURE6 = 0x16,
  D3DTS_TEXTURE7 = 0x17,
};

/* 479 */
enum D3DRENDERSTATETYPE
{
  D3DRS_ZENABLE = 0x7,
  D3DRS_FILLMODE = 0x8,
  D3DRS_SHADEMODE = 0x9,
  D3DRS_LINEPATTERN = 0xA,
  D3DRS_ZWRITEENABLE = 0xE,
  D3DRS_ALPHATESTENABLE = 0xF,
  D3DRS_LASTPIXEL = 0x10,
  D3DRS_SRCBLEND = 0x13,
  D3DRS_DESTBLEND = 0x14,
  D3DRS_CULLMODE = 0x16,
  D3DRS_ZFUNC = 0x17,
  D3DRS_ALPHAREF = 0x18,
  D3DRS_ALPHAFUNC = 0x19,
  D3DRS_DITHERENABLE = 0x1A,
  D3DRS_ALPHABLENDENABLE = 0x1B,
  D3DRS_FOGENABLE = 0x1C,
  D3DRS_SPECULARENABLE = 0x1D,
  D3DRS_ZVISIBLE = 0x1E,
  D3DRS_FOGCOLOR = 0x22,
  D3DRS_FOGTABLEMODE = 0x23,
  D3DRS_FOGSTART = 0x24,
  D3DRS_FOGEND = 0x25,
  D3DRS_FOGDENSITY = 0x26,
  D3DRS_EDGEANTIALIAS = 0x28,
  D3DRS_ZBIAS = 0x2F,
  D3DRS_RANGEFOGENABLE = 0x30,
  D3DRS_STENCILENABLE = 0x34,
  D3DRS_STENCILFAIL = 0x35,
  D3DRS_STENCILZFAIL = 0x36,
  D3DRS_STENCILPASS = 0x37,
  D3DRS_STENCILFUNC = 0x38,
  D3DRS_STENCILREF = 0x39,
  D3DRS_STENCILMASK = 0x3A,
  D3DRS_STENCILWRITEMASK = 0x3B,
  D3DRS_TEXTUREFACTOR = 0x3C,
  D3DRS_WRAP0 = 0x80,
  D3DRS_WRAP1 = 0x81,
  D3DRS_WRAP2 = 0x82,
  D3DRS_WRAP3 = 0x83,
  D3DRS_WRAP4 = 0x84,
  D3DRS_WRAP5 = 0x85,
  D3DRS_WRAP6 = 0x86,
  D3DRS_WRAP7 = 0x87,
  D3DRS_CLIPPING = 0x88,
  D3DRS_LIGHTING = 0x89,
  D3DRS_AMBIENT = 0x8B,
  D3DRS_FOGVERTEXMODE = 0x8C,
  D3DRS_COLORVERTEX = 0x8D,
  D3DRS_LOCALVIEWER = 0x8E,
  D3DRS_NORMALIZENORMALS = 0x8F,
  D3DRS_DIFFUSEMATERIALSOURCE = 0x91,
  D3DRS_SPECULARMATERIALSOURCE = 0x92,
  D3DRS_AMBIENTMATERIALSOURCE = 0x93,
  D3DRS_EMISSIVEMATERIALSOURCE = 0x94,
  D3DRS_VERTEXBLEND = 0x97,
  D3DRS_CLIPPLANEENABLE = 0x98,
  D3DRS_SOFTWAREVERTEXPROCESSING = 0x99,
  D3DRS_POINTSIZE = 0x9A,
  D3DRS_POINTSIZE_MIN = 0x9B,
  D3DRS_POINTSPRITEENABLE = 0x9C,
  D3DRS_POINTSCALEENABLE = 0x9D,
  D3DRS_POINTSCALE_A = 0x9E,
  D3DRS_POINTSCALE_B = 0x9F,
  D3DRS_POINTSCALE_C = 0xA0,
  D3DRS_MULTISAMPLEANTIALIAS = 0xA1,
  D3DRS_MULTISAMPLEMASK = 0xA2,
  D3DRS_PATCHEDGESTYLE = 0xA3,
  D3DRS_PATCHSEGMENTS = 0xA4,
  D3DRS_DEBUGMONITORTOKEN = 0xA5,
  D3DRS_POINTSIZE_MAX = 0xA6,
  D3DRS_INDEXEDVERTEXBLENDENABLE = 0xA7,
  D3DRS_COLORWRITEENABLE = 0xA8,
  D3DRS_TWEENFACTOR = 0xAA,
  D3DRS_BLENDOP = 0xAB,
  D3DRS_POSITIONORDER = 0xAC,
  D3DRS_NORMALORDER = 0xAD,
};

/* 488 */
enum D3DSTATEBLOCKTYPE
{
  D3DSBT_ALL = 0x1,
  D3DSBT_PIXELSTATE = 0x2,
  D3DSBT_VERTEXSTATE = 0x3,
};

/* 481 */
enum D3DTEXTURESTAGESTATETYPE
{
  D3DTSS_COLOROP = 0x1,
  D3DTSS_COLORARG1 = 0x2,
  D3DTSS_COLORARG2 = 0x3,
  D3DTSS_ALPHAOP = 0x4,
  D3DTSS_ALPHAARG1 = 0x5,
  D3DTSS_ALPHAARG2 = 0x6,
  D3DTSS_BUMPENVMAT00 = 0x7,
  D3DTSS_BUMPENVMAT01 = 0x8,
  D3DTSS_BUMPENVMAT10 = 0x9,
  D3DTSS_BUMPENVMAT11 = 0xA,
  D3DTSS_TEXCOORDINDEX = 0xB,
  D3DTSS_ADDRESSU = 0xD,
  D3DTSS_ADDRESSV = 0xE,
  D3DTSS_BORDERCOLOR = 0xF,
  D3DTSS_MAGFILTER = 0x10,
  D3DTSS_MINFILTER = 0x11,
  D3DTSS_MIPFILTER = 0x12,
  D3DTSS_MIPMAPLODBIAS = 0x13,
  D3DTSS_MAXMIPLEVEL = 0x14,
  D3DTSS_MAXANISOTROPY = 0x15,
  D3DTSS_BUMPENVLSCALE = 0x16,
  D3DTSS_BUMPENVLOFFSET = 0x17,
  D3DTSS_TEXTURETRANSFORMFLAGS = 0x18,
  D3DTSS_ADDRESSW = 0x19,
  D3DTSS_COLORARG0 = 0x1A,
  D3DTSS_ALPHAARG0 = 0x1B,
  D3DTSS_RESULTARG = 0x1C,
};

/* 482 */
enum D3DPRIMITIVETYPE
{
  D3DPT_POINTLIST = 0x1,
  D3DPT_LINELIST = 0x2,
  D3DPT_LINESTRIP = 0x3,
  D3DPT_TRIANGLELIST = 0x4,
  D3DPT_TRIANGLESTRIP = 0x5,
  D3DPT_TRIANGLEFAN = 0x6,
};

/* 512 */
struct IDirect3DDevice8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DDevice8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DDevice8 *This);
  int (__stdcall *Release)(IDirect3DDevice8 *This);
  int (__stdcall *TestCooperativeLevel)(IDirect3DDevice8 *This);
  int (__stdcall *GetAvailableTextureMem)(IDirect3DDevice8 *This);
  int (__stdcall *ResourceManagerDiscardBytes)(IDirect3DDevice8 *This, unsigned int Bytes);
  int (__stdcall *GetDirect3D)(IDirect3DDevice8 *This, IDirect3D8 **ppD3D8);
  int (__stdcall *GetDeviceCaps)(IDirect3DDevice8 *This, void *pCaps);
  int (__stdcall *GetDisplayMode)(IDirect3DDevice8 *This, void *pMode);
  int (__stdcall *GetCreationParameters)(IDirect3DDevice8 *This, void *pParameters);
  int (__stdcall *SetCursorProperties)(IDirect3DDevice8 *This, unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface8 *pCursorBitmap);
  int (__stdcall *SetCursorPosition)(IDirect3DDevice8 *This, unsigned int XScreenSpace, unsigned int YScreenSpace, unsigned int Flags);
  int (__stdcall *ShowCursor)(IDirect3DDevice8 *This, unsigned int bShow);
  int (__stdcall *CreateAdditionalSwapChain)(IDirect3DDevice8 *This, void *pPresentationParameters, IDirect3DSwapChain8 **pSwapChain);
  int (__stdcall *Reset)(IDirect3DDevice8 *This, void *pPresentationParameters);
  int (__stdcall *Present)(IDirect3DDevice8 *This, void *src_rect, void *dst_rect, unsigned int dst_window_override, void *dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DDevice8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
  int (__stdcall *GetRasterStatus)(IDirect3DDevice8 *This, void *pRasterStatus);
  int (__stdcall *SetGammaRamp)(IDirect3DDevice8 *This, unsigned int flags, void *ramp);
  int (__stdcall *GetGammaRamp)(IDirect3DDevice8 *This, void *pRamp);
  int (__stdcall *CreateTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 **ppTexture);
  int (__stdcall *CreateVolumeTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 **ppVolumeTexture);
  int (__stdcall *CreateCubeTexture)(IDirect3DDevice8 *This, unsigned int EdgeLength, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 **ppCubeTexture);
  int (__stdcall *CreateVertexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, unsigned int FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 **ppVertexBuffer);
  int (__stdcall *CreateIndexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 **ppIndexBuffer);
  int (__stdcall *CreateRenderTarget)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, unsigned int Lockable, IDirect3DSurface8 **ppSurface);
  int (__stdcall *CreateDepthStencilSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 **ppSurface);
  int (__stdcall *CreateImageSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, IDirect3DSurface8 **ppSurface);
  int (__stdcall *CopyRects)(IDirect3DDevice8 *This, IDirect3DSurface8 *src_surface, void *src_rects, unsigned int rect_count, IDirect3DSurface8 *dst_surface, void *dst_points);
  int (__stdcall *UpdateTexture)(IDirect3DDevice8 *This, IDirect3DBaseTexture8 *pSourceTexture, IDirect3DBaseTexture8 *pDestinationTexture);
  int (__stdcall *GetFrontBuffer)(IDirect3DDevice8 *This, IDirect3DSurface8 *pDestSurface);
  int (__stdcall *SetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 *pRenderTarget, IDirect3DSurface8 *pNewZStencil);
  int (__stdcall *GetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppRenderTarget);
  int (__stdcall *GetDepthStencilSurface)(IDirect3DDevice8 *This, IDirect3DSurface8 **ppZStencilSurface);
  int (__stdcall *BeginScene)(IDirect3DDevice8 *This);
  int (__stdcall *EndScene)(IDirect3DDevice8 *This);
  int (__stdcall *Clear)(IDirect3DDevice8 *This, unsigned int rect_count, void *rects, unsigned int flags, unsigned int color, float z, unsigned int stencil);
  int (__stdcall *SetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void *matrix);
  int (__stdcall *GetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, void *pMatrix);
  int (__stdcall *MultiplyTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void *matrix);
  int (__stdcall *SetViewport)(IDirect3DDevice8 *This, void *viewport);
  int (__stdcall *GetViewport)(IDirect3DDevice8 *This, void *pViewport);
  int (__stdcall *SetMaterial)(IDirect3DDevice8 *This, void *material);
  int (__stdcall *GetMaterial)(IDirect3DDevice8 *This, void *pMaterial);
  int (__stdcall *SetLight)(IDirect3DDevice8 *This, unsigned int index, void *light);
  int (__stdcall *GetLight)(IDirect3DDevice8 *This, unsigned int Index, void *pLight);
  int (__stdcall *LightEnable)(IDirect3DDevice8 *This, unsigned int Index, unsigned int Enable);
  int (__stdcall *GetLightEnable)(IDirect3DDevice8 *This, unsigned int Index, void *pEnable);
  int (__stdcall *SetClipPlane)(IDirect3DDevice8 *This, unsigned int index, void *plane);
  int (__stdcall *GetClipPlane)(IDirect3DDevice8 *This, unsigned int Index, void *pPlane);
  int (__stdcall *SetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, unsigned int Value);
  int (__stdcall *GetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, void *pValue);
  int (__stdcall *BeginStateBlock)(IDirect3DDevice8 *This);
  int (__stdcall *EndStateBlock)(IDirect3DDevice8 *This, void *pToken);
  int (__stdcall *ApplyStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CaptureStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *DeleteStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CreateStateBlock)(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, void *pToken);
  int (__stdcall *SetClipStatus)(IDirect3DDevice8 *This, void *clip_status);
  int (__stdcall *GetClipStatus)(IDirect3DDevice8 *This, void *pClipStatus);
  int (__stdcall *GetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 **ppTexture);
  int (__stdcall *SetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 *pTexture);
  int (__stdcall *GetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, void *pValue);
  int (__stdcall *SetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value);
  int (__stdcall *ValidateDevice)(IDirect3DDevice8 *This, void *pNumPasses);
  int (__stdcall *GetInfo)(IDirect3DDevice8 *This, unsigned int DevInfoID, void *pDevInfoStruct, unsigned int DevInfoStructSize);
  int (__stdcall *SetPaletteEntries)(IDirect3DDevice8 *This, unsigned int palette_idx, void *entries);
  int (__stdcall *GetPaletteEntries)(IDirect3DDevice8 *This, unsigned int PaletteNumber, void *pEntries);
  int (__stdcall *SetCurrentTexturePalette)(IDirect3DDevice8 *This, unsigned int PaletteNumber);
  int (__stdcall *GetCurrentTexturePalette)(IDirect3DDevice8 *This, void *PaletteNumber);
  int (__stdcall *DrawPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount);
  int (__stdcall *DrawIndexedPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int minIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount);
  int (__stdcall *DrawPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int primitive_count, void *data, unsigned int stride);
  int (__stdcall *DrawIndexedPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int min_vertex_idx, unsigned int vertex_count, unsigned int primitive_count, void *index_data, D3DFORMAT index_format, void *data, unsigned int stride);
  int (__stdcall *ProcessVertices)(IDirect3DDevice8 *This, unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer8 *pDestBuffer, unsigned int Flags);
  int (__stdcall *CreateVertexShader)(IDirect3DDevice8 *This, void *declaration, void *byte_code, void *shader, unsigned int usage);
  int (__stdcall *SetVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetVertexShader)(IDirect3DDevice8 *This, void *pHandle);
  int (__stdcall *DeleteVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void *data, unsigned int count);
  int (__stdcall *GetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void *pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetVertexShaderDeclaration)(IDirect3DDevice8 *This, unsigned int Handle, void *pData, void *pSizeOfData);
  int (__stdcall *GetVertexShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void *pData, void *pSizeOfData);
  int (__stdcall *SetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 *pStreamData, unsigned int Stride);
  int (__stdcall *GetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 **ppStreamData, void *pStride);
  int (__stdcall *SetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 *pIndexData, unsigned int BaseVertexIndex);
  int (__stdcall *GetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 **ppIndexData, void *pBaseVertexIndex);
  int (__stdcall *CreatePixelShader)(IDirect3DDevice8 *This, void *byte_code, void *shader);
  int (__stdcall *SetPixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetPixelShader)(IDirect3DDevice8 *This, void *pHandle);
  int (__stdcall *DeletePixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void *data, unsigned int count);
  int (__stdcall *GetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void *pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetPixelShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void *pData, void *pSizeOfData);
  int (__stdcall *DrawRectPatch)(IDirect3DDevice8 *This, unsigned int handle, void *segment_count, void *patch_info);
  int (__stdcall *DrawTriPatch)(IDirect3DDevice8 *This, unsigned int handle, void *segment_count, void *patch_info);
  int (__stdcall *DeletePatch)(IDirect3DDevice8 *This, unsigned int Handle);
};

/* 492 */
struct IDirect3DSurface8
{
  struct IDirect3DSurface8Vtbl *lpVtbl;
};

/* 491 */
struct IDirect3DSwapChain8
{
  struct IDirect3DSwapChain8Vtbl *lpVtbl;
};

/* 498 */
struct IDirect3DTexture8
{
  struct IDirect3DTexture8Vtbl *lpVtbl;
};

/* 499 */
struct IDirect3DVolumeTexture8
{
  struct IDirect3DVolumeTexture8Vtbl *lpVtbl;
};

/* 497 */
struct IDirect3DCubeTexture8
{
  struct IDirect3DCubeTexture8Vtbl *lpVtbl;
};

/* 494 */
struct IDirect3DVertexBuffer8
{
  struct IDirect3DVertexBuffer8Vtbl *lpVtbl;
};

/* 495 */
struct IDirect3DIndexBuffer8
{
  struct IDirect3DIndexBuffer8Vtbl *lpVtbl;
};

/* 496 */
struct IDirect3DBaseTexture8
{
  struct IDirect3DBaseTexture8Vtbl *lpVtbl;
};

/* 504 */
struct IDirect3DSurface8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DSurface8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DSurface8 *This);
  int (__stdcall *Release)(IDirect3DSurface8 *This);
  int (__stdcall *GetDevice)(IDirect3DSurface8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DSurface8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DSurface8 *This, unsigned int riid, void **ppContainer);
  int (__stdcall *GetDesc)(IDirect3DSurface8 *This, void *pDesc);
  int (__stdcall *LockRect)(IDirect3DSurface8 *This, void *locked_rect, void *rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DSurface8 *This);
};

/* 503 */
struct IDirect3DSwapChain8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DSwapChain8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DSwapChain8 *This);
  int (__stdcall *Release)(IDirect3DSwapChain8 *This);
  int (__stdcall *Present)(IDirect3DSwapChain8 *This, void *src_rect, void *dst_rect, unsigned int dst_window_override, void *dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DSwapChain8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 **ppBackBuffer);
};

/* 510 */
struct IDirect3DTexture8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DTexture8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DTexture8 *This);
  int (__stdcall *Release)(IDirect3DTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DTexture8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DTexture8 *This);
  int (__stdcall *GetType)(IDirect3DTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DTexture8 *This, unsigned int Level, void *pDesc);
  int (__stdcall *GetSurfaceLevel)(IDirect3DTexture8 *This, unsigned int Level, IDirect3DSurface8 **ppSurfaceLevel);
  int (__stdcall *LockRect)(IDirect3DTexture8 *This, unsigned int level, void *locked_rect, void *rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DTexture8 *This, void *dirty_rect);
};

/* 511 */
struct IDirect3DVolumeTexture8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DVolumeTexture8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *Release)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolumeTexture8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVolumeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DVolumeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DVolumeTexture8 *This, unsigned int Level, void *pDesc);
  int (__stdcall *GetVolumeLevel)(IDirect3DVolumeTexture8 *This, unsigned int Level, IDirect3DVolume8 **ppVolumeLevel);
  int (__stdcall *LockBox)(IDirect3DVolumeTexture8 *This, unsigned int level, void *locked_box, void *box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolumeTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyBox)(IDirect3DVolumeTexture8 *This, void *dirty_box);
};

/* 509 */
struct IDirect3DCubeTexture8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DCubeTexture8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DCubeTexture8 *This);
  int (__stdcall *Release)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DCubeTexture8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DCubeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DCubeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DCubeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DCubeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DCubeTexture8 *This, unsigned int Level, void *pDesc);
  int (__stdcall *GetCubeMapSurface)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level, IDirect3DSurface8 **ppCubeMapSurface);
  int (__stdcall *LockRect)(IDirect3DCubeTexture8 *This, unsigned int face, unsigned int level, void *locked_rect, void *rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DCubeTexture8 *This, unsigned int face, void *dirty_rect);
};

/* 506 */
struct IDirect3DVertexBuffer8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DVertexBuffer8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVertexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DVertexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void **ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DVertexBuffer8 *This, void *pDesc);
};

/* 507 */
struct IDirect3DIndexBuffer8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DIndexBuffer8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DIndexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DIndexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void **ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DIndexBuffer8 *This, void *pDesc);
};

/* 508 */
struct IDirect3DBaseTexture8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DBaseTexture8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DBaseTexture8 *This);
  int (__stdcall *Release)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DBaseTexture8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DBaseTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DBaseTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetType)(IDirect3DBaseTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DBaseTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DBaseTexture8 *This);
};

/* 490 */
struct IDirect3DVolume8
{
  struct IDirect3DVolume8Vtbl *lpVtbl;
};

/* 502 */
struct IDirect3DVolume8Vtbl
{
  int (__stdcall *QueryInterface)(IDirect3DVolume8 *This, unsigned int riid, void **ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolume8 *This);
  int (__stdcall *Release)(IDirect3DVolume8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolume8 *This, IDirect3DDevice8 **ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void *data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void *pData, void *pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolume8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DVolume8 *This, unsigned int riid, void **ppContainer);
  int (__stdcall *GetDesc)(IDirect3DVolume8 *This, void *pDesc);
  int (__stdcall *LockBox)(IDirect3DVolume8 *This, void *locked_box, void *box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolume8 *This);
};

/* 16 */
typedef char CHAR;

/* 238 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask
{
  CYyObject base;
  float field_4;
  float field_8;
  float field_C;
  unsigned __int8 _pad_10[36];
};
#pragma pack(pop)

/* 246 */
#pragma pack(push, 1)
/// recon: tree/include/CMoActorColorDriveTask.hpp, sizeof 0x88 from the binary's descriptor
struct CMoActorColorDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[84];
};
#pragma pack(pop)

/* 300 */
#pragma pack(push, 1)
/// recon: tree/include/CMoLockColorDriveTask.hpp, sizeof 0x7C from the binary's descriptor
struct CMoLockColorDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[72];
};
#pragma pack(pop)

/* 301 */
#pragma pack(push, 1)
/// recon: tree/include/CMoLockLookAtDriveTask.hpp, sizeof 0x80 from the binary's descriptor
struct CMoLockLookAtDriveTask
{
  CMoTask base;
  unsigned __int8 _pad_34[76];
};
#pragma pack(pop)

/* 306 */
#pragma pack(push, 1)
/// recon: tree/include/CMoPathDriveActorTask.hpp, sizeof 0xB0 from the binary's descriptor
struct CMoPathDriveActorTask
{
  CMoTask base;
  unsigned __int8 _pad_34[124];
};
#pragma pack(pop)

/* 234 */
struct CMoProcessor
{
  void *vftable;
  float ScratchMatrices[512];
  unsigned __int8 pad_0804[12];
  float RotationMatrix[64];
  unsigned __int8 pad_0910[256];
  float *ScratchMatrixFreeList[32];
  int ScratchMatrixStackTop;
  unsigned __int8 pad_0A94[12];
  float TransposeScratch[16];
  float TransposeScratchTemp[16];
  unsigned __int8 pad_0B20[144];
};

/* 296 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource
{
  CYyObject base;
  unsigned __int32 field_4;
  unsigned __int8 _pad_8[12];
  unsigned __int32 field_14;
  unsigned __int8 _pad_18[24];
};
#pragma pack(pop)

/* 315 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_103366C4.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_103366C4
{
  CMoResource base;
};
#pragma pack(pop)

/* 317 */
#pragma pack(push, 1)
/// recon: tree/include/CMoResource_VT_1032C588.hpp, sizeof 0x30 from the binary's descriptor
struct CMoResource_VT_1032C588
{
  CMoResource_VT_103366C4 base;
};
#pragma pack(pop)

/* 347 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032AFA8.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032AFA8
{
  CMoTask base;
};
#pragma pack(pop)

/* 348 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032B654.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032B654
{
  CMoTask base;
};
#pragma pack(pop)

/* 353 */
#pragma pack(push, 1)
/// recon: tree/include/CMoTask_VT_1032B95C.hpp, sizeof 0x34 from the binary's descriptor
struct CMoTask_VT_1032B95C
{
  CMoTask base;
};
#pragma pack(pop)

/* 236 */
struct CXiSkeletonActor
{
  char gap01_000[136];
  int posHandle;
  char gap02_08c[8];
  float moveSpeedBase;
  float moveSpeed;
  float walkSpeed;
  char gap03_0a0[16];
  char hiddenFlagByte;
  char gap04_0b1;
  __int16 hiddenFlagWord;
  char gap05_0b4[1452];
  unsigned int color;
  char gap06_664[4];
  unsigned int multiTexAlpha;
  unsigned int shadowAlpha;
  char gap07_670[192];
  int emapAlphaMode;
  unsigned int effectFlags;
  char gap08_738[12];
  float distAmount;
  float separateValue;
  unsigned int emapColor;
  void *emapTexture;
  char gap09_754[412];
  char boundUnk0[16];
  float boundRadius;
  char boundUnk1[12];
  char gap10_910[220];
  unsigned int updateSerial;
  unsigned int bucketA;
  unsigned int visibleSampleCount;
  char forceUpdateFlag;
  char gap11_9f9[3];
  unsigned int bucketB;
  char gap12_a00[4];
  unsigned int drawBudgetGate;
};

/* 429 */
#pragma pack(push, 1)
/// recon: tree/include/CYyModelDt.hpp, sizeof 0x3C from the binary's descriptor
struct CYyModelDt
{
  CYyObject base;
  unsigned __int8 _pad_4[56];
};
#pragma pack(pop)

/* 430 */
#pragma pack(push, 1)
/// recon: tree/include/CYyModelDt_VT_1032A430.hpp, sizeof 0x3C from the binary's descriptor
struct CYyModelDt_VT_1032A430
{
  CYyModelDt base;
};
#pragma pack(pop)

/* 513 */
struct D3DHolder
{
  void *vftable;
  IDirect3D8 *pD3D;
  IDirect3DDevice8 *pDevice;
  unsigned __int8 _rest[1132];
};

/* 115 */
typedef struct _iobuf FILE;

/* 116 */
struct _iobuf
{
  char *_ptr;
  int _cnt;
  char *_base;
  int _flag;
  int _file;
  int _charbuf;
  int _bufsiz;
  char *_tmpfname;
};

/* 170 */
struct HBITMAP__
{
  int unused;
};

/* 161 */
struct HICON__
{
  int unused;
};

/* 2 */
struct HINSTANCE__
{
  int unused;
};

/* 27 */
struct HKEY__
{
  int unused;
};

/* 153 */
typedef int HRESULT;

/* 127 */
struct HWND__
{
  int unused;
};

/* 121 */
typedef int LONG_PTR;

/* 120 */
typedef LONG_PTR LPARAM;

/* 159 */
typedef LONG_PTR LRESULT;

/* 129 */
typedef struct tagPOINT POINT;

/* 130 */
struct tagPOINT
{
  LONG x;
  LONG y;
};

/* 14 */
typedef unsigned int UINT;

/* 19 */
typedef unsigned __int16 wchar_t;

/* 18 */
typedef wchar_t WCHAR;

/* 49 */
typedef unsigned int UINT_PTR;

/* 119 */
typedef UINT_PTR WPARAM;

/* 73 */
struct _SYSTEMTIME
{
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
};

/* 88 */
typedef unsigned int size_t;

/* 126 */
typedef struct HWND__ *HWND;

/* 128 */
struct tagMSG
{
  HWND hwnd;
  UINT message;
  WPARAM wParam;
  LPARAM lParam;
  DWORD time;
  POINT pt;
};

/* 514 */
struct tagPALETTEENTRY
{
  BYTE peRed;
  BYTE peGreen;
  BYTE peBlue;
  BYTE peFlags;
};

/* 64 */
struct tagRECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
};

/* 222 */
#pragma pack(push, 8)
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};
#pragma pack(pop)

