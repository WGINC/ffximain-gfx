/* 0x101D6CC0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); the bytes are its proof. */
#include "vc6_shim.h"
struct XS { void fn_101D6CC0(void); };
// FUNCTION: FFXIMAIN 0x101D6CC0
void XS::fn_101D6CC0(void)
{
  void *self_ = this;

                    /* WARNING: Could not recover jumptable at 0x101d6cc2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*(int *)self_ + 0x48))();
  return;

}
