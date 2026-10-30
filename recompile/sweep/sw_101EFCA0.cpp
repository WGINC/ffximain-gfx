/* 0x101EFCA0 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Compiled /O1 (size); the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
struct XS { void Yk_ResetFields(void); };
// FUNCTION: FFXIMAIN 0x101EFCA0
void XS::Yk_ResetFields(void)
{
  void *self_ = this;

  *(uint8_t *)((int)self_ + 0x15) = *(uint8_t *)((int)self_ + 0x15) & 0xfb;
  *(uint32_t *)((int)self_ + 0xbc) = 0;
  *(uint32_t *)((int)self_ + 0xc0) = 0;
  *(uint32_t *)((int)self_ + 0xc4) = 0x3f800000;
  *(uint32_t *)((int)self_ + 200) = 0;
  *(uint32_t *)((int)self_ + 0xcc) = 0;
  *(uint32_t *)((int)self_ + 0xd0) = 0x3f800000;
  *(uint32_t *)((int)self_ + 0xd4) = 0;
  *(uint8_t *)((int)self_ + 0xd8) = 0;
  *(uint32_t *)((int)self_ + 0xdc) = 0;
  *(uint32_t *)((int)self_ + 0xe0) = 0;
  *(uint32_t *)((int)self_ + 0xe4) = 0x3f800000;
  *(uint32_t *)((int)self_ + 0xe8) = 0;
  *(uint8_t *)((int)self_ + 0xec) = 0;
  return;

}
