/* Task #118: 0x10040C20, a Direct3D-calling function not in the tree, written from the game's bytes, in the form of
   recompile/d3d_100431B0.cpp (the same scratch-matrix and function-local static object idioms; d3d_round_119.md
   rules 8-10, d3d_round_120.md rule 16). Callees' conventions: data/purges.json and data/ecx_use.json (0x100746A0,
   0x1000BD50, 0x1006AD90, 0x1006AF90, 0x10045D40, 0x10183760 read ECX: __thiscall; 0x10313486, atexit, pops 0: the
   caller adds 4). Device calls: IDirect3DDevice8 slot 50 (SetRenderState) and 63 (SetTextureStageState), by d3d8.h's
   vtable order. The zone map's call is written in both arms of the test on +0x16C, each after 0x10045D40 (the game
   decides the branch before that call and merges the arms' common tail), and the bit 18 test on a byte
   (`not dl`). Classes are stand-ins; callees and globals carry their addresses. */
typedef unsigned int _DWORD;
extern int g_pResourceMng_1047BFA8;
extern char *g_pProcessor_1047CF7C;
extern void *g_pZoneMap_10631F24;
extern int g_scratchValue_1047BC0C;
extern char g_stateBlock_1047BA1C[1];
extern char g_staticGuards_1047BAA4;
extern char g_textureStageStates_103528CC;
struct FUNt_100746A0 { int f(void); };
struct FUNt_1000BD50 { int f(int, void *); };
struct FUNt_1006AD90 { void *f(void); };
struct FUNt_1006AF90 { int f(int); };
struct FUNt_10183760 { void f(void *, int, int, int); };
extern "C" int __cdecl FUN_10313486(void *);
#define DEV(g) (*(_DWORD **)((g) + 3772))
#define TSS(st, s, v) { int g_ = g_pResourceMng_1047BFA8; (*(int (__stdcall **)(_DWORD, _DWORD, int, _DWORD))(*DEV(g_) + 252))((_DWORD)DEV(g_), (st), (s), (v)); }
#define RS(s, v) { int g_ = g_pResourceMng_1047BFA8; (*(int (__stdcall **)(_DWORD, int, int))(*DEV(g_) + 200))((_DWORD)DEV(g_), (s), (v)); }
struct XE_10040C20 {
  float gx() { return *(float *)((char *)this + 0x120); } float gy() { return *(float *)((char *)this + 0x124); }
  void FUN_10045D40();
  void fn_10040C20();
};
// FUNCTION: FFXIMAIN 0x10040C20
void XE_10040C20::fn_10040C20()
{
  char *self_ = (char *)this;
  int top;
  float *m;
  if ((*(unsigned char *)(self_ + 0x17b) & 3) != 0)
  {
    top = *(int *)(g_pProcessor_1047CF7C + 0xA90);
    m = ((float **)(g_pProcessor_1047CF7C + 0xA10))[top];
    *(int *)(g_pProcessor_1047CF7C + 0xA90) = top + 1;
    ((FUNt_100746A0 *)m)->f();
    m[8] = gx();
    m[9] = gy();
    TSS(0, 11, 0);
    TSS(0, 24, 2);
    { int gm = g_pResourceMng_1047BFA8; ((FUNt_1000BD50 *)(*(_DWORD *)(gm + 0xdc8)))->f(16, m); }
    { char *pr = g_pProcessor_1047CF7C; ((float **)(pr + 0xA10))[--*(int *)(pr + 0xA90)] = m; }
  }
  else
  {
    TSS(0, 11, 0);
    TSS(0, 24, 0);
    ((FUNt_1000BD50 *)(*(_DWORD *)(g_pResourceMng_1047BFA8 + 0xdc8)))->f(16, g_pProcessor_1047CF7C + 0x810);
  }
  if ((g_staticGuards_1047BAA4 & 1) == 0)
  {
    g_staticGuards_1047BAA4 |= 1;
    ((FUNt_1006AD90 *)g_stateBlock_1047BA1C)->f();
    FUN_10313486((void *)0x10040E10u);
  }
  ((FUNt_1006AF90 *)g_stateBlock_1047BA1C)->f((int)&g_textureStageStates_103528CC);
  RS(60, g_scratchValue_1047BC0C);
  if (*(unsigned char *)(self_ + 0x16c) == 100)
  {
    FUN_10045D40();
    ((FUNt_10183760 *)g_pZoneMap_10631F24)->f(self_ + 0xa0, *(int *)(self_ + 0x104), 1, (unsigned char)~(unsigned char)(*(unsigned int *)(self_ + 0x10c) >> 18) & 1);
  }
  else
  {
    FUN_10045D40();
    ((FUNt_10183760 *)g_pZoneMap_10631F24)->f(self_ + 0xa0, *(int *)(self_ + 0x104), (*(unsigned int *)(self_ + 0x10c) >> 28) & 1, (unsigned char)~(unsigned char)(*(unsigned int *)(self_ + 0x10c) >> 18) & 1);
  }
  if ((*(unsigned char *)(self_ + 0x17b) & 3) != 0)
  {
    ((FUNt_1000BD50 *)(*(_DWORD *)(g_pResourceMng_1047BFA8 + 0xdc8)))->f(16, g_pProcessor_1047CF7C + 0x810);
    TSS(0, 24, 0);
  }
}
