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
void *memset(void *, int, size_t);
char *strcpy(char *, const char *);
int memcmp(const void *, const void *, size_t);
char *strcat(char *, const char *);
int strcmp(const char *, const char *);
size_t strlen(const char *);
int _isnan(double);
typedef signed char sbyte;
typedef __int64 longlong;
typedef unsigned __int64 ulonglong;
typedef unsigned __int64 qword;
typedef unsigned char xi_bool;
typedef unsigned __int64 _QWORD;
DWORD timeGetTime(void);
typedef unsigned int XI_ANY;
// FUNCTION: FFXIMAIN 0x10036050
void Unit10035B00_SetScaled10351704(float param_1)
{
  float *new_var;
  new_var = &(*((float *) 0x103516F8u));
  *((float *) 0x10351704u) = (float) ((((long double) (*((float *) 0x103516FCu))) * ((long double) (*new_var))) * ((long double) param_1));
  return;
}
