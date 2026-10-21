struct CCamera { char pad[9]; char m_flag9; };
struct CObjA { void Method(); };
struct CObjB { void Method(); };
extern CCamera *g_pCamera;
extern CObjA *g_pObjA;
extern CObjB *g_pObjB;
void Fn10096640();
bool Fn100106C0()
{
    if (g_pCamera->m_flag9) {
        Fn10096640();
        if (g_pObjA) { g_pObjA->Method(); return false; }
        if (g_pObjB) g_pObjB->Method();
    }
    return false;
}
