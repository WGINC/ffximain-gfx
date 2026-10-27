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
// FUNCTION: FFXIMAIN 0x10123680
void Cell_SetWord1036D6B0(undefined4 param_1, undefined4 param_2)
{
  int iVar1;
  undefined4 *new_var;
  iVar1 = (int) ((short) param_1);
  *((uint16_t *) (((char *) (&(*((undefined4 *) 0x1036D6B0u)))) + (iVar1 * 6))) = 0;
  if (((char) param_2) != '\0')
  {
    new_var = &param_2;
    *((uint16_t *) (((char *) (&(*((undefined4 *) 0x1036D6B0u)))) + (iVar1 * 6))) = (uint16_t) ((((uint16_t) ((uint8_t) 0x10)) << 8) | ((uint8_t) ((char) (*new_var))));
  }
  return iVar1 * 3;
}
