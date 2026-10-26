# Data formats, read from the code that reads them

This document describes the game's model, skin, skeleton, motion, texture, zone-mesh, dancer (`sq*`) and shader
data. Everything here comes from the code that reads the data, and each fact names its source.

## Sources and how to read the tables

What the facts rest on (CLAUDE.md):

- **tree**: the gate-verified C++ in `tree/src`. The function address is its `// FUNCTION: FFXIMAIN 0x...` line.
- **insn**: the game's own instructions, read with `pipeline/disasm.py` and `pipeline/field_reads.py` (new,
  below). Data tables in the image are read the same way (`pipeline/vs_decl.py`, new).
- **SDK header**: constants are decoded with MinGW-w64's `d3d8types.h` / `d3d8.h`
  (`/usr/share/mingw-w64/include/`; `tree/include/xi_d3d8.hpp` is generated from them). D3DX filter values
  come from `d3dx9tex.h`, because no `d3dx8tex.h` is installed here. Those are marked "(d3dx9 header)".
- **decomp text**: some functions are not in the tree. For those, the Hex-Rays text in
  `data/exports/gfx_export.jsonl` or the Ghidra text in `data/exports/ghidra_frameset.jsonl` was used to find
  the reads. Every offset taken from such text was then checked against the instructions (`field_reads.py`
  lists the same displacements) unless the row says "decomp text only".

Confidence column:
- `code`: the read is in the tree source or in the instructions at the named address.
- `header`: a constant decoded with the named SDK header.
- `derived`: follows arithmetically from `code` facts; the row says how.
- `decomp`: only in decompiler text, not re-checked in the instructions.

A purpose ("positions", "bone index") is stated only when the code shows it, for example a value passed as an
FVF or used as a matrix index. Otherwise the field is described by how it is read.

Offsets: `d+X` is an offset into a resource chunk's data. In memory that data sits at `object+0x30+X`
(section 1). `obj+X` is an offset into the in-memory object. Per-frame relevance comes from
`data/frame_set.json`: "frame" means the function has `stack` or `self` evidence there.

New tools (read-only; they print to stdout and never write game bytes to the repo):

    FFXI_DLL=... python3 pipeline/field_reads.py ADDR...           # every [reg+disp] access, width, r/w
    FFXI_DLL=... python3 pipeline/field_reads.py --reg esi ADDR     # only through one base register
    FFXI_DLL=... python3 pipeline/vs_decl.py 10351628 103772E8 10377304   # decode D3D8 vertex declarations

---

## 1. Resource chunks (the container every format below sits in)

**Readers:**
- `0x10071270`: the resource factory. Not in the tree; read from insn, and the Ghidra text was used to find
  the reads.
- `0x10071E00`: the chunk copy. Not in the tree; insn.
- `CMoResource::FindWorker` `0x100722C0`: tree.
- `CYyTexMng::FindByNameInResource` `0x1003AF10`: tree.

**Chunk header (16 bytes, at the start of each chunk):**

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| +0x00 | 4 | u32 | FindWorker 0x100722C0 (`obj+0x20 == param_3`); 0x1001D8FD, 0x1001D90D (whole dword); 0x100198E1 (high 16 bits) | chunk id, compared as a 32-bit value. Examples in the code: 0x65666665 (bytes "effe") and 0x74737973 ("syst") at 0x1001D8FD/0x1001D90D | code |
| +0x04 | 4 | u32 bits 0-6 | 0x100712D5 `and eax,0x7f`, then a switch over 0..0x5F (jump table 0x1007189C, index bytes 0x10071914) | chunk type. Selects the class (table below). FindWorker compares `obj+0x24 & 0x7F` with the type it searches for | code |
| +0x04 | | bits 7-25 | 0x1007127B `shr 7; and 0x7ffff; shl 4`; 0x10071E08 (same) | chunk size in 16-byte units, header included. 0x10071E00 copies exactly that many bytes to `obj+0x20` | code |
| +0x08 | 8 | | not read by these functions | not known | |

**In memory:**
- The factory allocates `(size − 16) + N` bytes, with N per type (below).
- It constructs the class, then calls vtable slot 7. For every vtable below whose slots are in
  `data/class_vtables.json`, slot 7 is 0x10071E00, which copies the whole chunk to `obj+0x20`.
- So the header lands at `obj+0x20..+0x2F` and the data at `obj+0x30`.
- Object fields the code uses (CMoResource, tree):
  - `obj+0x08`: next-sibling handle;
  - `obj+0x14`: the object's own handle;
  - `obj+0x28`: parent handle;
  - `obj+0x3C` (type 1 only): FindWorker's end-of-scope test.
- Type 1 is a scope: FindWorker climbs to it and walks its children.

**Types with a class of their own** (0x10071270, insn: the targets of the jump table 0x1007189C, indexed by the
bytes at 0x10071914; N is each target's `lea reg,[edi+N]`, where edi = chunk bytes − 16, set at 0x1007127B..0x1007128F):

| Type | N | vtable / constructor | Setup (vtable slot 8) | Version string printed on a bad version |
|---|---|---|---|---|
| 0x01 | 0x40 | 0x1032C588 | 0x10074610 | |
| 0x05 | 0x128 | ctor 0x10047F80 (CMoGeneratorClone_VT_1032B114) | | |
| 0x06 | 0xB8 | 0x1032C5D0 (+ sub-object 0x1003B540 at +0x30) | | |
| 0x07 | 0x98 | 0x1032C644 (+ 0x1003B540 at +0x30) | | |
| 0x19 | 0x40 | 0x1032C5FC | 0x10054480 | |
| 0x1B, 0x20 | 0x6D | 0x1032C8AC | 0x1001D800 `TexRes_LoadBm2` | "Bm2 Version Error(%d)" (0x103511F4, pushed at 0x1001D841) |
| 0x1C | 0x30 | ctor 0x10176F60 | | |
| 0x1F | 0x30 | 0x1032C74C | 0x1003FEE0 | |
| 0x21 | 0x30 | 0x1032C720 | 0x1003E6B0 | |
| 0x25 | 0x50 | 0x1032C6F4 | 0x1003F420 | |
| 0x29 | 0x52 | 0x1032C828 | 0x10034140 | "Sk2 Version Error(%d)" (0x103516A8, at 0x10034156) |
| 0x2A | 0x70 | 0x1032C854 | 0x1002D7F0 | "Os2 Version Error(%d)" (0x10351654, at 0x1002D80D) |
| 0x2B | 0x3C | 0x1032C7FC | 0x10019730 | "Mo2 Version Error(%d)" (0x1035104C, at 0x1001974D) |
| 0x2E | 0x34 | ctor 0x10176E80 | | |
| 0x2F | 0xDC | ctor 0x101893B0 | | |
| 0x30, 0x31, 0x5F | 0x31 | 0x1032C69C, 0x1032C6C8, 0x1032C670 | | |
| 0x36 | 0x30 | ctor 0x10176FE0 | | |
| 0x3D, 0x58 | 0x40 | 0x1032C530 (IDB: CYySepRes) | | |
| 0x3E | 0x50 | 0x1032C55C | 0x10063C50 | |
| 0x45 | 0x40 | 0x1032C7D0 (+ 0x100220E0 at +0x30) | | |
| 0x4A | 0x50 | 0x1032C504 (IDB: CMoSphRes) | | |
| 0x54 | 0x50 | 0x1032C778 (+ 0x1001CF80 at +0x30) | 0x10071A20 | "Afb Version Error(%d)" (0x103511B0, referenced at 0x1001CFB7, in a code chunk `data/func_extents.json` assigns to 0x10071A20) |
| 0x5D | 0x51 | 0x1032C880 | 0x1001DF60 | |
| 0x5E | 0x84 | 0x1032C7A4 | 0x10038920 | |
| all others | 0x30 | the base resource (0x10071D80) | | |

Slot 8 addresses come from `data/class_vtables.json`. The version strings were found by searching the image for
"Version Error" and following the references (xref by byte search; the pushing instruction is named).

The version-string functions print through `nullsub_13` (0x10017D20, a bare `ret`), so it does nothing in this
build. A bad version
therefore produces no message:
- 0x1001D800 and 0x10034140 carry on regardless (0x10034140 only prints);
- 0x1002D7F0 returns before any setup (Os2, section 2).

**Not known:**
- bytes +0x08..+0x0F of the chunk header;
- what file or archive the chunks are read from: the factory receives the header pointer from its caller, which
  was not traced.

---

## 2. Os2: skinned mesh (chunk type 0x2A)

The game's own name comes from the string at 0x10351654 ("Os2 Version Error"). This is the format the
character draw path uses:
- `CYyModelDt::SkinAndDrawV2` 0x1002DC10: frame, 25,819 stack samples;
- `Model_SkinVerticesCpu` 0x1002F1D0: frame, 5,609 stack samples.

### 2.1 Header (chunk data, `d+X` = `obj+0x30+X`)

The setup is `0x1002D7F0`, slot 8 (insn). It calls:
- `0x1002D8F0` (insn) for kind 0;
- `0x10022470` (insn) for kind 1.

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| d+0x00 | 1 | u8 | 0x1002D7F5, 0x1002D808 | version. Only 0 and 1 are accepted; any other value returns from setup with nothing set up (the error print is a no-op) | code |
| d+0x01 | 1 | u8 | 0x1002D81B (read), 0x1002D837/0x1002D84B (write 1) | set-up flag: setup runs only while it is 0, then writes 1. Whether files hold 0 here is not known | code |
| d+0x02 | 1 | u8 bits 0-6 | 0x1002D822; FUN_1002d860 | kind. 0: GPU-buffered path (0x1002D8F0, 2.3). 1: streamed path (0x10022470, 2.6) | code |
| d+0x02 | | bit 7 | `[obj+0x32] & 0x80`, 0x1002F1D0 (18 sites) | when set, the 7-bit bone numbers in the per-vertex words index the bone list at d+0x0C, whose u16 entries are the matrix indices. When clear, the 7-bit value is the matrix index | code |
| d+0x03 | 1 | u8 | SkinAndDrawV2 0x1002DC10 `switch (this->field_33)` | sets per-draw "hide" bytes `a3+169..176` (2.5). Values 2, 3, 4, 5, 6, 0x12, 0x22, 0x32 do something; others nothing | code (tree) |
| d+0x04 | 1 | u8 | bit 0: SkinAndDrawV2, 0x1002F1D0, 0x1002C990; bit 1: 0x1002D929/0x1002D96B | bit 0: build and draw a second, mirrored copy (2.4). Bit 1: written at run time ("vertex buffer object made") | code |
| d+0x06 | 4 | u32 | 0x1002D8F4 | word offset of the draw-command stream (2.2). Fixed up to `obj + 0x30 + 2·off` | code |
| d+0x0A | 2 | | not read by any function here | not known | |
| d+0x0C | 4 | u32 | 0x1002D8F7 | word offset of the bone list: u16 entries | code |
| d+0x10 | 2 | u16 | `CYyModelDt::MarkReferencedBones` 0x100251A0 (tree: `*(_WORD*)(v3+64)`) | bone-list count. MarkReferencedBones writes 0x80 into `a2[list[i]]` for each entry | code (tree) |
| d+0x12 | 4 | u32 | 0x1002D905 | word offset of a pair `u16 count1, u16 count2`: the number of one-bone and two-bone vertices (2.4) | code |
| d+0x16 | 2 | | not read here | not known | |
| d+0x18 | 4 | u32 | 0x1002D90B | word offset of the per-vertex bone words: 4 bytes per vertex, `count1 + count2` entries (2.4) | code |
| d+0x1C | 2 | | not read here | not known | |
| d+0x1E | 4 | u32 | 0x1002D915; copied to the vertex-buffer object's +0x6E by 0x1002D310 | word offset of the source vertex data (2.4) | code |
| d+0x22 | 2 | | not read here | not known | |
| d+0x24 | 4 | u32 | 0x1002D91F (fixed up) | word offset of a block no function here reads | code (fix-up only) |
| d+0x28 | 6 | | not read here | not known | |
| d+0x2E | 4 | u32 | kind 0: zeroed at 0x1002D83F, then the CYyOs2VtxBuffer pointer stored at 0x1002D96F. Kind 1: fixed up as a word offset (0x100224A9) | kind 0: run-time pointer (2.3). Kind 1: offset of a block read by `CYyModelDt_PrepareRenderState` (2.6) | code |
| d+0x4C | 4 | u32 | kind 1 only: fixed up at 0x100224B3 | word offset; reader not traced | code (fix-up only) |

`CMoResource_VT_1032C854::vf004` 0x1002D880 (tree) is the inverse. It turns the six pointers back into word
offsets (`(ptr − obj − 0x30) >> 1`).

### 2.2 Draw-command stream (at the d+0x06 offset)

The stream is 16-bit words and ends at `0xFFFF`. Four functions walk it, and they agree on every record length:
- `SkinAndDrawV2` 0x1002DC10 (tree);
- the index counter `0x1002CB80` (insn);
- the buffer filler `0x1002CE20` (insn; reads found in the Ghidra text);
- `CYyModelDt_PrepareRenderState` 0x100224D0 (insn; reads found in the Hex-Rays text).

| First word | Record length | Fields as read | Read at | Confidence |
|---|---|---|---|---|
| bit 15 set, `(w>>4)&0xF == 0` | 18 bytes (9 words) | Texture. The low byte of word 0, bit 0, is the "resolved" flag. +2: 16-byte texture name. On first use, `CYyTexMng::FindByNameInResource` (searches chunk type 0x20, compares 16 bytes with the texture chunk's d+0x01) replaces the name's first dword with the texture object pointer and sets the flag. The object's `+0x40` (else `+0x44`) is passed to `SetTexture(0, ...)` | SkinAndDrawV2; 0x1002CC1E (`add edx,0x12`) | code |
| bit 15 set, `(w>>4)&0xF == 1` | 46 bytes (23 words) | Material: 11 dwords at +2, copied as `v39[0..10]`. Fields in the material table below | SkinAndDrawV2; 0x1002CBB6 (`rep movsd` of 0xB dwords), 0x1002CC19 (`add edx,0x2e`) | code |
| 0x5453 | 24 + 10·n bytes | Triangle strip. Word 1: n. Then 3 entries of (u16 index) at words 2-4, followed by 3 (u32,u32) pairs at words 5-16. Then n−1 entries of (u16 index, u32, u32) | 0x1002CE20; skip `5n+12` words in SkinAndDrawV2; `n·10+0x18` at 0x1002CC8B | code |
| 0x54 | 4 + 30·n bytes | Triangle list. Word 1: n. Then n triangles of (3 × u16 index, 3 × (u32,u32)) | 0x1002CE20; skip `15n+2` words; 0x1002CC53 | code |
| 0x43 | 4 + 10·n bytes | word 1: n; contents not read | skip only (all three walkers) | code (length only) |
| 0x4353 | 12 + 2·n bytes | word 1: n; contents not read | skip only | code (length only) |
| 0xFFFF | | end | all walkers | code |

**The (u32,u32) pair:**
- 0x1002CE20 stores it as the 8 bytes of output vertex v in the vertex buffer at CYyOs2VtxBuffer+0x4C.
- That buffer is bound as stream 2 with stride 8. The declaration (2.3) maps stream 2 to `v7 ← FLOAT2`
  (D3DVSDE_TEXCOORD0, d3d8types.h).
- So the pair is the vertex's texture coordinate, two floats. Confidence: code + header.

**Index counting** (0x1002CB80): strip records add n+2 indices and list records add 3n.

**Drawing:**
- SkinAndDrawV2 draws a strip as `DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP (5), 0, count1+count2+dups,
  start, n)` and then advances `start` by n+2.
- A list is drawn as `D3DPT_TRIANGLELIST (4)` with n primitives, then `start` advances by 3n.
- Primitive type values are from d3d8types.h.

**Material fields** (`v39[i]` = dword i of the record's 11; SkinAndDrawV2 0x1002DC10, tree, unless noted):

| Dword | Read as | Used for | Confidence |
|---|---|---|---|
| 0 | u32, per-byte | `TEXTUREFACTOR = (byte(g_modelColor) · byte(d0)) >> 7` per channel, with alpha forced to 0x80 (`| 0x80000000`). Used when neither special stage-1 path applies | code |
| 1, 2 | | not read | not known |
| 3 | bits 8-11 | "kind": index into the per-draw bytes `a3+168+kind`. A nonzero byte there sets `v29`, and every draw of the following geometry records is skipped. 0x1002CB80 also sets CYyOs2VtxBuffer+0x69 for kinds 1-4. For kind 4 with dword 7's low word nonzero, it rewrites kind to 8 in the record itself (0x1002CBF0) | code |
| 3 | bits 16-31 | nonzero, with `CDx+0x921 == 1` and a texture at `[[0x1045745C]+48]`: stage 1 gets that texture, `COLOROP = D3DTOP_MODULATEALPHA_ADDCOLOR (18)`, `COLORARG2 = D3DTA_TEXTURE`, `ALPHAOP = D3DTOP_ADD (7)`, `D3DTS_TEXTURE1 = g_texture1Matrix`, `TEXTURETRANSFORMFLAGS = D3DTTFF_COUNT3`, `TEXCOORDINDEX = 0x10001` (D3DTSS_TCI_CAMERASPACENORMAL \| 1) | code + header |
| 4 | float | multiplies each enabled light's diffuse R,G,B (the D3DLIGHT8 copies, 0x68 bytes each, sizeof(D3DLIGHT8) in d3d8types.h) and the ambient R,G,B (clamped to 255) before SetLight / `D3DRS_AMBIENT` | code + header |
| 7 | low u16 | nonzero, with CYyOs2VtxBuffer+0x69 set: kind forced to 8 | code |
| 9 | float | `alpha = (int)(d9 · 0.5)` in TEXTUREFACTOR, in both special stage-1 paths | code |
| 10 | float | nonzero, with `CDx+0x921 == 1`: stage 1 gets the texture `[0x104570F4]`, the same combiner as above, `D3DTS_TEXTURE1 = g_shadowTexMatrix` (IDB name) and `TEXCOORDINDEX = 0x30001` (D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR \| 1) | code + header |
| 5, 6, 8 | | not read | not known |

The default stage-1 setup is `COLOROP = D3DTOP_MODULATE2X (5)`, `COLORARG2 = D3DTA_TFACTOR (3)`,
`ALPHAOP = D3DTOP_MODULATE4X (6)`, `ALPHAARG2 = D3DTA_TFACTOR`, no transform. Values are from d3d8types.h.

### 2.3 The GPU form: CYyOs2VtxBuffer (object 0x9E bytes, `tree/include/CYyOs2VtxBuffer.hpp`)

How it is built:
- 0x1002D8F0 allocates the object and stores it at d+0x2E (kind 0 only, when d+0x04 bit 1 is clear).
- Its task step `0x1002C990` (vtable 0x1032A7B0 slot 8; insn; frame) drives a state machine in `+0x38`:
  1. `0x1002CB80`: counts indices into `+0x58`;
  2. `CYyOs2VtxBuffer_CreateBuffersStep` 0x1002CCB0: creates the index buffer, the UV buffer and the shadow
     array (state in `+0x3C`);
  3. `0x1002CE20`: fills index and UV buffers;
  4. `0x1002D310`: points `+0x6E` at d+0x1E;
  5. `0x1002D340`: builds mirrored vertices, when d+0x04 bit 0 is set;
  6. done: `+0x68 = 1`.
- At most 16 objects build at once (`g_os2VtxBufferSlots`, 0x1045B650). If an actor flag test passes, all
  steps run in one call: the actor's `+0x70` object `+0x120`, bit 7 or 8.
- The order of the states and the slot rule come from the Ghidra text of 0x1002C990. Its reads (+0x34, +0x38,
  +0x3C, +0x58, +0x68, +0x8A, the actor's +0x70) are in the instructions.

| Offset | Size | Type | Written / read at | Used for | Confidence |
|---|---|---|---|---|---|
| +0x34 | 4 | ptr to handle | Constructor 0x1002D680 (tree) | the Os2 resource (`**(+0x34)`) | code |
| +0x40 | 4 | IDirect3DIndexBuffer8* | CreateBuffersStep: `CreateIndexBuffer(2·[+0x58], 8, 0x65, 0, &+0x40)` (0x1002CD17..0x1002CD2F, vtable +0x60) | index buffer: `D3DUSAGE_WRITEONLY`, `D3DFMT_INDEX16 (101)`, `D3DPOOL_DEFAULT`. Bound by `SetIndices` in SkinAndDrawV2 | code + header |
| +0x44, +0x48 | 4+4 | IDirect3DVertexBuffer8* | `CYyModelBase::DrawLit` 0x1002A3B0 (tree) copies them from the part's buffers `j[7+j[6]]`, `j[9+j[6]]`. Those come from `CYyVbMng::AcquireVertexBuffer(12·n+4, 0x208, 0, [0x10457540])`, n = `+0x58` | streams 0 and 1, stride 12 (SkinAndDrawV2). `AcquireVertexBuffer` passes its 2nd/3rd arguments as CreateVertexBuffer's `Usage, FVF` (tree 0x1003B210; d3d8.h order Length, Usage, FVF, Pool). So: usage `D3DUSAGE_DYNAMIC\|D3DUSAGE_WRITEONLY` (0x208), FVF 0. Locked with `D3DLOCK_DISCARD` (0x2000) each draw | code + header |
| +0x4C | 4 | IDirect3DVertexBuffer8* | CreateBuffersStep: `AcquireVertexBuffer(8·[+0x58]+4, 0x208, 0, D3DPOOL_DEFAULT)` (0x1002CD52) | stream 2, stride 8: the per-vertex (u32,u32) of 2.2 | code + header |
| +0x50, +0x54 | 4+4 | IDirect3DVertexBuffer8* | DrawLit (third/fourth acquire, when the resource's d+0x04 bit 0 is set) | mirrored copy: streams 0/1 for the second DrawIndexedPrimitive | code |
| +0x58 | 4 | u32 | 0x1002CB80 | index count (sum of n+2 and 3n). Also sizes the IB, the UV VB and the position/normal VBs | code |
| +0x5C | 4 | u16[+0x58] | CreateBuffersStep (filled with 0xFFFF); 0x1002CE20 writes; 0x1002F1D0 reads | duplicate list. Entry k is the source vertex of output vertex `count1+count2+k` (2.4). 0x1002CE20 makes such a vertex when an index is met again with a (u32,u32) pair different from the one already stored for it in the +0x7A shadow. When the list is full, it resets `+0x60` to 0 and uses the original index | code |
| +0x60 | 4 | u32 | 0x1002CE20; 0x1002F1D0; SkinAndDrawV2 (`v10+96`) | number of duplicates. SkinAndDrawV2 passes `count1+count2+[+0x60]` as NumVertices | code |
| +0x64 | 4 | vertex-shader handle | Constructor 0x1002D680 (tree): `CreateVertexShader(0x10351628, NULL, &+0x64, 0)` | the declaration below; set by `Gfx_SetVertexShaderCached` before the draws | code |
| +0x68 | 1 | u8 | 0x1002C990 (writes 1 at the end); SkinAndDrawV2 (`v10+104 != 0` gates every draw) | "built" | code |
| +0x69 | 1 | u8 | 0x1002CB80 (writes 1); SkinAndDrawV2 (`v10+105`) | the kind-8 rule of the material table | code |
| +0x6E | 4 | float* | 0x1002D310 | source vertex data = the resource's d+0x1E block | code |
| +0x72 / +0x76 | 4+4 | alloc / 64-aligned ptr | 0x1002D5FF..0x1002D613: `Mem_AllocObject(24·count1 + 56·count2 + 0x40)` | mirrored source vertices, in the same layout as +0x6E | code |
| +0x7A | 4 | ptr | 0x1002CE20: `8·[+0x58]` bytes, filled with 0xFFFFFFFF | UV shadow, used to detect a vertex that is reused with a different UV (2.4) | code |
| +0x8E..+0x9A | 4×4 | locked pointers | SkinAndDrawV2: Lock(+0x44 → +0x8E, +0x48 → +0x92, +0x50 → +0x96, +0x54 → +0x9A) | destinations of the CPU skinning | code |

**Vertex declaration at 0x10351628** (`pipeline/vs_decl.py 10351628`; decoded with d3d8types.h D3DVSD_* and
D3DVSDE_*):

    20000000 40020000 20000001 40020003 20000002 40010007 FFFFFFFF
    stream 0:  v0 (D3DVSDE_POSITION)  <- FLOAT3
    stream 1:  v3 (D3DVSDE_NORMAL)    <- FLOAT3
    stream 2:  v7 (D3DVSDE_TEXCOORD0) <- FLOAT2

It is created with no shader code, so it is a fixed-function declaration (d3d8.h: `pFunction == NULL`).

### 2.4 Vertex and skinning data

Reader: `Model_SkinVerticesCpu` 0x1002F1D0, not in the tree; insn. The reads were found in the Ghidra text and
confirmed with field_reads (the same displacements). Strides are checked in the instructions:
`and 0x7f` / `shr 7` / `and 0x7f0` at 0x1002F317..0x1002F363, stride 0x38 at 0x1002F982.
`0x1002D340` (insn) reads the same layout.

**Counts** (`[d+0x12]` → `u16 count1, u16 count2`):
- vertices 0..count1−1 have one bone;
- vertices count1..count1+count2−1 have two;
- output vertex `count1+count2+k` copies source vertex `[+0x5C][k]` (one-bone if `< count1`, else two-bone).

**Per-vertex bone word** (`[d+0x18]`, 4 bytes per vertex; one-bone vertices use the first u16, two-bone
vertices both):

| Bits | Read at | Used for | Confidence |
|---|---|---|---|
| 0-6 | 0x1002F317 `and 0x7f` | bone number: the matrix index, or an index into the bone list when d+0x02 bit 7 is set. The matrix is read at `[0x10457550] + 64·index` | code |
| 7-13 | 0x1002F351 `shr 7; and 0x7f` | bone number for the mirrored copy (only when d+0x04 bit 0) | code |
| 14-15 | 0x1002D407 `shr 0xc; and 0xc` | mirror axis. Indexes the 16-byte table at 0x103515E8, which holds (1,1,1,0) (−1,1,1,0) (1,−1,1,0) (1,1,−1,0). The source position and normal are multiplied by it to build the mirrored source (0x1002D340) | code (table bytes read from the image) |

**Source vertex layout** (`[d+0x1E]`):

| Section | Stride | Fields as read | Read at | Confidence |
|---|---|---|---|---|
| one-bone | 24 bytes | 6 floats. `[0..2]` go to `Mat4_TransformPoint` (0x10028230, tree: `out = in·M + M.row3`). `[3..5]` go to `Mat4_TransformNormal` (0x100282C0). The results are written to the stream 0 and stream 1 locks, so they are position and normal | 0x1002F1D0; 0x1002D551..0x1002D59B | code |
| two-bone | 56 bytes | 14 floats: `xa xb ya yb za zb wa wb nxa nxb nya nyb nza nzb`. Position = `xa·A.r0 + ya·A.r1 + za·A.r2 + wa·A.r3 + xb·B.r0 + yb·B.r1 + zb·B.r2 + wb·B.r3`. Normal = the same without row 3, using the n* values. A and B are the two words' matrices | 0x1002F1D0 (Ghidra text: `pfVar14[-5..8]`, pfVar14 += 0xE); 0x1002D3E7..0x1002D4B2 (component order: +0,+4 x; +8,+C y; +10,+14 z; +18,+1C copied unscaled; +20..+34 normals) | code |

Derived: wa and wb multiply only the translation row. A blend of this form gives
`wa·(A·pa) + wb·(B·pb)` only if xa..za are already multiplied by wa (and likewise for b). The data is consumed
in this pre-multiplied form. Whether the files are authored that way is not shown by the code.

**Matrices:**
- `[0x10457550]` is set by SkinAndDrawV2 from the dword at 0x1045EC20 (IDB `g_boneWeightMatrixTable`).
- `Skel_ComposeMode` 0x100347D0 (tree) writes 64-byte matrices through the same 0x1045EC20 pointer, indexed by
  skeleton bone (section 3).

**The second route:** when `[0x1045B774]` is nonzero, 0x1002F1D0 passes the job (vertex total, this resource)
to that object's vtable slot 7 instead. That object is not traced.

### 2.5 Draw (SkinAndDrawV2 0x1002DC10, tree; frame)

1. **CPU skinning.** It locks streams 0/1 (and 2'/3' when mirrored) with `D3DLOCK_DISCARD`, runs
   `Model_SkinVerticesCpu` and unlocks. It skips this when `g_skinningCapabilityFlag` (0x10457460) is 0 and
   the per-draw byte `a3+184` is already 1, and it sets `a3+184 = 1` afterwards.
2. **Stream walk.** It sets the shader `+0x64`, streams 0 (`+0x44`, 12), 1 (`+0x48`, 12), 2 (`+0x4C`, 8),
   stream 3 = NULL, and indices `+0x40`, then walks the stream of 2.2.
3. **Extra passes.** Each strip or list is drawn once, plus once more for the mirrored copy. It is drawn again
   in the environment-map, projected-texture and off-screen passes when those are active:
   - `EnvMap_BeginPass` 0x10025C40;
   - `ProjTex_BeginPass` 0x10025330;
   - `Model_BeginOffscreenPass` 0x10033800.
4. **Hide bytes.** d+0x03 sets hide bytes `a3+169..176` (index = kind + 168):
   - 2: kind 1;
   - 3: kinds 1-2;
   - 4: kinds 1-3;
   - 5: kinds 1-4 and 8;
   - 6: kinds 1-3 and 8;
   - 0x12: kind 5;
   - 0x22: kind 7;
   - 0x32: kind 6.

   This function only sets them; what clears them is not shown here.

### 2.6 Kind 1 (streamed, no per-mesh buffers)

- 0x10022470 (insn) fixes up the same six offsets plus d+0x2E and d+0x4C.
- `CYyModelDt_PrepareRenderState` 0x100224D0 draws it. It is not in the current tree; insn, with the reads
  found in the Hex-Rays text.
- Strips are expanded into the shared vertex buffer `[0x10457154]`, created by `CreateSharedVertexBuffers`
  0x10029A50 (tree):
  - size 0x4000 bytes;
  - usage 0x208;
  - FVF 0x112 = `D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1`, 32 bytes;
  - written with `D3DLOCK_NOOVERWRITE` (0x1000) and wrapped with `D3DLOCK_DISCARD` past 512 vertices.
- Drawn with `SetVertexShader(0x112)` (0x10022CFD), stride 0x20 (0x10022D18), DrawPrimitive.
- Its per-vertex expansion (`CYyModelDt_ExpandTriStrip`) was not read.

### Not known (Os2)

- d+0x0A, d+0x16, d+0x1C, d+0x22, d+0x28..0x2D: never read by the functions here. They sit after each offset,
  as u16s, but their meaning is not shown.
- The d+0x24 block (and d+0x4C for kind 1): fixed up, but no reader traced.
- The contents of the 0x43 and 0x4353 records.
- Material dwords 1, 2, 5, 6, 8.
- Who sets and clears the per-draw hide bytes `a3+168..176` other than this function.
- Whether `j[6]` (DrawLit's choice between two buffer sets) flips per frame.
- The object at `[0x1045B774]`.

---

## 3. Sk2: skeleton (chunk type 0x29)

The setup `0x10034140` (insn) only checks d+0x00 == 0 ("Sk2 Version Error" otherwise) and fixes nothing up.

The readers below take the resource through a handle (`ResourceHandle_Deref`). Every one of them reads
`obj+0x32` and `obj+0x34`:
- `Skel_*` 0x10034620..0x10035330 (tree, unit_10034620_Skel.cpp);
- `CYyModelBase::AllocMatrixBuffers` 0x100342A0 (tree).

These functions do not show that the handle is a type-0x29 chunk. The "Sk2" name rests only on the setup's
string, and on the matching data start (`obj+0x30`).

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| d+0x00 | 1 | u8 | 0x10034143 | version: must be 0 (a no-op print otherwise; nothing else happens) | code |
| d+0x01 | 1 | | not read | not known | |
| d+0x02 | 2 | u16 | `Skel_LoadBoneRotTrans` 0x10034620, `Skel_ComposeMode` 0x100347D0, `Skel_GetBoneTableBase` 0x10035210, `AllocMatrixBuffers` 0x100342A0 (allocates `(count+1)·64` bytes, 16-aligned) | bone count | code (tree) |
| d+0x04 | 30·count | bone records | below | | |
| after bones | | point table | `Skel_GetRecordAt` 0x100351F0, `Skel_GetTableAfterBones` 0x10035250 | below | code (tree) |

**Bone record** (30 bytes, at `d+0x04 + 30·i`):

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| +0x00 | 1 | u8 | `Skel_ComposeMode` (`*local_48`), `Skel_MarkChain` 0x10035330 | parent bone index. The matrix of bone i is post-multiplied by the parent's. MarkChain recurses until the parent equals the bone itself | code (tree) |
| +0x01 | 1 | | not read | not known | |
| +0x02 | 16 | 4 × f32 | `Skel_LoadBoneRotTrans` (`puVar6[-1..2]`) | passed as one quaternion operand to `Quat_Multiply` 0x10032B50, then normalised | code (tree) |
| +0x12 | 12 | 3 × f32 | `Skel_LoadBoneRotTrans` (`puVar6[3..5]`) | added to the pose translation. For bone 0 that translation is also transformed by a Y rotation of 4.7123889923 rad (`Mat4_PostRotateY`) | code (tree) |

**Point table** (follows the bones at `T = d+0x04 + 30·count`):
- `T+0`: u16 point count (`Skel_GetTableAfterBones` multiplies it by 13 words).
- `T+2`: u16, not read.
- `T+4 + 26·i`: point record.
  - `+0x00` i16: bone index. `CYyModelBase::FindPartIndex` returns it; `Unit1002A0C0_ApplyToVec4` 0x1002A780
    post-multiplies by matrix `+0x14 + 64·bone`.
  - `+0x02`, `+0x06`, `+0x0A`: three floats passed to `Mat4_PostRotateX`, `Y` and `Z`.
  - `+0x0E`: three floats used as the translation (`Part_CopyTranslation` 0x1002A890).
  - All code (tree).
- After the points, at `U = T + 4 + 26·count`:
  - six floats read by `CXiSkeletonActor::GetHeightScale` 0x100D09B0 (`U[1]−U[0]`), `GetWidthScale` 0x100D08E0
    (`U[2]−U[3]`) and `GetDepthScale` 0x100D0A80 (`U[4]−U[5]`). Each difference is scaled by a constant
    (`0x1.2d2d2cp-1` width, `0x1.0d7944p-1` height and depth) and by the actor's scale;
  - `Skel_GetEntryAt` 0x10035270 returns `U + 0x48 + 12·i`: 12-byte entries.

### Not known (Sk2)

- d+0x01, bone +0x01, `T+2`.
- Which of the four quaternion floats is w: the operand order inside `Quat_Multiply` was not read.
- What the 12-byte entries at `U+0x48` hold, and how many there are; no reader traced.
- `U+0x18..U+0x47`.

---

## 4. Mo2: motion (chunk type 0x2B), header only

Setup: `0x10019730` (insn; not in the frame set).

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| d+0x00 | 1 | u8 | 0x1001973A | version. Nonzero prints "Mo2 Version Error" (no-op) and carries on | code |
| d+0x01 | 1 | u8 | 0x1001975A, 0x10019A19 | set-up flag (setup runs once, then writes 1) | code |
| d+0x02 | 2 | i16 | 0x1001977B `movsx` | element count E | code |
| d+0x04 | 2 | i16 | 0x10019765..0x1001976F: decremented by 1 at setup when nonzero | frame count F (the loops below run F floats per track) | code |
| d+0x0A | 84·E | elements | stride 0x54 (0x10019855, 0x100199FA) | below | code |

**Element** (84 bytes, base `B = d+0x0A + 84·k`). The setup touches only these fields, in two loops that each
run only in some cases:
- **First loop** (0x100197B1..0x10019861), when `GetGlobal_1047D114()` is 0xC122, 0xC40D or 0xC129:
  - B+0x3C..+0x44 holds 3 u32;
  - a nonzero value is an index i of F consecutive floats at `d+0x0A + 4·i`;
  - for a zero value, the float at B+0x48+4j is used instead;
  - any of those floats equal to 10.5 (0x1032A1A4) becomes −5.5.
- **Second loop** (0x1001990A..0x10019A0D), when that global passes the range tests through 0x100C51C0 and the
  chunk id's high 16 bits are 0x316C, 0x316D or 0x3170:
  - B+0x04..+0x0C holds 3 u32, masked with 0x7FFFFFFF; for a zero value, the constant at B+0x14+4j is used;
  - B+0x24..+0x2C holds 3 u32;
  - bit 31 of B+0x24 is set when every float those six name compares equal to 0.0 (0x103295D8).

So track floats are addressed from the same base as the elements (d+0x0A, dword index).

### Not known (Mo2)

- B+0x00, B+0x10, B+0x20, B+0x30..+0x38 and the remaining layout.
- The meaning of the three triples. The sampling code (CYyMotionQue, unit_10019A50_MotionQueue) was not read
  for this document.
- d+0x06..d+0x09.

---

## 5. Bm2: textures (chunk types 0x20 and 0x1B)

**Readers:**
- `TexRes_LoadBm2` 0x1001D800 (setup; insn, with reads found in the Ghidra text);
- `0x1001D8E0` (insn);
- `TexObj_InitFromHeader` 0x1001D940 (insn; Hex-Rays text used to find the reads; one difference below);
- `TexObj_Create` 0x10039A80 (insn, with reads found in the Ghidra text);
- `TexObj_Upload` 0x10039E50 (tree; frame).

Upload is paced by `TexMgr_UploadPump` 0x1003ABB0 (tree; frame).

### 5.1 Chunk data

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| d+0x00 | 1 | u8 bit 7 | 0x1001D92A `shr cl,7` → InitFromHeader arg 3 | allow the DXT block (5.2); without it the code never looks for one | code |
| d+0x00 | | bits 4-6 | 0x1001D924 `shr 4; and 7` → arg 4; 0x1001D818 | Bm2 version. 1-3 pass silently; 0 and 4-7 print "Bm2 Version Error" (no-op) and carry on. In InitFromHeader: 0/1 → pixels at d+0x39; 2 → DXT only, code at d+0x39; 3 → class byte at d+0x39, pixels at d+0x3D; above 3 → no pixel pointers | code |
| d+0x00 | | bits 0-3 | not read | not known | |
| d+0x01 | 16 | char[16] | 0x1001D8F8; `CYyTexBase_SetName`; `FindByNameInResource` compares 16 bytes at obj+0x31 | texture name: the key model and zone records use | code |
| d+0x11 | 0x28 | header | 0x1001D9A7: `eax = d+0x11` | block below; pixel data starts after it (`edi = d+0x11+0x28`) | code |
| d+0x15 | 4 (2 read) | u16 | 0x1001D9AF | width → texobj +0x24 | code |
| d+0x19 | 4 (2 read) | u16 | 0x1001D9B7 | height → texobj +0x26 | code |
| d+0x1F | 1 | u8 | 0x1001D9BF | bits per pixel → +0x28. Cases 4, 8, 16, 24, 32 | code |
| d+0x35 | 1 | u8 | 0x1001D9C5 | → +0x29: palette entry size in bits (16/24/32) for 4/8 bpp, and the upload format code | code |
| d+0x39 | 1 / 4 | | version 3: low nibble → texobj +0x2C bits 4-7 (0x1001DA3F..0x1001DA51). Version 2: u32 compared with the codes below | compression class / DXT code | code |

Derived: the 0x28-byte block puts width, height, bit count and the last byte read at the offsets where
`BITMAPINFOHEADER` (wingdi.h) has biWidth (+4), biHeight (+8), biBitCount (+0xE) and biClrImportant (+0x24).
0x28 is that structure's size. The code reads only those four fields, so whether the rest matches is not shown.

**Palette and pixels** (start P = d+0x39, or d+0x3D for version 3; InitFromHeader cases at 0x1001DA74..0x1001DB23):

| bpp | Palette (texobj +0x1C = P) | Pixels (texobj +0x20) | Pixel bytes |
|---|---|---|---|
| 4 | 16 entries × (+0x29/8) bytes (0x20/0x30/0x40 for 16/24/32) | after the palette | (w·h+1)/2 |
| 8 | 256 entries × (+0x29/8) bytes (0x200/0x300/0x400) | after the palette | w·h |
| 16 | none | P | 2·w·h |
| 24 | none | P | 3·w·h |
| 32 | none | P | 4·w·h |

How TexObj_Upload reads pixels (tree):
- **Row order:** it reads source row `h−1−y` for destination row y, so the rows are stored bottom-up.
- **4 bpp:** the high nibble is the left pixel.
- **32-bit source into an A8R8G8B8 surface:** the four bytes are copied unchanged. D3DFMT_A8R8G8B8's memory
  order is B,G,R,A, so the source bytes are B,G,R,A (code + header).
- **24-bit source into 32 bits:** B,G,R are copied and alpha is set to 0x80.
- **Into A1R5G5B5:** `B>>3 | (G&0xF8)<<2 | (R&0xF8)<<7`. The A bit is set for 24-bit sources and comes from bit 7
  of the alpha byte for 32-bit sources.

### 5.2 DXT block

- **Version 2:** at P.
- **Others:** after the pixels, when d+0x00 bit 7 is set and the dword there is nonzero.
- **The code:** a u32 compared with the five values at 0x103511CC / 0x103511E0. Those are the dwords
  0x44585431..0x44585435, i.e. the bytes "1TXD".."5TXD" in the file: the DXT FourCC with its bytes reversed.
  For comparison, d3d8types.h `MAKEFOURCC('D','X','T','1')` is 0x31545844.
- **Accepted only:** index 0 when the byte `CDx+0x910` is set, and index 2 when `CDx+0x911` is set. Indices 1, 3
  and 4 can never be accepted: the loop only tests i == 0 and i == 2 (0x1001DB42..0x1001DB6C).
- **On acceptance:** texobj `+0x29 = 0x80 | index` and `+0x20 = code + 4`.
- **TexObj_Upload then requires** the dword at `+0x20` to be nonzero. It passes `+0x20 + 8` as source memory,
  the dword at `+0x20 + 4` as the source pitch, and `g_texFourccTable[index]` (0x10351C94: D3DFMT_DXT1..DXT5)
  as the source format to `D3DXLoadSurfaceFromMemory` (0x102D2868), with filter 1 (D3DX_FILTER_NONE, d3dx9
  header).

### 5.3 Texture object (CYyTex, 0x48 bytes: `Mem_AllocDefault(0x48)` in 0x1001D800)

| Offset | Type | Set / read at | Used for | Confidence |
|---|---|---|---|---|
| +0x04 | u32 | 0x1001D800 (+1), 0x1001DBD0 (−1, release at 0) | reference count | code |
| +0x08 | ptr | 0x1001D8E9 | owning resource | code |
| +0x0C | char[16] | `CYyTexBase::SetName`; `CYyTexMng::FindByName` 0x1003AC80 compares 16 bytes | name | code (tree) |
| +0x1C, +0x20 | ptr | InitFromHeader | palette, pixels (or DXT block + 4) | code |
| +0x24, +0x26 | u16 | InitFromHeader; TexObj_Create (overwritten from level 0's surface description after upload when w or h is not 1, 2, 4, …, 4096; the read-back calls are at 0x10039E12 / 0x10039E25) | width, height | code |
| +0x28 | u8 | InitFromHeader | bpp | code |
| +0x29 | u8 | InitFromHeader; Create; Upload | 16 / 24 / 32, or 0x80\|DXT index | code |
| +0x2A | u8 | TexObj_Create 0x10039B17 (forced to 1 when > 1 and w or h < 32) | mip level count passed to D3DXCreateTexture | code (writer of the initial value not found) |
| +0x2C | u32 | bits 0-2: 5 when the name's first 8 bytes are "cubemap " (0x1001D99B: `and 0xfffffffd; or 5`; the Hex-Rays text shows `& 0xFFFFFFF8`, which is wrong). Bit 3: `Tex_HeightToNormalMap` for 32-bit sources (Upload). Bits 4-7: compression class. Bit 8: set when the parent chunk id is "effe" and the grandparent's is "syst" (0x1001D8FD..0x1001D91C) | type / flags | code |
| +0x3C | u32 | `TexObj_CreateAndUpload`, `TexObj_Reupload` (tree): 3 after upload | state | code (tree) |
| +0x40 | IDirect3DTexture8* | TexObj_Create | 2D texture. Draw code binds `+0x40` if non-null, else `+0x44` (SkinAndDrawV2, zone draw) | code |
| +0x44 | IDirect3DCubeTexture8* | TexObj_Create (`D3DXCreateCubeTexture`). Upload addresses face `+0x2C & 7` through GetCubeMapSurface (vtable +0x3C) | cube texture | code + header |

### 5.4 The D3D texture that gets created (TexObj_Create 0x10039A80, insn)

Let `class = (+0x2C >> 4) & 0xF` (0x10039B43..0x10039B4E).

| Condition (in this order) | Path |
|---|---|
| class == 9 | uncompressed |
| a DXT block was accepted (+0x29 bit 7 and the dword at +0x20 ≠ 0) | compressed |
| class == 10 | uncompressed if `[0x10456AB0]` (IDB: g_cfgMapCompression) ≠ 0, else compressed |
| class ≠ 0 and class ≤ `[0x10456AD0]` (IDB: g_cfgTextureCompression) | uncompressed |
| otherwise | compressed |

**Uncompressed path** (0x10039B82):
- `+0x29` 16 → `D3DFMT_A1R5G5B5 (0x19)`;
- `+0x29` 24 or 32 → `D3DFMT_A8R8G8B8 (0x15)`;
- through `D3DXCreateTexture(dev, w, h, +0x2A, 0, fmt, D3DPOOL_MANAGED (1), &+0x40)`, or
  `D3DXCreateCubeTexture(dev, w, +0x2A, 0, fmt, MANAGED, &+0x44)` when `+0x2C & 7`;
- any other `+0x29`: nothing is created.

**Compressed path** (0x10039BDD): index = `+0x29 & 0x7F` if a DXT block was accepted; else 0 for 16-bit, 2 for
24/32-bit, 0 otherwise.
- **Index 0:**
  - 2D: `D3DXCheckTextureRequirements(..., D3DFMT_DXT1 (0x31545844), MANAGED)`, then D3DXCreateTexture with the
    values it returns;
  - cube: `A1R5G5B5`.
- **Index 2:**
  - 2D: the same with `D3DFMT_DXT3 (0x33545844)`;
  - cube: `A8R8G8B8`.
- **Other indices:** nothing is created.

**Upload** (TexObj_Upload, tree):
- Non-DXT sources first go into a temporary `D3DXCreateTexture(w, h, 0, 0, F, MANAGED)`. F is:
  - `+0x29` 16 → A1R5G5B5;
  - 24 → R8G8B8;
  - 32 → A8R8G8B8.
- From there, level 0 is copied with `D3DXLoadSurfaceFromSurface(..., filter 1)`.
- Each further mip level is made from the previous one with filter 5 (D3DX_FILTER_BOX, d3dx9 header).

### Not known (Bm2)

- d+0x00 bits 0-3, and the header bytes not read (d+0x11..0x14, 0x1D..0x1E, 0x21..0x34, 0x36..0x38).
- Who sets texobj `+0x2A` initially, and the cube-face bits other than "cubemap ".
- What sets `CDx+0x910` / `+0x911`.
- Why chunk type 0x1B shares the class: the factory writes type 0x1B into `obj+0x24` before the chunk copy
  overwrites it, so the write has no lasting effect that the code shows.

---

## 6. Zone mesh (the world geometry; zone draw is the largest draw-call source, docs/RENDERER.md 8)

**Readers:**
- `ParseZoneMeshData` 0x1016FCD0 (tree; gate PASS with thin evidence, 9 of 100 trials returned). Its one caller
  is 0x101792F0 (call at 0x1017932A), whose caller is 0x10176ED0.
- The draw `0x1017C9C0` (insn; frame, 1,382 samples).
- Which chunk type carries the mesh is not traced.

### 6.1 Header (`a3` = the mesh data pointer passed in)

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| +0x00 | 3 | u24 | `*(_DWORD*)a3 & 0xFFFFFF` | byte size (bounds the decode below) | code (tree) |
| +0x03 | 1 | u8 | `*(a3+3)` | version: ≥ 5 → XOR decode (below); ≥ 2 → group count read from +0x20 (else 9) | code (tree) |
| +0x04 | 1 | u8 | `(unsigned __int8)*(a3+4)` | mesh kind: 1 → strips, 36-byte vertices; 2 → 48-byte vertices, lists, morph frames; other → lists, 36-byte vertices; 3 is assigned at run time (6.3) | code (tree) |
| +0x04 | 4 | u32 high 16 | 0x10170E84 (insn) | 0xFFFF → second decode step (below) | code |
| +0x05 | 1 | u8 | `*(a3+5) ^ 0xF0` | decode key | code (tree) |
| +0x08.. | size−8 | | XOR decode, in place | | code (tree) |
| +0x10 | 16 | 4 × u32 | copied to the mesh object's [2..5] | not traced further | code (tree) |
| +0x20 | 4 | u32 | `a3+32` | group count (version ≥ 2) | code (tree) |
| +0x24 | 24 | 6 × f32 | `a3+36` | three (a,b) pairs: (+0x24,+0x28), (+0x2C,+0x30), (+0x34,+0x38). They build 8 corner points, and half the length of (b−a) is stored as a radius | code (tree) |
| +0x3C + 4i | 4 | u32 | `a3 + 4·i + 60` | offset (from a3) of group i; 0 = absent | code (tree) |

**Decode 1** (version ≥ 5; tree, `ParseZoneMeshData` opening loop):
- `k = c_byteSubstTable[key]` (0x10376AC0), `n = 0`;
- for each byte b from +0x08: `n += 1; t = n + k; b ^= low byte of ((k8 | k8<<8) >> (t & 7)); n += 1;
  k = n + t`, where k8 = k & 0xFF (the tree's `(unsigned __int8)v12`).

**Decode 2** (0x10170E80, insn; only when `u32(+4) >> 16 == 0xFFFF`):
- `half = ((size − 8) & ~0xF) / 2`;
- for each 8-byte block i below `half`, the block is swapped with the one `half` bytes later when bit 0 of
  `T[key] + i·(key + 9)` is set, where T is the byte table at 0x10376DE8.

**Group** (at `a3 + offset`):
- u32 submesh count;
- 24 bytes copied;
- u32 at +0x1C;
- submeshes from +0x20.

**Submesh:**

| Offset | Size | Type | Read at | Used for | Confidence |
|---|---|---|---|---|---|
| +0x00 | 16 | char[16] | `Util_Copy16Bytes`; looked up in the texture manager (`sub_1003AD40`, then `sub_1003ACD0`) | texture name (5.1) | code (tree) |
| +0x10 | 4 | u32 bits 0-27 | `& 0xFFFFFFF` | vertex count | code (tree) |
| +0x10 | | bits 28-31 | stored as four booleans in the run-time record (words +0x20..+0x26) | not traced | code (tree) |
| +0x14 | count × stride | vertices | stride 36 (kinds 0, 1, 3) or 48 (kind 2) (`v20[10]`) | below | code (tree) |
| after | 4 + 2·m, padded to 4 | u32 m, u16[m] | `v43[4]`, `v43[5]` | index count and 16-bit indices. Primitives = m−2 for kinds 1/3, m/3 otherwise | code (tree) |

### 6.2 Vertex layouts and the buffers made from them

Usage and pool are read from `CDx+0x924` / `CDx+0x928` at run time.

- **Kinds 0, 1 and 3 (36-byte vertices).**
  - Each index's 36-byte vertex is copied, in index order, into a VB of `36·m` bytes created with
    FVF `[mesh+8]` = 338 = 0x152 = `D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1`.
  - The draw sets `SetVertexShader(0x152)` for kinds 0 and 1 (0x1017CE83, jump table 0x1017DE0C) and stream 0
    with stride `[mesh+0x24]` = 36 (0x1017DD88).
  - So, by D3D8's FVF order (d3d8types.h), the 36 bytes are: position (3 f32), normal (3 f32), diffuse (u32),
    texcoord (2 f32). Code + header.
- **Kind 2 (48-byte source vertices; `g_cfg0011` 0x10456AC0: 0 → 1 frame, 2 → 32, other → 16).**
  - Stream 1: a VB of `24·count` bytes gets source bytes +24..+47 of each vertex.
  - Stream 0: `frames` VBs of `12·count` bytes each get `pos + (f/(frames−1))·delta`, with pos = source
    +0..+11 and delta = +12..+23 (the tree's arithmetic).
  - An INDEX16 IB copy of the indices.
  - The draw (kind 2, 0x1017CE38) sets the fixed-function shader at `zone+0x39454`. That is declaration A
    below, whose stream 1 is NORMAL FLOAT3, DIFFUSE D3DCOLOR, TEXCOORD0 FLOAT2, 24 bytes. It binds stream 0
    stride 12 (0x1017DD18), stream 1 `[sub+0xB0]` stride 24 (0x1017DD34) and indices `[sub+0xB8]`, then
    DrawIndexedPrimitive.
  - So the 48-byte vertex is position (12), delta (12), normal (12), diffuse (4), texcoord (8). Code + header.
- **Kind 3.**
  - A kind-1 mesh becomes kind 3 when `g_pZoneMap+0x39498` is nonzero, `a5` is nonzero, and every submesh's
    texture name is found by `sub_1003ADC0(..., a5)`. Its constants are then `[7..11] = 3, 0x152, 36, 36, 5`.
  - Besides the 36-byte VB, it builds a second VB of 144 bytes per non-degenerate strip triangle. Each 48-byte
    vertex in it is the 36-byte vertex plus 3 floats from `sub_10170B30`.
  - The draw takes the programmable path (0x1017CEBC: constants c15..c17 and the shaders at `zone+0x3945C..`).
  - The programmable declaration (section 9) is 48 bytes: v0, v3, v5, v7 as above plus v15 FLOAT3. Binding
    this VB to that shader was not traced.

### Not known (zone)

- The chunk type, and the 16 bytes at +0x10.
- The group's 24 bytes and u32.
- Submesh bits 28-31.
- What `sub_10170B30` computes.
- Tables 0x10376AC0 / 0x10376DE8 are read as data but not described here (game content).

---

## 7. Dancer (`sq*`) library formats

**Readers** (all tree):
- `sqgxShape` 0x1028AB50 / 0x1028AFE0;
- `sqgxShapeIndexBufferBegin` 0x1028AA70;
- `sqgxVertexShaderCreate` 0x10284950, `sqgxVertexShaderSelect` 0x10284A80;
- `sqgxTextureAdd` 0x10281830, `sqgxTextureCreateFromImage` 0x10281510;
- `sqmdDMB` 0x10288BB0..0x10288E00.

docs/RENDERER.md 7 found no sqgx display-list draws in the in-game capture, so this path's frame relevance is low.

**Dynamic streams** (`g_pSqgxContext`; flags at dword [652] = byte +0xA30, per-texcoord flags at [653+j]):

| Flag | Buffer | Created as | Bound as | Declaration token (sqgxVertexShaderCreate) | Confidence |
|---|---|---|---|---|---|
| [652] bit 0 | [598] | `CreateVertexBuffer(12·3072, 0x208, D3DFVF_XYZ (2), DEFAULT)` | stride 12 | `v0 ← FLOAT3` (0x40020000) | code + header |
| [652] bit 1 | [600] | `12·3072`, FVF `D3DFVF_NORMAL (0x10)` | stride 12 | `v3 ← FLOAT3` | code + header |
| [653+j] bit 2 | [602+j] | `8·3072`, FVF `D3DFVF_TEX1 (0x100)` for j = 0, `D3DFVF_TEX2 (0x200)` for j > 0 | stride 8 | `v(7+j) ← FLOAT2` | code + header |
| [652] bit 4 | [606] | `12·3072`, FVF 0 | stride 12 | `v15 ← FLOAT3` (D3DVSDE_POSITION2) | code + header |
| [652] bit 3 | [610] | `4·3072`, FVF 0 | stride 4 | `v5 ← D3DCOLOR` (D3DVSDE_DIFFUSE). The declaration adds it when [653] bit 3, or [654] bit 3 with [710] > 0 (not the same test as the binding) | code + header |

- Index buffer: `CreateIndexBuffer(2·3072, 0x208, D3DFMT_INDEX16, DEFAULT)` (0x1028AA70).
- Each enabled element gets its own stream, numbered in this order. The declaration is built the same way and
  cached by its flag pair (`sqgxVertexShaderSelect`).

**Textures** (`sqgxTextureAdd`), by format kind:
- 4 → `D3DFMT_A8R8G8B8 (21)`;
- 3 → `X8R8G8B8 (22)`;
- 2 → `A8L8 (51)`;
- 1 → `L8 (50)` when `[180]` is set, else `P8 (41)`.

How they are created:
- **With no image:** `CreateTexture(w, h, 1, D3DUSAGE_RENDERTARGET, fmt, DEFAULT)`.
- **From an image** (`sqgxTextureCreateFromImage`): `CreateTexture(w, h, 1, 0, fmt, MANAGED)`. For kind 3,
  source bytes `R,G,B` (3 per pixel) are written as B,G,R into the 4-byte texel.
- **Palettes** (`sqgxTextureClutAdd`): 256 entries of 3 or 4 bytes, the 4th forced to 0 for 3-byte entries.
  They go to `SetPaletteEntries(slot 1..255)`.

**DMB string and matrix areas** (`SqmdDMB_ResolveMatrices` 0x10288BB0, `_ResolveStrings` 0x10288C20):
- Slots of the model's table `+0x50` holding 0xEFBEDDBA receive consecutive 64-byte blocks. The table has
  max(`+0x58`, `+0x5C`) slots.
- Then `+0x7C` "tag" strings and `+0x90` "file" strings follow: NUL-terminated, each padded to 4 bytes. Their
  pointers go to the tables at `+0x70` and `+0x84`; unused slots up to `+0x80` / `+0x94` are zeroed.
- All code (tree).

### Not known (dancer)

- The DMB file's own header: these functions take an already-allocated model.
- The sqim image header.
- How the 16-byte-aligned layout (`SqmdDMB_AlignMem`) maps to file offsets.

---

## 8. Other vertex and texture formats the code creates

**Shared vertex buffers** (`CreateSharedVertexBuffers` 0x10029A50, tree; static). Usage 0x208, pool
`[0x10457540]`; FVFs decoded with d3d8types.h:

| Global | Bytes | FVF | Stride (derived from the FVF) | Vertices (derived) |
|---|---|---|---|---|
| 0x10457148 | 28672 | 0x52 XYZ\|NORMAL\|DIFFUSE | 28 | 1024 |
| 0x1045714C | 36864 | 0x152 XYZ\|NORMAL\|DIFFUSE\|TEX1 | 36 | 1024 |
| 0x10457150 | 0x8000 | 0x112 XYZ\|NORMAL\|TEX1 | 32 | 1024 |
| 0x10457154 | 0x4000 | 0x112 | 32 | 512 (the Os2 kind-1 ring, 2.6) |

**DrawPrimitiveUP vertex formats** (tree, all functions):
- In every function that passes a constant FVF to SetVertexShader and a constant stride to `CDx_DrawPrimitiveUP`,
  the stride equals the FVF's size.
- Pairs found, with example functions:
  - 0x144 `XYZRHW|DIFFUSE|TEX1`, 28 bytes: 12 functions, e.g. 0x1011BBD0, 0x1021D4C0;
  - 0x44 `XYZRHW|DIFFUSE`, 20 bytes: 5 functions, e.g. `CDx::DrawFadeQuad` 0x1000A820;
  - 0x142 `XYZ|DIFFUSE|TEX1`, 24 bytes: 5 functions, e.g. 0x10182F10;
  - 0x42 `XYZ|DIFFUSE`, 16 bytes: 5 functions, e.g. `Dbg_DrawAxesAt` 0x100266C0.

**Font glyph cache:**
- `FontGlyphCache_CreateTexture` 0x1025D760 (tree) is called at 0x1025CB71 (insn) with
  `(0x400, 0x400, 1, 0, D3DFMT_A4R4G4B4 (0x1A), D3DPOOL_MANAGED)`: one 1024×1024 A4R4G4B4 texture.
- Its vertex buffer (0x1025D7B0, called at 0x1025C7B3) uses usage 0x208 and pool DEFAULT. The FVF is a register
  value at that site (not known statically).

**Render targets** (`CDx::CreateTexture` 0x1000A6E0, tree):
- `CreateTexture(w, h, 1, usage, fmt, pool)` is tried with fmt `A8R8G8B8 (21)`, then `A1R5G5B5 (25)`,
  `X8R8G8B8 (22)`, `X1R5G5B5 (24)`.
- Last, it tries the default back buffer's format (from `GetDesc`, surface vtable +0x20).
- Callers in the tree pass usage 1 (`D3DUSAGE_RENDERTARGET`) with pool 0 (DEFAULT), for example:
  - the occlusion target 16×16 (`CMoOcclusionMng`);
  - 256×256 capture targets (`unit_10038930`);
  - viewport-sized translucent targets (`unit_1001B890`).
- `unit_100014A0_Gfx` passes usage 0 and pool 1.

---

## 9. Shaders and vertex declarations

There are 7 `CreateVertexShader` sites and 1 `CreatePixelShader` site (docs/KIT.md "The static catalogue";
`data/shader_catalogue.json`). The declarations they pass:

| Site | Function | Declaration | Decoded (vs_decl.py / tree) | Code |
|---|---|---|---|---|
| 0x1002D6DC | `CYyOs2VtxBuffer::Constructor` 0x1002D680 | 0x10351628 | s0: v0 FLOAT3 · s1: v3 FLOAT3 · s2: v7 FLOAT2 | none (fixed function) |
| 0x101792BE | `ZoneMap_CreateVertexShaders` 0x101792A0 → zone+0x39454 | 0x103772E8 (A) | s0: v0 FLOAT3 · s1: v3 FLOAT3, v5 D3DCOLOR, v7 FLOAT2 | none |
| 0x101792DF | same → zone+0x39458 | 0x10377304 (B) | s0: v0 FLOAT3, v3 FLOAT3, v5 D3DCOLOR, v7 FLOAT2 | none |
| 0x10284A58 | `sqgxVertexShaderCreate` 0x10284950 | built on the stack (section 7) | one stream per enabled element | none |
| 0x101772A7 | `Gfx_AssembleVertexShader` 0x10177240, from `ZoneMap` 0x10178250 | built on the stack: s0 v0 FLOAT3, v3 FLOAT3, v5 D3DCOLOR, v7 FLOAT2, v15 FLOAT3 (docs/KIT.md) | 48 bytes per vertex | 5 vs.1.x shaders assembled by the game's D3DXAssembleShader from source-line tables 0x10376FF8, 0x10377048, 0x103770D0, 0x10377168, 0x10377210 (19/33/37/41/45 lines) |
| 0x10177323 (pixel) | `Gfx_CompilePixelShader` 0x101772E0 | — | — | 1 pixel shader, table 0x103772C4 (9 lines) |
| 0x10017810, 0x101771B0 | `Shader_CreateVertexShader`, `Gfx_CompileVertexShader` | caller's | — | not reached (docs/KIT.md); 0x101771B0 assembles `c:\test.vsh` |

- **Register numbers** in the fixed-function declarations are D3DVSDE_* (d3d8types.h):
  - 0 POSITION;
  - 3 NORMAL;
  - 5 DIFFUSE;
  - 7 TEXCOORD0;
  - 15 POSITION2.
- **Shader source and tokens** are game content and stay out of the repo (`runs/shaders/`, docs/KIT.md).
- **FVF codes the game passes to `SetVertexShader`:** tree immediates 0x144, 0x142, 0x44, 0x42, plus
  insn 0x112 (0x10022CFD) and 0x152 (0x1017CE83). Their sizes and layouts are in sections 6 and 8.

---

## 10. Effects and scenes

What the code shows:
- **Effect resources are chunk types with classes of their own** (section 1). Two examples:
  - type 0x05 constructs `CMoGeneratorClone_VT_1032B114` (0x10047F80);
  - type 0x54 prints "Afb Version Error".
- **Their setup functions** (slot 8: 0x10054480, 0x1003FEE0, 0x1003E6B0, 0x1003F420, 0x10063C50, 0x10071A20,
  0x1001DF60, 0x10038920) were not read for this document.
- **The element classes** in the tree (`CMoD3aElem`, `CMoD3mSpecularElem`, `CMoDist*Elem`, `CMoLfdElem`, …) read
  their parameters from run-time objects, not from chunk data in the functions present. The one with a draw,
  `CMoDistRingElem`, draws strips with `CDx_DrawPrimitiveUP(..., 5, ...)` at the stride
  `Gfx_CalcFVFStride(0x152)` (tree, CMoDistRingElem.cpp lines 151-152).
- **`sqsnScene*`** (tree) walks run-time scene structures (layers, items). No function in the tree parses a
  scene file: no FourCC or magic compare was found in `sq*`, `mdl`, `chn` or `StModel`.

### Not known (effects, scenes)

- The data layout of every effect chunk type.
- The meaning of chunk types without a version string.
- Any scene file format.

These need the setup functions above read the way sections 2-5 were.
