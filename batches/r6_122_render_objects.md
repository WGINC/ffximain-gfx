# r6_122 -- Types for the render objects: CDx, model, skin, skeleton (task #122, plan step 25)

Machine form: `data/names/r6_122_render_objects.json`. Nothing in `tree/`, `data/` (other than that file) or
`pipeline/` was changed.

## Which classes, and how they were identified

Every class below is named by the game's own **runtime-class descriptor** (slot 0 of the vtable is
`mov eax, DESC; ret`; DESC = {name string, object size, parent descriptor}). The size quoted is that descriptor's,
unless stated.

| Asked for | Class | How |
|---|---|---|
| CDx | `CDx` (0xA50) | descriptor 0x103299C8 "CDx"; the object in g_pCDx 0x1045666C |
| model | `CYyModel` (0xBC), `CYyModelBase` (0xC8), `CYyModelDt` (0x3C) and its two derived vtables 0x1032A430 / 0x1032A72C | descriptors 0x1032A6D0, 0x1032A6C4, 0x1032A6B8; all three are direct children of CYyObject (parent 0x103515A0), not of each other |
| skeleton | `CYySkl` (0x18) | descriptor 0x1032A87C "CYySkl"; CYyModelBase constructs one at its +0x8 (0x10029D3E) and it holds the bone matrices (+0xC) |
| skin | no class of its own. The CPU skinning (`0x1002F1D0`, tree `Model_SkinVerticesCpu`) and `0x1002DC10` (tree `CYyModelDt::SkinAndDrawV2`) run on the object reached through `*(*(CYyModelDt+0x8))`; its class is **not known**. Listed as `Obj_SkinAndDrawV2_this` with the fields the code shows. |

Left out on purpose: `CXiSkeletonActor` and `CXiSkeletonActorRes` (actor objects: plan step 25 lists actors
separately), `CMoSkeletonElem` (a CMoElem resource element, descriptor parent CMoElem 0x1032AFF0), `CYyOs2VtxBuffer`,
`CYyVb` (vertex buffers: not asked for). `CAcc`/`CAcc2` appear only as CDx members (their fields are given at the
CDx offsets where CDx's code fills them).

## Method (repeatable; scripts kept outside the repo)

1. **Object identity from the code, not from names.** A function's ECX is taken to be the object only where the code
   shows it: (a) constructors/destructors that store the class's vtable at `[this]`; (b) `data/this_class.json`
   (vtable-settled); (c) for CDx, every register loaded from `[0x1045666C]` (g_pCDx) anywhere in the image; (d) a
   function every one of whose direct calls passes the object in ECX and whose address appears nowhere in the image
   as a dword (so no vtable or pointer call reaches it), grown to a fixed point. The tree's placement by IDB name was
   **not** used as identity; where an SDK-typed use exists only in a function placed by IDB name, the field is at
   most **medium** and says so.
2. **Register dataflow per function** (capstone over `data/func_extents.json`, direct jumps; a call clobbers
   EAX/ECX/EDX and keeps EBX/ESI/EDI/EBP -- the MS x86 convention; joins keep a register only where all predecessors
   agree). Each `[obj+disp]` access is recorded with its width, read/write, x87 use, jcc, and where the loaded value
   goes (COM slot called on it, argument N of a call, ECX of a call).
3. **Types only from:** a D3D8/D3DX8/Win32 prototype the value reaches (d3d8.h, d3d8caps.h, d3d8types.h, d3dx8tex.h,
   winuser.h; COM slots from `data/com_spec.json`); x87 single-precision operands (`fld/fst dword`) for float; a
   descriptor's size for an embedded object; the D3D8 structure sizes/offsets used were **compiled** from the
   mingw-w64 headers (`sizeof(D3DCAPS8)=0xD4`, `sizeof(D3DVIEWPORT8)=24`, `sizeof(D3DMATRIX)=64`, the D3DCAPS8 member
   offsets quoted). A field only stored to (constants, zero) and never read in a way that types it is **unknown**,
   with its width. Bit patterns such as 0x3F800000 are reported as values, not as a float type.
4. Array counts bounded only by the next member's offset are marked "bounded by +X; no bound check found".

CDx `this` functions proven by step 1 (45): 0x10008F80 0x10008F90 0x10009100 0x10009120 0x100091B0 0x10009620
0x10009640 0x100096B0 0x10009710 0x10009720 0x10009730 0x10009740 0x10009750 0x10009770 0x10009780 0x100097F0
0x10009870 0x100098E0 0x10009920 0x10009D20 0x10009DC0 0x1000A1D0 0x1000A390 0x1000A3B0 0x1000A4F0 0x1000A550
0x1000A5A0 0x1000A6E0 0x1000A820 0x1000AAB0 0x1000ABE0 0x1000AD70 0x1000AE40 0x1000BB20 0x1000BB90 0x1000BD50
0x1000BDB0 0x1000C0B0 0x1000C6E0 0x1000C7C0 0x1000C850 0x1000C900 0x1000CC00 0x1000CD00 0x100191D0; plus g_pCDx
loads in 772 functions. Not proven (callers pass ECX the scan cannot follow, e.g. `mov ecx, [eax+0xdc8]` at
0x10043FF6): 0x10009E20 0x10009EA0 0x10009EE0 0x10009F20 0x1000A200 0x1000A230 0x1000A290 0x1000A2B0 0x1000A800
0x1000BF30 0x1000CB20 0x1000CF20 0x1000CF90 (tree places them in CDx by IDB name).

## Schema

No `data/names/r*_*.json` file holds fields (they are `{function address: {name, confidence, evidence, summary}}`).
The closest existing schema is `data/field_names.json` (`{class: {offset: {name, confidence, summary, evidence}}}`),
so the JSON uses it, adding per field `width`, `type` (null = not known), `status` (`typed`/`unknown`),
`name_confidence`, and optional `count` / `member_of`; class facts are under top-level `_classes`. **Note for the
integrator:** `hx2clean.py` and `community_reconcile.py` read `field_names.json` as `{class: {hex offset: ...}}`, so
the `_doc`/`_classes` keys must be skipped (or dropped) if this file is merged into it.

## Fields

Confidence: **high** = object identity proven from code (step 1) and the type fixed by a prototype/descriptor/x87
operand; **medium** = one of the two rests on a weaker link stated in the row; **low** = width-only or one store.
"unknown" rows give the width the code shows (or the gap size where no access was found).

### CDx

- Size: 0xA50 (runtime-class descriptor 0x103299C8 (returned by slot 0 0x10008F80 `mov eax, 0x103299c8`): name "CDx", size 0xA50, parent 0x103515A0 = CYyObject (size 4); the one allocation 0x100161C8 `push 0xa50` before the constructor call 0x100161EF)
- Vtable: 0x103299D4
- Identified by: Its own runtime-class descriptor names it. The object is the one in g_pCDx (0x1045666C): the only non-null write, 0x100161F6, stores the result of the constructor 0x10008F90 (which stores vtable 0x103299D4 at 0x100090C3) run on the 0xA50-byte allocation; the two other writes store 0 (0x100162B8, 0x10016376). Accesses counted: every [reg+disp] whose register was loaded from [0x1045666C] (772 functions), plus `this` in 45 functions whose every direct call passes that object in ECX and whose address appears nowhere in the image (list in the .md).

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x0 | 0x4 | void ** (vtable 0x103299D4) | vftable | high | 0x100090C3 `mov dword ptr [esi], 0x103299d4` in the constructor (0x10008F9B first stores CYyObject vtable 0x103299F0) |
| +0x4 | 0x4 | **unknown** | -- | unknown | no access found |
| +0x8 | 0x4 | IDirect3D8 * | pD3D | high | constructor 0x10008FDC stores arg1; its only caller passes [[0x103D3CBC]+4] (0x100161E8/0x100161EC); that holder field is written at 0x10003204 with the return of Direct3DCreate8 (call 0x102D2600 = jmp [0x10329498], the d3d8.dll import; d3d8.h: IDirect3D8* WINAPI Direct3DCreate8(UINT)). Uses agree: 0x1000935E calls IDirect3D8 slot 0x20 (GetAdapterDisplayMode). One write to +0x8 found (the constructor). |
| +0xC | 0x4 | IDirect3DDevice8 * | pDevice | high | constructor 0x10008FE1 stores arg2 = [[0x103D3CBC]+8]; that holder field is the ppReturnedDeviceInterface of IDirect3D8::CreateDevice (slot 0x3C) at 0x1000356E and 0x1000358A (`add esi, 8; push esi`, object [holder+4]) in 0x100034B0, reached from the holder's own methods (0x10003110 -> 0x100031F0 -> 0x100033C0 -> 0x100034B0, each passing its this). 1547 accesses, the only write is 0x10008FE1. This closes the value stored at construction; that nothing outside the scanned paths overwrites +0xC is not shown (com_fields.json device_via_CDx_this states the same limit). |
| +0x10 | 0x4 | IDirect3DTexture8 * [8 (bounded by +0x30; no bound check found)] | -- | medium | 0x1000A031 `mov [esi+edx*4+0x10], edi` (edi = arg1 of 0x10009F20); 0x1000A2DB calls slot 0x3C on it with (0, &s) and passes s as pRenderTarget to IDirect3DDevice8::SetRenderTarget (0x1000A2F6, d3d8.h: IDirect3DSurface8*), i.e. IDirect3DTexture8::GetSurfaceLevel; callers pass +0x194 etc., which hold CreateTexture results. Index = byte +0x158. Identity of 0x10009F20/0x1000A2B0 is the tree placement only (not in the proven set). |
| +0x30 | 0x4 | IDirect3DSurface8 * [8 (bounded by +0x50)] | -- | medium | 0x1000A305-0x1000A31A pushes [esi+eax*4+0x30] as pRenderTarget of SetRenderTarget (slot 0x7C); 0x1000A042 stores 0 there in 0x10009F20. Identity: tree placement only. |
| +0x50 | 0x4 | **unknown** [8 (bounded by +0x70)] | -- | unknown | 0x1000A052 stores arg2 of 0x10009F20 at [esi+ecx*4+0x50]; no read found |
| +0x70 | 0x4 | IDirect3DSurface8 * [8 (bounded by +0x90)] | -- | medium | pNewZStencil of SetRenderTarget at 0x1000A2EB and 0x1000A315; written at 0x1000A05C from arg3 of 0x10009F20, which 0x10009F52 passes as pNewZStencil itself; the proven 0x1001D240 reads it at 0x1001D2DD. Identity of the SDK-typed uses: tree placement. |
| +0x90 | 0x4 | IDirect3DSurface8 * | defaultBackBuffer (low) | medium | pRenderTarget of SetRenderTarget at 0x1000A32A/0x1000A334 when the stack empties (0x1000A2B0, tree placement); proven 0x1000A6F7 calls its slot 0x20 with one out pointer (IDirect3DSurface8::GetDesc). Written by 0x10009628 (arg2 of 0x10009620). |
| +0x94 | 0x4 | IDirect3DSurface8 * | defaultDepthSurface (medium) | medium | pNewZStencil of SetRenderTarget at 0x1000A320/0x1000A334; proven 0x1000AD84 calls its slot 0x20 (GetDesc); written by 0x1000962E (arg3 of 0x10009620). |
| +0x98 | 0x18 | D3DVIEWPORT8 [8 (0x98..0x157, bounded by +0x158; no bound check found)] | -- | high | proven 0x10009DC0 copies 6 dwords into [this+n*24+0x98] (n = byte +0x158) and passes that address to IDirect3DDevice8::SetViewport (slot 0xA0, CONST D3DVIEWPORT8*) at 0x10009DEA; 0x10009D20 writes X,Y,Width,Height,MinZ (=0),MaxZ (=0x3F800000) of entry n and passes it to SetViewport at 0x10009DB7. sizeof(D3DVIEWPORT8)=24 (mingw d3d8types.h, compiled). |
| +0x158 | 0x1 | uint8_t | renderTargetDepth (medium) | high | byte loads after `xor eax, eax` (zero-extended: 0x10009D22/0x10009D24), used as index *24 and *4; incremented in 0x10009F37, decremented in 0x1000A2BA; constructor stores 0 at 0x10008FE9 |
| +0x159 | 0x3 | **unknown** | -- | unknown | no access found |
| +0x15C | 0x4 | DWORD | -- | high | proven 0x10009D47 loads it and stores it into D3DVIEWPORT8.Width of entry n (0x10009D58, offset +8 of the entry) passed to SetViewport; written at 0x10016207 from global 0x10456A58; 0x100114D3 reads its low 16 bits |
| +0x160 | 0x4 | DWORD | -- | high | proven 0x10009D5F loads it into D3DVIEWPORT8.Height of entry n (0x10009D70); written at 0x10016218 from global 0x10456C08 |
| +0x164 | 0x4 | **unknown** | clearFlags (low) | unknown | written only by 0x10009648 (arg1 of 0x10009640); no read found, so no SDK type reaches it. The one caller found passes 3 (0x1001157A). |
| +0x168 | 0x4 | **unknown** | clearColor (low) | unknown | written by 0x10009652 (arg2 of 0x10009640) and by 0x10009670 (tree: SetClearColor); no read found. Caller 0x10011575 passes 0x80000000. |
| +0x16C | 0x4 | **unknown** | clearZ (low) | unknown | written by 0x1000965C (arg3); no read found. Caller 0x10011570 passes 0x3F800000. |
| +0x170 | 0x4 | **unknown** | clearStencil (low) | unknown | written by 0x10009662 (arg4); no read found. Caller 0x1001156F passes 0. |
| +0x174 | 0x4 | float | -- | high | x87 single-precision operands: 0x1000AE6C `fsubr dword ptr [ecx+0x174]`, 0x1000AE72 `fst dword ptr [ecx+0x174]` (proven 0x1000AE40); constructor stores 0x43000000 (128.0f) |
| +0x178 | 0x4 | **unknown** | -- | unknown | only writes found: 0x43000000 (bit pattern of 128.0f) at 0x100090CF, 0x1003DAAE, 0x1003DD21; no read |
| +0x17C | 0x4 | **unknown** [4 (0x17C..0x18B, each written 0x43000000 at 0x100090D5-0x100090E1)] | -- | unknown | only write found: constructor 0x100090D5 (0x43000000) |
| +0x18C | 0x4 | **unknown** | -- | unknown | only write found: constructor 0x100090ED `mov dword ptr [esi+0x18c], 0x80808080` |
| +0x190 | 0x1C | CAcc2 (embedded) | -- | high | constructor 0x10008F95 `lea ecx, [esi+0x190]; call 0x1000D040`, which stores vtable 0x10329A58 = CAcc2 (descriptor 0x103299BC: "CAcc2", size 0x1C) |
| +0x194 | 0x4 | IDirect3DTexture8 * [2 (+0x194, +0x198; read as [ecx+esi*4+0x194] at 0x10013796)] -- in CAcc2 at +0x190 (its +0x4) | -- | high | written at 0x100115E9 with the return of 0x1000A6E0 (proven), which returns the ppTexture out value of IDirect3DDevice8::CreateTexture (slot 0x50, 0x1000A737..0x1000A7B9) or 0 (0x1000A7CA); 0x10017CD8 calls its slot 0x3C |
| +0x198 | 0x4 | IDirect3DTexture8 * -- in CAcc2 at +0x190 (its +0x8) | -- | high | written at 0x1001178C with the return of 0x1000A6E0 (edi, 0x10011763) |
| +0x19C | 0x4 | **unknown** [2 (+0x19C, +0x1A0)] -- in CAcc2 at +0x190 (its +0xC) | -- | unknown | written 0x100115F9 from a local zeroed before the call; passed as arg2 of 0x10009E20 (-> +0x50[n]); no typed use found |
| +0x1A4 | 0x4 | IDirect3DSurface8 * -- in CAcc2 at +0x190 (its +0x14) | -- | high | written at 0x10011605 with the return of 0x1000AD70 (proven), which returns the ppSurface out value of IDirect3DDevice8::CreateDepthStencilSurface (slot 0x68, 0x1000ADB2) or 0 |
| +0x1A8 | 0x4 | IDirect3DSurface8 * -- in CAcc2 at +0x190 (its +0x18) | -- | high | written at 0x10011798 with the return of 0x1000AD70 |
| +0x1AC | 0x10 | CAcc (embedded) [3] | -- | high | constructor loop 0x10008FA6-0x10008FBC: ebx=3, `mov ecx, edi; call 0x1000CFB0; add edi, 0x10`; 0x1000CFB0 stores vtable 0x10329A3C = CAcc (descriptor 0x103299B0: "CAcc", size 0x10) |
| +0x1C0 | 0x4 | IDirect3DTexture8 * -- in CAcc[1] at +0x1BC (its +0x4) | -- | high | written at 0x10011859 with the return of 0x1000A6E0 (0x10011844) |
| +0x1C4 | 0x4 | **unknown** -- in CAcc[1] at +0x1BC (its +0x8) | -- | unknown | written 0x10011868 from a stack value; passed as arg2 of 0x10009E20; no typed use |
| +0x1C8 | 0x4 | IDirect3DSurface8 * -- in CAcc[1] at +0x1BC (its +0xC) | -- | high | written at 0x1001187F with the return of 0x1000AD70 (0x10011874) |
| +0x1DC | 0x10 | CAcc (embedded) | -- | high | constructor 0x10008FBE `lea ecx, [esi+0x1dc]; call 0x1000CFB0` |
| +0x1E0 | 0x4 | IDirect3DTexture8 * -- in CAcc at +0x1DC (its +0x4) | -- | high | written at 0x100116C1 with the return of 0x1000A6E0 (0x100116B6) |
| +0x1E4 | 0x4 | **unknown** -- in CAcc at +0x1DC (its +0x8) | -- | unknown | written 0x100116D1; passed as arg2 of 0x10009E20; no typed use |
| +0x1E8 | 0x4 | IDirect3DSurface8 * -- in CAcc at +0x1DC (its +0xC) | -- | high | written at 0x100116F2 with the return of 0x1000AD70 (0x100116E7) |
| +0x1EC | 0x4 | **unknown** | -- | unknown | zeroed by the constructor (0x10009081); its address is passed as arg2 of 0x10009870 (0x10009AC3) and 0x100098E0 (0x10009CFB); what those store there was not read |
| +0x1F0 | 0x4 | **unknown** | -- | unknown | zeroed by the constructor (0x10009087); address passed to 0x10009870 / 0x100098E0 (0x1000992E, 0x10009AB3) |
| +0x1F4 | 0x61C | embedded object, vtable 0x10329E88 (class name not known) | -- | high | constructor 0x10008FC9 `lea ecx, [esi+0x1f4]; call 0x10017030`, which stores vtable 0x10329E88 (slot 0 = 0x10017070, a scalar deleting destructor; no runtime-class slot) and writes its +0x604..+0x618. Extent 0x61C = up to +0x810 (layout bound; its last write ends at +0x61C) |
| +0x810 | 0x4 | 32-bit integer (signedness not shown) | stereoEye (medium) | medium | proven 0x1000CC00/0x1000CD00: `mov edi, [esi+0x810]`, stores 0 and 1, restores edi (0x1000CDB0); constructor stores 0 |
| +0x814 | 0x4 | IDirect3DTexture8 * [2 (+0x814, +0x818)] | -- | high | &+0x814 is arg1 of 0x1000C900 (0x1000C6FB); there it is the ppTexture of _D3DXCreateTexture@32 (0x102D2FB9; IDA library signature, d3dx8tex.h prototype ...LPDIRECT3DTEXTURE8* ppTexture) at 0x1000C935/0x1000C948, for 2 consecutive slots (ebp += 4, loop of 2 at 0x1000C9CA-0x1000C9D7); released through slot 8 |
| +0x81C | 0x4 | IDirect3DTexture8 * [2 (+0x81C, +0x820)] | -- | high | same, second call of 0x1000C900 with &+0x81C (0x1000C737) |
| +0x824 | 0x4 | IDirect3DTexture8 * | -- | high | written at 0x1000C798 with the return of 0x1000A6E0 (CreateTexture wrapper) |
| +0x828 | 0x1 | 8-bit integer (signedness not shown) | -- | medium | byte stores of 0/1 and byte loads only (0x10011C75, 0x10038CE9, 0x10039272) |
| +0x829 | 0x3 | **unknown** | -- | unknown | no access found |
| +0x82C | 0x4 | IDirect3DTexture8 * | stereoOddLineTexture (low) | high | &+0x82C is arg1 of 0x1000C850 (0x1000C76D), pushed as ppTexture of _D3DXCreateTexture@32 at 0x1000C86E/0x1000C87B |
| +0x830 | 0x4 | DWORD | -- | high | Initialize 0x100091B0: IDirect3DDevice8::GetDisplayMode (slot 0x20) into a local, then D3DDISPLAYMODE.Format - D3DFMT_A8R8G8B8 (0x15) switches: formats 0x15/0x16 store 0x20 (0x10009343), 0x17..0x19 (R5G6B5, X1R5G5B5, A1R5G5B5) store 0x10 (0x1000934B); i.e. bits per pixel of the display format (values from d3d8types.h) |
| +0x834 | 0xD4 | D3DCAPS8 | caps | high | Initialize 0x100091BF-0x100091C9: `lea esi, [ebp+0x834]` passed to IDirect3DDevice8::GetDeviceCaps (slot 0x1C, D3DCAPS8*); the code then reads caps members at their d3d8caps.h offsets (DevCaps +0x1C at 0x100091E2, VertexShaderVersion +0xC4, PixelShaderVersion +0xCC, MaxTextureWidth/Height +0x58/+0x5C). sizeof(D3DCAPS8)=0xD4 (compiled), ending exactly at +0x908. |
| +0x908 | 0x1 | 8-bit flag | -- | medium | set to 1 at 0x100091F2 when caps.DevCaps & 0x10000 (D3DDEVCAPS_HWTRANSFORMANDLIGHT) |
| +0x909 | 0x1 | 8-bit flag | -- | medium | set to 1 at 0x10009229 when caps.VertexShaderVersion >= 0xFFFE0101 (unsigned jb); compared at 0x101782E9 |
| +0x90A | 0x1 | 8-bit flag | -- | medium | set to 1 at 0x10009243 when caps.PixelShaderVersion >= 0xFFFF0101 |
| +0x90B | 0x1 | **unknown** | -- | unknown | no access found |
| +0x90C | 0x2 | 16-bit integer | -- | medium | word store of 0x101 at 0x10009230 (with +0x909) |
| +0x90E | 0x2 | 16-bit integer | -- | medium | word stores of 0x101 / 0x104 / 0x200 by pixel shader version (0x1000924A-0x10009272) |
| +0x910 | 0x1 | 8-bit flag | -- | medium | byte stores of 0/1 only |
| +0x911 | 0x1 | 8-bit flag | -- | medium | byte stores of 0/1 only |
| +0x912 | 0x1 | 8-bit flag | -- | medium | byte stores of 0/1, byte load |
| +0x913 | 0x1 | 8-bit flag | -- | medium | set at 0x10009287 when caps.PrimitiveMiscCaps & 0x800; 7 byte loads |
| +0x914 | 0x1 | 8-bit flag | -- | medium | set at 0x10009296 when caps.PrimitiveMiscCaps & 0x400 |
| +0x915 | 0x1 | 8-bit flag | -- | medium | set at 0x100092A9 when caps.Caps2 & 0x20000; compared with 0 (je) at 3 sites, e.g. 0x10011496 |
| +0x916 | 0x1 | 8-bit integer | -- | low | one byte store (constructor 0x10009033) |
| +0x917 | 0x1 | **unknown** | -- | unknown | no access found |
| +0x918 | 0x4 | DWORD | -- | high | 0x100092B0-0x100092B6 copies caps.MaxVertexBlendMatrixIndex (D3DCAPS8 +0xAC, DWORD) |
| +0x91C | 0x4 | 32-bit integer (signedness not shown) | -- | low | dword stores (2, 4, 0); no typed read found |
| +0x920 | 0x1 | 8-bit flag | -- | medium | byte stores of 0/1 (0x10009484), 6 byte loads |
| +0x921 | 0x1 | 8-bit flag | -- | medium | set at 0x100092E4 when caps.TextureCaps & 0x800; read at 0x1002DC38 |
| +0x922 | 0x2 | **unknown** | -- | unknown | no access found |
| +0x924 | 0x4 | 32-bit integer (signedness not shown) | -- | low | stores of 8 or 0x10 by caps.DevCaps (0x100091D6-0x10009206); read at 0x101702C1 |
| +0x928 | 0x4 | 32-bit integer (signedness not shown) | -- | low | stores of 1 or 2 with +0x924 |
| +0x92C | 0x1 | 8-bit flag | -- | medium | set at 0x100092D8 from caps.RasterCaps / TextureFilterCaps bits and caps.MaxAnisotropy >= 2 (D3DCAPS8 +0x6C) |
| +0x92D | 0x1 | 8-bit flag | -- | medium | byte stores and loads, compared with 0 |
| +0x92E | 0x1 | 8-bit flag | -- | medium | byte stores and loads, compared with 0 |
| +0x92F | 0x1 | 8-bit flag | -- | medium | byte stores and loads |
| +0x930 | 0x1 | 8-bit flag | -- | medium | set at 0x100092F1 when caps.TextureCaps & 2 |
| +0x931 | 0x1 | 8-bit flag | -- | medium | set at 0x100092FE when caps.TextureCaps & 0x20 |
| +0x932 | 0x2 | **unknown** | -- | unknown | no access found |
| +0x934 | 0x4 | DWORD | maxTextureWidth (high) | high | 0x10009305-0x10009308 copies caps.MaxTextureWidth (D3DCAPS8 +0x58); compared unsigned (jae) |
| +0x938 | 0x4 | DWORD | maxTextureHeight (high) | high | 0x1000930E-0x10009314 copies caps.MaxTextureHeight (+0x5C); compared unsigned (jae) |
| +0x93C | 0x40 | D3DMATRIX | cachedWorld (high) | high | proven 0x1000BD50: for State 0x100 (D3DTS_WORLD) `lea edi, [eax+0x93c]; rep movsd` x16 from the matrix it then passes to IDirect3DDevice8::SetTransform (slot 0x94, CONST D3DMATRIX*) at 0x1000BD96; 0x1000BDB0 copies it back out |
| +0x97C | 0x40 | D3DMATRIX | cachedView (high) | high | same, State 2 (D3DTS_VIEW): 0x1000BD7F / 0x1000BDF5 |
| +0x9BC | 0x40 | D3DMATRIX | cachedProjection (high) | high | same, State 3 (D3DTS_PROJECTION): 0x1000BD77 / 0x1000BDDF |
| +0x9FC | 0x4 | **unknown** | -- | unknown | only access: constructor 0x100090BD stores 0 |
| +0xA00 | 0x4 | **unknown** | -- | unknown | only access: Initialize 0x10009495 stores 0xFFFFFFFF |
| +0xA04 | 0x4 | HCURSOR [18 (0xA04..0xA4B, bounded by +0xA4C; no bound check found)] | -- | high | 0x10009833-0x1000983F: LoadCursorA(NULL, 0x7F00) (import 0x1032937C) stored at [edi+esi*4+0xA04] and passed to SetCursor (0x10329380); index from a switch on arg1 |
| +0xA4C | 0x4 | **unknown** | -- | unknown | only access: Initialize 0x100095ED stores 0 |

### CYyModel

- Size: 0xBC (descriptor 0x1032A6D0 (slot 0 0x100298C0): name "CYyModel", size 0xBC, parent CYyObject)
- Vtable: 0x1032A75C
- Identified by: Descriptor. `this` proven only in the constructor 0x1002B4B0 (stores vtable 0x1032A75C at 0x1002B4EE), the destructor 0x1002B550 (stores it at 0x1002B556) and 0x1002B530 (this_class.json). No other function was reached with this object from the code; the tree places 11 more by IDB name (not used for types here).

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x0 | 0x4 | void ** (vtable 0x1032A75C) | vftable | high | 0x1002B4EE |
| +0x4 | 0x2C | **unknown** | -- | unknown | no access in the proven functions (the tree-placed 0x1002C400 reads +0x24/+0x28/+0x2C with `fld dword` -- floats if that function runs on this object, which the code does not show) |
| +0x30 | 0x4 | **unknown** | -- | unknown | constructor 0x1002B4F4 stores 0 (dword); tree-placed 0x1002C4B2 tests byte & 0x10 |
| +0x34 | 0xC | **unknown** | -- | unknown | no access found |
| +0x40 | 0x4 | **unknown** | -- | unknown | constructor 0x1002B4F7 stores 0x40000000 (bit pattern of 2.0f); no read found |
| +0x44 | 0x4 | pointer to a polymorphic object (class not known) | -- | medium | destructor 0x1002B553-0x1002B567: if non-null, `mov eax, [ecx]; push 1; call [eax+0x18]` (vtable slot 6 with 1, the scalar-deleting-destructor slot of every CYyObject vtable in class_vtables.json), then stores 0 |
| +0x48 | 0x2C | embedded object, vtable 0x1032A844 (class name not known) | -- | high | constructor `lea ecx, [esi+0x48]; call 0x10033710` (stores vtable 0x1032A844 at 0x10033713); destructor tail-jumps to 0x100337C0 on it. Extent 0x2C = up to +0x74 (layout bound) |
| +0x74 | 0x18 | embedded object, vtable 0x1032A340 (class name not known) | -- | high | constructor `lea ecx, [esi+0x74]; call 0x1001D170` (stores vtable 0x1032A340); destructor calls 0x1001D1F0. Extent up to +0x8C (layout bound) |
| +0x8C | 0x4 | embedded object, vtable 0x1032A480 (class name not known) | -- | high | constructor `lea ecx, [esi+0x8c]; call 0x10025BF0` (stores vtable 0x1032A480 only); destructor calls 0x10025C30. Extent 4 (layout bound) |
| +0x90 | 0xC | embedded object, vtable 0x1032A460 (class name not known) | -- | high | constructor `lea ecx, [esi+0x90]; call 0x10025260`; destructor calls 0x100252F0. Extent up to +0x9C (layout bound) |
| +0x9C | 0xC | embedded object, vtable 0x1032A9EC (class name not known) | -- | high | constructor `lea ecx, [esi+0x9c]; call 0x10038A20` (stores the vtable and zeroes its +4, +8); destructor calls 0x10038A60 |
| +0xA8 | 0x4 | **unknown** | -- | unknown | constructor 0x1002B501 stores 0 |
| +0xAC | 0x4 | **unknown** | -- | unknown | constructor 0x1002B507 stores 0 |
| +0xB0 | 0x1 | **unknown** | -- | unknown | constructor 0x1002B50D byte store of 0 |
| +0xB1 | 0x3 | **unknown** | -- | unknown | no access found |
| +0xB4 | 0x4 | **unknown** | -- | unknown | constructor 0x1002B513 stores 0 |
| +0xB8 | 0x1 | **unknown** | -- | unknown | constructor 0x1002B519 byte store of 0 |
| +0xB9 | 0x3 | **unknown** | -- | unknown | no access found |

### CYyModelBase

- Size: 0xC8 (descriptor 0x1032A6C4 (slot 0 0x100298A0): name "CYyModelBase", size 0xC8, parent CYyObject)
- Vtable: 0x1032A710
- Identified by: Descriptor. `this` proven in the constructor 0x10029D30 (stores vtable 0x1032A710 at 0x10029D5A), the destructor 0x10029DD0 and 0x10029DB0 (this_class.json).

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x0 | 0x4 | void ** (vtable 0x1032A710) | vftable | high | 0x10029D5A |
| +0x4 | 0x4 | pointer to a polymorphic object (class not known) | -- | medium | destructor 0x10029DEB-0x10029DF9: if non-null, vtable slot 6 called with 1, then 0 stored; constructor stores 0 |
| +0x8 | 0x18 | CYySkl (embedded) | skeleton (medium) | high | constructor 0x10029D35-0x10029D3E `lea ecx, [esi+8]; call 0x10034230` (CYySkl constructor: stores vtable 0x1032A888, descriptor "CYySkl" size 0x18); destructor `lea ecx, [esi+8]` at 0x10029E3A |
| +0x20 | 0x4 | pointer to a polymorphic object (class not known) | -- | medium | destructor 0x10029DFC-0x10029E0A: slot 6 called with 1, then 0 stored; constructor stores 0 |
| +0x24 | 0x14 | CYyMotionQueList (embedded) [7 (0x24..0xAF)] | -- | high | constructor loop 0x10029D43-0x10029D56: ebx=7, `mov ecx, edi; call 0x1001ABF0; add edi, 0x14`; 0x1001ABF0 stores vtable 0x1032A210 = CYyMotionQueList (descriptor size 0x14); destructor loops 0x1001B140 and 0x1001AC30 over the same 7 |
| +0xB0 | 0x4 | **unknown** [3 (0xB0, 0xB4, 0xB8 copied from 0x103514F4/F8/FC)] | -- | unknown | constructor 0x10029D71 copies global 0x103514F4 |
| +0xBC | 0x4 | **unknown** | -- | unknown | constructor 0x10029D8F stores 0xBF800000 (bit pattern of -1.0f); no read found |
| +0xC0 | 0x4 | **unknown** | -- | unknown | constructor 0x10029D66 stores 0 |
| +0xC4 | 0x4 | **unknown** | -- | unknown | constructor 0x10029D99 stores 0xFFFFFFFF; the tree-placed DrawLit 0x1002A3B8/0x1002A3F0 reads and writes it as a dword |

### CYyModelDt

- Size: 0x3C (descriptor 0x1032A6B8 (slot 0 0x10029880): name "CYyModelDt", size 0x3C, parent CYyObject; the allocation 0x10029FD8 `push 0x3c` for the vtable-0x1032A72C objects)
- Vtable: 0x1032A6E0
- Identified by: Descriptor. Constructor 0x10029C90 (stores vtable 0x1032A6E0), destructor 0x10029CD0, 0x10029CB0 (this_class). Two vtables with no descriptor of their own report this one in slot 0: 0x1032A430 (constructor 0x10025010 calls 0x10029C90 first, then stores 0x1032A430) and 0x1032A72C (0x10029FEA calls 0x10029C90, then 0x10029FEF stores 0x1032A72C); their methods (this_class.json) access the same +0x8.

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x0 | 0x4 | void ** (vtable 0x1032A6E0) | vftable | high | 0x10029C9C |
| +0x4 | 0x4 | pointer to a polymorphic object (class not known) | -- | medium | destructor 0x10029CF8-0x10029D06: if non-null, vtable slot 6 called with 1, then 0 stored; constructor stores 0 (0x10029CA2) |
| +0x8 | 0x4 | void ** (handle: pointer to the pointer of the mesh object below) | -- | medium | written at 0x10029FC0 with the handle passed to 0x10029F70; in the derived classes' methods it is the argument of 0x10072FB0 (`mov eax, [esp+4]; test eax, eax; je; mov eax, [eax]; ret 4`: returns *arg), whose result 0x1002A02D passes as ECX to 0x1002DC10 (SkinAndDrawV2); the destructor 0x10029CD4-0x10029CE3 does `mov ecx, [+0x8]->[0]; call 0x10071EC0`, as CYyModelBase's destructor does on CYySkl +0x4 |
| +0xC | 0x4 | **unknown** | -- | unknown | constructor 0x10029CA8 stores 0; no read in the proven functions |
| +0x10 | 0x2C | embedded object, vtable 0x1032A6DC (class name not known) | -- | high | constructor 0x10029C92-0x10029C95 `lea ecx, [edx+0x10]; call 0x10029BB0`, which stores vtable 0x1032A6DC and zeroes its +0x4..+0x28. Extent 0x2C = to the end of the 0x3C object |

### CYyModelDt_VT_1032A430

- Size: 0x1C8 (allocation 0x10029F97 `push 0x1c8; call 0x1002C890` immediately followed by its constructor 0x10025010 (0x1002C890 is the allocator that 0x100161C8 calls with CDx's descriptor size 0xA50))
- Vtable: 0x1032A430
- Identified by: Vtable 0x1032A430 stored by constructor 0x10025010 after the CYyModelDt constructor; slot 0 is CYyModelDt's GetRuntimeClass, so it has no descriptor of its own (a class derived from CYyModelDt). Fields +0x0..+0x3B are CYyModelDt's.

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x3C | 0x1 | **unknown** | -- | unknown | constructor 0x10025025 `mov byte ptr [esi+0x3c], 1`; no read found |
| +0x40 | 0x4 | pointer to a heap block (from the allocator 0x10028640) | -- | medium | constructor 0x1002501E stores 0; destructor 0x100250C3 passes it to 0x10028700 (the matching free: frees what 0x10028640 returns, as CYySkl +0x8 shows) |
| +0x44 | 0x184 | **unknown** | -- | unknown | no access found in the 8 proven functions |

### CYySkl

- Size: 0x18 (descriptor 0x1032A87C (slot 0 0x10034070): name "CYySkl", size 0x18, parent CYyObject; CYyModelBase holds one at +0x8 and its next member is at +0x20)
- Vtable: 0x1032A888
- Identified by: Descriptor. Constructor 0x10034230 and destructor 0x10034270 (store vtable 0x1032A888), 0x10034250 (this_class). Also 0x100342A0 and 0x100352A0: their only direct calls pass `lea ecx, [model+8]` (0x10029E57, 0x1002A0C4), the CYySkl inside a CYyModelBase, and they touch the same +0x4/+0x8/+0xC/+0x14 as the constructor and destructor (medium: the callers' own object is known from the tree placement).

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x0 | 0x4 | void ** (vtable 0x1032A888) | vftable | high | 0x10034234 |
| +0x4 | 0x4 | void ** (handle, dereferenced by 0x10072FB0) | -- | medium | 0x100342A4-0x100342B2: passed to 0x10072FB0 (returns *arg); the object it yields has a 16-bit count at +0x32 (0x100342BB `mov di, word ptr [eax+0x32]`) that sizes +0x8/+0x14; CYyModelBase destructor reads it as [model+0xC] (0x10029DD7) and calls 0x10071EC0 on *it |
| +0x8 | 0x4 | pointer to a heap block from 0x10028640 | -- | medium | 0x100342C6-0x100342CE: 0x10028640((count+1)*64, 2); freed by 0x10028700 in the destructor (0x10034273-0x10034281) |
| +0xC | 0x4 | pointer to count 64-byte 4x4 float matrices, 16-byte aligned | boneMatrices (low) | medium | 0x100342D5-0x100342DC: (+0x8 + 15) & ~15; loop 0x100342E7-0x100342F5 calls 0x100279B0 on each 0x40-byte element, which stores 1.0f at element +0x0/+0x14/+0x28/+0x3C and 0 elsewhere (a 4x4 float identity). Same layout as D3DMATRIX; no D3D call taking them was traced here. |
| +0x10 | 0x1 | **unknown** | -- | unknown | constructor 0x10034243 byte store of 0 |
| +0x11 | 0x3 | **unknown** | -- | unknown | no access found |
| +0x14 | 0x4 | pointer to a heap block of count bytes | -- | medium | 0x100342F9-0x10034304: 0x10028640(count, 2); freed in the destructor (0x10034289-0x10034291); read by 0x100352A0 |

### Obj_SkinAndDrawV2_this

- Size: not known (not known)
- Vtable: none known
- Identified by: The object 0x1002DC10 (tree: CYyModelDt::SkinAndDrawV2) and 0x1002F1D0 (Model_SkinVerticesCpu) run on. 0x1002DC10's only call, 0x1002A02F, passes ECX = 0x10072FB0([CYyModelDt+0x8]) (the handle's target), not the CYyModelDt; 0x1002F1D0's only call, 0x1002DDF4, passes 0x1002DC10's own this (ebx). Neither address is taken. Its class is not settled by the code read. Dword fields at unaligned offsets (+0x36, +0x42, +0x5E) show a packed (file-image) layout.

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| +0x32 | 0x1 | 8-bit integer | -- | low | 4 byte loads in 0x1002DC10/0x1002F1D0 paths |
| +0x34 | 0x1 | 8-bit bit flags | -- | medium | `test byte ptr [ebx+0x34], 1` + je at 0x1002DD46, 0x1002DE30 |
| +0x36 | 0x4 | **unknown** | -- | unknown | 0x1002DC55 dword load (unaligned) stored to a local |
| +0x3C | 0x4 | **unknown** | -- | unknown | 4 dword loads |
| +0x42 | 0x4 | pointer to 16-bit values | -- | medium | 0x1002DC52/0x1002DC5B: `mov eax, [ebx+0x42]; mov cx, word ptr [eax]`, then word [eax+2] (0x1002DC73) |
| +0x48 | 0x4 | pointer to 4-byte elements | -- | medium | 0x1002DC7F/0x1002DC94: `mov eax, [ebx+0x48]` ... `lea eax, [eax+edx*4]` |
| +0x5E | 0x4 | pointer to a structure (class not known) | -- | medium | 0x1002DC82 `mov ebp, [ebx+0x5e]`; its +0x40 and +0x4C are tested for null (0x1002DC9C, 0x1002DCA7) |

## Where the tree's current declarations disagree with the code

| Tree declaration | Code | Evidence |
|---|---|---|
| `CDx::defaultDepthSurface` `uint8_t` at +0x94 | 4-byte `IDirect3DSurface8 *` | 0x1000962E dword store; 0x1000A320 pushes the dword as SetRenderTarget's pNewZStencil |
| `CDx::clearFlags/clearColor/clearZ/clearStencil` `uint8_t` at +0x164/+0x168/+0x16C/+0x170 | dwords | 0x10009648, 0x10009652, 0x1000965C, 0x10009662 `mov dword ptr [...]`; names rest on the caller's argument order only (no read found) |
| `CDx::field_814/818/81C/820/824` `uint8_t` | `IDirect3DTexture8 *` each | D3DXCreateTexture ppTexture (0x1000C935) / CreateTexture wrapper return (0x1000C798) |
| `CDx::stereoOddLineTexture` `uint8_t` at +0x82C | `IDirect3DTexture8 *` | 0x1000C86E pushes &+0x82C as ppTexture of _D3DXCreateTexture@32 |
| `CDx::field_830`, `field_834` `uint8_t` | +0x830 DWORD; +0x834 is a whole `D3DCAPS8` (0xD4 bytes, to +0x907) | 0x1000932E; 0x100091BF-0x100091C9 (GetDeviceCaps) |
| `CDx::field_90C`, `field_90E` `uint8_t` | 16-bit | 0x10009230, 0x1000924A word stores |
| `CDx::field_918/91C/924/928/934/938` `uint8_t` | 32-bit (934/938 = D3DCAPS8.MaxTextureWidth/Height) | 0x100092B6, 0x100091D6, 0x100091DC, 0x10009308, 0x10009314 dword stores |
| `CDx::cachedWorld/cachedView/cachedProjection` `uint8_t` | `D3DMATRIX` (64 bytes each) | 0x1000BD6F/0x1000BD7F/0x1000BD77 `rep movsd` of 16 dwords from SetTransform's pMatrix |
| `CDx::field_A00`, `field_A04`, `field_A4C` `uint8_t` | dwords; +0xA04 is an `HCURSOR` array | 0x10009495, 0x1000983F, 0x100095ED |
| `CDx::defaultBackBuffer` `_DWORD *` at +0x90 | `IDirect3DSurface8 *` (that it is the back buffer is not shown) | 0x1000A32A pRenderTarget of SetRenderTarget |
| `CDx`: +0x190..+0x1EB and +0x1F4..+0x80F are padding | embedded `CAcc2`, `CAcc[3]`, `CAcc`, and an object with vtable 0x10329E88 | constructor 0x10008F95, 0x10008FA6-0x10008FBC, 0x10008FBE, 0x10008FC9 |
| `CYyModelDt::field_8` `int32_t` | a handle (pointer to the pointer of the mesh object) | 0x10072FB0 `mov eax, [eax]` on it; destructor 0x10029CE1 `mov ecx, [eax]` |
| `CYyModelDt_VT_1032A430` `sizeof == 0x3C` | objects are 0x1C8 bytes; +0x3C/+0x40 are written | allocation 0x10029F97 `push 0x1c8` then constructor 0x10025010; 0x10025025, 0x1002501E |
| `CYyModelBase::AllocMatrixBuffers` (0x100342A0) is a CYyModelBase member | its ECX is the CYySkl at CYyModelBase+0x8 | only call 0x10029E57 `lea ecx, [esi+8]`; it touches CYySkl's +0x4/+0x8/+0xC/+0x14 |
| `CYyModelDt::SkinAndDrawV2` (0x1002DC10) is a CYyModelDt member | its ECX is `ResourceHandle_Deref([CYyModelDt+0x8])`, not the CYyModelDt | only call 0x1002A022-0x1002A02F `call 0x10072fb0; mov ecx, eax; call 0x1002dc10` |
| IDB `CYyModelBase.field_C` | +0xC is CYySkl's +0x4 (the skeleton's handle) | constructor 0x10029D35 (`lea ecx, [esi+8]` -> CYySkl ctor) |

`CYyModelDt::field_33`/`field_34` (tree) lie inside the embedded object at +0x10; no access to them was found by
this scan, so they are neither confirmed nor contradicted.

## Not settled (said as not known, not filled)

- The class of the object `SkinAndDrawV2`/`Model_SkinVerticesCpu` run on (`Obj_SkinAndDrawV2_this`), and its size.
- The classes behind the vtables 0x10329E88 (CDx+0x1F4), 0x1032A844, 0x1032A340, 0x1032A480, 0x1032A460,
  0x1032A9EC (CYyModel members), 0x1032A6DC (CYyModelDt+0x10): no runtime-class slot; extents are layout bounds.
- What the pointers at CYyModel+0x44, CYyModelBase+0x4/+0x20 and CYyModelDt+0x4 point to (only "deleted through
  vtable slot 6").
- Whether anything outside the scanned paths overwrites CDx+0x8/+0xC (the stored value is proven; the absence of
  other writers is shown only over the 772 g_pCDx functions and 45 `this` functions).
- Array lengths of CDx +0x10/+0x30/+0x50/+0x70 (8), +0x98 (8 viewports), +0xA04 (18 cursors): bounded by the next
  member, no bound check found.
- CDx +0x164..+0x170 ("clear" values), +0x178..+0x18C, +0x9FC, +0xA00, +0xA4C, +0x1EC, +0x1F0: written, never read in
  a way that gives a type.
- Whether CYySkl's matrices are used as `D3DMATRIX` by a D3D call (the layout is 16 floats, identity-initialised).
- Object identity in 0x10009F20/0x1000A2B0 (render-target push/pop) rests on the tree placement; the SDK-typed uses
  of CDx +0x10/+0x30/+0x70/+0x90/+0x94 are in those functions, hence medium.
- CYyModel +0x4..+0x2F and CYyModelDt_VT_1032A430 +0x44..+0x1C7: no access in the proven functions.
