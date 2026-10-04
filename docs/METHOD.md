# Method

How the reconstruction is produced and checked, as the pipeline does it now. The dated record of how it got
here, with the errors found on the way, is `HISTORY.md`.

## 1. Inputs

- **The game DLL** (`FFXiMain.dll`, build 2026-09-03, unpacked; `$FFXI_DLL`). It is read by the fact tools and
  run by the gate. It is never copied into the repo.
- **The Hex-Rays export** (`data/exports/gfx_export.jsonl`): one JSON record per graphics function, with the
  pseudocode, every referenced object with its IDB type, and, per call, whether its type is declared in the
  IDB (`typed`) or made up by Hex-Rays from the pushes (`cast`). It is written by `ida/ffxi_ida_fix.py`
  (section 6). `ida_fix_report.json` and `ida_fix_log.txt` beside it are that run's record.
- **Memory dumps of the running game** (optional, never in the repo): `pipeline/dump_verify.py` checks that a
  dump is of the analysed build before the gate uses it (section 4).

The graphics set is 768 functions: every function that reads or writes a device holder (`g_pD3DHolder`,
`g_pCDx`, `g_pDancerDevice`, 0x10669418), plus the graphics class families and functions with D3D-typed
signatures (`ida/ida_gfx.py`, `select`).

## 2. Facts, and where each comes from

The rule (`CLAUDE.md`): every fact the pipeline uses comes from a source that settles it. Where none does,
the pipeline says so: the gate's verdict is UNRESOLVED and names the call site.

| Fact | Source | Tool, output | Check |
|---|---|---|---|
| A direct callee's stack pop | its `ret N`; a D3DX CPU-dispatch thunk resolves to its default implementation's `ret N` | `purges.py`, `data/purges.json` | the code itself |
| An indirect call's pop | the caller's own stack balance: equal depth wherever paths meet, zero at every `ret`, solved by exact elimination; a value only where the equations pin it. After `and esp, -N` the depth carries the padding as a variable no equation may pin. Switch tables are followed only where the code in front bounds the index (one- and two-level MSVC forms) | `sp_solve.py`, `data/sp_solved.json` (4337 calls) | `--selftest` hides the 20642 known `ret N` of direct calls and solves them back: 20641 right, and the one miss is in a function normal use rejects, so 0 wrong values reach the output |
| A virtual call's callee set | vtables (944; the game classes have no RTTI); an object running method F can only have a vtable holding F, or what F's direct callers pass | `vcall_resolve.py`, `data/vcalls.json` | 0 disagreements with the solver or with measured pops |
| Whether a function reads its entry ECX | data flow with partial registers; `or ecx,-1` / `xor ecx,ecx` are writes; `push ecx` is undecided | `vcall_resolve.py`, `data/ecx_use.json` | agrees with 1151 of 1152 IDB `__thiscall` declarations; the 10 IDB `__stdcall` declarations it showed wrong are `__thiscall` in the IDB since 3 Oct (9 retyped, 1 already was) |
| A D3D8 method call's pop | the D3D8 SDK header (`data/com_spec.json`), once the object's interface is proven: `data/com_fields.json` lists access paths with the proof, and its checkable parts (which instructions write the global or the offset, who refers to the function that fills it) are recomputed on every run | `com_calls.py`, `data/com_calls.json` (217 calls) | an entry whose check fails is dropped; all agree with the solver where both apply |
| A call's type as the IDB declares it | the IDB's typed prototypes (`src: typed` in the export); `cast` is never a fact | `callsites.py`, `data/callsite_args.json` | |
| A pop measured on real code | the original running in the emulator, real code only (not a stub, not a fake COM object of uncertain interface) | `real_sp.py`, `data/real_sp.json` | |
| EAX at `ret`: 0/1, or AL 0/1 | forward data flow | `ret_kinds.py` | |
| x87: float returns, consumed results, exact constants | stack-depth analysis of the x87 stack | `fpret.py`, `fp_callsites.py`, `fpconst.py` | |
| cdecl argument slots really read | the callee's code | `cdecl_args.py` | |

`ida_sp_check.py` compares IDA's stack pointer (the export's `spd`) with the solver's exact depth at every
block head and names the one correction that explains each difference: a call with a known pop, or the jump
in front of a block that only jumps reach (IDA carries its SP over from the jump in address order). Over the
768 exported functions IDA is wrong in 4, each explained by one correction (`data/ida_sp_audit.json`); the
IDA script applies them.

`callsite_audit.py` sets the sources side by side for every indirect call in the graphics code
(`data/callsite_audit.json`): 2662 calls, 2453 settled, 198 with no source, 11 where two sources disagree
(all in statically linked D3DX code, where the IDB's typed prototype is one argument short), and 2 where
Hex-Rays' cast contradicts a definitive value (0x1005A93D, 0x1007531D). A conflict gives no value.

The documented MSVC x86 ABI is also used: EBX, ESI, EDI and EBP are callee-saved.

## 3. Translation

`hx2cpp.py` turns each record into one C++ unit. Globals become lvalues at their real addresses and callees
typed pointers to the originals, so a reconstruction reads the same memory and calls the same code as the
function it replaces. String literals map back to the game's own strings. Each repair of Hex-Rays output is a
rule applied wherever its pattern occurs: exact float constants, the x87 status word Hex-Rays leaves
unassigned, unconsumed float callees, `_DWORD` signedness, exact stack frames where Hex-Rays splits one
array, in-place buffers, unordered compares.

Where no rule can repair the output, the correction is data:
- `data/hx_patches/0xADDR.json`: the exact snippet and its replacement, with the reason and the disassembly
  evidence. It applies only while the export's text has the recorded SHA-1, so a re-export reports a patch
  as stale instead of applying it silently.
- `data/type_fixes.json`: a corrected prototype (or data type), applied to every reference and to the
  function's own signature, calling convention and parameter list.

`repair_compile.py` fixes the casts GCC rejects. `run.sh` records the export's SHA-1 at translation and the
later stages refuse any other export.

## 4. The gate

`diffemu_gfx.py` (on `diffemu_fixed.py`) runs the original and the reconstruction side by side in Unicorn,
from byte-identical states, over 100 trials per function, and compares everything a caller could observe:
memory writes outside the stack, every call with its arguments, the return registers, and x87 results at
their declared precision.

- **Callees are stubs**, except 5,112 small leaf functions, which run for real. A stub's pop comes only from
  the sources in section 2; its return is fuzzed within what the real callee can produce (return kinds, x87
  pushes only where the caller reads them, allocators return fresh zeroed blocks). The same sequence is
  replayed on both sides.
- **Fake Direct3D 8 objects**: every method call is logged as interface, method, `this` and arguments, and
  compared. Getters fill their outputs; methods that return an interface write another fake object.
- **Inputs**: fuzzed arguments and globals, with demand-paged data (a deterministic page per trial and
  address, the same on both sides), `this` with its real class vtable, documented value ranges
  (`data/input_ranges.json`), and the Win32 calls the game makes answered as Win32 documents them.
- **Stack addresses** passed as arguments compare as "a stack address": a rebuilt frame lays out its locals
  differently.

Verdicts:

| Verdict | Meaning |
|---|---|
| PASS | at least one trial ran comparably on both sides, and none differed |
| FAIL | a trial differed; the first divergence is recorded (`localize.py` gives the source line) |
| UNRESOLVED | no comparable trial, and some trial stopped at a call with no definitive pop or argument count; the call site is named |
| INCONCLUSIVE | no comparable trial: the original faulted or never returned on every input |

A PASS covers the comparable trials. A trial stopped as UNRESOLVED is not compared, and the record lists
it (`unresolved`), so a PASS with such a trial says so.

**Live inputs** (`FFXI_LIVE`): `live_state.py` turns a memory dump of the running game into the gate's
inputs, so fields the game always fills are not fuzzed. It uses a dump only when `dump_verify.py` has shown
it is the analysed build. The DLL's executable sections, relocated to the dump's base with the DLL's own
relocation chain, are compared byte for byte. The four dumps of 3 Oct match in every code byte except 4
function entries patched at run time: three jump to Hook.dll, one to memory of no module, and none is a
graphics function. A call into another module gets its pop from the same sources as any call, or from a
sound walk of the callee's code in the dump (every reachable `ret` agreeing); otherwise the trial is
UNRESOLVED.

**Combined verdict.** `run.sh rawlive` and `cleanlive` (with `LIVE="name=live.json ..."`) gate the raw
translation and the clean tree on each verified dump. `combine_gates.py` joins them with the fuzz gate: a
FAIL in any gate is a FAIL; otherwise a function is PASS if any gate compared it and passed; with no
comparable run anywhere the fuzz verdict stands. Each `// GATE:` line lists every gate's verdict and
returning trials, so a PASS resting on few trials shows it. `GATE_IN=gate_combined.jsonl` builds the tree
from the combined verdicts. Real state finds what random inputs rarely produce. The first full live gate
found a NaN in an input: `CXiSkeletonActor_Slot244_Unknown` branched with `test ah, 5; jp` (taken for
greater, equal and unordered), which Hex-Rays had written `a >= b` (false for NaN). It is patched; the
idiom occurs at about 250 branches in 49 graphics functions, which the next IDA export is to cover
(each float comparison exported with its instruction, as integer comparisons are).

**Gate strength**: a mutation test on the 1 Oct clean tree (`verification/mutants_clean.json`) rejected 33
of 40 random one-site mutants. 2 survivors were on paths the original reached in 3 of 100 trials; the other
5 were not analysed site by site.

## 5. The clean tree and placement

`hx2clean.py` (run by `regen_clean.sh`) writes `tree/` from the functions that passed: one class per header,
`#pragma pack(1)`, a `static_assert` on every field offset and on `sizeof`; members become real members
(`this->field_98`), D3D calls `pDevice->SetRenderState(...)` with named enum values, calls into game code
`orig::Name(...)`. The tree is then gated exactly as built (`cleangate`) and every function is annotated
with its result. `same_code.py` compares two builds function by function (bytes with relocation sites
masked, plus relocation targets), so renaming or moving code is shown to change nothing.

Placement (`tree/placement.json`, and each function's `// FUNCTION:` line):

**Class members.** The evidence, in order:
- the class model (PS2 DWARF and vtables);
- a virtual method in the class's vtable that never reads ECX (same ABI as a member);
- an IDB name `Class_Method` on a `__thiscall` function (or a static member when it takes no `this`);
- every call site loading the same global into ECX, where the code creating that global shows its class;
- every caller passing its own `this` unchanged (to their common base when several subclasses call it);
- IDA's `this` type.

**Free functions** go into the file of the source unit the binary places them in. MSVC links each object
file's functions contiguously (`unit_place.py`). The anchors are:
- an assert or log path;
- a constructor or destructor storing exactly one class vtable;
- the class model;
- a name with a known class or dancer-module prefix.

Vtable-slot names are not anchors, because a subclass vtable also holds inherited methods. Code between
anchors of different units is grouped as `unit_<first address>[_<common prefix>].cpp` in link order. The
dancer library (`sq*`) is C, so its functions are free in their true form.

## 6. The IDA round trip

`ida/ffxi_ida_fix.py` is self-contained: `pipeline/make_ida_fix.py` embeds its data into `ida/ida_fix.template`.
Run in IDA 9.4 (File > Script file), it:
1. applies the pending names and prototype fixes (`data/names_pending_idb.json`, `data/type_fixes.json`) and
   the viewport types;
2. retypes `__thiscall` the IDB `__stdcall` functions that use their entry ECX (`data/ecx_use.json`);
3. removes stack-pointer points set wrongly by an earlier run (`data/ida_sp_revert.json`), then sets IDA's
   SP change at each call whose effect is known exactly (measured, solved or vtable-proven), each read back;
4. forces the call type where Hex-Rays assigns the pushes wrongly, verified by decompiling;
5. re-decompiles all 768 functions and writes `gfx_export.jsonl`, `ida_fix_report.json` and
   `ida_fix_log.txt` next to the IDB. They go into `data/exports/`.

Each step runs even if an earlier one failed; the log has the traceback.

The D3D8 interface declarations come from MinGW-w64's `d3d8.h` (`gen_d3d8_decls.py`, `xi_d3d8.hpp`). On 4 Oct
2026 they were checked against Microsoft's own DirectX 8.1 SDK headers (the 16 Oct 2001 release the game's
D3DX code matches). All 250 methods of the 12 interfaces agree in order, return type and parameter types; only
parameter names differ, and `WINBOOL` stands for `BOOL`. The SDK is not in the repo.

## 7. Coverage

Every function that touches a Direct3D device holder is in the set; not every graphics function is.
`gfx_coverage.py` re-derives this from the DLL: 267 game functions reach the device only through other
graphics code (`data/gfx_extension.json`):
- tier 1 (43): named graphics code and callers of D3DX resource code;
- tier 2 (62): unnamed functions calling two or more graphics functions;
- tier 3 (162): one graphics helper, mostly game logic.

They can be exported with `ida_gfx.select_extension('data/gfx_extension.json')` and `export_step`, or headless
with Ghidra (`pipeline/ghidra/GfxExport.java`). The translator accepts Ghidra's C through `xi_ghidra.hpp`, but
the pipeline does not yet take Ghidra input end to end.

The statically linked D3DX 8.1 code (`data/d3dx/`) was identified by `libmatch.py`: every function of
`d3dx8.lib` from the DirectX 8.1 SDK byte-compared, with relocation sites masked, against the image. 825
functions match (524 by name and bytes, 301 by bytes alone). 16 of those lie inside game code and are not
linked library code: inline math helpers from the SDK headers, and tiny constructors that happen to match.
None of them is among the 48 graphics-set functions counted as D3DX. `d3dx81_bind.hpp` binds the 46 public
entry points the game links to their game addresses.
