# Fewer character draws: merging SkinAndDrawV2's strips (task 78)

What is established, what the merge needs, and the steps. Every fact names its source; the open points are listed
as open.

## Facts

**How the draws are issued** (tree/src/CYyModelDt.cpp, CYyModelDt::SkinAndDrawV2; gate PASS, and 937 of its 1,596
instructions covered by captured plaza calls):
- It walks the model's command list:
  - a `0x5453` record is one triangle strip, drawn with `CDx_DrawIndexedPrimitive(type 5, 0, numVerts, startIndex,
    primCount)`;
  - a record with bit 15 set is a material: `v14 == 0` sets the texture (SetTexture only if it differs from the last
    one this call set) and TSS 0 ARG1s; `v14 == 1` sets the lights (CDx_SetLight for each entry of the light table),
    AMBIENT, TEXTUREFACTOR and stage 1's state.
- When `field_34 & 1`, each strip is drawn twice: from streams `v10+68/+72`, then `v10+80/+84`.
- Per strip, three optional passes may draw it again: EnvMap_BeginPass when `g_envMapEnabled`, ProjTex_BeginPass when
  `g_projTexEnabled`, Model_BeginOffscreenPass when it returns non-zero.

**The plaza trace** (session 2, process 5180, label 0, 9 frames; kit/d3dprof/merge_potential.py and the scripts in
HISTORY.md):
- 1,620 DrawIndexedPrimitive a frame, 326 models.
- Every one of them with ALPHABLENDENABLE 0; 98% with ZWRITEENABLE 1. ZFUNC is never set in those frames; D3D8's
  documented default is D3DCMP_LESSEQUAL.
- Bounds on what merging leaves:
  - in place, same state: 1,525 (`--same-value`);
  - within runs that have no state call between them (only streams, indices, shader): 1,330;
  - regrouped by state, lights and transform within each model: 764;
  - with one shared skinning buffer as well: 510.

**Where the indices are** (IDA listing and tree/src/CYyOs2VtxBuffer.cpp):
- `v10` is a CYyOs2VtxBuffer: the index buffer at +0x40 (the `v10 + 64` that SkinAndDrawV2 binds), its vertex shader at
  +0x64 (`v10 + 100`), the flag byte at +0x68 (`v10 + 104`).
- The index buffer is made in CYyOs2VtxBuffer::CreateBuffersStep (0x1002CCB0) with usage 8 (D3DUSAGE_WRITEONLY) and
  D3DPOOL_DEFAULT, `2 * indexCount` bytes. So it cannot be read back.
- Its contents are written by sub_1002CE20, which:
  - locks the index buffer and the vertex buffer at +0x4C;
  - walks the command list from a cursor at +0x7E;
  - writes indices from a running position at +0x82;
  - assigns a new vertex slot (counter at +0x60, table at +0x7A, the array at +0x5C) when a vertex needs other texture
    coordinates than its slot holds.
  The indices are therefore an output of that walk, and the game keeps no copy of them (+0x5C is its working table,
  filled with 0xFFFF at creation).

## What a merged draw needs

Strips cannot be joined as one strip without degenerate indices, so a merged draw is an indexed triangle list built
from the strips' indices. That needs the indices on the CPU. Two ways, each to be checked before use:
1. **Capture them as they are written.** Hook sub_1002CE20 (or the index buffer's Lock / Unlock): give the game a
   memory copy to write into, then copy it into the real buffer at Unlock. The copy is then exactly what the game
   wrote, with no assumption about its logic. sub_1002CE20 is not in the kit's manifest (it is outside the exported
   graphics set), so it cannot be hooked by name. The route is the index buffer's own Lock / Unlock, for the buffers
   CreateBuffersStep makes: its CreateIndexBuffer call is identifiable by its return address. CreateBuffersStep itself
   is in the manifest (flag 4: not yet run in game; trampoline). Open: whether any other path writes those buffers.
2. **Make the buffer readable.** Create it in D3DPOOL_MANAGED without WRITEONLY (a fork of CreateBuffersStep's
   arguments), and read it back once it is complete (`+0x82 == indexCount`). Open: whether a managed buffer costs or
   saves time here (an A/B decides), and the Reset path (the game releases default-pool buffers on Reset).

Either way the merged list for a model is cached by (index buffer, its creation serial from CreateIndexBuffer), so a
buffer made again at the same address is never mistaken for the old one.

## The steps

1. **Exact merge** (no reordering of state changes): within a run of strips with no state call between them, draw
   all of the run's strips from streams A as one list, then all from streams B. The only reordering is a strip's B
   draw relative to a later strip's A draw, both opaque, under LESSEQUAL. Measured bound: 1,620 to 1,330.
   - Strips with an envmap, projected-texture or offscreen pass active are drawn as before.
   - A material record whose every call sets the value this call already set (tracked within the call) is a no-op,
     so the run continues across it. That moves toward the 764 bound without reordering any real state change.
2. **Checks:**
   - the gate, with the draw change declared (fork.py gate --declare; everything else must match the game);
   - the kit's picture comparison in game (trace_pictures, pictures.py): the frame with and without the fork;
   - the owner's eyes;
   - then speedfix's lever A/B in the plaza.
3. Only if 1 and 2 hold: regrouping across real state changes (the 764 bound), then a shared skinning buffer (510).
