# FFXiMain.dll graphics reconstruction (build 2026-09-03)

Gate-verified C++ for the graphics code of the FFXI PC client, produced from IDA Pro / Hex-Rays
output and checked function by function against the original in an emulator.
Private work: nothing here is published, and no game bytes are in the repo.

## Layout

| Path | What |
|---|---|
| `tree/` | The deliverable: clean, class-shaped C++ (`include/`, `src/`, `build.sh`). Each function carries its `// GATE:` result |
| `d3dx/` | The statically linked DirectX 8.1 D3DX code: manifest of 825 byte-matched functions, binding header |
| `verification/` | Gate results (raw and clean), cases, first-divergence localisation, mutation test |
| `docs/` | Reports (`REPORT-gfx2.md` is current), `RESIDUE.md` (every function not in the verified tree, with the reason), `PLAN.md` |
| `pipeline/` | IDA side (`ida_gfx.py`, `ida_regflow.py`), translator (`hx2cpp.py`, `hx2clean.py`), gate (`diffemu_gfx.py`), tools |
| `data/` | Metadata inputs: the Hex-Rays export, class vtables, prototypes, D3D8 spec, DancingMad names, class-model headers |
| `incoming/` | Work from elsewhere not yet merged (the parallel session's renamed/organised tree) |
| `runs/` | Scratch output of `run.sh` (ignored) |

## Reproduce

    export FFXI_DLL=/path/to/FFXiMain_unpacked.dll   # never copied into the repo
    ./run.sh all data/exports/gfx_export_current.jsonl myrun

`run.sh` translates the export, builds one unit per function, gates every function, generates the
clean tree from the passing ones, gates that tree as built, annotates it and writes `RESIDUE.md`,
all under `runs/myrun/`. Steps can be run one at a time (`./run.sh translate|cases|gate|clean|cleangate|residue`).
A full gate takes about 40 minutes on 2 cores.

Checked on 1 Oct 2026: `translate` reproduces the delivered raw translation object-for-object, and
`clean` + `annotate_gate.py` reproduce `tree/` byte-for-byte.

Requirements: Python 3 with `unicorn` and `pefile`, MinGW-w64 i686 (`i686-w64-mingw32-g++`),
IDA Pro 9.x with Hex-Rays for new exports. `gen_d3dx_bind.py` needs `DX81_INCLUDE` pointing at the
DirectX 8.1 SDK headers.

## Status

See `docs/REPORT-gfx2.md` and `docs/PLAN.md`. In short: 768 graphics functions, 624 verified as
decompiled C++, 620 verified in the clean tree as built (0 FAIL), 47 Microsoft D3DX, 9 not
expressible in C++, 42 FAIL / 32 inconclusive with first divergence recorded.
