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
| `pipeline/` | IDA side (`ida_gfx.py`, `ida_regflow.py`, `ida_apply_names.py`), translator (`hx2cpp.py`, `hx2clean.py`), gate (`diffemu_gfx.py`), tools |
| `data/` | Metadata inputs: the Hex-Rays export, `names_pending_idb.json` (reviewed names not yet in the IDB), class vtables, prototypes, D3D8 spec, DancingMad names, class-model headers |
| `incoming/` | The parallel session's renamed/organised tree. It has been reviewed and merged (`docs/REVIEW-parallel.md`) and is kept for provenance |
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

## Status (1 Oct 2026)

768 graphics functions:
- **633 are verified in the clean tree as built**, with 0 failures. Of these:
  - 477 are class members;
  - 156 are free functions, each in the file of the source unit it came from (29 in named units,
    127 in unnamed units grouped by link order).
- 47 are Microsoft D3DX.
- 9 cannot be expressed in C++.
- 43 fail the gate and 33 are inconclusive, each with its first divergence recorded.
- 3 do not compile.

Names:
- **1 Oct:** 545 previously unnamed functions were named from the code: 154 in the tree, plus the functions they call (`docs/NAMES-2026-10-01.md`). 6 tree functions are still unnamed.
- **2 Oct:** 193 placeholder globals and callees used by the tree were named from the code and the IDA listing (`docs/NAMES-2026-10-02.md`). The tree went from 24 `sub_` and 283 data placeholders to 13 and 105. The build is byte-identical.
  - The names are in `data/names_pending_idb.json`, and the generator applies them.
  - Run `pipeline/ida_apply_names.py` in IDA to put them in the IDB.

See `docs/PLAN.md`, `docs/RESIDUE.md`, `docs/PLACEMENT.md`, `docs/NAMES-2026-10-01.md` and `docs/NAMES-2026-10-02.md`.
