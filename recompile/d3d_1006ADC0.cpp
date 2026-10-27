/* Task #118: 0x1006ADC0, a Direct3D-calling function not in the tree, at the game's bytes (pipeline/vc6/match.py,
   /O2). Written from the game's bytes and Ghidra's text (recompile/d3d_round_120.md); not gate-verified (no tree
   source to gate). A state-block holder: if its id is not -1, the device's slot 0xE0 is called with it
   (DeleteStateBlock: slot 56 of IDirect3DDevice8 in tree/include/xi_d3d8.hpp), the id set to -1, and 0x1006AE10
   tail-called (its bytes unlink the object from a doubly linked list at +4 / +8). `SB` is a stand-in class name;
   the functions carry their addresses. */
struct SB { int id; void fn_1006ADC0(); void fn_1006AE10(); };
#define g_pResourceMng_1047BFA8 (*(char **)0x1047BFA8u)

// FUNCTION: FFXIMAIN 0x1006ADC0
void SB::fn_1006ADC0()
{
  if (id != -1)
  {
    void *d = *(void **)(g_pResourceMng_1047BFA8 + 0xebc);
    (*(int (__stdcall **)(void *, int))(*(int *)d + 0xe0))(d, id);
    id = -1;
    fn_1006AE10();
  }
}
