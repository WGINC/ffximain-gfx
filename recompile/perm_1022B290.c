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
undefined4 FUN_10014D60(void);
undefined4 FUN_100FCD70(void);
void FUN_1022B3B0(unsigned int);
// FUNCTION: FFXIMAIN 0x1022B290
void Unit1021D930_TickCounters(void)
{
  uint32_t uVar1;
  int iVar2;
  short *psVar3;
  psVar3 = &(*((undefined2 *) 0x106671E0u));
  do
  {
    if (0 < (*psVar3))
    {
      uVar1 = FUN_10014D60();
      *psVar3 = (*psVar3) - ((short) uVar1);
      if ((*psVar3) < 0)
      {
        *psVar3 = 0;
      }
    }
    psVar3 = psVar3 + 1;
  }
  while (((int) psVar3) < 0x106679e2);
  iVar2 = FUN_100FCD70();
  if (!(iVar2 == 0))
  {
    if ((*((int *) 0x10667A88u)) != iVar2)
    {
      FUN_1022B3B0(0x3c);
      *((int *) 0x10667A88u) = iVar2;
      return;
    }
  }
  else
  {
    *((int *) 0x10667A88u) = 0;
  }
  return;
}
