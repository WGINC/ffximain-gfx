/* Task #119: 0x1003EB30, a direct caller of the Direct3D-calling functions, at the game's bytes (pipeline/vc6/match.py, /O2). Behaviour: the tree's,
   gate-verified (tree/src/CMoD3aElem.cpp); written the way the game's bytes require (recompile/d3d_round_118.md).
   Callees and globals are named by their addresses, so match.py compares every call target and address. */
extern int g_pResourceMng_1047BFA8;
extern unsigned int g_scratchValue_1047BC0C;
struct FUNt_1003E910 { void f(int, int, int); };
/* the pointer at +0x104 is read through an inline accessor that tests it again: the game's bytes test it twice
   (test eax, eax; je / test eax, eax; je), which an inlined `p ? *p : 0` after the caller's own test compiles to */
struct Ref104 { int *p; int Get() { return p ? *p : 0; } };
#pragma pack(push, 1)
struct CMoD3aElem {
  void vf18();
  char _pad0[0x104];
  Ref104 field_104;
};
#pragma pack(pop)
// FUNCTION: FFXIMAIN 0x1003EB30
void CMoD3aElem::vf18()
{
  if ( field_104.p != 0 )
    ((FUNt_1003E910 *)field_104.Get())->f(g_pResourceMng_1047BFA8, (int)&g_scratchValue_1047BC0C, (int)this);
}
