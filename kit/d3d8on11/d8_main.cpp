// d8_main.cpp -- d3d8on11: the module, the log, formats, caps and the IDirect3D8 object.
#include "d8.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

// ---- caps ---------------------------------------------------------------------------------------------------------------
// The caps the game is given: what the owner's dgVoodoo answered on the owner's machine (probe8's caps.raw, phase 0
// run of 13 Oct; data/dgvoodoo_caps8.json decodes it). The game chooses its paths by caps, so it is told the same as
// under dgVoodoo and takes the paths its traces were recorded on. Where these promise something this device does not
// do yet (vertex blending, the game's own shaders), the call is logged when it comes, not hidden by smaller caps.
static const uint8_t kCaps[212] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0A, 0x20, 0x20, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x80,
  0x03, 0x00, 0x00, 0x00, 0xF0, 0xBE, 0x1B, 0x01, 0xF2, 0x0E, 0x00, 0x00, 0x91, 0x61, 0x73, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00,
  0xFF, 0x1F, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x08, 0x42, 0x08, 0x00, 0xC5, 0xEC, 0x01, 0x00, 0x00, 0x07, 0x03, 0x07, 0x00, 0x07, 0x03, 0x07,
  0x00, 0x07, 0x03, 0x07, 0x3F, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xB7, 0x43, 0xBA, 0x51, 0x80, 0x96, 0x18, 0xCB,
  0x80, 0x96, 0x18, 0xCB, 0x80, 0x96, 0x18, 0x4B, 0x80, 0x96, 0x18, 0x4B, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18, 0x00,
  0xFF, 0xFF, 0xFE, 0x03, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7B, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x0F, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0xFE, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x04, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F,
};
void d8_caps(D3DCAPS8 *c) {
  static_assert(sizeof(D3DCAPS8) == sizeof kCaps, "D3DCAPS8 is 212 bytes in d3d8caps.h");
  memcpy(c, kCaps, sizeof kCaps);
}

// ---- IDirect3D8 ---------------------------------------------------------------------------------------------------------
HRESULT D8::QueryInterface(REFIID r, void **o) {
  if (IsEqualGUID(r, IID_IUnknown) || IsEqualGUID(r, IID_IDirect3D8)) { *o = this; AddRef(); return S_OK; }
  *o = NULL; return E_NOINTERFACE;
}
ULONG D8::AddRef() { return InterlockedIncrement(&ref); }
ULONG D8::Release() { LONG n = InterlockedDecrement(&ref); if (!n) delete this; return n; }
static void current_mode(D3DDISPLAYMODE *m) {
  DEVMODEA dm; memset(&dm, 0, sizeof dm); dm.dmSize = sizeof dm;
  EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
  m->Width = dm.dmPelsWidth; m->Height = dm.dmPelsHeight; m->RefreshRate = dm.dmDisplayFrequency;
  m->Format = D3DFMT_X8R8G8B8;
}
HRESULT D8::GetAdapterIdentifier(UINT a, DWORD, D3DADAPTER_IDENTIFIER8 *id) {
  if (a || !id) return D3DERR_INVALIDCALL;
  memset(id, 0, sizeof *id); strcpy(id->Driver, "d3d8on11.dll"); strcpy(id->Description, "d3d8on11 (Direct3D 8 on Direct3D 11)");
  return D3D_OK;
}
// modes: the current one only (windowed rendering); the game's mode list, if it shows one, has that mode
UINT D8::GetAdapterModeCount(UINT a) { return a ? 0 : 1; }
HRESULT D8::EnumAdapterModes(UINT a, UINT i, D3DDISPLAYMODE *m) { if (a || i || !m) return D3DERR_INVALIDCALL; current_mode(m); return D3D_OK; }
HRESULT D8::GetAdapterDisplayMode(UINT a, D3DDISPLAYMODE *m) { if (a || !m) return D3DERR_INVALIDCALL; current_mode(m); return D3D_OK; }
HRESULT D8::CheckDeviceType(UINT a, D3DDEVTYPE t, D3DFORMAT, D3DFORMAT, WINBOOL) {
  return !a && t == D3DDEVTYPE_HAL ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDeviceFormat(UINT a, D3DDEVTYPE t, D3DFORMAT, DWORD usage, D3DRESOURCETYPE rt, D3DFORMAT f) {
  if (a || t != D3DDEVTYPE_HAL) return D3DERR_NOTAVAILABLE;
  FmtInfo i = d8_fmt(f);
  bool ok = i.dxgi != DXGI_FORMAT_UNKNOWN && (!(usage & D3DUSAGE_DEPTHSTENCIL) || i.depth) && rt != D3DRTYPE_VOLUMETEXTURE;
  if (!ok) d8_once((std::string("cdf") + std::to_string(f) + "/" + std::to_string(usage)).c_str(),
                   "CheckDeviceFormat: format %lu usage 0x%lX type %d answered not available", (unsigned long)f, (unsigned long)usage, (int)rt);
  return ok ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDeviceMultiSampleType(UINT a, D3DDEVTYPE, D3DFORMAT, WINBOOL, D3DMULTISAMPLE_TYPE m) {
  return !a && m == D3DMULTISAMPLE_NONE ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::CheckDepthStencilMatch(UINT a, D3DDEVTYPE, D3DFORMAT, D3DFORMAT, D3DFORMAT ds) {
  return !a && d8_fmt(ds).depth ? D3D_OK : D3DERR_NOTAVAILABLE;
}
HRESULT D8::GetDeviceCaps(UINT a, D3DDEVTYPE t, D3DCAPS8 *c) {
  if (a || t != D3DDEVTYPE_HAL || !c) return D3DERR_INVALIDCALL;
  d8_caps(c); return D3D_OK;
}
HMONITOR D8::GetAdapterMonitor(UINT a) { POINT p = {0, 0}; return a ? NULL : MonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY); }
HRESULT D8::CreateDevice(UINT a, D3DDEVTYPE t, HWND focus, DWORD beh, D3DPRESENT_PARAMETERS *pp, IDirect3DDevice8 **out) {
  if (!out) return D3DERR_INVALIDCALL;
  *out = NULL;
  if (a || t != D3DDEVTYPE_HAL || !pp) return D3DERR_INVALIDCALL;
  Dev *d = new Dev();
  HRESULT hr = d->init(this, focus, beh, pp);
  if (FAILED(hr)) { d8_log("CreateDevice failed: 0x%08lX", (unsigned long)hr); d->Release(); return hr; }
  *out = d; return D3D_OK;
}

// ---- the kit, loaded by this DLL (21 Nov; the owner: install once, no loader to start) -------------------------------
// The game loads d3d8.dll from its folder (FFXiMain.dll imports it), so the first Direct3DCreate8 runs inside the game
// with FFXiMain.dll in memory. If xikit.dll is next to this DLL and not loaded yet (xiload may have loaded it), and
// d3d8on11.ini's load_kit is not 0: the game's build (FFXiMain.dll's TimeDateStamp in memory) is looked up -- a
// manifest of its own (xikit_manifest_<stamp>.txt) or the analysed build's (!game_timestamp in xikit_manifest.txt). With
// one, xikit.dll is loaded (it loads speedfix and the other mods itself). Without: nothing of the kit is loaded, the
// game runs with this renderer alone, and a console window says so and where to get a kit for the build
// (update_url in d3d8on11.ini). Making a manifest for a new build (pipeline/build_translate.py) runs offline, in Python
// on an unpacked copy of the game: it is not done here.
static void kit_dir(char *d, size_t n) {
  HMODULE m = NULL; GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&kit_dir, &m);
  GetModuleFileNameA(m, d, (DWORD)n); char *s = strrchr(d, '\\'); if (s) *s = 0;
}
static uint32_t manifest_stamp(const char *path) {
  FILE *f = fopen(path, "r"); if (!f) return 0;
  char line[256]; uint32_t ts = 0;
  for (int i = 0; i < 40 && fgets(line, sizeof line, f); i++) if (!strncmp(line, "!game_timestamp ", 16)) { ts = (uint32_t)strtoul(line + 16, NULL, 16); break; }
  fclose(f); return ts;
}
static void kit_load() {
  static bool done; if (done) return; done = true;
  char dir[MAX_PATH], ini[MAX_PATH + 32], kit[MAX_PATH + 32], url[512];
  kit_dir(dir, sizeof dir);
  snprintf(ini, sizeof ini, "%s\\d3d8on11.ini", dir); snprintf(kit, sizeof kit, "%s\\xikit.dll", dir);
  if (!GetPrivateProfileIntA("d3d8on11", "load_kit", 1, ini)) { d8_log("load_kit=0: the kit is not loaded by d3d8.dll"); return; }
  if (GetFileAttributesA(kit) == INVALID_FILE_ATTRIBUTES) return;            // no kit installed: the renderer alone
  if (GetModuleHandleA("xikit.dll")) { d8_log("load_kit: xikit.dll is already loaded (xiload): left as it is"); return; }
  HMODULE game = GetModuleHandleA("FFXiMain.dll");
  if (!game) { d8_log("load_kit: no FFXiMain.dll in this process: the kit is not loaded"); return; }
  IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)game + ((IMAGE_DOS_HEADER *)game)->e_lfanew);
  uint32_t ts = nt->FileHeader.TimeDateStamp;
  char m1[MAX_PATH + 48], m0[MAX_PATH + 32];
  snprintf(m1, sizeof m1, "%s\\xikit_manifest_%08X.txt", dir, (unsigned)ts); snprintf(m0, sizeof m0, "%s\\xikit_manifest.txt", dir);
  bool known = manifest_stamp(m1) == ts || manifest_stamp(m0) == ts;
  if (known) {
    HMODULE k = LoadLibraryA(kit);
    d8_log("load_kit: FFXiMain.dll build %08X has a manifest: xikit.dll %s", (unsigned)ts, k ? "loaded" : "could not be loaded");
    if (!k) d8_log("load_kit: LoadLibrary error %lu", (unsigned long)GetLastError());
    return;
  }
  GetPrivateProfileStringA("d3d8on11", "update_url", "the project's GitHub page", url, sizeof url, ini);
  d8_log("load_kit: FFXiMain.dll build %08X: this kit has no manifest for it; the kit and its mods are not loaded (the renderer runs alone). Update: %s", (unsigned)ts, url);
  if (GetPrivateProfileIntA("d3d8on11", "update_notice", 1, ini) && AllocConsole()) {
    SetConsoleTitleA("Longthinx gobbiefix: gobbie needs update");
    HANDLE o = GetStdHandle(STD_OUTPUT_HANDLE); char msg[1400]; DWORD w;
    int n = snprintf(msg, sizeof msg,
      "Pshhh... Shkohhh... Gobbie sniffsniff this FFXiMain.dll (build %08X)... gobbie no know this one!\r\n\r\n"
      "Game still go, with gobbie's shiny renderer. But gobbie's other fixings (speedfix and friends)\r\n"
      "stay in the sack, because gobbie no know where things is in this build. Pshhh...\r\n\r\n"
      "Youuu fetch new gobbiefix from: %s\r\n\r\n"
      "(Close this window, game keep going. No want this notice? update_notice=0 in gobbiefix.ini.) Shkohhh...\r\n", (unsigned)ts, url);
    WriteConsoleA(o, msg, (DWORD)n, &w, NULL);
  }
}
// ---- gobbiefix.ini: the public settings (14 Nov) ------------------------------------------------------------------
// The handful of settings meant for players live in gobbiefix.ini (kit/install/README.md says what each does). Each is
// written through to the component file and key that reads it, before anything reads them: d3d8on11.ini (read by the
// log and Dev::init, later in this start) and speedfix.ini (read when the kit loads speedfix, in kit_load below). Only
// keys present in gobbiefix.ini, and only when the value differs. No gobbiefix.ini: nothing changes (the sessions'
// own ini files rule). debug=1, or the file gobbiefix_debug.on, turns the statistics lines on (log_stats=1 here,
// stats_every=60 in speedfix, log_hooks=1 in xikit: a line per device method hooked).
struct PubKey { const char *pub, *file, *sect, *key; };
static const PubKey kPub[] = {
  {"frame_cap", "speedfix.ini", "speedfix", "frame_cap_fps"},
  {"smooth_pacing", "speedfix.ini", "speedfix", "pace_adaptive"},
  {"smooth_names", "speedfix.ini", "speedfix", "name_smooth"},
  {"fitted_shadows", "speedfix.ini", "speedfix", "shadow_fit"},
  {"sound_retry", "speedfix.ini", "speedfix", "sound_retry"},
  {"swap_buffers", "d3d8on11.ini", "d3d8on11", "swap_buffers"},
  {"mipmaps", "d3d8on11.ini", "d3d8on11", "mipmaps"},
  {"anisotropy", "d3d8on11.ini", "d3d8on11", "anisotropy"},
  {"renderer", "d3d8on11.ini", "d3d8on11", "backend"},
  {"update_notice", "d3d8on11.ini", "d3d8on11", "update_notice"},
};
static std::string g_pub_note;                              // what was applied, logged once the log is open
static void pub_put(const char *dir, const char *file, const char *sect, const char *key, const char *val) {
  char path[MAX_PATH + 32], cur[64]; snprintf(path, sizeof path, "%s\\%s", dir, file);
  GetPrivateProfileStringA(sect, key, "\x01", cur, sizeof cur, path);
  if (!strcmp(cur, val)) return;
  WritePrivateProfileStringA(sect, key, val, path);
  g_pub_note += std::string(" ") + file + ":" + key + "=" + val;
}
void d8_apply_public(const char *dir) {
  char gob[MAX_PATH + 32]; snprintf(gob, sizeof gob, "%s\\gobbiefix.ini", dir);
  if (GetFileAttributesA(gob) == INVALID_FILE_ATTRIBUTES) return;
  for (const PubKey &k : kPub) {
    char v[64]; GetPrivateProfileStringA("gobbiefix", k.pub, "", v, sizeof v, gob);
    char *e = v + strlen(v); while (e > v && (e[-1] == ' ' || e[-1] == '\t')) *--e = 0;
    const char *b = v; while (*b == ' ' || *b == '\t') b++;
    if (*b) pub_put(dir, k.file, k.sect, k.key, b);
  }
  // debug: gobbiefix.ini's debug=1, or the marker file gobbiefix_debug.on (what gobbiefix_debug.bat on makes)
  char v[16], mk[MAX_PATH + 32]; GetPrivateProfileStringA("gobbiefix", "debug", "", v, sizeof v, gob);
  snprintf(mk, sizeof mk, "%s\\gobbiefix_debug.on", dir);
  bool marker = GetFileAttributesA(mk) != INVALID_FILE_ATTRIBUTES;
  if (*v || marker) { bool dbg = marker || atoi(v) != 0;
    pub_put(dir, "d3d8on11.ini", "d3d8on11", "log_stats", dbg ? "1" : "0");
    pub_put(dir, "speedfix.ini", "speedfix", "stats_every", dbg ? "60" : "0");
    pub_put(dir, "xikit.ini", "xikit", "log_hooks", dbg ? "1" : "0"); }
  if (g_pub_note.empty()) g_pub_note = " (all as they were)";
}
extern "C" __declspec(dllexport) IDirect3D8 *WINAPI Direct3DCreate8(UINT sdk) {
  { static bool pub; if (!pub) { pub = true; char dir[MAX_PATH]; kit_dir(dir, sizeof dir); d8_apply_public(dir); } }
  d8_log("d3d8on11: Direct3DCreate8(%u)", sdk);
  if (!g_pub_note.empty()) { d8_log("gobbiefix.ini applied:%s", g_pub_note.c_str()); g_pub_note.clear(); }
  d8_crash_install();
  kit_load();
  return new D8();
}
