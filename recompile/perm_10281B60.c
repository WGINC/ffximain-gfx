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
// FUNCTION: FFXIMAIN 0x10281B60
void sqgxTextureGetSize(int param_1, undefined4 *param_2, undefined4 *param_3)
{
  undefined4 *new_var2;
  undefined4 *new_var4;
  undefined4 *new_var3;
  undefined4 *new_var;
  new_var4 = (undefined4 *) 0x1099D61Cu;
  new_var3 = param_2;
  if (1)
  {
    *new_var3 = (&(*new_var4))[param_1 * 10];
    new_var = &(*((undefined4 *) 0x1099D620u));
  }
  *(new_var2 = param_3) = new_var[param_1 * 10];
  return;
}
