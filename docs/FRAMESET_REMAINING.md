# Frame-set functions still outside the tree

Plan item #109. Which game-code functions of the frame set are not in the verified tree, what stops each one, and
what was fixed in the pipeline for the largest buckets that have a definitive fix. Nothing here is installed into
`tree/`; the raw gate verdicts are for the integrator's regeneration and install.

## The set

The frame set as it was before e6aeca1 / 93185fe (`data/frame_set.json` at db12b4e: 4,984 functions). Outside the
tree there are 760 functions that are not `d3dx` entries. 376 of them are library code by hx2cpp.py's rule
(D3DX 8.1 by the manifest or name, or the statically linked runtime from 0x102D0000 on: `status: library` in the
translation's meta.json). The other **384** are game code: the fs19 addresses (`runs/fs19/addrs.txt`, 405) less
the 21 that reached the tree since. They are the subject of the tables below. The functions the frame set gained
in 93185fe are a separate group at the end and are not counted in these tables.

Sources of each verdict: the translation's meta.json (export/translate status, with hx2cpp's reason), the build
(`raw/obj`, `raw/fndll`), gfx_cases.py's cases, and the raw gate (`diffemu_gfx.py`). "Before" is the fs19 records
(`data/exports/ghidra_frameset19.jsonl`, `verification/gate_raw_frameset19.jsonl`) with the fs20 / fs21 verdicts
where those runs re-gated a function. A re-export of all 384 with the current facts and no pipeline change
(scratch run, not committed) gave the same distribution, except 0x1025F640, which now PASSes (FAIL in fs19).
"After" is fs22: `data/exports/ghidra_frameset22.jsonl` (the 384 addresses, `XI_RETW` for the 28 of them the
fs19/20/21 runs exported with `retw`), gated raw into `verification/gate_raw_frameset22.jsonl` (170 functions).

## Counts per bucket

| blocker, before (fs19; fs20/fs21 verdicts where newer) | functions |
|---|---:|
| PASS raw | 8 |
| FAIL | 69 |
| INCONCLUSIVE | 49 |
| UNRESOLVED | 9 |
| does not compile | 29 |
| does not link | 12 |
| no gate case | 11 |
| does not translate | 15 |
| does not export (skipped) | 181 |
| no export record | 1 |
| total | 384 |

| blocker, after (fs22) | functions |
|---|---:|
| PASS raw | 52 |
| FAIL | 72 |
| INCONCLUSIVE | 36 |
| UNRESOLVED | 10 |
| does not compile | 30 |
| does not translate | 1 |
| does not export (skipped) | 182 |
| no export record | 1 |
| total | 384 |

PASS raw: 8 before, 52 after. 44 of the 52 are new from the fixes below, one (0x1025F640) from the current facts,
seven were PASS before and stay out of the tree for the reason `tree/left_out.json` gives. The eighth PASS of
"before", 0x10077A00 (fs20, uncommitted run, `retw`), FAILs in fs22 and in the no-change re-export alike
(`EAX 0x3000137C vs 0x101A1CB0`): the change is in the current facts or export, not in this pass's fixes; not
investigated further.

## What was fixed (each from a definitive source)

| Bucket (before) | Functions | Blocker | Fix | Source |
|---|---:|---|---|---|
| gateable, no verdict | 12 | No DLL: `SQRT` was `__builtin_sqrtl`, which GCC inlines as `fsqrt` only where it proves the operand non-negative and otherwise calls `sqrtl` for errno; the freestanding build does not link `sqrtl` (worker log: FileNotFoundError on the missing `fndll/fn_*.dll`) | `SQRT(x)` is the `fsqrt` instruction (`xi_ghidra.hpp`) | Ghidra's FLOAT_SQRT p-code comes from the x87 `fsqrt` in the game's code (all 12 are x87 records, `sse: false`) |
| does not translate | 14 | `import Ordinal_N: no SDK prototype`: the game imports WS2_32.dll by ordinal only, so Ghidra has no name to find the SDK prototype by | `pipeline/import_libs.py` reads the IMPORT_ORDINAL short-import members of Visual Studio 6's `WS2_32.LIB` (all 25 of the game's WS2_32 ordinals named; pefile's own table agrees on all 25); `export.sh` passes them to `GfxExport.java`, which applies that name's SDK prototype. `xi_compat_win.hpp` declares the winsock2.h types those prototypes use (`u_long`, `SOCKET`, `hostent`, `in_addr`, `WSADATA`), with the SDK's sizes asserted | the SDK's import library (Microsoft PE/COFF specification, import library format) and VC98/INCLUDE/WINSOCK2.H |
| INCONCLUSIVE (import) | 13 | Every trial stopped at an import with no solved call site (`import_pops.json` lists only imports some solved call reaches; the IMM32 thunks at 0x102BC8xx are reached only through the thunk) | diffemu_gfx.py stubs such an import with the bytes its import library's decorated symbol pops (`_Name@N` __stdcall pops N, `_Name` __cdecl pops 0), and takes a __stdcall import's argument count as N / 4 where `import_nargs.json` has none | `data/import_libs.json` from the same import libraries; checked against `import_pops.json` (195 imports, all agree) and `import_nargs.json` (260 imports, all agree) |
| no gate case | 11 | __fastcall records: gfx_cases.py made no case for the convention | A __fastcall record (storage ECX, EDX, then the stack) runs as a __thiscall one with EDX an entry register drawn per trial (`edx_in`), the way a `regin EDX` record already runs | the record's own storage (GfxExport `storage`) |
| (the same 11, once gated) | 11 | `stack cleanup differs: original pops 0, reconstruction pops 4`: GCC passes a __fastcall's (or __thiscall's) register parameter in its register only when it is an integer or pointer; Ghidra's `undefined4` is a struct here, so the export read EDX from a stack slot and popped it | hx2cpp.py builds such a definition through `regparm`, which passes the struct in the register, as it already does for register inputs (affects no record of the installed tree) | GCC manual, x86 function attributes; checked by compiling both forms |

Result: of these 50 functions (the fastcall 11 counted once), 44 PASS raw; 3 FAIL (0x1002AC60, 0x10032E70,
0x1027A8A0, now gated: FAIL kinds below); 0x100D93E0 is UNRESOLVED (EAX past a declared bool, no caller-read fact);
0x100ED7E0 is now skipped (Ghidra shows an `in_addr` register flow); 0x1026CDF4 does not compile (below).
0x1026D634 stays untranslatable: WSACreateEvent is imported by name, and Ghidra's SDK archive has no prototype
for it.

The tree is not changed. Two of the changes reach the tree when the integrator regenerates it: the `SQRT`
definition (20 installed Ghidra records use `SQRT`, so they want a re-gate) and the import stub fallback (only a
case marked `"imports": "stub"` uses it, and only for an import with no solved call site).

## New raw PASS (fs22)

| Function | Name | Before |
|---|---|---|
| `0x10001CC0` | FUN_10001cc0 | INCONCLUSIVE: import |
| `0x10008020` | FUN_10008020 | INCONCLUSIVE: import |
| `0x10015AB0` | FUN_10015ab0 | INCONCLUSIVE: import |
| `0x100275D0` | FUN_100275d0 | gateable, no verdict |
| `0x10027600` | FUN_10027600 | gateable, no verdict |
| `0x10036C70` | FUN_10036c70 | gateable, no verdict |
| `0x1003B980` | FUN_1003b980 | gateable, no verdict |
| `0x10047F80` | FUN_10047f80 | no gate case |
| `0x1006CB50` | FUN_1006cb50 | no gate case |
| `0x10071D80` | FUN_10071d80 | no gate case |
| `0x10072990` | FUN_10072990 | no gate case |
| `0x1007BF30` | FUN_1007bf30 | gateable, no verdict |
| `0x100CD040` | FUN_100cd040 | gateable, no verdict |
| `0x100D95D0` | FUN_100d95d0 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC2D0` | FUN_100dc2d0 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC360` | FUN_100dc360 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC3A0` | FUN_100dc3a0 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC3B0` | FUN_100dc3b0 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC650` | FUN_100dc650 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC670` | FUN_100dc670 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100DC6E0` | FUN_100dc6e0 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x100F0130` | FUN_100f0130 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x10103330` | FUN_10103330 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x10126C60` | FUN_10126c60 | no gate case |
| `0x1012A360` | FUN_1012a360 | no gate case |
| `0x10136670` | FUN_10136670 | no gate case |
| `0x1016A990` | FUN_1016a990 | gateable, no verdict |
| `0x10176E80` | FUN_10176e80 | no gate case |
| `0x10176F60` | FUN_10176f60 | no gate case |
| `0x10176FE0` | FUN_10176fe0 | no gate case |
| `0x101893B0` | FUN_101893b0 | no gate case |
| `0x101AFF50` | ImmGetContext | INCONCLUSIVE: import |
| `0x101AFF60` | ImmReleaseContext | INCONCLUSIVE: import |
| `0x101B8E70` | FUN_101b8e70 | INCONCLUSIVE: fault,import |
| `0x101B94A0` | FUN_101b94a0 | INCONCLUSIVE: import |
| `0x1024E280` | FUN_1024e280 | gateable, no verdict |
| `0x1025F640` | FUN_1025f640 | FAIL: memory differs at N byte(s), first N: original N / reconstruction unch |
| `0x1026CC30` | FUN_1026cc30 | untranslatable: import Ordinal_N: no SDK prototype |
| `0x102746C0` | FUN_102746c0 | gateable, no verdict |
| `0x102BC826` | ImmGetDescriptionA | INCONCLUSIVE: import |
| `0x102BC82C` | ImmGetContext | INCONCLUSIVE: import |
| `0x102BC832` | ImmReleaseContext | INCONCLUSIVE: import |
| `0x102BC83E` | ImmSetCandidateWindow | INCONCLUSIVE: import |
| `0x102BC844` | ImmSetCompositionWindow | INCONCLUSIVE: import |
| `0x102BC874` | ImmSetConversionStatus | INCONCLUSIVE: import |

## Every function by bucket (fs22)

### PASS raw (52)

- **not installed** (45): `0x10001CC0`, `0x10008020`, `0x10015AB0`, `0x100275D0`, `0x10027600`, `0x10036C70`, `0x1003B980`, `0x10047F80`, `0x1006CB50`, `0x10071D80`, `0x10072990`, `0x1007BF30`, `0x100CD040`, `0x100D95D0`, `0x100DC2D0`, `0x100DC360`, `0x100DC3A0`, `0x100DC3B0`, `0x100DC650`, `0x100DC670`, `0x100DC6E0`, `0x100F0130`, `0x10103330`, `0x10126C60`, `0x1012A360`, `0x10136670`, `0x1016A990`, `0x10176E80`, `0x10176F60`, `0x10176FE0`, `0x101893B0`, `0x101AFF50` ImmGetContext, `0x101AFF60` ImmReleaseContext, `0x101B8E70`, `0x101B94A0`, `0x1024E280`, `0x1025F640`, `0x1026CC30`, `0x102746C0`, `0x102BC826` ImmGetDescriptionA, `0x102BC82C` ImmGetContext, `0x102BC832` ImmReleaseContext, `0x102BC83E` ImmSetCandidateWindow, `0x102BC844` ImmSetCompositionWindow, `0x102BC874` ImmSetConversionStatus
- **left out of the tree: CDx_SetDefaultRenderState: calls 0x100098E0 reads EAX, which its wrapper does not pass** (1): `0x10009920` CDx_SetDefaultRenderState
- **left out of the tree: App_StepGameState5Connect: calls 0x1001E4B0 reads EAX, which its wrapper does not pass** (1): `0x10013AD0` App_StepGameState5Connect
- **left out of the tree: sub_10018850: does not compile in this tree: no match for 'operator!=' (operand types are 'PTR_s_mov999_pmv_10350e54_t' {aka 'undefined4'} and 'undefined*' {aka 'unsigned char*'})** (1): `0x10018850`
- **left out of the tree: CMoDistMorphElem_ValidateMorphData: calls 0x1006AE30 reads ECX, which its wrapper does not pass** (1): `0x10042D00` CMoDistMorphElem_ValidateMorphData
- **left out of the tree: CMoGeneratorClone_VT_1032B114_Destructor: calls 0x100538E0 reads EAX, which its wrapper does not pass** (1): `0x10048000`
- **left out of the tree: sub_100A1AB0: calls 0x100A19B0 reads EDX, which its wrapper does not pass** (1): `0x100A1AB0`
- **left out of the tree: sub_10125360: calls 0x10118DB0 reads EAX, which its wrapper does not pass** (1): `0x10125360` sub_10125360

### FAIL (72)

- **a call differs (target, ECX or an argument)** (24): `0x10014390`, `0x100157E0` App_WinMain, `0x1002AC60`, `0x10074CC0`, `0x100C33D0`, `0x100DAF30`, `0x1011A150`, `0x101540A0`, `0x1015B410`, `0x1017B510` sub_1017B510, `0x10181BB0`, `0x10182070`, `0x101A03D0`, `0x101A04E0`, `0x101A0680`, `0x101C9FA0`, `0x101CB280`, `0x102041D0`, `0x102110C0`, `0x10221030`, `0x1022D7A0`, `0x10234A90`, `0x1024DA10`, `0x10275D80`
- **memory written differs** (18): `0x10027F60`, `0x1002C680`, `0x10032E70`, `0x100363B0`, `0x10086900` sub_10086900, `0x1008F740`, `0x100979B0`, `0x10189750`, `0x1018C2B0`, `0x1019D2B0`, `0x101C3A90`, `0x101C6960`, `0x10221680`, `0x10250030`, `0x102710B0`, `0x102712E0`, `0x1027A8A0`, `0x1027F0E0`
- **EAX at return differs** (16): `0x1001A8E0`, `0x100274B0`, `0x10040C20` CMoElemFamily_SharedEarlyExit, `0x1006D5D0`, `0x10077A00`, `0x100FC3B0` Unit100D7420_SetHandler, `0x10119FB0`, `0x1011A560`, `0x10139FE0` fn_1013A100, `0x10154D70`, `0x10155320`, `0x10155500`, `0x101768A0`, `0x101BE510`, `0x102590E0` Unit10253A20_DeleteNodesA, `0x1025A800` Unit10253A20_DeleteNodesB
- **reconstruction faults where the original returns** (10): `0x1000A2B0`, `0x1006A0E0`, `0x1008B460`, `0x1008C480`, `0x1008E3F0`, `0x100964A0`, `0x100EF680`, `0x10159FF0`, `0x101BB070`, `0x10203C80`
- **EDX:EAX at return differs** (3): `0x1015E830`, `0x10189870`, `0x1019ED50`
- **x87 result differs** (1): `0x10125FE0`

### INCONCLUSIVE (36)

- **original outcomes: fault** (26): `0x100058F0`, `0x10017010`, `0x10047960`, `0x10056C70`, `0x10057D60`, `0x10071E90`, `0x10071F60`, `0x10071F70`, `0x10071F80`, `0x1008ABE0`, `0x1008E290`, `0x100F1770` thunk_FUN_1030ff33, `0x100F1780` thunk_FUN_1030ff49, `0x100F1910` thunk_FUN_10310025, `0x100F1920` thunk_FUN_10310030, `0x100F1930` thunk_FUN_103100e0, `0x100F1940` thunk_FUN_103100ca, `0x100F2A70`, `0x10163640`, `0x10164B80`, `0x1017A690`, `0x1018ABB0`, `0x101C2E70`, `0x101CC620`, `0x102224C0`, `0x1028D980`
- **original outcomes: limit** (8): `0x10072DB0`, `0x10108600`, `0x101086B0` thunk_FUN_10108600, `0x101C2B60`, `0x101C2C80`, `0x101C4080`, `0x10270840`, `0x102709E0`
- **original outcomes: fault,limit** (2): `0x10056BB0`, `0x101D01B0`

### UNRESOLVED (10)

- **call N: pop count unknown** (6): `0x1000ABE0`, `0x1003BA20`, `0x10069DD0`, `0x1006A070`, `0x1006C140` CMoOcclusionMng_Constructor, `0x1021DD50`
- **virtual call on a fuzzed object returning to N: target unknown, its xN result read** (2): `0x100D4560` CXiSkeletonActor_GetElem, `0x100D4800` CXiSkeletonActor_GetElemLocal
- **EAX at return differs past the declared bool** (1): `0x100D93E0`
- **entry EAX passed to N differs in bytes it is only assumed to read (eax_inputs.json)** (1): `0x101A07D0`

### does not compile (30)

- **an x87 value cast to or from a 4-byte type (`xi_castbits: one size`): mostly an unmarked indirect call Ghidra shows returning in ST0, whose pops no source gives (so it is not marked and is typed as returning int)** (9): `0x1003D640`, `0x10041A40`, `0x10064010`, `0x100C83C0`, `0x100CF750`, `0x100D6D30`, `0x10123970`, `0x1017C9C0`, `0x1024FDF0`
- **a call printed with fewer or more arguments than the callee prototype: tail jumps to targets not in func_extents.json (0x101185C0, 0x101188C0, 0x10152EF0, 0x10278D00), and 0x100720B0 calling 0x10071FD0 (a `keeps ecx` __thiscall) without its object** (7): `0x100720B0`, `0x1015DD50`, `0x1015DD70`, `0x101CBC10`, `0x101D0C70`, `0x1023BE10`, `0x10273810`
- **switch case labels on a value Ghidra typed as a pointer** (4): `0x1001D940`, `0x1002C990`, `0x101587F0`, `0x10285040`
- **a function pointer assigned from a data symbol** (2): `0x1002CE20`, `0x10202FE0`
- **`cannot convert 'T_CCCCCCCC*' {aka 'unsigned int*'} to 'undefined1*' {aka 'unsigned char*'} in assignment`** (2): `0x1026CDF4`, `0x1026D9B1`
- **`no match for 'operator=' (operand types are 'undefined4' and 'void')`** (2): `0x1026D1B4`, `0x1026DAC6`
- **`invalid cast from type 'float' to type 'float*'`** (1): `0x100553C0`
- **`invalid array assignment`** (1): `0x1006F6B0`
- **`'register0x00000010' was not declared in this scope`** (1): `0x101B20E0`
- **`'stack0x00000008' was not declared in this scope`** (1): `0x10285890`

### does not translate (1)

- **import WSACreateEvent: no SDK prototype** (1): `0x1026D634`

### does not export (skipped) (182)

- **in_<reg> (a register read on entry, not a parameter)** (47): `0x10008ED0`, `0x1006AE30` StateBlock_ApplyRSTSS, `0x1006CCE0`, `0x1006E710`, `0x10071E50`, `0x100749B0`, `0x100749F0`, `0x10074A00`, `0x10074A20`, `0x10077BA0`, `0x100814F0`, `0x10081550`, `0x10085240`, `0x10085270`, `0x100977A0`, `0x100A57F0`, `0x100A8770`, `0x100C1190`, `0x100DA750`, `0x100DBE10`, `0x100ED7E0`, `0x101181D0`, `0x1011BA40`, `0x1011D940`, `0x1011FB60`, `0x1011FC80`, `0x10126520`, `0x101284F0`, `0x10129B40`, `0x10129B80`, `0x10153B70`, `0x1015DB80`, `0x10162510`, `0x1018FDF0`, `0x1019E040`, `0x1019E150`, `0x1019E290`, `0x1019E550`, `0x101C9B50`, `0x102014F0`, `0x1021F240`, `0x10221ED0`, `0x10223A90`, `0x10223B10`, `0x1022BF10`, `0x102347D0`, `0x1025BE80`
- **in_stack (stack slot past the parameters)** (25): `0x100150D0`, `0x100151A0`, `0x100151F0`, `0x10015510` Camera_ApplyViewProjection, `0x1006F670`, `0x100FA5C0`, `0x100FA9F0`, `0x101032E0`, `0x1011DAA0`, `0x1011FF70`, `0x101243B0`, `0x10124EB0` Cursor_Draw, `0x10128740`, `0x10129980`, `0x1015BC40`, `0x101620F0`, `0x101622E0`, `0x1019FD50`, `0x101C0BC0`, `0x1021BCC0`, `0x1021BE50`, `0x10251020`, `0x102756A0`, `0x10280DE0` sqgxSprite, `0x102A1F20`
- **extraout_ST (a callee's x87 result)** (24): `0x100016C0`, `0x100171B0`, `0x1001E0A0`, `0x10033360`, `0x10045D00`, `0x100496F0` sub_100496F0, `0x1004A980` sub_1004A980, `0x10053E60`, `0x100C63D0` CXiSkeletonActor_OnMove, `0x100C8930`, `0x100C8BB0`, `0x100D4BC0`, `0x1011A250`, `0x101602F0`, `0x10169140`, `0x1016E7E0`, `0x101892B0`, `0x101CFA00`, `0x101F7570`, `0x1024E760`, `0x1027ACF0`, `0x1027B2C0`, `0x1027D110`, `0x1029C260`
- **unaff_<reg> (a callee-saved register or the return address read)** (24): `0x1000A0A0`, `0x1000AD70`, `0x1000CA00` CDx_GetStereoSecondEyeTarget, `0x1001AD00`, `0x1002A140`, `0x1003B080`, `0x10043320` CMoDistModelElem_BuildDistortedMesh, `0x10045190` sub_10045190, `0x10055F60`, `0x1006F560`, `0x100700A0`, `0x10102340`, `0x1012A340`, `0x1012A4F0`, `0x10183760` sub_10183760, `0x1018D3D0`, `0x101A1CB0`, `0x101BE690`, `0x1021D9B0`, `0x1026EFB0`, `0x1026F080`, `0x1027DD90`, `0x1027EB10`, `0x10281500`
- **extraout_<reg> (a register a callee left)** (22): `0x10026890`, `0x1002C8F0`, `0x10057FB0` CMoSchedularTask_Interpret, `0x10063A60`, `0x10072FD0`, `0x100731B0`, `0x10093E50`, `0x1009B550`, `0x100A4ED0`, `0x100F7790`, `0x100F78C0`, `0x100F7960`, `0x10117520`, `0x10181540`, `0x10189980`, `0x1019CFA0`, `0x1019D320`, `0x1019D350`, `0x101FCFB0`, `0x10202530`, `0x1025BBE0`, `0x102751D0`
- **extraout_var (upper bytes of a callee's return)** (16): `0x10039A80` TexObj_Create, `0x10040BA0`, `0x10076FF0`, `0x10094190`, `0x100AC5A0`, `0x100EE160`, `0x10108460`, `0x10118830`, `0x10119110`, `0x10159BA0`, `0x10162140`, `0x101626E0`, `0x10227BF0`, `0x102573E0`, `0x1025E100`, `0x1025E170`
- **several kinds: in_stack (stack slot past the parameters); unaff_<reg> (a callee-saved register or the return address read)** (4): `0x100412B0` CMoD3mSpecularElem_vf18, `0x100CFF40`, `0x100FA180`, `0x100FA4A0`
- **several kinds: extraout_ST (a callee's x87 result); in_<reg> (a register read on entry, not a parameter)** (3): `0x10005A40`, `0x10175910`, `0x101760F0`
- **several kinds: extraout_<reg> (a register a callee left); in_<reg> (a register read on entry, not a parameter)** (3): `0x10007AE0`, `0x100FB830`, `0x1015EEA0`
- **several kinds: extraout_var (upper bytes of a callee's return); in_<reg> (a register read on entry, not a parameter)** (3): `0x101501A0`, `0x10150B40`, `0x10257310`
- **several kinds: extraout_ST (a callee's x87 result); extraout_var (upper bytes of a callee's return); unaff_<reg> (a callee-saved register or the return address read)** (2): `0x1001EE60`, `0x100A6240`
- **several kinds: extraout_<reg> (a register a callee left); in_stack (stack slot past the parameters)** (2): `0x10128CE0`, `0x102203B0`
- **several kinds: extraout_<reg> (a register a callee left); extraout_var (upper bytes of a callee's return); in_<reg> (a register read on entry, not a parameter)** (1): `0x10021900`
- **several kinds: extraout_<reg> (a register a callee left); extraout_ST (a callee's x87 result); unaff_<reg> (a callee-saved register or the return address read)** (1): `0x1004E8D0` sub_1004E8D0
- **construct: returns the x87 status word** (1): `0x100548D0` sub_100548D0
- **several kinds: extraout_<reg> (a register a callee left); extraout_var (upper bytes of a callee's return); in_<reg> (a register read on entry, not a parameter); unaff_<reg> (a callee-saved register or the return address read)** (1): `0x100C1650`
- **several kinds: extraout_var (upper bytes of a callee's return); in_stack (stack slot past the parameters)** (1): `0x10122E30`
- **several kinds: in_<reg> (a register read on entry, not a parameter); in_stack (stack slot past the parameters)** (1): `0x101299F0`
- **several kinds: extraout_<reg> (a register a callee left); unaff_<reg> (a callee-saved register or the return address read)** (1): `0x10162560`

### no export record (1)

- **Ghidra has no function there** (1): `0x10312CB2`


## What the remaining buckets need

**An in-game capture (next owner kit).** The fuzzed inputs are not enough, and no fact in the repository gives
them; a capture of the real call (xcap.py replay, or a live dump for `rawlive`) would:
- INCONCLUSIVE, the original faults on every trial (26) or runs out of instructions (8), or both (2): the
  function reads object state the fuzzer cannot build. Capture each one's `this`/arguments in play:
  0x100058F0, 0x10017010, 0x10047960, 0x10056C70, 0x10057D60, 0x10071E90, 0x10071F60, 0x10071F70, 0x10071F80,
  0x1008ABE0, 0x1008E290, and the rest of the `fault` line above; `limit`: 0x10072DB0, 0x10108600, 0x101086B0,
  0x101C2B60, 0x101C2C80, 0x101C4080, 0x10270840, 0x102709E0; `fault,limit`: 0x10056BB0, 0x101D01B0.
- UNRESOLVED, `virtual call on a fuzzed object ... target unknown` (2): 0x100D4560 CXiSkeletonActor_GetElem,
  0x100D4800 CXiSkeletonActor_GetElemLocal: the object's vtable in play names the target.
- UNRESOLVED, `call N: pop count unknown` (6): 0x1000ABE0, 0x1003BA20, 0x10069DD0, 0x1006A070,
  0x1006C140 CMoOcclusionMng_Constructor, 0x1021DD50. sp_solve.py leaves these calls out; the real code run
  (real_sp.py) or a capture of the call gives the pops.

**The integrator.** The seven PASS raw functions left out of the tree (`tree/left_out.json`): six call a function
reading a register its tree wrapper does not pass, and 0x10018850 does not compile in the tree. The 45 new raw
PASSes are ready for the regeneration.

**No definitive fix in this pass (pipeline facts or translator work, named):**
- does not export (182): Ghidra's text keeps a value no parameter carries. The classes, from the variable names:
  a register read on entry (`in_EDX` mostly: 47, often the entry EDX handed back in EDX:EAX on a path, which
  `regs_after.json`'s may-analysis says some caller may read, so the return cannot be narrowed); a stack slot past
  the parameters (25); a callee's x87 result where the callee's ST0 return is not established (24); a callee-saved
  register or the return address (24); a register a callee left (22); the upper bytes of a callee's return (16).
  Each needs an exact fact (which callers read EDX; the callee's float return; the argument count) that is not in
  `data/` today.
- FAIL (72): the first mismatches are spread over many causes; the largest kinds are a call differing in an
  argument or ECX (24; several pass 0xCCCCCCCC, the original's uninitialised stack, where the reconstruction
  passes its own), memory written (18), EAX at return (16; 0x100274B0 and 0x1006D5D0 return the x87 status word,
  whose TOP field the translation does not model), a reconstruction fault (10), EDX:EAX (3), x87 result (1).
- does not compile (30) and the rest: as listed above.

## Newly reached functions (frame set of 93185fe)

Not counted above. 93185fe ("frame set follows the recorded indirect targets", #106) grew `data/frame_set.json`
from 4,984 to 5,433 functions; no function left it. Of the **449** added (448 reached by a recorded indirect
target, one static):

| Status | Functions |
|---|---:|
| in the tree (all PASS) | 10 |
| library by address (0x102D0000 on: hx2cpp.py's rule) | 3 |
| game code by address, outside the tree, never exported | 436 |
| total | 449 |

The 436 have no Ghidra or IDA record yet (only one has a name in the IDA export), so hx2cpp's by-name library
rule has not been applied to them; their buckets come from a first export (an fs23 run of these addresses).
In the tree: 0x10002C00 Gfx_SetGammaRGB, 0x10002C30 Gfx_SetGammaR, 0x10002C80 Gfx_SetGammaG,
0x10002CD0 Gfx_SetGammaB, 0x1003CB30 CMoCamera_SetPathData, 0x1003EF80 CMoVuAnmElem_vf012,
0x10041120 CMoElemFamily_IsKind0, 0x10052DD0 CMoVuDist_vf012, 0x1006D4D0 CMoProcessor_VT_1032C368_vf014,
0x100D08E0 CXiSkeletonActor_GetWidthScale. Library: 0x103108E9, 0x103108F4, 0x10311856.

Game code, not exported (436):

`0x10002BE0`, `0x10002D50`, `0x10002D70`, `0x10002DA0`, `0x10002DC0`, `0x10002DE0`, `0x10002E00`, `0x10002E20`, `0x10002E40`, `0x10014330`, `0x10019230`, `0x10022320`, `0x10022340`, `0x10035E80`, `0x10035EC0`, `0x100372A0`, `0x100372E0`, `0x100493C0`, `0x100569D0`, `0x10056B80`, `0x10057010`, `0x10062ED0`, `0x1006D190`, `0x1006D9C0`, `0x10071E00`, `0x100747C0`, `0x10079170`, `0x1007A7A0`, `0x1007A9A0`, `0x1007AA80`, `0x1007B710`, `0x1007B820`, `0x1007B900`, `0x1007BB90`, `0x1007D240`, `0x1007E490`, `0x1007E8D0`, `0x10080340`, `0x10082460`, `0x10082780`, `0x10086FD0`, `0x10087630`, `0x10087650`, `0x10087B20`, `0x1008E220`, `0x10096220`, `0x10096250`, `0x100962C0`, `0x100962F0`, `0x10096350`, `0x10096380`, `0x100963B0`, `0x100963E0`, `0x10096410`, `0x10096470`, `0x10097B40`, `0x10097B50`, `0x10097E80`, `0x10098080`, `0x100980C0`, `0x10098100`, `0x10099680`, `0x1009AB20`, `0x100A0010`, `0x100A0080`, `0x100A17F0`, `0x100A1860`, `0x100A2790`, `0x100A28E0`, `0x100A2B70`, `0x100A2FD0`, `0x100A3040`, `0x100A30E0`, `0x100A8C20`, `0x100A8C40`, `0x100ABD90`, `0x100ABDA0`, `0x100ABE30`, `0x100ABE40`, `0x100ABFB0`, `0x100AC9C0`, `0x100AD660`, `0x100AEC20`, `0x100C3FF0`, `0x100C4050`, `0x100C4BC0`, `0x100C5410`, `0x100C5440`, `0x100D6F50`, `0x100D8AA0`, `0x100D8AE0`, `0x100D8B20`, `0x100D8BA0`, `0x100D8DE0`, `0x100D92A0`, `0x100D9320`, `0x100DC710`, `0x100E16A0`, `0x100E1710`, `0x100E1740`, `0x100E1820`, `0x100E1F20`, `0x100E2060`, `0x100E2860`, `0x100E63B0`, `0x100EB4F0`, `0x100EBE60`, `0x100F0C80`, `0x100F1080`, `0x100F2020`, `0x100F3380`, `0x100F7BC0`, `0x100F7FA0`, `0x100F9600`, `0x100F9680`, `0x100FC300`, `0x100FD5B0`, `0x100FD6E0`, `0x101061B0`, `0x101061D0`, `0x10106CD0`, `0x10106DE0`, `0x10106EB0`, `0x10107120`, `0x101072B0`, `0x101072D0`, `0x10107300`, `0x10107320`, `0x10107340`, `0x10107370`, `0x10118530`, `0x10118BC0`, `0x101192B0`, `0x1011A710`, `0x1011A8F0`, `0x1011ADB0`, `0x1011D380`, `0x1011D440`, `0x1011D970`, `0x101262E0`, `0x10127320`, `0x10127F60`, `0x10128060`, `0x10129090`, `0x101293F0`, `0x10129490`, `0x101295D0`, `0x10129C20`, `0x1012AC50`, `0x1012AC80`, `0x1012AED0`, `0x1012AF30`, `0x1012B2D0`, `0x1012B4B0`, `0x1012BF50`, `0x101304A0`, `0x10133A40`, `0x101369B0`, `0x10136A20`, `0x10137550`, `0x1013BFB0`, `0x1013BFC0`, `0x1013C110`, `0x1013C180`, `0x1013E3B0`, `0x1013E4F0`, `0x1013E560`, `0x1013E590`, `0x1013EFF0`, `0x10140890`, `0x101408A0`, `0x10150870`, `0x101508F0`, `0x10150A70`, `0x10155D10`, `0x10155D20`, `0x10155D30`, `0x10155D40`, `0x10155D50`, `0x10155D60`, `0x10155D70`, `0x10155EA0`, `0x10155F00`, `0x10155F60`, `0x10156080`, `0x101565C0`, `0x101565D0`, `0x101565E0`, `0x10156630`, `0x10156660`, `0x10156930`, `0x10156940`, `0x10156950`, `0x10156960`, `0x101569B0`, `0x10156BA0`, `0x10156C00`, `0x10157850`, `0x10157CB0`, `0x10157CF0`, `0x10159C80`, `0x10159CC0`, `0x1015A680`, `0x1015BB80`, `0x1015DF60`, `0x1015F760`, `0x10160C80`, `0x10161870`, `0x10161890`, `0x101618D0`, `0x10161910`, `0x10161BA0`, `0x10161BE0`, `0x10161C20`, `0x101624B0`, `0x10162CC0`, `0x10162D90`, `0x10162E00`, `0x10162E20`, `0x10162E60`, `0x10162E90`, `0x10162EA0`, `0x10162EF0`, `0x10162F30`, `0x10162FE0`, `0x10163020`, `0x10163050`, `0x10163070`, `0x10168FC0`, `0x1016BDF0`, `0x1016C5D0`, `0x1016CB60`, `0x10177730`, `0x10177750`, `0x1017A590`, `0x101815D0`, `0x10181930`, `0x1018AA50`, `0x1018AAE0`, `0x1018AB60`, `0x1018DA10`, `0x1018DA20`, `0x1018DFD0`, `0x1018E620`, `0x1018E780`, `0x1018E7D0`, `0x1018E890`, `0x1018EBC0`, `0x1018EBF0`, `0x1018EC10`, `0x1018EC50`, `0x1018ECE0`, `0x10190B50`, `0x101938D0`, `0x10196F60`, `0x10197030`, `0x101978E0`, `0x10197A70`, `0x101A1790`, `0x101A17E0`, `0x101A1840`, `0x101A1E20`, `0x101A23D0`, `0x101A2410`, `0x101A2440`, `0x101A2870`, `0x101A2980`, `0x101A2A10`, `0x101A40B0`, `0x101AA650`, `0x101AAA90`, `0x101ABBD0`, `0x101ABBF0`, `0x101AEB80`, `0x101B0060`, `0x101B3AE0`, `0x101B3F00`, `0x101B3FC0`, `0x101B4030`, `0x101B40C0`, `0x101B4340`, `0x101B4360`, `0x101B5080`, `0x101B5850`, `0x101B5A80`, `0x101B5AA0`, `0x101B5CD0`, `0x101B7160`, `0x101B7890`, `0x101B7E90`, `0x101B91B0`, `0x101BF710`, `0x101BF7F0`, `0x101C1F80`, `0x101C2030`, `0x101C4800`, `0x101C4D50`, `0x101C8B80`, `0x101CBC60`, `0x101CBE20`, `0x101CBE80`, `0x101CBE90`, `0x101CBEC0`, `0x101CC040`, `0x101CC0A0`, `0x101CC0D0`, `0x101CC1B0`, `0x101CC320`, `0x101CC4C0`, `0x101CCE50`, `0x101CCE90`, `0x101CD2D0`, `0x101CD990`, `0x101CD9A0`, `0x101CDD90`, `0x101CDF50`, `0x101CE1E0`, `0x101CE9E0`, `0x101CEA30`, `0x101CEA80`, `0x101CEAC0`, `0x101CEAD0`, `0x101CEAE0`, `0x101CEAF0`, `0x101CEB40`, `0x101CEBE0`, `0x101CEBF0`, `0x101CEC50`, `0x101CECB0`, `0x101CEEF0`, `0x101D3380`, `0x101D3910`, `0x101D39A0`, `0x101D3E70`, `0x101D3F60`, `0x101D3FB0`, `0x101D55D0`, `0x101D5E00`, `0x101D5E40`, `0x101D5EA0`, `0x101D5FE0`, `0x101D6020`, `0x101D6C90`, `0x101D6CC0`, `0x101E15C0`, `0x101E7E90`, `0x101EE040`, `0x101EF990`, `0x101EFAD0`, `0x101EFB10`, `0x101EFFD0`, `0x101EFFE0`, `0x101F1100`, `0x101F1160`, `0x101F11E0`, `0x101F1230`, `0x101F23A0`, `0x101F35A0`, `0x101F4B20`, `0x101F66B0`, `0x101F6720`, `0x101F68B0`, `0x101F68F0`, `0x101F7020`, `0x101F7080`, `0x101F7100`, `0x101F74E0`, `0x101F79E0`, `0x101F7C00`, `0x101F7DB0`, `0x101F7EC0`, `0x101F7FE0`, `0x101F8020`, `0x101F80B0`, `0x101F8360`, `0x101F85F0`, `0x101F8650`, `0x101F86B0`, `0x101F8EA0`, `0x101F8FB0`, `0x101F9010`, `0x101F9110`, `0x101F92E0`, `0x101F93D0`, `0x101F9A20`, `0x101F9A50`, `0x101FA680`, `0x101FAAB0`, `0x101FAAF0`, `0x101FAD00`, `0x101FAD40`, `0x101FAD70`, `0x101FAFF0`, `0x101FB1B0`, `0x10201690`, `0x10203120`, `0x10203430`, `0x10203480`, `0x102034C0`, `0x10207360`, `0x1021CB80`, `0x1021CCB0`, `0x10221300`, `0x102231F0`, `0x10223240`, `0x10223360`, `0x10223390`, `0x102233E0`, `0x10223760`, `0x10223B4E`, `0x10223C00`, `0x10224120`, `0x10224160`, `0x102260C0`, `0x102262B0`, `0x102262EE`, `0x10226320`, `0x1022635E`, `0x10227000`, `0x1022B850`, `0x1022B8E0`, `0x1022C000`, `0x1022C730`, `0x1022C770`, `0x1022D290`, `0x1022D370`, `0x1022D510`, `0x10234610`, `0x102391D0`, `0x10239240`, `0x1023C540`, `0x10241800`, `0x10244E60`, `0x10245460`, `0x102454E0`, `0x10245520`


## The 436 newly reached, exported and gated (fs24)

`runs/fs24/addrs.txt` is the 436 addresses above. Export: `data/exports/ghidra_frameset24.jsonl` (all 436 have a
Ghidra function). Raw gate: `verification/gate_raw_frameset24.jsonl` (401 functions). The first pass (no
`XI_RETW`; scratch, not committed) left 38 functions UNRESOLVED with `EAX at return differs past the declared
void; no fact says no caller reads it` and no other blocker. P20 settles that class, so the committed export is
the second pass: the same 436 with `XI_RETW` listing exactly those 38 (marked `(retw)` below). All 38 PASS raw in
it. No other function changed bucket between the passes (73 records' text changed: the 38 and their callers
within the 436).

Library code by hx2cpp.py's rules (D3DX 8.1 by the manifest or a `D3DX` name, or 0x102D0000 on): none of the 436
(no `status: library` in the translation's meta.json; all lie below 0x102D0000, and no exported name matches).

| blocker (fs24) | functions |
|---|---:|
| PASS raw | 387 |
| FAIL | 9 |
| INCONCLUSIVE | 3 |
| UNRESOLVED | 2 |
| does not compile | 2 |
| does not export (skipped) | 33 |
| total | 436 |

Notes, from the records named:
- Three of the PASSes (0x10223B4E, 0x102262EE, 0x1022635E, Ghidra's `caseD_1`) are switch case blocks: each is
  the one target recorded at a `jmp dword ptr [eax*4 + T]` site (`indirect_targets.json`: kind `jmp`, reason
  `jmp_switch`, in 0x10223B10, 0x102262B0, 0x10226320), and none is a recorded call target. frame_set.py follows a
  recorded target of any kind when `func_extents.json` lists it as a function entry, and it lists these three.
  The gate runs each as a function entered by a call; no record used here shows anything calling them. Not
  changed in this pass.
- UNRESOLVED (2): 0x10163020 and 0x10163050 make a virtual call (`call dword ptr [eax + 0x54]` at 0x10163026 /
  0x10163056) whose pops no source gives; sp_solve.py leaves it out.
- does not compile (2): 0x100D8B20 passes `(LPLONG)0x0` to the import at 0x103291D8; `xi_compat_win.hpp` does
  not declare the SDK's `LPLONG`. 0x101CC0D0 uses Ghidra's `CONCAT14`, which `xi_ghidra.hpp` does not define (it
  has CONCAT11..13 and CONCAT17). Neither added in this pass.
- FAIL (9), first mismatches (trial 0 unless named): 0x100372A0 and 0x100372E0 `memory differs at 4 byte(s),
  first 0x103517F4 (0x103517F8): original 52 / reconstruction 24`: the record's `type` declares
  `undefined4 param_1` while its text takes `float param_1` and stores it into a float global; the translation
  takes the `undefined4`, so the store converts the integer to float instead of copying the bits. 0x1015A680
  `stubbed call #2 differs: original 0x10311BBB(ecx=0, 0x1C) / reconstruction 0x10311BBB(ecx=0, 0x38)` (the
  switch on `*(uint *)(DAT_10456a28 + 8)` reaches another case than the original). 0x10162EA0 `memory differs at
  4 byte(s), first 0x30000F84: original 52 / reconstruction 148`. 0x101CBE80, 0x101CBE90, 0x101CBEC0 `EDX:EAX
  differs` (each returns the `undefined8` of 0x101CBE20, called with the entry EAX). 0x101CDD90 (trial 14)
  `memory differs at 2 byte(s), first 0x30C78D14: original 253 / reconstruction 255`. 0x101EF990 (trial 3)
  `EAX 0x00008184 vs 0x00000100`. The causes past the 0x100372A0/0x100372E0 one are not read.
- INCONCLUSIVE (3): the original faults on every trial (0x10056B80, 0x101CEC50, 0x101D6CC0): an in-game capture.

### Every function by bucket (fs24)

#### PASS raw (387)

- `0x10002BE0`, `0x10002D50`, `0x10002D70`, `0x10002DA0`, `0x10002DC0`, `0x10002DE0`, `0x10002E00`, `0x10002E20`, `0x10002E40`, `0x10014330`, `0x10019230`, `0x10022320`, `0x10035E80`, `0x10035EC0`, `0x100493C0`, `0x100569D0` (retw), `0x10057010`, `0x10062ED0`, `0x1006D190`, `0x1006D9C0`, `0x10071E00` (retw), `0x100747C0`, `0x10079170`, `0x1007A9A0`, `0x1007AA80`, `0x1007BB90`, `0x1007D240`, `0x1007E490`, `0x1007E8D0`, `0x10080340`, `0x10082460`, `0x10082780`, `0x10086FD0` ActerBuff_FindByField78, `0x10087630`, `0x10087650`, `0x10087B20`, `0x1008E220`, `0x10096220`, `0x10096250`, `0x100962C0`, `0x100962F0`, `0x10096350`, `0x10096380`, `0x100963B0`, `0x100963E0`, `0x10096410`, `0x10096470`, `0x10097B40`, `0x10097B50`, `0x10097E80`, `0x10098080`, `0x100980C0`, `0x10098100`, `0x10099680`, `0x1009AB20`, `0x100A0010`, `0x100A0080`, `0x100A17F0` (retw), `0x100A1860` (retw), `0x100A2790`, `0x100A28E0`, `0x100A2B70`, `0x100A2FD0` (retw), `0x100A3040`, `0x100A30E0`, `0x100A8C20`, `0x100A8C40`, `0x100ABD90`, `0x100ABDA0`, `0x100ABE30`, `0x100ABE40`, `0x100ABFB0`, `0x100AC9C0`, `0x100AD660`, `0x100AEC20` (retw), `0x100C3FF0`, `0x100C4050`, `0x100C4BC0`, `0x100C5410` (retw), `0x100C5440` (retw), `0x100D6F50` (retw), `0x100D8AA0`, `0x100D8AE0`, `0x100D8BA0`, `0x100D8DE0`, `0x100D92A0`, `0x100D9320`, `0x100DC710`, `0x100E16A0`, `0x100E1740`, `0x100E1F20`, `0x100E2060`, `0x100E63B0`, `0x100F0C80`, `0x100F1080`, `0x100F3380`, `0x100F7BC0`, `0x100F7FA0`, `0x100F9600`, `0x100F9680`, `0x100FC300`, `0x100FD5B0`, `0x100FD6E0`, `0x101061B0`, `0x101061D0`, `0x10106CD0`, `0x10106DE0`, `0x10106EB0`, `0x10107120`, `0x101072B0`, `0x101072D0`, `0x10107300`, `0x10107320`, `0x10107340`, `0x10107370`, `0x10118530`, `0x10118BC0`, `0x101192B0`, `0x1011A710`, `0x1011ADB0` (retw), `0x1011D380`, `0x1011D440`, `0x1011D970`, `0x101262E0`, `0x10127320`, `0x10127F60`, `0x10128060`, `0x101293F0`, `0x10129490`, `0x10129C20`, `0x1012AC50`, `0x1012AC80`, `0x1012B2D0`, `0x1012B4B0`, `0x1012BF50`, `0x101304A0`, `0x10133A40`, `0x101369B0`, `0x10136A20`, `0x10137550`, `0x1013BFB0` thunk_FUN_1013e590, `0x1013BFC0`, `0x1013C110`, `0x1013C180` (retw), `0x1013E3B0`, `0x1013E4F0`, `0x1013E560` (retw), `0x1013E590`, `0x1013EFF0`, `0x10140890`, `0x101408A0` (retw), `0x10150870`, `0x101508F0`, `0x10150A70`, `0x10155D10`, `0x10155D20`, `0x10155D30`, `0x10155D40`, `0x10155D50`, `0x10155D60`, `0x10155D70`, `0x10155EA0`, `0x10155F00`, `0x10155F60`, `0x10156080`, `0x101565C0`, `0x101565D0`, `0x101565E0`, `0x10156630`, `0x10156660`, `0x10156930`, `0x10156940`, `0x10156950`, `0x10156960`, `0x101569B0`, `0x10156BA0`, `0x10156C00`, `0x10157850`, `0x10157CB0`, `0x10157CF0`, `0x10159C80`, `0x10159CC0`, `0x1015BB80`, `0x1015DF60`, `0x1015F760`, `0x10160C80`, `0x10161870`, `0x10161890`, `0x101618D0`, `0x10161910`, `0x10161BA0`, `0x10161BE0`, `0x10161C20`, `0x101624B0`, `0x10162CC0`, `0x10162E00`, `0x10162E20`, `0x10162E60`, `0x10162E90` thunk_FUN_10162ef0, `0x10162EF0`, `0x10162F30`, `0x10162FE0`, `0x10163070`, `0x10168FC0`, `0x1016BDF0`, `0x1016C5D0`, `0x1016CB60`, `0x1017A590`, `0x101815D0`, `0x10181930`, `0x1018AA50`, `0x1018AAE0`, `0x1018AB60`, `0x1018DA10`, `0x1018DA20` (retw), `0x1018DFD0` (retw), `0x1018E620`, `0x1018E780`, `0x1018E7D0`, `0x1018E890`, `0x1018EBC0`, `0x1018EBF0`, `0x1018EC10`, `0x1018EC50`, `0x1018ECE0`, `0x10190B50`, `0x101938D0`, `0x10196F60`, `0x10197030`, `0x101978E0`, `0x10197A70`, `0x101A1790` (retw), `0x101A17E0`, `0x101A1840`, `0x101A1E20` (retw), `0x101A23D0`, `0x101A2410`, `0x101A2440`, `0x101A2870`, `0x101A2980` (retw), `0x101A2A10` (retw), `0x101AA650`, `0x101AAA90`, `0x101ABBD0`, `0x101ABBF0`, `0x101AEB80`, `0x101B0060`, `0x101B3F00`, `0x101B3FC0`, `0x101B4030`, `0x101B40C0`, `0x101B4340`, `0x101B4360`, `0x101B5080`, `0x101B5850`, `0x101B5A80`, `0x101B5AA0`, `0x101B5CD0` (retw), `0x101B7160`, `0x101B7890`, `0x101B7E90` (retw), `0x101B91B0`, `0x101BF710`, `0x101BF7F0`, `0x101C1F80`, `0x101C2030`, `0x101C4800`, `0x101C4D50`, `0x101CBC60` thunk_FUN_10097e80, `0x101CC0A0`, `0x101CC1B0`, `0x101CC320`, `0x101CCE50`, `0x101CCE90`, `0x101CD2D0`, `0x101CD990`, `0x101CD9A0`, `0x101CDF50`, `0x101CE1E0`, `0x101CE9E0`, `0x101CEA30`, `0x101CEA80`, `0x101CEAC0`, `0x101CEAD0`, `0x101CEAE0`, `0x101CEAF0`, `0x101CEB40`, `0x101CEBE0`, `0x101CEBF0`, `0x101CECB0`, `0x101CEEF0`, `0x101D3380` (retw), `0x101D3910` (retw), `0x101D39A0`, `0x101D3E70`, `0x101D3F60`, `0x101D3FB0`, `0x101D55D0`, `0x101D5E00`, `0x101D5E40`, `0x101D5EA0`, `0x101D5FE0`, `0x101D6020`, `0x101D6C90`, `0x101E15C0`, `0x101E7E90`, `0x101EE040`, `0x101EFAD0` (retw), `0x101EFB10`, `0x101EFFD0`, `0x101EFFE0`, `0x101F23A0`, `0x101F35A0`, `0x101F4B20` (retw), `0x101F66B0`, `0x101F6720` (retw), `0x101F68F0`, `0x101F7020` (retw), `0x101F7080`, `0x101F7100`, `0x101F74E0` thunk_FUN_10097e80, `0x101F79E0`, `0x101F7C00`, `0x101F7DB0`, `0x101F7EC0`, `0x101F7FE0`, `0x101F8020`, `0x101F80B0`, `0x101F8360`, `0x101F85F0`, `0x101F8650`, `0x101F86B0`, `0x101F8EA0`, `0x101F8FB0`, `0x101F9010`, `0x101F9110` (retw), `0x101F92E0`, `0x101F93D0`, `0x101F9A20`, `0x101FA680` (retw), `0x101FAAB0`, `0x101FAAF0`, `0x101FAD00` (retw), `0x101FAD40`, `0x101FAD70`, `0x101FAFF0` (retw), `0x101FB1B0`, `0x10201690`, `0x10203120`, `0x10203430`, `0x10203480`, `0x102034C0`, `0x10207360`, `0x1021CB80`, `0x1021CCB0`, `0x10221300`, `0x102231F0`, `0x10223240`, `0x10223360`, `0x10223390`, `0x102233E0`, `0x10223760`, `0x10223B4E` caseD_1, `0x10223C00` (retw), `0x10224120`, `0x10224160`, `0x102260C0`, `0x102262EE` caseD_1, `0x1022635E` caseD_1, `0x10227000`, `0x1022B850`, `0x1022B8E0`, `0x1022C000` (retw), `0x1022C730`, `0x1022C770`, `0x1022D290` (retw), `0x1022D370`, `0x1022D510`, `0x10234610` (retw), `0x102391D0`, `0x10239240` (retw), `0x1023C540`, `0x10241800` (retw), `0x10244E60` (retw), `0x10245460`, `0x102454E0`, `0x10245520`

#### FAIL (9)

- **memory written differs** (4): `0x100372A0`, `0x100372E0`, `0x10162EA0`, `0x101CDD90`
- **EDX:EAX at return differs** (3): `0x101CBE80`, `0x101CBE90`, `0x101CBEC0`
- **a call differs (target, ECX or an argument)** (1): `0x1015A680`
- **EAX at return differs** (1): `0x101EF990`

#### INCONCLUSIVE (3)

- **original outcomes: fault** (3): `0x10056B80`, `0x101CEC50`, `0x101D6CC0`

#### UNRESOLVED (2)

- **call N: pop count unknown** (2): `0x10163020`, `0x10163050`

#### does not compile (2)

- **'LPLONG' was not declared in this scope; did you mean 'LONG'?** (1): `0x100D8B20`
- **'CONCAT14' was not declared in this scope; did you mean 'CONCAT11'?** (1): `0x101CC0D0`

#### does not export (skipped) (33)

- **in_<reg> (a register read on entry, not a parameter)** (22): `0x1007A7A0`, `0x1007B710`, `0x1007B820`, `0x1007B900`, `0x100E1710`, `0x100EB4F0`, `0x100EBE60`, `0x1012AED0`, `0x1012AF30`, `0x10177730`, `0x10177750`, `0x101A40B0`, `0x101B3AE0`, `0x101CBE20`, `0x101CC040`, `0x101CC4C0`, `0x101F1100`, `0x101F1160`, `0x101F11E0`, `0x101F68B0`, `0x102262B0`, `0x10226320`
- **in_stack (stack slot past the parameters)** (5): `0x100E1820`, `0x100E2860`, `0x10129090`, `0x101295D0`, `0x101C8B80`
- **extraout_<reg> (a register a callee left)** (3): `0x10022340`, `0x101F1230`, `0x101F9A50`
- **extraout_var (upper bytes of a callee's return)** (2): `0x100F2020`, `0x10162D90`
- **unaff_<reg> (a callee-saved register or the return address read)** (1): `0x1011A8F0`
