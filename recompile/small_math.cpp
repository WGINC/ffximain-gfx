// Sources that Visual C++ 6.0 SP6 with the Processor Pack's code generator (pipeline/vc6/setup_vc6.sh) compiles
// at /O2 to exactly the bytes FFXiMain.dll holds at each marked address (pipeline/vc6/match.py; docs/RECOMPILE.md).
// Each function is checked on its own: the source shape here is the one that gives those bytes, not a claim about
// names or types beyond what the bytes show (a constructor returns this in EAX; a bool return sets only AL).
struct M { float m[16]; void Identity(); };
// FUNCTION: FFXIMAIN 0x100279B0
void M::Identity() {
  m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f; m[4] = 0.0f; m[6] = 0.0f; m[7] = 0.0f; m[8] = 0.0f; m[9] = 0.0f;
  m[11] = 0.0f; m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f;
  m[0] = 1.0f; m[5] = 1.0f; m[10] = 1.0f; m[15] = 1.0f;
}
struct V { float x, y, z, w; };
struct N { char pad[0x30]; float x, y, z, w; void SetRow3(const V *p); };
// FUNCTION: FFXIMAIN 0x10027CD0
void N::SetRow3(const V *p) { x = p->x; y = p->y; z = p->z; w = p->w; }

// FUNCTION: FFXIMAIN 0x10026EB0
extern "C" void Vec3_Copy(float *d, const float *s) { d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; }
// FUNCTION: FFXIMAIN 0x10026E90
extern "C" void Vec4_Copy(float *d, const float *s) { d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3]; }
// FUNCTION: FFXIMAIN 0x10026F20
extern "C" float *Vec3_AddInPlace(float *a, const float *b) { a[0] += b[0]; a[1] += b[1]; a[2] += b[2]; return a; }
// FUNCTION: FFXIMAIN 0x10027120
extern "C" void Vec3_Sub(float *d, const float *a, const float *b) { d[0] = a[0] - b[0]; d[1] = a[1] - b[1]; d[2] = a[2] - b[2]; }
// FUNCTION: FFXIMAIN 0x100272B0
extern "C" void Vec3_ScaleInPlace(float *v, float s) { v[0] *= s; v[1] *= s; v[2] *= s; }
struct T {
  float m[16]; int f;
  T *ReturnThis(); void SetTranslation(const float *p);
};
// FUNCTION: FFXIMAIN 0x10027990
T *T::ReturnThis() { return this; }
// FUNCTION: FFXIMAIN 0x100279A0
extern "C" void Unit10026C70_Nop() {}
// FUNCTION: FFXIMAIN 0x10027CF0
void T::SetTranslation(const float *p) { m[12] = p[0]; m[13] = p[1]; m[14] = p[2]; }
struct W { short a, b; W *ClearWords(); };
// FUNCTION: FFXIMAIN 0x10126320
W *W::ClearWords() { b = 0; a = 0; return this; }
struct Cam { char pad[0x28]; float scale; };
extern Cam *DAT_104568fc;
// FUNCTION: FFXIMAIN 0x10014CF0
extern "C" float Camera_GetFrameScale() { if (DAT_104568fc->scale < 1.0f) return 1.0f; return DAT_104568fc->scale; }

struct N7 { int v[7]; N7(); };
// FUNCTION: FFXIMAIN 0x10126350
N7::N7() { v[0] = v[1] = v[2] = v[3] = v[4] = 0; v[5] = 0; v[6] = 0; }
struct RC { char pad[0x18]; int a; int pad2; int b; bool ResetCounters(); };
// FUNCTION: FFXIMAIN 0x101BA5C0
bool RC::ResetCounters() { a = 0; b = 0; return true; }
