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
undefined8 FUN_10285890(unsigned int, ...);
// FUNCTION: FFXIMAIN 0x10277A50
uint32_t SqmdModel_Validate(int param_1)
{
  uint32_t uVar1;
  unsigned int uVar2;
  int new_var;
  uVar1 = *((uint32_t *) (param_1 + 0xa8));
  if ((uVar1 & 1) == 0)
  {
    FUN_10285890(*((char (*)[1]) 0x103B379Cu), *((char (*)[1]) 0x103B37ECu), 0x77);
    uVar2 = FUN_10285890(*((char (*)[1]) 0x103B3848u));
    return ((uint32_t) ((uint32_t) (((uint64_t) uVar2) >> 8))) << 8;
  }
  if ((uVar1 & 2) == 0)
  {
    new_var = 0x7d;
    FUN_10285890(*((char (*)[1]) 0x103B379Cu), *((char (*)[1]) 0x103B37ECu), new_var);
    uVar2 = FUN_10285890(*((char (*)[1]) 0x103B381Cu));
    return ((uint32_t) ((uint32_t) (((uint64_t) uVar2) >> 8))) << 8;
  }
  return (uint32_t) ((((uint32_t) ((int32_t) (uVar1 >> 8))) << 8) | ((uint8_t) 0xff));
}
