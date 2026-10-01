# Where each function lives in `tree/` (1 Oct 2026)

Every function's reason is in `tree/placement.json` and on its `// FUNCTION:` line.

## Class members (477)

| Rule | Count | Evidence |
|---|---|---|
| Class model | 209 | The class model (PS2 DWARF + vtables) places it in the class |
| Class model, ignores `this` | 124 | A virtual method in the class's vtable whose body never reads ECX. IDA types these `__stdcall`; a member that ignores `this` and pops the same stack arguments is the same ABI. Static when the class model says so (`GetRuntimeClass`) |
| IDB name | 73 | The name is `Class_Method` for a known class, and the function is `__thiscall` |
| IDB name, static | 49 | The name says the class, but the function takes no `this`: a static member with its original convention. Includes the `X_GetRuntimeClass_variantN` copies, which are an inline accessor emitted into other units |
| Callers' global object | 10 | Every call site loads the same global into ECX, and the code that creates that global shows its class. For example, `g_pZoneMap` is assigned `new` + the `CTsZoneMap` constructor, so `ModelActor_Draw` is a `CTsZoneMap` method. Four globals hold objects with no vtable; their class is unnamed (`Obj_104E1D4C` etc.) |
| Callers pass their own `this` | 6 | Every caller is a method passing its own `this` unchanged. When several subclasses call it, it goes to their common base (`CMoElemFamily_SharedCleanup` -> `CMoElem`) |
| Typed `this` | 6 | IDA's `this` type is `CDx *` / `D3DHolder *` |

## Free functions (156), filed by source unit

Each free function goes into the file of the source unit the binary places it in. MSVC links each
object file's functions contiguously (`pipeline/unit_place.py`).

**Anchors:**
- an assert or log path (`__FILE__`);
- a constructor or destructor storing exactly one class vtable;
- the class model;
- an IDB, DancingMad or PS2 name with a known class or dancer-module prefix (`sqgxTexture*`).

Vtable-slot names are excluded as anchors, because a subclass vtable also holds inherited methods.

**Placement:**
- **Named units (29):** the function is an anchor itself, or the nearest anchors on both sides are
  the same unit and within 4 KB. A function called only from one unit and adjacent to it also
  goes there.
- **Unnamed units (127):** the code sits between anchors of different units, so it comes from an
  object file nothing identifies. These are grouped into `unit_<first address>[_<common name
  prefix>].cpp` in link order, instead of being cut into 64 KB slices.

**Why some functions stay free:**
- 40 are in the dancer library, which is C (`sq*` modules); free functions are their true form.
- 56 are game functions that take no `this` (helpers such as the `Gfx_*` device bootstrap).
- About 60 are `__thiscall` functions whose class nothing yet shows. Their callers pass a
  sub-object (`lea ecx, [esi+N]`), a stack object, or a value from a call.
  - **Next step:** name sub-object classes by their owner's field, using the constructors the parallel
    session's `gfx_fields.hpp` lists.

## Checks

- **Placement changes no code.** `pipeline/same_code.py` compiles two trees with `-ffunction-sections`
  and compares every function's section: bytes with relocation sites masked, plus relocation
  targets. The final tree matches the gated build in all 633 functions, except
  `CXiSkeletonActor::OnSkeletonLoaded`, which was fixed and re-gated (PASS).
- **Gate:** all 633 tree functions passed the gate in the clean build, 278,719 D3D calls compared.
