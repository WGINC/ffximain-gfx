/* 0x1031A00D at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#define DAT_103d0900_as_v0 (*(DWORD *)0x103D0900u)
typedef uint32_t (__stdcall *GetCurrentThreadId_t)();
#define GetCurrentThreadId (*(GetCurrentThreadId_t *)0x10329218u)
typedef uint32_t (__stdcall *GetLastError_t)();
#define GetLastError (*(GetLastError_t *)0x10329168u)
typedef void (__stdcall *SetLastError_t)(XI_ANY);
#define SetLastError (*(SetLastError_t *)0x103290C4u)
typedef uint32_t (__stdcall *TlsGetValue_t)(XI_ANY);
#define TlsGetValue (*(TlsGetValue_t *)0x10329278u)
typedef uint32_t (__stdcall *TlsSetValue_t)(XI_ANY, XI_ANY);
#define TlsSetValue (*(TlsSetValue_t *)0x1032927Cu)
int * __cdecl FUN_103175B0(unsigned int, unsigned int);
void __cdecl FUN_10319FFA(unsigned int);
void __cdecl FUN_103167CC(unsigned int);
// FUNCTION: FFXIMAIN 0x1031A00D
DWORD * __cdecl fn_1031A00D(void)
{
  DWORD dwErrCode;
  DWORD *a2;
  int iVar1;
  DWORD DVar2;
  
  dwErrCode = GetLastError();
  a2 = TlsGetValue(DAT_103d0900_as_v0);
  if (a2 == (DWORD *)0x0) {
    a2 = (DWORD *)FUN_103175B0(1, 0x74);
    if (a2 != (DWORD *)0x0) {
      iVar1 = TlsSetValue(DAT_103d0900_as_v0,a2);
      if (iVar1 != 0) {
        FUN_10319FFA((int)a2);
        DVar2 = GetCurrentThreadId();
        a2[1] = 0xffffffff;
        *a2 = DVar2;
        goto LAB_1031a068;
      }
    }
    FUN_103167CC(0x10);
  }
LAB_1031a068:
  SetLastError(dwErrCode);
  return a2;
}