# Working rules for this repository

## Do not guess, do not assume (the owner's standing instruction)

Every fact the pipeline uses (a call's pop count, an argument count, a calling convention, a comparison's
signedness, a type) must come from a definitive source, and the source must be named where it is used:

- the game's own code: a callee's `ret N` (`data/purges.json`), the caller's stack balance solved exactly
  (`pipeline/sp_solve.py`, which must pass `--selftest`), the jump instruction a comparison compiled to;
- the original running real game code (`pipeline/real_sp.py`, trusted calls only);
- declared types: the IDB's typed prototypes, the D3D8/Windows SDK headers (not types Hex-Rays makes up
  from the pushes at a call site: `callsite_args.json` entries with `"src": "cast"` are not facts).

Where no definitive source exists, the pipeline says so -- the gate's `UNRESOLVED` verdict names the call
site, `sp_solve.py` leaves the call out -- instead of picking the value that "works" or looks plausible.
A heuristic is never presented as a fact. When a rule is found to be an assumption (as the tail-call
and switch-table rules in `sp_solve.py` were), it is removed, not tuned. In conversation with the owner,
say what is known, from where, and what is not known; do not fill gaps with likely answers.

## Repository rules

- Game binaries never enter the repo (`.gitignore`); point `FFXI_DLL` at a local copy.
- No model identifiers in commits or files.
- Push only to the designated working branch; no pull request unless asked.
