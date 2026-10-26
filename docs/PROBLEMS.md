# Problems log

Open problems found while working, each with what is known, from where, and what is not known. A problem moves
to **Resolved** with what fixed it (and the commit) once that is shown, not when it seems likely.

## Open

### P20. The frame set's next functions stop at "EAX past the declared void" (26 Oct, fs15)
- **Known:** the Ghidra export of the 943 frame-set functions not in the tree, together with the 51 tree functions not
  PASS, gave 993 records:
  - 430 translated; 376 are the statically linked C runtime (not reconstruction targets); 172 skipped (a register
    or stack input Ghidra did not tie to a parameter: `extraout_` 79, `in_stack_` 28, `in_EDX` 24, `in_ECX` 14,
    ...); 15 untranslatable (imports by ordinal with no SDK prototype).
  - Gated: 43 PASS (39 new functions, 4 of the 51), 61 FAIL, 69 INCONCLUSIVE, 205 UNRESOLVED.
  - 154 of the UNRESOLVED are one case: the function is declared to return nothing, its EAX at the return differs
    between the original and the reconstruction, and no fact says no caller reads it (caller_reads.json has no
    entry). 39 more are pushed slots whose reading is not settled (P17).
- **Not known:** whether any caller reads those EAX values. Most of the functions are virtual methods, whose callers
  caller_reads.py does not see, because they call through vtables.
- **What would settle it:** the callers of each vtable slot (vcalls.json targets give some), each checked for an
  EAX read after the call; or the reconstruction leaving EAX as the original does.

### P19. 0x100DA750's first switch: its bound is not provable from the facts (26 Oct)
- **Known:** the switch at 0x100DA883 is bounded by `cmp eax, ebp; ja` (0x100DA873), with EBP set to 0xCB at
  0x100DA7A8. Between the two, on the paths to the compare, are calls to 0x100DB170 and 0x10103920.
- **Why not bounded:** data/reg_saved.json lists no register either callee is proven to restore. EBP keeping 0xCB
  across them is the calling convention's rule, not something their code is shown to do, so the switch stays
  unbounded (sp_solve.switch_targets) and the fs14 FAIL behind it (0x100DAF30) stays as it is.
- **What would settle it:** reg_saved proving EBP restored in both callees, or the kit's `jmp` recording of this
  site (session A of kit/sessions/pending_26oct records it: the targets the game took).

### P18. Five FAILs and three INCONCLUSIVEs from the re-gate of 26 Oct (read in part)
- **Known:** with the stack filled with 0xA5A5A5A5 and the new facts, these were PASS before and are not now:
  - FAIL 0x101C3A90 (memory differs at 29 bytes), 0x1021C380 (a stub argument 0xA5A5A500 against 0), 0x100542F0
    (the reconstruction faults on 0xA5A5A5A5), 0x101BE6C0 (a stub argument 0xA5A5A5A5 against 0), 0x100F1670
    (EAX 1 against -1);
  - INCONCLUSIVE 0x100CD540 (every trial hits the step limit), 0x100E08D0, 0x100CFA50 (the original faults).
- **What it suggests, not established:** several values are the fill, so one side reads stack it never wrote, which a
  zeroed stack hid. Which side, and whether the original's result depends on what the real stack holds, is not
  read yet.
- **Read so far (26 Oct):**
  - **0x1021C380:** the game writes one byte of a dword local (`mov byte ptr [esp+0x10], 0`) and pushes the whole
    dword to 0x100EB490 (`mov ebp, [esp+0x10]; push ebp`). 0x100EB490 loads it into ECX and reads CL; on one path
    it returns with the other three bytes still in ECX. arg_reads.py now follows a whole-dword load through the
    registers (26 Oct). It shows those bytes reach ECX at the return of callers whose own callers are not all
    known (virtual methods), so whether anything reads them is not settled, and the FAIL stands.
  - **0x101BE6C0:** the original passes a slot it never wrote (0xA5A5A5A5) to 0x101BE2C0, which passes the
    argument's address on to 0x10314D91. Not read further.
  - **Settled (26 Oct): 0x100542F0, and 3 UNRESOLVED (0x100157B0, 0x101033C0, 0x1027CCF0).** Each was built from an
    older export whose raw PASS predates the current gate rules. Every older-export record of the tree's
    non-PASS functions that another export passes raw was gated again under the current rules
    (verification/gate_raw_regate.jsonl, 21 records, read by hx2clean.py through XI_REGATE; tree/source.json
    `raw_regate`). None still passes, so these four are now built from fs15's translation, and each PASSes the
    clean gate under both its cases. Six functions no export now passes raw left the tree, and the game's code
    runs in their place: 0x10073D80, 0x100D4330, 0x100EEE90, 0x101C09B0, 0x101C0A40, 0x101C3A90. None of the
    six passed the clean gate.
  - **Not re-gated:** the older records of the other 41 non-PASS tree functions (3 more are built from fs15,
    whose verdicts are under the current rules). For them no other export passes raw, so a re-gate could only
    take them out of the tree.

### P17. Gate PASS functions that pass fewer arguments than the game (25 Oct, found by the VC6 sweep)
- **Known, from the game's bytes:**
  - **0x100F1870** pushes six arguments to 0x10310067 (`push -2; push -1; push -2; push -2; push eax; push -2;
    call; add esp, 0x18`). The tree calls `orig::FUN_10310067()` with none: its wrapper is typed
    `undefined4 __stdcall ()`, the IDB's prototype.
    0x10310067 is a dispatch thunk (`mov eax, [0x10456A2C]; jmp [eax+0x30C]`), so no callee code gives its count.
  - **0x10028640** (Mem_AllocObject) pushes three to 0x10028FD0 (`push 0; push eax; push ecx; call;
    add esp, 0xC`); the tree passes two.
- **Why the gate passes them:** for a caller-cleanup call, the stub compares only the argument slots the callee
  is known to read:
  - data/cdecl_args.json, from the callee's code;
  - else the IDB's count;
  - else none (gfx_cases.py, the stub's `args`).
  Slots past those are not compared. An argument the tree leaves out is invisible when the callee's code cannot be
  read (0x10310067). It is also invisible when the callee reads fewer slots than the caller pushes.
- **How many:** in 103 DIFFERS functions of the sweep, the game cleans more bytes directly after some call than
  the tree's compiled code does. That is a flag to read each one, not a count of wrong functions: our code and the
  game's can defer cleanup differently. Functions that did not compile, or were not swept, are not covered.
- **Not known:**
  - how many of the 103 really pass fewer arguments;
  - whether the extra slots are arguments the callee uses or slots it never reads (deferred cleanup of something
    else). The game's bytes say what is pushed, not what is read.
- **What settles it:** a call's arguments as the caller pushes them, from the caller's own code and exact stack
  depth. A call followed directly by `add esp, N`, with no other call since the depth was last N bytes lower on
  that path, pushed exactly N bytes for that call. The tree's wrapper counts are checked against that, and the
  gate compares those slots.
- **Done (26 Oct):**
  - `pipeline/call_pushes.py` (data/call_pushes.json) settles 11,570 caller-cleanup calls; 2,376 callees have one
    count over every settled site.
  - `gfx_cases.pushed_args`: a caller-cleanup stub compares that many slots where no exact read set of the callee
    says the rest go unread. `case_pushes.py` applies the same rule to verification/cases_raw_final.json: 181
    stubs in 166 cases.
  - The gate's stack starts filled with 0xA5A5A5A5 instead of zeros (diffemu_fixed.py). Mem_AllocObject's
    missing third argument is a 0 the game pushes, and an unpushed slot read 0 on a zeroed stack.
  - A difference in a slot past those the callee is known to read leaves the trial UNRESOLVED, not FAIL (`args_may`,
    kept apart as `eax_may` is): the game pushes it, the tree does not, and no source says whether the callee reads
    it. 0x10028FD0, for one, reads only its first two of the three its callers push.
  - Full re-gate of the tree (fuzz, 4,280 functions): 4,229 PASS, 42 UNRESOLVED (this class), 5 FAIL,
    4 INCONCLUSIVE (P18). 50 functions that were PASS are not any more.
- **Still open:** the tree's code for these functions can only pass the arguments once the decompiler sees the
  right prototypes. The 74 wrapper declarations the pushed counts contradict need an IDA or Ghidra re-export,
  which this environment cannot run. ida/ffxi_ida_fix.py now carries the counts (step 2c, `fix_arg_counts`). It
  retypes a caller-cleanup function `__cdecl` with the bytes its callers push, only where the IDB declares fewer
  and the prototype is not variadic. The extra parameters are `unsigned int`, their meaning unknown. The step
  has not been run in IDA yet.

### P1. The live-dump gates were not run on the current tree (22 Oct)
- **Known:** `verification/live/gate_clean_*.jsonl` (combat, town, town2, zonetoinn) were run on the 4 Oct tree.
  Some functions' code has changed since under the same symbol (0x10038E10 now comes from the frame-set export).
- **Not known:** whether this build's functions pass on the dumps. The dumps are not in this environment.
- **Consequence:** `verification/gate_clean_final.jsonl` holds fuzz verdicts only. The kit
  (`kit/xikit/make_manifest.py`, "PASS on fuzz and the four dumps") is not rebuilt from it until the dumps are
  rerun or the owner accepts fuzz alone.

### P2. 0x1001A850 (`CYyMotionQue_GetResource`): the game's load order is not reproduced (22 Oct)
- **Known:** the game's code is `mov eax,[esp+4]; mov ecx,[ecx+34h]; mov [eax],ecx; ret 4`, the parameter loaded
  before the field. Visual C++ 6.0 + c2.dll 13.0.9044 compiles `*p = field_34` the other way round.
- **Tried, all compile to the other order:**
  - spellings: through a `self_` copy, through `this`, a member field, an `int` member, a 4-byte struct copy, an
    inline getter, a reference parameter;
  - options: `/O1`, `/Ox`, `/O2 /G5`, `/O2 /G6`, `/O2 /GX` (`/Oy-`, `/Og /Oi /Ot` and `/Od` add a frame).
- **Not known:** what source or build gives that order.

### P3. A gate worker process exited mid-piece (22 Oct)
- **Known:** in the clean gate of 22 Oct (runs/promo9), the worker for the piece holding 0x10257FF0 exited and left
  no result. Run on its own, 0x10257FF0 gave PASS.
- **Not known:** why the worker exited. Nothing in its log says.

### P4. Clean-tree functions not verified (22 Oct, clean gate before the R2 fix)
| Function | Verdict | What is known |
|---|---|---|
| 0x10040220 | FAIL | trial 1: AL 128 vs 144 (return byte differs) |
| 0x1006F0A0 | FAIL | trial 1: memory differs at 0x30004FA4: original 224 / reconstruction 223 |
| 0x1006F800 | FAIL | trial 25: memory differs at 0x30000F54: original 132 / reconstruction 131 |
| 0x10003890 | UNRESOLVED | the call at 0x10003A50 has no definitive pop count |
| 0x10016320 | INCONCLUSIVE | the original never returns under the gate's inputs |

The causes of the three FAILs have not been looked into yet.

### P5. verify_all `export-sp`: IDA's stack depths wrong in 0x100412B0 (before 21 Oct)
- **Known:** from 0x100419F4 on, IDA's SP is 4 more than the exact depth (`data/ida_sp_audit.json`). The function's
  IDA-route raw gate FAILs, and it is not in the tree.
- **Open:** verify_all reports this as a failure, because no correction is recorded for it.

### P6. 51 gate-passed functions left out of the tree (22 Oct)
- **Known:** each calls a game function that may read a register its wrapper does not pass: 49 EAX, 1 ECX, 1 EDX
  (`tree/left_out.json`).
- **Needed to bring them back:** all callers known (`data/eax_inputs.json` `callers_known`), or declared void
  prototypes from the IDB, or the reads shown to be masked off.

### P7. msvc_tu.py covers only part of the tree (22 Oct)
- **Covered:** integer code, member and free functions, globals (declared from `gfx_globals.hpp` and checked by
  address), calls to the game (`orig::`, typed wrappers and untyped XI_ANY arguments, checked by address).
- **Left out, each with its reason:** floating point, address-taken frames (`xi_frame`), calls through a pointer,
  entry-EAX reads, calls whose wrapper returns XI_ANY, `orig_void::` calls.
- **Compile errors still met:**
  - implicit pointer conversions that GCC's `-fpermissive` accepts and MSVC6 rejects (`void *` to `T *`, between
    unrelated pointer types);
  - a few globals the body names that `gfx_globals.hpp` does not bind (e.g. `DAT_10485643`).

### P8. The IDB declares `g_pCamera` (0x104568FC) as `std::codecvt_base *` (22 Oct)
- **Known:** the IDA export's code writes it from `App_Constructor`'s result: `g_pCamera = (std::codecvt_base *)v3;`
  (`data/exports/gfx_export.jsonl`). `tree/include/gfx_globals.hpp` carries that type. A C++ library facet type
  for an object the game's own constructor makes contradicts the code, so the declaration looks like a wrong
  library-signature match in the IDB.
- **Not known:** the global's real type. It is left as declared until a source gives one (the class
  `App_Constructor` builds, from its vtable or the PS2 debug data).

### P9. A function's tree code and its clean-gate case can come from different routes (22 Oct)
- **Known:** the clean cases come from `verification/cases_raw_final.json`, where a function with an IDA case keeps
  it. 3 tree functions take their code from another route because the IDA route's raw gate does not pass them, and
  for 2 the routes disagree on the return: 0x10040220 (IDA `bool`, frame set `void`) and 0x1003E910 (IDA `int`,
  frame set `void`).
- **Effect:** the clean gate holds the frame set's code to the IDB's prototype. For 0x10040220 that is a FAIL: AL
  differs, so the Ghidra translation does not return the value the IDB declares. Its frame-set raw PASS was gated
  against Ghidra's `void`, which is no fact.
- **Open:** which case should hold. The IDB's typed prototype is a declared type, so the stricter IDA case stays.
- **26 Oct, fs15 promotion:** of the 34 new or changed tree functions, 23 have an older case in cases_raw_final.json
  as well as fs15's own (runs/fs15/cases.json). For 17 the two differ: return width, argument specs, stubs or
  fuzz ranges. Each of the 17 was gated under both cases. 14 PASS under both. 3 PASS only under fs15's case:
  - 0x10119BC0: the old case compares EDX:EAX (the first frame-set export's `undefined8`, widened because a caller's
    decompiled code reads EDX). fs15's case compares AL (caller_reads.json: callers read only AL of EAX). Under
    the old case, all 100 trials differ.
  - 0x100180D0: the old case compares EDX:EAX; fs15's case is `void`. 3 of 100 trials differ.
  - 0x100289F0: the argument specs differ; under the old case, 99 of 100 original runs fault (INCONCLUSIVE).
  - The tree keeps the stricter verdict, FAIL, FAIL and INCONCLUSIVE (verification/gate_clean_final.jsonl).
  - **Not known:** whether any caller reads the EDX these functions leave. caller_reads.py follows EAX only, so no
    fact covers it. The same walk over EDX would settle it.

### P10. The gate compared EAX only as wide as the decompiler's prototype (22 Oct; changed in b4d1d31)
- **Known:** a case's `returns` of void, bool or a word limited the EAX comparison. Where `caller_reads.json` does
  not list the function, that width is only the prototype's. In the tree: 309 Ghidra-route `void` functions and 73
  IDA-route ones whose callers may read EAX, and 92 `bool` functions whose callers may read more than AL.
- **Change:** those bytes are now compared too. A difference only there makes the trial, and so the function,
  UNRESOLVED. On a sample of 24, 5 differ there.
- **Pending:** the four raw gates and then the clean gate rerun with it (started 22 Oct). Counts will drop where
  functions were PASS only because EAX went unseen.

### P11. Ghidra's `CONCAT22(param >> 16, K)` returns: the source construct is not found (22 Oct)
- **Known:** 0x10005A00's game code loads its argument into EAX and returns with only AX written (`xor ax, ax` /
  `mov ax, 0x7f` / `mov ax, 0xff81`, then `ret`). The top half of EAX is the argument's.
- **Tried, none gives that code:**
  - a 16-bit write through the parameter's address;
  - a 4-byte struct by value and a union by value;
  - a local struct or union copy;
  - and/or masks;
  - early-return and if/else orders.
- **Not known:** what source MSVC6 compiles to a partial-register write like this.

### P12. Functions called through vtables are UNRESOLVED on EAX past their declared return (22 Oct)
- **Known:** with P10's rule, the IDA route's raw gate goes from 770 PASS to 666 (ida9, gate_raw10):
  - 99 functions differ in EAX bytes past their declared return: 69 `bool`, 22 `void`, 8 word;
  - 5 are made UNRESOLVED by an entry-EAX difference that one trial showed.
- **Why the facts do not cover them:** `caller_reads.py` lists a function only if every reference to it is a direct
  call or a tail jump. Of the 111 functions UNRESOLVED this way, 90 have their address stored as data in the image,
  mostly vtable slots, so their callers are not all known.
- **Paths, measured:**
  - Through vtable slots: callers through a slot are known only if every indirect call with that slot's offset is
    settled. vcalls.json settles 555 of the DLL's 7,774 `call [reg+disp]` sites. For example, 72 of 478 at +0x18
    and 3 of 528 at +0x8, so this recovers few until vcall_resolve settles more.
  - Declared prototypes: the export records do not say which IDB prototypes a person declared and which Hex-Rays
    inferred, so the IDB's `bool` is not usable as a declared type here. The owner could mark the ones they typed.
  - Otherwise they stay UNRESOLVED, which is what is known.
- **The raw gates' 244 such functions** (ida9, frameset9, fsfall9; 22 Oct):
  - 130 are referenced as data (vtables, tables). Only 47 slot offsets below 0x400 have every `call [reg+disp]`
    through them settled, while the busiest have hundreds unsettled (923 at +200, 712 at +0).
  - 114 have no data reference. In a sample of 3:
    - one has no direct caller either, so how it is reached is not known;
    - two have a caller that returns right after the call (`ret 0x18`; `pop …; ret`), passing EAX up to callers
      that are not all known.

  For these, UNRESOLVED is correct. Recovering them needs the callers along the whole chain, not a local fix.

### P13. IDB prototypes include types previous sessions inferred (22 Oct, from the owner)
- **Known:** the owner confirms some IDB types were inferred by earlier sessions. The ones `ida/ffxi_ida_fix.py`
  applied are listed in its data, with notes `[type 2026-10-02]`. Others cannot be told apart from declared ones,
  because the export records no prototype's origin.
- **Dependency:** the gate takes a call's pop count, ECX-as-`this` and compared argument count from
  `callsite_args.json` `"src": "typed"`. Of 8,381 such sites, 4,784 have no other source (2,960 `__thiscall`, 823
  `__stdcall`, 629 `__cdecl`, 301 `__fastcall`, 71 other).
- **Origins, from the IDB itself** (`data/ida_proto_origin.json`, `ida/ida_proto_origin.py`, run by the owner's
  local session on `FFXiMain_recon.i64`):
  - The IDB was checked to be the analysed build: the `.text` sha256 is `b55f8b4c…`, the PE timestamp is
    `0x6A995428`, and there are no patched bytes.
  - Of the 4,784 sites with no other source:

    | Origin | Sites |
    |---|---|
    | IDA's own default, not user-set | 3,331 |
    | User-set (owner or a session; IDA does not say which) | 553 |
    | Inlined helpers, or not in today's decompilation | 623 |
    | Computed address (now a cast) | 186 |
    | Identical to the SDK library | 43 |
    | A session's `[type]` note | 32 |
    | D3D vtable members (local copies) | 15 |
    | A local variable | 1 |
- **Where the gate uses them:** `site_pops` is consulted only for:
  - calls to imports with no SDK argument count;
  - calls into other modules (live mode);
  - calls through a fuzzed function pointer that input repair aims at a trap.

  A direct call to a game function is stubbed with its pops from the callee's `ret N` (`purges.json`), never from
  this file. So the IDB type decides a pop count only at indirect sites.
- **Direct sites checked against the callees' `ret N`:**
  - 3,531 agree;
  - 275 have no known `ret N`;
  - 66 disagree. All 66 are `__usercall`/`__userpurge` types (plus `_sprintf`), whose count `callsites.py` took as
    stack dwords, although such a type places arguments in registers. That rule is removed: those 107 sites are now
    `"src": "regs"`, not `typed`. They were direct calls, so no verdict used them.
- **Applied (22 Oct 03:01, 8a68276):** the gate takes an IDB call type as a source only where
  `data/ida_proto_origin.json` shows it identical to the SDK type library's (`diffemu_gfx.site_pops`). None of the 912
  indirect sites that rested on an IDB type alone has such an origin, so each is UNRESOLVED wherever the gate reaches
  it. Every verdict since, the tree's 4,049 PASS included, is under this rule. (The note that stood here, "the
  proposal stands, awaiting the owner", was written 32 minutes before that commit.)
- **Still open, for the owner:** `requests/review/02_user_types.md` lists the 372 user-set types the gate's sites or the
  tree use. 148 have a recorded origin (143 `session`, 5 `library`); 224 are blank, and only the owner can say which
  are theirs. The gate does not yet read the marks: once there are `mine` entries, `site_pops` can take those types
  as a source too (an owner-typed prototype is a declared type), and the sites they settle re-gated.

### P14. The Ghidra-route methods do not know their class (22 Oct)
- **Known:** in the regenerated tree, 1,477 functions reach fields by raw offset from `self_` (9,543 accesses), as
  free `__thiscall` functions. Fields need the class.
- **Measured** (`pipeline/this_class.py`, `data/this_class.json`; vcall_resolve's classes(F), vtables named by
  `class_vtables.json`):
  - 21 of them have a settled class (256 accesses);
  - 1,412 have none: either no vtable reaches them (methods of classes without one; 2,734 such functions in the DLL),
    or a caller passes an object the code does not type (TOP);
  - 3,438 functions DLL-wide see a vtable `class_vtables.json` does not name.
- **Needed for more:** the class from where the object is built (its constructor or allocation site), and names for
  the unnamed vtables. Both are larger analyses, not started.

### P15. Naming sessions invented area words (22 Oct)
- **Found:** 653 of the 1,739 names the four naming sessions wrote (and 2 of my own pilot's) begin with an area word
  their batch's dossier never shows (`Crypt_` with no crypt anywhere in batch 031; `Pool_`, `Layout_`, `Ui_`).
  The README rule against it reached the sessions after most batches were written.
- **Done:** `check_names.py` now refuses such a prefix mechanically: it must be the batch's class, `Unit<address>`
  or the batch's label, or whole CamelCase parts of a name the dossier shows (`StAvatar` from `StAvatarInit`).
  `reprefix_names.py` applies the README's fallback (the batch's prefix) and keeps the session's name under
  `invented_prefix_name`; `merge_names.py` runs it on every merge. Where two names then collide, or two sessions
  gave one name to two functions, the entry is dropped (51): which one was meant is not known.
- **Not known:** the words after the prefix are not checked mechanically; `low` names (most) describe code
  mechanically by the README's own definition.

### P16. How accurate the naming sessions are, measured (22 Oct)
- **Method:** `pipeline/name_bench.py` builds the round-2 dossier for 100 functions whose names are known (23 PS2
  symbols matched on a unique literal; 77 DancingMad names in the frame set), with every spelling of the known name
  removed; the answer key is kept apart. Two models, a smaller (A) and a larger (B), each named all 100 under `batches/README.md`. A third
  session judged each name against the true one, blind to the model: correct (same action and object), vague (true
  but generic), partial, wrong. `pipeline/name_bench_score.py` scores word overlap.
- **Result:**

  | | correct | vague | partial | wrong | skipped |
  |---|---|---|---|---|---|
  | model A (smaller) | 63 | 24 | 12 | 1 | 0 |
  | model B (larger) | 61 | 24 | 10 | 2 | 4 |

  By confidence (both models): `high` 12 of 12 correct; `medium` 87 of 110 correct, 2 wrong; `low` mostly vague.
- **Read:** the larger model does not name better. Wrong names are rare; the loss is vagueness, and it comes from
  the dossier, not the model: a getter of `this+0x9C` cannot be named `SetWalkSpeed` without knowing the field.
  The judge was lenient in places (`ResourceHandle_Deref` accepted for `GetFirstDwordOrZero`); "correct" is an
  upper bound. DancingMad's names are a reference, not proven.
- **Not a source of field names:** the PS2 DWARF types (2,758, with members) do not share the PC layouts
  (`XiActor` 160 bytes on PS2, `CXiActor` 196 on PC), so their member names apply only where a matched function pair
  shows the same field on both sides.

## Resolved

### R1. 0x100CD540 (`CXiSkeletonActor::SetMotion`) FAILed in the clean build only (22 Oct)
- **Cause:** `pipeline/map_clean_cases.py` took the first export for each demangled name. Two `SetMotion` overloads
  share it, so 0x100CD540 was gated against 0x100CD490's code.
- **Fix:** an overloaded name is resolved by the parameter count of the function's declaration in the tree's
  headers; a name still ambiguous is not mapped. Gated against its own code: PASS (commit 4c7729e).

### R2. 0x1003C7D0 FAILed in the clean build only, 1 trial in 100 (22 Oct)
- **Cause:** trial 43 set a vtable slot to 0x60253808, inside the reconstruction DLL's zero padding. Both runs
  executed the padding (`add [esi], cl`), whose writes depend on leftover registers.
- **Fix:** the game never runs the reconstruction's code, so in the original's run that code is not executable.
  Landing there is execution of data, repaired like any other (`pipeline/diffemu_gfx.py`, commit 4c7729e). The trial
  then passes. The whole clean gate is being rerun with the fix.
