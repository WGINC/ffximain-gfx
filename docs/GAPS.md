# Gaps: what stands between us and changing any part of the pipeline on knowledge, not trial

The goal (owner, 6 Oct): no more poke, guess and check. For any part of the graphics pipeline we should be able
to say:
- **Know:** what it does, from code that was checked against the original.
- **Change:** that we can change it, by replacing or editing it at a known place.
- **Predict:** what the change will do, before trying it in the game.
- **Confirm:** that it did that and nothing else, in the game.

This document measures where each of those fails today and names the tool or work that closes each gap. All
counts are from the repo's data as of 6 Oct; the source is named with each.

## 1. Where we stand, in numbers

| Measure | Value | Source |
|---|---|---|
| Graphics functions exported from IDA | 867 | data/exports/gfx_export.jsonl |
| Of those, verified in the tree | 774 PASS (plus 2 INCONCLUSIVE, 1 FAIL in the tree) | verification/gate_clean_final.jsonl |
| Exported but not in the verified tree | 90: 48 D3DX library, 13 FAIL, 11 constructs, 9 compile, 8 UNRESOLVED, 1 INCONCLUSIVE | docs/RESIDUE.md |
| Functions outside the export that use the device | 0 | data/gfx_device_users.json |
| Functions outside the export that call graphics code | 267: tier 1 (named in a graphics family, or calls D3DX) 43, tier 2 (unnamed, calls 2 or more) 62, tier 3 (calls exactly one) 162 | data/gfx_extension.json |
| Instructions of the tree's functions the gate's trials ran | 63.3% (46,960 of 74,133) | verification/coverage_clean.json |
| Tree functions with every instruction run | 620 of 777 | same |
| Swapped functions that ran in one 25-minute session | 236 of 619 | xikit_calls_11000.csv |
| Hookable by a mod | 774 (721 with a trampoline to the game's code) | kit manifest |
| Functions named only by address or vtable slot | 346 of 777 | tree/placement.json |
| Class fields named / unnamed (`field_XXX`) | 86 / 720, plus 528,778 bytes of padding not yet described | tree/include |
| Virtual calls through an untyped slot in the tree | 810 | pipeline/artifact_count.py |
| Memory dumps the gate runs on | 4 (town, town2, combat, zonetoinn) | verification/live |

## 2. Know: what the code does

### 2a. Code nobody has checked

**The hot paths are mostly unverified.** Instructions the gate's trials never ran:

| Function | Instructions | Never run | Its role |
|---|---|---|---|
| Frame_Render | 1,424 | 1,371 | The frame (RENDERER.md section 2) |
| CYyModelDt::SkinAndDrawV2 | 1,596 | 1,534 | Character skinning and drawing; the top caller in capture 11000 |
| fn_1017C9C0 | 1,403 | 941 | The zone mesh |
| TexObj_Upload | 1,077 | 917 | Texture upload |
| CYyModelDt::PrepareRenderState | 823 | 786 | Model render state |
| Cell_DrawQuadBlended | 597 | 475 | UI quads |
| CMoSchedularTask::Interpret | 7,419 | 5,816 | The effect scheduler's interpreter |

A PASS says nothing about code no trial reached. The cause is the inputs: fuzzed objects and four dumps rarely
hold a loaded model, a full zone or a busy scheduler at the moment a function starts.

**What closes it: an in-game input recorder (to build).**
- xikit writes a process dump at the entry of a chosen function, when the game calls it, with that thread's
  registers. It can be once per function or once per new path through it.
- The gate's live mode already runs a function from a process dump of the real game (pipeline/live_state.py),
  so each recorded call becomes a gate case with real inputs.
- That gives a definitive comparison, original against reconstruction, on exactly the states the game produces.
  It needs no new kind of evidence.

### 2b. Code outside the verified tree

- **8 UNRESOLVED.** Each is blocked at one or two calls whose pop count has no definitive source. Among them:
  - the occlusion readback 0x1006C8B0, at 0x1006C8CD;
  - CDx_PushRenderTargetTexture, at 0x10009F44;
  - sqgxSprite, at 0x102811DC.

  Most are Direct3D calls on texture or surface objects whose interface is not proven at that site. The in-game
  DLL's gate leaves two more for fuzz (0x10003890, 0x100CB4E0).

  **Closes it: an in-game fact recorder (to build).** For a named call site, the kit records ESP before and after
  the call while the game's own code runs. CLAUDE.md accepts "the original running real game code" as a source
  (pipeline/real_sp.py, trusted calls only). The same recorder can note which method a COM call reached, from the
  vtable entry it jumped through.
- **13 FAIL.** 10 have x87 comparisons Hex-Rays left as unassigned condition variables (README). The others
  differ at a stubbed call. Each needs its comparison read from the instructions, and a fix in the translator
  or an hx_patch.
- **11 constructs** C++ cannot express: `__userpurge`/`__usercall` callees, an `__asm` block, the x87 status
  word. Each needs a hand-written assembly shim with the exact register contract, checked by the gate.
- **9 compile failures** (translator gaps; named in RESIDUE.md).
- **The tree's own 3:**
  - `Post_DrawRotatingGhostLayers`: a FAIL on -0.0 against +0.0;
  - sub_10016320 and sub_1006B3B0: INCONCLUSIVE, the original never returns under the gate's inputs. The input
    recorder (2a) gives them real inputs.

### 2c. The part of the pipeline outside the export

The device is only used inside the export, but **what gets drawn is decided outside it**, by 267 functions that
call into graphics code (data/gfx_extension.json). Among them:
- the world update and draw path the frame's state machine calls (sub_1018B130, sub_1018C2B0,
  StDancer_Update, ...; RENDERER.md section 2);
- the menu layer that leads to the UI draws (UI.md: not reached by direct calls);
- asset loading and decoding: where textures, models and fonts come from (TexObj_Upload is in the tree; the
  DAT readers are not).

**Closes it:** export these from IDA in tiers (tier 1 first), then translate, gate and add them to the manifest,
as was done for the 4 Oct export.

### 2d. Code we can read but not understand

- 346 of 777 functions have only an address or slot name.
- 720 fields are unnamed, against 86 named.
- 810 virtual calls go through an untyped slot.
- 32,469 uses of Hex-Rays' numbered variables remain (`vN`, `aN`).

A change to such code is reasoned from offsets and arithmetic, which is where guessing creeps back in.

**Closes it:** the de-artifacting plan (docs/DEARTIFACT.md), fed by two new kinds of evidence the kit can record:
- the targets of virtual calls observed in the game (which function each slot reached, per call site). This is
  observed, so it is labelled as such, as RENDERER.md does;
- field values seen in the game (which fields change per frame, which hold pointers to which classes).

### 2e. What the GPU is asked to do

- **Shaders.** The vertex shader assembly text is in the binary (`vs.1.1` source near CTsZoneMap's strings,
  file offset 0x376250). Seven CreateVertexShader sites exist. The catalogue below now maps each shader to its
  source, declaration and builder. A port and any shading change start there.
- **State per draw.** RENDERER.md lists which states are set where. Nobody has recorded the full device state at
  each draw, so "what does this draw look like to the GPU" is reconstructed by reading code paths.
- **Resources.** There is no catalogue of textures, buffers and render targets: format, size, who creates them,
  who reads them.

**Closes it:**
- a shader catalogue from the binary and the 7 creation sites (static, definitive). **Built** (6 Oct,
  `kit/d3dprof/shader_catalogue.py`, `data/shader_catalogue.json`, docs/KIT.md): 5 vertex shaders and 1 pixel shader,
  assembled by the game's own D3DX in Unicorn; the other creation sites are fixed-function or unreachable. The
  runtime half: every vertex shader the game creates is saved as created, decoded and named against it (`vshader.py`);
- a per-draw state snapshot in the trace. **Built:** ` s=` per draw, `#S` per state, the known states only;
  `trace_diff.py` names the states a finding's draw differs in;
- a resource log (creation calls with their parameters and the creating function). **Built:**
  `xikit_resources_<pid>.csv`.

## 3. Change: where and how we can intervene

| Kind of change | Possible today | Missing |
|---|---|---|
| Replace a whole function | Any of the 774 manifest functions, from a mod, with its type checked at compile time | The 90 exported functions outside the tree; the 267 callers outside the export; every other game function |
| Call the game's own code | 721 functions through a trampoline, all reconstructed ones through their game entry (`xigame::`), and the game functions the tree calls (`orig::`) | Typed access to game functions the tree does not call |
| Edit a function slightly | **Built:** a fork (`sdk/fork.py`, `XI_FORK`): the function's gate-verified source, edited, compiled into the mod with the tree's flags; `fork.py diff` shows the edit, `fork.py gate` runs it against the game's code | Nothing for the 777 tree functions; the functions outside the tree have no source to fork |
| Change device-level behaviour (state filtering, batching, timing) | **Built:** `XI_DEVICE_HOOK`, in front of the profiler and trace, typed from d3d8.h's order, the slot checked against the kit's table | Not run in the game yet |
| Change data (textures, models, fonts) | No | The asset path (2c), then an override API |
| Iterate quickly | **Built:** `mod_reload=1`: a rebuilt mod is loaded again while the game runs (its hooks off, `xikit_mod_unload`, the new build's init) | Not run in the game yet |

**Hooking beyond the manifest** needs the same proof of a safe entry the manifest gives:
- the entry bytes are known;
- the first 5 bytes are inside the function;
- nothing jumps into them;
- there is a definitive prototype: the IDB's typed prototypes, or the tree's declaration.

make_manifest.py already does the entry analysis for the whole image's branch targets. It can be run for any
function whose prototype is definitive.

## 4. Predict: knowing what a change will do before running the game

**What exists:**
- the gate, which compares a function with the original in an emulator;
- the alternating trace, which compares the calls the game makes.

- **Built (6 Oct): the differential gate with declared differences** (pipeline/declared.py; `fork.py gate
  --declare`). A changed function runs against the original on fuzz or a dump; each difference must be on the
  change's list, which can name calls dropped, added or changed (by Direct3D method or game function, with
  argument conditions), memory, and the return value. The report counts each declaration seen and names the
  declarations never seen. In this mode each stubbed call's answer depends on the call itself, so a dropped call
  does not shift the answers of later calls. It runs on forks today; a replacement written from scratch in a mod
  has no gate build yet.

**Missing:**
- **The gate for a mod's own replacement functions** (not forks): the mod DLL imports Windows and the C library,
  which the gate's loader does not provide.
- **Real inputs for the hot paths** (2a). Without them, a prediction about SkinAndDrawV2 rests on 4% of its
  code.
- **Cost prediction.** Nothing measures time yet (ROADMAP.md P0). A change's effect on frame time is not
  predicted, only observed afterwards.

## 5. Confirm: knowing in the game that it did that and nothing else

| Tool | Answers | State |
|---|---|---|
| Alternating trace (calls, arguments, small data, buffer hashes) | Did the Direct3D calls change beyond what the scene changes? | Built |
| Profiler (per call site), call counts (per function) | How many calls, from where | Built |
| Picture capture and comparison | Did the pixels change? Where? | **Missing.** Read back the frame (or a named render target) on the alternating frames and compare: the trace compares calls, not what they draw |
| Timing: Present wait, per-call time, sampling profiler | Where the time went | **Missing** (ROADMAP.md P0) |
| Coverage in the game | Which swapped or hooked code actually ran this session | Call counts per function only; not per path inside a function |

## 6. Correctness blind spots that undermine "know"

| Blind spot | Effect | State |
|---|---|---|
| Task #64: callees that leave a register untouched (ecx_use/edx_use miss inputs passed through them) | A function could be typed without an input it has | Closed in 7a925bd (both analyses follow `reg_preserve.py`); `data/edx_use.json` regenerated identical on 6 Oct. Left: the "cannot tell" functions (indirect calls, a pushed register) |
| The committed raw-stage gate predates the 6 Oct translator changes (the clean tree was re-gated after them, the raw records were not) | The residue classification of non-tree functions rests on older translations | Re-run the raw gate |
| `Gfx_DestroyD3DHolder` crashes when swapped (excluded) | Cause unknown | Open |
| `Post_DrawRotatingGhostLayers` FAIL (-0.0 / +0.0) | Cause unknown | Open |
| The `CDx+0xC` device declaration is accepted, not proven | Everything resting on it is marked | Accepted by the owner |

## 7. The tools that close the most, in order

| # | Tool | Closes | Size / state |
|---|---|---|---|
| 1 | **In-game input recorder**: a process dump at a chosen function's entry, replayed by the gate | The unverified hot paths (2a), the 2 INCONCLUSIVE, predictions on real inputs (4) | Built: `capture=` and pipeline/xcap.py (the kit writes its own capture of the reachable pages, not a minidump) |
| 2 | **Timing**: Present wait, per-call time, render-thread sampling profiler | Cost: where time goes, what a change saves (4, 5) | Built: Present wait timer, sampler; exact per-call timing not built |
| 3 | **Picture capture and comparison** on the alternating trace's frames | "Nothing else changed" for pixels (5) | Built: `trace_pictures=`, pictures.py |
| 4 | **In-game fact recorder**: pops and COM methods at named call sites | The 8 + 2 UNRESOLVED, the occlusion readback (2b) | Built: pop counts and call targets (`facts=1`, pipeline/fact_sites.py); no measurements taken in game yet |
| 5 | **Per-draw state snapshot and resource log** in the trace; **shader catalogue** from the binary | What the GPU is asked to do (2e) | Built (static catalogue: `shader_catalogue.py`, 6 Oct) |
| 6 | **SDK: fork workflow, device-interception API, mod reload** | Edits as diffs against verified code; device-level fixes; fast iteration (3) | Built (Wine-tested; not run in the game) |
| 7 | **Differential gate with declared differences** | Predicting a mod's effect offline (4) | Built for forks (`fork.py gate --declare`) and for a mod's own replacements (`fork.py gate-hook`, 6 Oct) |
| 8 | **Next exports**: tier 1 of the 267 callers (world draw path, menu path, asset loading) | What gets drawn and from what (2c) | IDA run, then the existing pipeline |
| 9 | **De-artifacting** with observed virtual-call targets and field values | Reading the code without offsets (2d) | Long, incremental |
| 10 | Task #64, the raw re-gate, the two open defects | Blind spots (6) | Small each |

Items 1 to 3 are also ROADMAP.md's P0 and prerequisite 2: the performance patch needs them first anyway.
