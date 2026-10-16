# Fewer character draws: merging SkinAndDrawV2's list records (task 78)

What is established, what the merge does, how it is checked, and what is open. Every fact names its source.

## Facts

**How the draws are issued** (tree/src/CYyModelDt.cpp, CYyModelDt::SkinAndDrawV2; gate PASS):
- It walks the model's command list. A `0x54` record is a triangle list: `CDx_DrawIndexedPrimitive(4, 0, numVerts,
  v9, v26)`, then `v9 += 3 * v26` (the `LABEL_145` step). Consecutive list records therefore draw consecutive ranges of
  the same index buffer. A `0x5453` record is a strip (type 5).
- A record with bit 15 set is a material: `v14 == 0` sets the texture and TSS 0 ARG1s; `v14 == 1` sets the lights,
  AMBIENT, TEXTUREFACTOR and stage 1's state.
- When `field_34 & 1`, each record is drawn twice: from streams `v10+68/+72`, then `v10+80/+84`.
  The second draw is a mirrored copy: 0x1002D340 builds the `+0x6e` array from `+0x76` by multiplying each vertex
  through a sign table at 0x103515E8 (1, 1, 1, 0, -1, 1, ...), one axis negated (disassembly; first named in an outside
  write-up, HISTORY.md 11 Oct).
- Per record, three optional passes may draw it again: EnvMap_BeginPass when `g_envMapEnabled`, ProjTex_BeginPass when
  `g_projTexEnabled`, Model_BeginOffscreenPass when it returns non-zero.
- **Model_BeginOffscreenPass (0x10033800), disassembled:** it calls 0x10027990 (`mov eax, ecx / ret`) twice, then
  `cmp word [0x1045B77A], 1 / jne`, `fld [0x1045B80C] / fcomp [0x1032A3A0]` (4.0) `/ test ah, 5 / jp`, and
  `[esi+8] == 0 / je`. All three exit to 0x10033A4A: two calls to 0x100279A0 (`ret`), `xor al, al`, `ret 8`. So unless
  `g_actorShadowMode == 1`, `g_shadowCasterDistance < 4.0` and `[this+8] != 0`, it returns 0 and writes nothing
  outside its own frame.

**The plaza trace** (session 2, process 5180, label 0, 9 frames):
- 1,620 DrawIndexedPrimitive a frame. The primitive type is the first argument after the trace's argument count; 97%
  are type 4 lists. (An earlier reading took the count, 5, for the type: the draws are not strips. HISTORY.md, 6 Oct.)
- Of those, 1,577 a frame are SkinAndDrawV2 list draws: a type-4 DIP directly after a SetIndices whose call site is in
  SkinAndDrawV2 (0x1002E71E). That is 916 records, 656 of them drawn twice.
- Every one with ALPHABLENDENABLE 0; 98% with ZWRITEENABLE 1. ZFUNC is never set in those frames; D3D8's documented
  default is D3DCMP_LESSEQUAL.
- **What the fork's rule leaves:** 1,330 a frame (about 247 fewer draws, 15%). Consecutive list draws merge when only
  shader / stream / index calls lie between them, with the same index buffer, minIndex and numVerts, and a start index
  that follows on. This is an upper bound: records that make no device call (which the fork also stops at) are not
  visible in a trace. The script is in HISTORY.md, 6 Oct.
- Earlier bounds that count more than the fork does:
  - regrouped by state, lights and transform within each model: 764;
  - with one shared skinning buffer as well: 510.

**Where the indices are** (IDA listing, tree/src/CYyOs2VtxBuffer.cpp):
- `v10` is a CYyOs2VtxBuffer: index buffer at +0x40, vertex shader at +0x64, flag byte at +0x68. The index buffer is
  D3DUSAGE_WRITEONLY in D3DPOOL_DEFAULT (CreateBuffersStep 0x1002CCB0). It is filled by sub_1002CE20, which is not in
  the kit's manifest. The game keeps no CPU copy (+0x5C is a working table).

## No index capture needed

The first plan assumed strips, which cannot be joined without new indices, so it needed the indices on the CPU (a
capture of the index buffer, or a readable buffer). The records are lists, and consecutive list records already sit
next to each other in the index buffer (`v9 += 3 * v26`). A run of them is one range, `[first, first + 3 * prims)`,
which the game's own buffer draws as one DrawIndexedPrimitive. So the merge reads no indices and writes no buffer.
The capture is only needed if the regrouped bound (764) is pursued, since that reorders records.

## The fork (kit/speedfix/fork/CYyModelDt__SkinAndDrawV2.fork.cpp)

A `0x54` record is held, its draw not issued, when all of these hold:
- the merge is switched on (`xi_skinmerge_on`, speedfix's `skin_merge`);
- the record would draw (`!v29`, the flag byte at +0x68);
- `g_envMapEnabled == 0` and `g_projTexEnabled == 0`;
- Model_BeginOffscreenPass's guard is false (the same three tests, so it would return 0);
- it starts where the held run ends;
- the run stays at 256 records or fewer and at most 65,535 primitives. That cap is the MaxPrimitiveCount speedfix logs
  in the owner's runs.

A held record:
- adds to the game's primitive and draw counters as its draws would have;
- skips the pass block. EnvMap_BeginPass and ProjTex_BeginPass are not reached with their globals 0, and
  Model_BeginOffscreenPass would return 0 (above).

The run is drawn as the game draws one record: the shader, streams 0 to 3, SetIndices, then DIP(4, 0, numVerts, first,
prims), and when `field_34 & 1`, streams 0 and 1 from +80/+84 and the DIP again. It is drawn at the first record that
is not a held list (a material, a strip, any other record, a record that fails the test) and before the function
returns.

The one reordering: a record's B draw (streams +80/+84) moves after a later record's A draw. All are opaque, under
LESSEQUAL (above).

## Checks (kit/speedfix/test/test_skinmerge.py)

1. **Merge off:** `fork.py gate`, no declarations. PASS: the fork is the game's function.
2. **Merge on**, a copy built with `XI_SKINMERGE_GATE`: `fork.py gate --declare fork/skinmerge.declare.json`. The
   declarations allow:
   - calls to Gfx_SetVertexShaderCached, Gfx_SetStreamSourceCached, CDx_DrawIndexedPrimitive and SetIndices to be
     dropped, added or changed;
   - calls to Model_BeginOffscreenPass to be dropped.

   PASS, but the fuzzed inputs never reach a held record (no trial shows a declared difference), so:
3. **Merge on, replayed from the owner's captures** (pipeline/xcap.py; CAPTURES=dir), with Model_BeginOffscreenPass
   running as the game's own code. The gate's stub answers with any value, including "pass active" in a trial whose
   memory makes its guard false. The original then takes a path the game's code cannot (seen: capture 11952_2, a
   0x1002F1A0 call after the stubbed Begin answered non-zero). With the real code:
   - 11952_1, 11952_3 and 5180_2 PASS;
   - 11952_2 PASS, with a merged draw seen;
   - 5180_1 and 5180_3 INCONCLUSIVE: their command lists run past the captured pages.
4. **The speedfix host test** (test_speedfix.py, Wine): with skin_merge=1 at the start, SkinAndDrawV2's entry jumps
   into speedfix.dll; with it off, the entry is the game's code.

Still open:
- What a merged draw saves in time. The owner's A/B (`ab_levers=skin_merge`) decides.
- Whether the picture is the same. The kit's picture comparison of a frame with and without, and the owner's eyes.
- The regrouped bound (764), which needs the indices (above) and an argument about reordering state changes.
