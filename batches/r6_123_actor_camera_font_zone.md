# r6_123 -- Field layouts of the actor, camera, zone and font objects (task #123, plan step 25)

Not applied: the integrator applies it through the tree regeneration and gates what changes. The CDx, model, skin and
skeleton objects are #122's (batches/r6_122_render_objects.md) and are not typed here. CXiSkeletonActor and the
classes derived from it are skeleton-bearing actors: their own members (0x5F8 and up) are left out; their methods are used
only for the members they inherit from the classes below (offsets under 0x5F8).

## Which classes, and how they were identified

Every class with a vtable is identified by **the game's own runtime-class descriptor**: vtable slot 0 is a getter
`mov eax, DESC; ret`, and `DESC` is `{char *name; uint32 size; DESC *parent}` (e.g. CMoCamera: 0x1003C880 returns
0x1032AB28 = {"CMoCamera", 0xB0, 0x103515A0 = {"CYyObject", 4, ...}}). The size field is the object size: where an
allocation was found it matches (CYyCamMng2 0x118 at 0x1001E470, XiZone 0x1DC at 0x1018B139; also CDx 0xA50, #122).
135 such descriptors exist; the ones in scope:

| Group | Class | Size | Parents (descriptor chain) | vtable |
|---|---|---|---|---|
| actor | CXiActor | 0xC4 | CMoTask (0x34) : CYyObject (4) | 0x1032CFE0 |
| actor | CXiAtelActor | 0xD4 | CXiActor | 0x1032E3C8 |
| actor | CXiControlActor | 0x5C4 | CXiAtelActor | 0x1032DFC8 |
| actor | CXiCollisionActor | 0x5F8 | CXiControlActor | 0x1032DB40 |
| actor | CXiDollActor | 0x440 | CXiActor | 0x1032F938 |
| actor | XiLiftActor | 0x600 | descriptor: CXiAtelActor; code: CXiControlActor (see below) | 0x10330288 |
| actor | XiDoorActor | 0x894 | CXiControlActor | 0x1032FD58 |
| actor | XiModelActor | 0x794 | CXiControlActor | 0x10330AA0 |
| actor | XiFurnitureActor | 0x794 | XiModelActor (same size: no members of its own) | 0x103306A0 |
| camera | CMoCamera | 0xB0 | CYyObject | 0x1032C5D0 |
| camera | CYyCamMng2 | 0x118 | CYyObject | 0x1032A37C |
| camera | CMoCameraTask | 0xC8 | CMoTask | 0x1032AB60 |
| zone | XiZone | 0x1DC | CYyObject (g_pXiZone 0x1065CB04) | 0x10336838 |
| zone | CTsZoneMap | 0x395B4 | CMoTask (g_pZoneMap 0x10631F24) | 0x10336748 |
| font | Obj_104E1B90 | 0x84 | none (no descriptor, no vtable) | none |
| font | Obj_104E1BF8 | not known | none (static object) | none |

Out of scope but seen: CXiSkeletonActor 0xA0C (descriptor 0x10330EBC) : CXiCollisionActor; its derived CXiDancerActor,
CXiGeomActor, CXiMannequinActor (0xCB8), CXiSklFurnitureActor (0xCBC), XiSkeletonActor2 / XiChocoboActor2 (0xA34).

"Font" objects: neither has a descriptor, so their class names are not known. Obj_104E1B90 is the 0x84-byte object
whose pointer is 0x104E1B90 (App_InitScene 0x100111BB: `push 0x84; call Allocator_Generic`, constructor 0x1011BA90);
its methods bind the "FontMoji" texture (0x1011BAE0) and draw glyph quads (0x1011BBD0, 0x1011C080). Obj_104E1BF8 is a
static object whose methods look up the "font fontshp" shape and bind the font and damage-number textures
(0x10120510, 0x10120590, 0x101207B0); its size is not known. A "camera" class for the camera/view matrix used by
Camera_GetViewMatrixPtr was not found among the descriptors; it is not typed here.

## Method (repeatable; scripts kept outside the repo, in the session scratchpad)

1. **Functions whose `this` (ECX) is an object of a class, from the code:** (a) every slot of the class's vtable,
   tagged with the lowest common class of all vtables that hold that function; (b) data/this_class.json entries whose
   vtable set maps (through the descriptors) to one common class; (c) constructors: functions that store the class's
   vtable at this+0; (d) functions every direct call of which passes ECX = the object (`mov ecx, [0x10456D6C]` etc.),
   and whose address appears nowhere else in .text/.rdata/.data; (e) the same through calls: a callee is tagged when
   every direct call to it passes a tagged ECX. data/class_flow.json's "callers" entries were not used as given: one
   of them (0x10188FA0, "XiZone") is also called with ECX = [ebp+0x39138] (0x1017D4E9), so (d) checks every call site.
2. **Which instructions read or write the object:** ECX in the entry block; a register copied from ECX in the entry
   block and otherwise only popped in the epilogue (so it holds `this` everywhere); within a basic block, a register
   loaded from a global that holds only that class (g_pXiZone, g_pZoneMap, 0x104E1B90, 0x10456D6C) or set to a static
   object's address. Each `[reg+off]` operand is one access: offset, operand width, instruction.
3. **One level into callees:** where `lea reg, [this+off]` is passed (pushed, or as ECX) to a direct call, the callee's
   accesses through that argument are added at off+k, marked "passes this+0x.. as argN of ..; there ...". These rows
   are at most **medium**.
4. **Which class owns an offset:** the class in the chain whose range [parent size, size) holds it.
5. **Type, from the instructions only:** x87 `fld/fst/fstp/fadd/...` with a 4-byte operand -> float (8 -> double);
   `fild/fist` -> signed integer; `movsx` or a following `jl/jg/jle/jge` -> signed; `movzx` or `jb/ja/jbe/jae` ->
   unsigned; the loaded value used as the base of another memory access -> pointer (pointee not known unless stated).
   Plain `mov`/`cmp ... ; je`/`test`/`inc`/`and` give the width only: such fields are **unknown**, with the width.
   Several widths at one offset -> unknown. A constant stored (e.g. `mov [esi+0xb4], 0x40400000`) is not taken as a
   float: the bits fit a float, but a store of a constant does not show the type.
6. **Confidence:** high = an instruction in a function whose `this` comes from a vtable slot, a constructor or a
   global/static object; medium = the `this` comes from data/this_class.json or call propagation, or the instruction is
   in a callee (step 3); low = unknown-type rows (width only).
7. **Names** only where a source names the field or its accessor: data/field_names.json (kept with its confidence and
   evidence) and accessors that data/dm_names.json matches to PS2 symbols/slots (medium); the PC-only accessors it
   names without a PS2 match give low names. Nothing else is named. No area words (P15).

Fields no instruction reaches are not listed; the gap between listed offsets is "not known".

## Schema

No `data/names/r*_*.json` file holds fields (they are `{function: {name, confidence, evidence, summary}}`), so the
JSON uses the shape #122 used (batches/r6_122_render_objects.json, which follows data/field_names.json): `{class:
{"0xOFF": {name, confidence, summary, evidence, width, type, status, name_confidence, name_evidence, functions}}}`
plus `_doc` and `_classes` (size, size_source, vtable, parents, identified_by, notes). Keys starting with `_` are not
classes and must be skipped if merged into data/field_names.json.

## Fields

Columns: offset, width (bytes; "1/4" = used with both widths), type ("unknown" = not known from evidence), name (only
with a source), confidence (type; "low" for unknown rows), evidence (address + instruction; for names, the name source).

## Actor objects

### CXiActor

- Size: 0xC4. Source: runtime-class descriptor 0x1032cf7c (returned by vtable slot 0, 0x10081720 `mov eax, 0x1032cf7c; ret`): {char *name = "CXiActor"; size 0xc4; parent descriptor}
- Chain: CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032cfe0
- Fields found: 37 (8 typed, 29 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x34 | 4 | unknown | focal (medium) | low | moved/compared/tested only (100820E0 `lea eax, [ecx + 0x34]`): width known, type not; its address is also taken (100820E0 `lea eax, [ecx + 0x34]`). Name: accessor 0x10082450 `lea eax,[ecx+0x34]; ret` is matched to PS2 XiActor::GetFocal() (slot 75 -> PC 153; data/dm_names.json) |
| 0x38 | 4 | float |  | high | x87 float operand: 100822D0 `fld dword ptr [ecx + 0x38]` |
| 0x3C | 4 | unknown |  | low | moved/compared/tested only (1008210F `mov edx, dword ptr [ecx + 0x3c]`): width known, type not |
| 0x40 | 4 | unknown |  | low | moved/compared/tested only (10082115 `mov ecx, dword ptr [ecx + 0x40]`): width known, type not |
| 0x44 | 4 | unknown |  | low | moved/compared/tested only (100820F0 `lea eax, [ecx + 0x44]`): width known, type not; its address is also taken (100820F0 `lea eax, [ecx + 0x44]`) |
| 0x48 | 4 | float |  | high | x87 float operand: 100C6ED6 `fld dword ptr [esi + 0x48]` |
| 0x4C | 4 | float |  | high | x87 float operand: 100C6F10 `fld dword ptr [esi + 0x4c]` |
| 0x50 | 4 | unknown |  | low | moved/compared/tested only (10086EA9 passes this+0x44 as arg0 of 10026E90; there 10026EAB `mov dword ptr [ecx + 0xc], eax`): width known, type not |
| 0x54 | 4 | unknown |  | low | moved/compared/tested only (10081AFD `mov dword ptr [esi + 0x54], eax`): width known, type not |
| 0x58 | 4 | unknown |  | low | moved/compared/tested only (10081B00 `mov dword ptr [esi + 0x58], 0`): width known, type not |
| 0x5C | 4 | unknown |  | low | moved/compared/tested only (100A9F98 `lea ecx, [edi + 0x5c]`): width known, type not; its address is also taken (100A9F98 `lea ecx, [edi + 0x5c]`) |
| 0x60 | 4 | unknown |  | low | moved/compared/tested only (10081CFB passes this+0x5C as ecx of 100814A0; there 100814B5 `mov dword ptr [eax + 4], 0`): width known, type not |
| 0x64 | 4 | unknown |  | low | moved/compared/tested only (10081CFB passes this+0x5C as ecx of 100814A0; there 100814A6 `mov ecx, dword ptr [eax + 8]`): width known, type not |
| 0x67 | 1 | unknown |  | low | moved/compared/tested only (10081CFB passes this+0x5C as ecx of 100814A0; there 100814A2 `mov byte ptr [eax + 0xb], 4`): width known, type not |
| 0x68 | 4 | unknown |  | low | moved/compared/tested only (10085575 `mov esi, dword ptr [ebx + 0x68]`): width known, type not |
| 0x6C | 4 | unknown |  | low | moved/compared/tested only (100855C2 `mov esi, dword ptr [ebx + 0x6c]`): width known, type not |
| 0x70 | 4 | pointer | pModelInfo (medium) | high | its value is dereferenced: 100C7F63 `load` then 100C7F66 mov edx, dword ptr [ecx + 0x12c]. Name: data/field_names.json (CXiSkeletonActor +0x70, medium): read by CXiActor_GetModelField170/GetModelByteEE |
| 0x74 | 4 | unknown |  | low | moved/compared/tested only (10081B12 `mov dword ptr [esi + 0x74], eax`): width known, type not |
| 0x78 | 4 | unknown |  | low | moved/compared/tested only (10081C34 `mov dword ptr [esi + 0x78], eax`): width known, type not |
| 0x7C | 4 | unknown |  | low | moved/compared/tested only (100D066F `mov ecx, dword ptr [esi + 0x7c]`): width known, type not |
| 0x80 | 4 | unknown | refCount (medium) | low | moved/compared/tested only (100832D0 `inc dword ptr [ecx + 0x80]`): width known, type not. Name: data/field_names.json (high there); accessors 0x100832D0 `inc dword [ecx+0x80]` / 0x100832E0 are matched to PS2 AddRef/SubRef slots 38/39 by elimination (data/dm_names.json), so the name rests on that match: medium |
| 0x84 | 4 | unknown | schRefCount (medium) | low | moved/compared/tested only (10083390 `inc dword ptr [ecx + 0x84]`): width known, type not. Name: accessors 0x10083390 `inc dword [ecx+0x84]` / 0x100833A0 matched to PS2 AddSchRef/SubSchRef (slots 42/43 -> PC 90/91; data/dm_names.json) |
| 0x88 | 1/4 | unknown |  | low | accessed with widths 1/4: not one type |
| 0x8C | 4 | unknown |  | low | moved/compared/tested only (10085004 `mov dword ptr [ecx + 0x8c], eax`): width known, type not |
| 0x94 | 4 | float | moveSpeedBase (medium) | high | x87 float operand: 10082090 `fld dword ptr [ecx + 0x94]`. Name: accessors 0x10082090 `fld dword [ecx+0x94]` / 0x100820A0 matched to PS2 GetMoveSpeedBase/SetMoveSpeedBase (slots 53/54; data/dm_names.json) |
| 0x98 | 4 | float | moveSpeed (medium) | high | x87 float operand: 10082070 `fld dword ptr [ecx + 0x98]`. Name: accessors 0x10082070 `fld dword [ecx+0x98]` / 0x10082080 matched to PS2 GetMoveSpeed/SetMoveSpeed (slots 51/52; data/dm_names.json) |
| 0x9C | 4 | unknown | walkSpeed (low) | low | moved/compared/tested only (100820C4 `mov dword ptr [ecx + 0x9c], eax`): width known, type not. Name: only writer is 0x100820C0, matched to PS2 SetWalkSpeed (slot 56); GetWalkSpeed (0x100820B0) does not read it (it scales +0x98), so the name rests on the setter alone |
| 0xA0 | 4 | unknown |  | low | moved/compared/tested only (10081C4D `mov dword ptr [esi + 0xa0], ebx`): width known, type not |
| 0xA4 | 4 | int32_t | subActorStatus (medium) | high | compared with a signed jump: 100827A0 `load` then 100827A6 cmp eax, 1 ; jl. Name: PS2 XiActor::IsChocobo()/IsFishingRod() test SUBACTOR_STATUS (PS2 +0x80); the PC bodies 0x100827A0/0x10082820 test +0xA4 the same way (data/dm_names.json) |
| 0xA8 | 4 | unknown |  | low | moved/compared/tested only (10082374 `mov dword ptr [ecx + 0xa8], eax`): width known, type not |
| 0xB0 | 1 | unknown |  | low | moved/compared/tested only (100C7912 `mov byte ptr [esi + 0xb0], 1`): width known, type not |
| 0xB2 | 1/2 | unknown |  | low | accessed with widths 1/2: not one type |
| 0xB3 | 1 | unknown |  | low | moved/compared/tested only (10085AD3 `test byte ptr [esi + 0xb3], 1` ; je): width known, type not |
| 0xB4 | 4 | unknown | model (medium) | low | moved/compared/tested only (10082204 `mov ecx, dword ptr [ecx + 0xb4]`): width known, type not. Name: accessor 0x10082200 `mov eax,[ecx+0xb4]` matched to PS2 XiActor GetModel (slot 65 -> PC 119; data/dm_names.json) |
| 0xB8 | 4 | unknown |  | low | only its address is taken (10082F58 `lea eax, [esi + 0xb8]`): an embedded member whose type the code here does not show |
| 0xBC | 4 | float | distanceToCamera (medium) | high | x87 float operand: 100A5081 `fld dword ptr [esi + 0xbc]`. Name: data/field_names.json (CXiSkeletonActor +0xBC, medium) |
| 0xC0 | 1 | unknown |  | low | moved/compared/tested only (10081C62 `mov byte ptr [esi + 0xc0], bl`): width known, type not |

### CXiAtelActor

- Size: 0xD4. Source: runtime-class descriptor 0x1032d44c (returned by vtable slot 0, 0x10086e80 `mov eax, 0x1032d44c; ret`): {char *name = "CXiAtelActor"; size 0xd4; parent descriptor}
- Chain: CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032e3c8
- Fields found: 4 (2 typed, 2 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0xC4 | 4 | float |  | medium | x87 float operand: 100AA27A `fld dword ptr [esi + 0xc4]` |
| 0xC8 | 4 | float |  | medium | x87 float operand: 100AA280 `fld dword ptr [esi + 0xc8]` |
| 0xCC | 4 | unknown |  | low | moved/compared/tested only (100AA233 `mov edx, dword ptr [esi + 0xcc]`): width known, type not |
| 0xD0 | 4 | unknown |  | low | moved/compared/tested only (100AA23C `mov eax, dword ptr [esi + 0xd0]`): width known, type not |

### CXiControlActor

- Size: 0x5C4. Source: runtime-class descriptor 0x1032df68 (returned by vtable slot 0, 0x100a4c00 `mov eax, 0x1032df68; ret`): {char *name = "CXiControlActor"; size 0x5c4; parent descriptor}
- Chain: CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032dfc8
- Fields found: 67 (18 typed, 49 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0xD4 | 4 | float |  | medium | x87 float operand: 100AC635 passes this+0xD4 as arg1 of 10028230; there 10028246 `fld dword ptr [eax]` |
| 0xD8 | 4 | float |  | medium | x87 float operand: 100AC635 passes this+0xD4 as arg1 of 10028230; there 10028241 `fmul dword ptr [eax + 4]` |
| 0xDC | 4 | float |  | medium | x87 float operand: 100AC635 passes this+0xD4 as arg1 of 10028230; there 1002823B `fmul dword ptr [eax + 8]` |
| 0xE0 | 4 | unknown |  | low | moved/compared/tested only (100A8A10 `mov dword ptr [esi + 0xe0], eax`): width known, type not |
| 0xE4 | 4 | unknown |  | low | moved/compared/tested only (100C6833 `lea eax, [esi + 0xe4]`): width known, type not; its address is also taken (100C6833 `lea eax, [esi + 0xe4]`) |
| 0xE8 | 4 | float |  | medium | x87 float operand: 100AA1D4 `fstp dword ptr [esi + 0xe8]` |
| 0xEC | 4 | unknown |  | low | moved/compared/tested only (100AC532 `mov ecx, dword ptr [esi + 0xec]`): width known, type not |
| 0xF0 | 4 | unknown |  | low | moved/compared/tested only (100C4AA7 `mov eax, dword ptr [esi + 0xf0]`): width known, type not |
| 0xF4 | 4 | float |  | high | x87 float operand: 100A5222 `fadd dword ptr [esi + 0xf4]` |
| 0xF8 | 1 | unknown |  | low | moved/compared/tested only (100A8A16 `mov byte ptr [esi + 0xf8], bl`): width known, type not |
| 0xF9 | 1 | unknown | isFreeRun (medium) | low | moved/compared/tested only (100A4670 `mov al, byte ptr [ecx + 0xf9]`): width known, type not. Name: accessors 0x100A4670/0x100A4660 (byte) matched to PS2 XiControlActor::IsFreeRun() / IsFreeRun(char) (PS2 byte +0xD5; data/dm_names.json) |
| 0xFA | 1 | unknown | isWalkLock (medium) | low | moved/compared/tested only (100A4690 `mov al, byte ptr [ecx + 0xfa]`): width known, type not. Name: accessors 0x100A4690/0x100A4680 matched to PS2 IsWalkLock() / IsWalkLock(char) (PS2 byte +0xD6) |
| 0xFB | 1 | unknown | isParallelMove (medium) | low | moved/compared/tested only (100A46B0 `mov al, byte ptr [ecx + 0xfb]`): width known, type not. Name: accessors 0x100A46B0/0x100A46A0 matched to PS2 IsParallelMove() / IsParallelMove(char) (PS2 byte +0xD7) |
| 0xFC | 4 | unknown | controlLock (medium) | low | moved/compared/tested only (100A46C0 `mov eax, dword ptr [ecx + 0xfc]`): width known, type not. Name: accessor 0x100A46C0 matched to PS2 IsControlLock(), which returns this counter |
| 0x100 | 1 | unknown |  | low | moved/compared/tested only (100A8A41 `mov byte ptr [esi + 0x100], bl`): width known, type not |
| 0x101 | 1 | unknown |  | low | moved/compared/tested only (100C6528 `mov al, byte ptr [esi + 0x101]`): width known, type not |
| 0x102 | 1 | unknown |  | low | moved/compared/tested only (100C651A `mov al, byte ptr [esi + 0x102]`): width known, type not |
| 0x103 | 1 | unknown |  | low | moved/compared/tested only (100C64EC `mov al, byte ptr [esi + 0x103]`): width known, type not |
| 0x104 | 1 | unknown |  | low | moved/compared/tested only (100C64FA `mov al, byte ptr [esi + 0x104]`): width known, type not |
| 0x108 | 4 | float |  | medium | x87 float operand: 100A9FD0 `fld dword ptr [esi + 0x108]` |
| 0x10C | 4 | float |  | medium | x87 float operand: 100AA153 `fmul dword ptr [esi + 0x10c]` |
| 0x110 | 4 | unknown |  | low | moved/compared/tested only (100A9FEF `mov eax, dword ptr [esi + 0x110]`): width known, type not |
| 0x114 | 4 | unknown |  | low | moved/compared/tested only (100CA904 `mov eax, dword ptr [esi + 0x114]`): width known, type not |
| 0x118 | 4 | float |  | medium | x87 float operand: 100AADFA `fld dword ptr [edi + 0x118]` |
| 0x11C | 4 | unknown |  | low | moved/compared/tested only (100A8B78 `mov dword ptr [esi + 0x11c], ebx`): width known, type not |
| 0x120 | 4 | unknown |  | low | moved/compared/tested only (100A8B83 `mov dword ptr [esi + 0x120], eax`): width known, type not |
| 0x124 | 4 | unknown |  | low | moved/compared/tested only (100A8B8F `mov dword ptr [esi + 0x124], ecx`): width known, type not |
| 0x128 | 4 | unknown |  | low | moved/compared/tested only (100A8B9B `mov dword ptr [esi + 0x128], edx`): width known, type not |
| 0x12C | 4 | unknown |  | low | moved/compared/tested only (100A8BAC `mov dword ptr [esi + 0x12c], eax`): width known, type not |
| 0x130 | 4 | float |  | medium | x87 float operand: 100AAD04 passes this+0x130 as arg0 of 10027530; there 10027546 `fld dword ptr [eax]` |
| 0x134 | 4 | float |  | medium | x87 float operand: 100AABD6 `fadd dword ptr [esi + 0x134]` |
| 0x138 | 4 | float |  | medium | x87 float operand: 100AABE3 `fadd dword ptr [esi + 0x138]` |
| 0x13C | 4 | unknown |  | low | moved/compared/tested only (100A8B1B `mov dword ptr [esi + 0x13c], ecx`): width known, type not |
| 0x140 | 4 | unknown |  | low | moved/compared/tested only (100AACBA `mov edx, dword ptr [esi + 0x140]`): width known, type not |
| 0x144 | 4 | float |  | medium | x87 float operand: 100AAB7D `fld dword ptr [esi + 0x144]` |
| 0x148 | 4 | float |  | medium | x87 float operand: 100CA8EF `fld dword ptr [esi + 0x148]` |
| 0x14C | 4 | float |  | medium | x87 float operand: 100AABED `fld dword ptr [esi + 0x14c]` |
| 0x150 | 4 | unknown |  | low | moved/compared/tested only (100AAC00 `lea ecx, [esi + 0x150]`): width known, type not; its address is also taken (100AAC00 `lea ecx, [esi + 0x150]`) |
| 0x154 | 4 | unknown |  | low | moved/compared/tested only (100A8B48 `mov dword ptr [esi + 0x154], eax`): width known, type not |
| 0x158 | 4 | unknown |  | low | moved/compared/tested only (100A8B54 `mov dword ptr [esi + 0x158], ecx`): width known, type not |
| 0x15C | 4 | unknown |  | low | moved/compared/tested only (100A8B60 `mov dword ptr [esi + 0x15c], edx`): width known, type not |
| 0x160 | 4 | float | groundNormal (medium) | medium | x87 float operand: 100A4C6F passes this+0x160 as arg0 of 10026E30; there 10026E4A `fstp dword ptr [eax]`. Name: accessor 0x100A4590 `lea eax,[ecx+0x160]` (PC slot 132) matched to the PS2 body `addiu v0,a0,0x140` returning ground_normal (data/dm_names.json) |
| 0x164 | 4 | unknown |  | low | moved/compared/tested only (100A8A6B `mov dword ptr [esi + 0x164], 0xbf800000`): width known, type not |
| 0x168 | 4 | unknown |  | low | moved/compared/tested only (100A8A75 `mov dword ptr [esi + 0x168], ebx`): width known, type not |
| 0x16C | 4 | unknown |  | low | moved/compared/tested only (100A8A7B `mov dword ptr [esi + 0x16c], eax`): width known, type not |
| 0x170 | 4 | float | groundHeight (low) | high | x87 float operand: 100A45B0 `fld dword ptr [ecx + 0x170]`. Name: getter 0x100A45B0 / setter 0x100A45A0 (PC slots 129/130); data/dm_names.json calls it ground_height but does not state a PS2 match |
| 0x174 | 4 | unknown | groundMaterial (low) | low | moved/compared/tested only (100A45C4 `mov dword ptr [ecx + 0x174], eax`): width known, type not. Name: getter 0x100A45D0 / setter 0x100A45C0 (PC slots 134/133); no PS2 match stated in data/dm_names.json |
| 0x178 | 4 | float | waterHeight (low) | high | x87 float operand: 100A45F0 `fld dword ptr [ecx + 0x178]`. Name: getter 0x100A45F0 / setter 0x100A45E0 (PC slots 136/135); data/dm_names.json calls it water_height but does not state a PS2 match |
| 0x17C | 1 | unknown |  | low | moved/compared/tested only (100CA23B `mov cl, byte ptr [ebp + 0x17c]`): width known, type not |
| 0x180 | 4 | unknown |  | low | indexed access: 100A460A `lea eax, [ecx + eax*8 + 0x180]` |
| 0x388 | 4 | unknown |  | low | only its address is taken (100AAF54 `lea edx, [ecx + 0x388]`): an embedded member whose type the code here does not show |
| 0x3F0 | 4 | unknown |  | low | only its address is taken (100AAF7A `lea esi, [ebx + 0x3f0]`): an embedded member whose type the code here does not show |
| 0x590 | 4 | unknown |  | low | moved/compared/tested only (100CA22F `mov eax, dword ptr [ebp + 0x590]`): width known, type not |
| 0x594 | 4 | unknown |  | low | moved/compared/tested only (100A8A47 `mov dword ptr [esi + 0x594], ebx`): width known, type not |
| 0x598 | 4 | unknown |  | low | moved/compared/tested only (100A8A4D `mov dword ptr [esi + 0x598], ebx`): width known, type not |
| 0x59C | 4 | float |  | high | x87 float operand: 100C76F0 `fstp dword ptr [esi + 0x59c]` |
| 0x5A0 | 4 | unknown |  | low | moved/compared/tested only (100A8CA3 `lea ecx, [esi + 0x5a0]`): width known, type not; its address is also taken (100A8CA3 `lea ecx, [esi + 0x5a0]`) |
| 0x5A4 | 4 | unknown |  | low | moved/compared/tested only (100A89F8 passes this+0x5A0 as ecx of 100814A0; there 100814B5 `mov dword ptr [eax + 4], 0`): width known, type not |
| 0x5A8 | 4 | unknown |  | low | moved/compared/tested only (100A89F8 passes this+0x5A0 as ecx of 100814A0; there 100814A6 `mov ecx, dword ptr [eax + 8]`): width known, type not |
| 0x5AB | 1 | unknown |  | low | moved/compared/tested only (100A89F8 passes this+0x5A0 as ecx of 100814A0; there 100814A2 `mov byte ptr [eax + 0xb], 4`): width known, type not |
| 0x5AC | 2 | unknown |  | low | moved/compared/tested only (100A8AC6 `mov word ptr [esi + 0x5ac], bx`): width known, type not |
| 0x5AE | 2 | unknown |  | low | moved/compared/tested only (100A8ACD `mov word ptr [esi + 0x5ae], bx`): width known, type not |
| 0x5B0 | 1 | unknown |  | low | moved/compared/tested only (100A8AD4 `mov byte ptr [esi + 0x5b0], bl`): width known, type not |
| 0x5B1 | 1 | unknown |  | low | moved/compared/tested only (100A8BA6 `mov byte ptr [esi + 0x5b1], bl`): width known, type not |
| 0x5B4 | 4 | unknown |  | low | moved/compared/tested only (100C6AC7 `lea ecx, [esi + 0x5b4]`): width known, type not; its address is also taken (100C6AC7 `lea ecx, [esi + 0x5b4]`) |
| 0x5B8 | 4 | unknown |  | low | moved/compared/tested only (100C6AD3 passes this+0x5B4 as arg1 of 10026EB0; there 10026EBC `mov edx, dword ptr [eax + 4]`): width known, type not |
| 0x5BC | 4 | unknown |  | low | moved/compared/tested only (100C6AD3 passes this+0x5B4 as arg1 of 10026EB0; there 10026EC2 `mov eax, dword ptr [eax + 8]`): width known, type not |

### CXiCollisionActor

- Size: 0x5F8. Source: runtime-class descriptor 0x1032db30 (returned by vtable slot 0, 0x100a4c10 `mov eax, 0x1032db30; ret`): {char *name = "CXiCollisionActor"; size 0x5f8; parent descriptor}
- Chain: CXiCollisionActor (0x5F8) : CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032db40
- Fields found: 11 (0 typed, 11 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x5C4 | 4 | unknown | pos (medium) | low | moved/compared/tested only (100C65A6 `lea ecx, [esi + 0x5c4]`): width known, type not; its address is also taken (100C65A6 `lea ecx, [esi + 0x5c4]`). Name: accessor 0x100A4CD0 `lea eax,[ecx+0x5c4]` matched to PS2 slot 56 GetPos (-> PC 111; data/dm_names.json) |
| 0x5C8 | 4 | unknown |  | low | moved/compared/tested only (100A4C74 `mov eax, dword ptr [esi + 0x5c8]`): width known, type not |
| 0x5CC | 4 | unknown |  | low | moved/compared/tested only (100C65B2 passes this+0x5C4 as arg1 of 10026EB0; there 10026EC2 `mov eax, dword ptr [eax + 8]`): width known, type not |
| 0x5E0 | 1 | unknown | isTouch (medium) | low | moved/compared/tested only (100A46E4 `mov byte ptr [ecx + 0x5e0], al`): width known, type not. Name: accessor 0x100A46D0 matched to PS2 IsTouchDown (slot 47 -> PC 102), which reads IsTouch at PS2 +0x1C0 (data/dm_names.json) |
| 0x5E1 | 1 | unknown |  | low | moved/compared/tested only (100A5077 `mov al, byte ptr [esi + 0x5e1]`): width known, type not |
| 0x5E2 | 1 | unknown |  | low | moved/compared/tested only (100A46F0 `mov al, byte ptr [ecx + 0x5e2]`): width known, type not |
| 0x5E4 | 4 | unknown | currentAreaId (medium) | low | moved/compared/tested only (100A4704 `mov dword ptr [ecx + 0x5e4], eax`): width known, type not. Name: accessors 0x100A4700/0x100A4710 (PC-only slots) set/get it; data/dm_names.json maps it to PS2 DWARF XiCollisionActor::current_area_id (+0x1C4) by the class-level shift |
| 0x5E8 | 4 | unknown |  | low | moved/compared/tested only (100A4C62 `mov dword ptr [esi + 0x5e8], edx`): width known, type not |
| 0x5EC | 1 | unknown |  | low | moved/compared/tested only (100A4730 `mov al, byte ptr [ecx + 0x5ec]`): width known, type not |
| 0x5F0 | 4 | unknown |  | low | moved/compared/tested only (100A53FD `mov ecx, dword ptr [esi + 0x5f0]`): width known, type not |
| 0x5F4 | 1 | unknown |  | low | moved/compared/tested only (100A5247 `mov al, byte ptr [esi + 0x5f4]`): width known, type not |

### CXiDollActor

- Size: 0x440. Source: runtime-class descriptor 0x1032f910 (returned by vtable slot 0, 0x100abca0 `mov eax, 0x1032f910; ret`): {char *name = "CXiDollActor"; size 0x440; parent descriptor}
- Chain: CXiDollActor (0x440) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032f938
- Fields found: 21 (3 typed, 18 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0xC4 | 4 | unknown |  | low | moved/compared/tested only (100AC118 `lea edi, [esi + 0xc4]`): width known, type not; its address is also taken (100AC118 `lea edi, [esi + 0xc4]`) |
| 0xD0 | 4 | unknown |  | low | moved/compared/tested only (100ABD0F passes this+0xC4 as ecx of 1003B7B0; there 1003B7F8 `mov dword ptr [esi + 0xc], edi`): width known, type not |
| 0xD8 | 4 | unknown |  | low | only its address is taken (100ABD0F passes this+0xC4 as ecx of 1003B7B0; there 1003B7EA `lea ebx, [esi + 0x14]`): an embedded member whose type the code here does not show |
| 0xE0 | 4 | unknown |  | low | moved/compared/tested only (100AC120 passes this+0xC4 as ecx of 1003B6D0; there 1003B6D0 `mov eax, dword ptr [ecx + 0x1c]`): width known, type not |
| 0xE4 | 4 | unknown |  | low | only its address is taken (100ABEB3 passes this+0xC4 as ecx of 1003B540; there 1003B54B `lea ecx, [esi + 0x20]`): an embedded member whose type the code here does not show |
| 0x104 | 4 | unknown |  | low | only its address is taken (100ABF57 `lea ebp, [edi + 0x104]`): an embedded member whose type the code here does not show |
| 0x108 | 4 | unknown |  | low | only its address is taken (100AC1FF `lea eax, [esi + 0x108]`): an embedded member whose type the code here does not show |
| 0x404 | 4 | unknown |  | low | moved/compared/tested only (100AC156 `mov dword ptr [esi + 0x404], eax`): width known, type not; its address is also taken (100ABD90 `lea eax, [ecx + 0x404]`) |
| 0x408 | 4 | unknown |  | low | moved/compared/tested only (100AC1A7 `mov dword ptr [esi + 0x408], eax`): width known, type not |
| 0x40C | 4 | unknown |  | low | moved/compared/tested only (100AC1F8 `mov dword ptr [esi + 0x40c], eax`): width known, type not |
| 0x410 | 4 | unknown |  | low | moved/compared/tested only (100ABEFB passes this+0x404 as arg0 of 10026E90; there 10026EAB `mov dword ptr [ecx + 0xc], eax`): width known, type not |
| 0x414 | 4 | unknown |  | low | moved/compared/tested only (100ABDA0 `lea eax, [ecx + 0x414]`): width known, type not; its address is also taken (100ABDA0 `lea eax, [ecx + 0x414]`) |
| 0x418 | 4 | unknown |  | low | moved/compared/tested only (100ABD04 passes this+0x414 as arg0 of 10026E90; there 10026E9F `mov dword ptr [ecx + 4], edx`): width known, type not |
| 0x41C | 4 | unknown |  | low | moved/compared/tested only (100ABD04 passes this+0x414 as arg0 of 10026E90; there 10026EA5 `mov dword ptr [ecx + 8], edx`): width known, type not |
| 0x420 | 4 | unknown |  | low | moved/compared/tested only (100ABD04 passes this+0x414 as arg0 of 10026E90; there 10026EAB `mov dword ptr [ecx + 0xc], eax`): width known, type not |
| 0x424 | 1 | unknown |  | low | moved/compared/tested only (100AC0F6 `mov al, byte ptr [esi + 0x424]`): width known, type not |
| 0x425 | 1 | unknown |  | low | moved/compared/tested only (100AC0FC `mov cl, byte ptr [esi + 0x425]`): width known, type not |
| 0x426 | 1 | unknown |  | low | moved/compared/tested only (100AC102 `mov dl, byte ptr [esi + 0x426]`): width known, type not |
| 0x434 | 4 | float |  | high | x87 float operand: 100ABE30 `fld dword ptr [ecx + 0x434]` |
| 0x438 | 4 | float |  | high | x87 float operand: 100ABE40 `fld dword ptr [ecx + 0x438]` |
| 0x43C | 4 | float |  | high | x87 float operand: 100ABE50 `fld dword ptr [ecx + 0x43c]` |

### XiLiftActor

- Size: 0x600. Source: runtime-class descriptor 0x10330278 (returned by vtable slot 0, 0x100c3890 `mov eax, 0x10330278; ret`): {char *name = "XiLiftActor"; size 0x600; parent descriptor}
- Chain: XiLiftActor (0x600) : CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x10330288
- Note: the descriptor's parent pointer names CXiAtelActor (0xD4), but the constructor 0x100C3990 first calls 0x100A89E0, which stores 0x1032E3C8 and then 0x1032DFC8 (the CXiControlActor vtable) at this+0; XiLiftActor's vtable also shares CXiControlActor's slots (0x100A8C10, 0x100A4CC0). Its layout below 0x5C4 is CXiControlActor's; own fields start at 0x5C4.
- Fields found: 5 (0 typed, 5 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x5C4 | 4 | unknown |  | low | moved/compared/tested only (100C39B2 `mov dword ptr [esi + 0x5c4], eax`): width known, type not |
| 0x5C8 | 4 | unknown |  | low | moved/compared/tested only (100C3A03 `mov eax, dword ptr [esi + 0x5c8]`): width known, type not |
| 0x5CC | 1 | unknown |  | low | moved/compared/tested only (100C39B8 `mov byte ptr [esi + 0x5cc], al`): width known, type not |
| 0x5F0 | 4 | unknown |  | low | moved/compared/tested only (100C39C4 `mov dword ptr [esi + 0x5f0], eax`): width known, type not |
| 0x5FC | 4 | unknown |  | low | moved/compared/tested only (100C3998 `lea ecx, [esi + 0x5fc]`): width known, type not; its address is also taken (100C3998 `lea ecx, [esi + 0x5fc]`) |

### XiDoorActor

- Size: 0x894. Source: runtime-class descriptor 0x1032fd4c (returned by vtable slot 0, 0x100ac340 `mov eax, 0x1032fd4c; ret`): {char *name = "XiDoorActor"; size 0x894; parent descriptor}
- Chain: XiDoorActor (0x894) : CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032fd58
- Fields found: 21 (0 typed, 21 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x5C4 | 4 | unknown |  | low | moved/compared/tested only (100AC4F1 `mov dword ptr [esi + 0x5c4], eax`): width known, type not; its address is also taken (100AC9C0 `lea eax, [ecx + 0x5c4]`) |
| 0x5C8 | 4 | unknown |  | low | moved/compared/tested only (100AC4FD `mov dword ptr [esi + 0x5c8], ecx`): width known, type not |
| 0x5CC | 4 | unknown |  | low | moved/compared/tested only (100AC50E `mov dword ptr [esi + 0x5cc], edx`): width known, type not |
| 0x5D0 | 4 | unknown |  | low | moved/compared/tested only (100AC514 `mov dword ptr [esi + 0x5d0], eax`): width known, type not |
| 0x5D4 | 4 | unknown |  | low | moved/compared/tested only (100AC520 `mov dword ptr [esi + 0x5d4], ecx`): width known, type not; its address is also taken (100AC9D0 `lea eax, [ecx + 0x5d4]`) |
| 0x5D8 | 4 | unknown |  | low | moved/compared/tested only (100AC52C `mov dword ptr [esi + 0x5d8], edx`): width known, type not |
| 0x5DC | 4 | unknown |  | low | moved/compared/tested only (100AC538 `mov dword ptr [esi + 0x5dc], ecx`): width known, type not |
| 0x5E0 | 4 | unknown |  | low | moved/compared/tested only (100AC544 `mov dword ptr [esi + 0x5e0], eax`): width known, type not |
| 0x628 | 4 | unknown |  | low | only its address is taken (100AC91B `lea eax, [esi + 0x628]`): an embedded member whose type the code here does not show |
| 0x664 | 4 | unknown |  | low | moved/compared/tested only (100AC8FD `mov dword ptr [esi + 0x664], ebx`): width known, type not |
| 0x668 | 4 | unknown |  | low | moved/compared/tested only (100ACA3E `mov eax, dword ptr [esi + 0x668]`): width known, type not |
| 0x870 | 4 | unknown |  | low | moved/compared/tested only (100AD329 `mov eax, dword ptr [ebp + 0x870]`): width known, type not |
| 0x874 | 4 | unknown |  | low | moved/compared/tested only (100AC966 `mov dword ptr [esi + 0x874], ebx`): width known, type not |
| 0x878 | 1 | unknown |  | low | moved/compared/tested only (100AC903 `mov byte ptr [esi + 0x878], bl`): width known, type not |
| 0x87C | 4 | unknown |  | low | moved/compared/tested only (100AC54A `mov eax, dword ptr [esi + 0x87c]`): width known, type not |
| 0x880 | 4 | unknown |  | low | moved/compared/tested only (100AC57B `mov eax, dword ptr [esi + 0x880]`): width known, type not |
| 0x884 | 4 | unknown |  | low | moved/compared/tested only (100AC90F `mov dword ptr [esi + 0x884], ebx`): width known, type not |
| 0x888 | 4 | unknown |  | low | moved/compared/tested only (100AC915 `mov dword ptr [esi + 0x888], ebx`): width known, type not |
| 0x88C | 4 | unknown |  | low | only its address is taken (100AC88E `lea ecx, [esi + 0x88c]`): an embedded member whose type the code here does not show |
| 0x88E | 2 | unknown |  | low | moved/compared/tested only (100AC8F0 passes this+0x88C as ecx of 1006C260; there 1006C267 `mov word ptr [eax + 2], cx`): width known, type not |
| 0x890 | 4 | unknown |  | low | moved/compared/tested only (100AC8F0 passes this+0x88C as ecx of 1006C260; there 1006C264 `mov dword ptr [eax + 4], ecx`): width known, type not |

### XiModelActor

- Size: 0x794. Source: runtime-class descriptor 0x10330684 (returned by vtable slot 0, 0x100c3dd0 `mov eax, 0x10330684; ret`): {char *name = "XiModelActor"; size 0x794; parent descriptor}
- Chain: XiModelActor (0x794) : CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x10330aa0
- Fields found: 39 (4 typed, 35 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x5C4 | 4 | pointer |  | high | its value is dereferenced: 100C4C5E `load` then 100C4C6A mov ecx, dword ptr [eax + 0x30] |
| 0x5C8 | 4 | unknown |  | low | only its address is taken (100C4C4C `lea ebp, [esi + 0x5c8]`): an embedded member whose type the code here does not show |
| 0x5E0 | 4 | unknown |  | low | moved/compared/tested only (100C4C76 `mov dword ptr [esi + 0x5e0], ecx`): width known, type not |
| 0x608 | 4 | unknown |  | low | moved/compared/tested only (100C4C64 `lea edi, [esi + 0x608]`): width known, type not; its address is also taken (100C4C64 `lea edi, [esi + 0x608]`) |
| 0x60C | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279B2 `mov dword ptr [ecx + 4], eax`): width known, type not |
| 0x610 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279B5 `mov dword ptr [ecx + 8], eax`): width known, type not |
| 0x614 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279B8 `mov dword ptr [ecx + 0xc], eax`): width known, type not |
| 0x618 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279BB `mov dword ptr [ecx + 0x10], eax`): width known, type not |
| 0x61C | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279DD `mov dword ptr [ecx + 0x14], eax`): width known, type not |
| 0x620 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279BE `mov dword ptr [ecx + 0x18], eax`): width known, type not |
| 0x624 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279C1 `mov dword ptr [ecx + 0x1c], eax`): width known, type not |
| 0x628 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279C4 `mov dword ptr [ecx + 0x20], eax`): width known, type not |
| 0x62C | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279C7 `mov dword ptr [ecx + 0x24], eax`): width known, type not |
| 0x630 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279E0 `mov dword ptr [ecx + 0x28], eax`): width known, type not |
| 0x634 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279CA `mov dword ptr [ecx + 0x2c], eax`): width known, type not |
| 0x638 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279CD `mov dword ptr [ecx + 0x30], eax`): width known, type not |
| 0x63C | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279D0 `mov dword ptr [ecx + 0x34], eax`): width known, type not |
| 0x640 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279D3 `mov dword ptr [ecx + 0x38], eax`): width known, type not |
| 0x644 | 4 | unknown |  | low | moved/compared/tested only (100C4C7E passes this+0x608 as ecx of 100279B0; there 100279E3 `mov dword ptr [ecx + 0x3c], eax`): width known, type not |
| 0x69C | 2 | unknown |  | low | moved/compared/tested only (100C4C6D `mov word ptr [esi + 0x69c], 0`): width known, type not |
| 0x734 | 4 | unknown |  | low | only its address is taken (100C4805 `lea ebp, [esi + 0x734]`): an embedded member whose type the code here does not show |
| 0x738 | 4 | unknown |  | low | moved/compared/tested only (100C481A `mov dword ptr [esi + 0x738], ecx`): width known, type not |
| 0x73C | 4 | unknown |  | low | moved/compared/tested only (100C4832 `mov dword ptr [esi + 0x73c], edx`): width known, type not |
| 0x740 | 4 | unknown |  | low | moved/compared/tested only (100C4826 `mov dword ptr [esi + 0x740], 0x3f800000`): width known, type not |
| 0x744 | 4 | float |  | high | x87 float operand: 100C4940 `fld dword ptr [esi + 0x744]` |
| 0x748 | 4 | float |  | high | x87 float operand: 100C48A9 `fsub dword ptr [esi + 0x748]` |
| 0x74C | 4 | float |  | high | x87 float operand: 100C48B7 `fsub dword ptr [esi + 0x74c]` |
| 0x750 | 4 | unknown |  | low | moved/compared/tested only (100C4AAD `mov dword ptr [esi + 0x750], eax`): width known, type not |
| 0x770 | 4 | unknown |  | low | moved/compared/tested only (100C4CBA `mov eax, dword ptr [esi + 0x770]`): width known, type not |
| 0x774 | 4 | unknown |  | low | moved/compared/tested only (100C4CAF `mov eax, dword ptr [esi + 0x774]`): width known, type not |
| 0x778 | 1 | unknown |  | low | moved/compared/tested only (100C484C `mov al, byte ptr [esi + 0x778]`): width known, type not |
| 0x779 | 1 | unknown |  | low | moved/compared/tested only (100C483E `mov al, byte ptr [esi + 0x779]`): width known, type not |
| 0x77A | 1 | unknown |  | low | moved/compared/tested only (100C4A2D `mov al, byte ptr [esi + 0x77a]`): width known, type not |
| 0x77C | 4 | unknown |  | low | moved/compared/tested only (100C416E `lea ecx, [esi + 0x77c]`): width known, type not; its address is also taken (100C416E `lea ecx, [esi + 0x77c]`) |
| 0x780 | 1 | unknown |  | low | moved/compared/tested only (100C41FE passes this+0x77C as ecx of 1001D170; there 1001D1B2 `mov byte ptr [ebp + 4], 0`): width known, type not |
| 0x784 | 4 | unknown |  | low | moved/compared/tested only (100C4174 passes this+0x77C as ecx of 1001D1F0; there 1001D1F6 `mov eax, dword ptr [esi + 8]`): width known, type not |
| 0x788 | 4 | unknown |  | low | moved/compared/tested only (100C4174 passes this+0x77C as ecx of 1001D1F0; there 1001D20C `mov eax, dword ptr [esi + 0xc]`): width known, type not |
| 0x78C | 4 | unknown |  | low | moved/compared/tested only (100C41FE passes this+0x77C as ecx of 1001D170; there 1001D17C `lea edi, [ebp + 0x10]`): width known, type not; its address is also taken (100C41FE passes this+0x77C as ecx of 1001D170; there 1001D17C `lea edi, [ebp + 0x10]`) |
| 0x790 | 4 | unknown |  | low | moved/compared/tested only (100C4174 passes this+0x77C as ecx of 1001D1F0; there 1001D22C `mov eax, dword ptr [esi + 0x14]`): width known, type not |

### XiFurnitureActor

- Size: 0x794. Source: runtime-class descriptor 0x10330690 (returned by vtable slot 0, 0x100c3df0 `mov eax, 0x10330690; ret`): {char *name = "XiFurnitureActor"; size 0x794; parent descriptor}
- Chain: XiFurnitureActor (0x794) : XiModelActor (0x794) : CXiControlActor (0x5C4) : CXiAtelActor (0xD4) : CXiActor (0xC4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x103306a0
- Note: same size as its parent XiModelActor (0x794): it adds no data members.
- Fields found: 0 (0 typed, 0 unknown)

## Camera objects

### CMoCamera

- Size: 0xB0. Source: runtime-class descriptor 0x1032ab28 (returned by vtable slot 0, 0x1003c880 `mov eax, 0x1032ab28; ret`): {char *name = "CMoCamera"; size 0xb0; parent descriptor}
- Chain: CMoCamera (0xB0) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032c5d0
- Note: the tree places GetSharedInstance (0x10071980, returns 0x109F81C0) in CMoCamera, but 0x109F81C0 is zero-filled (beyond .data's raw data), no instruction stores a vtable there, its at-exit destructor 0x100719B0 is a bare `ret`, and 0x10071980 has no direct caller: the code does not show that 0x109F81C0 is a CMoCamera, so its absolute accesses are not used here
- Fields found: 11 (0 typed, 11 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x20 | 4 | unknown |  | low | moved/compared/tested only (1003CB3C `lea ecx, [eax + 0x20]`): width known, type not; its address is also taken (1003CB3C `lea ecx, [eax + 0x20]`) |
| 0x30 | 4 | unknown |  | low | only its address is taken (10071C67 `lea ecx, [esi + 0x30]`): an embedded member whose type the code here does not show |
| 0x34 | 4 | unknown |  | low | only its address is taken (1003CB57 `lea esi, [eax + 0x34]`): an embedded member whose type the code here does not show |
| 0x3C | 4 | unknown |  | low | moved/compared/tested only (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B646 `mov dword ptr [esi + 0xc], edi`): width known, type not |
| 0x40 | 4 | unknown |  | low | moved/compared/tested only (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B649 `mov dword ptr [esi + 0x10], edi`): width known, type not |
| 0x44 | 4 | unknown |  | low | only its address is taken (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B65F `lea ecx, [esi + 0x14]`): an embedded member whose type the code here does not show |
| 0x50 | 4 | unknown |  | low | only its address is taken (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B674 `lea ecx, [esi + 0x20]`): an embedded member whose type the code here does not show |
| 0x5C | 4 | unknown |  | low | moved/compared/tested only (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B64C `mov dword ptr [esi + 0x2c], edi`): width known, type not |
| 0x60 | 4 | unknown |  | low | moved/compared/tested only (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B64F `mov dword ptr [esi + 0x30], edi`): width known, type not |
| 0x64 | 4 | unknown |  | low | moved/compared/tested only (1003CB99 passes this+0x30 as ecx of 1003B640; there 1003B67C `mov dword ptr [esi + 0x34], edi`): width known, type not |
| 0x70 | 4 | unknown |  | low | only its address is taken (1003CB6A `lea edi, [eax + 0x70]`): an embedded member whose type the code here does not show |

### CYyCamMng2

- Size: 0x118. Source: runtime-class descriptor 0x1032a36c (returned by vtable slot 0, 0x1001e2a0 `mov eax, 0x1032a36c; ret`): {char *name = "CYyCamMng2"; size 0x118; parent descriptor}; confirmed by 0x1001E470: `push 0x118; call 0x1002C890 (Mem_AllocDefault)`, constructor 0x1001E600, result stored in 0x10456D6C
- Chain: CYyCamMng2 (0x118) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032a37c
- Fields found: 4 (0 typed, 4 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x4 | 4 | unknown |  | low | only its address is taken (1001E604 `lea ecx, [esi + 4]`): an embedded member whose type the code here does not show |
| 0xB4 | 4 | unknown |  | low | moved/compared/tested only (1001E598 `mov dword ptr [esi + 0xb4], 0x40400000`): width known, type not |
| 0xB8 | 4 | unknown |  | low | moved/compared/tested only (1001E5A2 `mov dword ptr [esi + 0xb8], 0x40c00000`): width known, type not |
| 0xF0 | 1 | unknown |  | low | moved/compared/tested only (1001E616 `mov byte ptr [esi + 0xf0], bl`): width known, type not |

### CMoCameraTask

- Size: 0xC8. Source: runtime-class descriptor 0x1032ab34 (returned by vtable slot 0, 0x1003cc70 `mov eax, 0x1032ab34; ret`): {char *name = "CMoCameraTask"; size 0xc8; parent descriptor}
- Chain: CMoCameraTask (0xC8) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x1032ab60
- Fields found: 12 (3 typed, 9 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x34 | 4 | unknown |  | low | moved/compared/tested only (1003CC8E `lea esi, [ebp + 0x34]`): width known, type not; its address is also taken (1003CC8E `lea esi, [ebp + 0x34]`) |
| 0x38 | 4 | unknown |  | low | moved/compared/tested only (1003CCAF passes this+0x34 as ecx of 1003C740; there 1003C740 `mov edx, dword ptr [ecx + 4]`): width known, type not |
| 0x40 | 4 | unknown |  | low | moved/compared/tested only (1003CCBB passes this+0x34 as ecx of 1003B7B0; there 1003B7F8 `mov dword ptr [esi + 0xc], edi`): width known, type not |
| 0x44 | 4 | unknown |  | low | moved/compared/tested only (1003CCC7 passes this+0x34 as ecx of 1003B830; there 1003B878 `mov dword ptr [esi + 0x10], edi`): width known, type not |
| 0x48 | 4 | unknown |  | low | only its address is taken (1003CCBB passes this+0x34 as ecx of 1003B7B0; there 1003B7EA `lea ebx, [esi + 0x14]`): an embedded member whose type the code here does not show |
| 0x54 | 4 | unknown |  | low | only its address is taken (1003CCC7 passes this+0x34 as ecx of 1003B830; there 1003B86A `lea ebx, [esi + 0x20]`): an embedded member whose type the code here does not show |
| 0x74 | 4 | pointer |  | high | its value is dereferenced: 1003D164 `load` then 1003D167 mov eax, dword ptr [ecx + 0x74] |
| 0x78 | 4 | float |  | high | x87 float operand: 1003D22D `fld dword ptr [esi + 0x78]` |
| 0x7C | 4 | unknown |  | low | moved/compared/tested only (1003CCD8 `mov dword ptr [ebp + 0x7c], ecx`): width known, type not |
| 0x80 | 4 | unknown |  | low | only its address is taken (1003CCE5 `lea esi, [ebp + 0x80]`): an embedded member whose type the code here does not show |
| 0xC0 | 4 | unknown |  | low | moved/compared/tested only (1003CD74 `mov dword ptr [ebp + 0xc0], 0`): width known, type not |
| 0xC4 | 4 | pointer |  | high | its value is dereferenced: 1003CE34 `load` then 1003CE3A fstp dword ptr [edx + 0x20] |

## Zone objects

### XiZone

- Size: 0x1DC. Source: runtime-class descriptor 0x1033682c (returned by vtable slot 0, 0x1018b100 `mov eax, 0x1033682c; ret`): {char *name = "XiZone"; size 0x1dc; parent descriptor}; confirmed by 0x1018B139 `push 0x1dc; call 0x1002C890 (Mem_AllocDefault)` before the constructor 0x1018B170; the result is g_pXiZone (0x1065CB04)
- Chain: XiZone (0x1DC) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x10336838
- Fields found: 26 (6 typed, 20 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x4 | 4 | unknown |  | low | moved/compared/tested only (1018B1CD `mov dword ptr [esi + 4], ebx`): width known, type not |
| 0x8 | 4 | unknown |  | low | moved/compared/tested only (1018AA1B `mov edx, dword ptr [ecx + 8]`): width known, type not |
| 0x14 | 4 | unknown |  | low | moved/compared/tested only (1018B6B0 `mov dword ptr [esi + 0x14], ecx`): width known, type not |
| 0x18 | 4 | unknown |  | low | moved/compared/tested only (1018B871 `mov eax, dword ptr [esi + 0x18]`): width known, type not |
| 0x1C | 4 | pointer |  | high | its value is dereferenced: 10187000 `load` then 10187005 mov cl, byte ptr [eax + 0x10] |
| 0xB4 | 4 | unknown |  | low | only its address is taken (10186E90 `lea eax, [ecx + 0xb4]`): an embedded member whose type the code here does not show |
| 0xD4 | 4 | unknown |  | low | only its address is taken (10186EB0 `lea eax, [ecx + 0xd4]`): an embedded member whose type the code here does not show |
| 0xE4 | 4 | unknown |  | low | only its address is taken (10186F70 `lea eax, [ecx + 0xe4]`): an embedded member whose type the code here does not show |
| 0x124 | 4 | float |  | high | x87 float operand: 100237D6 `fld dword ptr [eax + 0x124]` |
| 0x128 | 4 | float |  | high | x87 float operand: 100237E3 `fld dword ptr [eax + 0x128]` |
| 0x12C | 4 | float |  | high | x87 float operand: 100237F0 `fld dword ptr [eax + 0x12c]` |
| 0x134 | 4 | unknown |  | low | moved/compared/tested only (10023730 `mov ecx, dword ptr [edx + 0x134]`): width known, type not |
| 0x138 | 4 | unknown |  | low | moved/compared/tested only (100BA04E `mov ecx, dword ptr [eax + 0x138]`): width known, type not |
| 0x13C | 4 | unknown |  | low | moved/compared/tested only (100BA05A `mov edx, dword ptr [eax + 0x13c]`): width known, type not |
| 0x1AC | 4 | unknown |  | low | moved/compared/tested only (1018B770 `mov ecx, dword ptr [ecx + 0x1ac]`): width known, type not |
| 0x1B4 | 4 | unknown |  | low | moved/compared/tested only (100CBF80 `cmp dword ptr [eax + 0x1b4], 0x21` ; jne): width known, type not |
| 0x1BC | 4 | unknown |  | low | moved/compared/tested only (1018B5E0 `mov eax, dword ptr [ecx + 0x1bc]`): width known, type not |
| 0x1C0 | 4 | unknown |  | low | moved/compared/tested only (1018B1C1 `mov dword ptr [esi + 0x1c0], esi`): width known, type not |
| 0x1C4 | 4 | unknown |  | low | moved/compared/tested only (1018B199 `mov dword ptr [esi + 0x1c4], ebx`): width known, type not |
| 0x1C8 | 4 | unknown |  | low | moved/compared/tested only (1018B17A `lea edi, [esi + 0x1c8]`): width known, type not; its address is also taken (1018B17A `lea edi, [esi + 0x1c8]`) |
| 0x1CC | 2 | int16_t |  | high | sign-extending load: 101878A2 `movsx ecx, word ptr [eax + 0x1cc]` |
| 0x1CE | 1 | unknown |  | low | moved/compared/tested only (1018B19F `mov byte ptr [esi + 0x1ce], bl`): width known, type not |
| 0x1CF | 1 | unknown |  | low | moved/compared/tested only (100D4D74 `mov al, byte ptr [edx + 0x1cf]`): width known, type not |
| 0x1D0 | 4 | unknown |  | low | moved/compared/tested only (1018B1B1 `mov dword ptr [esi + 0x1d0], 0x422c0000`): width known, type not |
| 0x1D4 | 4 | unknown |  | low | moved/compared/tested only (1018B1BB `mov dword ptr [esi + 0x1d4], ebx`): width known, type not |
| 0x1D8 | 4 | pointer |  | high | its value is dereferenced: 1018BB10 `load` then 1018BB1A mov eax, dword ptr [eax + ecx*4] |

### CTsZoneMap

- Size: 0x395B4. Source: runtime-class descriptor 0x1033673c (returned by vtable slot 0, 0x101773c0 `mov eax, 0x1033673c; ret`): {char *name = "CTsZoneMap"; size 0x395b4; parent descriptor}
- Chain: CTsZoneMap (0x395B4) : CMoTask (0x34) : CYyObject (0x4)
- Identified by: the game's own runtime-class descriptor names it; the vtable whose slot 0 returns that descriptor is 0x10336748; the object in g_pZoneMap (0x10631F24, data/global_objects.json)
- Fields found: 226 (53 typed, 173 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x34 | 2 | int16_t |  | high | sign-extending load: 10178B34 `movsx eax, word ptr [ebx + 0x34]` |
| 0x38 | 4 | pointer |  | high | its value is dereferenced: 101787DF `load` then 101787E2 lea ecx, [eax + eax*2] |
| 0x3C | 4 | unknown |  | low | moved/compared/tested only (10177E59 `mov dword ptr [esi + 0x3c], ebx`): width known, type not |
| 0x40 | 4 | unknown |  | low | moved/compared/tested only (10178D99 `mov dword ptr [ecx + 0x40], eax`): width known, type not |
| 0x44 | 4 | unknown |  | low | moved/compared/tested only (101787C7 `mov eax, dword ptr [esi + 0x44]`): width known, type not |
| 0x48 | 2/4 | unknown |  | low | accessed with widths 2/4: not one type |
| 0x4C | 4 | unknown |  | low | moved/compared/tested only (10178811 `mov dword ptr [esi + 0x4c], eax`): width known, type not; its address is also taken (10178828 `lea ecx, [esi + 0x4c]`) |
| 0x50 | 4 | unknown |  | low | moved/compared/tested only (101788D7 `mov dword ptr [esi + 0x50], eax`): width known, type not |
| 0x54 | 4 | unknown |  | low | moved/compared/tested only (10178803 `mov dword ptr [esi + 0x54], 1`): width known, type not |
| 0x58 | 4 | unknown |  | low | only its address is taken (10177D5C `lea ecx, [esi + 0x58]`): an embedded member whose type the code here does not show |
| 0x60 | 4 | unknown |  | low | indexed access: 101787E8 `mov edx, dword ptr [esi + ecx*8 + 0x60]`; its address is also taken (10177D0B `lea edi, [esi + 0x60]`) |
| 0x98 | 4 | unknown |  | low | only its address is taken (1017C0EB `lea ecx, [ebp + 0x98]`): an embedded member whose type the code here does not show |
| 0xA0 | 4 | unknown |  | low | only its address is taken (1017AD99 `lea esi, [ecx + 0xa0]`): an embedded member whose type the code here does not show |
| 0xB4 | 4 | unknown |  | low | only its address is taken (10186D6E `lea esi, [ecx + 0xb4]`): an embedded member whose type the code here does not show |
| 0xD8 | 4 | unknown |  | low | only its address is taken (10178914 `lea ebp, [esi + 0xd8]`): an embedded member whose type the code here does not show |
| 0x150 | 4 | unknown |  | low | moved/compared/tested only (10178CC1 `mov eax, dword ptr [ebx + 0x150]`): width known, type not |
| 0x154 | 4 | unknown |  | low | moved/compared/tested only (10177D97 `mov dword ptr [esi + 0x154], ebx`): width known, type not |
| 0x158 | 4 | unknown |  | low | moved/compared/tested only (10178C01 `mov dword ptr [ebx + 0x158], 1`): width known, type not |
| 0x15C | 4 | unknown |  | low | moved/compared/tested only (10177EBD `mov dword ptr [esi + 0x15c], ebx`): width known, type not |
| 0x164 | 4 | float |  | medium | x87 float operand: 1017BF9A `fstp dword ptr [ebp + 0x164]` |
| 0x168 | 4 | float |  | medium | x87 float operand: 1017BFF7 `fld dword ptr [ebp + 0x168]` |
| 0x16C | 4 | unknown |  | low | moved/compared/tested only (10177ECE `mov dword ptr [esi + 0x16c], eax`): width known, type not |
| 0x170 | 4 | float |  | high | x87 float operand: 10181319 `fadd dword ptr [esi + 0x170]` |
| 0x174 | 4 | pointer |  | medium | its value is dereferenced: 1017B2AA `load` then 1017B2B6 mov eax, dword ptr [eax + 0x14] |
| 0x178 | - | embedded object (vtable 0x10336634 at its +0; constructor 0x10167800) |  | high | CTsZoneMap constructor 0x10177D00: `lea ecx, [esi+0x178]; call 0x10167800`, and 0x10167800 does `mov dword ptr [edx], 0x10336634` (edx = ecx); the destructor path calls 0x10336634's object destructor on it (tree/src/CTsZoneMap.cpp: Vt10336634_Destructor(&field_178)). Its size is not known (no descriptor for that vtable). |
| 0x17C | 4 | unknown |  | low | moved/compared/tested only (101788A4 `mov dword ptr [esi + 0x17c], ecx`): width known, type not |
| 0x180 | 4 | unknown |  | low | only its address is taken (1017B296 passes this+0x178 as ecx of 10167950; there 1016798B `lea edi, [esi + 8]`): an embedded member whose type the code here does not show |
| 0x18C | 4 | unknown |  | low | moved/compared/tested only (1017B296 passes this+0x178 as ecx of 10167950; there 10167954 `mov eax, dword ptr [esi + 0x14]`): width known, type not |
| 0x194 | 4 | unknown |  | low | moved/compared/tested only (1017B296 passes this+0x178 as ecx of 10167950; there 10167964 `mov eax, dword ptr [esi + 0x1c]`): width known, type not |
| 0x19C | 4 | unknown |  | low | moved/compared/tested only (1017B296 passes this+0x178 as ecx of 10167950; there 10167974 `mov eax, dword ptr [esi + 0x24]`): width known, type not |
| 0x1D8 | 4 | unknown |  | low | moved/compared/tested only (1018144C passes this+0x178 as ecx of 1016A2D0; there 1016A2DE `mov ecx, dword ptr [ebp + 0x60]`): width known, type not |
| 0x181F4 | 4 | unknown |  | low | moved/compared/tested only (1017939C passes this+0x178 as ecx of 1016C150; there 1016C150 `mov eax, dword ptr [ecx + 0x1807c]`): width known, type not |
| 0x19A74 | 4 | unknown |  | low | moved/compared/tested only (10177D29 passes this+0x178 as ecx of 10167800; there 1016781C `mov dword ptr [edx + 0x198fc], ecx`): width known, type not |
| 0x342B4 | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 101678E2 `mov dword ptr [ecx + 0x3413c], eax`): width known, type not |
| 0x342B8 | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 101678E8 `mov dword ptr [ecx + 0x34140], eax`): width known, type not |
| 0x342BC | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 101678EE `mov dword ptr [ecx + 0x34144], eax`): width known, type not |
| 0x342C0 | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 101678FA `mov dword ptr [ecx + 0x34148], eax`): width known, type not |
| 0x342C4 | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 10167900 `mov dword ptr [ecx + 0x3414c], eax`): width known, type not |
| 0x342C8 | 4 | unknown |  | low | moved/compared/tested only (10179410 `mov eax, dword ptr [ecx + 0x342c8]`): width known, type not |
| 0x342CC | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 10167912 `mov dword ptr [ecx + 0x34154], eax`): width known, type not |
| 0x342D0 | 4 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 1016790C `mov dword ptr [ecx + 0x34158], eax`): width known, type not |
| 0x342D4 | 1 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 10167918 `mov byte ptr [ecx + 0x3415c], al`): width known, type not |
| 0x342D5 | 1 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 1016791E `mov byte ptr [ecx + 0x3415d], al`): width known, type not |
| 0x342D6 | 1 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 10167924 `mov byte ptr [ecx + 0x3415e], al`): width known, type not |
| 0x342D7 | 1 | unknown |  | low | moved/compared/tested only (1017912C passes this+0x178 as ecx of 101678E0; there 1016792A `mov byte ptr [ecx + 0x3415f], al`): width known, type not |
| 0x34330 | - | embedded object (vtable 0x1033668C at its +0; constructor 0x101718E0) |  | high | CTsZoneMap constructor 0x10177D00: `lea ecx, [esi+0x34330]; call 0x101718E0`, and 0x101718E0 does `mov dword ptr [esi], 0x1033668c` (esi = ecx). Its size is not known (no descriptor for that vtable). |
| 0x343E4 | 4 | unknown | overlayTex (medium) | low | moved/compared/tested only (10177EB0 `mov dword ptr [esi + 0x343e4], ebx`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: CYyTex* of the multiplicative overlay quad |
| 0x343E8 | 4 | unknown |  | low | only its address is taken (10177D39 `lea ecx, [esi + 0x343e8]`): an embedded member whose type the code here does not show |
| 0x34428 | 4 | unknown |  | low | moved/compared/tested only (10177D44 `lea ecx, [esi + 0x34428]`): width known, type not; its address is also taken (10177D44 `lea ecx, [esi + 0x34428]`) |
| 0x3442C | 4 | unknown |  | low | moved/compared/tested only (10177D4A passes this+0x34428 as ecx of 10167440; there 10167477 `mov dword ptr [ebp + 4], 0`): width known, type not |
| 0x34430 | 4 | unknown |  | low | only its address is taken (10177D4A passes this+0x34428 as ecx of 10167440; there 1016744B `lea edi, [ebp + 8]`): an embedded member whose type the code here does not show |
| 0x355B0 | 4 | float |  | medium | x87 float operand: 1017BF88 `fst dword ptr [ebp + 0x355b0]` |
| 0x355B4 | 4 | float |  | medium | x87 float operand: 10179007 `fstp dword ptr [esi + 0x355b4]` |
| 0x37508 | 4 | unknown |  | low | moved/compared/tested only (10177D81 `lea edi, [esi + 0x37508]`): width known, type not; its address is also taken (10177D81 `lea edi, [esi + 0x37508]`) |
| 0x3750C | 4 | float |  | medium | x87 float operand: 10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F79 `fstp dword ptr [eax + 4]` |
| 0x37510 | 4 | float |  | medium | x87 float operand: 10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F4C `fstp dword ptr [eax + 8]` |
| 0x37514 | 4 | float |  | medium | x87 float operand: 10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F57 `fstp dword ptr [eax + 0xc]` |
| 0x37518 | 4 | float |  | medium | x87 float operand: 10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F62 `fstp dword ptr [eax + 0x10]` |
| 0x37520 | 4 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F82 `mov dword ptr [eax + 0x18], edx`): width known, type not |
| 0x37524 | 4 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F8B `mov dword ptr [eax + 0x1c], edx`): width known, type not |
| 0x37528 | 1 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F94 `mov byte ptr [eax + 0x20], dl`): width known, type not |
| 0x37529 | 1 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168F9D `mov byte ptr [eax + 0x21], dl`): width known, type not |
| 0x3752A | 1 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168FA6 `mov byte ptr [eax + 0x22], dl`): width known, type not |
| 0x3752B | 1 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168FAF `mov byte ptr [eax + 0x23], dl`): width known, type not |
| 0x3753C | 4 | unknown |  | low | moved/compared/tested only (10181A8D passes this+0x37508 as arg0 of 10168F40; there 10168FB8 `mov dword ptr [eax + 0x34], ecx`): width known, type not |
| 0x37540 | 4 | unknown |  | low | indexed access: 1017C301 `mov dword ptr [ebp + ecx*4 + 0x37540], eax`; its address is also taken (1017C48E `lea esi, [ebp + 0x37540]`) |
| 0x37544 | 4 | unknown |  | low | indexed access: 1017C315 `mov dword ptr [ebp + edx*4 + 0x37544], eax` |
| 0x38D40 | 4 | int32_t |  | medium | compared with a signed jump: 1017C2F0 `load` then 1017C2F6 cmp ecx, 0x200 ; jge |
| 0x38D44 | 4 | unknown |  | low | moved/compared/tested only (1017B5CE `mov dword ptr [ebp + 0x38d44], ebx`): width known, type not |
| 0x38D48 | 4 | unknown |  | low | moved/compared/tested only (1017B5D4 `mov dword ptr [ebp + 0x38d48], ebx`): width known, type not |
| 0x38D4C | 4 | unknown |  | low | moved/compared/tested only (101793CF `mov dword ptr [esi + 0x38d4c], ecx`): width known, type not |
| 0x38D50 | 4 | unknown |  | low | moved/compared/tested only (101793DE `mov dword ptr [esi + 0x38d50], edx`): width known, type not |
| 0x38D54 | 4 | unknown |  | low | moved/compared/tested only (101793E4 `mov dword ptr [esi + 0x38d54], eax`): width known, type not |
| 0x38D58 | 4 | unknown |  | low | moved/compared/tested only (101793EA `mov dword ptr [esi + 0x38d58], ecx`): width known, type not |
| 0x38D90 | 4 | unknown |  | low | moved/compared/tested only (10177E98 `mov dword ptr [esi + 0x38d90], ebx`): width known, type not |
| 0x38D94 | 4 | unknown |  | low | moved/compared/tested only (10178B03 `mov ecx, dword ptr [esi + 0x38d94]`): width known, type not; its address is also taken (1017B5C8 `lea esi, [ebp + 0x38d94]`) |
| 0x38D98 | 1 | unknown |  | low | moved/compared/tested only (10177EE5 `mov byte ptr [esi + 0x38d98], bl`): width known, type not |
| 0x38D9C | 4 | unknown |  | low | only its address is taken (1017BEFC `lea eax, [ebp + 0x38d9c]`): an embedded member whose type the code here does not show |
| 0x38DDC | 4 | unknown |  | low | only its address is taken (1017BF0F `lea edx, [ebp + 0x38ddc]`): an embedded member whose type the code here does not show |
| 0x38E1C | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833CD `fmul dword ptr [ebx]` |
| 0x38E20 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833F2 `fmul dword ptr [ebx + 4]` |
| 0x38E24 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 10183408 `fmul dword ptr [ebx + 8]` |
| 0x38E28 | 4 | unknown |  | low | moved/compared/tested only (10177E1D `mov dword ptr [esi + 0x38e28], ebx`): width known, type not |
| 0x38E2C | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833BF `fmul dword ptr [ebx + 0x10]` |
| 0x38E30 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833EB `fmul dword ptr [ebx + 0x14]` |
| 0x38E34 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 10183401 `fmul dword ptr [ebx + 0x18]` |
| 0x38E38 | 4 | unknown |  | low | moved/compared/tested only (10177E0B `mov dword ptr [esi + 0x38e38], ebx`): width known, type not |
| 0x38E3C | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833BA `fmul dword ptr [ebx + 0x20]` |
| 0x38E40 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833E6 `fmul dword ptr [ebx + 0x24]` |
| 0x38E44 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833FC `fmul dword ptr [ebx + 0x28]` |
| 0x38E48 | 4 | unknown |  | low | moved/compared/tested only (10177DF9 `mov dword ptr [esi + 0x38e48], ebx`): width known, type not |
| 0x38E4C | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833E1 `fstp dword ptr [ebx + 0x30]` |
| 0x38E50 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 101833F9 `fstp dword ptr [ebx + 0x34]` |
| 0x38E54 | 4 | float |  | medium | x87 float operand: 10178BBF passes this+0x38E1C as arg0 of 10183370; there 1018340F `fstp dword ptr [ebx + 0x38]` |
| 0x38E58 | 4 | unknown |  | low | moved/compared/tested only (10177E2F `mov dword ptr [esi + 0x38e58], edi`): width known, type not |
| 0x38E5C | 4 | unknown |  | low | only its address is taken (1017BF82 `lea esi, [ebp + 0x38e5c]`): an embedded member whose type the code here does not show |
| 0x38E9C | 4 | unknown |  | low | only its address is taken (10178FEC `lea edi, [esi + 0x38e9c]`): an embedded member whose type the code here does not show |
| 0x38EDC | 4 | unknown |  | low | only its address is taken (1017C264 `lea eax, [ebp + 0x38edc]`): an embedded member whose type the code here does not show |
| 0x38F5C | 4 | unknown |  | low | only its address is taken (1017BFFD `lea eax, [ebp + 0x38f5c]`): an embedded member whose type the code here does not show |
| 0x38F9C | 4 | unknown |  | low | only its address is taken (1017C0A6 `lea edi, [ebp + 0x38f9c]`): an embedded member whose type the code here does not show |
| 0x38FDC | 4 | unknown |  | low | only its address is taken (1017BFCB `lea edi, [ebp + 0x38fdc]`): an embedded member whose type the code here does not show |
| 0x390F0 | 4 | float |  | medium | x87 float operand: 1017CA68 `fsub dword ptr [ebp + 0x390f0]` |
| 0x390F4 | 4 | float |  | medium | x87 float operand: 1017CA72 `fsub dword ptr [ebp + 0x390f4]` |
| 0x390F8 | 4 | float |  | medium | x87 float operand: 1017CA7C `fsub dword ptr [ebp + 0x390f8]` |
| 0x390FC | 4 | unknown | cullCW (medium) | low | moved/compared/tested only (1017C1FC `mov dword ptr [ebp + 0x390fc], ebx`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: one-sided batches: nonzero -> D3DCULL_CW, else CCW |
| 0x39100 | 4 | unknown |  | low | moved/compared/tested only (1017C14C `mov dword ptr [ebp + 0x39100], ebx`): width known, type not |
| 0x39104 | 4 | unknown |  | low | moved/compared/tested only (1017C152 `mov dword ptr [ebp + 0x39104], ebx`): width known, type not |
| 0x39108 | 4 | unknown |  | low | moved/compared/tested only (1017C158 `mov dword ptr [ebp + 0x39108], ebx`): width known, type not |
| 0x3910C | 4 | unknown |  | low | moved/compared/tested only (1017C15E `mov dword ptr [ebp + 0x3910c], ebx`): width known, type not |
| 0x39110 | 4 | unknown |  | low | moved/compared/tested only (1017BB57 `mov dword ptr [ebp + 0x39110], ebx`): width known, type not |
| 0x39114 | 4 | unknown |  | low | moved/compared/tested only (1017BB4F `mov dword ptr [ebp + 0x39114], ebx`): width known, type not |
| 0x39118 | 4 | unknown |  | low | moved/compared/tested only (1017AE0E `mov dword ptr [esi + 0x39118], edi`): width known, type not |
| 0x3911C | 4 | unknown |  | low | moved/compared/tested only (1017C0F4 `mov dword ptr [ebp + 0x3911c], ebx`): width known, type not |
| 0x39120 | 4 | unknown | curTwoSided (medium) | low | moved/compared/tested only (1017C1F6 `mov dword ptr [ebp + 0x39120], ebx`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: cached two-sided state of the last batch |
| 0x39128 | 4 | unknown |  | low | moved/compared/tested only (10178FCE `mov dword ptr [esi + 0x39128], ebx`): width known, type not |
| 0x3912C | 4 | unknown |  | low | moved/compared/tested only (1017C0E5 `mov eax, dword ptr [ebp + 0x3912c]`): width known, type not |
| 0x39130 | 4 | unknown |  | low | moved/compared/tested only (10177E9E `mov dword ptr [esi + 0x39130], ebx`): width known, type not |
| 0x39134 | 4 | unknown |  | low | moved/compared/tested only (1017C112 `mov dword ptr [ebp + 0x39134], 0xffffffff`): width known, type not |
| 0x39138 | 4 | unknown |  | low | moved/compared/tested only (1017ADE9 `mov dword ptr [esi + 0x39138], 0xffffffff`): width known, type not |
| 0x3913C | 4 | unknown |  | low | only its address is taken (1017C0C0 `lea ecx, [ebp + 0x3913c]`): an embedded member whose type the code here does not show |
| 0x39140 | 4 | unknown |  | low | moved/compared/tested only (1017913F `mov dword ptr [esi + 0x39140], eax`): width known, type not |
| 0x39144 | 4 | unknown |  | low | moved/compared/tested only (10179145 `mov dword ptr [esi + 0x39144], eax`): width known, type not |
| 0x39148 | 4 | unknown |  | low | moved/compared/tested only (10179152 `mov dword ptr [esi + 0x39148], ebx`): width known, type not |
| 0x3914C | 4 | unknown |  | low | moved/compared/tested only (10179158 `mov dword ptr [esi + 0x3914c], eax`): width known, type not |
| 0x39150 | 4 | unknown |  | low | moved/compared/tested only (1017915E `mov dword ptr [esi + 0x39150], eax`): width known, type not |
| 0x39154 | 4 | unknown |  | low | moved/compared/tested only (10179164 `mov dword ptr [esi + 0x39154], eax`): width known, type not |
| 0x39158 | 4 | unknown |  | low | moved/compared/tested only (1017916A `mov dword ptr [esi + 0x39158], ebx`): width known, type not |
| 0x3915C | 4 | unknown |  | low | moved/compared/tested only (10179170 `mov dword ptr [esi + 0x3915c], ebx`): width known, type not |
| 0x39160 | 4 | unknown |  | low | moved/compared/tested only (10179176 `mov dword ptr [esi + 0x39160], ebx`): width known, type not |
| 0x39164 | 4 | unknown |  | low | moved/compared/tested only (1017917C `mov dword ptr [esi + 0x39164], ebx`): width known, type not |
| 0x39168 | 4 | unknown |  | low | moved/compared/tested only (10179182 `mov dword ptr [esi + 0x39168], ebx`): width known, type not |
| 0x3917C | 4 | unknown |  | low | moved/compared/tested only (10179188 `mov dword ptr [esi + 0x3917c], 0x42480000`): width known, type not |
| 0x39180 | 4 | unknown |  | low | only its address is taken (1017BC4D `lea esi, [ebp + 0x39180]`): an embedded member whose type the code here does not show |
| 0x391C0 | 4 | unknown |  | low | moved/compared/tested only (1017BD59 `mov eax, dword ptr [ebp + 0x391c0]`): width known, type not |
| 0x391C4 | 4 | unknown |  | low | moved/compared/tested only (1017BD5F `mov ecx, dword ptr [ebp + 0x391c4]`): width known, type not |
| 0x391C8 | 4 | unknown |  | low | moved/compared/tested only (1017BD65 `mov edx, dword ptr [ebp + 0x391c8]`): width known, type not |
| 0x391E8 | 4 | unknown |  | low | moved/compared/tested only (1017BC80 `lea edx, [ebp + 0x391e8]`): width known, type not; its address is also taken (1017BC80 `lea edx, [ebp + 0x391e8]`) |
| 0x39228 | 4 | float |  | medium | x87 float operand: 1017D787 passes this+0x391E8 as arg1 of 1000BF30; there 1000BF52 `fld dword ptr [ebx + 0x40]` |
| 0x3922C | 4 | float |  | medium | x87 float operand: 1017D787 passes this+0x391E8 as arg1 of 1000BF30; there 1000BF62 `fld dword ptr [ebx + 0x44]` |
| 0x39230 | 4 | float |  | medium | x87 float operand: 1017D787 passes this+0x391E8 as arg1 of 1000BF30; there 1000BF72 `fld dword ptr [ebx + 0x48]` |
| 0x39234 | 4 | float |  | medium | x87 float operand: 1017D787 passes this+0x391E8 as arg1 of 1000BF30; there 1000BF8F `fld dword ptr [ebx + 0x4c]` |
| 0x39244 | 4 | float |  | medium | x87 float operand: 1017D787 passes this+0x391E8 as arg1 of 1000BF30; there 1000BFAC `fld dword ptr [ebx + 0x5c]` |
| 0x39250 | 4 | unknown |  | low | moved/compared/tested only (1017BD02 `lea edi, [ebp + 0x39250]`): width known, type not; its address is also taken (1017BD02 `lea edi, [ebp + 0x39250]`) |
| 0x39254 | 4 | unknown |  | low | moved/compared/tested only (1017BD38 `lea edx, [ebp + 0x39254]`): width known, type not; its address is also taken (1017BD38 `lea edx, [ebp + 0x39254]`) |
| 0x39258 | 4 | float |  | medium | x87 float operand: 1017BE09 `fld dword ptr [ebp + 0x39258]` |
| 0x3925C | 4 | float |  | medium | x87 float operand: 1017BE0F `fsub dword ptr [ebp + 0x3925c]` |
| 0x39260 | 4 | unknown |  | low | only its address is taken (1017BC63 `lea edi, [ebp + 0x39260]`): an embedded member whose type the code here does not show |
| 0x392A0 | 4 | unknown |  | low | moved/compared/tested only (1017D489 `mov ecx, dword ptr [ebp + 0x392a0]`): width known, type not |
| 0x392A4 | 4 | unknown |  | low | moved/compared/tested only (1017D48F `mov edx, dword ptr [ebp + 0x392a4]`): width known, type not |
| 0x392A8 | 4 | unknown |  | low | moved/compared/tested only (1017D495 `mov eax, dword ptr [ebp + 0x392a8]`): width known, type not |
| 0x392C8 | 4 | unknown |  | low | only its address is taken (1017BC96 `lea edi, [ebp + 0x392c8]`): an embedded member whose type the code here does not show |
| 0x39330 | 4 | unknown |  | low | only its address is taken (1017D5EE `lea esi, [ebp + 0x39330]`): an embedded member whose type the code here does not show |
| 0x39334 | 4 | unknown |  | low | only its address is taken (1017D42D `lea esi, [ebp + 0x39334]`): an embedded member whose type the code here does not show |
| 0x39338 | 4 | unknown |  | low | only its address is taken (1017D425 `lea ebx, [ebp + 0x39338]`): an embedded member whose type the code here does not show |
| 0x3933C | 4 | float |  | medium | x87 float operand: 1017D4FD `fsub dword ptr [ebp + 0x3933c]` |
| 0x39340 | 4 | unknown |  | low | only its address is taken (1017BC70 `lea edi, [ebp + 0x39340]`): an embedded member whose type the code here does not show |
| 0x393A8 | 4 | unknown |  | low | only its address is taken (1017BCA3 `lea edi, [ebp + 0x393a8]`): an embedded member whose type the code here does not show |
| 0x39420 | 4 | float |  | medium | x87 float operand: 1017BF3A `fst dword ptr [ebp + 0x39420]` |
| 0x39424 | 4 | float |  | medium | x87 float operand: 1017BF45 `fstp dword ptr [ebp + 0x39424]` |
| 0x39428 | 4 | float |  | medium | x87 float operand: 1017BF58 `fstp dword ptr [ebp + 0x39428]` |
| 0x3942C | 4 | unknown |  | low | moved/compared/tested only (1017C106 `mov dword ptr [ebp + 0x3942c], ebx`): width known, type not |
| 0x39430 | 4 | unknown |  | low | moved/compared/tested only (1017C10C `mov dword ptr [ebp + 0x39430], ebx`): width known, type not |
| 0x39438 | 4 | unknown |  | low | moved/compared/tested only (10177E80 `mov dword ptr [esi + 0x39438], ebx`): width known, type not |
| 0x3943C | 4 | unknown |  | low | moved/compared/tested only (10177E86 `mov dword ptr [esi + 0x3943c], ebx`): width known, type not |
| 0x39440 | 4 | unknown |  | low | moved/compared/tested only (10177E8C `mov dword ptr [esi + 0x39440], ebx`): width known, type not |
| 0x39444 | 4 | unknown |  | low | moved/compared/tested only (10177E92 `mov dword ptr [esi + 0x39444], ebx`): width known, type not |
| 0x39448 | 4 | unknown |  | low | moved/compared/tested only (1017C100 `mov dword ptr [ebp + 0x39448], ebx`): width known, type not |
| 0x3944C | 4 | unknown |  | low | moved/compared/tested only (1017C0FA `mov dword ptr [ebp + 0x3944c], ebx`): width known, type not |
| 0x39450 | 4 | unknown |  | low | moved/compared/tested only (10177DDF `mov dword ptr [esi + 0x39450], ebx`): width known, type not |
| 0x39454 | 4 | unknown | vsHandleA (medium) | low | moved/compared/tested only (10178087 `mov eax, dword ptr [esi + 0x39454]`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: vertex shader from g_zoneVSDeclA |
| 0x39458 | 4 | unknown | vsHandleB (medium) | low | moved/compared/tested only (10177DA3 `mov dword ptr [esi + 0x39458], ebx`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: vertex shader from g_zoneVSDeclB |
| 0x3945C | 4 | pointer |  | medium | its value is dereferenced: 1017D00D `load` then 1017D020 fld dword ptr [ecx + 0x60] |
| 0x39460 | 4 | pointer |  | medium | its value is dereferenced: 1017D11E `load` then 1017D124 fld dword ptr [ecx + 0x60] |
| 0x39464 | 4 | pointer |  | medium | its value is dereferenced: 1017D21C `load` then 1017D222 fld dword ptr [eax + 0x60] |
| 0x39468 | 4 | pointer |  | medium | its value is dereferenced: 1017D319 `load` then 1017D31F fld dword ptr [edx + 0x60] |
| 0x3946C | 4 | unknown | vsHandles (medium) | low | moved/compared/tested only (10177DAF `mov dword ptr [esi + 0x3946c], ebx`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: 5 more vertex shader handles |
| 0x39470 | 4 | unknown |  | low | moved/compared/tested only (10177DB5 `mov dword ptr [esi + 0x39470], ebx`): width known, type not |
| 0x39474 | 4 | unknown |  | low | moved/compared/tested only (10177DBB `mov dword ptr [esi + 0x39474], ebx`): width known, type not |
| 0x39478 | 4 | unknown |  | low | moved/compared/tested only (10177DC1 `mov dword ptr [esi + 0x39478], ebx`): width known, type not |
| 0x3947C | 4 | unknown |  | low | moved/compared/tested only (10177DC7 `mov dword ptr [esi + 0x3947c], ebx`): width known, type not |
| 0x39480 | 4 | unknown |  | low | indexed access: 1017D36A `mov edx, dword ptr [ebp + esi*4 + 0x39480]` |
| 0x39494 | 4 | unknown | psHandle (medium) | low | moved/compared/tested only (1017D3BE `mov edx, dword ptr [ebp + 0x39494]`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: pixel shader handle |
| 0x39498 | 4 | unknown |  | low | moved/compared/tested only (101782C6 `mov dword ptr [esi + 0x39498], eax`): width known, type not |
| 0x3949C | 4 | int32_t |  | medium | compared with a signed jump: 1017BD4C `cmp dword ptr [ebp + 0x3949c], 1` ; jl |
| 0x394A0 | 4 | unknown |  | low | moved/compared/tested only (10178CE4 `mov dword ptr [ebx + 0x394a0], eax`): width known, type not |
| 0x394A4 | 4 | unknown | useTwoDirLights (medium) | low | moved/compared/tested only (1017BB5D `mov dword ptr [ebp + 0x394a4], edi`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: nonzero: two zone-environment directional lights |
| 0x394A8 | 4 | unknown | lightSlotDir0 (medium) | low | moved/compared/tested only (1017BB87 `mov edx, dword ptr [ebp + 0x394a8]`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: D3D light index, directional light 0 |
| 0x394AC | 4 | unknown | lightSlotDir1 (medium) | low | moved/compared/tested only (1017BC0F `mov edx, dword ptr [ebp + 0x394ac]`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: D3D light index, directional light 1 |
| 0x394B0 | 4 | unknown | lightSlotNamed (medium) | low | moved/compared/tested only (1017BB9C `mov edx, dword ptr [ebp + 0x394b0]`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: D3D light index of zone-table light 'l_01' |
| 0x394B4 | 4 | unknown |  | low | moved/compared/tested only (1017BBB5 `mov edx, dword ptr [ebp + 0x394b4]`): width known, type not |
| 0x394B8 | 4 | unknown |  | low | moved/compared/tested only (1017BBCE `mov edx, dword ptr [ebp + 0x394b8]`): width known, type not |
| 0x394BC | 4 | unknown |  | low | moved/compared/tested only (1017BBE7 `mov edx, dword ptr [ebp + 0x394bc]`): width known, type not |
| 0x394C0 | 4 | unknown |  | low | moved/compared/tested only (1017BD92 `mov dword ptr [ebp + 0x394c0], ebx`): width known, type not; its address is also taken (1017BE6D `lea ecx, [ebp + 0x394c0]`) |
| 0x394C4 | 4 | unknown |  | low | moved/compared/tested only (1017BD98 `mov dword ptr [ebp + 0x394c4], ebx`): width known, type not |
| 0x394C8 | 4 | unknown |  | low | moved/compared/tested only (1017BDA8 `mov dword ptr [ebp + 0x394c8], ebx`): width known, type not |
| 0x394CC | 4 | unknown |  | low | moved/compared/tested only (1017BDAE `mov dword ptr [ebp + 0x394cc], ebx`): width known, type not |
| 0x394D0 | 4 | unknown |  | low | moved/compared/tested only (1017BDDF `mov dword ptr [ebp + 0x394d0], eax`): width known, type not |
| 0x394D4 | 4 | unknown |  | low | moved/compared/tested only (1017BDEB `mov dword ptr [ebp + 0x394d4], ecx`): width known, type not |
| 0x394D8 | 4 | unknown |  | low | moved/compared/tested only (1017BDF1 `mov dword ptr [ebp + 0x394d8], edx`): width known, type not |
| 0x394DC | 4 | unknown |  | low | moved/compared/tested only (1017BDF7 `mov dword ptr [ebp + 0x394dc], ebx`): width known, type not |
| 0x394E0 | 4 | unknown |  | low | only its address is taken (1017BDE5 `lea eax, [ebp + 0x394e0]`): an embedded member whose type the code here does not show |
| 0x394F0 | 4 | unknown |  | low | moved/compared/tested only (1017BE20 `mov dword ptr [ebp + 0x394f0], eax`): width known, type not |
| 0x394F4 | 4 | unknown |  | low | moved/compared/tested only (1017BE26 `mov dword ptr [ebp + 0x394f4], eax`): width known, type not |
| 0x394F8 | 4 | unknown |  | low | moved/compared/tested only (1017BE32 `mov dword ptr [ebp + 0x394f8], eax`): width known, type not |
| 0x394FC | 4 | unknown |  | low | moved/compared/tested only (1017BE38 `mov dword ptr [ebp + 0x394fc], eax`): width known, type not |
| 0x39500 | 4 | float |  | medium | x87 float operand: 1017BE78 `fstp dword ptr [ebp + 0x39500]` |
| 0x39504 | 4 | unknown |  | low | moved/compared/tested only (1017BE43 `mov dword ptr [ebp + 0x39504], ebx`): width known, type not |
| 0x39508 | 4 | unknown |  | low | moved/compared/tested only (1017BE49 `mov dword ptr [ebp + 0x39508], ecx`): width known, type not |
| 0x3950C | 4 | unknown |  | low | moved/compared/tested only (1017BE4F `mov dword ptr [ebp + 0x3950c], ebx`): width known, type not |
| 0x39510 | 4 | unknown |  | low | moved/compared/tested only (1017BE55 `mov dword ptr [ebp + 0x39510], eax`): width known, type not |
| 0x39514 | 4 | unknown |  | low | moved/compared/tested only (1017BE5B `mov dword ptr [ebp + 0x39514], eax`): width known, type not |
| 0x39518 | 4 | unknown |  | low | moved/compared/tested only (1017BE61 `mov dword ptr [ebp + 0x39518], eax`): width known, type not |
| 0x3951C | 4 | unknown |  | low | moved/compared/tested only (1017BE67 `mov dword ptr [ebp + 0x3951c], eax`): width known, type not |
| 0x39540 | 4 | float |  | medium | x87 float operand: 1017D8D3 `fld dword ptr [ebp + 0x39540]` |
| 0x39544 | 4 | float |  | medium | x87 float operand: 1017D8BA `fmul dword ptr [ebp + 0x39544]` |
| 0x39548 | 4 | float |  | medium | x87 float operand: 1017D8C3 `fmul dword ptr [ebp + 0x39548]` |
| 0x3954C | 4 | float |  | medium | x87 float operand: 1017BE2C `fdivr dword ptr [ebp + 0x3954c]` |
| 0x39550 | 4 | unknown |  | low | only its address is taken (10046EE7 `lea edi, [eax + 0x39550]`): an embedded member whose type the code here does not show |
| 0x39590 | 4 | unknown |  | low | moved/compared/tested only (10177F6B `mov dword ptr [esi + 0x39590], eax`): width known, type not |
| 0x39594 | 4 | float |  | high | x87 float operand: 100461F3 `fld dword ptr [ecx + 0x39594]` |
| 0x39598 | 1 | unknown | overlayUseProjection (medium) | low | moved/compared/tested only (10177F64 `mov byte ptr [esi + 0x39598], 1`): width known, type not. Name: data/field_names.json: parallel session (gfx_fields.hpp of its bundle, in git history at 3b1bb71), checked against the uses in tree/src/CTsZoneMap.cpp: byte: overlay uses saved projection, else Z-bias |
| 0x3959C | 4 | unknown |  | low | moved/compared/tested only (10178D5A `lea ecx, [esi + 0x3959c]`): width known, type not; its address is also taken (10178D5A `lea ecx, [esi + 0x3959c]`) |
| 0x395A0 | 4 | unknown |  | low | moved/compared/tested only (10178F5B passes this+0x3959C as ecx of 10055040; there 10055097 `mov dword ptr [esi + 4], eax`): width known, type not |
| 0x395A4 | 4 | unknown |  | low | moved/compared/tested only (10178F5B passes this+0x3959C as ecx of 10055040; there 100550D0 `mov dword ptr [esi + 8], 0`): width known, type not |
| 0x395A8 | 4 | float |  | medium | x87 float operand: 10178B92 passes this+0x3959C as ecx of 10055130; there 10055130 `fld dword ptr [ecx + 0xc]` |
| 0x395B0 | 4 | unknown |  | low | moved/compared/tested only (10178F7E passes this+0x3959C as ecx of 100550F0; there 100550F4 `mov esi, dword ptr [edi + 0x14]`): width known, type not |

## Font objects

### Obj_104E1B90

- Size: 0x84. Source: App_InitScene 0x100111BB `push 0x84; call 0x10311BBB (Allocator_Generic, size argument 0)`, then `mov ecx, eax; call 0x1011BA90`; the result is the pointer at 0x104E1B90 (data/global_objects.json: created only there)
- Identified by: no runtime-class descriptor and no vtable: identified only as the object whose pointer 0x104E1B90 holds. Its methods (called with ECX = [0x104E1B90]) look up the "FontMoji" texture (0x1011BAE0) and draw glyph quads (0x1011BBD0, 0x1011C080): docs/NAMES.md Font_* names. The class name is not known.
- Fields found: 13 (8 typed, 5 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x0 | 4 | float |  | medium | x87 float operand: 1011BBE8 `fstp dword ptr [esi]` |
| 0x4 | 4 | float |  | medium | x87 float operand: 1011BC0E `fstp dword ptr [esi + 4]` |
| 0x10 | 4 | unknown |  | low | moved/compared/tested only (1011C1B5 `mov dword ptr [ebp + 0x10], eax`): width known, type not |
| 0x1C | 4 | float |  | medium | x87 float operand: 1011BBFF `fstp dword ptr [esi + 0x1c]` |
| 0x20 | 4 | float |  | medium | x87 float operand: 1011BC0B `fst dword ptr [esi + 0x20]` |
| 0x2C | 4 | unknown |  | low | moved/compared/tested only (1011C1B2 `mov dword ptr [ebp + 0x2c], eax`): width known, type not |
| 0x38 | 4 | float |  | medium | x87 float operand: 1011BBE5 `fst dword ptr [esi + 0x38]` |
| 0x3C | 4 | float |  | medium | x87 float operand: 1011BC34 `fstp dword ptr [esi + 0x3c]` |
| 0x48 | 4 | unknown |  | low | moved/compared/tested only (1011C1AF `mov dword ptr [ebp + 0x48], eax`): width known, type not |
| 0x54 | 4 | float |  | medium | x87 float operand: 1011BBFC `fst dword ptr [esi + 0x54]` |
| 0x58 | 4 | float |  | medium | x87 float operand: 1011BC31 `fst dword ptr [esi + 0x58]` |
| 0x64 | 4 | unknown |  | low | moved/compared/tested only (1011C1AC `mov dword ptr [ebp + 0x64], eax`): width known, type not |
| 0x74 | 4 | unknown |  | low | moved/compared/tested only (1011BAE6 `mov eax, dword ptr [esi + 0x74]`): width known, type not |

### Obj_104E1BF8

- Size: not known. Source: not known: a static object (never allocated); no descriptor, no allocation
- Identified by: the static object at 0x104E1BF8 (data/global_objects.json); every direct call of its methods passes ECX = 0x104E1BF8. Its methods look up the "font fontshp" shape (0x10120510) and bind the font and damage-number textures (0x10120590, 0x101207B0; docs/NAMES.md ShapeMng_*). The class name is not known.
- Fields found: 8 (2 typed, 6 unknown)

| Offset | Width | Type | Name | Conf. | Evidence |
|---|---|---|---|---|---|
| 0x10 | 4 | unknown |  | low | moved/compared/tested only (101204B7 `mov ebp, dword ptr [edi + 0x10]`): width known, type not |
| 0x1C | 4 | unknown |  | low | indexed access: 1012062C `mov eax, dword ptr [esi + eax*4 + 0x1c]` |
| 0x24 | 4 | unknown |  | low | only its address is taken (10120201 `lea eax, [ecx + 0x24]`): an embedded member whose type the code here does not show |
| 0x28 | 4 | unknown |  | low | only its address is taken (101205FB `lea ecx, [esi + 0x28]`): an embedded member whose type the code here does not show |
| 0x2C | 4 | pointer |  | high | its value is dereferenced: 10120536 `load` then 10120542 mov edx, dword ptr [ecx] |
| 0x30 | 4 | unknown |  | low | indexed access: 1012084C `mov eax, dword ptr [esi + eax*4 + 0x30]` |
| 0x3C | 4 | unknown |  | low | only its address is taken (1012081B `lea ecx, [esi + 0x3c]`): an embedded member whose type the code here does not show |
| 0x40 | 4 | pointer |  | high | its value is dereferenced: 10120756 `load` then 10120762 mov edx, dword ptr [ecx] |


## Where the tree's current declarations (and the IDB) disagree with the code

| Where | Declared | The code | Evidence |
|---|---|---|---|
| tree/include/Obj_104E1B90.hpp +0x0 | `void **vftable` | a float; the object has no vtable | 0x1011BBE8 `fstp dword ptr [esi]` in 0x1011BBD0 (this = the [0x104E1B90] object, medium: call propagation); no descriptor; data/global_objects.json: "no vtable"; the same function stores floats at +0x4, +0x1C, +0x20, +0x38, +0x3C, +0x54, +0x58 (the 28-byte-stride quad the header itself describes) |
| tree/include/CTsZoneMap.hpp +0x178 | `uint8_t field_178` | an embedded object with vtable 0x10336634 | 0x10177D23 `lea ecx, [esi+0x178]; call 0x10167800`; 0x10167800 stores 0x10336634 at [ecx] |
| tree/include/CTsZoneMap.hpp +0x34330 | `uint8_t field_34330` | an embedded object with vtable 0x1033668C | 0x10177D2E `lea ecx, [esi+0x34330]; call 0x101718E0`; 0x101718E0 stores 0x1033668C at [ecx] |
| tree/include/CTsZoneMap.hpp +0x34428, +0x3959C | `uint8_t` | addresses passed as ECX to callees (0x10167440 for +0x34428), which access a dword at +0 | 0x10177D44 `lea ecx, [esi+0x34428]; call 0x10167440`; 0x10178D5A `lea ecx, [esi+0x3959c]`: an embedded member, not a byte (its type is not known) |
| tree/include/CTsZoneMap.hpp +0x194 | `uint8_t field_194` | inside the +0x178 object; read as a dword | 0x1017B296 passes this+0x178 as ECX of 0x10167950, which reads `dword ptr [esi+0x1c]` |
| tree/include/CTsZoneMap.hpp +0x3954C | `uint32_t field_3954C` | float | 0x1017BE2C `fdivr dword ptr [ebp+0x3954c]` (medium) |
| XiLiftActor's descriptor 0x10330278 | parent CXiAtelActor | built on CXiControlActor | constructor 0x100C3990 calls 0x100A89E0 (stores 0x1032DFC8, the CXiControlActor vtable) first; its vtable shares CXiControlActor's slots 0x100A8C10 and 0x100A4CC0 |
| data/ida_named_fields.json CXiSkeletonActor +0x9C | `walkSpeed`, float | written only, by a dword `mov` | 0x100820C4 `mov dword ptr [ecx+0x9c], eax` (SetWalkSpeed); GetWalkSpeed 0x100820B0 reads +0x98 (`fld dword ptr [ecx+0x98]; fmul`), not +0x9C; no float use found: type not known |
| data/ida_named_fields.json CXiSkeletonActor +0x88 | `posHandle`, int (4) | used as bytes and as dwords | 0x10081AE3 `test byte ptr [esi+0x88], 2`; dword accesses elsewhere: one type is not shown |
| data/ida_named_fields.json CXiSkeletonActor +0xB2 | `hiddenFlagWord`, __int16 | used as bytes and as words | 0x10085AB0 `and byte ptr [ecx+0xb2], 0x7f`; 0x100C7D12 `cmp word ptr [esi+0xb2], 0` |
| data/idb_members.json CXiActor +0xA4 | `unsigned __int32` (set by a project script) | signed | 0x100827A0 loads it, 0x100827A6 `cmp eax, 1; jl` |
| CXiSkeletonActor size (not this batch's class) | IDB named struct 0xA08 (data/ida_named_fields.json) | descriptor 0x10330EBC says 0xA0C; data/field_names.json has pSkeletonRes at +0xA08 (4 bytes, ending at 0xA0C) | for #122/the integrator |
| tree/include/CMoCamera.hpp, GetSharedInstance 0x10071980 | a CMoCamera method returning a shared CMoCamera | not shown to be a CMoCamera | returns 0x109F81C0: zero-filled, nothing stores a vtable there, at-exit destructor 0x100719B0 is a bare `ret`, no direct caller |

The tree's other declared fields in these headers (CXiActor field_80/84 uint32, field_94/98 float; CTsZoneMap's named
fields; Obj_104E1B90 quad colours at +0x10/+0x2C/+0x48/+0x64) agree with the code as far as it shows them.

## Not settled (said as not known, not filled)

- **Most fields are width-only (398 of 505).** The code moves, compares for equality, tests bits or counts them; none
  of that types them. They stay `unknown` with their width.
- **Pointees.** 14 fields are pointers by dereference; what they point to is not shown (a load from the pointee, not a
  vtable). CXiActor +0x70 (`pModelInfo`, data/field_names.json: model descriptor) is a pointer; whether it points to one
  of #122's model classes is not shown here and is not asserted. CXiActor +0xB4 (`model`, PS2 GetModel) is width-only.
- **Embedded members** (`lea` only: CXiActor +0x34, +0x44, +0x5C, +0xB8; CMoCamera +0x30, +0x34, +0x70; CYyCamMng2 +0x4;
  XiZone +0xB4, +0xD4, +0xE4; and others): their type and size are not known. CXiActor +0x34 is the "focal" member
  (PS2 GetFocal) and +0x38 is a float inside it; that it is a vector is not shown.
- **Accesses past a class's size:** 0x100A9FC0 and 0x100AA1F0, whose possible `this` vtables (data/this_class.json)
  include CXiControlActor itself (size 0x5C4), write +0x5C4..+0x608. Either those functions never run on a plain
  CXiControlActor or that vtable set is too wide; which is not known, and those accesses are not attributed.
- **CYyCamMng2 (0x118)** is reached by only its constructor and reset (4 fields); the camera globals at 0x10456D70..
  0x10456DA8 that its constructor clears are separate globals, not its members.
- **Obj_104E1BF8's size** and both font objects' class names are not known. CMoCamera's static instance (above) is not
  known to be one.
- **CTsZoneMap** has 226 accessed offsets over 0x395B4 bytes; most are width-only.

