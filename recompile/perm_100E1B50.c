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
// FUNCTION: FFXIMAIN 0x100E1B50
int Unit100D7420_CountActive(void)
{
  int iVar1;
  int new_var;
  int *piVar2;
  int iVar3;
  iVar1 = 0;
  new_var = *((int *) 0x104DF770u);
  if ((new_var != 0) && ((*((int *) (new_var + 0x1369c))) != 0))
  {
    piVar2 = (int *) ((*((int *) (new_var + 0x1369c))) + 0x1f20);
    iVar3 = 2;
    do
    {
      if ((*piVar2) != 0)
      {
        iVar1 = iVar1 + 1;
      }
      piVar2 = piVar2 + 0x1f5;
      iVar3 = iVar3 + (-1);
    }
    while (iVar3 != 0);
  }
  return iVar1;
}
