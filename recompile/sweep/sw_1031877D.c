/* 0x1031877D at the game's bytes: pipeline/vc6/sweep.py's source of it as the tree writes it, unchanged but for this note and the
   mark below (pipeline/vc6/emit_identical.py). Its record: a decompiler export's, outside the tree (its raw gate did not pass); compiled /O1 (size), the bytes are its proof. */
// VC6FLAGS: /O1
#include "vc6_shim.h"
#ifndef _FILE_DEFINED
struct _iobuf { char *_ptr; int _cnt; char *_base; int _flag; int _file; int _charbuf; int _bufsiz; char *_tmpfname; };
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif
undefined8 __cdecl FUN_10312EAD(unsigned int);
// FUNCTION: FFXIMAIN 0x1031877D
void __cdecl fn_1031877D(FILE * _File)
{
  if (((_File->_flag & 0x83U) != 0) && ((_File->_flag & 8U) != 0)) {
    FUN_10312EAD(_File->_base);
    *(ushort *)&_File->_flag = (ushort)_File->_flag & 0xfbf7;
    _File->_ptr = (char *)0x0;
    _File->_base = (char *)0x0;
    _File->_cnt = 0;
  }
  return;
}