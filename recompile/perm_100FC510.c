/* pipeline/vc6/permute.py: a source decomp-permuter found and match.py proved identical at /O2. It gives the
   game's bytes; it is not the original source (the permuter's rewrites are what made it). */

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned __int64 uint64_t;
typedef __int64 int64_t;
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short undefined2;
typedef unsigned short ushort;
typedef unsigned short word;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef unsigned int dword;
typedef unsigned __int64 undefined8;
typedef unsigned long ulong;
typedef unsigned int size_t;
typedef int BOOL;
typedef void *LPVOID;
typedef unsigned long DWORD;
typedef unsigned int _DWORD;
typedef unsigned short _WORD;
typedef unsigned char _BYTE;
typedef unsigned char bool;
typedef int code();
double sqrt(double);
typedef unsigned char XI_PT1;
typedef unsigned short XI_PT2;
typedef unsigned int XI_PT4;
typedef unsigned __int64 XI_PT8;
typedef signed char XI_PS1;
typedef short XI_PS2;
typedef int XI_PS4;
typedef __int64 XI_PS8;
void *memcpy(void *, const void *, size_t);
DWORD timeGetTime(void);
// FUNCTION: FFXIMAIN 0x100FC510
uint8_t *Unit100D7420_GetIdPair(void)
{
  int new_var;
  if ((*((int *) 0x104DFD98u)) == 0)
  {
    return (uint8_t *) 0x0;
  }
  new_var = (*((int *) 0x104DFD98u)) + 0x3c390;
  *((undefined4 *) 0x104DF928u) = (uint32_t) (*((uint16_t *) ((*((int *) 0x104DFD98u)) + 0x40da8)));
  *((undefined4 *) 0x104DF92Cu) = *((uint32_t *) new_var);
  return (uint8_t *) (&(*((undefined4 *) 0x104DF928u)));
}
