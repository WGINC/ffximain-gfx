# The TkMenu UI layer: the owner's documentation checked against the game

`docs/owner_ui/` holds the owner's UI documentation as given: the TkMenu function declarations
(`TkMenuManagerFunctionDefinitions.ixx`), their byte signatures (`tkmenu_signatures.txt`), the hooks that use them
(`tkmenu_hooks.cpp`), struct layouts (`CTk*.ixx`) and the IDA strings list of menu names (`menu_strings.txt`).
Nothing in those files is used as a fact until it is checked against FFXiMain.dll. `pipeline/ui_sigs.py` checks
it, and its output is `data/ui_functions.json`, rebuilt by `verify_all.py`.

## Functions (data/ui_functions.json)

Each signature matches **exactly once** in `.text`, at an address `data/purges.json` already lists as a function
start. Two checks compare the declaration with the game's code:

- "ret N = declared": the callee's own `ret N` equals the stack bytes the declaration passes (every parameter after
  `this` for `__thiscall`, 4 bytes each).
- "cc = declared": whether ECX is an input (read before written on some path, all paths followed) agrees with
  `__thiscall` or `__stdcall`.

| Signature | Address | Declared cc | `ret N` | ret N = declared | cc = declared |
|---|---|---|---|---|---|
| fnCloseDrawMenu | 0x1015E6E0 | thiscall | 20 | yes | yes |
| fnCreateDrawMenu | 0x1015E350 | thiscall | 12 | yes | yes |
| fnConvertMenuId | 0x10160760 | thiscall | 8 | yes | yes |
| fnGetFromPrimTable | 0x1015DC70 | stdcall | 8 | yes | yes |
| fnIsEnableCreateDrawMenu | 0x1015E0F0 | thiscall | 4 | yes | yes |
| GetResourceMenu | 0x1015E030 | thiscall | 4 | yes | yes |
| GetMenuCtrlData | 0x1015DC10 | thiscall | 4 | yes | yes |
| TkMenuCtrlDataRePosition | 0x10119110 | thiscall | 8 | yes | yes |
| TkMenuCtrlDataResizeWindow | 0x1011A5A0 | thiscall | 28 | yes | yes |
| fnDestroyDrawMenu | 0x1015E910 | thiscall | 4 | yes | yes |
| fnOnKeyDown | 0x1015DD00 | thiscall | 4 | yes | yes |

Notes, each with its source:

- **The menu manager is a static object at 0x10621838**, not a pointer. At most direct call sites the instruction
  before the call is `mov ecx, 0x10621838`: 457 of 491 for CreateDrawMenu, 519 of 556 for DestroyDrawMenu, and
  35 of 40 for CloseDrawMenu (`this_at_calls`; this is a listing over a linear decode, not a proof of the
  object's type).
- **GetFromPrimTable's second parameter is an output flag, not a string.** The code
  (0x1015DCA6–0x1015DCC1) never reads through it. It writes one byte to it, 0 when the name is not in the table
  and 1 when it is. The table starts at 0x103722D8, has 0x2C-byte entries, and compares the first 0x10 bytes of
  each name (the cdecl compare at 0x10312940). The function returns `*[entry+0x20]`, or 0 when the name is not
  found.
- **The calling convention is settled through a callee.** GetFromPrimTable reaches the call to 0x10312940 before
  it writes ECX. That callee writes ECX (`mov ecx,[ebp+10h]`) before any read or branch, so ECX is not an input
  of either function.

## Layout fields the code agrees with

These are matches, not proofs of the whole layout:

| Field | Owner's layout | Instruction in the game |
|---|---|---|
| CTkMenuMng `m_ResourceDatas` | +0x1C | GetResourceMenu (0x1015E030): `lea edi,[ecx+1Ch]` |
| CTkMenuCtrlData `m_DrawShapes` | +0x14 | ResizeWindow (0x1011A5A0): `lea ecx,[esi+14h]` |
| CTkMenuPrimitive `m_pParentMCD` | +0x08 | RePosition and ResizeWindow get `this` from `mov ecx,[reg+8]` at 59 of their 67 and 26 of their 40 call sites |

## Relation to the renderer: established by direct calls (28 Oct)

From the per-menu draw that ffxi_hide_ui's kSigMenuDraw locates (0x101177D0, our Menu_DrawWindowItems), the
game's own direct calls reach the renderer: 0x101177D0 calls the sprite draw 0x1011FC80 (kSigSpriteDraw), which
reaches DrawPrimitiveUP (0x1000CD00, kSigPrimitiveDraw) through 0x1011E980 and 0x1011F300; within four calls of
the per-menu draw lie two of the nine Cell_/Font_ callers below, 0x1011F2D0 and 0x10120000. The walk of 26 Oct
went up from Cell_/Font_ to the manager's eleven functions, and the per-menu draw is not one of them
(data/genoxd_scan.json, docs/COMMUNITY.md).

The earlier finding, kept as written:


- The 10 Cell_/Font_ draw functions in the export have 9 direct callers outside it: 0x100D7FA0, 0x1011BBA0,
  0x1011BE10, 0x1011ED70, 0x1011F2D0, 0x1011FF70, 0x10120000, 0x1013A350 and 0x10150B40.
- Most of these lie near the CTkMenuCtrlData methods above. Being near in address is not evidence of being
  the same class.
- Walking direct callers up four levels from Cell_/Font_ reaches none of the 11 functions above. The menu
  manager's functions create, find and destroy menus; the draw path to Cell_/Font_ is not found by direct calls
  alone (it may go through vtables, which this walk does not follow).
