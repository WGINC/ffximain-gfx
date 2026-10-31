# Problems log

Open problems found while working, each with what is known, from where, and what is not known. A problem moves
to **Resolved** with what fixed it (and the commit) once that is shown, not when it seems likely.

## Open

### P20. The frame set's next functions stopped at "EAX past the declared void" (26 Oct, fs15; settled 26 Oct)
- **Was:** 155 of fs15's UNRESOLVED were functions declared to return nothing whose EAX at return differs, with no
  fact on whether a caller reads it. Most are virtual methods, whose callers are not all known.
- **Why no caller fact:** a sound caller set for a virtual method takes every indirect call that could reach it.
  Narrowing that to `call [vtable+4k]` at the method's own slot would assume its address never travels as data,
  which the code does not show.
- **Settled instead by the reconstruction:** export.sh's `XI_RETW` gives the listed functions a 4-byte EAX return
  (GfxExport.java `retw`, before the callee-widening rounds). The translation then hands back what the original
  leaves in EAX, and the gate compares all of it: no fact about callers is needed. fs16 export of the 155:
  131 raw PASS; 128 promoted (the clean gate under both cases).

### P19. 0x100DA750's first switch: its bound is not provable from the facts (26 Oct)
- **Known:** the switch at 0x100DA883 is bounded by `cmp eax, ebp; ja` (0x100DA873), with EBP set to 0xCB at
  0x100DA7A8. Between the two, on the paths to the compare, are calls to 0x100DB170 and 0x10103920.
- **Why not bounded:** data/reg_saved.json lists no register either callee is proven to restore. EBP keeping 0xCB
  across them is the calling convention's rule, not something their code is shown to do, so the switch stays
  unbounded (sp_solve.switch_targets) and the fs14 FAIL behind it (0x100DAF30) stays as it is.
- **What would settle it:** reg_saved proving EBP restored in both callees, or the kit's `jmp` recording of this
  site (session A of kit/sessions/pending_26oct records it: the targets the game took).

### P18. Five FAILs and three INCONCLUSIVEs from the re-gate of 26 Oct (read; 27 Oct, every remaining FAIL read)
- **27 Oct: each one read, and the tree's and fs14's remaining FAILs with them.** The table below gives, per
  function, its first mismatch's cause and where that cause is shown. Four causes were defects with a definitive fix,
  made in the pipeline (commits of 27 Oct):
  - **Thunks had no convention.** GfxExport.java set conventions only on the direct callees that are not Ghidra
    thunks. A thunk (`jmp target`) has no signature of its own in Ghidra (Function.isThunk: it shows its target's),
    and the targets were not in the set, so a call to one printed as `undefined f(void)`: its ECX object dropped
    (0x1013A100 calls 0x10150CC0 -> 0x10150CE0, `mov esi, ecx` at 0x10150CED), its result a 1-byte placeholder.
    Now the thunk's target takes the convention, the commit, and the EAX-width rule (`sigOwner`).
  - **A thunk used as a value read as its target.** Ghidra names a thunk after its target, and GfxExport bound a
    function-valued token by name: 0x1015B410 pushes 0x1011D980 (`jmp 0x1011CF70`) and the translation passed
    0x1011CF70. Now the address is the one of the target and its thunks that the function's own instructions hold
    as an operand, where exactly one does (`valueAddress`).
  - **`lea ecx, [ecx]` counted as reading ECX** (vcall_resolve.ecx_effect). It is the 3-byte no-op of alignment
    padding (strpbrk 0x10322BC1); by the Intel SDM's LEA it sets ECX to ECX. ecx_use.json: 4 entries change
    (0x10322B70, 0x10322BB0 false; 0x10319EF3, 0x10312487 undecided). _stat (0x10312487) was `__thiscall` with no
    stack arguments, so 0x101BE6C0's call to it passed neither of the two arguments the game pushes; it now takes
    `spops 8 pushed` (data/call_pushes.json). regen_facts.py: no other fact changed.
  - **hx2cpp:** a float stored into a global Ghidra types `undefined4` is a bit copy (ghidra_float_stores: Ghidra
    prints every numeric conversion as a cast; 0x102710B0 `fstp dword ptr [esi+0x78]`), and a virtual call through
    an object expression, `(**(code **)(**(int **)(this + 0xa0) + 0xa4))()`, passes that object as `this` under
    the same ecx_this_sites rule as a named one (vcall_sites; 0x1008C480).
- **Not fixed, with the fact that would settle each:** the original's EDX past a 1-byte or void return
  (0x10119BC0, 0x100180D0: regs_after.json says a caller may read it; C++ cannot hand back the EDX a callee left);
  stack contents the original passes or copies that nothing in its own frame wrote (0x1021C380, 0x100D0510,
  0x101C3A90, 0x101A04E0, 0x101A03D0); outputs a stubbed callee writes in the game but the stub does not (the three
  INCONCLUSIVEs, 0x101C3A90); pop counts or object facts not known (0x10014390, 0x1008B460, 0x10159FF0, 0x100DAF30
  through P19). No gate rule was changed for any of them.
- **Regressions looked for:**
  - GfxExport, old against new: the 31 functions above and the 104 tree PASS functions that call a `jmp` thunk
    (133 addresses), exported with the committed GfxExport and with the fixed one, same facts and retw lists. 99
    records differ; raw gate of each pair (runs/p18old, runs/p18new; 95 compiled in both): old 86 PASS, 8 FAIL,
    1 UNRESOLVED; new 92 PASS, 2 FAIL, 1 UNRESOLVED. Six FAIL -> PASS, nothing worse. The UNRESOLVED, 0x10129DE0
    (EAX past the declared bool), is the same under both.
  - hx2cpp: of the tree's 3,778 Ghidra records, 5 prepare differently (0x10034620: float stores; 0x100D0FC0,
    0x101B29F0, 0x101B36F0, 0x101B6810: virtual calls through `(int *)this`). All 5 PASS raw (runs/p18t5).
- **Not promoted:** ghidra_frameset23.jsonl is not in tree/source.json. The tree regenerated from the committed
  exports changes in those 5 functions only (the hx2cpp rules); they are to be gated clean when it is installed.

Per function (before: the verdict this entry started from, tree = verification/gate_clean_final.jsonl, fs14 =
gate_raw_frameset13.jsonl / its fs19 re-export; after: the raw gate of the 27 Oct export, pipeline/ghidra/export.sh
with the fixes, current facts, verification/gate_raw_frameset23.jsonl):

| Function | Before | First mismatch's cause | Source of the cause | After |
|---|---|---|---|---|
| 0x1013A100 | tree FAIL | thunk 0x10150CC0 called without its ECX object | 0x10150CC0 `jmp 0x10150CE0`; 0x10150CED `mov esi, ecx` (ecx_use.json); GfxExport skipped thunks | PASS |
| 0x101BE6C0 | tree FAIL | _stat (0x10312487) called with neither pushed argument; its output buffer then read unwritten | 0x101BE6CB `push eax; push ecx; call; add esp, 8`; ecx_use's `lea ecx, [ecx]` read (0x10322BC1) made it `__thiscall 0` | PASS |
| 0x1021C380 | tree FAIL | the game pushes a dword local with only its low byte written; 3 stack bytes reach ECX at returns whose callers are not all known | 0x1021C3xx `mov byte ptr [esp+0x10], 0` ... `push ebp` (26 Oct reading); arg_reads.py | not changed: no fact |
| 0x10119BC0 | tree FAIL | EDX at return: the game leaves `param_1` or `param_1[1]` (0x10119BC0/0x10119BD4), the C++ whatever it holds; the case compares EDX:EAX (the first export's `undefined8`) | regs_after.json: a caller may read EDX (via 0x10119C00, 0x10117520, 0x101177D0 to 0x1015F72E, a may-walk through callees whose EDX use is undecided) | raw PASS, because the current case returns bool and compares no EDX; the tree's clean case (int64) still FAILs (P9). Not changed: no fact; C++ cannot hand back that EDX |
| 0x100180D0 | tree FAIL | EDX at return is whatever the tail-dispatched callee leaves; in 3 trials the call goes through the fuzzed table slot 0x1035097C to the gate's trap, which leaves EDX, and the C++ had written EDX before the call | 0x10018124 `call [ecx*4+0x1035097C]`; regs_after.json (a caller may read EDX) | raw PASS under the current case (void: no EDX compared); the tree's clean case (int64) still FAILs (P9). Not changed: no fact |
| 0x100D0510 | tree FAIL | two locals passed to 0x100D0880 are outputs of the virtual call at 0x100D062B (slot 0x278), which the gate's trap does not write: the game passes the 0xA5 fill, the C++ a slot its source never writes on that path | 0x100D061A-0x100D062B (`lea eax, [esp+0xc]; lea ecx, [esp+8]; push..; call [edx+0x278]`); no vcalls.json or game_outs.json entry for the site | not changed: no fact |
| 0x100F1670 | fs15 FAIL | 0x1030FF75 called with none of the two arguments pushed | fs15 vs fs17 translations; fixed 26 Oct by `spops N pushed` (P17) | PASS (fs17, in the tree) |
| 0x101C3A90 | fs19 FAIL (left the tree) | strcpy of _sprintf's output buffer, which the stub never writes: each side copies its own stack past the buffer | 0x101C3CD3-0x101C3CE6 (byte copy to NUL) after `call 0x10311FF1` | not changed: the stub writes no output (no fact) |
| 0x100CD540 | tree INCONCLUSIVE | the loop runs while the iterator's current field is non-zero; only ResourceIter_FindFirst/FindNext (stubbed) write it, so it stays the fill | 0x100CD70A `mov eax, [esp+0x2c]; test; jne 0x100CD5A9` | not changed: no fact |
| 0x100E08D0 | tree INCONCLUSIVE | compares two name buffers 0x100E09A0 writes (stubbed, not a leaf): both are the fill, the strcmp walks the stack, and the index chain in the fuzzed table can cycle | 0x100E092C/0x100E093A calls, 0x100E094A loop | not changed: no fact |
| 0x100CFA50 | tree INCONCLUSIVE | the original faults walking 0x10071E90's circular resource list through fuzzed memory (the walk ends only where a node points back) | 0x10071EB2 `cmp edx, ecx; je`; 0x10071EB8 `jmp 0x10071E90` | not changed: no fact |
| 0x1015B410 | fs14 FAIL | the thunk 0x1011D980 passed as a callback was translated as its target 0x1011CF70 | 0x1015B4xx `push 0x1011D980`; 0x1011D980 `jmp 0x1011CF70` | PASS |
| 0x10221030 | fs14 FAIL | the result of thunk 0x1011D010 printed as Ghidra's 1-byte `undefined`; the caller compares all of EAX | 0x1022105F `cmp eax, ecx`; 0x1011D010 `jmp 0x1011D000` | PASS |
| 0x10221680 | fs14 FAIL | the same thunk's result stored as one byte where the game stores the dword | as above | PASS |
| 0x100F7790, 0x100F7960, 0x100F78C0 | fs14 FAIL | the results of 0x100F1920 and 0x100F1910, which Ghidra holds as thunks (of the dispatch thunks 0x10310030, 0x10310025), printed as the 1-byte `undefined`; the game stores the dword (`mov [ecx+0x18], eax`) | 0x100F7812-0x100F781D; the export's objects: `undefined(void)` before, `undefined4 __stdcall(void)` after | PASS |
| 0x102710B0 | fs14 FAIL | a float stored into a global typed `undefined4` converted to an integer | 0x10271273 `fstp dword ptr [esi+0x78]` | PASS |
| 0x1008C480 | fs14 FAIL | virtual calls through `*(this+0xA0)` printed without their object | ecx_this_sites.json (slots 41, 42, 166: ECX = the object, not pushed; pops 0, 4, 12) | FAIL: the calls now pass the object; in trial 4 the reconstruction then executes 0xA5A5A5A5 through a vtable slot. Not read further |
| 0x10014390 | fs14 FAIL | virtual calls through slot 0x1BC printed without their object; three sites of the slot have no pop count | ecx_this_sites.json; 0x10014587, 0x10014592, 0x100146EA in no pops source (sp_solved, vcalls, game_sp, real_sp) | FAIL: no fact |
| 0x1008B460 | fs14 FAIL | the same rule: slot 166 has a site whose ECX is not the object (0x1008B75D) and sites with no pop count | ecx_this_sites.json | FAIL: no fact |
| 0x10159FF0 | fs14 FAIL | virtual calls with no pop count at any site (slots 1-8) | ecx_this_sites.json; no pops source | FAIL: no fact |
| 0x100DAF30 | fs14 FAIL | 0x100DA750 called without ECX: its convention is not set because its entry pushes are not proven saves (the unbounded switch, P19) | entry_regs.json (push ebx/esi/ebp/edi), conv_facts `usercall` | UNRESOLVED (EAX past the declared bool, P20's class). The text still calls 0x100DA750 without ECX; no trial showed an ECX difference this time, which proves nothing |
| 0x10275D80 | fs14 FAIL | 0x102751D0 is listed as reading ECX through 0x102756A0, whose read (0x10275A6C `mov edi, ecx` after `mov cl, [esi+1]`) keeps only CL: ecx_use does not follow the copy, so the stub compares an ECX no fact says is read | vcall_resolve.ecx_inputs | FAIL: not fixed (ecx_inputs would have to follow ECX through a register copy) |
| 0x1022D7A0 | fs14 FAIL | Ghidra drops the store `mov word ptr [esp+0x14], 0x49` (0x1022D807) made between the pushes of a call's arguments; the loop then reads the slot unwritten | 0x1022D803-0x1022D80E; 0x1022D987 | FAIL: Ghidra's text, not fixed |
| 0x10189750 | fs14 FAIL | a float parameter Ghidra types `undefined4` read through FLOAT2FLOAT is printed `(float10)xi_s4`, which C++ converts as an integer | 0x10189774 `fmul dword ptr [esp+0x20]` | FAIL: not fixed. The text alone cannot tell FLOAT2FLOAT from INT2FLOAT on an `undefined4`; GfxExport would have to mark it, as it marks CASTs |
| 0x101540A0, 0x1027F0E0 | fs14 FAIL | first mismatch read (a different first callee; stores two bytes off), cause not established | | FAIL: not read further |
| 0x102110C0 | fs14 FAIL | 0x102112B0 reads only the low word (HISTORY 25 Oct: the must-store analysis that shows it is not committed) | arg_reads.json | FAIL: not changed |
| 0x101A04E0, 0x101A03D0 | fs14 FAIL | an output slot of File_FindByKind passed on: the game passes the `push ecx` reservation (0xCCCCCCCC) | HISTORY 25 Oct; data/game_outs.json has no record of the sites | not changed: no fact |

- **The entry as it stood on 26 Oct.** Known: with the stack filled with 0xA5A5A5A5 and the new facts, these were PASS before and are not now:
  - FAIL 0x101C3A90 (memory differs at 29 bytes), 0x1021C380 (a stub argument 0xA5A5A500 against 0), 0x100542F0
    (the reconstruction faults on 0xA5A5A5A5), 0x101BE6C0 (a stub argument 0xA5A5A5A5 against 0), 0x100F1670
    (EAX 1 against -1);
  - INCONCLUSIVE 0x100CD540 (every trial hits the step limit), 0x100E08D0, 0x100CFA50 (the original faults).
- **What it suggests, not established:** several values are the fill, so one side reads stack it never wrote, which a
  zeroed stack hid. Which side, and whether the original's result depends on what the real stack holds, is not
  read yet. Read on 26 Oct:
  - **0x1021C380:** the game writes one byte of a dword local (`mov byte ptr [esp+0x10], 0`) and pushes the whole
    dword to 0x100EB490 (`mov ebp, [esp+0x10]; push ebp`). 0x100EB490 loads it into ECX and reads CL; on one path
    it returns with the other three bytes still in ECX. arg_reads.py now follows a whole-dword load through the
    registers (26 Oct). It shows those bytes reach ECX at the return of callers whose own callers are not all
    known (virtual methods), so whether anything reads them is not settled, and the FAIL stands.
  - **0x101BE6C0:** the original passes a slot it never wrote (0xA5A5A5A5) to 0x101BE2C0, which passes the
    argument's address on to 0x10314D91. (27 Oct: the slot is _stat's output buffer; the tree passed _stat neither
    argument. See the table.)
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
- **26 Oct, mostly settled:** conv_facts.py now also gives a callee of undecided convention that pops 0 the
  stack bytes every settled caller pushes (`spops N pushed`, data/call_pushes.json), so the callers' decompiled
  code passes them as the game does. The fs17 export of the 45 functions UNRESOLVED on pushed slots: 38 raw PASS;
  in the tree 31 UNRESOLVED and 1 FAIL became PASS (their older records re-gated under the current rules first,
  gate_raw_regate.jsonl), plus 6 new functions. Left on this cause: 0x101BE6C0 and two fs17 records (0x10312487's
  pushed slots).
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

### P1. The live-dump gates were not run on the current tree (22 Oct; 27 Oct: run for the functions not PASS on fuzz; partly open)
- **Known:** `verification/live/gate_clean_*.jsonl` (combat, town, town2, zonetoinn) were run on the 4 Oct tree
  (a43e697, the last commit to touch them). Some functions' code has changed since under the same symbol
  (0x10038E10 now comes from the frame-set export). `verification/gate_clean_final.jsonl` holds fuzz verdicts
  only (4,463 functions on 27 Oct).
- **27 Oct, late:** the four dumps' states rebuilt with live_state.py and the 16 tree r14 functions not PASS on fuzz
  gated on each (verification/gate_clean_live14_*.jsonl), combined by combine_gates.py: 0x10003890, 0x100E08D0 and
  0x10010D20 PASS (the first two on 3 and 2 returning trials per dump; docs/HISTORY.md 27 Oct late). **Still open:**
  the other 4,881 verdicts are fuzz-only; a full live gate of the tree, or the owner's decision that fuzz is enough
  for the kit.
- **27 Oct, what changed:**
  - The dumps are in this environment after all: `/root/ffxi_dumps/dumps/*.dmp` (dated 3 Oct, outside the
    repository), with the 4 Oct live states `/root/ffxi_dumps/out/live_*.json`. dump_verify.json records the combat
    dump as the analysed build (timestamp 0x6A995428). The 22 Oct line "the dumps are not in this environment" was
    true of that session's container, not of this one.
  - The kit was rebuilt from the fuzz-only file anyway: af692f3 (26 Oct, "kit rebuilt on the re-gated tree", 4,225
    hookable), after ec1cdb7 (22 Oct). No owner decision that fuzz alone is enough is recorded in the repository.
    `kit/xikit/make_manifest.py`'s docstring and docs/KIT.md said "PASS on fuzz and all four dumps"; both now say
    what the manifest reads (27 Oct).
- **Not known:** whether the current tree's functions pass on the dumps, and whether the 4 Oct live states still
  serve the current gate: live_state.py precomputed the `this` sources and instances for the 4 Oct cases' vtables,
  and the gate has changed since (the 0xA5 stack fill, import stubs, pushed slots).
- **What would settle it:** a run, not a missing fact. With the new tree installed and no other gate running:
  live_state.py on each dump with the current DLL and cases (`DUMP.dmp - OUT.json DLL CASES`), then
  `LIVE="combat=... town=... town2=... zonetoinn=..." ./run.sh cleanlive RUN`, which combines them with the fuzz
  gate (combine_gates.py: a FAIL anywhere is a FAIL). Until then the kit's "verified" means fuzz PASS only, and
  that is for the owner to accept or not.

### P2. 0x1001A850 (`CYyMotionQue_GetResource`): the game's load order is not reproduced (22 Oct; settled 27 Oct)
- **Known:** the game's code is `mov eax,[esp+4]; mov ecx,[ecx+34h]; mov [eax],ecx; ret 4`, the parameter loaded
  before the field. Visual C++ 6.0 + c2.dll 13.0.9044 compiles `*p = field_34` the other way round.
- **Tried, all compile to the other order:**
  - spellings: through a `self_` copy, through `this`, a member field, an `int` member, a 4-byte struct copy, an
    inline getter, a reference parameter;
  - options: `/O1`, `/Ox`, `/O2 /G5`, `/O2 /G6`, `/O2 /GX` (`/Oy-`, `/Og /Oi /Ot` and `/Od` add a frame).
  - the VC6 sweep (verification/vc6_sweep.json) has the tree's `void` code DIFFERS at +1 (`0x41 here, 0x44 in the
    game`: ours loads into ECX first), with the `retthis` rule tried.
- **Not known:** what source or build gives that order.
- **Settled (27 Oct): the function returns its parameter.** The game's EAX at `ret 4` holds the parameter (the bytes
  above), and none of the spellings tried returned it. `unsigned int *MQ::GetResource(unsigned int *out) { *out =
  resource; return out; }` (field at +0x34) compiles at `/O2` to the game's bytes: match.py IDENTICAL, with the
  toolchain that stamps 48 / 49 build 9044, the DLL's Rich header entry (`match.py --compid`; a local copy of the
  toolchain, outside the repository). The same body declared `void` gives DIFFERS at +1. Added to
  recompile/small_math.cpp (all 15 of its functions IDENTICAL) and verification/recompile_match.json. What it shows
  is that a source returning the pointer gives these bytes. It does not show that no other source does, and what the
  game's callers do with the returned value is not looked up. The tree keeps its `void` code, which PASSes the clean
  gate with the EAX past `void` compared (P10). No verdict changes.

### P3. A gate worker process exited mid-piece (22 Oct; re-read 27 Oct, open)
- **Known:** in the clean gate of 22 Oct (runs/promo9), the worker for the piece holding 0x10257FF0 exited and left
  no result. Run on its own, 0x10257FF0 gave PASS. The same happened to 0x1018EA50 in the promo12 run (9354bbe,
  22 Oct: gated alone, PASS).
- **What the gate does when it happens** (pipeline/gate_some.py): the dead worker's chunk has no results, the log
  says `worker exited during ...` and `INCOMPLETE: N of M cases have results`, and the chunk's functions are
  missing from the output. A missing function is never PASS.
- **Since:** no `worker exited` line in any gate log kept under runs/ (searched 27 Oct). The `INCOMPLETE` lines in
  the fs11 to fs19 logs (11 or 12 cases each) have no `worker exited` line before them, so they are cases that
  gave no result for another reason. docs/FRAMESET_REMAINING.md names one (no DLL built, the `SQRT` row); that it
  is the reason in each of these logs is not checked.
- **Not known:** why the two workers exited. Neither log says, and the runs are not kept.
- **What would settle it:** the next exit, with the worker's exit status recorded. gate_some.py does not record it
  (it reads only the worker's stdout); `w.wait()`'s return code in the log would say whether a signal ended it.

### P4. Clean-tree functions not verified (22 Oct, clean gate before the R2 fix; settled 27 Oct but 0x10003890)
| Function | Verdict | What is known |
|---|---|---|
| 0x10040220 | FAIL | trial 1: AL 128 vs 144 (return byte differs) |
| 0x1006F0A0 | FAIL | trial 1: memory differs at 0x30004FA4: original 224 / reconstruction 223 |
| 0x1006F800 | FAIL | trial 25: memory differs at 0x30000F54: original 132 / reconstruction 131 |
| 0x10003890 | UNRESOLVED | the call at 0x10003A50 has no definitive pop count |
| 0x10016320 | INCONCLUSIVE | the original never returns under the gate's inputs |

- **Settled (27 Oct, from the history of verification/gate_clean_final.jsonl):**
  - 0x1006F0A0, 0x1006F800: PASS since bae9357 (22 Oct evening). The FAIL was one ulp: the export's `sse` flag
    reached the raw translation but not the clean tree; hx2clean now carries it (HISTORY, 22 Oct evening).
  - 0x10016320: PASS since bae9357, under the fixed inputs data/case_inputs.json states (its GATE line says so).
  - 0x10040220: left the tree in d3e3a50, back and PASS since 081ee53 (26 Oct: the fs16 export with XI_RETW, a
    4-byte EAX return). It passes under the IDA case, which declares `bool` (P9), and under its other case
    (gate_clean_altcases.jsonl).
- **Still open, 0x10003890:** INCONCLUSIVE. The pop count is no longer the cause: data/sp_solved.json settles the
  call at 0x10003A50 (pops 0). The original faults in all 100 trials through objects in its own `this`
  (gate_clean_final.jsonl: reads at 0x0 to 0xD). Its 4 Oct live gates PASS on all four dumps, on the code of that
  time. What would settle it: the live gates on the current tree (P1).

### P5. verify_all `export-sp`: IDA's stack depths wrong in 0x100412B0 (before 21 Oct; re-read 27 Oct, open)
- **Known:** from 0x100419F4 on, IDA's SP is 4 more than the exact depth (`data/ida_sp_audit.json`). The function's
  IDA-route raw gate FAILs, and it is not in the tree.
- **Open:** verify_all reports this as a failure, because no correction is recorded for it. Still so on 27 Oct
  (verification/verify_all.json, `export-sp`: ok false; every commit since reports "all PASS but export-sp").
- **Why the audit cannot name the cause (27 Oct):** ida_sp_check.py compares IDA's SP with the exact depth only
  where sp_solve has one. 0x100419F4 is a block head that the jumps at 0x100412C0 and 0x100412FD reach at depth
  28. On the path through the function's body, 13 calls have no pop count from any source (sp_solved, vcalls,
  com_calls, purges, real_sp): 0x100413AF, 0x10041435, 0x10041453, 0x10041709, 0x10041729, 0x10041749,
  0x10041769, 0x100417CE, 0x10041945, 0x10041965 (virtual calls), and 0x100414A2, 0x1004157F, 0x10041644 (to
  0x1006AE30, which ends `jmp [ecx+0xd8]` and so has no `ret N` of its own). Past the first of them the exact
  depth is not known, so where IDA's SP first goes wrong cannot be placed.
- **Also:** the frame set reaches it by the Ghidra route, and there it is blocked on its own: in_stack / unaff reads
  (docs/FRAMESET_REMAINING.md).
- **What would settle it:** pop counts for those calls. Then the audit pins IDA's wrong change point, and
  ida/ffxi_ida_fix.py can set it in the IDB (an owner IDA run) before a re-export.

### P6. 51 gate-passed functions left out of the tree (22 Oct; re-read 27 Oct, open for 9)
- **Known:** each calls a game function that may read a register its wrapper does not pass: 49 EAX, 1 ECX, 1 EDX
  (`tree/left_out.json`).
- **Needed to bring them back:** all callers known (`data/eax_inputs.json` `callers_known`), or declared void
  prototypes from the IDB, or the reads shown to be masked off.
- **27 Oct, from the history of tree/left_out.json:** 51 (8ace87a) became 25 (d3e3a50), then 9 (bae9357,
  pipeline/gate_sees_eax.py: a kept-EAX call the gate runs as the game does is the gate's to judge). 31b491b added
  one function that does not compile (0x10018850), listed with the compiler's message. That makes 10.
- **The 9, and the fact behind each** (hx2clean.wrapper_bad, from data/eax_inputs.json, eax_kept_sites.json,
  ecx_use.json, edx_use.json):
  - the callee reads its entry EAX: 0x10057FB0 (calls 0x10062D50), 0x1006B3B0 (0x10044490), 0x10125360
    (0x10118DB0);
  - the callee hands back EAX bytes this caller reads after the call: 0x10009920 (0x100098E0), 0x10013AD0
    (0x1001E4B0), 0x10048000 (0x100538E0), 0x100C63D0 (0x10084350). `callers_known` is true for all seven
    callees, so the first way out named above does not apply: the reads are facts;
  - ECX: 0x10042D00 (0x1006AE30); EDX: 0x100A1AB0 (0x100A19B0).
- **What would bring them back:** a call type that passes the register, a `/*regs:...*/` type, which
  wrapper_bad already accepts, on the export's record of the callee from a proven entry read. For the kept EAX: a
  callee wrapper that hands back all of EAX. The exports the tree was built from give these callees neither
  (the reasons in left_out.json).

### P7. msvc_tu.py covers only part of the tree (22 Oct; settled 27 Oct)
- **Covered:** integer code, member and free functions, globals (declared from `gfx_globals.hpp` and checked by
  address), calls to the game (`orig::`, typed wrappers and untyped XI_ANY arguments, checked by address).
- **Left out, each with its reason:** floating point, address-taken frames (`xi_frame`), calls through a pointer,
  entry-EAX reads, calls whose wrapper returns XI_ANY, `orig_void::` calls.
- **Compile errors still met:**
  - implicit pointer conversions that GCC's `-fpermissive` accepts and MSVC6 rejects (`void *` to `T *`, between
    unrelated pointer types);
  - a few globals the body names that `gfx_globals.hpp` does not bind (e.g. `DAT_10485643`).
- **Settled (27 Oct):** the two compile errors were fixed within the hour: cdb38d1 writes the implicit conversions
  as their casts, e9b86c4 makes address-taken frames ordinary locals and finds every global. msvc_tu.py has since
  been replaced as the measure by pipeline/vc6/sweep.py (129ddde, 25 Oct, to 331cf27, 26 Oct; docs/RECOMPILE.md).
  verification/vc6_sweep.json gives every tree function a verdict: 1,650 IDENTICAL, 2,278 DIFFERS, 71 NO_COMPILE (cl
  failed), and 252 LEFT_OUT, each with its reason. What is left is recorded there per function.

### P8. The IDB declares `g_pCamera` (0x104568FC) as `std::codecvt_base *` (22 Oct; re-read 27 Oct, open)
- **Known:** the IDA export's code writes it from `App_Constructor`'s result: `g_pCamera = (std::codecvt_base *)v3;`
  (`data/exports/gfx_export.jsonl`). `tree/include/gfx_globals.hpp` carries that type. A C++ library facet type
  for an object the game's own constructor makes contradicts the code, so the declaration looks like a wrong
  library-signature match in the IDB.
- **Known since, from the game's code (27 Oct):** CApp_RunMainLoop allocates 0x33C bytes (0x10001569 `push 0x33c;
  call 0x10311BBB`), constructs them with 0x10010700 and stores the result at 0x104568FC (0x10001587). 0x10010700
  stores the vtable 0x10329C14 at [this+0] (0x10010777). class_vtables.json does not name that vtable, so
  data/this_class.json gives its methods no class. The dword before the vtable is no RTTI locator (the game classes
  have none, docs/METHOD.md). 0x10010700 also calls 0x10027990 ten times, which the IDB names
  `??0Iostream_init@@QAE@XZ_0`. That function is `mov eax, ecx; ret` (recompile/small_math.cpp, `return this`): a
  body that short matches any empty constructor, so the library name says nothing of the type. Windower 5 calls the
  pointer `render` (data/community_objects.json), which is a community name, not a type.
- **Not known:** the global's real type. It is left as declared until a source gives one.
- **What would settle it:** a name for vtable 0x10329C14 (class_vtables.json), or a PS2 match of 0x10010700
  (data/ps2_matches.json has none).

### P9. A function's tree code and its clean-gate case can come from different routes (22 Oct; settled 27 Oct)
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
  - **Settled (26 Oct):** data/regs_after.json (pipeline/regs_after.py: every caller known, the callers' code
    followed from each return address) says some caller of 0x10119BC0 may read the EDX it leaves, so the older
    case's EDX comparison stands and so does its FAIL. 0x100180D0's callers are not all known: the same.
- **Settled (27 Oct):** the rule that settles "which case holds" is the one applied since fs15: a function with two
  cases is gated under both and keeps the stricter verdict (gate_clean_altcases.jsonl,
  gate_clean_fs15_altcases.jsonl, gate_clean_regate_altcases.jsonl). Both return disagreements now pass the stricter
  case. 0x10040220's code returns the 4-byte EAX (081ee53, fs16 XI_RETW) and PASSes the IDA `bool` case. 0x1003E910
  PASSes its IDA `int` case. 0x10119BC0 and 0x100180D0 stay FAIL on EDX, a fact C++ cannot meet (P18). 0x100289F0
  stays INCONCLUSIVE: under the older case's pointer arguments the original walks a list (`mov esi, [esi+0x10]; cmp
  esi, ebx; jne`, 0x10028A18) through fuzzed memory and faults.

### P10. The gate compared EAX only as wide as the decompiler's prototype (22 Oct; changed in b4d1d31; settled 27 Oct)
- **Known:** a case's `returns` of void, bool or a word limited the EAX comparison. Where `caller_reads.json` does
  not list the function, that width is only the prototype's. In the tree: 309 Ghidra-route `void` functions and 73
  IDA-route ones whose callers may read EAX, and 92 `bool` functions whose callers may read more than AL.
- **Change:** those bytes are now compared too. A difference only there makes the trial, and so the function,
  UNRESOLVED. On a sample of 24, 5 differ there.
- **Pending:** the four raw gates and then the clean gate rerun with it (started 22 Oct). Counts will drop where
  functions were PASS only because EAX went unseen.
- **Settled (27 Oct):** the reruns were done. The raw gates: ida9, frameset9, fsfall9 (P12's numbers); the tree's raw
  verdicts committed in 1f4fa1c (22 Oct). The clean gate: promo12 (d3e3a50, 22 Oct 06:38, after b4d1d31 at 01:26),
  and the whole tree again in 9858cc1 (26 Oct). The rule is still in the gate (diffemu_gfx.py: "EAX at return
  differs past the declared ..."). The functions it made UNRESOLVED were the subject of P12 and P20. One is left in
  the tree: 0x1026E1F0 (P12).
  verification/gate_raw_final.jsonl (5c295cd, 21 Oct) predates the rule; the tree is not built from it, and
  gate_some.py reads it only to print changes.

### P11. Ghidra's `CONCAT22(param >> 16, K)` returns: the source construct is not found (22 Oct; settled 27 Oct)
- **Known:** 0x10005A00's game code loads its argument into EAX and returns with only AX written (`xor ax, ax` /
  `mov ax, 0x7f` / `mov ax, 0xff81`, then `ret`). The top half of EAX is the argument's.
- **Tried, none gives that code:**
  - a 16-bit write through the parameter's address;
  - a 4-byte struct by value and a union by value;
  - a local struct or union copy;
  - and/or masks;
  - early-return and if/else orders.
- **Not known:** what source MSVC6 compiles to a partial-register write like this.
- **Settled (27 Oct): the construct is a 16-bit return.** The VC6 sweep's `ret16` rule (4c1a60e, 25 Oct;
  docs/RECOMPILE.md) writes `return (HIGH << 16) | (uint16_t)(LOW);` as `return LOW;` in a function returning
  `unsigned short`. 14 functions are then IDENTICAL to the game's bytes (verification/vc6_sweep.json,
  `rule_identical`). Among them is 0x100DB930, the same shape: `mov eax, [0x104DFD98]; test eax, eax; jne; xor ax,
  ax; ret`. The upper half of EAX is whatever the compiler left there, as in the game. 0x10005A00 itself (27 Oct):
  `extern "C" short Input_MapAxisHigh(unsigned int x) { if (x == 0xffffffff) return 0; if (x < 0x1a5e || x > 0x7242)
  return -127; if (x <= 0x2bf2 || x >= 0x60ae) return 0; return 0x7f; }` compiles to the game's bytes (match.py
  IDENTICAL, build 9044; recompile/small_math.cpp, verification/recompile_match.json). The unsigned compares are the
  game's own (`jb`, `ja`, `jbe`, `jae`). The same tests nested under `x != -1` DIFFER at +7: the order of the tests
  shows in the bytes. The tree's code returns all of EAX and PASSes the gate; it is not changed, and no verdict
  changes.

### P12. Functions called through vtables are UNRESOLVED on EAX past their declared return (22 Oct; settled 27 Oct for the tree)
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
- **Settled for the tree (27 Oct), by the reconstruction rather than by caller facts:** P20's XI_RETW (081ee53,
  4fce05a, 26 Oct). A listed function is exported with a 4-byte EAX return, so the translation hands back what the
  original leaves and the gate compares all of it. Of the tree's 4,463 functions, 1 is UNRESOLVED this way
  (verification/gate_clean_final.jsonl): 0x1026E1F0. The tree still builds it from the `void` record. Its retw
  records (ghidra_frameset16.jsonl, ghidra_frameset21.jsonl, `undefined4`) PASS raw. Why the tree does not use them
  was not looked up here. The IDA route's raw gate under P10's rule is not re-measured here.

### P13. IDB prototypes include types previous sessions inferred (22 Oct, from the owner; re-read 27 Oct, open)
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
- **27 Oct:** unchanged. The file has no `mine` entry (the latest commit touching requests/ is 2a3eb35), and
  `site_pops_init` still takes only the SDK-identical types. What would settle it: the owner's marks in that file.

### P14. The Ghidra-route methods do not know their class (22 Oct; re-read 27 Oct, open)
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
- **27 Oct:** the first was started the same day. pipeline/class_flow.py (c4755b1, 22 Oct 16:48) follows vtables,
  vtable stores and callers to a fixpoint. It settles 983 functions DLL-wide (data/class_flow.json `_counts`: 694 by
  vtable, 206 that store it, 83 by their callers), but only 23 of the tree's 1,108 class-less functions at the time.
  data/this_class.json is unchanged since 9e3bc32 and still has 3,438 functions behind an unnamed vtable. Naming
  the vtables is not started. What would settle more: names for those vtables, from a definitive source (a
  constructor whose class is known, a PS2 match).

### P15. Naming sessions invented area words (22 Oct; settled 22 Oct, 519dee1)
- **Found:** 653 of the 1,739 names the four naming sessions wrote (and 2 of my own pilot's) begin with an area word
  their batch's dossier never shows (`Crypt_` with no crypt anywhere in batch 031; `Pool_`, `Layout_`, `Ui_`).
  The README rule against it reached the sessions after most batches were written.
- **Done:** `check_names.py` now refuses such a prefix mechanically: it must be the batch's class, `Unit<address>`
  or the batch's label, or whole CamelCase parts of a name the dossier shows (`StAvatar` from `StAvatarInit`).
  `reprefix_names.py` applies the README's fallback (the batch's prefix) and keeps the session's name under
  `invented_prefix_name`; `merge_names.py` runs it on every merge. Where two names then collide, or two sessions
  gave one name to two functions, the entry is dropped (51): which one was meant is not known.
- **Settled (27 Oct, re-read):** the prefix check is in check_names.py (519dee1, 22 Oct 06:00), and merge_names.py
  runs reprefix_names.py and check_names.py on every names file it merges, through round 5 (42c6c25, 26 Oct). What
  it does not cover is stated below and measured in P16.
- **Not known:** the words after the prefix are not checked mechanically; `low` names (most) describe code
  mechanically by the README's own definition.

### P16. How accurate the naming sessions are, measured (22 Oct; settled 22 Oct, e0b9820)
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
- **Settled (27 Oct, re-read):** the entry records a measurement, made in e0b9820, and asks nothing further. Its
  limits stand: "correct" is an upper bound, and a session's name is a reading of the dossier, not a fact.

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
