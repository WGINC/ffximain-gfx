/* 0x10311F61 at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#ifndef _FILE_DEFINED
struct _iobuf { char *_ptr; int _cnt; char *_base; int _flag; int _file; int _charbuf; int _bufsiz; char *_tmpfname; };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif
undefined4 __cdecl FUN_10318805(unsigned int);
void __cdecl FUN_1031877D(unsigned int);
undefined4 __cdecl FUN_10315D9F(unsigned int);
undefined8 __cdecl FUN_10312EAD(unsigned int);
// FUNCTION: FFXIMAIN 0x10311F61
undefined4 __cdecl fn_10311F61(FILE * param_1)
{
  int iVar1;
  undefined4 uVar2;
  
  uVar2 = 0xffffffff;
  if ((param_1->_flag & 0x83) != 0) {
    uVar2 = FUN_10318805((int *)param_1);
    FUN_1031877D(param_1);
    iVar1 = FUN_10315D9F(param_1->_file);
    if (iVar1 < 0) {
      uVar2 = 0xffffffff;
    }
    else if (param_1->_tmpfname != (char *)0x0) {
      FUN_10312EAD(param_1->_tmpfname);
      param_1->_tmpfname = (char *)0x0;
    }
  }
  param_1->_flag = 0;
  return uVar2;
}