# FFXiMain.pdb

    FFXI_DLL=/path/to/FFXiMain.dll python3 sdk/pdb/make_pdb.py OUT_DIR
    FFXI_DLL=/path/to/that/build/FFXiMain.dll python3 sdk/pdb/make_pdb.py OUT_DIR kit/xikit/builds/6AE818B4.json
    FFXI_DLL=/path/to/FFXiMain.dll python3 sdk/pdb/test_pdb.py

What it holds, how it is made and checked, and how to load it in each debugger: `docs/DEVKIT.md`, "FFXiMain.pdb";
the rules for names are in `make_pdb.py`'s docstring. `OUT_DIR/FFXiMain.pdb.json` lists every symbol with the
source of its name.

| File | |
|---|---|
| `make_pdb.py` | The generator (clang, lld-link, llvm-pdbutil) |
| `types_flags.txt` | clang's flags for the tree's headers |
| `test_pdb.py`, `test/pdb_check.c` | The PDB loaded through Windows' dbghelp API (under Wine) and asked for known answers |
