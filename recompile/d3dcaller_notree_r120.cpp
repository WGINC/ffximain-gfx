/* Task #119: seven direct callers of the render core that are not in the tree, at the game's bytes
   (pipeline/vc6/match.py, /O2). Written from the game's bytes and Ghidra's text (recompile/d3d_round_120.md); not
   gate-verified (no tree source to gate). Class names are stand-ins except CDx (0x10009F20 is the tree's CDx
   method, as in recompile/d3dcaller_10009EA0.cpp) and CMoTask (0x10076030 is the tree's CMoTask::DrawQuadStates);
   functions the repository does not name carry their addresses.
   0x1006B090 is a destructor: it stores the vtable 0x1032C10C (Ghidra's label PTR_CMoDxStatusMng_GetRuntimeClass),
   walks the list at +0xC calling 0x1006ADC0 on each node, and ends in the destructor of the member at +4
   (0x1006ADB0, tail-jumped) -- what the compiler emits for this class; the vtable and the member's destructor get
   their game addresses by SYMBOL lines. */
// SYMBOL: ??_7CMoDxStatusMng@@6B@ 0x1032C10C
// SYMBOL: ??1List4@@QAE@XZ 0x1006ADB0
// SYMBOL: ?Begin@Pass@@QAE_NXZ 0x10054D70
// SYMBOL: ?End@Pass@@QAEXPAUV3@@0@Z 0x10054F80
typedef unsigned int undefined4;
struct FUNt_10009F20 { int f(int, int, int, __int16 *, __int16 *, int, int); };
struct FUNt_100759C0 { int f(int, int); };
struct FUNt_1003AAB0 { int *f(void); };
struct Q4 { float a; undefined4 b, c, d; };
struct CMoTask { int DrawQuadStates(Q4 *q, int e); int fn_10075FF0(float a, undefined4 b, undefined4 c, undefined4 d, int e); };
struct CDx { void fn_10009E60(int *param_1, undefined4 xi_s2, undefined4 xi_s3); };
struct TH { int tex; int fn_10075700(int, int, float, undefined4); void fn_10075690(int a, int b, float c, undefined4 d, char *name); };
struct Node { char pad[8]; Node *next; void fn_1006ADC0(); };
struct List4 { char pad[8]; ~List4(); };
struct CMoDxStatusMng { List4 list; Node *head; virtual void v0(); ~CMoDxStatusMng(); };
extern "C" float __cdecl fn_10002D80(void);
struct CMoDistModelElem { int DrawBatches(undefined4 *scratch, int elem); };
struct XE { void fn_10040BA0(); void fn_10040C20(); };
extern undefined4 DAT_1047BC0C;
struct V3 { float x, y, z; };
struct Pass { char c; bool Begin(); void End(V3 *, V3 *); };     /* as recompile/d3dcaller_10040E70.cpp */
struct XD { float pad[52]; float px, py, pz; void fn_1003EB30(); void fn_10045D40(); void fn_1003EE70(); };
extern FUNt_100759C0 DAT_1047BB90;
#define g_1047B970 (*(FUNt_1003AAB0 **)0x1047B970u)

// FUNCTION: FFXIMAIN 0x10044250
extern "C" void __cdecl fn_10044250(int stage, int color)
{
  DAT_1047BB90.f(stage, color);
}

// FUNCTION: FFXIMAIN 0x10009E60
void CDx::fn_10009E60(int *param_1, undefined4 xi_s2, undefined4 xi_s3)
{
  __int16 local_4[2];
  local_4[0] = 0;
  local_4[1] = 0;
  ((FUNt_10009F20 *)this)->f((int)param_1, xi_s2, xi_s3, local_4, local_4, 0, 1);
}

// FUNCTION: FFXIMAIN 0x10075FF0
int CMoTask::fn_10075FF0(float a, undefined4 b, undefined4 c, undefined4 d, int e)
{
  Q4 q;
  q.a = a; q.b = b; q.c = c; q.d = d;
  return DrawQuadStates(&q, e);
}

// FUNCTION: FFXIMAIN 0x10075690
void TH::fn_10075690(int a, int b, float c, undefined4 d, char *name)
{
  if (tex == 0)
  {
    int found = 0;
    int *pp = g_1047B970->f();
    for (; *pp != 0; pp = (int *)(*pp + 0x38))
    {
      int i;
      for (i = 8; i < 16; i++)
        if (*(char *)(*pp + 0xc + i) != name[i]) break;
      if (i == 16) { found = *pp; break; }
    }
    tex = found;
  }
  fn_10075700(a, b, c, d);
}

// FUNCTION: FFXIMAIN 0x1006B090
CMoDxStatusMng::~CMoDxStatusMng()
{
  Node *n = head;
  while (n != 0) { Node *nx = n->next; n->fn_1006ADC0(); n = nx; }
}

// FUNCTION: FFXIMAIN 0x10040BA0
void XE::fn_10040BA0()
{
  char *self_ = (char *)this;
  if (*(int *)(self_ + 0x104) != 0)
  {
    int o = *(int **)(self_ + 0xfc) ? **(int **)(self_ + 0xfc) : 0;
    if ((*(char *)(o + 0xde) & 0x80) && fn_10002D80() == 0.0f) return;   /* & 0x80: test/jns, as the game; < 0 gives jge */
    if (*(char *)(self_ + 0x179) != 0) { fn_10040C20(); return; }
    ((CMoDistModelElem *)(*(int **)(self_ + 0x104) ? **(int **)(self_ + 0x104) : 0))->DrawBatches(&DAT_1047BC0C, (int)self_);
  }
}

// FUNCTION: FFXIMAIN 0x1003EE70
void XD::fn_1003EE70()
{
  if (*(int *)((char *)this + 0x104) != 0)
  {
    Pass pass;
    pass.Begin();
    fn_1003EB30();
    V3 v;
    fn_10045D40(); v.x = px; fn_10045D40(); v.y = py; fn_10045D40(); v.z = pz;
    V3 z;
    z.z = 0.0f; z.y = 0.0f; z.x = 0.0f;
    pass.End(&v, &z);
  }
}
