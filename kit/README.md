# In-game kit

Tools that run inside the game process. Each one checks first that FFXiMain.dll is the analysed build
(PE TimeDateStamp 0x6A995428, `data/binary.json`), and does nothing if it isn't.

## d3dprof: a per-frame profile of the Direct3D 8 calls

`d3dprof/d3dprof.dll` (build: `d3dprof/build.sh`; MinGW, 32-bit like the game) records every call on the
device. It finds the device through `[[g_pCDx]+0xC]` (0x1045666C in the analysed image, rebased to where the
game's DLL is loaded) and gives that one object a copy of its vtable that counts each call. The driver's
own vtable and the game's code are not written.

Every 600 frames it rewrites `d3dprof_<pid>.csv` next to the DLL:

- a header line: frame time (avg/min/max ms, Present to Present), and per frame the calls, draws,
  primitives, and *redundant* state sets: SetRenderState, SetTextureStageState, SetTexture,
  SetVertexShader, SetStreamSource or SetIndices with the value that state already has. State set inside
  a state block recording is not tracked, and ApplyStateBlock and Reset clear what is known;
- one row per (method, call site): calls, redundant sets and primitives per frame. A call site is the
  return address as the analysed build's address, so it matches `data/render_map.json`. Calls from
  outside FFXiMain.dll are marked `other module`.

It is a plain DLL injected into pol.exe, the way the owner's speedfix_hook is (System Informer "Inject
DLL" or any injector). DllMain starts a thread. The thread waits, with no time limit, for FFXiMain.dll,
then for the device. It works the same on x86 Windows and on Windows on ARM, where the game runs under
x86 emulation.

With speedfix_hook: speedfix_hook patches the slots of the driver's vtable, and d3dprof gives the device a
copy of whichever vtable it finds. Either order chains: a call goes through both hooks. When speedfix
caches or merges calls, d3dprof counts them on its side of speedfix:
- injected after speedfix, d3dprof sits in front of it and counts what the game asks for;
- injected before, it counts what reaches the driver.

`test_thunks.py` runs the thunks in Unicorn: a call through the copy reaches the original
method with the caller's stack unchanged, returns with the method's own pop, and is counted.

What it does not measure: time spent inside each D3D call (that needs a return hook), and calls made on
other threads (counted as `other_thread_calls`, not profiled).
