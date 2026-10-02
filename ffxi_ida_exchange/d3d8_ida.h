// generated from MinGW-w64 d3d8.h / d3d8types.h
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
enum D3DDEVTYPE {
  D3DDEVTYPE_HAL = 1,
  D3DDEVTYPE_REF = 2,
  D3DDEVTYPE_SW = 3
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
struct IDirect3D8;
struct IDirect3DVolume8;
struct IDirect3DSwapChain8;
struct IDirect3DSurface8;
struct IDirect3DResource8;
struct IDirect3DVertexBuffer8;
struct IDirect3DIndexBuffer8;
struct IDirect3DBaseTexture8;
struct IDirect3DCubeTexture8;
struct IDirect3DTexture8;
struct IDirect3DVolumeTexture8;
struct IDirect3DDevice8;
struct IDirect3D8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3D8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3D8 *This);
  int (__stdcall *Release)(IDirect3D8 *This);
  int (__stdcall *RegisterSoftwareDevice)(IDirect3D8 *This, void * pInitializeFunction);
  int (__stdcall *GetAdapterCount)(IDirect3D8 *This);
  int (__stdcall *GetAdapterIdentifier)(IDirect3D8 *This, unsigned int Adapter, unsigned int Flags, void * pIdentifier);
  int (__stdcall *GetAdapterModeCount)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *EnumAdapterModes)(IDirect3D8 *This, unsigned int Adapter, unsigned int Mode, void * pMode);
  int (__stdcall *GetAdapterDisplayMode)(IDirect3D8 *This, unsigned int Adapter, void * pMode);
  int (__stdcall *CheckDeviceType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE CheckType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, unsigned int Windowed);
  int (__stdcall *CheckDeviceFormat)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, unsigned int Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
  int (__stdcall *CheckDeviceMultiSampleType)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, unsigned int Windowed, D3DMULTISAMPLE_TYPE MultiSampleType);
  int (__stdcall *CheckDepthStencilMatch)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
  int (__stdcall *GetDeviceCaps)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, void * pCaps);
  int (__stdcall *GetAdapterMonitor)(IDirect3D8 *This, unsigned int Adapter);
  int (__stdcall *CreateDevice)(IDirect3D8 *This, unsigned int Adapter, D3DDEVTYPE DeviceType, unsigned int hFocusWindow, unsigned int BehaviorFlags, void * pPresentationParameters, IDirect3DDevice8 ** ppReturnedDeviceInterface);
};
struct IDirect3D8 { struct IDirect3D8Vtbl *lpVtbl; };
struct IDirect3DVolume8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVolume8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolume8 *This);
  int (__stdcall *Release)(IDirect3DVolume8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolume8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolume8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolume8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DVolume8 *This, unsigned int riid, void ** ppContainer);
  int (__stdcall *GetDesc)(IDirect3DVolume8 *This, void * pDesc);
  int (__stdcall *LockBox)(IDirect3DVolume8 *This, void * locked_box, void * box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolume8 *This);
};
struct IDirect3DVolume8 { struct IDirect3DVolume8Vtbl *lpVtbl; };
struct IDirect3DSwapChain8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DSwapChain8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DSwapChain8 *This);
  int (__stdcall *Release)(IDirect3DSwapChain8 *This);
  int (__stdcall *Present)(IDirect3DSwapChain8 *This, void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DSwapChain8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer);
};
struct IDirect3DSwapChain8 { struct IDirect3DSwapChain8Vtbl *lpVtbl; };
struct IDirect3DSurface8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DSurface8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DSurface8 *This);
  int (__stdcall *Release)(IDirect3DSurface8 *This);
  int (__stdcall *GetDevice)(IDirect3DSurface8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DSurface8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DSurface8 *This, unsigned int refguid);
  int (__stdcall *GetContainer)(IDirect3DSurface8 *This, unsigned int riid, void ** ppContainer);
  int (__stdcall *GetDesc)(IDirect3DSurface8 *This, void * pDesc);
  int (__stdcall *LockRect)(IDirect3DSurface8 *This, void * locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DSurface8 *This);
};
struct IDirect3DSurface8 { struct IDirect3DSurface8Vtbl *lpVtbl; };
struct IDirect3DResource8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DResource8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DResource8 *This);
  int (__stdcall *Release)(IDirect3DResource8 *This);
  int (__stdcall *GetDevice)(IDirect3DResource8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DResource8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DResource8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DResource8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DResource8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DResource8 *This);
  int (__stdcall *PreLoad)(IDirect3DResource8 *This);
  int (__stdcall *GetType)(IDirect3DResource8 *This);
};
struct IDirect3DResource8 { struct IDirect3DResource8Vtbl *lpVtbl; };
struct IDirect3DVertexBuffer8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVertexBuffer8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DVertexBuffer8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVertexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVertexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DVertexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DVertexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DVertexBuffer8 *This, void * pDesc);
};
struct IDirect3DVertexBuffer8 { struct IDirect3DVertexBuffer8Vtbl *lpVtbl; };
struct IDirect3DIndexBuffer8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DIndexBuffer8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Release)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDevice)(IDirect3DIndexBuffer8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DIndexBuffer8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DIndexBuffer8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *PreLoad)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetType)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *Lock)(IDirect3DIndexBuffer8 *This, unsigned int OffsetToLock, unsigned int SizeToLock, void ** ppbData, unsigned int Flags);
  int (__stdcall *Unlock)(IDirect3DIndexBuffer8 *This);
  int (__stdcall *GetDesc)(IDirect3DIndexBuffer8 *This, void * pDesc);
};
struct IDirect3DIndexBuffer8 { struct IDirect3DIndexBuffer8Vtbl *lpVtbl; };
struct IDirect3DBaseTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DBaseTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DBaseTexture8 *This);
  int (__stdcall *Release)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DBaseTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DBaseTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DBaseTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DBaseTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetType)(IDirect3DBaseTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DBaseTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DBaseTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DBaseTexture8 *This);
};
struct IDirect3DBaseTexture8 { struct IDirect3DBaseTexture8Vtbl *lpVtbl; };
struct IDirect3DCubeTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DCubeTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DCubeTexture8 *This);
  int (__stdcall *Release)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DCubeTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DCubeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DCubeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DCubeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DCubeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DCubeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DCubeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DCubeTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetCubeMapSurface)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level, IDirect3DSurface8 ** ppCubeMapSurface);
  int (__stdcall *LockRect)(IDirect3DCubeTexture8 *This, unsigned int face, unsigned int level, void * locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DCubeTexture8 *This, unsigned int FaceType, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DCubeTexture8 *This, unsigned int face, void * dirty_rect);
};
struct IDirect3DCubeTexture8 { struct IDirect3DCubeTexture8Vtbl *lpVtbl; };
struct IDirect3DTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DTexture8 *This);
  int (__stdcall *Release)(IDirect3DTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DTexture8 *This);
  int (__stdcall *GetType)(IDirect3DTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetSurfaceLevel)(IDirect3DTexture8 *This, unsigned int Level, IDirect3DSurface8 ** ppSurfaceLevel);
  int (__stdcall *LockRect)(IDirect3DTexture8 *This, unsigned int level, void * locked_rect, void * rect, unsigned int flags);
  int (__stdcall *UnlockRect)(IDirect3DTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyRect)(IDirect3DTexture8 *This, void * dirty_rect);
};
struct IDirect3DTexture8 { struct IDirect3DTexture8Vtbl *lpVtbl; };
struct IDirect3DVolumeTexture8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DVolumeTexture8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *Release)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetDevice)(IDirect3DVolumeTexture8 *This, IDirect3DDevice8 ** ppDevice);
  int (__stdcall *SetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void * data, unsigned int data_size, unsigned int flags);
  int (__stdcall *GetPrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid, void * pData, void * pSizeOfData);
  int (__stdcall *FreePrivateData)(IDirect3DVolumeTexture8 *This, unsigned int refguid);
  int (__stdcall *SetPriority)(IDirect3DVolumeTexture8 *This, unsigned int PriorityNew);
  int (__stdcall *GetPriority)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *PreLoad)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetType)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *SetLOD)(IDirect3DVolumeTexture8 *This, unsigned int LODNew);
  int (__stdcall *GetLOD)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelCount)(IDirect3DVolumeTexture8 *This);
  int (__stdcall *GetLevelDesc)(IDirect3DVolumeTexture8 *This, unsigned int Level, void * pDesc);
  int (__stdcall *GetVolumeLevel)(IDirect3DVolumeTexture8 *This, unsigned int Level, IDirect3DVolume8 ** ppVolumeLevel);
  int (__stdcall *LockBox)(IDirect3DVolumeTexture8 *This, unsigned int level, void * locked_box, void * box, unsigned int flags);
  int (__stdcall *UnlockBox)(IDirect3DVolumeTexture8 *This, unsigned int Level);
  int (__stdcall *AddDirtyBox)(IDirect3DVolumeTexture8 *This, void * dirty_box);
};
struct IDirect3DVolumeTexture8 { struct IDirect3DVolumeTexture8Vtbl *lpVtbl; };
struct IDirect3DDevice8Vtbl {
  int (__stdcall *QueryInterface)(IDirect3DDevice8 *This, unsigned int riid, void ** ppvObject);
  int (__stdcall *AddRef)(IDirect3DDevice8 *This);
  int (__stdcall *Release)(IDirect3DDevice8 *This);
  int (__stdcall *TestCooperativeLevel)(IDirect3DDevice8 *This);
  int (__stdcall *GetAvailableTextureMem)(IDirect3DDevice8 *This);
  int (__stdcall *ResourceManagerDiscardBytes)(IDirect3DDevice8 *This, unsigned int Bytes);
  int (__stdcall *GetDirect3D)(IDirect3DDevice8 *This, IDirect3D8 ** ppD3D8);
  int (__stdcall *GetDeviceCaps)(IDirect3DDevice8 *This, void * pCaps);
  int (__stdcall *GetDisplayMode)(IDirect3DDevice8 *This, void * pMode);
  int (__stdcall *GetCreationParameters)(IDirect3DDevice8 *This, void * pParameters);
  int (__stdcall *SetCursorProperties)(IDirect3DDevice8 *This, unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface8 * pCursorBitmap);
  int (__stdcall *SetCursorPosition)(IDirect3DDevice8 *This, unsigned int XScreenSpace, unsigned int YScreenSpace, unsigned int Flags);
  int (__stdcall *ShowCursor)(IDirect3DDevice8 *This, unsigned int bShow);
  int (__stdcall *CreateAdditionalSwapChain)(IDirect3DDevice8 *This, void * pPresentationParameters, IDirect3DSwapChain8 ** pSwapChain);
  int (__stdcall *Reset)(IDirect3DDevice8 *This, void * pPresentationParameters);
  int (__stdcall *Present)(IDirect3DDevice8 *This, void * src_rect, void * dst_rect, unsigned int dst_window_override, void * dirty_region);
  int (__stdcall *GetBackBuffer)(IDirect3DDevice8 *This, unsigned int BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8 ** ppBackBuffer);
  int (__stdcall *GetRasterStatus)(IDirect3DDevice8 *This, void * pRasterStatus);
  int (__stdcall *SetGammaRamp)(IDirect3DDevice8 *This, unsigned int flags, void * ramp);
  int (__stdcall *GetGammaRamp)(IDirect3DDevice8 *This, void * pRamp);
  int (__stdcall *CreateTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8 ** ppTexture);
  int (__stdcall *CreateVolumeTexture)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8 ** ppVolumeTexture);
  int (__stdcall *CreateCubeTexture)(IDirect3DDevice8 *This, unsigned int EdgeLength, unsigned int Levels, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8 ** ppCubeTexture);
  int (__stdcall *CreateVertexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, unsigned int FVF, D3DPOOL Pool, IDirect3DVertexBuffer8 ** ppVertexBuffer);
  int (__stdcall *CreateIndexBuffer)(IDirect3DDevice8 *This, unsigned int Length, unsigned int Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8 ** ppIndexBuffer);
  int (__stdcall *CreateRenderTarget)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, unsigned int Lockable, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CreateDepthStencilSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CreateImageSurface)(IDirect3DDevice8 *This, unsigned int Width, unsigned int Height, D3DFORMAT Format, IDirect3DSurface8 ** ppSurface);
  int (__stdcall *CopyRects)(IDirect3DDevice8 *This, IDirect3DSurface8 * src_surface, void * src_rects, unsigned int rect_count, IDirect3DSurface8 * dst_surface, void * dst_points);
  int (__stdcall *UpdateTexture)(IDirect3DDevice8 *This, IDirect3DBaseTexture8 * pSourceTexture, IDirect3DBaseTexture8 * pDestinationTexture);
  int (__stdcall *GetFrontBuffer)(IDirect3DDevice8 *This, IDirect3DSurface8 * pDestSurface);
  int (__stdcall *SetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 * pRenderTarget, IDirect3DSurface8 * pNewZStencil);
  int (__stdcall *GetRenderTarget)(IDirect3DDevice8 *This, IDirect3DSurface8 ** ppRenderTarget);
  int (__stdcall *GetDepthStencilSurface)(IDirect3DDevice8 *This, IDirect3DSurface8 ** ppZStencilSurface);
  int (__stdcall *BeginScene)(IDirect3DDevice8 *This);
  int (__stdcall *EndScene)(IDirect3DDevice8 *This);
  int (__stdcall *Clear)(IDirect3DDevice8 *This, unsigned int rect_count, void * rects, unsigned int flags, unsigned int color, float z, unsigned int stencil);
  int (__stdcall *SetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void * matrix);
  int (__stdcall *GetTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE State, void * pMatrix);
  int (__stdcall *MultiplyTransform)(IDirect3DDevice8 *This, D3DTRANSFORMSTATETYPE state, void * matrix);
  int (__stdcall *SetViewport)(IDirect3DDevice8 *This, void * viewport);
  int (__stdcall *GetViewport)(IDirect3DDevice8 *This, void * pViewport);
  int (__stdcall *SetMaterial)(IDirect3DDevice8 *This, void * material);
  int (__stdcall *GetMaterial)(IDirect3DDevice8 *This, void * pMaterial);
  int (__stdcall *SetLight)(IDirect3DDevice8 *This, unsigned int index, void * light);
  int (__stdcall *GetLight)(IDirect3DDevice8 *This, unsigned int Index, void * pLight);
  int (__stdcall *LightEnable)(IDirect3DDevice8 *This, unsigned int Index, unsigned int Enable);
  int (__stdcall *GetLightEnable)(IDirect3DDevice8 *This, unsigned int Index, void * pEnable);
  int (__stdcall *SetClipPlane)(IDirect3DDevice8 *This, unsigned int index, void * plane);
  int (__stdcall *GetClipPlane)(IDirect3DDevice8 *This, unsigned int Index, void * pPlane);
  int (__stdcall *SetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, unsigned int Value);
  int (__stdcall *GetRenderState)(IDirect3DDevice8 *This, D3DRENDERSTATETYPE State, void * pValue);
  int (__stdcall *BeginStateBlock)(IDirect3DDevice8 *This);
  int (__stdcall *EndStateBlock)(IDirect3DDevice8 *This, void * pToken);
  int (__stdcall *ApplyStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CaptureStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *DeleteStateBlock)(IDirect3DDevice8 *This, unsigned int Token);
  int (__stdcall *CreateStateBlock)(IDirect3DDevice8 *This, D3DSTATEBLOCKTYPE Type, void * pToken);
  int (__stdcall *SetClipStatus)(IDirect3DDevice8 *This, void * clip_status);
  int (__stdcall *GetClipStatus)(IDirect3DDevice8 *This, void * pClipStatus);
  int (__stdcall *GetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 ** ppTexture);
  int (__stdcall *SetTexture)(IDirect3DDevice8 *This, unsigned int Stage, IDirect3DBaseTexture8 * pTexture);
  int (__stdcall *GetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, void * pValue);
  int (__stdcall *SetTextureStageState)(IDirect3DDevice8 *This, unsigned int Stage, D3DTEXTURESTAGESTATETYPE Type, unsigned int Value);
  int (__stdcall *ValidateDevice)(IDirect3DDevice8 *This, void * pNumPasses);
  int (__stdcall *GetInfo)(IDirect3DDevice8 *This, unsigned int DevInfoID, void * pDevInfoStruct, unsigned int DevInfoStructSize);
  int (__stdcall *SetPaletteEntries)(IDirect3DDevice8 *This, unsigned int palette_idx, void * entries);
  int (__stdcall *GetPaletteEntries)(IDirect3DDevice8 *This, unsigned int PaletteNumber, void * pEntries);
  int (__stdcall *SetCurrentTexturePalette)(IDirect3DDevice8 *This, unsigned int PaletteNumber);
  int (__stdcall *GetCurrentTexturePalette)(IDirect3DDevice8 *This, void * PaletteNumber);
  int (__stdcall *DrawPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount);
  int (__stdcall *DrawIndexedPrimitive)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE PrimitiveType, unsigned int minIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount);
  int (__stdcall *DrawPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int primitive_count, void * data, unsigned int stride);
  int (__stdcall *DrawIndexedPrimitiveUP)(IDirect3DDevice8 *This, D3DPRIMITIVETYPE primitive_type, unsigned int min_vertex_idx, unsigned int vertex_count, unsigned int primitive_count, void * index_data, D3DFORMAT index_format, void * data, unsigned int stride);
  int (__stdcall *ProcessVertices)(IDirect3DDevice8 *This, unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer8 * pDestBuffer, unsigned int Flags);
  int (__stdcall *CreateVertexShader)(IDirect3DDevice8 *This, void * declaration, void * byte_code, void * shader, unsigned int usage);
  int (__stdcall *SetVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetVertexShader)(IDirect3DDevice8 *This, void * pHandle);
  int (__stdcall *DeleteVertexShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void * data, unsigned int count);
  int (__stdcall *GetVertexShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void * pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetVertexShaderDeclaration)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *GetVertexShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *SetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 * pStreamData, unsigned int Stride);
  int (__stdcall *GetStreamSource)(IDirect3DDevice8 *This, unsigned int StreamNumber, IDirect3DVertexBuffer8 ** ppStreamData, void * pStride);
  int (__stdcall *SetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 * pIndexData, unsigned int BaseVertexIndex);
  int (__stdcall *GetIndices)(IDirect3DDevice8 *This, IDirect3DIndexBuffer8 ** ppIndexData, void * pBaseVertexIndex);
  int (__stdcall *CreatePixelShader)(IDirect3DDevice8 *This, void * byte_code, void * shader);
  int (__stdcall *SetPixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *GetPixelShader)(IDirect3DDevice8 *This, void * pHandle);
  int (__stdcall *DeletePixelShader)(IDirect3DDevice8 *This, unsigned int Handle);
  int (__stdcall *SetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int reg_idx, void * data, unsigned int count);
  int (__stdcall *GetPixelShaderConstant)(IDirect3DDevice8 *This, unsigned int Register, void * pConstantData, unsigned int ConstantCount);
  int (__stdcall *GetPixelShaderFunction)(IDirect3DDevice8 *This, unsigned int Handle, void * pData, void * pSizeOfData);
  int (__stdcall *DrawRectPatch)(IDirect3DDevice8 *This, unsigned int handle, void * segment_count, void * patch_info);
  int (__stdcall *DrawTriPatch)(IDirect3DDevice8 *This, unsigned int handle, void * segment_count, void * patch_info);
  int (__stdcall *DeletePatch)(IDirect3DDevice8 *This, unsigned int Handle);
};
struct IDirect3DDevice8 { struct IDirect3DDevice8Vtbl *lpVtbl; };
