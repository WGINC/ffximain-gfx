#!/usr/bin/env bash
# run.sh -- the graphics reconstruction pipeline, end to end, from an IDA export.
#
#   FFXI_DLL=/path/to/FFXiMain_unpacked.dll ./run.sh all [EXPORT] [RUN]
#
# Steps (each can be run alone: ./run.sh facts|translate|cases|gate|clean|cleangate|residue ...; ./run.sh test
# runs the rule tests and the stack solver's self-test):
#   facts      per-function facts read from the DLL: x87 constants, return kinds, consumed x87 results
#   translate  Hex-Rays export -> one C++ unit per function (runs/RUN/raw), build, cast repairs
#   cases      gate cases for every translated function
#   gate       equivalence gate, original vs reconstruction (work queue, $JOBS workers, default one per core;
#              verdicts of provably identical runs reused from runs/gate_cache, pipeline/gate_cache.py)
#   clean      class-shaped tree from the passing functions -> runs/RUN/tree
#   cleangate  gate the clean tree exactly as built; annotate each function with its result
#   residue    RESIDUE.md: every graphics function not in the verified tree, with the reason
# The game DLL is read from $FFXI_DLL and is never copied into the repo.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"; P="$ROOT/pipeline"
EXP=$(realpath "${2:-$ROOT/data/exports/gfx_export.jsonl}"); RUN=${3:-dev}; W="$ROOT/runs/$RUN"
mkdir -p "$W"
# the verdicts the tree is built from: the fuzz gate, or GATE_IN=FILE (combine_gates.py: fuzz and live gates)
GIN=$(realpath -m "${GATE_IN:-$W/gate_raw.jsonl}"); CLN="$W/gate_clean.jsonl"; [ -n "${GATE_IN:-}" ] && CLN="$W/gate_clean_combined.jsonl"
# every stage after translate must read the export the raw translation came from (runs/RUN/raw/export.sha1)
same_export() {
  [ -f "$W/raw/export.sha1" ] || return 0
  [ "$(sha1sum < "$EXP")" = "$(cut -d' ' -f2- "$W/raw/export.sha1")" ] && return 0
  echo "run.sh: $EXP is not the export runs/$RUN/raw was translated from ($(cut -d' ' -f1 "$W/raw/export.sha1")); pass that one" >&2; exit 1
}
gate2() {  # gate2 DLL CASES OUTPREFIX: a work queue of small chunks, $JOBS workers (default: one per core)
  python3 "$P/gate_some.py" "$(dirname "$3")" all --dll "$1" --cases "$2" --out "$(basename "$3")" --jobs "${JOBS:-$(nproc)}" > "$3.log" 2>&1
}
case "$1" in
  test)          # the translator rules and the stack solver: fixed expectations, then the solver's self-test
                 python3 -m unittest discover -s "$P/tests" || exit 1
                 python3 "$P/sp_solve.py" --selftest || exit 1 ;;
  facts|all)     # facts read from the game DLL that the translator and the gate use (data/*.json)
                 python3 "$P/fpconst.py" "${FFXI_DLL:?set FFXI_DLL}" "$EXP" "$ROOT/data/fp_consts.json"
                 python3 "$P/ret_kinds.py" "$FFXI_DLL" "$ROOT/data/ret_kinds.json"
                 python3 "$P/fpret.py" "$FFXI_DLL" "$ROOT/data/func_extents.json" "$ROOT/data/fpret.json"
                 python3 "$P/fp_callsites.py" "$FFXI_DLL" "$EXP" "$ROOT/data/fp_callsites.json"
                 python3 "$P/purges.py" "$FFXI_DLL" "$ROOT/data/purges.json"
                 python3 "$P/vcall_resolve.py" "$ROOT/data/vcalls.json"
                 python3 "$P/com_calls.py" "$ROOT/data/com_calls.json"
                 python3 "$P/edx_inputs.py" "$ROOT/data/edx_use.json"
                 python3 "$P/sp_solve.py" "$ROOT/data/sp_solved.json"
                 python3 "$P/cdecl_args.py" "$FFXI_DLL" "$ROOT/data/cdecl_args.json"
                 python3 "$P/leaf_funcs.py" "$FFXI_DLL" "$ROOT/data/leaf_funcs.json"
                 python3 "$P/callsites.py" "$EXP" "$ROOT/data/callsite_args.json"
                 python3 "$P/callsite_audit.py" "$EXP" "$ROOT/data/callsite_audit.json"
                 python3 "$P/ida_sp_check.py" "$EXP" "$ROOT/data/ida_sp_audit.json"
                 python3 "$P/fcmp.py" --export "$EXP" "$ROOT/data/fcmp_exact.json"
                 python3 "$P/make_ida_types.py" "$ROOT/data/ida_classes.json" ;;&
  translate|all) rm -rf "$W/raw"; python3 "$P/hx2cpp.py" "$EXP" "$W/raw"; echo "$EXP $(sha1sum < "$EXP")" > "$W/raw/export.sha1"
                 "$P/build_raw.sh" "$W/raw" | tail -1
                 python3 "$P/repair_compile.py" "$W/raw" | tail -1; "$P/build_raw.sh" "$W/raw" | tail -1 ;;&
  cases|all)     same_export; python3 "$P/gfx_cases.py" "$EXP" "$W/raw/meta.json" "$ROOT/data/protos_fixed.json" "$W/cases_all.json"
                 python3 -c "import json,os;c=[x for x in json.load(open('$W/cases_all.json')) if os.path.exists('$W/raw/obj/fn_%s.o'%x['original'][2:])];json.dump(c,open('$W/cases.json','w'));print(len(c),'gateable')" ;;&
  gate|all)      gate2 "$W/raw/fndll" "$W/cases.json" "$W/gate_raw" ;;&      # one DLL per function (build_raw.sh)
  clean|all)     same_export; "$P/regen_clean.sh" "$EXP" "$GIN" "$W/tree" | tee "$W/tree.log"
                 grep -q 'failed: none$' "$W/tree.log" || { echo "run.sh: the clean tree does not build (runs/$RUN/tree/build/err)" >&2; exit 1; } ;;&
  cleangate|all) python3 "$P/map_clean_cases.py" "$W/tree/build/gfx_recon.dll" "$W/tree/placement.json" "$W/cases.json" "$W/cases_clean_all.json" | tail -1
                 python3 -c "import json;g={json.loads(l)['original'] for l in open('$GIN') if json.loads(l)['verdict']=='PASS'};c=[x for x in json.load(open('$W/cases_clean_all.json')) if x['original'] in g];json.dump(c,open('$W/cases_clean.json','w'))"
                 gate2 "$W/tree/build/gfx_recon.dll" "$W/cases_clean.json" "$W/gate_clean"
                 python3 "$P/annotate_gate.py" "$W/tree" "$W/gate_clean.jsonl" ;;&
  rawlive|cleanlive)
                 # LIVE="name=live.json ..." (live_state.py, one per verified dump): the raw translation (rawlive) or
                 # the clean tree as built (cleanlive) gated on each dump's real state, then combined with the fuzz
                 # gate (combine_gates.py: a FAIL anywhere is a FAIL) into gate_combined / gate_clean_combined
                 same_export; K=${1%live}; D=raw/fndll; CS=cases.json; B=gate_raw
                 [ "$K" = clean ] && { D=tree/build/gfx_recon.dll; CS=cases_clean.json; B=gate_clean; }
                 L=()
                 for x in ${LIVE:?set LIVE=\"name=live.json ...\"}; do
                   rm -f "$W/${B}_live_${x%%=*}.jsonl"
                   FFXI_LIVE="${x#*=}" gate2 "$W/$D" "$W/$CS" "$W/${B}_live_${x%%=*}"; L+=("${x%%=*}=$W/${B}_live_${x%%=*}.jsonl")
                 done
                 python3 "$P/combine_gates.py" "$W/$B.jsonl" "$W/${B%_raw}_combined.jsonl" "${L[@]}"
                 [ "$K" = clean ] && python3 "$P/annotate_gate.py" "$W/tree" "$W/gate_clean_combined.jsonl" ;;
  residue|all)   same_export; (cd "$W" && python3 "$P/gen_residue.py" "$EXP" "$W/raw/meta.json" "$GIN" "$ROOT/verification/localize_fail.json" "$W/no_byte_returns.json" "$CLN" "$W/RESIDUE.md") ;;
  *) ;;
esac
