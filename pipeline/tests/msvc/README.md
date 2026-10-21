# Functions matched byte for byte with the game's compiler

Each `.cpp` here compiles, with Visual C++ 6.0 and the Processor Pack code generator (c2.dll 13.0.9044, the build
FFXiMain.dll's Rich header names for its 799 C++ and 208 C objects) and `/O2`, to exactly the bytes of the game
function at the address in its name. A relocated field is compared by its target (`pipeline/msvc_match.py`).

    MSVC6_PP=<bin dir> FFXI_DLL=... python3 pipeline/msvc_match.py pipeline/tests/msvc/fn_100106C0.cpp '?Fn100106C0' 0x100106C0 pipeline/tests/msvc/fn_100106C0.json
    MSVC6_PP=<bin dir> FFXI_DLL=... python3 pipeline/msvc_match.py pipeline/tests/msvc/CDx_StepFadeCounter_1000AE40.cpp '?StepFadeCounter' 0x1000AE40 -- /O2

Both match with either front end: SP5's (C1XX 12.0.8964) or SP6's (C1XX 12.0.9782), each with the Processor
Pack's c2.dll. Both write the @comp.id 49/9044 the DLL carries. Which front end built the game is not yet known.
The compiler is never in this repository.
