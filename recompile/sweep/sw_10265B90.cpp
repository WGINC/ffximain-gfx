/* 0x10265B90 at the game's bytes: pipeline/vc6/sweep.py's source of it under rule xiany, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's (ghidra_frameset18), outside the tree; the bytes are its proof. */
#include "vc6_shim.h"
typedef uint32_t (__stdcall *CloseHandle_t)(unsigned int);
#define CloseHandle (*(CloseHandle_t *)0x10329104u)
struct XS { void fn_10265B90(void); };
// FUNCTION: FFXIMAIN 0x10265B90
void XS::fn_10265B90(void)
{
  void *self_ = this;

  if (*(int *)self_ != 0) {
    CloseHandle(*(int *)self_);
  }
  return;

}
