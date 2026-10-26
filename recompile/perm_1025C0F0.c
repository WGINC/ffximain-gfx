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
uint32_t FUN_1025BE80(unsigned int);
uint32_t FUN_1025BFB0(unsigned int, unsigned int, unsigned int);
undefined4 FUN_1011DA80(unsigned int);
size_t FUN_1011DE80(unsigned int, unsigned int, unsigned int);
// FUNCTION: FFXIMAIN 0x1025C0F0
int Unit10253A20_WrapSjisAt(int param_1, int param_2, uint8_t *param_3)
{
  uint32_t uVar1;
  int iVar2;
  size_t sVar3;
  uVar1 = FUN_1025BE80(param_3);
  if (((int) uVar1) >= param_2)
  {
    return 0;
  }
  uVar1 = FUN_1025BFB0((uint8_t *) param_1, param_2, param_3);
  iVar2 = FUN_1011DA80(param_3);
  ;
  return FUN_1011DE80((char *) (uVar1 + param_1), param_2 - uVar1, param_3 + iVar2) + uVar1;
}
