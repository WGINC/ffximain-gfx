/* Tasks #118/#119, round 121: render-core functions not in the tree, at the game's bytes (pipeline/vc6/match.py, /O2).
   Written from the game's bytes and the sweep's text (recompile/d3d_round_121.md); not gate-verified (no tree source
   to gate). Being at the game's bytes, each computes what the game's function computes. Class names are stand-ins
   except CMoTask (0x10076520 is the tree's CMoTask::DrawQuadDynVB, recompile/d3d_10076520.cpp); the parameter types
   noted below are the ones that give the game's bytes, not declared types. */
// SYMBOL: ??_7X25260@@6B@ 0x1032A460
// SYMBOL: ?DrawQuadDynVB@CMoTask@@QAEHPAM0D@Z 0x10076520
struct IUnk { virtual long __stdcall QueryInterface(void *, void **); virtual unsigned long __stdcall AddRef(); virtual unsigned long __stdcall Release(); };
struct FUNt_1000A6E0 { IUnk *f(unsigned short, unsigned short, void *, int, int); };
extern "C" short __cdecl FUN_10015690(void);
extern "C" short __cdecl FUN_10015680(void);
#define g_targetTex (*(IUnk **)0x10456E6Cu)
#define g_targetSurf (*(IUnk **)0x10456E70u)
#define g_pCDx (*(FUNt_1000A6E0 **)0x1045666Cu)
#define g_projTexTarget (*(int *)0x10456E64u)

/* 0x10025260 (#119): a constructor -- it stores the vtable 0x1032A460, creates the shared render target through
   0x1000A6E0 (CDx::CreateTexture, ret 20 in data/purges.json) once, and keeps a reference to it and to the surface
   at 0x10456E70 (AddRef, IUnknown slot 1). The 0 pushed as the last argument is the game's `push eax` of the tested
   global, which VC6 gives for the literal. */
struct X25260 { IUnk *tex; IUnk *surf; virtual void v0(); X25260(); };
// FUNCTION: FFXIMAIN 0x10025260
X25260::X25260()
{
  IUnk *t = g_targetTex;
  if (t != 0 || (t = g_pCDx->f(FUN_10015680(), FUN_10015690(), &g_targetSurf, 1, 0), g_targetTex = t, t != 0))
  {
    t->AddRef();
    t = g_targetTex;
  }
  tex = t;
  IUnk *s = g_targetSurf;
  if (s != 0)
  {
    s->AddRef();
    s = g_targetSurf;
  }
  surf = s;
  g_projTexTarget = 0;
}

/* 0x100764C0 (#119): __thiscall, ret 0x24 (nine stack arguments; ECX passed through to 0x10076520, ret 12). It copies
   its first four arguments into two 16-byte locals, re-reading each from the stack, and passes both and its ninth
   argument on. As floats the four give the game's two reads (VC6 copies floats through integer registers and does not
   reuse the loads); as integers, structs or memcpy they give one. Arguments five to eight are not read. */
struct CMoTask { int DrawQuadDynVB(float *a2, float *a3, char a4); int fn_100764C0(float a, float b, float c, float d, int x5, int x6, int x7, int x8, char e); };
// FUNCTION: FFXIMAIN 0x100764C0
int CMoTask::fn_100764C0(float a, float b, float c, float d, int x5, int x6, int x7, int x8, char e)
{
  float p[4], q[4];
  p[0] = a; p[1] = b; p[2] = c; p[3] = d;
  q[0] = a; q[1] = b; q[2] = c; q[3] = d;
  return DrawQuadDynVB(p, q, e);
}
