# Worker sessions: matching, naming and typing batches

docs/PLAN.md ("Speeding it up", item 7). Work that shares no files is handed to separate sessions, each on its own
machine. This session (the integrator) owns everything shared: data/ facts, tree/ regeneration, gating, promotion,
verify_all and the commits. A worker never edits those; it returns files the integrator checks and installs.

CLAUDE.md holds for workers as for this session. In particular:
- every fact names a definitive source;
- game binaries and the compiler never enter the repo;
- no model identifiers in commits or files;
- pushes go only to the branch the worker was given.

## What a worker needs (not in the repo)

| Need | Why | How it reaches the worker |
|---|---|---|
| FFXiMain.dll, the unpacked build the repo's facts are for (data/binary.json: its SHA-256) | match.py compares against its bytes | Uploaded by the owner into the worker's session; `FFXI_DLL` points at it |
| The Visual Studio 6 archive (en_vs6_ent.zip) | setup_vc6.sh builds the game's compiler from it | Uploaded by the owner, or fetched by the worker from the owner's link if the environment's network allows it |
| Wine, cabextract, unzip, pycdlib | setup_vc6.sh and match.py | Installed by the worker (as this session did) |

Not known: whether a worker session can be given the owner's uploads without the owner uploading each time.
Until that is known, starting a worker costs the owner two uploads.

The setup check: `VC6=... python3 pipeline/vc6/match.py --compid` must print the stamp docs/RECOMPILE.md lists
(48/49 build 9044), and `match.py --all` must report every function in recompile/ IDENTICAL. A worker whose
toolchain fails either does no matching.

## Matching batch

**Input:** a list of addresses (from verification/vc6_sweep.json and the frame-set status file), each with:
- the tree source of its function (tree/src);
- the sweep's verdict and first difference;
- its callers and callees (data/).

**Work:** for each function, write a source in `recompile/<batch>.cpp`, preceded by `// FUNCTION: FFXIMAIN 0x...`,
that match.py reports IDENTICAL. The rules:
- **Behaviour is the tree's.** The source changes how the code is written, never what it computes. The tree's
  version is gate-verified; a source that matches the bytes but computes something else cannot exist, because the
  bytes are the game's.
- **Types, names and field layouts come from the tree, the IDB's declared prototypes or the D3D8/Windows SDK
  headers.** A shape the bytes force (a constructor returns this; a bool sets only AL) is written as such, with a
  comment, not given a name it has no source for.
- **A function that will not match after a fair attempt is reported, not forced.** The report gives the
  function's address, the closest diff (first differing offset, both instruction streams) and what was tried.
- **A rule found to decide bytes is written down with a minimal example.** For example: statement order decides
  store order; member-by-member assignment beats a struct copy. Such rules go into sweep.py's bulk rewrites (task
  #163).

**Output:**
- the batch's `.cpp`;
- a report `recompile/<batch>.md`: per address IDENTICAL, or not matched with the closest diff;
- the rules found.

**Integration:** the integrator runs `match.py --all`. Every function in recompile/ must be IDENTICAL, or nothing
from the batch is installed.

## Naming and typing batches

As rounds 1–4 (batches/round4/: one `.md` per batch, one `data/names/rN_*.json` per batch). A name or a type has the
confidence its evidence gives (high / medium / low, with the evidence quoted). Area words the code does not show are
not used (docs/PROBLEMS.md P15). The integrator applies them through the tree regeneration and gates what changes.

## Batches and conflicts

- One batch is one file set, so two workers never write the same file.
- Batches go in plan order: the render core first (PLAN.md step 20), then its callers, then the rest of the frame set.
- The integrator keeps the list of batches handed out (batches/match/index.json) and what came back.
